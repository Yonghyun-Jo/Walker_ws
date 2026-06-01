# generated from rosidl_generator_py/resource/_idl.py.em
# with input from p73_msgs:msg/IKTaskCmd.idl
# generated code does not contain a copyright notice

# This is being done at the module level and not on the instance level to avoid looking
# for the same variable multiple times on each instance. This variable is not supposed to
# change during runtime so it makes sense to only look for it once.
from os import getenv

ros_python_check_fields = getenv('ROS_PYTHON_CHECK_FIELDS', default='')


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

# Member 'target_pos'
import numpy  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_IKTaskCmd(type):
    """Metaclass of message 'IKTaskCmd'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('p73_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'p73_msgs.msg.IKTaskCmd')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__ik_task_cmd
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__ik_task_cmd
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__ik_task_cmd
            cls._TYPE_SUPPORT = module.type_support_msg__msg__ik_task_cmd
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__ik_task_cmd

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class IKTaskCmd(metaclass=Metaclass_IKTaskCmd):
    """Message class 'IKTaskCmd'."""

    __slots__ = [
        '_ik_mode',
        '_target_link',
        '_target_pos',
        '_traj_time',
        '_check_fields',
    ]

    _fields_and_field_types = {
        'ik_mode': 'boolean',
        'target_link': 'uint32',
        'target_pos': 'double[3]',
        'traj_time': 'double',
    }

    # This attribute is used to store an rosidl_parser.definition variable
    # related to the data type of each of the components the message.
    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint32'),  # noqa: E501
        rosidl_parser.definition.Array(rosidl_parser.definition.BasicType('double'), 3),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        if 'check_fields' in kwargs:
            self._check_fields = kwargs['check_fields']
        else:
            self._check_fields = ros_python_check_fields == '1'
        if self._check_fields:
            assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
                'Invalid arguments passed to constructor: %s' % \
                ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.ik_mode = kwargs.get('ik_mode', bool())
        self.target_link = kwargs.get('target_link', int())
        if 'target_pos' not in kwargs:
            self.target_pos = numpy.zeros(3, dtype=numpy.float64)
        else:
            self.target_pos = kwargs.get('target_pos')
        self.traj_time = kwargs.get('traj_time', float())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.get_fields_and_field_types().keys(), self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    if self._check_fields:
                        assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.ik_mode != other.ik_mode:
            return False
        if self.target_link != other.target_link:
            return False
        if any(self.target_pos != other.target_pos):
            return False
        if self.traj_time != other.traj_time:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def ik_mode(self):
        """Message field 'ik_mode'."""
        return self._ik_mode

    @ik_mode.setter
    def ik_mode(self, value):
        if self._check_fields:
            assert \
                isinstance(value, bool), \
                "The 'ik_mode' field must be of type 'bool'"
        self._ik_mode = value

    @builtins.property
    def target_link(self):
        """Message field 'target_link'."""
        return self._target_link

    @target_link.setter
    def target_link(self, value):
        if self._check_fields:
            assert \
                isinstance(value, int), \
                "The 'target_link' field must be of type 'int'"
            assert value >= 0 and value < 4294967296, \
                "The 'target_link' field must be an unsigned integer in [0, 4294967295]"
        self._target_link = value

    @builtins.property
    def target_pos(self):
        """Message field 'target_pos'."""
        return self._target_pos

    @target_pos.setter
    def target_pos(self, value):
        if self._check_fields:
            if isinstance(value, numpy.ndarray):
                assert value.dtype == numpy.float64, \
                    "The 'target_pos' numpy.ndarray() must have the dtype of 'numpy.float64'"
                assert value.size == 3, \
                    "The 'target_pos' numpy.ndarray() must have a size of 3"
                self._target_pos = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 len(value) == 3 and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -1.7976931348623157e+308 or val > 1.7976931348623157e+308) or math.isinf(val) for val in value)), \
                "The 'target_pos' field must be a set or sequence with length 3 and each value of type 'float' and each double in [-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000, 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000]"
        self._target_pos = numpy.array(value, dtype=numpy.float64)

    @builtins.property
    def traj_time(self):
        """Message field 'traj_time'."""
        return self._traj_time

    @traj_time.setter
    def traj_time(self, value):
        if self._check_fields:
            assert \
                isinstance(value, float), \
                "The 'traj_time' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'traj_time' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._traj_time = value
