#include <pybind11/pybind11.h>
#include <pybind11/embed.h> 

#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

#include <iostream>
#include <filesystem>

#include <embind11/convert.hpp>

namespace py = pybind11;
namespace em = emscripten;

// PYBIND11_EMBEDDED_MODULE(emsfun, m) {
//     py::class_<std::vector<int>>(m, "myvec")
//     ;
// }

void pseudo_init(py::module_ & m)
{
    py::object scope  = m.attr("__dict__");
    // py::object scope = py::module_::import("__main__").attr("__dict__");
    py::exec(R"pycode(

# module level __getattr__ to do thinks like:
# js.document instead of val.get_global("document")
def __getattr__(name):
    ret = val.get_global(name)
    if ret.type_string() == "undefined":
        raise AttributeError(f"has no attribute {name}")
    return ret

def extend_val():

    def val_call(self, *args, **kwargs):
        if hasattr(self, '_embind11_parent'):
            return self._embind11_parent.call(self._embind11_self_key, *args, **kwargs)
        else:
            return self._fcal(*args, **kwargs)
    val.__call__ = val_call

    def val_getattr(self, key):
        if(key == "_embind11_parent"):
            raise AttributeError()
        ret = self._raw__getitem__(key)
        if ret.type_string() == "undefined":
            raise AttributeError(f"has no attribute {key}")
        ret._embind11_parent = self
        ret._embind11_self_key = key
        return ret

    val.__getitem__ = val_getattr
    val.__getattr__ = val_getattr

extend_val()
del extend_val

def js_callback(py_function):
  js_py_object = val.py_object(py_function)
  return js_py_object['__call__'].bind(js_py_object)

def console_log(*args):
    val.get_global("console").log(*args)

)pycode",scope);
}




void export_val(py::module_ & m)
{   
    // py::class_<EmValProxy>(m, "EmValProxy")
    // ;

    py::class_<em::val>(m, "val",  py::dynamic_attr())


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
        .def(py::init([](float arg) {
            return std::unique_ptr<em::val>(new em::val(arg));
        }))
        .def(py::init([](double arg) {
            return std::unique_ptr<em::val>(new em::val(arg));
        }))
        .def(py::init([](bool arg) {
            return std::unique_ptr<em::val>(new em::val(arg));
        }))


        .def("hasOwnProperty",[](em::val * v, const std::string & key){
            return  v->hasOwnProperty(key.c_str( ));
        })

        .def("_raw__getitem__",[](em::val * v, const std::string & key){
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

        .def("_fcal",[](em::val * v){
            return v->operator()();
        })
        .def("_fcal",[](em::val * v, em::val  arg1){
            return v->operator()(arg1);
        })
        .def("_fcal",[](em::val * v, em::val  arg1,  em::val  arg2){
            return v->operator()(arg1, arg2);
        })
        .def("_fcal",[](em::val * v, em::val  arg1,  em::val  arg2, em::val  arg3){
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
        .def("type_string", [](em::val * v){
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
    ;

    py::implicitly_convertible<std::string, em::val>();
    py::implicitly_convertible<float, em::val>();
    py::implicitly_convertible<double, em::val>();
    py::implicitly_convertible<int, em::val>();
    py::implicitly_convertible<bool, em::val>();

    m.def("g", []( std::string  arg) {
        return em::val::global(arg.c_str());
    });

    m.def("add", [](int i, int j) {
        return i + j;
    });
}

void export_exec(py::module_ & m)
{
    // m.def("run_script", [](const std::string & code){
    //     emscripten_run_script(code.c_str());
    // });
    // m.def("eval",[](const std::string & code) -> em::val {
    //     return em::val::global("eval").operator()(code);
    // });
}

void export_js_module(py::module_ & m)
{  
    export_val(m);
    pseudo_init(m);
}