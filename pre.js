Module['_apply_try_catch'] = function(val, self, args){
    try {

        return val.apply( self, args)
    }
    catch(e)
    {
        const err_obj = {
            __embind11__error__: e
        }
        return err_obj
    }
}


Module['_call_py_object_variadic'] = function(py_val, ...args){
    return py_val['__call_variadic__'](args)
} 