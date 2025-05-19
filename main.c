#include <stdio.h>

#include "header.h"

#define PLAYER_FILE "example.py"

#define ERROR(msg)        \
  printf("[ERROR] " msg); \
  return 1;

int main() {
  Py_Initialize();
  PyObject* sysmodule = PyImport_ImportModule("sys");
  PyObject* syspath = PyObject_GetAttrString(sysmodule, "path");
  PyList_Append(syspath, PyBytes_FromString("."));
  Py_DECREF(syspath), Py_DECREF(sysmodule);

  FILE* exp_file;
  PyObject *main_module, *global_dict;
  // Load the relevant functions from the file
  exp_file = fopen(PLAYER_FILE, "r");
  PyRun_SimpleFile(exp_file, PLAYER_FILE);

  main_module = PyImport_AddModule("__main__");
  global_dict = PyModule_GetDict(main_module);

  char* exec_str = calloc(300, sizeof(char));
  sprintf(exec_str, "CONSTANT_1 = %d\nCONSTANT_2 = %d\n", CONSTANT_1,
          CONSTANT_2);
  PyRun_SimpleString(exec_str);
  free(exec_str);

  PyObject* exampleFunc = PyDict_GetItemString(global_dict, "exampleFunc");
  if (exampleFunc == NULL || exampleFunc == NULL) {
    ERROR("Failed to load Python functions")
  }

  printf(
      "---------------\nINTEDED PYTHON PROGRAM OUTPUT:\nConstant 1: %d "
      "Constant 2: %d\n",
      CONSTANT_1, CONSTANT_2);

  printf("---------------\nPYTHON PROGRAM OUTPUT:\n");

  PyObject* args = PyTuple_New(1);
  PyObject* value = PyList_New(3);
  int sum = 0;
  for (int i = 0; i < 3; ++i) {
    PyList_SetItem(value, i, PyLong_FromLong(i));
    sum += i;
  }
  PyTuple_SetItem(args, 0, value);
  PyObject* output = PyObject_CallObject(exampleFunc, args);

  printf("---------------\n");

  int result[2] = {PyLong_AsLong(PyList_GetItem(output, 0)),
                   PyLong_AsLong(PyList_GetItem(output, 1))};

  printf("Intended return value [%d %d]\n", CONSTANT_1 + CONSTANT_2, sum);
  printf("Python function returned [%d %d]\n", result[0], result[1]);
}