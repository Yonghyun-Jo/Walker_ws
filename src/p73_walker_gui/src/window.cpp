#include "p73_gui/window.hpp"

// uic가 p73.ui로부터 생성 (CMAKE_AUTOUIC 덕분에 자동 include 가능)
#include "ui_p73.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <cmath>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent), rclcpp::Node("p73_gui_node"), ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    
    // Set initial window size (width, height)
    resize(1600, 1000);
    // Set minimum window size to prevent too small windows
    setMinimumSize(800, 600);

	connect(ui_->torqueon_button, &QPushButton::clicked, this, [this]() { guiCommandSend("torqOn"); });
	connect(ui_->torqueoff_button, &QPushButton::clicked, this, [this]() { guiCommandSend("torqOff"); });
	connect(ui_->safeRstBtn, &QPushButton::clicked, this, [this]() { guiCommandSend("safetyReset"); });

	connect(ui_->initYawBtn, &QPushButton::clicked, this, [this]() { guiCommandSend("initYaw"); });
	connect(ui_->stateEstimateBtn, &QPushButton::clicked, this, [this]() { guiCommandSend("stateEstimate"); });
	connect(ui_->taskSendBtn, &QPushButton::clicked, this, &MainWindow::taskModeSend);
	connect(ui_->posCtrlBtn, &QPushButton::clicked, this, &MainWindow::posCtrlModeSend);
	connect(ui_->jointSendBtn, &QPushButton::clicked, this, &MainWindow::jointSendModeSend);
	connect(ui_->gravCompBtn, &QPushButton::clicked, this, &MainWindow::gravCtrlModeSend);

	connect(ui_->initPoseBtn, &QPushButton::clicked, this, [this]() { posModeSend(init_pos_); });
	connect(ui_->pos1Btn, &QPushButton::clicked, this, [this]() { posModeSend(left_hand_up_); });
	connect(ui_->pos2Btn, &QPushButton::clicked, this, [this]() { posModeSend(right_hand_up_); });
	connect(ui_->pos3Btn, &QPushButton::clicked, this, [this]() { posModeSend(both_hands_up_); });
	connect(ui_->zeroPoseBtn, &QPushButton::clicked, this, [this]() { posModeSend(zero_pos_); });

	connect(ui_->gwBtn, &QPushButton::clicked, this, &MainWindow::gwIkTestmodeSend);

	// ROS 2 Publishers
    ctrl_mode_pub_ = create_publisher<std_msgs::msg::UInt32>("p73/ctrlMode", 10);
    task_cmd_pub_ = create_publisher<p73_msgs::msg::TaskCmd>("p73/taskCommand", 10);
    pos_cmd_pub_ = create_publisher<p73_msgs::msg::PosCmd>("p73/posCommand", 10);
	gui_cmd_pub_ = create_publisher<std_msgs::msg::String>("p73/guiCommand", 10);

    iktask_mode_pub_ = create_publisher<p73_msgs::msg::IKTaskCmd>("p73/ikTaskmode", 10);

    // ROS 2 Subscribers
    ctrl_time_sub_ = create_subscription<std_msgs::msg::Float32>(
		"p73/ctrlTime", 10, 
		std::bind(&MainWindow::ctrlTimeCallback, this, std::placeholders::_1));

	sys_state_sub_ = create_subscription<std_msgs::msg::Int8MultiArray>(
		"p73/sysState", 10, 
		std::bind(&MainWindow::sysStateCallback, this, std::placeholders::_1));

    joint_state_sub_ = create_subscription<sensor_msgs::msg::JointState>(
		"p73/jointState", 10, 
		std::bind(&MainWindow::jointStateCallback, this, std::placeholders::_1));
    elmo_state_sub_ = create_subscription<std_msgs::msg::Int8MultiArray>(
		"p73/elmoState", 10, 
		std::bind(&MainWindow::elmoStateCallback, this, std::placeholders::_1));

	pelv_state_sub_ = create_subscription<std_msgs::msg::Float64MultiArray>(
		"p73/pelvState", 10, 
		std::bind(&MainWindow::pelvStateCallback, this, std::placeholders::_1));
	imu_state_sub_ = create_subscription<std_msgs::msg::Float64MultiArray>(
		"p73/imuState", 10, 
		std::bind(&MainWindow::imuStateCallback, this, std::placeholders::_1));

	status_log_sub_ = create_subscription<std_msgs::msg::String>(
		"p73/statusLog", 10,
		std::bind(&MainWindow::statusLogCallback, this, std::placeholders::_1));

	// Setup elmo state labels
	elmo_state_labels.resize(32);
	elmo_ctrl_state_labels.resize(32);
	setupElmoLabels(elmo_state_labels, 0, 6, ui_->leftleg_safety);
	setupElmoLabels(elmo_ctrl_state_labels, 0, 6, ui_->leftleg_elmo);
	setupElmoLabels(elmo_state_labels, 6, 6, ui_->rightleg_safety);
	setupElmoLabels(elmo_ctrl_state_labels, 6, 6, ui_->rightleg_elmo);
	setupElmoLabels(elmo_state_labels, 12, 3, ui_->waist_safety);
	setupElmoLabels(elmo_ctrl_state_labels, 12, 3, ui_->waist_elmo);
	setupElmoLabels(elmo_state_labels, 15, 7, ui_->leftarm_safety);
	setupElmoLabels(elmo_ctrl_state_labels, 15, 7, ui_->leftarm_elmo);
	setupElmoLabels(elmo_state_labels, 22, 7, ui_->rightarm_safety);
	setupElmoLabels(elmo_ctrl_state_labels, 22, 7, ui_->rightarm_elmo);
	setupElmoLabels(elmo_state_labels, 29, 3, ui_->head_safety);
	setupElmoLabels(elmo_ctrl_state_labels, 29, 3, ui_->head_elmo);

	// Initialize joint label groups with appropriate sizes
	lleg_labels = JointLabels(6);   // 6 DOF for legs
	rleg_labels = JointLabels(6);
	larm_labels = JointLabels(7);   // 7 DOF for arms  
	rarm_labels = JointLabels(7);
	waist_labels = JointLabels(3);  // 3 DOF for waist
	neck_labels = JointLabels(3);   // 3 DOF for neck

	// Setup all joint label groups
	setupJointLabels(lleg_labels, 6, ui_->position_layout, ui_->velocity_layout, ui_->torque_layout);
	setupJointLabels(rleg_labels, 6, ui_->position_layout_4, ui_->velocity_layout_4, ui_->torque_layout_4);
	setupJointLabels(larm_labels, 7, ui_->position_layout_6, ui_->velocity_layout_6, ui_->torque_layout_6);
	setupJointLabels(rarm_labels, 7, ui_->position_layout_5, ui_->velocity_layout_5, ui_->torque_layout_5);
	setupJointLabels(waist_labels, 3, ui_->position_layout_7, ui_->velocity_layout_7, ui_->torque_layout_7);
	setupJointLabels(neck_labels, 3, ui_->position_layout_8, ui_->velocity_layout_8, ui_->torque_layout_8);

	joint_pos_cmd_spinboxes.resize(32);
	
	// Setup spinboxes for joint position commands
	setupSpinBoxes(0, 6, ui_->formLayout); // Left leg
	setupSpinBoxes(6, 6, ui_->formLayout_2); // Right leg
	setupSpinBoxes(12, 3, ui_->formLayout_5); // Waist
	setupSpinBoxes(15, 7, ui_->formLayout_3); // Left arm
	setupSpinBoxes(22, 3, ui_->formLayout_6); // Neck
	setupSpinBoxes(25, 7, ui_->formLayout_4); // Right arm

	// Setup pelv state labels
	pelv_state_labels.resize(12);
	imu_state_labels.resize(9);
	for (int i = 0; i < 3; i++) {
		pelv_state_labels[i] = new QLabel(ui_->comPos->parentWidget());
		ui_->comPos->addWidget(pelv_state_labels[i]);
		pelv_state_labels[i+3] = new QLabel(ui_->linVel->parentWidget());
		ui_->linVel->addWidget(pelv_state_labels[i+3]);
		pelv_state_labels[i+6] = new QLabel(ui_->comPos_2->parentWidget());
		ui_->comPos_2->addWidget(pelv_state_labels[i+6]);
		pelv_state_labels[i+9] = new QLabel(ui_->linVel_2->parentWidget());
		ui_->linVel_2->addWidget(pelv_state_labels[i+9]);

		imu_state_labels[i] = new QLabel(ui_->imuQuaternion->parentWidget());
		ui_->imuQuaternion->addWidget(imu_state_labels[i]);
		imu_state_labels[i+3] = new QLabel(ui_->imuAngVel->parentWidget());
		ui_->imuAngVel->addWidget(imu_state_labels[i+3]);
		imu_state_labels[i+6] = new QLabel(ui_->imuLinAcc->parentWidget());
		ui_->imuLinAcc->addWidget(imu_state_labels[i+6]);
	}
	// imu_state_labels[3] = new QLabel(ui_->imuQuaternion->parentWidget());
	// ui_->imuQuaternion->addWidget(imu_state_labels[3]);
	
	// Setup IK target combobox with custom data
	// Index 0 (Pelvis) -> actual value 0
	ui_->ikTarget->setItemData(0, 0);
	// Index 1 (Left Hand) -> actual value 22
	ui_->ikTarget->setItemData(1, 22);
	// Index 2 (Right Hand) -> actual value 32
	ui_->ikTarget->setItemData(2, 32);
	
	updateProgressBarColor(0);
}

