#!/bin/bash
cd bld_ems

export PREFIX=$HOME/micromamba/envs/emtest
export CMAKE_PREFIX_PATH=$PREFIX 
export CMAKE_SYSTEM_PREFIX_PATH=$PREFIX 

make -j12

