// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from p73_msgs:msg/IKTaskCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/ik_task_cmd.hpp"


#ifndef P73_MSGS__MSG__DETAIL__IK_TASK_CMD__TRAITS_HPP_
#define P73_MSGS__MSG__DETAIL__IK_TASK_CMD__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "p73_msgs/msg/detail/ik_task_cmd__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace p73_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const IKTaskCmd & msg,
  std::ostream & out)
{
  out << "{";
  // member: ik_mode
  {
    out << "ik_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.ik_mode, out);
    out << ", ";
  }

  // member: target_link
  {
    out << "target_link: ";
    rosidl_generator_traits::value_to_yaml(msg.target_link, out);
    out << ", ";
  }

  // member: target_pos
  {
    if (msg.target_pos.size() == 0) {
      out << "target_pos: []";
    } else {
      out << "target_pos: [";
      size_t pending_items = msg.target_pos.size();
      for (auto item : msg.target_pos) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: traj_time
  {
    out << "traj_time: ";
    rosidl_generator_traits::value_to_yaml(msg.traj_time, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const IKTaskCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: ik_mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ik_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.ik_mode, out);
    out << "\n";
  }

  // member: target_link
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "target_link: ";
    rosidl_generator_traits::value_to_yaml(msg.target_link, out);
    out << "\n";
  }

  // member: target_pos
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.target_pos.size() == 0) {
      out << "target_pos: []\n";
    } else {
      out << "target_pos:\n";
      for (auto item : msg.target_pos) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: traj_time
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "traj_time: ";
    rosidl_generator_traits::value_to_yaml(msg.traj_time, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const IKTaskCmd & msg, bool use_flow_style = false)
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
  const p73_msgs::msg::IKTaskCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  p73_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use p73_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const p73_msgs::msg::IKTaskCmd & msg)
{
  return p73_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<p73_msgs::msg::IKTaskCmd>()
{
  return "p73_msgs::msg::IKTaskCmd";
}

template<>
inline const char * name<p73_msgs::msg::IKTaskCmd>()
{
  return "p73_msgs/msg/IKTaskCmd";
}

template<>
struct has_fixed_size<p73_msgs::msg::IKTaskCmd>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<p73_msgs::msg::IKTaskCmd>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<p73_msgs::msg::IKTaskCmd>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // P73_MSGS__MSG__DETAIL__IK_TASK_CMD__TRAITS_HPP_
