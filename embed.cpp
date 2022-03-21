#include <pybind11/embed.h> 
#include <emscripten/bind.h>

namespace py = pybind11;
namespace em = emscripten;

void export_embind11_module(py::module_ & m);
void export_js_module();

// define the python module
PYBIND11_EMBEDDED_MODULE(embind11, m) {
    export_embind11_module(m);
}

// define the js module
EMSCRIPTEN_BINDINGS(my_module) {
    export_js_module();
}