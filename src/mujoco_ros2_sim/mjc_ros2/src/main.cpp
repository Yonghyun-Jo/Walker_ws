#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <thread>

#include <mujoco/mujoco.h>
#include "glfw_adapter.h"
#include "simulate.h"
#include "array_safety.h"

#define MUJOCO_PLUGIN_DIR "mujoco_plugin"

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <atomic>
#include <GLFW/glfw3.h>

#include "mjc_ros2/mjc2ros.h"

namespace {
namespace mj = ::mujoco;
namespace mju = ::mujoco::sample_util;

// Global shutdown flag
std::atomic<bool> g_shutdown_requested(false);
mj::Simulate* g_sim_ptr = nullptr;

// constants
const double syncMisalign = 0.1;        // maximum mis-alignment before re-sync (simulation seconds)
const double simRefreshFraction = 0.7;  // fraction of refresh available for simulation
const int kErrorLength = 1024;          // load error string length

// model and data
mjModel* m = nullptr;
mjData* d = nullptr;

std::vector<std::string> joint_names;

using Seconds = std::chrono::duration<double>;

// Control callback function - called every simulation step
void controlCallback(const mjModel* m, mjData* d) {
    // Check if simulation is paused - if paused, don't execute control logic
    if (g_sim_ptr && !g_sim_ptr->run) {
        return;
    }
    // Detect reset: if simulation time goes backwards, reset counter
    if (d->time < prev_sim_time) {
        cnt = 0;
        std::printf("[MuJoCo] Simulation reset detected. Counter reset to 0.\n");
    }
    prev_sim_time = d->time;
    ////////////////////////////////////////////
    // read data from mujoco
    ////////////////////////////////////////////

    // mjc time
    joint_status.ecat_cnt = cnt++;
    // std_msgs::msg::Float32 sim_time_msg;
    // sim_time_msg.data = d->time;
    // sim_time_pub->publish(sim_time_msg);

    for (auto it = joint_names.begin(); it != joint_names.end(); ++it)
    {
        const std::string& joint_name = *it;

        int joint_id = mj_name2id(m, mjOBJ_JOINT, joint_name.c_str());
        if (joint_id < 0)
        {
            std::printf("[WARN] joint '%s' not found in model\n", joint_name.c_str());
            continue;
        }

        int qpos_adr = m->jnt_qposadr[joint_id];
        int qvel_adr = m->jnt_dofadr[joint_id];

        size_t idx = std::distance(joint_names.begin(), it);

        joint_status.position_external[idx] = d->qpos[qpos_adr];
        joint_status.velocity[idx]          = d->qvel[qvel_adr];
        joint_status.torque[idx]            = d->qacc[qvel_adr];
    }

    // publish virtual state to ros
    std_msgs::msg::Float64MultiArray virtual_state_msgs;
    virtual_state_msgs.data.resize(6);
    std::copy(d->qpos, d->qpos + 3, virtual_state_msgs.data.begin());
    std::copy(d->qvel, d->qvel + 3, virtual_state_msgs.data.begin() + 3);
    virtual_state_pub->publish(virtual_state_msgs);

    // TODO: this is for future use when shm has been implemented
    // std::copy(d->qpos, d->qpos + 3, joint_status.virtual_position);
    // std::copy(d->qvel, d->qvel + 3, joint_status.virtual_velocity);

    // imu datas by mjc data
    // std::copy(d->qpos + 3, d->qpos + 3 + 4, imu_status.orientation); //wxyz
    // std::copy(d->qvel + 3, d->qvel + 3 + 3, imu_status.angular_velocity);
    // std::copy(d->qacc + 3, d->qacc + 3 + 3, imu_status.linear_acceleration);
    // imu datas by mjc imu sensor data
    std::copy(d->sensordata + 0, d->sensordata + 4,  imu_status.orientation); //wxyz
    std::copy(d->sensordata + 4, d->sensordata + 7,  imu_status.angular_velocity);
    std::copy(d->sensordata + 7, d->sensordata + 10, imu_status.linear_acceleration);

    double age_sec = 0.0;
    while (spsc_joint_command_pop_with_age(joint_command_buffer, &joint_command, &age_sec))
    {
        // joint_command_received = true;

        // if (age_sec >= COMMAND_STALE_THRESHOLD_SEC)
        // {
        //     trigger_safety(SAFETY_REASON_COMMAND_STALE, -1, age_sec);
        // }
    }
    joint_status.system_status = ECAT_OPERATIONAL;
    if (joint_command.system_command == CONTROL_COMMAND) 
        joint_status.system_status = ECAT_CONTROL;
    spsc_joint_status_push(joint_status_buffer, &joint_status);
    spsc_imu_status_push(imu_status_buffer, &imu_status);

    // write data to mujoco
    std::copy(joint_command.target_torque, joint_command.target_torque + JOINT_NUM, d->ctrl);
}


//---------------------------------------- plugin handling -----------------------------------------
// return the path to the directory containing the current executable
// used to determine the location of auto-loaded plugin libraries
std::string getExecutableDir() {
#if defined(_WIN32) || defined(__CYGWIN__)
    constexpr char kPathSep = '\\';
    std::string realpath = [&]() -> std::string {
    std::unique_ptr<char[]> realpath(nullptr);
    DWORD buf_size = 128;
    bool success = false;
    while (!success) {
        realpath.reset(new(std::nothrow) char[buf_size]);
        if (!realpath) {
        std::cerr << "cannot allocate memory to store executable path\n";
        return "";
        }

        DWORD written = GetModuleFileNameA(nullptr, realpath.get(), buf_size);
        if (written < buf_size) {
        success = true;
        } else if (written == buf_size) {
        // realpath is too small, grow and retry
        buf_size *=2;
        } else {
        std::cerr << "failed to retrieve executable path: " << GetLastError() << "\n";
        return "";
        }
    }
    return realpath.get();
    }();
#else
    constexpr char kPathSep = '/';
#if defined(__APPLE__)
    std::unique_ptr<char[]> buf(nullptr);
    {
    std::uint32_t buf_size = 0;
    _NSGetExecutablePath(nullptr, &buf_size);
    buf.reset(new char[buf_size]);
    if (!buf) {
        std::cerr << "cannot allocate memory to store executable path\n";
        return "";
    }
    if (_NSGetExecutablePath(buf.get(), &buf_size)) {
        std::cerr << "unexpected error from _NSGetExecutablePath\n";
    }
    }
    const char* path = buf.get();
#else
    const char* path = "/proc/self/exe";
#endif
    std::string realpath = [&]() -> std::string {
    std::unique_ptr<char[]> realpath(nullptr);
    std::uint32_t buf_size = 128;
    bool success = false;
    while (!success) {
        realpath.reset(new(std::nothrow) char[buf_size]);
        if (!realpath) {
        std::cerr << "cannot allocate memory to store executable path\n";
        return "";
        }

        std::size_t written = readlink(path, realpath.get(), buf_size);
        if (written < buf_size) {
        realpath.get()[written] = '\0';
        success = true;
        } else if (written == -1) {
        if (errno == EINVAL) {
            // path is already not a symlink, just use it
            return path;
        }

        std::cerr << "error while resolving executable path: " << strerror(errno) << '\n';
        return "";
        } else {
        // realpath is too small, grow and retry
        buf_size *= 2;
        }
    }
    return realpath.get();
    }();
#endif

    if (realpath.empty()) {
    return "";
    }

    for (std::size_t i = realpath.size() - 1; i > 0; --i) {
    if (realpath.c_str()[i] == kPathSep) {
        return realpath.substr(0, i);
    }
    }

    // don't scan through the entire file system's root
    return "";
}



// scan for libraries in the plugin directory to load additional plugins
void scanPluginLibraries() {
    // check and print plugins that are linked directly into the executable
    int nplugin = mjp_pluginCount();
    if (nplugin) {
    std::printf("Built-in plugins:\n");
    for (int i = 0; i < nplugin; ++i) {
        std::printf("    %s\n", mjp_getPluginAtSlot(i)->name);
    }
    }

    // define platform-specific strings
#if defined(_WIN32) || defined(__CYGWIN__)
    const std::string sep = "\\";
#else
    const std::string sep = "/";
#endif


    // try to open the ${EXECDIR}/MUJOCO_PLUGIN_DIR directory
    // ${EXECDIR} is the directory containing the simulate binary itself
    // MUJOCO_PLUGIN_DIR is the MUJOCO_PLUGIN_DIR preprocessor macro
    const std::string executable_dir = getExecutableDir();
    if (executable_dir.empty()) {
    return;
    }

    const std::string plugin_dir = getExecutableDir() + sep + MUJOCO_PLUGIN_DIR;
    mj_loadAllPluginLibraries(
        plugin_dir.c_str(), +[](const char* filename, int first, int count) {
        std::printf("Plugins registered by library '%s':\n", filename);
        for (int i = first; i < first + count; ++i) {
            std::printf("    %s\n", mjp_getPluginAtSlot(i)->name);
        }
        });
}


//------------------------------------------- simulation -------------------------------------------

const char* Diverged(int disableflags, const mjData* d) {
    if (disableflags & mjDSBL_AUTORESET) {
    for (mjtWarning w : {mjWARN_BADQACC, mjWARN_BADQVEL, mjWARN_BADQPOS}) {
        if (d->warning[w].number > 0) {
        return mju_warningText(w, d->warning[w].lastinfo);
        }
    }
    }
    return nullptr;
}

mjModel* LoadModel(const char* file, mj::Simulate& sim) {
    // this copy is needed so that the mju::strlen call below compiles
    char filename[mj::Simulate::kMaxFilenameLength];
    mju::strcpy_arr(filename, file);

    // make sure filename is not empty
    if (!filename[0]) {
    return nullptr;
    }

    // load and compile
    char loadError[kErrorLength] = "";
    mjModel* mnew = 0;
    auto load_start = mj::Simulate::Clock::now();

    std::string filename_str(filename);
    std::string extension;
    size_t dot_pos = filename_str.rfind('.');

    if (dot_pos != std::string::npos && dot_pos < filename_str.length() - 1) {
    extension = filename_str.substr(dot_pos);
    }

    if (extension == ".mjb") {
    mnew = mj_loadModel(filename, nullptr);
    if (!mnew) {
        mju::strcpy_arr(loadError, "could not load binary model");
    }
#if defined(mjUSEUSD)
    } else if (extension == ".usda" || extension == ".usd" ||
                extension == ".usdc" || extension == ".usdz" ) {
    mnew = mj_loadUSD(filename, nullptr, loadError, kErrorLength);
#endif
    } else {
    mnew = mj_loadXML(filename, nullptr, loadError, kErrorLength);

    // remove trailing newline character from loadError
    if (loadError[0]) {
        int error_length = mju::strlen_arr(loadError);
        if (loadError[error_length-1] == '\n') {
        loadError[error_length-1] = '\0';
        }
    }
    }
    auto load_interval = mj::Simulate::Clock::now() - load_start;
    double load_seconds = Seconds(load_interval).count();

    if (!mnew) {
    std::printf("%s\n", loadError);
    mju::strcpy_arr(sim.load_error, loadError);
    return nullptr;
    }

    // compiler warning: print and pause
    if (loadError[0]) {
    // mj_forward() below will print the warning message
    std::printf("Model compiled, but simulation warning (paused):\n  %s\n", loadError);
    sim.run = 0;
    }

    // if no error and load took more than 1/4 seconds, report load time
    else if (load_seconds > 0.25) {
    mju::sprintf_arr(loadError, "Model loaded in %.2g seconds", load_seconds);
    }

    mju::strcpy_arr(sim.load_error, loadError);

    return mnew;
}

// simulate in background thread (while rendering in main thread)
void PhysicsLoop(mj::Simulate& sim) {
    // cpu-sim synchronization point
    std::chrono::time_point<mj::Simulate::Clock> syncCPU;
    mjtNum syncSim = 0;

    // run until asked to exit
    while (!sim.exitrequest.load() && !g_shutdown_requested.load() && rclcpp::ok()) {
    if (sim.droploadrequest.load()) {
        sim.LoadMessage(sim.dropfilename);
        mjModel* mnew = LoadModel(sim.dropfilename, sim);
        sim.droploadrequest.store(false);

        mjData* dnew = nullptr;
        if (mnew) dnew = mj_makeData(mnew);
        if (dnew) {
        sim.Load(mnew, dnew, sim.dropfilename);

        // lock the sim mutex
        const std::unique_lock<std::recursive_mutex> lock(sim.mtx);

        mj_deleteData(d);
        mj_deleteModel(m);

        m = mnew;
        d = dnew;
        
        // Load keyframe if available (qpos only)
        if (m->nkey > 0) {
            int key_index = mj_name2id(m, mjOBJ_KEY, "front");
            if (key_index < 0) key_index = 0;
            mju_copy(d->qpos, m->key_qpos + key_index*m->nq, m->nq);
            std::printf("Loaded qpos from keyframe (drop-reload)\n");
        }
        
        // Set geom group visualization: only show groups 1 and 3
        for (int i = 0; i < mjNGROUP; i++) {
            sim.opt.geomgroup[i] = 0;
        }
        sim.opt.geomgroup[1] = 1;
        sim.opt.geomgroup[3] = 1;
        
        mj_forward(m, d);

        } else {
        sim.LoadMessageClear();
        }
    }

    if (sim.uiloadrequest.load()) {
        sim.uiloadrequest.fetch_sub(1);
        sim.LoadMessage(sim.filename);
        mjModel* mnew = LoadModel(sim.filename, sim);
        mjData* dnew = nullptr;
        if (mnew) dnew = mj_makeData(mnew);
        if (dnew) {
        sim.Load(mnew, dnew, sim.filename);

        // lock the sim mutex
        const std::unique_lock<std::recursive_mutex> lock(sim.mtx);

        mj_deleteData(d);
        mj_deleteModel(m);

        m = mnew;
        d = dnew;
        
        // Load keyframe if available (qpos only)
        if (m->nkey > 0) {
            int key_index = mj_name2id(m, mjOBJ_KEY, "front");
            if (key_index < 0) key_index = 0;
            mju_copy(d->qpos, m->key_qpos + key_index*m->nq, m->nq);
            std::printf("Loaded qpos from keyframe (UI-reload)\n");
        }
        
        // Set geom group visualization: only show groups 1 and 3
        for (int i = 0; i < mjNGROUP; i++) {
            sim.opt.geomgroup[i] = 0;
        }
        sim.opt.geomgroup[1] = 1;
        sim.opt.geomgroup[3] = 1;
        
        mj_forward(m, d);

        } else {
        sim.LoadMessageClear();
        }
    }

    // sleep for 1 ms or yield, to let main thread run
    //  yield results in busy wait - which has better timing but kills battery life
    if (sim.run && sim.busywait) {
        std::this_thread::yield();
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    {
        // lock the sim mutex
        const std::unique_lock<std::recursive_mutex> lock(sim.mtx);

        // run only if model is present
        if (m) {
        // running
        if (sim.run) {
            bool stepped = false;

            // record cpu time at start of iteration
            const auto startCPU = mj::Simulate::Clock::now();

            // elapsed CPU and simulation time since last sync
            const auto elapsedCPU = startCPU - syncCPU;
            double elapsedSim = d->time - syncSim;

            // requested slow-down factor
            double slowdown = 100 / sim.percentRealTime[sim.real_time_index];

            // misalignment condition: distance from target sim time is bigger than syncMisalign
            bool misaligned =
                std::abs(Seconds(elapsedCPU).count()/slowdown - elapsedSim) > syncMisalign;

            // out-of-sync (for any reason): reset sync times, step
            if (elapsedSim < 0 || elapsedCPU.count() < 0 || syncCPU.time_since_epoch().count() == 0 ||
                misaligned || sim.speed_changed) {
            // re-sync
            syncCPU = startCPU;
            syncSim = d->time;
            sim.speed_changed = false;

            // inject noise
            sim.InjectNoise(sim.key);

            // run single step, let next iteration deal with timing
            mj_step(m, d);
            const char* message = Diverged(m->opt.disableflags, d);
            if (message) {
                sim.run = 0;
                mju::strcpy_arr(sim.load_error, message);
            } else {
                stepped = true;
            }
            }

            // in-sync: step until ahead of cpu
            else {
            bool measured = false;
            mjtNum prevSim = d->time;

            double refreshTime = simRefreshFraction/sim.refresh_rate;

            // step while sim lags behind cpu and within refreshTime
            while (Seconds((d->time - syncSim)*slowdown) < mj::Simulate::Clock::now() - syncCPU &&
                    mj::Simulate::Clock::now() - startCPU < Seconds(refreshTime)) {
                // measure slowdown before first step
                if (!measured && elapsedSim) {
                sim.measured_slowdown =
                    std::chrono::duration<double>(elapsedCPU).count() / elapsedSim;
                measured = true;
                }

                // inject noise
                sim.InjectNoise(sim.key);

                // call mj_step
                mj_step(m, d);
                const char* message = Diverged(m->opt.disableflags, d);
                if (message) {
                sim.run = 0;
                mju::strcpy_arr(sim.load_error, message);
                } else {
                stepped = true;
                }

                // break if reset
                if (d->time < prevSim) {
                break;
                }
            }
            }

            // save current state to history buffer
            if (stepped) {
            sim.AddToHistory();
            }
        }

        // paused
        else {
            // run mj_forward, to update rendering and joint sliders
            mj_forward(m, d);
            if (sim.pause_update) {
            mju_copy(d->qacc_warmstart, d->qacc, m->nv);
            }
            sim.speed_changed = true;
        }
        }
    }  // release std::lock_guard<std::mutex>
    }
}

//-------------------------------------- physics_thread --------------------------------------------

void PhysicsThread(mj::Simulate* sim, const char* filename) {
    // request loadmodel if file given (otherwise drag-and-drop)
    if (filename != nullptr) {
      sim->LoadMessage(filename);
      m = LoadModel(filename, *sim);
      if (m) {
        // lock the sim mutex
        const std::unique_lock<std::recursive_mutex> lock(sim->mtx);
  
        d = mj_makeData(m);
      }
      if (d) {
        sim->Load(m, d, filename);
  
        // lock the sim mutex
        const std::unique_lock<std::recursive_mutex> lock(sim->mtx);
  
        // Load keyframe if available (qpos only)
        if (m->nkey > 0) {
            int key_index = mj_name2id(m, mjOBJ_KEY, "front");
            if (key_index < 0) key_index = 0;  // fallback
        
            // qpos만 복사
            mju_copy(d->qpos, m->key_qpos + key_index*m->nq, m->nq);
        
            std::printf("Loaded qpos from keyframe index: %d (time=%f)\n",
                        key_index, m->key_time[key_index]);
        }
  
        mj_forward(m, d);
        
        // Set geom group visualization: only show groups 1 and 3
        for (int i = 0; i < mjNGROUP; i++) {
            sim->opt.geomgroup[i] = 0;  // Hide all groups first
        }
        sim->opt.geomgroup[1] = 1;  // Show group 1
        sim->opt.geomgroup[3] = 1;  // Show group 3
        std::printf("Geom group visualization set to groups 1 and 3 only.\n");
        
        // Start simulation in paused state
        sim->run = 0;
        std::printf("Simulation started in paused state. Press Space to start.\n");
  
      } else {
        sim->LoadMessageClear();
      }
    }
  
    PhysicsLoop(*sim);
  
    // delete everything we allocated
    mj_deleteData(d);
    mj_deleteModel(m);
  }

}  // namespace

// Signal handler for Ctrl+C
void signalHandler(int signum) {
    std::printf("\n[MuJoCo] Shutdown signal received (Ctrl+C). Exiting...\n");
    (void)signum;
    
    // Request ROS2 shutdown
    rclcpp::shutdown();
    
    // Request simulation exit
    g_shutdown_requested.store(true);
    if (g_sim_ptr) {
        g_sim_ptr->exitrequest.store(true);
    }
    
    // Wake up GLFW event loop
    glfwPostEmptyEvent();
}

int main(int argc, char** argv) {

    // Register signal handler BEFORE rclcpp::init
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    rclcpp::init(argc, argv);
    auto nh = std::make_shared<rclcpp::Node>("mujoco_ros2");

    // Declare parameters
    nh->declare_parameter<std::string>("model_file", "");
    nh->declare_parameter<std::vector<std::string>>("joint_names", std::vector<std::string>());
    nh->get_parameter("joint_names", joint_names);

    // Declare ros Publisher
    // sim_time_pub = nh->create_publisher<std_msgs::msg::Float32>("/p73/mjcSimTime", 10);
    virtual_state_pub = nh->create_publisher<std_msgs::msg::Float64MultiArray>("/p73/mjcVirtualState", 10);

    std::printf("MuJoCo version %s\n", mj_versionString());
    if (mjVERSION_HEADER!=mj_version()) {
      mju_error("Headers and library have different versions");
    }

    scanPluginLibraries();
    mjvCamera cam;
    mjv_defaultCamera(&cam);
    mjvOption opt;
    mjv_defaultOption(&opt);
    mjvPerturb pert;
    mjv_defaultPerturb(&pert);
  
    // simulate object encapsulates the UI
    auto sim = std::make_unique<mj::Simulate>(
        std::make_unique<mj::GlfwAdapter>(),
        &cam, &opt, &pert, /* is_passive = */ false
    );
    
    // Set global pointer for signal handler
    g_sim_ptr = sim.get();

    // Load model if parameter is provided
    std::string model_file;
    const char* model_file_cstr = nullptr;
    if (nh->get_parameter("model_file", model_file))
    {
        RCLCPP_INFO(nh->get_logger(), "Model is at %s", model_file.c_str());
        model_file_cstr = model_file.c_str();
    }
  
    ////////////////////////////////////////////
    // ETHERCAT Shared Memory Initialization
    ////////////////////////////////////////////
    ecat_shm_t *mjc_shm = ecat_shm_get();
    if (!mjc_shm)
    {
        printf("Failed to get shared memory\n");
        return -1;
    }

    joint_status_buffer = &mjc_shm->joint_status_buffer;
    joint_command_buffer = &mjc_shm->joint_command_buffer;

    joint_status.system_status = ECAT_OPERATIONAL;
    for (int joint = 0; joint < JOINT_NUM; ++joint)
    {
        joint_status.control_status[joint] = ECAT_OPERATIONAL;
    }
    joint_command.system_command = CONTROL_COMMAND_IDLE;
    joint_command.timestamp = ecat_monotonic_now();

    spsc_joint_command_init(joint_command_buffer);
    spsc_joint_status_init(joint_status_buffer);

    ////////////////////////////////////////////
    // IMU Shared Memory Initialization
    ////////////////////////////////////////////
    imu_shm_t *imu_shm = imu_shm_get();
    if (!imu_shm)
    {
        printf("Failed to get shared memory\n");
        return -1;
    }

    imu_status_buffer = &imu_shm->imu_status_buffer;
    spsc_imu_status_init(imu_status_buffer);

    // Register MJC control callback
    mjcb_control = controlCallback;
    std::printf("Control callback registered for shared memory updates\n");

    // start physics thread
    std::thread physicsthreadhandle(&PhysicsThread, sim.get(), model_file_cstr);
  
    // start simulation UI loop (blocking call)
    sim->RenderLoop();
    physicsthreadhandle.join();
    rclcpp::shutdown();
    
    ////////////////////////////////////////////
    // Cleanup
    ////////////////////////////////////////////
    g_sim_ptr = nullptr;
    mjcb_control = nullptr;
    ecat_shm_detach(mjc_shm);
    imu_shm_detach(imu_shm);
    mjc_shm = nullptr;
    imu_shm = nullptr;
    ecat_shm_destroy();
    imu_shm_destroy();
    
    std::printf("[MuJoCo] Cleanup completed. Exiting.\n");
    
    return 0;
}