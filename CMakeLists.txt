cmake_minimum_required(VERSION 3.9)
project(embind11 VERSION 0.1.4 DESCRIPTION "embind11 description")



set(EM_FLAGS "${EM_FLAGS} -Oz --bind   -s EXPORT_ALL=1 -s LINKABLE=1 -s EXPORT_ALL=1 -std=c++17 -fPIC -s DISABLE_EXCEPTION_THROWING=0 -s DISABLE_EXCEPTION_CATCHING=0  -s NO_DISABLE_EXCEPTION_CATCHING -fexceptions -s ALLOW_MEMORY_GROWTH=1")
set(EM_FLAGS "${EM_FLAGS} -s ASYNCIFY -s ")
set(EM_FLAGS "${EM_FLAGS} --pre-js pre.js")
set(EM_FLAGS "${EM_FLAGS} --post-js post.js")
set(EM_FLAGS "${EM_FLAGS} --extern-pre-js extern_pre.js")
# linker flags
set(EM_LINKER_FLAGS "${EM_LINKER_FLAGS} -WASM=1 -s FORCE_FILESYSTEM=1 -s MODULARIZE -s ERROR_ON_UNDEFINED_SYMBOLS=1")
set(EM_LINKER_FLAGS "${EM_LINKER_FLAGS} -s EXPORT_NAME=\"createModule\" -s EXPORT_ES6=0 -s USE_ES6_IMPORT_META=0 -s ERROR_ON_UNDEFINED_SYMBOLS=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EM_FLAGS} ${EM_LINKER_FLAGS}")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}  ${EM_FLAGS} ${EM_LINKER_FLAGS}")


find_package(pybind11 REQUIRED CONFIG)

SET(PYTHON_UTIL_LIBS 
    $ENV{PREFIX}/lib/python_internal/libmpdec.a
    $ENV{PREFIX}/lib/python_internal/libexpat.a
    $ENV{PREFIX}/lib/libbz2.a
    $ENV{PREFIX}/lib/libz.a
)


configure_file(embind11.html        ${CMAKE_CURRENT_BINARY_DIR}/embind11.html COPYONLY)
configure_file(pre.js               ${CMAKE_CURRENT_BINARY_DIR}/pre.js COPYONLY)


################################################################
# SIDE_MODULE
# create compiled extension named "embind11" which we want to consume
# in another wasm executable  
################################################################
pybind11_add_module(embind11_side SHARED embind11_side.cpp)





###########################################################################
# embed python in a wasm executable (this IS MAIN
###########################################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(embed 
    embed.cpp
    src/js/export_py_interpreter.cpp
    src/js/export_py_eval.cpp
    src/js/export_py_object.cpp
    src/js/export_js_module.cpp
    src/py/convert.cpp
    src/py/pure_python_init.cpp
    src/py/export_js_proxy.cpp
    src/py/export_embind11_module.cpp
)
set_target_properties(embed PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF)
set_target_properties(embed PROPERTIES 
    LINK_FLAGS "-s MAIN_MODULE=1  -lstdc++ ${EM_FLAGS}"
    CXX_FLAGS  "${EM_FLAGS} "
)

target_link_libraries(embed PRIVATE ${PYTHON_UTIL_LIBS} pybind11::embed)



include(GNUInstallDirs)
install(TARGETS embed
    ARCHIVE DESTINATION ${CMAKE_INSTALL_BINDIR})

install(TARGETS embind11_side
    LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})


install(FILES
    "$<TARGET_FILE_DIR:embed>/embed.js"
    "$<TARGET_FILE_DIR:embed>/embed.wasm"
    "$<TARGET_FILE_DIR:embed>/embind11.html"
    DESTINATION ${CMAKE_INSTALL_BINDIR})
