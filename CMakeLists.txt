cmake_minimum_required(VERSION 3.9)
project(embind11 VERSION 0.3.0 DESCRIPTION "embind11 description")


option(ENV_NODE "Build for the node env" OFF)



if(ENV_NODE)
    SET(ENVIRONMENT "node")
else()
    SET(ENVIRONMENT "web")
endif()


option(WITH_JS_PY_BRIDGE "Build experimental js-py bridge" OFF)





if(WITH_JS_PY_BRIDGE)
    add_compile_definitions(WITH_JS_PY_BRIDGE)
endif()


set(EM_FLAGS "${EM_FLAGS} -Os -s EXPORT_ALL=1  -s ENVIRONMENT=${ENVIRONMENT} --bind")

# set(EM_FLAGS "${EM_FLAGS} -s ASYNCIFY")

# set(EM_FLAGS "${EM_FLAGS} -s DISABLE_EXCEPTION_THROWING=0")
# set(EM_FLAGS "${EM_FLAGS} -s DISABLE_EXCEPTION_CATCHING=0 ")
# set(EM_FLAGS "${EM_FLAGS} -s NO_DISABLE_EXCEPTION_CATCHING")
set(EM_FLAGS "${EM_FLAGS} -fwasm-exceptions -s LZ4=1")

set(EM_FLAGS "${EM_FLAGS} -s ALLOW_MEMORY_GROWTH")
set(EM_FLAGS "${EM_FLAGS} -WASM=1 -s FORCE_FILESYSTEM=1 -s MODULARIZE")
set(EM_FLAGS "${EM_FLAGS} -s ERROR_ON_UNDEFINED_SYMBOLS=1")
set(EM_FLAGS "${EM_FLAGS} -s EXPORT_NAME=\"createModule\"")
set(EM_FLAGS "${EM_FLAGS} -s EXPORT_ES6=0 -s USE_ES6_IMPORT_META=0")

set(EM_FLAGS "${EM_FLAGS} --pre-js pre.js -fPIC")
set(EM_FLAGS "${EM_FLAGS} --post-js post.js")

message(STATUS ${EM_FLAGS})

find_package(pybind11 REQUIRED CONFIG)

SET(PYTHON_UTIL_LIBS 
    $ENV{PREFIX}/lib/libbz2.a
    $ENV{PREFIX}/lib/libz.a
    $ENV{PREFIX}/lib/libsqlite3.a
    $ENV{PREFIX}/lib/libffi.a
)


configure_file(embind11.html        ${CMAKE_CURRENT_BINARY_DIR}/embind11.html COPYONLY)
configure_file(pre.js               ${CMAKE_CURRENT_BINARY_DIR}/pre.js COPYONLY)
configure_file(post.js               ${CMAKE_CURRENT_BINARY_DIR}/post.js COPYONLY)


################################################################
# SIDE_MODULES
# create compiled extensions 
################################################################
pybind11_add_module(embind11_side SHARED    embind11_side.cpp)
pybind11_add_module(embind11_side2 SHARED   embind11_side2.cpp)



###########################################################################
# embed python in a wasm executable (this IS MAIN
###########################################################################

SET(SOURCES
    src/js/export_py_eval.cpp
    src/js/export_py_interpreter.cpp
    src/js/export_js_module.cpp
    src/js/export_py_object.cpp
)
if(WITH_JS_PY_BRIDGE)
    SET(SOURCES ${SOURCES}

    src/py/convert.cpp
    src/py/pure_python_init.cpp
    src/py/export_js_proxy.cpp
    src/py/export_embind11_module.cpp
    )
endif()




include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${pybind11_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR})

add_library(embedlib STATIC ${SOURCES})
set_target_properties(embedlib PROPERTIES 
    LINK_FLAGS "${EM_FLAGS} -fPIC     -s EXPORT_BINDINGS=1  -s EXPORTED_RUNTIME_METHODS=\"['_js_get_global']\""
    COMPILE_FLAGS  "${EM_FLAGS} -fPIC -s EXPORT_BINDINGS=1")
target_link_libraries(embedlib PRIVATE  ${PYTHON_UTIL_LIBS} pybind11::embed)
add_executable(embed 
    embed.cpp
    # ${SOURCES}
)

target_link_libraries(embed PRIVATE  embedlib )

set_target_properties(embed PROPERTIES 
    LINK_FLAGS "-s MAIN_MODULE=1  -lstdc++ ${EM_FLAGS} -fPIC -s EXPORT_BINDINGS=1 -s EXPORT_ALL=1  -s EXPORTED_RUNTIME_METHODS=\"['_js_get_global']\""
    COMPILE_FLAGS  "${EM_FLAGS} -fPIC -s EXPORT_BINDINGS=1 -s EXPORT_ALL=1 "
)

include(GNUInstallDirs)
install(TARGETS embed
    ARCHIVE DESTINATION ${CMAKE_INSTALL_BINDIR})

install(TARGETS embind11_side
    LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})

install(TARGETS embind11_side2
    LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})


install(FILES
    "$<TARGET_FILE_DIR:embed>/embed.js"
    "$<TARGET_FILE_DIR:embed>/embed.wasm"
    "$<TARGET_FILE_DIR:embed>/embind11.html"
    DESTINATION ${CMAKE_INSTALL_BINDIR})
