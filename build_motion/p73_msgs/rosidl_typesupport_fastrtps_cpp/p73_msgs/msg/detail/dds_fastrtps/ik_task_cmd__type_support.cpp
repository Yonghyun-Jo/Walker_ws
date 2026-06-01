// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from p73_msgs:msg/IKTaskCmd.idl
// generated code does not contain a copyright notice
#include "p73_msgs/msg/detail/ik_task_cmd__rosidl_typesupport_fastrtps_cpp.hpp"
#include "p73_msgs/msg/detail/ik_task_cmd__functions.h"
#include "p73_msgs/msg/detail/ik_task_cmd__struct.hpp"

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_fastrtps_cpp/identifier.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_fastrtps_cpp/serialization_helpers.hpp"
#include "rosidl_typesupport_fastrtps_cpp/wstring_conversion.hpp"
#include "fastcdr/Cdr.h"


// forward declaration of message dependencies and their conversion functions

namespace p73_msgs
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{


bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_p73_msgs
cdr_serialize(
  const p73_msgs::msg::IKTaskCmd & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: ik_mode
  cdr << (ros_message.ik_mode ? true : false);

  // Member: target_link
  cdr << ros_message.target_link;

  // Member: target_pos
  {
    cdr << ros_message.target_pos;
  }

  // Member: traj_time
  cdr << ros_message.traj_time;

  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_p73_msgs
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  p73_msgs::msg::IKTaskCmd & ros_message)
{
  // Member: ik_mode
  {
    uint8_t tmp;
    cdr >> tmp;
    ros_message.ik_mode = tmp ? true : false;
  }

  // Member: target_link
  cdr >> ros_message.target_link;

  // Member: target_pos
  {
    cdr >> ros_message.target_pos;
  }

  // Member: traj_time
  cdr >> ros_message.traj_time;

  return true;
}  // NOLINT(readability/fn_size)


size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_p73_msgs
get_serialized_size(
  const p73_msgs::msg::IKTaskCmd & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: ik_mode
  {
    size_t item_size = sizeof(ros_message.ik_mode);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: target_link
  {
    size_t item_size = sizeof(ros_message.target_link);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: target_pos
  {
    size_t array_size = 3;
    size_t item_size = sizeof(ros_message.target_pos[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: traj_time
  {
    size_t item_size = sizeof(ros_message.traj_time);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}


size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_p73_msgs
max_serialized_size_IKTaskCmd(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Member: ik_mode
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }
  // Member: target_link
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // Member: target_pos
  {
    size_t array_size = 3;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }
  // Member: traj_time
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = p73_msgs::msg::IKTaskCmd;
    is_plain =
      (
      offsetof(DataType, traj_time) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_p73_msgs
cdr_serialize_key(
  const p73_msgs::msg::IKTaskCmd & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: ik_mode
  cdr << (ros_message.ik_mode ? true : false);

  // Member: target_link
  cdr << ros_message.target_link;

  // Member: target_pos
  {
    cdr << ros_message.target_pos;
  }

  // Member: traj_time
  cdr << ros_message.traj_time;

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_p73_msgs
get_serialized_size_key(
  const p73_msgs::msg::IKTaskCmd & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: ik_mode
  {
    size_t item_size = sizeof(ros_message.ik_mode);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: target_link
  {
    size_t item_size = sizeof(ros_message.target_link);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: target_pos
  {
    size_t array_size = 3;
    size_t item_size = sizeof(ros_message.target_pos[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: traj_time
  {
    size_t item_size = sizeof(ros_message.traj_time);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_p73_msgs
max_serialized_size_key_IKTaskCmd(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Member: ik_mode
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: target_link
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: target_pos
  {
    size_t array_size = 3;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Member: traj_time
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = p73_msgs::msg::IKTaskCmd;
    is_plain =
      (
      offsetof(DataType, traj_time) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}


static bool _IKTaskCmd__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const p73_msgs::msg::IKTaskCmd *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _IKTaskCmd__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<p73_msgs::msg::IKTaskCmd *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _IKTaskCmd__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const p73_msgs::msg::IKTaskCmd *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _IKTaskCmd__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_IKTaskCmd(full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}

static message_type_support_callbacks_t _IKTaskCmd__callbacks = {
  "p73_msgs::msg",
  "IKTaskCmd",
  _IKTaskCmd__cdr_serialize,
  _IKTaskCmd__cdr_deserialize,
  _IKTaskCmd__get_serialized_size,
  _IKTaskCmd__max_serialized_size,
  nullptr
};

static rosidl_message_type_support_t _IKTaskCmd__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_IKTaskCmd__callbacks,
  get_message_typesupport_handle_function,
  &p73_msgs__msg__IKTaskCmd__get_type_hash,
  &p73_msgs__msg__IKTaskCmd__get_type_description,
  &p73_msgs__msg__IKTaskCmd__get_type_description_sources,
};

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace p73_msgs

namespace rosidl_typesupport_fastrtps_cpp
{

template<>
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_EXPORT_p73_msgs
const rosidl_message_type_support_t *
get_message_type_support_handle<p73_msgs::msg::IKTaskCmd>()
{
  return &p73_msgs::msg::typesupport_fastrtps_cpp::_IKTaskCmd__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, p73_msgs, msg, IKTaskCmd)() {
  return &p73_msgs::msg::typesupport_fastrtps_cpp::_IKTaskCmd__handle;
}

#ifdef __cplusplus
}
#endif
