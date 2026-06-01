// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from p73_msgs:msg/IKTaskCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/ik_task_cmd.h"


#ifndef P73_MSGS__MSG__DETAIL__IK_TASK_CMD__STRUCT_H_
#define P73_MSGS__MSG__DETAIL__IK_TASK_CMD__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/IKTaskCmd in the package p73_msgs.
typedef struct p73_msgs__msg__IKTaskCmd
{
  bool ik_mode;
  uint32_t target_link;
  double target_pos[3];
  double traj_time;
} p73_msgs__msg__IKTaskCmd;

// Struct for a sequence of p73_msgs__msg__IKTaskCmd.
typedef struct p73_msgs__msg__IKTaskCmd__Sequence
{
  p73_msgs__msg__IKTaskCmd * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} p73_msgs__msg__IKTaskCmd__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // P73_MSGS__MSG__DETAIL__IK_TASK_CMD__STRUCT_H_
