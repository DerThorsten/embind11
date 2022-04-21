// #ifdef WITH_JS_PY_BRIDGE
#include <pybind11/pybind11.h> 
// #endif

#include <emscripten/bind.h>
#include <emscripten.h>
#include <emscripten/val.h>

#ifdef WITH_JS_PY_BRIDGE

void export_embind11_module(pybind11::module_ & m);
// define the python module

#endif

//  // __attribute__((used))
// void js_get_global(const char *name);


EM_JS(void, js_get_global, (const char * name), {

   console.log('js_get_global name: ' + [name]);
});

// define the js module
void export_js_module();

EMSCRIPTEN_BINDINGS(my_module) {
    export_js_module();
}


int main(){
    if(false){
        js_get_global("hello");
    }
}