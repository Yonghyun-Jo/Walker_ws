// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from p73_msgs:msg/PosCmd.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "p73_msgs/msg/detail/pos_cmd__rosidl_typesupport_introspection_c.h"
#include "p73_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "p73_msgs/msg/detail/pos_cmd__functions.h"
#include "p73_msgs/msg/detail/pos_cmd__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  p73_msgs__msg__PosCmd__init(message_memory);
}

void p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_fini_function(void * message_memory)
{
  p73_msgs__msg__PosCmd__fini(message_memory);
}

size_t p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__size_function__PosCmd__position(
  const void * untyped_member)
{
  (void)untyped_member;
  return 32;
}

const void * p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__get_const_function__PosCmd__position(
  const void * untyped_member, size_t index)
{
  const double * member =
    (const double *)(untyped_member);
  return &member[index];
}

void * p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__get_function__PosCmd__position(
  void * untyped_member, size_t index)
{
  double * member =
    (double *)(untyped_member);
  return &member[index];
}

void p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__fetch_function__PosCmd__position(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__get_const_function__PosCmd__position(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__assign_function__PosCmd__position(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__get_function__PosCmd__position(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_message_member_array[3] = {
  {
    "position",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    32,  // array size
    false,  // is upper bound
    offsetof(p73_msgs__msg__PosCmd, position),  // bytes offset in struct
    NULL,  // default value
    p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__size_function__PosCmd__position,  // size() function pointer
    p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__get_const_function__PosCmd__position,  // get_const(index) function pointer
    p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__get_function__PosCmd__position,  // get(index) function pointer
    p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__fetch_function__PosCmd__position,  // fetch(index, &value) function pointer
    p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__assign_function__PosCmd__position,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "traj_time",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(p73_msgs__msg__PosCmd, traj_time),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "gravity",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(p73_msgs__msg__PosCmd, gravity),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_message_members = {
  "p73_msgs__msg",  // message namespace
  "PosCmd",  // message name
  3,  // number of fields
  sizeof(p73_msgs__msg__PosCmd),
  false,  // has_any_key_member_
  p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_message_member_array,  // message members
  p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_init_function,  // function to initialize message memory (memory has to be allocated)
  p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_message_type_support_handle = {
  0,
  &p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_message_members,
  get_message_typesupport_handle_function,
  &p73_msgs__msg__PosCmd__get_type_hash,
  &p73_msgs__msg__PosCmd__get_type_description,
  &p73_msgs__msg__PosCmd__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_p73_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, p73_msgs, msg, PosCmd)() {
  if (!p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_message_type_support_handle.typesupport_identifier) {
    p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &p73_msgs__msg__PosCmd__rosidl_typesupport_introspection_c__PosCmd_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