MainWindow::~MainWindow() = default;

void MainWindow::guiCommandSend(QString cmd){
	std_msgs::msg::String gui_cmd;
	gui_cmd.data = cmd.toStdString();
	gui_cmd_pub_->publish(gui_cmd);
}


void MainWindow::taskModeSend(){
    p73_msgs::msg::TaskCmd task_cmd;
    task_cmd.task_mode = ui_->taskMode->currentIndex();
	std::cout << "task_cmd.task_mode: " << ui_->taskMode->currentText().toStdString() << std::endl;
    task_cmd_pub_->publish(task_cmd);
}

void MainWindow::posCtrlModeSend(){
	std_msgs::msg::UInt32 ctrl_mode;
	ctrl_mode.data = 1;
	ctrl_mode_pub_->publish(ctrl_mode);
}

void MainWindow::jointSendModeSend(){
	p73_msgs::msg::PosCmd pos_cmd;
	pos_cmd.gravity = ui_->pc_grav->isChecked();
	pos_cmd.traj_time = ui_->trajTime->value();

	// TODO: this is available when the robot has neck motor 
	// for(int i = 0; i < 32; i++) {
	// 	pos_cmd.position[i] = joint_pos_cmd_spinboxes[i]->value();
	// }
	for (int i = 0; i < 22; i++) 
		pos_cmd.position[i] = joint_pos_cmd_spinboxes[i]->value();
	for (int i = 25; i < 32; i++) 
		pos_cmd.position[i-3] = joint_pos_cmd_spinboxes[i]->value();

	pos_cmd_pub_->publish(pos_cmd);
}

