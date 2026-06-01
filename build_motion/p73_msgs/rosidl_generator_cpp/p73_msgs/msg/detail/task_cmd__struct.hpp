// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from p73_msgs:msg/TaskCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/task_cmd.hpp"


#ifndef P73_MSGS__MSG__DETAIL__TASK_CMD__STRUCT_HPP_
#define P73_MSGS__MSG__DETAIL__TASK_CMD__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__p73_msgs__msg__TaskCmd __attribute__((deprecated))
#else
# define DEPRECATED__p73_msgs__msg__TaskCmd __declspec(deprecated)
#endif

namespace p73_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct TaskCmd_
{
  using Type = TaskCmd_<ContainerAllocator>;

  explicit TaskCmd_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->task_mode = 0ul;
    }
  }

  explicit TaskCmd_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->task_mode = 0ul;
    }
  }

  // field types and members
  using _task_mode_type =
    uint32_t;
  _task_mode_type task_mode;

  // setters for named parameter idiom
  Type & set__task_mode(
    const uint32_t & _arg)
  {
    this->task_mode = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    p73_msgs::msg::TaskCmd_<ContainerAllocator> *;
  using ConstRawPtr =
    const p73_msgs::msg::TaskCmd_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<p73_msgs::msg::TaskCmd_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<p73_msgs::msg::TaskCmd_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      p73_msgs::msg::TaskCmd_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<p73_msgs::msg::TaskCmd_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      p73_msgs::msg::TaskCmd_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<p73_msgs::msg::TaskCmd_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<p73_msgs::msg::TaskCmd_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<p73_msgs::msg::TaskCmd_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__p73_msgs__msg__TaskCmd
    std::shared_ptr<p73_msgs::msg::TaskCmd_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__p73_msgs__msg__TaskCmd
    std::shared_ptr<p73_msgs::msg::TaskCmd_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const TaskCmd_ & other) const
  {
    if (this->task_mode != other.task_mode) {
      return false;
    }
    return true;
  }
  bool operator!=(const TaskCmd_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct TaskCmd_

// alias to use template instance with default allocator
using TaskCmd =
  p73_msgs::msg::TaskCmd_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace p73_msgs

#endif  // P73_MSGS__MSG__DETAIL__TASK_CMD__STRUCT_HPP_
