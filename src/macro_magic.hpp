#include <pybind11/pybind11.h>
#include <pybind11/embed.h> 

#define BEGIN_PYCODE \
namespace py = pybind11; \
void pseudo_init(py::module_ & m){ \
    py::object scope  = m.attr("__dict__");\
    py::exec(

#define END_PYCODE ,scope);}