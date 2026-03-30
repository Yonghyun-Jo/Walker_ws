#include "cc.h"
#include <cmath>
#include <iomanip>
#include <numeric>

// =====================================================================
// Joint order permutation helpers
// =====================================================================
namespace {
using Vec12 = Eigen::Matrix<double, 12, 1>;

// P73 lower 12 joints → IsaacLab order
inline Vec12 p73_to_isaac(const Eigen::VectorXd &p73_13, const std::array<int, 12> &map) {
    Vec12 isaac;
    for (int i = 0; i < 12; ++i)
        isaac(map[i]) = p73_13(i);
    return isaac;
}

// IsaacLab 12 actions → P73 lower 12 indices
inline void isaac_to_p73(const Vec12 &isaac, Eigen::VectorXd &p73_13, const std::array<int, 12> &map) {
    for (int i = 0; i < 12; ++i)
        p73_13(map[i]) = isaac(i);
}
}  // namespace

// =====================================================================
// Constructor
// =====================================================================
CustomController::CustomController(DataContainer &dc, RobotEigenData &rd)
    :   dc_(dc), rd_(rd),
        env(ORT_LOGGING_LEVEL_WARNING, "p73_walker_cc"),
        memory_info(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault)),
        session(nullptr)
{
    weight_dir_ = std::string(getenv("HOME")) + "/Walker_ws/src/p73_walker_cc/policy/policy.onnx";

    if (is_write_file_) {
        writeFile.open("/tmp/p73_walker_cc_data.csv", ofstream::out);
        writeFile << fixed << setprecision(8);
    }

    loadOnnX();
    initVariable();
    startVelSubscriber();
}

// =====================================================================
// initVariable
// =====================================================================
void CustomController::initVariable()
{
    cout << "[p73_walker_cc] Initializing variables" << endl;

    // --- Default joint positions in P73 order (from p73_walker.py asset config) ---
    // P73:  L_HipYaw, L_HipRoll, L_HipPitch, L_Knee, L_AnklePitch, L_AnkleRoll,
    //       R_HipYaw, R_HipRoll, R_HipPitch, R_Knee, R_AnklePitch, R_AnkleRoll,
    //       WaistYaw
    q_default_p73_ <<  0.0,   0.0,  0.36,  0.77, -0.41, 0.0,    // L leg
                       0.0,   0.0, -0.36, -0.77,  0.41, 0.0,    // R leg
                       0.0;                                       // WaistYaw

    // --- Default joint positions in IsaacLab lower body order (12) ---
    // Isaac: L_HipRoll, L_HipPitch, L_HipYaw, L_Knee, L_AnklePitch, L_AnkleRoll,
    //        R_HipRoll, R_HipPitch, R_HipYaw, R_Knee, R_AnklePitch, R_AnkleRoll
    q_default_isaac_ <<  0.0,  0.36, 0.0,  0.77, -0.41, 0.0,    // L leg
                         0.0, -0.36, 0.0, -0.77,  0.41, 0.0;    // R leg

    // --- PD gains in P73 order (from ActionsCfg, permuted to P73 order) ---
    // ActionsCfg p_gains (IsaacLab order): [1536, 937.5, 625, 747.552, 490.644, 490.104,
    //                                       1536, 937.5, 625, 747.552, 490.644, 490.104, 576]
    kp_p73_ << 625.0, 1536.0, 937.5, 747.552, 490.644, 490.104,   // L leg (P73 order)
               625.0, 1536.0, 937.5, 747.552, 490.644, 490.104,   // R leg (P73 order)
               576.0;                                               // WaistYaw

    // ActionsCfg d_gains (IsaacLab order): [76.8, 37.5, 12.5, 37.378, 16.355, 16.337,
    //                                       76.8, 37.5, 12.5, 37.378, 16.355, 16.337, 19.2]
    kd_p73_ << 12.5, 76.8, 37.5, 37.378, 16.355, 16.337,   // L leg (P73 order)
               12.5, 76.8, 37.5, 37.378, 16.355, 16.337,    // R leg (P73 order)
               19.2;                                          // WaistYaw

    // --- Torque limits in P73 order (N*m) ---
    // ActionsCfg: [352, 220, 95, 220, 95, 95, 352, 220, 95, 220, 95, 95, 152]
    torque_bound_p73_ << 95.0, 352.0, 220.0, 220.0, 95.0, 95.0,   // L leg (P73 order)
                         95.0, 352.0, 220.0, 220.0, 95.0, 95.0,    // R leg (P73 order)
                         152.0;                                      // WaistYaw

    // --- Joint position limits in P73 order (for q_des clamping) ---
    // IsaacLab clamps: q_des = clamp(q_des, lower_lim, upper_lim)
    // Isaac order: [(-0.58,0.3), (-1.57,2.09), (-0.78,0.78), (0.0,2.56), (-1.05,0.7), (-0.42,0.42),
    //              (-0.58,0.3), (-2.09,1.57), (-0.78,0.78), (-2.56,0.0), (-0.7,1.05), (-0.42,0.42)]
    // Permuted to P73 order:
    q_limit_lower_p73_ << -0.78, -0.58, -1.57,  0.0,  -1.05, -0.42,   // L leg
                          -0.78, -0.58, -2.09, -2.56, -0.7,  -0.42;    // R leg
    q_limit_upper_p73_ <<  0.78,  0.3,   2.09,  2.56,  0.7,   0.42,   // L leg
                           0.78,  0.3,   1.57,  0.0,   1.05,  0.42;    // R leg

    // --- Buffers ---
    rl_action_.setZero();
    last_action_processed_.setZero();
    torque_rl_.setZero();

    policy_frame_.assign(num_single_obs, 0.0f);
    policy_obs_hist_term_major_.assign(policy_obs_dim_, 0.0f);
    policy_hist_initialized_ = false;
}

