/**
 * main_headless.cpp — MuJoCo headless simulation (no GLFW, no GUI)
 *
 * 물리 시뮬레이션 + SHM (p73_controller 연동) + ROS2 topic publish만 수행.
 * mujoco_eval.py에서 자동화 평가용으로 사용.
 *
 * 기존 main.cpp의 controlCallback을 그대로 사용 (mjc2ros.h 공유).
 * GLFW/simulate.cc는 링크하지 않음.
 */

#include <chrono>
#include <cstdio>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <atomic>
#include <signal.h>

#include <mujoco/mujoco.h>

#include "mjc_ros2/mjc2ros.h"

// Shutdown flag
static std::atomic<bool> g_shutdown(false);

void signalHandler(int) { g_shutdown.store(true); }

// Global model/data
static mjModel* m = nullptr;
static mjData* d = nullptr;

// Joint names (populated from ROS2 parameter)
static std::vector<std::string> joint_names;

// controlCallback: reads joint state → SHM, writes torque → MuJoCo
// Reuse the exact same callback from main.cpp via the header.
// We need to define it here since main_headless links separately.
void controlCallback(const mjModel* m, mjData* d) {
    if (d->time < prev_sim_time) {
        cnt = 0;
    }
    prev_sim_time = d->time;

    joint_status.ecat_cnt = cnt++;

    // Read joint states from MuJoCo
    for (size_t i = 0; i < joint_names.size(); ++i) {
        int jid = mj_name2id(m, mjOBJ_JOINT, joint_names[i].c_str());
        if (jid < 0) continue;
        int qa = m->jnt_qposadr[jid];
        int va = m->jnt_dofadr[jid];
        joint_status.position_external[i] = d->qpos[qa];
        joint_status.velocity[i] = d->qvel[va];
        joint_status.torque[i] = d->qacc[va];
    }

    // Publish virtual state (base pos + vel)
    std_msgs::msg::Float64MultiArray vs_msg;
    vs_msg.data.resize(6);
    std::copy(d->qpos, d->qpos + 3, vs_msg.data.begin());
    std::copy(d->qvel, d->qvel + 3, vs_msg.data.begin() + 3);
    virtual_state_pub->publish(vs_msg);

    // Publish full state (qpos + sim_time at the end) for recorder
    if (full_state_pub) {
        std_msgs::msg::Float64MultiArray fs_msg;
        fs_msg.data.assign(d->qpos, d->qpos + m->nq);
        fs_msg.data.push_back(d->time);  // last element = sim_time
        full_state_pub->publish(fs_msg);
    }

    // IMU sensor data
    std::copy(d->sensordata + 0, d->sensordata + 4, imu_status.orientation);
    std::copy(d->sensordata + 4, d->sensordata + 7, imu_status.angular_velocity);
    std::copy(d->sensordata + 7, d->sensordata + 10, imu_status.linear_acceleration);

    // SHM: read command, push status
    double age_sec = 0.0;
    while (spsc_joint_command_pop_with_age(joint_command_buffer, &joint_command, &age_sec)) {}
    joint_status.system_status = ECAT_OPERATIONAL;
    if (joint_command.system_command == CONTROL_COMMAND)
        joint_status.system_status = ECAT_CONTROL;
    spsc_joint_status_push(joint_status_buffer, &joint_status);
    spsc_imu_status_push(imu_status_buffer, &imu_status);

    // Write torque to MuJoCo
    std::copy(joint_command.target_torque, joint_command.target_torque + JOINT_NUM, d->ctrl);

    // Ghost robot (if present)
    {
        static int ghost_jid = -2;
        static int ghost_qadr = -1, ghost_vadr = -1;
        if (ghost_jid == -2) {
            ghost_jid = mj_name2id(m, mjOBJ_JOINT, "ghost_world_to_base");
            if (ghost_jid >= 0) {
                ghost_qadr = m->jnt_qposadr[ghost_jid];
                ghost_vadr = m->jnt_dofadr[ghost_jid];
            }
        }
        if (ghost_jid >= 0) {
            std::lock_guard<std::mutex> lock(ghost_mutex);
            std::copy(ghost_qpos, ghost_qpos + 20, d->qpos + ghost_qadr);
            std::fill(d->qvel + ghost_vadr, d->qvel + ghost_vadr + 19, 0.0);
        }
    }
}


