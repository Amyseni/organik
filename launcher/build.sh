#!/usr/bin/bash

cl_test=`which cl`
if [ ! -f "$cl_test" ]; then echo "ERROR"; exit 2; fi
if [ ! -f "$PWD/setup_cmake.sh" ]; then echo "ERROR"; exit 2; fi
if [ "$fresh" = "--fresh" ]; then
    ./setup_cmake.sh -f
    else
    ./setup_cmake.sh
fi
cd build
make