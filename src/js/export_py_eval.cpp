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

py::object main_scope()
{
    return py::module_::import("__main__").attr("__dict__");
}
py::object  eval_code(const std::string & code, py::object scope)
{
    return  py::eval(code.c_str(),scope);
}

void exec_code(const std::string & code, py::object scope)
{
    // py::scoped_interpreter guard{};
    //py::object scope = py::module_::import("__main__").attr("__dict__");

    try{
        
        py::exec(code.c_str(),scope);
    }
    catch (py::error_already_set &e) {
        std::cerr<<e.what()<<"\n";
    }
    catch (const std::exception& e) { 
        std::cerr<<e.what()<<"\n";
    }
    catch (...) {
        std::cerr<<"unknown obscure error\n";
        throw;
    }
}


void export_py_eval()
{   
    em::function("main_scope", &main_scope);
    em::function("exec_code",  &exec_code);
    em::function("eval_code",  &exec_code);
}