void MainWindow::gravCtrlModeSend(){
	std_msgs::msg::UInt32 ctrl_mode;
	ctrl_mode.data = 2;
	ctrl_mode_pub_->publish(ctrl_mode);
}

void MainWindow::ctrlTimeCallback(const std_msgs::msg::Float32::SharedPtr msg) 
{
    ui_->currenttime->setText(QString::number(msg->data, 'f', 3));
}

void MainWindow::sysStateCallback(const std_msgs::msg::Int8MultiArray::SharedPtr msg) {
	if (msg->data[0] == 0) { // se
		ui_->label_sestatus->setStyleSheet("QLabel { background-color : red ; color : black; }");
		ui_->label_sestatus->setText(QString::fromUtf8("OFF"));
	}
	else {
		ui_->label_sestatus->setStyleSheet("QLabel { background-color : rgb(138, 226, 52) ; color : black; }");
		ui_->label_sestatus->setText(QString::fromUtf8("ON"));
	}

	if (msg->data[1] == 0) { // TC Mode
		ui_->label_tcstatus->setStyleSheet("QLabel { background-color : red ; color : black; }");
		ui_->label_tcstatus->setText(QString::fromUtf8("OFF"));
	}
	else {
		ui_->label_tcstatus->setStyleSheet("QLabel { background-color : rgb(138, 226, 52) ; color : black; }");
		ui_->label_tcstatus->setText(QString::fromUtf8("ON"));
	}

	if (msg->data[2] == 0) { // IMU Status
		ui_->label_imustatus->setStyleSheet("QLabel { background-color : red ; color : black; }");
		ui_->label_imustatus->setText(QString::fromUtf8("OFF"));
	}
	else if (msg->data[2] == 2) {
		ui_->label_imustatus->setStyleSheet("QLabel { background-color : rgb(226, 214, 52) ; color : black; }");
		ui_->label_imustatus->setText(QString::fromUtf8("SIM MODE"));
	}
	else {
		ui_->label_imustatus->setStyleSheet("QLabel { background-color : rgb(138, 226, 52) ; color : black; }");
		ui_->label_imustatus->setText(QString::fromUtf8("ON"));
	}

	/*
	E-CAT Status
		0: ECAT_NOT_READY
		1: ECAT_OPERATIONAL
		2: ECAT_CONTROL
		3: ECAT_SAFETY
		4: ECAT_ERROR
	*/
	if (msg->data[3] == 0) {
		ui_->label_ecatstatus->setStyleSheet("QLabel { background-color : gray ; color : black; }");
		ui_->label_ecatstatus->setText(QString::fromUtf8("NOT READY"));
	}
	else if (msg->data[3] == 1) {
		ui_->label_ecatstatus->setStyleSheet("QLabel { background-color : rgb(138, 226, 52) ; color : black; }");
		ui_->label_ecatstatus->setText(QString::fromUtf8("OPER"));
	}
	else if (msg->data[3] == 2) {
		ui_->label_ecatstatus->setStyleSheet("QLabel { background-color : rgb(138, 226, 52) ; color : black; }");
		ui_->label_ecatstatus->setText(QString::fromUtf8("CONTROL"));
	}
	else if (msg->data[3] == 3) {
		ui_->label_ecatstatus->setStyleSheet("QLabel { background-color : rgb(226, 214, 52) ; color : black; }");
		ui_->label_ecatstatus->setText(QString::fromUtf8("SAFETY"));
	}
	else if (msg->data[3] == 4) {
		ui_->label_ecatstatus->setStyleSheet("QLabel { background-color : red ; color : black; }");
		ui_->label_ecatstatus->setText(QString::fromUtf8("ERROR"));
	}

	// if (msg->data[4] == 0) { // STO Status
	// 	ui_->label_stostatus->setStyleSheet("QLabel { background-color : red ; color : black; }");
	// 	ui_->label_stostatus->setText(QString::fromUtf8("OFF"));
	// }
	// else if (msg->data[4] == 2) {
	// 	ui_->label_stostatus->setStyleSheet("QLabel { background-color : rgb(226, 214, 52) ; color : black; }");
	// 	ui_->label_stostatus->setText(QString::fromUtf8("SIM MODE"));
	// }
	// else {
	// 	ui_->label_stostatus->setStyleSheet("QLabel { background-color : rgb(138, 226, 52) ; color : black; }");
	// 	ui_->label_stostatus->setText(QString::fromUtf8("ON"));
	// }
}