// =====================================================================
// loadOnnX - Load ONNX model and infer dimensions from shapes
// =====================================================================
void CustomController::loadOnnX()
{
    string cur_path = weight_dir_;
    cout << "[p73_walker_cc] Loading network from " << cur_path << endl;

    Ort::SessionOptions session_options;
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_DISABLE_ALL);
    session_options.AddConfigEntry("session.use_deterministic_compute", "1");
    session = Ort::Session(env, cur_path.c_str(), session_options);

    Ort::AllocatorWithDefaultOptions allocator;
    input_number = session.GetInputCount();
    output_number = session.GetOutputCount();

    input_names.resize(input_number);
    output_names.resize(output_number);
    input_names_char.resize(input_number);
    output_names_char.resize(output_number);

    for (size_t i = 0; i < input_number; i++) {
        Ort::AllocatedStringPtr name = session.GetInputNameAllocated(i, allocator);
        input_names[i] = name.get();
    }
    for (size_t i = 0; i < output_number; i++) {
        Ort::AllocatedStringPtr name = session.GetOutputNameAllocated(i, allocator);
        output_names[i] = name.get();
    }

    cout << "[p73_walker_cc] Input names: ";
    copy(input_names.begin(), input_names.end(), ostream_iterator<string>(cout, " "));
    cout << endl;
    cout << "[p73_walker_cc] Output names: ";
    copy(output_names.begin(), output_names.end(), ostream_iterator<string>(cout, " "));
    cout << endl;

    for (size_t i = 0; i < input_names.size(); ++i) {
        input_names_char[i] = input_names[i].c_str();
        if (input_names[i] == "policy_obs_history" || input_names[i] == "obs")
            input_policy_idx_ = static_cast<int>(i);
        if (input_names[i] == "critic_obs")
            input_critic_idx_ = static_cast<int>(i);
    }
    for (size_t i = 0; i < output_names.size(); ++i) {
        output_names_char[i] = output_names[i].c_str();
        if (output_names[i] == "actions") output_actions_idx_ = static_cast<int>(i);
        if (output_names[i] == "value")   output_value_idx_ = static_cast<int>(i);
    }

    if (input_policy_idx_ < 0) {
        throw std::runtime_error("[p73_walker_cc] ONNX input 'obs' or 'policy_obs_history' not found.");
    }
    if (output_actions_idx_ < 0) {
        throw std::runtime_error("[p73_walker_cc] ONNX output 'actions' not found.");
    }

    // Initialize input tensors
    for (size_t i = 0; i < input_number; ++i) {
        Ort::TypeInfo type_info = session.GetInputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        std::vector<int64_t> input_shape = tensor_info.GetShape();
        cout << "[p73_walker_cc] Input " << i << " (" << input_names[i] << ") shape: ";
        for (size_t k = 0; k < input_shape.size(); k++)
            cout << input_shape[k] << (k + 1 < input_shape.size() ? "x" : "");
        cout << endl;

        std::vector<float> input_tensor_values(tensor_info.GetElementCount(), 0.0f);
        input_states_buffer.push_back(std::move(input_tensor_values));

        input_tensors.emplace_back(Ort::Value::CreateTensor<float>(
            memory_info,
            input_states_buffer.back().data(),
            input_states_buffer.back().size(),
            input_shape.data(),
            input_shape.size()));
    }

    // Infer history_length from policy_obs_history shape: (1, 47*H)
    if (input_policy_idx_ >= 0) {
        Ort::TypeInfo type_info = session.GetInputTypeInfo(static_cast<size_t>(input_policy_idx_));
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        auto s = tensor_info.GetShape();
        if (s.size() == 2 && s[1] > 0) {
            policy_obs_dim_ = static_cast<int>(s[1]);
            if (policy_obs_dim_ % num_single_obs != 0) {
                throw std::runtime_error("[p73_walker_cc] policy_obs_history dim must be divisible by 47.");
            }
            history_length_ = policy_obs_dim_ / num_single_obs;
            cout << "[p73_walker_cc] Inferred policy_obs_dim=" << policy_obs_dim_
                 << " (history_length=" << history_length_ << ")" << endl;
        }
    }

    cout << "[p73_walker_cc] Network loaded successfully." << endl;
}

