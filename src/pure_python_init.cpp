#include "macro_magic.hpp"
BEGIN_PYCODE R"pycode(#"

class _js(object):
    def __getattr__(self, name):
        ret = js_get_global(name)
        if JsValue.type_string(ret) == "undefined":
            raise AttributeError(f"has no attribute {name}")
        return ret
js = _js()


def extend_val():
    
 
    def __val_call(self, *args):
        if hasattr(self, '_embind11_parent'):
            bound = self.bind(self._embind11_parent)
            return apply(bound, args=args)
            #return member_apply(self._embind11_parent,js_function=self, args=args)
        else:
            return apply(self, args=args)
    JsValue.__call__ = __val_call

    def val_getattr(self, key):
        if(key == "_embind11_parent"):
            raise AttributeError()
        ret = JsValue._raw__getitem__(self, key)
        if JsValue.type_string(ret) == "undefined":
            raise AttributeError(f"has no attribute {key}")
        ret._embind11_parent = self
        ret._embind11_self_key = key
        return ret

    JsValue.__getitem__ = val_getattr
    JsValue.__getattr__ = val_getattr


extend_val()
del extend_val


def js_callback(py_function):
    _js_py_object = js_py_object(py_function)
    return _js_py_object['__call__'].bind(_js_py_object)

def console_log(*args):
    JsValue.get_global("console").log(*args)

def ensure_js_val(arg):
    if isinstance(arg, JsValue):
        return arg
    else:
        return JsValue(arg)


def error_checked(ret):
    type_string = JsValue.type_string(ret)

    if type_string == "object" and JsValue.has_own_property(ret, '__embind11__error__'):
        js_error = ret.__embind11__error__
        raise RuntimeError(f"{str(js_error.pythonize())}")
    return ret


def apply(js_function, args):
    js_array_args = js_array()
    for arg in args:
        js_arg = ensure_js_val(arg)
        val_call(js_array_args, "push", js_arg)

    applyTryCatch = js.applyTryCatch
    ret  = val_function_call(applyTryCatch, js_function, js_null(), js_array_args)
    return error_checked(ret)

def member_apply(self, js_function, args):
    js_array_args = js_array()
    for arg in args:
        js_arg = ensure_js_val(arg)
        val_call(js_array_args, "push", js_arg)

    applyTryCatch = js.applyTryCatch
    ret  = val_function_call(applyTryCatch, js_function, self, js_array_args)
    return error_checked(ret)

#)pycode"
END_PYCODE
