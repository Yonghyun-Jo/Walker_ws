// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from p73_msgs:msg/TaskCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/task_cmd.hpp"


#ifndef P73_MSGS__MSG__DETAIL__TASK_CMD__BUILDER_HPP_
#define P73_MSGS__MSG__DETAIL__TASK_CMD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "p73_msgs/msg/detail/task_cmd__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace p73_msgs
{

namespace msg
{

namespace builder
{

class Init_TaskCmd_task_mode
{
public:
  Init_TaskCmd_task_mode()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::p73_msgs::msg::TaskCmd task_mode(::p73_msgs::msg::TaskCmd::_task_mode_type arg)
  {
    msg_.task_mode = std::move(arg);
    return std::move(msg_);
  }

private:
  ::p73_msgs::msg::TaskCmd msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::p73_msgs::msg::TaskCmd>()
{
  return p73_msgs::msg::builder::Init_TaskCmd_task_mode();
}

}  // namespace p73_msgs

#endif  // P73_MSGS__MSG__DETAIL__TASK_CMD__BUILDER_HPP_
