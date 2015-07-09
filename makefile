include make_vars.mk
include make_version.mk

.PHONY: build_all build_libs 
build_all: build_cate build_algorithms
	@echo "\nBuild process of CATE successfully terminated.\n"

build_libs: build_lua build_gmp

# Makefile targets for preparing the Lua library.
include make_lua.mk

# Makefile targets for preparing the GMP library.
include make_gmp.mk

# Makefile targets for building CATE.
include make_cate.mk

# Makefile targets for building all algorithms.
include make_algs.mk

# Makefile targets for provided unit tests.
include make_test.mk

# Debugging: 
# valgrind --tool=memcheck --leak-check=yes ./build/cate/cate <...>

#-----------------------------------------------------------------------

.PHONY: clean
clean: clean_lua clean_gmp clean_cate clean_algorithms clean_utest
	@echo "\nCleaned build directory.\n"

