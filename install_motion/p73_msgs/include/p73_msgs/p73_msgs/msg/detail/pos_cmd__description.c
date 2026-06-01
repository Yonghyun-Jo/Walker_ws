// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from p73_msgs:msg/PosCmd.idl
// generated code does not contain a copyright notice

#include "p73_msgs/msg/detail/pos_cmd__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_p73_msgs
const rosidl_type_hash_t *
p73_msgs__msg__PosCmd__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x1c, 0xc4, 0x1a, 0x90, 0xf5, 0xe1, 0xb9, 0xd1,
      0x69, 0xb8, 0x0c, 0x38, 0x9e, 0x6e, 0x41, 0xfc,
      0xc7, 0x3c, 0x2c, 0xf3, 0x61, 0x3e, 0x8c, 0x98,
      0x11, 0xc8, 0xfa, 0xf4, 0x17, 0x56, 0x35, 0xc9,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char p73_msgs__msg__PosCmd__TYPE_NAME[] = "p73_msgs/msg/PosCmd";

// Define type names, field names, and default values
static char p73_msgs__msg__PosCmd__FIELD_NAME__position[] = "position";
static char p73_msgs__msg__PosCmd__FIELD_NAME__traj_time[] = "traj_time";
static char p73_msgs__msg__PosCmd__FIELD_NAME__gravity[] = "gravity";

static rosidl_runtime_c__type_description__Field p73_msgs__msg__PosCmd__FIELDS[] = {
  {
    {p73_msgs__msg__PosCmd__FIELD_NAME__position, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_ARRAY,
      32,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {p73_msgs__msg__PosCmd__FIELD_NAME__traj_time, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {p73_msgs__msg__PosCmd__FIELD_NAME__gravity, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
p73_msgs__msg__PosCmd__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {p73_msgs__msg__PosCmd__TYPE_NAME, 19, 19},
      {p73_msgs__msg__PosCmd__FIELDS, 3, 3},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "float64[32] position\n"
  "float64 traj_time\n"
  "bool gravity";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
p73_msgs__msg__PosCmd__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {p73_msgs__msg__PosCmd__TYPE_NAME, 19, 19},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 51, 51},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
p73_msgs__msg__PosCmd__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *p73_msgs__msg__PosCmd__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
