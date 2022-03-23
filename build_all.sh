#bin/bash
set -e

mkdir -p bld_ems
cd bld_ems

emboa pack python core $HOME/micromamba/envs/emforgetest --version=3.10



export PREFIX=$HOME/micromamba/envs/emforgetest
export CMAKE_PREFIX_PATH=$PREFIX 
export CMAKE_SYSTEM_PREFIX_PATH=$PREFIX 
# export EMCC_FORCE_STDLIBS=1
emcmake cmake  \
    -DCMAKE_BUILD_TYPE=Release\
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ON \
    -DCMAKE_PROJECT_INCLUDE=overwriteProp.cmake \
    -DWITH_JS_PY_BRIDGE=OFF \
    ..

make -j12

emboa pack python pure-so \
    $HOME/micromamba/envs/emforgetest \
    embind11_side.so \
    embind11_side \
    --version=3.10

emboa pack python pure-so \
    $HOME/micromamba/envs/emforgetest \
    embind11_side2.so \
    embind11_side2 \
    --version=3.10
