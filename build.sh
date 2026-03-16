#!/usr/bin/env zsh

local fresh;
local cl_test="`which cl`"
zmodload zsh/pcre
if [[ "$1" -pcre-match "-f((?<=--f)resh|(?<=^-f)$)" ]]; then
    fresh="-f"
    else
    unset fresh
fi
if [ ! -f "$cl_test" ]; then echo "ERROR"; exit 2; fi
if [ ! -f "$PWD/setup_cmake.sh" ]; then echo "ERROR"; exit 2; fi
./setup_cmake.sh "$fresh"
pushd build
cmake --build . -j 24
popd