void MainWindow::jointStateCallback(const sensor_msgs::msg::JointState::SharedPtr msg) 
{
	// Helper lambda to update labels
	auto updateLabels = [&msg](JointLabels& labels, int start_idx, int count) {
		for(int i = 0; i < count; i++) {
			labels.pos[i]->setText(QString::number(msg->position[start_idx + i], 'f', 3));
			labels.vel[i]->setText(QString::number(msg->velocity[start_idx + i], 'f', 3));
			labels.torq[i]->setText(QString::number(msg->effort[start_idx + i], 'f', 3));
		}
	};

	// Update all joint groups
	updateLabels(lleg_labels, 0, 6);    // Left leg: indices 0-5
	updateLabels(rleg_labels, 6, 6);     // Right leg: indices 6-11
	updateLabels(waist_labels, 12, 3);   // Waist: indices 12-14
	updateLabels(larm_labels, 15, 7);    // Left arm: indices 15-21
	updateLabels(rarm_labels, 22, 7);     // Right arm: indices 22-28
}

void MainWindow::elmoStateCallback(const std_msgs::msg::Int8MultiArray::SharedPtr msg) {
	for (int i = 0; i < 29; i++) {
		int elmo_stat = msg->data[i];
		// int elmo_ctrl_stat = msg->data[i+29];

		elmo_state_labels[i]->setText(QString::number(elmo_stat));
		elmo_state_labels[i]->setStyleSheet(
			elmo_stat == 0 ? "QLabel { background-color : red ; color : black; }" :
			elmo_stat == 1 ? "QLabel { background-color : rgb(138, 226, 52) ; color : black; }" :
			elmo_stat == 2 ? "QLabel { background-color : rgb(226, 214, 52) ; color : black; }" :
			"QLabel { background-color : red ; color : black; }"
		);
		/*
		
		*/
	}

}

void MainWindow::pelvStateCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg) {
	for (int i = 0; i < 3; i++) {
		pelv_state_labels[i]->setText(QString::number(msg->data[i], 'f', 3));
		pelv_state_labels[i+3]->setText(QString::number(msg->data[i+3], 'f', 3));
		pelv_state_labels[i+6]->setText(QString::number(msg->data[i+6], 'f', 3));
		pelv_state_labels[i+9]->setText(QString::number(msg->data[i+9], 'f', 3));
	}
	int prg_val = (int)(100.0 * msg->data[12]);
	ui_->progressBar->setValue(prg_val);
	updateProgressBarColor(prg_val);
}

void MainWindow::imuStateCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg) {
	for (int i = 0; i < 3; i++) {
		imu_state_labels[i]->setText(QString::number(msg->data[i], 'f', 2));
	// }
	// for (int i = 0; i < 3; i++) {
		imu_state_labels[i+3]->setText(QString::number(msg->data[i+4], 'f', 3));
		imu_state_labels[i+6]->setText(QString::number(msg->data[i+7], 'f', 3));
	}
}

