#include "rclcpp/rclcpp.hpp"
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

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