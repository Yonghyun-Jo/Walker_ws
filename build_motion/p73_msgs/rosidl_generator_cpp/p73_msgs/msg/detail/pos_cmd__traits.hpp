// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from p73_msgs:msg/PosCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/pos_cmd.hpp"


#ifndef P73_MSGS__MSG__DETAIL__POS_CMD__TRAITS_HPP_
#define P73_MSGS__MSG__DETAIL__POS_CMD__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "p73_msgs/msg/detail/pos_cmd__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace p73_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const PosCmd & msg,
  std::ostream & out)
{
  out << "{";
  // member: position
  {
    if (msg.position.size() == 0) {
      out << "position: []";
    } else {
      out << "position: [";
      size_t pending_items = msg.position.size();
      for (auto item : msg.position) {
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
    out << ", ";
  }

  // member: gravity
  {
    out << "gravity: ";
    rosidl_generator_traits::value_to_yaml(msg.gravity, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const PosCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: position
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.position.size() == 0) {
      out << "position: []\n";
    } else {
      out << "position:\n";
      for (auto item : msg.position) {
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

  // member: gravity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "gravity: ";
    rosidl_generator_traits::value_to_yaml(msg.gravity, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const PosCmd & msg, bool use_flow_style = false)
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
  const p73_msgs::msg::PosCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  p73_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use p73_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const p73_msgs::msg::PosCmd & msg)
{
  return p73_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<p73_msgs::msg::PosCmd>()
{
  return "p73_msgs::msg::PosCmd";
}

template<>
inline const char * name<p73_msgs::msg::PosCmd>()
{
  return "p73_msgs/msg/PosCmd";
}

template<>
struct has_fixed_size<p73_msgs::msg::PosCmd>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<p73_msgs::msg::PosCmd>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<p73_msgs::msg::PosCmd>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // P73_MSGS__MSG__DETAIL__POS_CMD__TRAITS_HPP_