// =====================================================================
// processObservation - Build 47D policy frame and update term-major history
// =====================================================================
void CustomController::processObservation()
{
    // === Build single-frame observation (47D) in IsaacLab term order ===
    // 0) base_ang_vel(3)         [body frame]
    // 1) projected_gravity(3)    [body frame]
    // 2) velocity_commands(3)    [vx, vy, wz]
    // 3) gait_phase_sin(1)
    // 4) gait_phase_cos(1)
    // 5) motor_joint_pos(12)     [IsaacLab order, relative to default]
    // 6) motor_joint_vel(12)     [IsaacLab order]
    // 7) last_action(12)         [processed = raw * scale]

    // q_virtual_ layout: [pos(3), quat_xyzw(4), joints(13)] = 20D
    // q_dot_virtual_ layout: [lin_vel(3), ang_vel(3), joint_vel(13)] = 19D
    Quaterniond q;
    q.x() = rd_cc_.q_virtual_(3);
    q.y() = rd_cc_.q_virtual_(4);
    q.z() = rd_cc_.q_virtual_(5);
    q.w() = rd_cc_.q_virtual_(6);

    Vector3d ang_vel_w = rd_cc_.q_dot_virtual_.segment<3>(3);
    Vector3d ang_vel_b = quatRotateInverse(q, ang_vel_w);

    Vector3d g_w(0.0, 0.0, -1.0);
    Vector3d projected_gravity_b = quatRotateInverse(q, g_w);

    // Joint pos/vel in P73 order (lower 12 joints)
    // Joints start at index 7 in q_virtual_ (after pos3 + quat4)
    // Joints start at index 6 in q_dot_virtual_ (after lin_vel3 + ang_vel3)
    VectorXd q_pos_p73 = rd_cc_.q_virtual_.segment<12>(7);
    VectorXd q_vel_p73 = rd_cc_.q_dot_virtual_.segment<12>(6);

    // Permute to IsaacLab order
    Vec12 q_pos_isaac = p73_to_isaac(q_pos_p73, kP73ToIsaac);
    Vec12 q_vel_isaac = p73_to_isaac(q_vel_p73, kP73ToIsaac);
    Vec12 q_pos_rel_isaac = q_pos_isaac - q_default_isaac_;

    // Read velocity commands (thread-safe)
    double local_vel_x, local_vel_y, local_vel_yaw;
    {
        std::lock_guard<std::mutex> lock(vel_mutex_);
        local_vel_x = target_vel_x_;
        local_vel_y = target_vel_y_;
        local_vel_yaw = target_vel_yaw_;
    }

    // Gait phase (use same velocity as obs, not subscriber values)
    const double obs_vel_x = 0.5, obs_vel_y = 0.0, obs_vel_yaw = 0.0;  // must match obs below
    double cmd_norm = std::sqrt(obs_vel_x * obs_vel_x +
                                obs_vel_y * obs_vel_y +
                                obs_vel_yaw * obs_vel_yaw);
    double phase = 0.0;
    if (cmd_norm > cmd_zero_max_) {
        phase = static_cast<double>(gait_step_counter_ % gait_period_steps_) /
                static_cast<double>(gait_period_steps_);
    }
    double gait_sin = std::sin(2.0 * M_PI * phase);
    double gait_cos = std::cos(2.0 * M_PI * phase);

    // Fill policy frame
    int idx = 0;

    // base_ang_vel (3)
    policy_frame_[idx++] = static_cast<float>(ang_vel_b(0));
    policy_frame_[idx++] = static_cast<float>(ang_vel_b(1));
    policy_frame_[idx++] = static_cast<float>(ang_vel_b(2));

    // projected_gravity (3)
    policy_frame_[idx++] = static_cast<float>(projected_gravity_b(0));
    policy_frame_[idx++] = static_cast<float>(projected_gravity_b(1));
    policy_frame_[idx++] = static_cast<float>(projected_gravity_b(2));

    // velocity_commands (3) — hardcoded for testing, use local_vel_* for teleop
    policy_frame_[idx++] = 0.5f;  // local_vel_x
    policy_frame_[idx++] = 0.0f;  // local_vel_y
    policy_frame_[idx++] = 0.0f;  // local_vel_yaw

    // gait_phase_sin (1)
    policy_frame_[idx++] = static_cast<float>(gait_sin);

    // gait_phase_cos (1)
    policy_frame_[idx++] = static_cast<float>(gait_cos);

    // motor_joint_pos relative to default (12, IsaacLab order)
    for (int i = 0; i < 12; i++)
        policy_frame_[idx++] = static_cast<float>(q_pos_rel_isaac(i));

    // motor_joint_vel (12, IsaacLab order)
    for (int i = 0; i < 12; i++)
        policy_frame_[idx++] = static_cast<float>(q_vel_isaac(i));

    // last_action (12, processed = raw * scale)
    for (int i = 0; i < num_action; i++)
        policy_frame_[idx++] = static_cast<float>(last_action_processed_(i));

    // === Update term-major history buffer ===
    // IsaacLab ObservationManager layout (term-major):
    //   [ang_vel(3*H), gravity(3*H), cmd(3*H), sin(1*H), cos(1*H),
    //    joint_pos(12*H), joint_vel(12*H), last_action(12*H)]
    const int H = history_length_;

    // Term dimensions and offsets
    constexpr int dims[] = {3, 3, 3, 1, 1, 12, 12, 12};  // = 47
    int offsets[8];
    offsets[0] = 0;
    for (int t = 1; t < 8; t++)
        offsets[t] = offsets[t - 1] + dims[t - 1] * H;

    // Frame offsets within policy_frame_
    int frame_offsets[8];
    frame_offsets[0] = 0;
    for (int t = 1; t < 8; t++)
        frame_offsets[t] = frame_offsets[t - 1] + dims[t - 1];

    auto shift_append = [&](int offset, int dim, const float *cur) {
        std::memmove(
            policy_obs_hist_term_major_.data() + offset,
            policy_obs_hist_term_major_.data() + offset + dim,
            sizeof(float) * dim * (H - 1));
        std::memcpy(
            policy_obs_hist_term_major_.data() + offset + dim * (H - 1),
            cur, sizeof(float) * dim);
    };

    auto fill_all = [&](int offset, int dim, const float *cur) {
        for (int t = 0; t < H; ++t)
            std::memcpy(policy_obs_hist_term_major_.data() + offset + dim * t,
                        cur, sizeof(float) * dim);
    };

    if (!policy_hist_initialized_) {
        for (int t = 0; t < 8; t++)
            fill_all(offsets[t], dims[t], policy_frame_.data() + frame_offsets[t]);
        policy_hist_initialized_ = true;
    } else {
        for (int t = 0; t < 8; t++)
            shift_append(offsets[t], dims[t], policy_frame_.data() + frame_offsets[t]);
    }

    // Copy to ONNX input buffer
    std::memcpy(input_states_buffer[input_policy_idx_].data(),
                policy_obs_hist_term_major_.data(),
                sizeof(float) * policy_obs_dim_);

    // === Build critic_obs if needed ===
    if (input_critic_idx_ >= 0) {
        std::vector<float> &critic_in = input_states_buffer[input_critic_idx_];

        Vector3d lin_vel_w = rd_cc_.q_dot_virtual_.segment<3>(0);
        Vector3d lin_vel_b = quatRotateInverse(q, lin_vel_w);

        // gt_vel3 = [vx_b, vy_b, wz_b]
        critic_in[0] = static_cast<float>(lin_vel_b(0));
        critic_in[1] = static_cast<float>(lin_vel_b(1));
        critic_in[2] = static_cast<float>(ang_vel_b(2));

        // gt_foot_force6 = zeros (contact forces not available in simple deployment)
        for (int i = 3; i < 9; i++)
            critic_in[i] = 0.0f;

        // Copy policy single frame (47D)
        if (critic_in.size() >= static_cast<size_t>(9 + num_single_obs))
            std::memcpy(critic_in.data() + 9, policy_frame_.data(), sizeof(float) * num_single_obs);
    }

    // Increment gait step counter
    gait_step_counter_++;
}