void MainWindow::updateProgressBarColor(int percentage) {
	// Calculate color gradient: Red (0%) -> Yellow (50%) -> Green (100%)
	int red, green, blue;
	
	if (percentage <= 50) {
		// 0-50%: Red (255,0,0) -> Yellow (255,255,0)
		red = 255;
		green = (255 * percentage) / 50;
		blue = 0;
	} else {
		// 50-100%: Yellow (255,255,0) -> Green (0,255,0)
		red = 255 - ((255 * (percentage - 50)) / 50);
		green = 255;
		blue = 0;
	}
	
	// Apply stylesheet with gradient color
	QString style = QString(
		"QProgressBar {"
		"	border: 1px solid black;"
		"	border-radius: 3px;"
		"	text-align: center;"
		"	background-color: rgb(200, 200, 200);"
		"}"
		"QProgressBar::chunk {"
		"	background-color: rgb(%1, %2, %3);"
		"	border-radius: 2px;"
		"}"
	).arg(red).arg(green).arg(blue);
	
	ui_->progressBar->setStyleSheet(style);
}

void MainWindow::statusLogCallback(const std_msgs::msg::String::SharedPtr msg) {
	ui_->plainTextEdit->appendPlainText(QString::fromStdString(msg->data));
}

void MainWindow::setupElmoLabels(std::vector<QLabel*>& labels, int start_idx, int count, 
	QLayout* elmo_layout) {
	for (int i = start_idx; i < start_idx + count; i++) {
		labels[i] = new QLabel(elmo_layout->parentWidget());
		elmo_layout->addWidget(labels[i]);
		labels[i]->setFrameShape(QFrame::Panel);
	}
}

void MainWindow::setupJointLabels(JointLabels& labels, int count, 
	QLayout* pos_layout, QLayout* vel_layout, QLayout* torq_layout) {
	for(int i = 0; i < count; i++) {
		// Position labels
		labels.pos[i] = new QLabel(pos_layout->parentWidget());
		pos_layout->addWidget(labels.pos[i]);
		labels.pos[i]->setFrameShape(QFrame::Panel);

		// Velocity labels
		labels.vel[i] = new QLabel(vel_layout->parentWidget());
		vel_layout->addWidget(labels.vel[i]);
		labels.vel[i]->setFrameShape(QFrame::Panel);

		// Torque labels
		labels.torq[i] = new QLabel(torq_layout->parentWidget());
		torq_layout->addWidget(labels.torq[i]);
		labels.torq[i]->setFrameShape(QFrame::Panel);
	}
}

void MainWindow::setupSpinBoxes(int start_idx, int count, QLayout* form_layout)
{
	for(int i = start_idx; i < start_idx + count; i++) {
		joint_pos_cmd_spinboxes[i] = new QDoubleSpinBox(this);
		joint_pos_cmd_spinboxes[i]->setRange(-3.14, 3.14);  // -π to π radians
		joint_pos_cmd_spinboxes[i]->setDecimals(2);
		joint_pos_cmd_spinboxes[i]->setSingleStep(0.01);
		joint_pos_cmd_spinboxes[i]->setValue(0.0);
		
		// Add to existing row in form layout (right side only)
		// Assuming the labels already exist, we just add the spinbox to the right
		QFormLayout* formLayout = qobject_cast<QFormLayout*>(form_layout);
		if (formLayout) {
			// Get the existing label from the row
			QLabel* existingLabel = qobject_cast<QLabel*>(formLayout->itemAt(i - start_idx, QFormLayout::LabelRole)->widget());
			if (existingLabel) {
				formLayout->setWidget(i - start_idx, QFormLayout::FieldRole, joint_pos_cmd_spinboxes[i]);
			}
		}
	}
}

void MainWindow::posModeSend(const float* position){
	for(int i = 0; i < 32; i++) {
		joint_pos_cmd_spinboxes[i]->setValue(position[i]);
	}
}

void MainWindow::gwIkTestmodeSend(){
	p73_msgs::msg::IKTaskCmd ik_taskmode;
	ik_taskmode.ik_mode = true;
	
	// Get the actual value (0, 22, or 29) instead of index (0, 1, 2)
	ik_taskmode.target_link = ui_->ikTarget->currentData().toInt();
	
	ik_taskmode.target_pos = {ui_->ikX->value(), ui_->ikY->value(), ui_->ikZ->value()};
	ik_taskmode.traj_time = ui_->ikTrajTime->value();
	iktask_mode_pub_->publish(ik_taskmode);
}