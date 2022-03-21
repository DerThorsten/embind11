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



void export_py_interpreter()
{

    em::function("initialize_interpreter",em::select_overload<void()>([](){
        py::initialize_interpreter(true,0,nullptr,false);
    }));
    em::function("finalize_interpreter",em::select_overload<void()>([](){
        py::finalize_interpreter();
    }));


}


