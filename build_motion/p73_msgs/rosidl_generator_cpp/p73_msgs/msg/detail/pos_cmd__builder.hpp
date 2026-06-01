// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from p73_msgs:msg/PosCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/pos_cmd.hpp"


#ifndef P73_MSGS__MSG__DETAIL__POS_CMD__BUILDER_HPP_
#define P73_MSGS__MSG__DETAIL__POS_CMD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "p73_msgs/msg/detail/pos_cmd__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace p73_msgs
{

namespace msg
{

namespace builder
{

class Init_PosCmd_gravity
{
public:
  explicit Init_PosCmd_gravity(::p73_msgs::msg::PosCmd & msg)
  : msg_(msg)
  {}
  ::p73_msgs::msg::PosCmd gravity(::p73_msgs::msg::PosCmd::_gravity_type arg)
  {
    msg_.gravity = std::move(arg);
    return std::move(msg_);
  }

private:
  ::p73_msgs::msg::PosCmd msg_;
};

class Init_PosCmd_traj_time
{
public:
  explicit Init_PosCmd_traj_time(::p73_msgs::msg::PosCmd & msg)
  : msg_(msg)
  {}
  Init_PosCmd_gravity traj_time(::p73_msgs::msg::PosCmd::_traj_time_type arg)
  {
    msg_.traj_time = std::move(arg);
    return Init_PosCmd_gravity(msg_);
  }

private:
  ::p73_msgs::msg::PosCmd msg_;
};

class Init_PosCmd_position
{
public:
  Init_PosCmd_position()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_PosCmd_traj_time position(::p73_msgs::msg::PosCmd::_position_type arg)
  {
    msg_.position = std::move(arg);
    return Init_PosCmd_traj_time(msg_);
  }

private:
  ::p73_msgs::msg::PosCmd msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::p73_msgs::msg::PosCmd>()
{
  return p73_msgs::msg::builder::Init_PosCmd_position();
}

}  // namespace p73_msgs

#endif  // P73_MSGS__MSG__DETAIL__POS_CMD__BUILDER_HPP_
