// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from p73_msgs:msg/PosCmd.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "p73_msgs/msg/detail/pos_cmd__functions.h"
#include "p73_msgs/msg/detail/pos_cmd__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace p73_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void PosCmd_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) p73_msgs::msg::PosCmd(_init);
}

void PosCmd_fini_function(void * message_memory)
{
  auto typed_message = static_cast<p73_msgs::msg::PosCmd *>(message_memory);
  typed_message->~PosCmd();
}

size_t size_function__PosCmd__position(const void * untyped_member)
{
  (void)untyped_member;
  return 32;
}

const void * get_const_function__PosCmd__position(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<double, 32> *>(untyped_member);
  return &member[index];
}

void * get_function__PosCmd__position(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<double, 32> *>(untyped_member);
  return &member[index];
}

void fetch_function__PosCmd__position(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__PosCmd__position(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__PosCmd__position(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__PosCmd__position(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember PosCmd_message_member_array[3] = {
  {
    "position",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    32,  // array size
    false,  // is upper bound
    offsetof(p73_msgs::msg::PosCmd, position),  // bytes offset in struct
    nullptr,  // default value
    size_function__PosCmd__position,  // size() function pointer
    get_const_function__PosCmd__position,  // get_const(index) function pointer
    get_function__PosCmd__position,  // get(index) function pointer
    fetch_function__PosCmd__position,  // fetch(index, &value) function pointer
    assign_function__PosCmd__position,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "traj_time",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(p73_msgs::msg::PosCmd, traj_time),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "gravity",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(p73_msgs::msg::PosCmd, gravity),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers PosCmd_message_members = {
  "p73_msgs::msg",  // message namespace
  "PosCmd",  // message name
  3,  // number of fields
  sizeof(p73_msgs::msg::PosCmd),
  false,  // has_any_key_member_
  PosCmd_message_member_array,  // message members
  PosCmd_init_function,  // function to initialize message memory (memory has to be allocated)
  PosCmd_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t PosCmd_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &PosCmd_message_members,
  get_message_typesupport_handle_function,
  &p73_msgs__msg__PosCmd__get_type_hash,
  &p73_msgs__msg__PosCmd__get_type_description,
  &p73_msgs__msg__PosCmd__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace p73_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<p73_msgs::msg::PosCmd>()
{
  return &::p73_msgs::msg::rosidl_typesupport_introspection_cpp::PosCmd_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, p73_msgs, msg, PosCmd)() {
  return &::p73_msgs::msg::rosidl_typesupport_introspection_cpp::PosCmd_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
