// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from p73_msgs:msg/TaskCmd.idl
// generated code does not contain a copyright notice

#include "p73_msgs/msg/detail/task_cmd__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_p73_msgs
const rosidl_type_hash_t *
p73_msgs__msg__TaskCmd__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xcb, 0x34, 0x83, 0x6c, 0x64, 0xf9, 0x0f, 0x32,
      0xf9, 0xfd, 0xe8, 0x8e, 0xc9, 0x42, 0xe9, 0xd8,
      0x0a, 0xf3, 0x87, 0x1d, 0x26, 0x86, 0x92, 0x65,
      0x6b, 0xaf, 0xa9, 0x96, 0x4f, 0x52, 0xee, 0xc9,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char p73_msgs__msg__TaskCmd__TYPE_NAME[] = "p73_msgs/msg/TaskCmd";

// Define type names, field names, and default values
static char p73_msgs__msg__TaskCmd__FIELD_NAME__task_mode[] = "task_mode";

static rosidl_runtime_c__type_description__Field p73_msgs__msg__TaskCmd__FIELDS[] = {
  {
    {p73_msgs__msg__TaskCmd__FIELD_NAME__task_mode, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
p73_msgs__msg__TaskCmd__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {p73_msgs__msg__TaskCmd__TYPE_NAME, 20, 20},
      {p73_msgs__msg__TaskCmd__FIELDS, 1, 1},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint32 task_mode";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
p73_msgs__msg__TaskCmd__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {p73_msgs__msg__TaskCmd__TYPE_NAME, 20, 20},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 16, 16},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
p73_msgs__msg__TaskCmd__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *p73_msgs__msg__TaskCmd__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
