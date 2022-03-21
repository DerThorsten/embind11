#include <pybind11/pybind11.h>
#include <pybind11/embed.h> 

#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

#include <iostream>
#include <filesystem>

#include <embind11/convert.hpp>
#include "macro_magic.hpp"

namespace py = pybind11;
namespace em = emscripten;


void export_js_proxy(py::module_ & m);

PYTHON_INIT_DECL(embind11);

void export_embind11_module(py::module_ & embind11_module)
{  
    export_js_proxy(embind11_module);
    PYTHON_INIT(embind11)(embind11_module);
}