int main(int argc, char** argv) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    rclcpp::init(argc, argv);
    auto nh = std::make_shared<rclcpp::Node>("mujoco_ros2");

    // Parameters
    nh->declare_parameter<std::string>("model_file", "");
    nh->declare_parameter<std::vector<std::string>>("joint_names", std::vector<std::string>());
    nh->declare_parameter<double>("auto_shutdown_duration", 0.0);
    nh->get_parameter("joint_names", joint_names);
    double shutdown_dur = 0.0;
    nh->get_parameter("auto_shutdown_duration", shutdown_dur);

    std::string model_file;
    nh->get_parameter("model_file", model_file);

    if (model_file.empty()) {
        std::fprintf(stderr, "No model_file parameter\n");
        return 1;
    }
    std::printf("[Headless] Model: %s\n", model_file.c_str());
    if (shutdown_dur > 0)
        std::printf("[Headless] Auto-shutdown after %.1f sim seconds\n", shutdown_dur);

    // ROS2 publishers
    virtual_state_pub = nh->create_publisher<std_msgs::msg::Float64MultiArray>("/p73/mjcVirtualState", 10);
    full_state_pub = nh->create_publisher<std_msgs::msg::Float64MultiArray>("/mujoco/full_state", 10);

    // Ghost subscriber (optional)
    ghost_state_sub = nh->create_subscription<std_msgs::msg::Float64MultiArray>(
        "/p73/ghost_state", 10,
        [](const std_msgs::msg::Float64MultiArray::SharedPtr msg) {
            if (msg->data.size() >= 20) {
                std::lock_guard<std::mutex> lock(ghost_mutex);
                for (int i = 0; i < 20; i++)
                    ghost_qpos[i] = msg->data[i];
            }
        });

    // SHM initialization
    ecat_shm_t* mjc_shm = ecat_shm_get();
    if (!mjc_shm) { std::fprintf(stderr, "Failed to get SHM\n"); return 1; }
    joint_status_buffer = &mjc_shm->joint_status_buffer;
    joint_command_buffer = &mjc_shm->joint_command_buffer;
    joint_status.system_status = ECAT_OPERATIONAL;
    for (int j = 0; j < JOINT_NUM; ++j)
        joint_status.control_status[j] = ECAT_OPERATIONAL;
    joint_command.system_command = CONTROL_COMMAND_IDLE;
    joint_command.timestamp = ecat_monotonic_now();
    spsc_joint_command_init(joint_command_buffer);
    spsc_joint_status_init(joint_status_buffer);

    imu_shm_t* imu_shm = imu_shm_get();
    if (!imu_shm) { std::fprintf(stderr, "Failed to get IMU SHM\n"); return 1; }
    imu_status_buffer = &imu_shm->imu_status_buffer;
    spsc_imu_status_init(imu_status_buffer);

    // Load model
    char error[1024] = "";
    m = mj_loadXML(model_file.c_str(), nullptr, error, sizeof(error));
    if (!m) { std::fprintf(stderr, "Load failed: %s\n", error); return 1; }
    d = mj_makeData(m);

    // Load keyframe (same as main.cpp)
    if (m->nkey > 0) {
        int key_idx = mj_name2id(m, mjOBJ_KEY, "front");
        if (key_idx < 0) key_idx = 0;
        mju_copy(d->qpos, m->key_qpos + key_idx * m->nq, m->nq);
        std::printf("[Headless] Loaded keyframe %d\n", key_idx);
    }
    mj_forward(m, d);

    // Register control callback
    mjcb_control = controlCallback;
    std::printf("[Headless] Control callback registered\n");

    // ROS2 spin thread
    std::thread ros_thread([nh]() { rclcpp::spin(nh); });

    // ── Warm-up: run mj_forward (not mj_step) to let p73_controller initialize ──
    // This mirrors GUI mode where robot sits paused while user clicks initYaw/stateEstimate
    nh->declare_parameter<double>("warmup_seconds", 3.0);
    double warmup_sec = 0.0;
    nh->get_parameter("warmup_seconds", warmup_sec);
    std::printf("[Headless] Warm-up %.1fs (p73_controller init, send initYaw/stateEstimate now)...\n", warmup_sec);
    {
        auto warmup_start = std::chrono::steady_clock::now();
        while (!g_shutdown.load() && rclcpp::ok()) {
            mj_forward(m, d);  // update sensors without stepping physics
            // Trigger controlCallback manually (SHM exchange)
            if (mjcb_control) mjcb_control(m, d);
            auto elapsed = std::chrono::duration<double>(
                std::chrono::steady_clock::now() - warmup_start).count();
            if (elapsed >= warmup_sec) break;
            std::this_thread::sleep_for(std::chrono::duration<double>(m->opt.timestep));
        }
    }
    std::printf("[Headless] Warm-up done. Starting physics...\n");

    // ── Physics loop (real-time, matching simulation.launch.py behavior) ──
    std::printf("[Headless] Simulation running...\n");
    const double sim_dt = m->opt.timestep;

    while (!g_shutdown.load() && rclcpp::ok()) {
        // Auto-shutdown
        if (shutdown_dur > 0 && d->time >= shutdown_dur) {
            std::printf("[Headless] Auto-shutdown: sim time %.1f >= %.1f\n", d->time, shutdown_dur);
            break;
        }

        // Step physics (real-time pacing)
        static int print_cnt = 0;
        if (++print_cnt % 1000 == 0)
            std::printf("[Headless] sim_time=%.2f\n", d->time);

        auto step_start = std::chrono::steady_clock::now();
        mj_step(m, d);
        auto step_end = std::chrono::steady_clock::now();

        // Sleep to maintain real-time (sim_dt minus step computation time)
        double step_elapsed = std::chrono::duration<double>(step_end - step_start).count();
        double sleep_time = sim_dt - step_elapsed;
        if (sleep_time > 0) {
            std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
        }
    }

    std::printf("[Headless] Shutting down (sim time: %.1f)\n", d->time);

    mj_deleteData(d);
    mj_deleteModel(m);
    rclcpp::shutdown();
    if (ros_thread.joinable()) ros_thread.join();

    return 0;
}
