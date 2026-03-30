#include "cc.h"
#include <cmath>
#include <iomanip>
#include <numeric>

// =====================================================================
// NOTE on joint ordering:
//
// SHM data (from MuJoCo via launch joint_names) is in MuJoCo/IsaacLab order:
//   L_HipRoll, L_HipPitch, L_HipYaw, L_Knee, L_AnklePitch, L_AnkleRoll,
//   R_HipRoll, R_HipPitch, R_HipYaw, R_Knee, R_AnklePitch, R_AnkleRoll,
//   WaistYaw
//
// This is the SAME order as IsaacLab _LOWER_JOINT_NAMES and MuJoCo XML actuators.
// Therefore NO permutation is needed — data flows directly.
// =====================================================================

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
// initVariable — ALL values in MuJoCo/IsaacLab order (Roll, Pitch, Yaw)
// =====================================================================
void CustomController::initVariable()
{
    cout << "[p73_walker_cc] Initializing variables" << endl;

    q_default_p73_ <<  0.0,  0.36, 0.0,  0.77, -0.41, 0.0,
                       0.0, -0.36, 0.0, -0.77,  0.41, 0.0,
                       0.0;

    q_default_isaac_ <<  0.0,  0.36, 0.0,  0.77, -0.41, 0.0,
                         0.0, -0.36, 0.0, -0.77,  0.41, 0.0;

    kp_p73_ << 1536.0, 937.5, 625.0, 747.552, 490.644, 490.104,
               1536.0, 937.5, 625.0, 747.552, 490.644, 490.104,
               576.0;

    kd_p73_ << 76.8, 37.5, 12.5, 37.378, 16.355, 16.337,
               76.8, 37.5, 12.5, 37.378, 16.355, 16.337,
               19.2;

    torque_bound_p73_ << 352.0, 220.0, 95.0, 220.0, 95.0, 95.0,
                         352.0, 220.0, 95.0, 220.0, 95.0, 95.0,
                         152.0;

    q_limit_lower_p73_ << -0.58, -1.57, -0.78,  0.0,  -1.05, -0.42,
                          -0.58, -2.09, -0.78, -2.56, -0.7,  -0.42;
    q_limit_upper_p73_ <<  0.3,   2.09,  0.78,  2.56,  0.7,   0.42,
                           0.3,   1.57,  0.78,  0.0,   1.05,  0.42;

    rl_action_.setZero();
    last_action_processed_.setZero();
    torque_rl_.setZero();

    policy_frame_.assign(num_single_obs, 0.0f);
    policy_obs_hist_term_major_.assign(policy_obs_dim_, 0.0f);
    policy_hist_initialized_ = false;
}

// =====================================================================
// loadOnnX
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

    if (input_policy_idx_ < 0)
        throw std::runtime_error("[p73_walker_cc] ONNX input 'obs' or 'policy_obs_history' not found.");
    if (output_actions_idx_ < 0)
        throw std::runtime_error("[p73_walker_cc] ONNX output 'actions' not found.");

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

    if (input_policy_idx_ >= 0) {
        Ort::TypeInfo type_info = session.GetInputTypeInfo(static_cast<size_t>(input_policy_idx_));
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        auto s = tensor_info.GetShape();
        if (s.size() == 2 && s[1] > 0) {
            policy_obs_dim_ = static_cast<int>(s[1]);
            if (policy_obs_dim_ % num_single_obs != 0)
                throw std::runtime_error("[p73_walker_cc] policy_obs_history dim must be divisible by 47.");
            history_length_ = policy_obs_dim_ / num_single_obs;
            cout << "[p73_walker_cc] Inferred policy_obs_dim=" << policy_obs_dim_
                 << " (history_length=" << history_length_ << ")" << endl;
        }
    }

    cout << "[p73_walker_cc] Network loaded successfully." << endl;
}