// =====================================================================
// feedforwardPolicy
// =====================================================================
void CustomController::feedforwardPolicy()
{
    output_tensors = session.Run(
        Ort::RunOptions{nullptr},
        input_names_char.data(), input_tensors.data(), input_number,
        output_names_char.data(), output_number);

    for (size_t i = 0; i < output_tensors.size(); i++) {
        if (!output_tensors[i].IsTensor()) {
            cerr << "[p73_walker_cc] Output " << i << " is not a valid tensor." << endl;
            continue;
        }
    }

    // Extract actions (12D, IsaacLab order)
    const float *actions_ptr = output_tensors[output_actions_idx_].GetTensorMutableData<float>();
    for (int i = 0; i < num_action; i++)
        rl_action_(i) = actions_ptr[i];

    // Extract value (optional)
    if (output_value_idx_ >= 0) {
        const float *value_ptr = output_tensors[output_value_idx_].GetTensorMutableData<float>();
        value_ = static_cast<double>(value_ptr[0]);
    }

    // Update last_action_processed for next observation
    // last_processed_action in IsaacLab = raw_action * scale (clipped)
    for (int i = 0; i < num_action; i++)
        last_action_processed_(i) = DyrosMath::minmax_cut(rl_action_(i) * action_scale_, -1.0, 1.0);
}

