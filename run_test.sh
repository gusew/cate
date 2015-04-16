#!/bin/sh

# Set library path for libunittest-shared library 
export LD_LIBRARY_PATH=$(cd ./libunittest/lib; pwd):$LD_LIBRARY_PATH

# Run tests
#./build/test/cate_unittest -v
$1 $2

# For memory check of unittests
#valgrind --tool=memcheck --leak-check=yes $1 $2
