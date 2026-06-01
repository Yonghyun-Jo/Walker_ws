// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from p73_msgs:msg/IKTaskCmd.idl
// generated code does not contain a copyright notice
#include "p73_msgs/msg/detail/ik_task_cmd__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
p73_msgs__msg__IKTaskCmd__init(p73_msgs__msg__IKTaskCmd * msg)
{
  if (!msg) {
    return false;
  }
  // ik_mode
  // target_link
  // target_pos
  // traj_time
  return true;
}

void
p73_msgs__msg__IKTaskCmd__fini(p73_msgs__msg__IKTaskCmd * msg)
{
  if (!msg) {
    return;
  }
  // ik_mode
  // target_link
  // target_pos
  // traj_time
}

bool
p73_msgs__msg__IKTaskCmd__are_equal(const p73_msgs__msg__IKTaskCmd * lhs, const p73_msgs__msg__IKTaskCmd * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // ik_mode
  if (lhs->ik_mode != rhs->ik_mode) {
    return false;
  }
  // target_link
  if (lhs->target_link != rhs->target_link) {
    return false;
  }
  // target_pos
  for (size_t i = 0; i < 3; ++i) {
    if (lhs->target_pos[i] != rhs->target_pos[i]) {
      return false;
    }
  }
  // traj_time
  if (lhs->traj_time != rhs->traj_time) {
    return false;
  }
  return true;
}

bool
p73_msgs__msg__IKTaskCmd__copy(
  const p73_msgs__msg__IKTaskCmd * input,
  p73_msgs__msg__IKTaskCmd * output)
{
  if (!input || !output) {
    return false;
  }
  // ik_mode
  output->ik_mode = input->ik_mode;
  // target_link
  output->target_link = input->target_link;
  // target_pos
  for (size_t i = 0; i < 3; ++i) {
    output->target_pos[i] = input->target_pos[i];
  }
  // traj_time
  output->traj_time = input->traj_time;
  return true;
}

p73_msgs__msg__IKTaskCmd *
p73_msgs__msg__IKTaskCmd__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  p73_msgs__msg__IKTaskCmd * msg = (p73_msgs__msg__IKTaskCmd *)allocator.allocate(sizeof(p73_msgs__msg__IKTaskCmd), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(p73_msgs__msg__IKTaskCmd));
  bool success = p73_msgs__msg__IKTaskCmd__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
p73_msgs__msg__IKTaskCmd__destroy(p73_msgs__msg__IKTaskCmd * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    p73_msgs__msg__IKTaskCmd__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
p73_msgs__msg__IKTaskCmd__Sequence__init(p73_msgs__msg__IKTaskCmd__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  p73_msgs__msg__IKTaskCmd * data = NULL;

  if (size) {
    data = (p73_msgs__msg__IKTaskCmd *)allocator.zero_allocate(size, sizeof(p73_msgs__msg__IKTaskCmd), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = p73_msgs__msg__IKTaskCmd__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        p73_msgs__msg__IKTaskCmd__fini(&data[i - 1]);
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
p73_msgs__msg__IKTaskCmd__Sequence__fini(p73_msgs__msg__IKTaskCmd__Sequence * array)
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
      p73_msgs__msg__IKTaskCmd__fini(&array->data[i]);
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

p73_msgs__msg__IKTaskCmd__Sequence *
p73_msgs__msg__IKTaskCmd__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  p73_msgs__msg__IKTaskCmd__Sequence * array = (p73_msgs__msg__IKTaskCmd__Sequence *)allocator.allocate(sizeof(p73_msgs__msg__IKTaskCmd__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = p73_msgs__msg__IKTaskCmd__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
p73_msgs__msg__IKTaskCmd__Sequence__destroy(p73_msgs__msg__IKTaskCmd__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    p73_msgs__msg__IKTaskCmd__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
p73_msgs__msg__IKTaskCmd__Sequence__are_equal(const p73_msgs__msg__IKTaskCmd__Sequence * lhs, const p73_msgs__msg__IKTaskCmd__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!p73_msgs__msg__IKTaskCmd__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
p73_msgs__msg__IKTaskCmd__Sequence__copy(
  const p73_msgs__msg__IKTaskCmd__Sequence * input,
  p73_msgs__msg__IKTaskCmd__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(p73_msgs__msg__IKTaskCmd);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    p73_msgs__msg__IKTaskCmd * data =
      (p73_msgs__msg__IKTaskCmd *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!p73_msgs__msg__IKTaskCmd__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          p73_msgs__msg__IKTaskCmd__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!p73_msgs__msg__IKTaskCmd__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
