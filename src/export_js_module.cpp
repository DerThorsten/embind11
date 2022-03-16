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
class _js(object):
    def __getattr__(self, name):
        ret = js_get_global(name)
        if ret.type_string() == "undefined":
            raise AttributeError(f"has no attribute {name}")
        return ret
js = _js()



def extend_val():

    def __val_call(self, *args):
        if hasattr(self, '_embind11_parent'):
            bound_f = self.bind(self._embind11_parent)
            return apply(bound_f, args=args)
        else:
            return apply(self, args=args)
    val.__call__ = __val_call

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
  _js_py_object = js_py_object(py_function)
  return _js_py_object['__call__'].bind(_js_py_object)

def console_log(*args):
    val.get_global("console").log(*args)

def ensure_js_val(arg):
    if isinstance(arg, val):
        return arg
    else:
        return val(arg)

def apply(js_function, args):
    js_array_args = js_array()
    for arg in args:
        js_arg = ensure_js_val(arg)
        val_call(js_array_args, "push", js_arg)
    return val_call(js_function,"apply", js_null(), js_array_args)


def apply_member(this, js_function_name, args):
    js_array_args = js_array()
    for arg in args:
        js_arg = ensure_js_val(arg)
        val_call(js_array_args, "push", js_arg)
    return val_call(js_function,"apply", this, js_array_args)



)pycode",scope);
}




void export_val(py::module_ & m)
{   
    // py::class_<EmValProxy>(m, "EmValProxy")
    // ;


    m.def( "js_get_global", [](const std::string & arg){
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

    m.def("val_function_call",[](em::val * v, em::val  arg1){
        return v->operator()(arg1);
    });
    m.def("val_function_call",[](em::val * v, em::val  arg1,  em::val  arg2){
        return v->operator()(arg1, arg2);
    });

    py::class_<em::val>(m, "val",  py::dynamic_attr())


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

// void export_exec(py::module_ & m)
// {
//     // m.def("run_script", [](const std::string & code){
//     //     emscripten_run_script(code.c_str());
//     // });
//     // m.def("eval",[](const std::string & code) -> em::val {
//     //     return em::val::global("eval").operator()(code);
//     // });
// }

void export_js_module(py::module_ & m)
{  
    export_val(m);
    pseudo_init(m);
}