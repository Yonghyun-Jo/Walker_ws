// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from p73_msgs:msg/TaskCmd.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "p73_msgs/msg/detail/task_cmd__rosidl_typesupport_introspection_c.h"
#include "p73_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "p73_msgs/msg/detail/task_cmd__functions.h"
#include "p73_msgs/msg/detail/task_cmd__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  p73_msgs__msg__TaskCmd__init(message_memory);
}

void p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_fini_function(void * message_memory)
{
  p73_msgs__msg__TaskCmd__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_message_member_array[1] = {
  {
    "task_mode",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(p73_msgs__msg__TaskCmd, task_mode),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_message_members = {
  "p73_msgs__msg",  // message namespace
  "TaskCmd",  // message name
  1,  // number of fields
  sizeof(p73_msgs__msg__TaskCmd),
  false,  // has_any_key_member_
  p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_message_member_array,  // message members
  p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_init_function,  // function to initialize message memory (memory has to be allocated)
  p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_message_type_support_handle = {
  0,
  &p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_message_members,
  get_message_typesupport_handle_function,
  &p73_msgs__msg__TaskCmd__get_type_hash,
  &p73_msgs__msg__TaskCmd__get_type_description,
  &p73_msgs__msg__TaskCmd__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_p73_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, p73_msgs, msg, TaskCmd)() {
  if (!p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_message_type_support_handle.typesupport_identifier) {
    p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &p73_msgs__msg__TaskCmd__rosidl_typesupport_introspection_c__TaskCmd_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
