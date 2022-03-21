Module['_apply_try_catch'] =function(val, self, args){
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