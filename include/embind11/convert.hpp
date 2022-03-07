#pragma once

#include <pybind11/pybind11.h>
#include <emscripten/val.h>

namespace embind11
{

    namespace py = pybind11;
    namespace em = emscripten;

    py::object convert_impl(em::val em_val);
    std::string type_string(em::val em_val);
    bool is_array(em::val em_val);
}