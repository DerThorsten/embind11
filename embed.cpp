#ifdef WITH_JS_PY_BRIDGE
#include <pybind11/embed.h> 
#endif

#include <emscripten/bind.h>


#ifdef WITH_JS_PY_BRIDGE
void export_embind11_module(py::module_ & m);
// define the python module
PYBIND11_EMBEDDED_MODULE(embind11, m) {
    export_embind11_module(m);
}
#endif


// define the js module
void export_js_module();

EMSCRIPTEN_BINDINGS(my_module) {
    export_js_module();
}

