// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from p73_msgs:msg/PosCmd.idl
// generated code does not contain a copyright notice
#include "p73_msgs/msg/detail/pos_cmd__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
p73_msgs__msg__PosCmd__init(p73_msgs__msg__PosCmd * msg)
{
  if (!msg) {
    return false;
  }
  // position
  // traj_time
  // gravity
  return true;
}

void
p73_msgs__msg__PosCmd__fini(p73_msgs__msg__PosCmd * msg)
{
  if (!msg) {
    return;
  }
  // position
  // traj_time
  // gravity
}

bool
p73_msgs__msg__PosCmd__are_equal(const p73_msgs__msg__PosCmd * lhs, const p73_msgs__msg__PosCmd * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // position
  for (size_t i = 0; i < 32; ++i) {
    if (lhs->position[i] != rhs->position[i]) {
      return false;
    }
  }
  // traj_time
  if (lhs->traj_time != rhs->traj_time) {
    return false;
  }
  // gravity
  if (lhs->gravity != rhs->gravity) {
    return false;
  }
  return true;
}

bool
p73_msgs__msg__PosCmd__copy(
  const p73_msgs__msg__PosCmd * input,
  p73_msgs__msg__PosCmd * output)
{
  if (!input || !output) {
    return false;
  }
  // position
  for (size_t i = 0; i < 32; ++i) {
    output->position[i] = input->position[i];
  }
  // traj_time
  output->traj_time = input->traj_time;
  // gravity
  output->gravity = input->gravity;
  return true;
}

p73_msgs__msg__PosCmd *
p73_msgs__msg__PosCmd__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  p73_msgs__msg__PosCmd * msg = (p73_msgs__msg__PosCmd *)allocator.allocate(sizeof(p73_msgs__msg__PosCmd), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(p73_msgs__msg__PosCmd));
  bool success = p73_msgs__msg__PosCmd__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
p73_msgs__msg__PosCmd__destroy(p73_msgs__msg__PosCmd * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    p73_msgs__msg__PosCmd__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
p73_msgs__msg__PosCmd__Sequence__init(p73_msgs__msg__PosCmd__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  p73_msgs__msg__PosCmd * data = NULL;

  if (size) {
    data = (p73_msgs__msg__PosCmd *)allocator.zero_allocate(size, sizeof(p73_msgs__msg__PosCmd), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = p73_msgs__msg__PosCmd__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        p73_msgs__msg__PosCmd__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
p73_msgs__msg__PosCmd__Sequence__fini(p73_msgs__msg__PosCmd__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      p73_msgs__msg__PosCmd__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

p73_msgs__msg__PosCmd__Sequence *
p73_msgs__msg__PosCmd__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  p73_msgs__msg__PosCmd__Sequence * array = (p73_msgs__msg__PosCmd__Sequence *)allocator.allocate(sizeof(p73_msgs__msg__PosCmd__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = p73_msgs__msg__PosCmd__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
p73_msgs__msg__PosCmd__Sequence__destroy(p73_msgs__msg__PosCmd__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    p73_msgs__msg__PosCmd__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
p73_msgs__msg__PosCmd__Sequence__are_equal(const p73_msgs__msg__PosCmd__Sequence * lhs, const p73_msgs__msg__PosCmd__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!p73_msgs__msg__PosCmd__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
p73_msgs__msg__PosCmd__Sequence__copy(
  const p73_msgs__msg__PosCmd__Sequence * input,
  p73_msgs__msg__PosCmd__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(p73_msgs__msg__PosCmd);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    p73_msgs__msg__PosCmd * data =
      (p73_msgs__msg__PosCmd *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!p73_msgs__msg__PosCmd__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          p73_msgs__msg__PosCmd__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!p73_msgs__msg__PosCmd__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
