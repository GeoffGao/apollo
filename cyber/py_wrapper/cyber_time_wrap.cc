/******************************************************************************
 * Copyright 2019 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <Python.h>
#include <set>
#include <string>

#include "cyber/py_wrapper/py_time.h"

#define PYOBJECT_NULL_STRING PyString_FromStringAndSize("", 0)

template <typename T>
T PyObjectToPtr(PyObject* pyobj, const std::string& type_ptr) {
  T obj_ptr = (T)PyCapsule_GetPointer(pyobj, type_ptr.c_str());
  if (obj_ptr == nullptr) {
    AINFO << "PyObjectToPtr failed,type->" << type_ptr << "pyobj: " << pyobj;
  }
  return obj_ptr;
}

PyObject* cyber_new_PyTime(PyObject* self, PyObject* args) {
  uint64_t nanoseconds = 0;
  if (!PyArg_ParseTuple(args, const_cast<char*>("K:cyber_new_PyTime"),
                        &nanoseconds)) {
    AERROR << "cyber_new_PyTime parsetuple failed!";
    return Py_None;
  }

  apollo::cyber::PyTime* pytime = new apollo::cyber::PyTime(nanoseconds);
  PyObject* pyobj_time = PyCapsule_New(pytime, "apollo_cybertron_pytime", NULL);
  return pyobj_time;
}

PyObject* cyber_delete_PyTime(PyObject* self, PyObject* args) {
  PyObject* pyobj_time = nullptr;
  if (!PyArg_ParseTuple(args, const_cast<char*>("O:cyber_delete_PyTime"),
                        &pyobj_time)) {
    return Py_None;
  }

  auto pytime = (apollo::cyber::PyTime*)PyCapsule_GetPointer(
      pyobj_time, "apollo_cybertron_pytime");
  if (nullptr == pytime) {
    AINFO << "cyber_delete_PyTime:time ptr is null!";
    return Py_None;
  }
  delete pytime;
  return Py_None;
}

PyObject* cyber_PyTime_to_sec(PyObject* self, PyObject* args) {
  PyObject* pyobj_time = nullptr;
  if (!PyArg_ParseTuple(args, const_cast<char*>("O:cyber_PyTime_to_sec"),
                        &pyobj_time)) {
    AERROR << "cyber_PyTime_to_sec:PyArg_ParseTuple failed!";
    return PyFloat_FromDouble(0);
  }

  auto pytime = (apollo::cyber::PyTime*)PyCapsule_GetPointer(
      pyobj_time, "apollo_cybertron_pytime");
  if (nullptr == pytime) {
    AINFO << "cyber_PyTime_to_sec ptr is null!";
    return PyFloat_FromDouble(0);
  }

  double num = pytime->to_sec();
  return PyFloat_FromDouble(num);
}

PyObject* cyber_PyTime_to_nsec(PyObject* self, PyObject* args) {
  PyObject* pyobj_time = nullptr;
  if (!PyArg_ParseTuple(args, const_cast<char*>("O:cyber_PyTime_to_nsec"),
                        &pyobj_time)) {
    AERROR << "cyber_PyTime_to_nsec:PyArg_ParseTuple failed!";
    return PyLong_FromUnsignedLongLong(0);
  }

  auto pytime = (apollo::cyber::PyTime*)PyCapsule_GetPointer(
      pyobj_time, "apollo_cybertron_pytime");
  if (nullptr == pytime) {
    AINFO << "cyber_PyTime_to_nsec ptr is null!";
    return PyLong_FromUnsignedLongLong(0);
  }

  uint64_t num = pytime->to_nsec();
  return PyLong_FromUnsignedLongLong(num);
}

PyObject* cyber_PyTime_sleep_until(PyObject* self, PyObject* args) {
  PyObject* pyobj_time = nullptr;
  uint64_t nanoseconds = 0;
  if (!PyArg_ParseTuple(args, const_cast<char*>("OK:cyber_PyTime_sleep_until"),
                        &pyobj_time, &nanoseconds)) {
    AERROR << "cyber_PyTime_sleep_until:PyArg_ParseTuple failed!";
    return Py_None;
  }

  auto pytime = (apollo::cyber::PyTime*)PyCapsule_GetPointer(
      pyobj_time, "apollo_cybertron_pytime");
  if (nullptr == pytime) {
    AINFO << "cyber_PyTime_sleep_until ptr is null!";
    return Py_None;
  }

  pytime->sleep_until(nanoseconds);
  return Py_None;
}

PyObject* cyber_PyTime_now(PyObject* self, PyObject* args) {
  apollo::cyber::PyTime now = apollo::cyber::PyTime::now();
  return PyLong_FromUnsignedLongLong(now.to_nsec());
}

PyObject* cyber_PyTime_mono_time(PyObject* self, PyObject* args) {
  apollo::cyber::PyTime mono_time = apollo::cyber::PyTime::mono_time();
  return PyLong_FromUnsignedLongLong(mono_time.to_nsec());
}

// duration
PyObject* cyber_new_PyDuration(PyObject* self, PyObject* args) {
  uint64_t nanoseconds = 0;
  if (!PyArg_ParseTuple(args, const_cast<char*>("L:cyber_new_PyDuration"),
                        &nanoseconds)) {
    AERROR << "cyber_new_PyDuration parsetuple failed!";
    return Py_None;
  }

  apollo::cyber::PyDuration* pyduration =
      new apollo::cyber::PyDuration(nanoseconds);
  PyObject* pyobj_duration =
      PyCapsule_New(pyduration, "apollo_cybertron_pyduration", NULL);
  return pyobj_duration;
}

PyObject* cyber_delete_PyDuration(PyObject* self, PyObject* args) {
  PyObject* pyobj_duration = nullptr;
  if (!PyArg_ParseTuple(args, const_cast<char*>("O:cyber_delete_PyDuration"),
                        &pyobj_duration)) {
    return Py_None;
  }

  auto pyduration = (apollo::cyber::PyDuration*)PyCapsule_GetPointer(
      pyobj_duration, "apollo_cybertron_pyduration");
  if (nullptr == pyduration) {
    AINFO << "cyber_delete_PyDuration:pyduration ptr is null!";
    return Py_None;
  }
  delete pyduration;
  return Py_None;
}

PyObject* cyber_PyDuration_sleep(PyObject* self, PyObject* args) {
  PyObject* pyobj_duration = nullptr;
  if (!PyArg_ParseTuple(args, const_cast<char*>("O:cyber_PyDuration_sleep"),
                        &pyobj_duration)) {
    return Py_None;
  }

  auto pyduration = (apollo::cyber::PyDuration*)PyCapsule_GetPointer(
      pyobj_duration, "apollo_cybertron_pyduration");
  if (nullptr == pyduration) {
    AINFO << "cyber_PyDuration_sleep:pyduration ptr is null!";
    return Py_None;
  }
  pyduration->sleep();
  return Py_None;
}

// rate
PyObject* cyber_new_PyRate(PyObject* self, PyObject* args) {
  uint64_t nanoseconds = 0;
  if (!PyArg_ParseTuple(args, const_cast<char*>("L:cyber_new_PyRate"),
                        &nanoseconds)) {
    AERROR << "cyber_new_PyRate parsetuple failed!";
    return Py_None;
  }

  apollo::cyber::PyRate* pyrate = new apollo::cyber::PyRate(nanoseconds);
  PyObject* pyobj_rate = PyCapsule_New(pyrate, "apollo_cybertron_pyrate", NULL);
  return pyobj_rate;
}

PyObject* cyber_delete_PyRate(PyObject* self, PyObject* args) {
  PyObject* pyobj_rate = nullptr;
  if (!PyArg_ParseTuple(args, const_cast<char*>("O:cyber_delete_PyRate"),
                        &pyobj_rate)) {
    return Py_None;
  }

  auto pyrate = (apollo::cyber::PyRate*)PyCapsule_GetPointer(
      pyobj_rate, "apollo_cybertron_pyrate");
  if (nullptr == pyrate) {
    AINFO << "cyber_delete_PyRate:rate ptr is null!";
    return Py_None;
  }
  delete pyrate;
  return Py_None;
}

PyObject* cyber_PyRate_sleep(PyObject* self, PyObject* args) {
  PyObject* pyobj_rate = nullptr;
  if (!PyArg_ParseTuple(args, const_cast<char*>("O:cyber_PyRate_sleep"),
                        &pyobj_rate)) {
    return Py_None;
  }

  auto pyrate = (apollo::cyber::PyRate*)PyCapsule_GetPointer(
      pyobj_rate, "apollo_cybertron_pyrate");
  if (nullptr == pyrate) {
    AINFO << "cyber_PyRate_sleep:rate ptr is null!";
    return Py_None;
  }
  pyrate->sleep();
  return Py_None;
}

PyObject* cyber_PyRate_reset(PyObject* self, PyObject* args) {
  PyObject* pyobj_rate = nullptr;
  if (!PyArg_ParseTuple(args, const_cast<char*>("O:cyber_PyRate_reset"),
                        &pyobj_rate)) {
    return Py_None;
  }

  auto pyrate = (apollo::cyber::PyRate*)PyCapsule_GetPointer(
      pyobj_rate, "apollo_cybertron_pyrate");
  if (nullptr == pyrate) {
    AINFO << "cyber_PyRate_reset:rate ptr is null!";
    return Py_None;
  }
  pyrate->reset();
  return Py_None;
}

PyObject* cyber_PyRate_get_cycle_time(PyObject* self, PyObject* args) {
  PyObject* pyobj_rate = nullptr;
  if (!PyArg_ParseTuple(args,
                        const_cast<char*>("O:cyber_PyRate_get_cycle_time"),
                        &pyobj_rate)) {
    return PyLong_FromUnsignedLongLong(0);
  }

  auto pyrate = (apollo::cyber::PyRate*)PyCapsule_GetPointer(
      pyobj_rate, "apollo_cybertron_pyrate");
  if (nullptr == pyrate) {
    AINFO << "cyber_PyRate_get_cycle_time:rate ptr is null!";
    return PyLong_FromUnsignedLongLong(0);
  }
  uint64_t ctime = pyrate->get_cycle_time();
  return PyLong_FromUnsignedLongLong(ctime);
}

PyObject* cyber_PyRate_get_expected_cycle_time(PyObject* self, PyObject* args) {
  PyObject* pyobj_rate = nullptr;
  if (!PyArg_ParseTuple(
          args, const_cast<char*>("O:cyber_PyRate_get_expected_cycle_time"),
          &pyobj_rate)) {
    return PyLong_FromUnsignedLongLong(0);
  }

  auto pyrate = (apollo::cyber::PyRate*)PyCapsule_GetPointer(
      pyobj_rate, "apollo_cybertron_pyrate");
  if (nullptr == pyrate) {
    AINFO << "cyber_PyRate_get_expected_cycle_time:rate ptr is null!";
    return PyLong_FromUnsignedLongLong(0);
  }
  uint64_t exp_cycle_time = pyrate->get_expected_cycle_time();
  return PyLong_FromUnsignedLongLong(exp_cycle_time);
}

static PyMethodDef _cyber_time_methods[] = {
    // PyTime fun
    {"new_PyTime", cyber_new_PyTime, METH_VARARGS, ""},
    {"delete_PyTime", cyber_delete_PyTime, METH_VARARGS, ""},
    {"PyTime_now", cyber_PyTime_now, METH_VARARGS, ""},
    {"PyTime_mono_time", cyber_PyTime_mono_time, METH_VARARGS, ""},
    {"PyTime_to_sec", cyber_PyTime_to_sec, METH_VARARGS, ""},
    {"PyTime_to_nsec", cyber_PyTime_to_nsec, METH_VARARGS, ""},
    {"PyTime_sleep_until", cyber_PyTime_sleep_until, METH_VARARGS, ""},

    // PyDuration
    {"new_PyDuration", cyber_new_PyDuration, METH_VARARGS, ""},
    {"delete_PyDuration", cyber_delete_PyDuration, METH_VARARGS, ""},
    {"PyDuration_sleep", cyber_PyDuration_sleep, METH_VARARGS, ""},

    // PyRate
    {"new_PyRate", cyber_new_PyRate, METH_VARARGS, ""},
    {"delete_PyRate", cyber_delete_PyRate, METH_VARARGS, ""},
    {"PyRate_sleep", cyber_PyRate_sleep, METH_VARARGS, ""},
    {"PyRate_reset", cyber_PyRate_reset, METH_VARARGS, ""},
    {"PyRate_get_cycle_time", cyber_PyRate_get_cycle_time, METH_VARARGS, ""},
    {"PyRate_get_expected_cycle_time", cyber_PyRate_get_expected_cycle_time,
     METH_VARARGS, ""},

    {NULL, NULL, 0, NULL} /* sentinel */
};

/// Init function of this module
PyMODINIT_FUNC init_cyber_time(void) {
  AINFO << "init _cyber_time";
  Py_InitModule("_cyber_time", _cyber_time_methods);
}
