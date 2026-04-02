#pragma once
#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QDoubleSpinBox>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <p73_msgs/msg/task_cmd.hpp>
#include <p73_msgs/msg/pos_cmd.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/u_int32.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <std_msgs/msg/int8_multi_array.hpp>
#include <sensor_msgs/msg/joint_state.hpp>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow, public rclcpp::Node {
	Q_OBJECT

public:
	static constexpr int DOF_NUM = 13;

	explicit MainWindow(QWidget* parent=nullptr);
	~MainWindow();

	// Joint label groups for different body parts
	struct JointLabels {
		std::vector<QLabel*> pos, vel, torq;
		JointLabels() = default;  // Default constructor
		JointLabels(int size) : pos(size), vel(size), torq(size) {}
	};

	std::vector<QLabel*> pelv_state_labels;
	std::vector<QLabel*> imu_state_labels;
	std::vector<QLabel*> elmo_state_labels;
	std::vector<QLabel*> elmo_ctrl_state_labels;
	
	JointLabels lleg_labels, rleg_labels, waist_labels;
	std::vector<QDoubleSpinBox*> joint_pos_cmd_spinboxes;

private slots:
	void guiCommandSend(QString cmd);
  	void taskModeSend();

	void posCtrlModeSend();
	void jointSendModeSend();
	void posModeSend(const float* position);

private:
	// Helper function to create and setup labels
	void setupJointLabels(JointLabels& labels, int count, 
		QLayout* pos_layout, QLayout* vel_layout, QLayout* torq_layout);
	void setupElmoLabels(std::vector<QLabel*>& labels, int start_idx, int count, 
		QLayout* elmo_layout);
	void setupSpinBoxes(int start_idx, int count, QLayout* form_layout);
	void updateProgressBarColor(int percentage);
	
	// ROS 2 callbacks
	void ctrlTimeCallback(const std_msgs::msg::Float32::SharedPtr msg);
	void sysStateCallback(const std_msgs::msg::Int8MultiArray::SharedPtr msg);
	void jointStateCallback(const sensor_msgs::msg::JointState::SharedPtr msg);
	void elmoStateCallback(const std_msgs::msg::Int8MultiArray::SharedPtr msg);
	void statusLogCallback(const std_msgs::msg::String::SharedPtr msg);
	void pelvStateCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg);
	void imuStateCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg);

  	std::unique_ptr<Ui::MainWindow> ui_;
  	QTimer timer_;

	// ROS 2 publishers
  	rclcpp::Publisher<p73_msgs::msg::TaskCmd>::SharedPtr task_cmd_pub_;
	rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr ctrl_mode_pub_;
	rclcpp::Publisher<p73_msgs::msg::PosCmd>::SharedPtr pos_cmd_pub_;
	rclcpp::Publisher<std_msgs::msg::String>::SharedPtr gui_cmd_pub_;

	// ROS 2 subscribers
	rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr ctrl_time_sub_;
	rclcpp::Subscription<std_msgs::msg::Int8MultiArray>::SharedPtr sys_state_sub_;
	rclcpp::Subscription<std_msgs::msg::Int8MultiArray>::SharedPtr elmo_state_sub_;
	rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;
	rclcpp::Subscription<std_msgs::msg::String>::SharedPtr status_log_sub_;
	rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr pelv_state_sub_;
	rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr imu_state_sub_;

	float zero_pos_[DOF_NUM] = {
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0
	};

	float init_pos_[DOF_NUM] = {
		0.0, +0.18, 0.0,  0.35, -0.17, 0.0,
		0.0, -0.18, 0.0, -0.35,  0.17, 0.0,
		0.0
	};

};