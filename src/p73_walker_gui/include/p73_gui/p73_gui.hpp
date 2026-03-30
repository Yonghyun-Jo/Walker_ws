#pragma once

#include <rqt_gui_cpp/plugin.h>
#include <QWidget>
#include <QTimer>
#include <memory>

class MainWindow;

namespace p73_gui {

class P73Gui : public rqt_gui_cpp::Plugin
{
  Q_OBJECT

public:
  P73Gui();
  
  virtual void initPlugin(qt_gui_cpp::PluginContext& context) override;
  virtual void shutdownPlugin() override;
  // virtual void saveSettings(qt_gui_cpp::Settings& plugin_settings,
  //                          qt_gui_cpp::Settings& instance_settings) const override;
  // virtual void restoreSettings(const qt_gui_cpp::Settings& plugin_settings,
  //                             const qt_gui_cpp::Settings& instance_settings) override;

private:
  QWidget* widget_;
  MainWindow* main_window_;
  QTimer ros_timer_;  // ROS 2 spin timer
};

} // namespace p73_gui

