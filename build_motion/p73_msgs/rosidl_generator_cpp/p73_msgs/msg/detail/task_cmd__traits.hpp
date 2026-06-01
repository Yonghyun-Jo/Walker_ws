// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from p73_msgs:msg/TaskCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/task_cmd.hpp"


#ifndef P73_MSGS__MSG__DETAIL__TASK_CMD__TRAITS_HPP_
#define P73_MSGS__MSG__DETAIL__TASK_CMD__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "p73_msgs/msg/detail/task_cmd__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace p73_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const TaskCmd & msg,
  std::ostream & out)
{
  out << "{";
  // member: task_mode
  {
    out << "task_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.task_mode, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const TaskCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: task_mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "task_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.task_mode, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const TaskCmd & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace p73_msgs

namespace rosidl_generator_traits
{

[[deprecated("use p73_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const p73_msgs::msg::TaskCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  p73_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use p73_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const p73_msgs::msg::TaskCmd & msg)
{
  return p73_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<p73_msgs::msg::TaskCmd>()
{
  return "p73_msgs::msg::TaskCmd";
}

template<>
inline const char * name<p73_msgs::msg::TaskCmd>()
{
  return "p73_msgs/msg/TaskCmd";
}

template<>
struct has_fixed_size<p73_msgs::msg::TaskCmd>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<p73_msgs::msg::TaskCmd>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<p73_msgs::msg::TaskCmd>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // P73_MSGS__MSG__DETAIL__TASK_CMD__TRAITS_HPP_
