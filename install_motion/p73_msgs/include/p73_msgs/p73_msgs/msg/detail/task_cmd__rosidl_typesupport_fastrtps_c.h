// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from p73_msgs:msg/TaskCmd.idl
// generated code does not contain a copyright notice
#ifndef P73_MSGS__MSG__DETAIL__TASK_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define P73_MSGS__MSG__DETAIL__TASK_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "p73_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "p73_msgs/msg/detail/task_cmd__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_p73_msgs
bool cdr_serialize_p73_msgs__msg__TaskCmd(
  const p73_msgs__msg__TaskCmd * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_p73_msgs
bool cdr_deserialize_p73_msgs__msg__TaskCmd(
  eprosima::fastcdr::Cdr &,
  p73_msgs__msg__TaskCmd * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_p73_msgs
size_t get_serialized_size_p73_msgs__msg__TaskCmd(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_p73_msgs
size_t max_serialized_size_p73_msgs__msg__TaskCmd(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_p73_msgs
bool cdr_serialize_key_p73_msgs__msg__TaskCmd(
  const p73_msgs__msg__TaskCmd * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_p73_msgs
size_t get_serialized_size_key_p73_msgs__msg__TaskCmd(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_p73_msgs
size_t max_serialized_size_key_p73_msgs__msg__TaskCmd(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_p73_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, p73_msgs, msg, TaskCmd)();

#ifdef __cplusplus
}
#endif

#endif  // P73_MSGS__MSG__DETAIL__TASK_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
