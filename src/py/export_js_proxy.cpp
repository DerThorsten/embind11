#include <pybind11/pybind11.h>

#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

#include <iostream>
#include <filesystem>

#include <embind11/convert.hpp>

namespace py = pybind11;
namespace em = emscripten;

void export_js_proxy(py::module_ & m)
{   


    py::module_ m_internal = m.def_submodule("internal", "A submodule of 'embind11'");

    m.def("sleep",[](const uint64_t t){
        emscripten_sleep(t);
    });


    m.def("js_get_global", [](const std::string & arg){
        return em::val::global(arg.c_str());
    });
    m.def( "module_property", [](const std::string & arg){
        return em::val::module_property(arg.c_str());
    });


    m.def("js_array", [](){return em::val::array();});
    m.def("js_object", [](){return em::val::object();});
    m.def("js_undefined", [](){return em::val::undefined();});
    m.def("js_null", [](){return em::val::null();});
    m.def("js_py_object",[](py::object py_object){
        return em::val(std::move(py_object));
    }, py::return_value_policy::copy);



    m.def("val_call",[](em::val * v, const std::string & key, em::val & arg1){
        return v->call<em::val>(key.c_str(), arg1);
    });

    m.def("val_call",[](em::val * v, const std::string & key, em::val & arg1,  em::val & arg2){
        return v->call<em::val>(key.c_str(), arg1, arg2);
    });

    m_internal.def("val_function_call",[](em::val * v){
        return v->operator()();
    });
    m_internal.def("val_function_call",[](em::val * v, em::val  arg1){
        return v->operator()(arg1);
    });
    m_internal.def("val_function_call",[](em::val * v, em::val  arg1,  em::val  arg2){
        return v->operator()(arg1, arg2);
    });
    m_internal.def("val_function_call",[](em::val * v, em::val  arg1,  em::val  arg2,  em::val  arg3){
        return v->operator()(arg1, arg2, arg3);
    });
    m_internal.def("val_function_call",[](em::val * v, em::val  arg1,  em::val  arg2,  em::val  arg3,  em::val  arg4){
        return v->operator()(arg1, arg2, arg3, arg4);
    });

    py::class_<em::val>(m, "JsValue",  py::dynamic_attr())

        .def(py::init([](std::string arg) {
            return std::unique_ptr<em::val>(new em::val(arg.c_str()));
        }))
        .def(py::init([](int arg) {
            return std::unique_ptr<em::val>(new em::val(arg));
        }))
        .def(py::init([](float arg) {
            return std::unique_ptr<em::val>(new em::val(arg));
        }))
        .def(py::init([](double arg) {
            return std::unique_ptr<em::val>(new em::val(arg));
        }))
        .def(py::init([](bool arg) {
            return std::unique_ptr<em::val>(new em::val(arg));
        }))
        // .def("new",[](em::val  v, em::val arg1){
        //     return  v.new_(arg1);
        // })
        // .def("__bool__",[](em::val * v){
        //     return  v->as<bool>();
        // })
        .def_static("has_own_property",[](em::val * v, const std::string & key){
            return  v->hasOwnProperty(key.c_str( ));
        })

        .def_static("_raw__getitem__",[](em::val * v, const std::string & key){
            return  v->operator[](key);
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


        .def_static("type_string", [](em::val * v){
            return v->typeOf().as<std::string>();
        })

        .def("keys", [](em::val * v){
            auto keys =  em::val::global("Object").call<em::val>("keys", *v);
            int length = keys["length"].as<int>();
            // for (int i = 0; i < length; ++i) {
            //     printf("%s\n", keys[i].as<std::string>().c_str());
            // }
            return keys;
        })
        .def("try_length",[](em::val * v){
            if(v->hasOwnProperty("length"))
            {
                return v->operator[]("length").as<int>();
            }
            else
            {
                return -1;
            }
        })
        .def("type_of", [](em::val * v){
            return v->typeOf();
        })
        .def("as_string", [](em::val * v){
            return v->as<std::string>();
        })
        .def("pythonize", [](em::val * v)-> py::object {
            return embind11::convert_impl(*v);
        })
    ;

    py::implicitly_convertible<std::string, em::val>();
    py::implicitly_convertible<float, em::val>();
    py::implicitly_convertible<double, em::val>();
    py::implicitly_convertible<int, em::val>();
    py::implicitly_convertible<bool, em::val>();

}
