// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from p73_msgs:msg/IKTaskCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/ik_task_cmd.hpp"


#ifndef P73_MSGS__MSG__DETAIL__IK_TASK_CMD__BUILDER_HPP_
#define P73_MSGS__MSG__DETAIL__IK_TASK_CMD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "p73_msgs/msg/detail/ik_task_cmd__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace p73_msgs
{

namespace msg
{

namespace builder
{

class Init_IKTaskCmd_traj_time
{
public:
  explicit Init_IKTaskCmd_traj_time(::p73_msgs::msg::IKTaskCmd & msg)
  : msg_(msg)
  {}
  ::p73_msgs::msg::IKTaskCmd traj_time(::p73_msgs::msg::IKTaskCmd::_traj_time_type arg)
  {
    msg_.traj_time = std::move(arg);
    return std::move(msg_);
  }

private:
  ::p73_msgs::msg::IKTaskCmd msg_;
};

class Init_IKTaskCmd_target_pos
{
public:
  explicit Init_IKTaskCmd_target_pos(::p73_msgs::msg::IKTaskCmd & msg)
  : msg_(msg)
  {}
  Init_IKTaskCmd_traj_time target_pos(::p73_msgs::msg::IKTaskCmd::_target_pos_type arg)
  {
    msg_.target_pos = std::move(arg);
    return Init_IKTaskCmd_traj_time(msg_);
  }

private:
  ::p73_msgs::msg::IKTaskCmd msg_;
};

class Init_IKTaskCmd_target_link
{
public:
  explicit Init_IKTaskCmd_target_link(::p73_msgs::msg::IKTaskCmd & msg)
  : msg_(msg)
  {}
  Init_IKTaskCmd_target_pos target_link(::p73_msgs::msg::IKTaskCmd::_target_link_type arg)
  {
    msg_.target_link = std::move(arg);
    return Init_IKTaskCmd_target_pos(msg_);
  }

private:
  ::p73_msgs::msg::IKTaskCmd msg_;
};

class Init_IKTaskCmd_ik_mode
{
public:
  Init_IKTaskCmd_ik_mode()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_IKTaskCmd_target_link ik_mode(::p73_msgs::msg::IKTaskCmd::_ik_mode_type arg)
  {
    msg_.ik_mode = std::move(arg);
    return Init_IKTaskCmd_target_link(msg_);
  }

private:
  ::p73_msgs::msg::IKTaskCmd msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::p73_msgs::msg::IKTaskCmd>()
{
  return p73_msgs::msg::builder::Init_IKTaskCmd_ik_mode();
}

}  // namespace p73_msgs

#endif  // P73_MSGS__MSG__DETAIL__IK_TASK_CMD__BUILDER_HPP_