// =====================================================================
// processObservation — uses rd_ directly (no copyRobotData)
// =====================================================================
void CustomController::processObservation()
{
    Quaterniond q;
    q.x() = rd_.q_virtual_(3);
    q.y() = rd_.q_virtual_(4);
    q.z() = rd_.q_virtual_(5);
    q.w() = rd_.q_virtual_(6);

    Vector3d ang_vel_w = rd_.q_dot_virtual_.segment<3>(3);
    Vector3d ang_vel_b = quatRotateInverse(q, ang_vel_w);

    Vector3d g_w(0.0, 0.0, -1.0);
    Vector3d projected_gravity_b = quatRotateInverse(q, g_w);

    // Joint pos/vel from rd_.q_ (SHM order = MuJoCo/IsaacLab order)
    VectorXd q_pos = rd_.q_.head<12>();
    VectorXd q_vel = rd_.q_dot_.head<12>();
    VectorXd q_pos_rel = q_pos - q_default_isaac_.cast<double>();

    double local_vel_x, local_vel_y, local_vel_yaw;
    {
        std::lock_guard<std::mutex> lock(vel_mutex_);
        local_vel_x = target_vel_x_;
        local_vel_y = target_vel_y_;
        local_vel_yaw = target_vel_yaw_;
    }
    // DEBUG: override velocity command for testing (remove when teleop works)
    local_vel_x = 0.5;
    local_vel_y = 0.0;
    local_vel_yaw = 0.0;

    double cmd_norm = std::sqrt(local_vel_x * local_vel_x +
                                local_vel_y * local_vel_y +
                                local_vel_yaw * local_vel_yaw);
    double phase = 0.0;
    if (cmd_norm > cmd_zero_max_) {
        phase = static_cast<double>(gait_step_counter_ % gait_period_steps_) /
                static_cast<double>(gait_period_steps_);
    }
    double gait_sin = std::sin(2.0 * M_PI * phase);
    double gait_cos = std::cos(2.0 * M_PI * phase);

    int idx = 0;
    policy_frame_[idx++] = static_cast<float>(ang_vel_b(0));
    policy_frame_[idx++] = static_cast<float>(ang_vel_b(1));
    policy_frame_[idx++] = static_cast<float>(ang_vel_b(2));
    policy_frame_[idx++] = static_cast<float>(projected_gravity_b(0));
    policy_frame_[idx++] = static_cast<float>(projected_gravity_b(1));
    policy_frame_[idx++] = static_cast<float>(projected_gravity_b(2));
    policy_frame_[idx++] = static_cast<float>(local_vel_x);
    policy_frame_[idx++] = static_cast<float>(local_vel_y);
    policy_frame_[idx++] = static_cast<float>(local_vel_yaw);
    policy_frame_[idx++] = static_cast<float>(gait_sin);
    policy_frame_[idx++] = static_cast<float>(gait_cos);
    for (int i = 0; i < 12; i++)
        policy_frame_[idx++] = static_cast<float>(q_pos_rel(i));
    for (int i = 0; i < 12; i++)
        policy_frame_[idx++] = static_cast<float>(q_vel(i));
    for (int i = 0; i < num_action; i++)
        policy_frame_[idx++] = static_cast<float>(last_action_processed_(i));

    // Term-major history
    const int H = history_length_;
    constexpr int dims[] = {3, 3, 3, 1, 1, 12, 12, 12};
    int offsets[8]; offsets[0] = 0;
    for (int t = 1; t < 8; t++) offsets[t] = offsets[t-1] + dims[t-1] * H;
    int frame_offsets[8]; frame_offsets[0] = 0;
    for (int t = 1; t < 8; t++) frame_offsets[t] = frame_offsets[t-1] + dims[t-1];

    auto shift_append = [&](int offset, int dim, const float *cur) {
        std::memmove(policy_obs_hist_term_major_.data() + offset,
                     policy_obs_hist_term_major_.data() + offset + dim,
                     sizeof(float) * dim * (H - 1));
        std::memcpy(policy_obs_hist_term_major_.data() + offset + dim * (H - 1),
                    cur, sizeof(float) * dim);
    };
    auto fill_all = [&](int offset, int dim, const float *cur) {
        for (int t = 0; t < H; ++t)
            std::memcpy(policy_obs_hist_term_major_.data() + offset + dim * t,
                        cur, sizeof(float) * dim);
    };

    if (!policy_hist_initialized_) {
        for (int t = 0; t < 8; t++) fill_all(offsets[t], dims[t], policy_frame_.data() + frame_offsets[t]);
        policy_hist_initialized_ = true;
    } else {
        for (int t = 0; t < 8; t++) shift_append(offsets[t], dims[t], policy_frame_.data() + frame_offsets[t]);
    }

    std::memcpy(input_states_buffer[input_policy_idx_].data(),
                policy_obs_hist_term_major_.data(), sizeof(float) * policy_obs_dim_);

    if (input_critic_idx_ >= 0) {
        std::vector<float> &critic_in = input_states_buffer[input_critic_idx_];
        Vector3d lin_vel_w = rd_.q_dot_virtual_.segment<3>(0);
        Vector3d lin_vel_b = quatRotateInverse(q, lin_vel_w);
        critic_in[0] = static_cast<float>(lin_vel_b(0));
        critic_in[1] = static_cast<float>(lin_vel_b(1));
        critic_in[2] = static_cast<float>(ang_vel_b(2));
        for (int i = 3; i < 9; i++) critic_in[i] = 0.0f;
        if (critic_in.size() >= static_cast<size_t>(9 + num_single_obs))
            std::memcpy(critic_in.data() + 9, policy_frame_.data(), sizeof(float) * num_single_obs);
    }

    gait_step_counter_++;
}

