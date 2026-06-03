// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from p73_msgs:msg/IKTaskCmd.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "p73_msgs/msg/detail/ik_task_cmd__struct.h"
#include "p73_msgs/msg/detail/ik_task_cmd__functions.h"

#include "rosidl_runtime_c/primitives_sequence.h"
#include "rosidl_runtime_c/primitives_sequence_functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool p73_msgs__msg__ik_task_cmd__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[36];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("p73_msgs.msg._ik_task_cmd.IKTaskCmd", full_classname_dest, 35) == 0);
  }
  p73_msgs__msg__IKTaskCmd * ros_message = _ros_message;
  {  // ik_mode
    PyObject * field = PyObject_GetAttrString(_pymsg, "ik_mode");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->ik_mode = (Py_True == field);
    Py_DECREF(field);
  }
  {  // target_link
    PyObject * field = PyObject_GetAttrString(_pymsg, "target_link");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->target_link = PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // target_pos
    PyObject * field = PyObject_GetAttrString(_pymsg, "target_pos");
    if (!field) {
      return false;
    }
    {
      // TODO(dirk-thomas) use a better way to check the type before casting
      assert(field->ob_type != NULL);
      assert(field->ob_type->tp_name != NULL);
      assert(strcmp(field->ob_type->tp_name, "numpy.ndarray") == 0);
      PyArrayObject * seq_field = (PyArrayObject *)field;
      Py_INCREF(seq_field);
      assert(PyArray_NDIM(seq_field) == 1);
      assert(PyArray_TYPE(seq_field) == NPY_FLOAT64);
      Py_ssize_t size = 3;
      double * dest = ros_message->target_pos;
      for (Py_ssize_t i = 0; i < size; ++i) {
        double tmp = *(npy_float64 *)PyArray_GETPTR1(seq_field, i);
        memcpy(&dest[i], &tmp, sizeof(double));
      }
      Py_DECREF(seq_field);
    }
    Py_DECREF(field);
  }
  {  // traj_time
    PyObject * field = PyObject_GetAttrString(_pymsg, "traj_time");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->traj_time = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * p73_msgs__msg__ik_task_cmd__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of IKTaskCmd */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("p73_msgs.msg._ik_task_cmd");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "IKTaskCmd");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  p73_msgs__msg__IKTaskCmd * ros_message = (p73_msgs__msg__IKTaskCmd *)raw_ros_message;
  {  // ik_mode
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->ik_mode ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ik_mode", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // target_link
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->target_link);
    {
      int rc = PyObject_SetAttrString(_pymessage, "target_link", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // target_pos
    PyObject * field = NULL;
    field = PyObject_GetAttrString(_pymessage, "target_pos");
    if (!field) {
      return NULL;
    }
    assert(field->ob_type != NULL);
    assert(field->ob_type->tp_name != NULL);
    assert(strcmp(field->ob_type->tp_name, "numpy.ndarray") == 0);
    PyArrayObject * seq_field = (PyArrayObject *)field;
    assert(PyArray_NDIM(seq_field) == 1);
    assert(PyArray_TYPE(seq_field) == NPY_FLOAT64);
    assert(sizeof(npy_float64) == sizeof(double));
    npy_float64 * dst = (npy_float64 *)PyArray_GETPTR1(seq_field, 0);
    double * src = &(ros_message->target_pos[0]);
    memcpy(dst, src, 3 * sizeof(double));
    Py_DECREF(field);
  }
  {  // traj_time
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->traj_time);
    {
      int rc = PyObject_SetAttrString(_pymessage, "traj_time", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
