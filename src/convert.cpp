
#include <pybind11/pybind11.h>
#include <emscripten/val.h>
#include <string>

namespace py = pybind11;
namespace em = emscripten;

namespace embind11
{

std::string type_string(em::val em_val)
{
    return em_val.typeOf().as<std::string>();
}
bool is_array(em::val em_val)
{
    return em::val::global("Array").call<em::val>("isArray", em_val).as<bool>();
}

py::object convert_impl(em::val em_val)
{
    const auto js_type = type_string(em_val);

    // is array
    if(is_array(em_val))
    {
        const int length = em_val["length"].as<int>();
        auto values =  em::val::global("Object").call<em::val>("values", em_val);
        py::list py_list;
        for (int i = 0; i < length; ++i) {
            emscripten::val js_val = values[i];
            py::object py_val = convert_impl(js_val);
            py_list.append(py_val);
        }
        return py_list;
    }
    // is dict-like
    else if(js_type == "object")
    {   
        py::dict py_dict;
        auto keys =  em::val::global("Object").call<em::val>("keys", em_val);
        auto values =  em::val::global("Object").call<em::val>("values", em_val);
        const int length = keys["length"].as<int>();

        for (int i = 0; i < length; ++i) {
            emscripten::val js_key = keys[i];
            emscripten::val js_val = values[i];

            py::object py_key = convert_impl(js_key);
            py::object py_val = convert_impl(js_val);
            py_dict[py_key] = py_val;
        }
        return py_dict;
    }
    else if(js_type == "string")
    {
        return py::cast(em_val.as<std::string>());
    }
    else if(js_type == "boolean")
    {
        return py::cast(em_val.as<bool>());
    }
    else if(js_type == "number")
    {
        return py::cast(em_val.as<double>());
    }
    else if(js_type == "function")
    {
        return py::cast("FUNCTION");
    }
    else if(js_type == "undefined")
    {
        return py::none();
    }
    else
    {
        return py::cast(std::string("unknown type:") + js_type);
    }
}


}