// =====================================================================
// feedforwardPolicy
// =====================================================================
void CustomController::feedforwardPolicy()
{
    // Use local variable instead of member output_tensors to avoid
    // Ort::Value destructor interfering with heap between calls
    auto local_output = session.Run(
        Ort::RunOptions{nullptr},
        input_names_char.data(), input_tensors.data(), input_number,
        output_names_char.data(), output_number);

    if (output_actions_idx_ >= 0 &&
        static_cast<size_t>(output_actions_idx_) < local_output.size() &&
        local_output[output_actions_idx_].IsTensor()) {
        const float *actions_ptr = local_output[output_actions_idx_].GetTensorMutableData<float>();
        for (int i = 0; i < num_action; i++)
            rl_action_(i) = actions_ptr[i];
    }

    if (output_value_idx_ >= 0 &&
        static_cast<size_t>(output_value_idx_) < local_output.size() &&
        local_output[output_value_idx_].IsTensor()) {
        const float *value_ptr = local_output[output_value_idx_].GetTensorMutableData<float>();
        value_ = static_cast<double>(value_ptr[0]);
    }

    for (int i = 0; i < num_action; i++)
        last_action_processed_(i) = DyrosMath::minmax_cut(rl_action_(i) * action_scale_, -1.0, 1.0);
    // local_output destroyed here — Ort::Value cleanup happens at function exit
}

// =====================================================================
// computeFast — uses rd_ directly, NO copyRobotData
// =====================================================================
void CustomController::computeFast()
{
    float control_time_us = rd_.control_time_us_;

    static bool init = true;
    if (init) {
        init = false;
        start_time_ = control_time_us;
        torque_init_ = rd_.torque_desired;
        time_inference_pre_ = control_time_us - policy_dt_ * 1e6;
        rl_action_.setZero();
        last_action_processed_.setZero();
        gait_step_counter_ = 0;
        policy_hist_initialized_ = false;
        std::fill(policy_obs_hist_term_major_.begin(), policy_obs_hist_term_major_.end(), 0.0f);
        cout << "[p73_walker_cc] Mode started." << endl;

        processObservation();
        feedforwardPolicy();
    }

    // Policy update at 50Hz
    if ((control_time_us - time_inference_pre_) / 1.0e6 >= policy_dt_) {
        processObservation();
        feedforwardPolicy();
        time_inference_pre_ = control_time_us;
    }

    // Action → Target Position → PD (every tick, no 200Hz hold)
    VectorQd target_pos = q_default_p73_;
    for (int i = 0; i < num_action; i++) {
        double dq = rl_action_(i) * action_scale_;
        dq = DyrosMath::minmax_cut(dq, -1.0, 1.0);
        target_pos(i) = q_default_p73_(i) + dq;
        target_pos(i) = DyrosMath::minmax_cut(target_pos(i), q_limit_lower_p73_(i), q_limit_upper_p73_(i));
    }
    for (int i = 0; i < MODEL_DOF; i++) {
        torque_rl_(i) = kp_p73_(i) * (target_pos(i) - rd_.q_(i))
                      - kd_p73_(i) * rd_.q_dot_(i);
        torque_rl_(i) = DyrosMath::minmax_cut(torque_rl_(i),
                        -torque_bound_p73_(i), torque_bound_p73_(i));
    }

    // Spline transition for first 100ms
    if (control_time_us < start_time_ + 0.1e6) {
        for (int i = 0; i < MODEL_DOF; i++)
            torque_spline_(i) = DyrosMath::cubic(control_time_us,
                start_time_, start_time_ + 0.1e6,
                torque_init_(i), torque_rl_(i), 0.0, 0.0);
        rd_.torque_desired = torque_spline_;
    } else {
        rd_.torque_desired = torque_rl_;
    }

    // Debug
    static int dbg = 0;
    if (dbg++ % 500 == 0) {
        Eigen::IOFormat fmt(3, 0, " ", " ");
        cout << "[cc] t=" << control_time_us/1e6
             << " act: " << rl_action_.transpose().format(fmt)
             << " | gait: " << gait_step_counter_ << endl;
    }
}

// =====================================================================
void CustomController::computeSlow() {}

void CustomController::copyRobotData(RobotEigenData &rd_l)
{
    // DEPRECATED: memcpy on RobotEigenData corrupts std::vector members.
    // Use rd_ directly instead.
    (void)rd_l;
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
    if (vel_spin_thread_.joinable()) vel_spin_thread_.join();
    vel_sub_.reset();
    vel_node_.reset();
}