// =====================================================================
// computeFast - Main control loop (called from TaskCtrlThread at ~2kHz)
// =====================================================================
void CustomController::computeFast()
{
    copyRobotData(rd_);

    static int debug_counter = 0;
    if (debug_counter++ % 2000 == 0) {
        cout << "[p73_walker_cc] task_mode=" << dc_.task_cmd_.task_mode
             << " tc_mode=" << dc_.tc_mode << " cc_init=" << cc_init_
             << " simMode=" << dc_.simMode << endl;
        if (dc_.task_cmd_.task_mode >= 5 && dc_.task_cmd_.task_mode < 10) {
            cout << "[p73_walker_cc] action: " << rl_action_.transpose().format(Eigen::IOFormat(3, 0, " ", " ")) << endl;
            cout << "[p73_walker_cc] torque: " << rd_.torque_desired.transpose().format(Eigen::IOFormat(3, 0, " ", " ")) << endl;
            VectorXd q_pos = rd_cc_.q_virtual_.segment<MODEL_DOF>(7);
            cout << "[p73_walker_cc] q_pos:  " << q_pos.transpose().format(Eigen::IOFormat(3, 0, " ", " ")) << endl;
        }
    }

    if (dc_.task_cmd_.task_mode >= 5 && dc_.task_cmd_.task_mode < 10)
    {
        if (cc_init_)
        {
            cc_init_ = false;
            cout << "[p73_walker_cc] Mode " << dc_.task_cmd_.task_mode << " started." << endl;
            start_time_ = rd_cc_.control_time_us_;
            torque_init_ = rd_.torque_desired;
            time_inference_pre_ = rd_cc_.control_time_us_ - policy_dt_ * 1e6;

            // Reset state
            rl_action_.setZero();
            last_action_processed_.setZero();
            gait_step_counter_ = 0;
            policy_hist_initialized_ = false;
            std::fill(policy_obs_hist_term_major_.begin(), policy_obs_hist_term_major_.end(), 0.0f);

            // First inference
            processObservation();
            feedforwardPolicy();
        }

        // Policy update at policy_dt_ (50Hz)
        const bool do_policy_update =
            (rd_cc_.control_time_us_ - time_inference_pre_) / 1.0e6 >= policy_dt_;

        if (do_policy_update) {
            processObservation();
            feedforwardPolicy();
            time_inference_pre_ = rd_cc_.control_time_us_;
        }

        // === Action → Target Position → PD → Torque ===
        // IsaacLab: q_des = q_default + raw_action * scale (clipped to [-1,1])
        // PD gains applied directly (no /9 or /3 scaling, unlike tocabi)
        Vec12 delta_q_isaac;
        for (int i = 0; i < num_action; i++) {
            double dq = rl_action_(i) * action_scale_;  // scale = 0.5
            delta_q_isaac(i) = DyrosMath::minmax_cut(dq, -1.0, 1.0);
        }

        // Convert to P73 order and compute PD torque for lower 12 joints
        VectorQd target_pos = q_default_p73_;
        VectorXd delta_p73 = VectorXd::Zero(12);
        isaac_to_p73(delta_q_isaac, delta_p73, kIsaacToP73);
        for (int i = 0; i < 12; i++) {
            target_pos(i) = q_default_p73_(i) + delta_p73(i);
            // Clamp q_des to joint limits (matches IsaacLab action clamping)
            target_pos(i) = DyrosMath::minmax_cut(target_pos(i), q_limit_lower_p73_(i), q_limit_upper_p73_(i));
        }

        // PD torque for all 13 joints (WaistYaw held at default by PD)
        VectorXd q_pos_p73 = rd_cc_.q_virtual_.segment<MODEL_DOF>(7);
        VectorXd q_vel_p73 = rd_cc_.q_dot_virtual_.segment<MODEL_DOF>(6);

        for (int i = 0; i < MODEL_DOF; i++) {
            torque_rl_(i) = kp_p73_(i) * (target_pos(i) - q_pos_p73(i))
                          - kd_p73_(i) * q_vel_p73(i);
            torque_rl_(i) = DyrosMath::minmax_cut(torque_rl_(i),
                            -torque_bound_p73_(i), torque_bound_p73_(i));
        }

        // Spline transition for first 100ms
        if (rd_cc_.control_time_us_ < start_time_ + 0.1e6) {
            for (int i = 0; i < MODEL_DOF; i++)
                torque_spline_(i) = DyrosMath::cubic(rd_cc_.control_time_us_,
                    start_time_, start_time_ + 0.1e6,
                    torque_init_(i), torque_rl_(i), 0.0, 0.0);
            rd_.torque_desired = torque_spline_;
        } else {
            rd_.torque_desired = torque_rl_;
        }
    }
}

