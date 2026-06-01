// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from p73_msgs:msg/PosCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/pos_cmd.hpp"


#ifndef P73_MSGS__MSG__DETAIL__POS_CMD__STRUCT_HPP_
#define P73_MSGS__MSG__DETAIL__POS_CMD__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__p73_msgs__msg__PosCmd __attribute__((deprecated))
#else
# define DEPRECATED__p73_msgs__msg__PosCmd __declspec(deprecated)
#endif

namespace p73_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct PosCmd_
{
  using Type = PosCmd_<ContainerAllocator>;

  explicit PosCmd_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<double, 32>::iterator, double>(this->position.begin(), this->position.end(), 0.0);
      this->traj_time = 0.0;
      this->gravity = false;
    }
  }

  explicit PosCmd_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : position(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<double, 32>::iterator, double>(this->position.begin(), this->position.end(), 0.0);
      this->traj_time = 0.0;
      this->gravity = false;
    }
  }

  // field types and members
  using _position_type =
    std::array<double, 32>;
  _position_type position;
  using _traj_time_type =
    double;
  _traj_time_type traj_time;
  using _gravity_type =
    bool;
  _gravity_type gravity;

  // setters for named parameter idiom
  Type & set__position(
    const std::array<double, 32> & _arg)
  {
    this->position = _arg;
    return *this;
  }
  Type & set__traj_time(
    const double & _arg)
  {
    this->traj_time = _arg;
    return *this;
  }
  Type & set__gravity(
    const bool & _arg)
  {
    this->gravity = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    p73_msgs::msg::PosCmd_<ContainerAllocator> *;
  using ConstRawPtr =
    const p73_msgs::msg::PosCmd_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<p73_msgs::msg::PosCmd_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<p73_msgs::msg::PosCmd_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      p73_msgs::msg::PosCmd_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<p73_msgs::msg::PosCmd_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      p73_msgs::msg::PosCmd_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<p73_msgs::msg::PosCmd_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<p73_msgs::msg::PosCmd_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<p73_msgs::msg::PosCmd_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__p73_msgs__msg__PosCmd
    std::shared_ptr<p73_msgs::msg::PosCmd_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__p73_msgs__msg__PosCmd
    std::shared_ptr<p73_msgs::msg::PosCmd_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const PosCmd_ & other) const
  {
    if (this->position != other.position) {
      return false;
    }
    if (this->traj_time != other.traj_time) {
      return false;
    }
    if (this->gravity != other.gravity) {
      return false;
    }
    return true;
  }
  bool operator!=(const PosCmd_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct PosCmd_

// alias to use template instance with default allocator
using PosCmd =
  p73_msgs::msg::PosCmd_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace p73_msgs

#endif  // P73_MSGS__MSG__DETAIL__POS_CMD__STRUCT_HPP_
