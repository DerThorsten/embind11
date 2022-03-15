#bin/bash
set -e

mkdir -p bld_ems
cd bld_ems

emboa pack python core $HOME/micromamba/envs/emforgetest --version=3.11



export PREFIX=$HOME/micromamba/envs/emforgetest
export CMAKE_PREFIX_PATH=$PREFIX 
export CMAKE_SYSTEM_PREFIX_PATH=$PREFIX 
# export EMCC_FORCE_STDLIBS=1
emcmake cmake  \
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ON \
    -DCMAKE_VERBOSE_MAKEFILE=ON \
    -DCMAKE_FIND_DEBUG_MODE=ON \
    -DCMAKE_PROJECT_INCLUDE=overwriteProp.cmake \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    ..

make -j12

emboa pack python pure-so \
    $HOME/micromamba/envs/emforgetest \
    embind11.so \
    embind11 \
    --version=3.11