// =====================================================================
// computeSlow - Reserved for future slow-rate computations
// =====================================================================
void CustomController::computeSlow()
{
}

// =====================================================================
// Utility
// =====================================================================
void CustomController::copyRobotData(RobotEigenData &rd_l)
{
    std::memcpy(&rd_cc_, &rd_l, sizeof(RobotEigenData));
}

Vector3d CustomController::quatRotateInverse(const Quaterniond &q, const Vector3d &v)
{
    Vector3d q_vec = q.vec();
    double q_w = q.w();
    Vector3d a = v * (2.0 * q_w * q_w - 1.0);
    Vector3d b = 2.0 * q_w * q_vec.cross(v);
    Vector3d c = 2.0 * q_vec * q_vec.dot(v);
    return a - b + c;
}

// =====================================================================
// ROS2 Velocity Command Subscriber
// =====================================================================
void CustomController::velCmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    std::lock_guard<std::mutex> lock(vel_mutex_);
    target_vel_x_   = msg->linear.x;
    target_vel_y_   = msg->linear.y;
    target_vel_yaw_ = msg->angular.z;
}

void CustomController::startVelSubscriber()
{
    vel_node_ = rclcpp::Node::make_shared("p73_vel_cmd_listener");
    vel_sub_ = vel_node_->create_subscription<geometry_msgs::msg::Twist>(
        "p73/cmd_vel", 10,
        std::bind(&CustomController::velCmdCallback, this, std::placeholders::_1));

    vel_spin_running_ = true;
    vel_spin_thread_ = std::thread([this]() {
        while (vel_spin_running_ && rclcpp::ok()) {
            rclcpp::spin_some(vel_node_);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    cout << "[p73_walker_cc] Velocity command subscriber started on topic: p73/cmd_vel" << endl;
    cout << "[p73_walker_cc] Usage: ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args -r cmd_vel:=p73/cmd_vel" << endl;
}

void CustomController::stopVelSubscriber()
{
    vel_spin_running_ = false;
    if (vel_spin_thread_.joinable())
        vel_spin_thread_.join();
    vel_sub_.reset();
    vel_node_.reset();
}
