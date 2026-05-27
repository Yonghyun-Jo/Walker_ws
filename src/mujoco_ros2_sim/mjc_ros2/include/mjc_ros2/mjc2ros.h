#include "rclcpp/rclcpp.hpp"
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

#include <mutex>

#include "ecat_data.h"
#include "imu_data.h"
#include "shm.h"

static joint_status_t joint_status;
static joint_command_t joint_command;
static imu_status_t imu_status;
static spsc_joint_status_t *joint_status_buffer;
static spsc_joint_command_t *joint_command_buffer;
static spsc_imu_status_t *imu_status_buffer;

// Global variables
int cnt = 0;
double prev_sim_time = -1.0;  // For reset detection

rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr sim_time_pub;
rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr virtual_state_pub;
rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr full_state_pub;

// Automation parameters
static bool g_auto_start = false;
static double g_auto_shutdown_duration = 0.0;  // 0 = disabled

// Ghost robot: driven by /p73/ghost_state (20D absolute pose)
static std::mutex ghost_mutex;
static double ghost_qpos[20] = {0, 0.5, 0.895, 1, 0, 0, 0,
                                 0, 0.18, 0, 0.35, -0.17, 0,
                                 0, -0.18, 0, -0.35, 0.17, 0,
                                 0};
static bool ghost_state_received = false;
static rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr ghost_state_sub;