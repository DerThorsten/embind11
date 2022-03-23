
void export_py_interpreter();
void export_py_eval();

// #ifdef WITH_JS_PY_BRIDGE
void export_py_object();

void export_js_module()
{
    export_py_interpreter();
    export_py_eval();
    export_py_object();

}