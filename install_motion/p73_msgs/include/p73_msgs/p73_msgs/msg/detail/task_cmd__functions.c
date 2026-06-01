// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from p73_msgs:msg/TaskCmd.idl
// generated code does not contain a copyright notice
#include "p73_msgs/msg/detail/task_cmd__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
p73_msgs__msg__TaskCmd__init(p73_msgs__msg__TaskCmd * msg)
{
  if (!msg) {
    return false;
  }
  // task_mode
  return true;
}

void
p73_msgs__msg__TaskCmd__fini(p73_msgs__msg__TaskCmd * msg)
{
  if (!msg) {
    return;
  }
  // task_mode
}

bool
p73_msgs__msg__TaskCmd__are_equal(const p73_msgs__msg__TaskCmd * lhs, const p73_msgs__msg__TaskCmd * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // task_mode
  if (lhs->task_mode != rhs->task_mode) {
    return false;
  }
  return true;
}

bool
p73_msgs__msg__TaskCmd__copy(
  const p73_msgs__msg__TaskCmd * input,
  p73_msgs__msg__TaskCmd * output)
{
  if (!input || !output) {
    return false;
  }
  // task_mode
  output->task_mode = input->task_mode;
  return true;
}

p73_msgs__msg__TaskCmd *
p73_msgs__msg__TaskCmd__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  p73_msgs__msg__TaskCmd * msg = (p73_msgs__msg__TaskCmd *)allocator.allocate(sizeof(p73_msgs__msg__TaskCmd), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(p73_msgs__msg__TaskCmd));
  bool success = p73_msgs__msg__TaskCmd__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
p73_msgs__msg__TaskCmd__destroy(p73_msgs__msg__TaskCmd * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    p73_msgs__msg__TaskCmd__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
p73_msgs__msg__TaskCmd__Sequence__init(p73_msgs__msg__TaskCmd__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  p73_msgs__msg__TaskCmd * data = NULL;

  if (size) {
    data = (p73_msgs__msg__TaskCmd *)allocator.zero_allocate(size, sizeof(p73_msgs__msg__TaskCmd), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = p73_msgs__msg__TaskCmd__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        p73_msgs__msg__TaskCmd__fini(&data[i - 1]);
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
p73_msgs__msg__TaskCmd__Sequence__fini(p73_msgs__msg__TaskCmd__Sequence * array)
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
      p73_msgs__msg__TaskCmd__fini(&array->data[i]);
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

p73_msgs__msg__TaskCmd__Sequence *
p73_msgs__msg__TaskCmd__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  p73_msgs__msg__TaskCmd__Sequence * array = (p73_msgs__msg__TaskCmd__Sequence *)allocator.allocate(sizeof(p73_msgs__msg__TaskCmd__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = p73_msgs__msg__TaskCmd__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
p73_msgs__msg__TaskCmd__Sequence__destroy(p73_msgs__msg__TaskCmd__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    p73_msgs__msg__TaskCmd__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
p73_msgs__msg__TaskCmd__Sequence__are_equal(const p73_msgs__msg__TaskCmd__Sequence * lhs, const p73_msgs__msg__TaskCmd__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!p73_msgs__msg__TaskCmd__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
p73_msgs__msg__TaskCmd__Sequence__copy(
  const p73_msgs__msg__TaskCmd__Sequence * input,
  p73_msgs__msg__TaskCmd__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(p73_msgs__msg__TaskCmd);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    p73_msgs__msg__TaskCmd * data =
      (p73_msgs__msg__TaskCmd *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!p73_msgs__msg__TaskCmd__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          p73_msgs__msg__TaskCmd__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!p73_msgs__msg__TaskCmd__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
