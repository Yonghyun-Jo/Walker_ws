#include "p73_gui/p73_gui.hpp"
#include "p73_gui/window.hpp"
#include <pluginlib/class_list_macros.hpp>
#include <QStringList>

namespace p73_gui {

P73Gui::P73Gui()
  : rqt_gui_cpp::Plugin()
  , widget_(nullptr)
  , main_window_(nullptr)
{
  setObjectName("P73Gui");
}

void P73Gui::initPlugin(qt_gui_cpp::PluginContext& context)
{
  // Create MainWindow instance
  main_window_ = new MainWindow();
  widget_ = main_window_;
  
  // Add widget to the user interface
  context.addWidget(widget_);
  
  // Set window title
  widget_->setWindowTitle("P73 GUI");
  
  // Setup ROS 2 spin timer to process callbacks
  connect(&ros_timer_, &QTimer::timeout, [this]() {
    rclcpp::spin_some(main_window_->get_node_base_interface());
  });
  ros_timer_.start(10);  // 1ms마다 ROS 콜백 처리
}

void P73Gui::shutdownPlugin()
{
  // Plugin shutdown
  if (main_window_) {
    // Clean up ROS 2 node properly
    delete main_window_;
    main_window_ = nullptr;
    widget_ = nullptr;
  }
}

// void P73Gui::saveSettings(qt_gui_cpp::Settings& plugin_settings,
//                           qt_gui_cpp::Settings& instance_settings) const
// {
//   // Save intrinsic configuration, usually using:
//   // instance_settings.setValue(k, v)
// }

// void P73Gui::restoreSettings(const qt_gui_cpp::Settings& plugin_settings,
//                              const qt_gui_cpp::Settings& instance_settings)
// {
//   // Restore intrinsic configuration, usually using:
//   // v = instance_settings.value(k)
// }

} // namespace p73_gui

// Export plugin
PLUGINLIB_EXPORT_CLASS(p73_gui::P73Gui, rqt_gui_cpp::Plugin)

