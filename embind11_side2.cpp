#include <pybind11/pybind11.h>
#include <emscripten/val.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

int multiply(int i, int j) {
    return i * j;
}

namespace py = pybind11;
namespace em = emscripten;

PYBIND11_MODULE(embind11_side2, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------
        .. currentmodule:: embind11_side
        .. autosummary::
           :toctree: _generate
           multiply
           subtract
    )pbdoc";

    m.def("multiply", &multiply, R"pbdoc(
        multiply two numbers
        Some other explanation about the multiply function.
    )pbdoc");

    m.def("js_get_global", [](const std::string & arg){
        return em::val::global(arg.c_str());
    });


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}