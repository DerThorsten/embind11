#include <pybind11/pybind11.h> 
#include <emscripten/val.h>
#include <emscripten/bind.h>
#include <iostream>
#include <filesystem>


namespace py = pybind11;
namespace em = emscripten;

// PYBIND11_EMBEDDED_MODULE(emsfun, m) {
//     py::class_<std::vector<int>>(m, "myvec")
//     ;
// }

struct EmValProxy
{
   em::EM_VAL emval; 
};


void export_ems_module(py::module_ & m)
{   
    py::class_<EmValProxy>(m, "EmValProxy")
    ;

    py::class_<em::val>(m, "val")


        .def_static( "get_global", [](const std::string & arg){
            return em::val::global(arg.c_str());
        })
        .def_static( "module_property", [](const std::string & arg){
            return em::val::module_property(arg.c_str());
        })
        .def_static("array", [](){return em::val::array();})
        .def_static("object", [](){return em::val::object();})
        .def_static("undefined", [](){return em::val::undefined();})
        .def_static("null", [](){return em::val::null();})
        .def_static("py_object",[](py::object py_object){
            return em::val(std::move(py_object));
        }, py::return_value_policy::copy)
        // .def_static( "as_handle", [](){return em::val::as_handle();})


        .def(py::init([](std::string arg) {
            return std::unique_ptr<em::val>(new em::val(arg.c_str()));
        }))
        .def(py::init([](int arg) {
            return std::unique_ptr<em::val>(new em::val(arg));
        }))
        .def(py::init([](bool arg) {
            return std::unique_ptr<em::val>(new em::val(arg));
        }))



        .def("hasOwnProperty",[](em::val * v, const std::string & key){
            return  v->hasOwnProperty(key.c_str( ));
        })

        .def("__getitem__",[](em::val * v, const std::string & key){
            return  v->operator[](key);
        })

        .def("set_func",[](em::val * v, const std::string & key, py::object pyobject){
            std::function<void()> cpp_func([](){
                std::cout<<"C++ CALLBACK]\n";
            });
            return  v->set(key, std::move(cpp_func));
        })
        .def("set_pyobject",[](em::val * v, const std::string & key, py::object pyobject){
            return  v->set(key, pyobject);
        })
        .def("__setitem__",[](em::val * v, const std::string & key, em::val & val){
            return  v->set(key, val);
        })

        .def("bind", [](em::val * v, em::val & arg1){
            return v->call<em::val>("bind", arg1);
        })

        .def("__call__",[](em::val * v){
            return v->operator()();
        })
        .def("__call__",[](em::val * v, em::val & arg1){
            std::cout<<"invoke 1\n";
            v->operator()(arg1);
        })
        .def("__call__",[](em::val * v, em::val & arg1,  em::val & arg2){
            return v->operator()(arg1, arg2);
        })
        .def("__call__",[](em::val * v, em::val & arg1,  em::val & arg2, em::val & arg3){
            return v->operator()(arg1, arg2, arg3);
        })

        .def("call",[](em::val * v, const std::string & key){
            return v->call<em::val>(key.c_str());
        })
        .def("call",[](em::val * v, const std::string & key, em::val & arg1){
            return v->call<em::val>(key.c_str(), arg1);
        })
        .def("call",[](em::val * v, const std::string & key, em::val & arg1,  em::val & arg2){
            return v->call<em::val>(key.c_str(), arg1, arg2);
        })
        .def("call",[](em::val * v, const std::string & key, em::val & arg1,  em::val & arg2, em::val & arg3){
            return v->call<em::val>(key.c_str(), arg1, arg2, arg3);
        })
        .def("call",[](em::val * v, const std::string & key, em::val & arg1,  em::val & arg2, em::val & arg3,  em::val & arg4){
            return v->call<em::val>(key.c_str(), arg1, arg2, arg3, arg4);
        })
    ;

    py::implicitly_convertible<std::string, em::val>();
    py::implicitly_convertible<int, em::val>();
    py::implicitly_convertible<bool, em::val>();

    m.def("g", []( std::string  arg) {
        return em::val::global(arg.c_str());
    });

    m.def("add", [](int i, int j) {
        return i + j;
    });
}