// everything needed for embedding
#include <pybind11/embed.h> 
#include <emscripten/val.h>
#include <emscripten/bind.h>
#include <iostream>
#include <filesystem>
#include <embind11/convert.hpp>
#include <strings.h>

namespace py = pybind11;
namespace em = emscripten;

void export_py_interpreter();
void export_py_object();
void export_py_eval();

void export_js_module()
{
    export_py_interpreter();
    export_py_eval();
    export_py_object();
}