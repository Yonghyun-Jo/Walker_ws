// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from p73_msgs:msg/IKTaskCmd.idl
// generated code does not contain a copyright notice

#include "p73_msgs/msg/detail/ik_task_cmd__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_p73_msgs
const rosidl_type_hash_t *
p73_msgs__msg__IKTaskCmd__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x50, 0x04, 0x1f, 0x1e, 0x0c, 0x44, 0x50, 0x75,
      0xbf, 0xe4, 0x20, 0xc2, 0x78, 0xc4, 0x84, 0xc0,
      0xe1, 0xf8, 0x9f, 0x27, 0xd4, 0x9b, 0x0c, 0xa5,
      0x9d, 0xf7, 0xce, 0x51, 0x0a, 0x0c, 0x3b, 0x28,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char p73_msgs__msg__IKTaskCmd__TYPE_NAME[] = "p73_msgs/msg/IKTaskCmd";

// Define type names, field names, and default values
static char p73_msgs__msg__IKTaskCmd__FIELD_NAME__ik_mode[] = "ik_mode";
static char p73_msgs__msg__IKTaskCmd__FIELD_NAME__target_link[] = "target_link";
static char p73_msgs__msg__IKTaskCmd__FIELD_NAME__target_pos[] = "target_pos";
static char p73_msgs__msg__IKTaskCmd__FIELD_NAME__traj_time[] = "traj_time";

static rosidl_runtime_c__type_description__Field p73_msgs__msg__IKTaskCmd__FIELDS[] = {
  {
    {p73_msgs__msg__IKTaskCmd__FIELD_NAME__ik_mode, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {p73_msgs__msg__IKTaskCmd__FIELD_NAME__target_link, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {p73_msgs__msg__IKTaskCmd__FIELD_NAME__target_pos, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_ARRAY,
      3,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {p73_msgs__msg__IKTaskCmd__FIELD_NAME__traj_time, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
p73_msgs__msg__IKTaskCmd__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {p73_msgs__msg__IKTaskCmd__TYPE_NAME, 22, 22},
      {p73_msgs__msg__IKTaskCmd__FIELDS, 4, 4},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "bool ik_mode\n"
  "uint32 target_link\n"
  "float64[3] target_pos\n"
  "float64 traj_time";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
p73_msgs__msg__IKTaskCmd__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {p73_msgs__msg__IKTaskCmd__TYPE_NAME, 22, 22},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 71, 71},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
p73_msgs__msg__IKTaskCmd__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *p73_msgs__msg__IKTaskCmd__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
