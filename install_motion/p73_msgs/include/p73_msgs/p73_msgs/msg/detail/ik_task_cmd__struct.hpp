// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from p73_msgs:msg/IKTaskCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "p73_msgs/msg/ik_task_cmd.hpp"


#ifndef P73_MSGS__MSG__DETAIL__IK_TASK_CMD__STRUCT_HPP_
#define P73_MSGS__MSG__DETAIL__IK_TASK_CMD__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__p73_msgs__msg__IKTaskCmd __attribute__((deprecated))
#else
# define DEPRECATED__p73_msgs__msg__IKTaskCmd __declspec(deprecated)
#endif

namespace p73_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct IKTaskCmd_
{
  using Type = IKTaskCmd_<ContainerAllocator>;

  explicit IKTaskCmd_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ik_mode = false;
      this->target_link = 0ul;
      std::fill<typename std::array<double, 3>::iterator, double>(this->target_pos.begin(), this->target_pos.end(), 0.0);
      this->traj_time = 0.0;
    }
  }

  explicit IKTaskCmd_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : target_pos(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->ik_mode = false;
      this->target_link = 0ul;
      std::fill<typename std::array<double, 3>::iterator, double>(this->target_pos.begin(), this->target_pos.end(), 0.0);
      this->traj_time = 0.0;
    }
  }

  // field types and members
  using _ik_mode_type =
    bool;
  _ik_mode_type ik_mode;
  using _target_link_type =
    uint32_t;
  _target_link_type target_link;
  using _target_pos_type =
    std::array<double, 3>;
  _target_pos_type target_pos;
  using _traj_time_type =
    double;
  _traj_time_type traj_time;

  // setters for named parameter idiom
  Type & set__ik_mode(
    const bool & _arg)
  {
    this->ik_mode = _arg;
    return *this;
  }
  Type & set__target_link(
    const uint32_t & _arg)
  {
    this->target_link = _arg;
    return *this;
  }
  Type & set__target_pos(
    const std::array<double, 3> & _arg)
  {
    this->target_pos = _arg;
    return *this;
  }
  Type & set__traj_time(
    const double & _arg)
  {
    this->traj_time = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    p73_msgs::msg::IKTaskCmd_<ContainerAllocator> *;
  using ConstRawPtr =
    const p73_msgs::msg::IKTaskCmd_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<p73_msgs::msg::IKTaskCmd_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<p73_msgs::msg::IKTaskCmd_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      p73_msgs::msg::IKTaskCmd_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<p73_msgs::msg::IKTaskCmd_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      p73_msgs::msg::IKTaskCmd_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<p73_msgs::msg::IKTaskCmd_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<p73_msgs::msg::IKTaskCmd_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<p73_msgs::msg::IKTaskCmd_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__p73_msgs__msg__IKTaskCmd
    std::shared_ptr<p73_msgs::msg::IKTaskCmd_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__p73_msgs__msg__IKTaskCmd
    std::shared_ptr<p73_msgs::msg::IKTaskCmd_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const IKTaskCmd_ & other) const
  {
    if (this->ik_mode != other.ik_mode) {
      return false;
    }
    if (this->target_link != other.target_link) {
      return false;
    }
    if (this->target_pos != other.target_pos) {
      return false;
    }
    if (this->traj_time != other.traj_time) {
      return false;
    }
    return true;
  }
  bool operator!=(const IKTaskCmd_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct IKTaskCmd_

// alias to use template instance with default allocator
using IKTaskCmd =
  p73_msgs::msg::IKTaskCmd_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace p73_msgs

#endif  // P73_MSGS__MSG__DETAIL__IK_TASK_CMD__STRUCT_HPP_
