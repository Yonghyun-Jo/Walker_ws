// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from p73_msgs:msg/PosCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/pos_cmd.h"


#ifndef P73_MSGS__MSG__DETAIL__POS_CMD__STRUCT_H_
#define P73_MSGS__MSG__DETAIL__POS_CMD__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/PosCmd in the package p73_msgs.
typedef struct p73_msgs__msg__PosCmd
{
  double position[32];
  double traj_time;
  bool gravity;
} p73_msgs__msg__PosCmd;

// Struct for a sequence of p73_msgs__msg__PosCmd.
typedef struct p73_msgs__msg__PosCmd__Sequence
{
  p73_msgs__msg__PosCmd * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} p73_msgs__msg__PosCmd__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // P73_MSGS__MSG__DETAIL__POS_CMD__STRUCT_H_
