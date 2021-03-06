include make_vars.mk
include make_version.mk

# CATE with default mode: combined memory & time metering
.PHONY: build_all build_libs check_dependencies
build_all: check_dependencies build_cate build_algorithms
	@echo "\nBuild process of CATE successfully terminated.\n"

# CATE with time metering only
.PHONY: build_all_nomem 
build_all_nomem: CFLAGS += $(MEMDISABLE)
build_all_nomem: build_all

build_libs: build_lua build_gmp

# check some dependencies before doing anything
check_dependencies: 
	./tools/checkdep.sh

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

