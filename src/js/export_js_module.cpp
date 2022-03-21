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

void embed_hello_world() 
{

    // start the interpreter and keep it alive
    py::scoped_interpreter guard{}; 

    // use the Python API
    py::print("Hello, World!"); 
}

void exec_code(const std::string & code)
{
    // py::scoped_interpreter guard{};
    py::object scope = py::module_::import("__main__").attr("__dict__");

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


void set_pythonhome(const std::string & home)
{
    setenv("PYTHONHOME",home.c_str(),1); // does overwrite
}
void set_pythonpath(const std::string & path)
{
    setenv("PYTHONPATH",path.c_str(),1); // does overwrite
}

void export_py_object();

void export_js_module()
{
    export_py_object();

    em::function("initialize_interpreter",em::select_overload<void()>([](){
        py::initialize_interpreter(true,0,nullptr,false);
    }));
    em::function("finalize_interpreter",em::select_overload<void()>([](){
        py::finalize_interpreter();
    }));




    em::function("set_pythonhome", &set_pythonhome);
    em::function("set_pythonpath", &set_pythonpath);

    em::function("exec_code", &exec_code);
}