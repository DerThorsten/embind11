

function ShowMessageExternPre(firstName, lastName) {
    alert("Hello Extern Pre" + firstName + " " + lastName);
    console.log("\n\nLOADED IT",Module,"END")
}


function TheThrowing(){
    throw "ERROR"
}

function applyTryCatch(val, self, args){
    try {

        return val.apply( self, args)
    }
    catch(e)
    {
        console.log("WE CATCHED THAT ERROR",e)
        const err_obj = {
            __embind11__error__: e
        }
        return err_obj
    }
}