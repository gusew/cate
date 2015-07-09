# compiler flag settings
TEST_C_FLAGS	= -g -O0 -Wall -Werror -pedantic -std=c++11 -fPIC -fmax-errors=3 -pthread -fmessage-length=0 -D_GLIBCXX_USE_NANOSLEEP
# locations for files 
TEST_OBJ_DIR	= $(BUILD)test/obj/
TEST_EXEC_DIR	= $(BUILD)test/
TEST_LIB_DIR	= ./libunittest/
TEST_LIB_SO 	= $(TEST_LIB_DIR)lib/libunittest.so
TEST_BUILD_SO	= $(BUILD)libunittest/libunittest.so
TEST_SRCS_DIR	= ./test/
TEST_RUNNER		= $(TEST_EXEC_DIR)run_test.sh


# create a small script for running tests with the libunittest library
$(TEST_RUNNER):
	$(MKDIR)
	@echo "#!/bin/sh\n" > $@
	@echo "# Set library path for libunittest-shared library" >> $@
	@echo 'export LD_LIBRARY_PATH=$$(cd ./libunittest/lib; pwd):$$LD_LIBRARY_PATH' >> $@
	@echo '$$1 $$2' >> $@
	@chmod +x $@

# copy libunittest shared library file
$(TEST_BUILD_SO): $(TEST_LIB_SO)
	$(MKDIR)
	cp $(TEST_LIB_SO) $(TEST_BUILD_SO)

# build libunittest library 
$(TEST_LIB_SO):
	make -C $(TEST_LIB_DIR)

# create a softlink to libunittest-headerfiles
$(INCLUDE)/libunittest:
	-ln -s ../$(TEST_LIB_DIR)include/libunittest $@

# build each test object file
$(TEST_OBJ_DIR)%.o: $(TEST_SRCS_DIR)%.cpp
	$(MKDIR)
	$(CC) $(TEST_C_FLAGS) -I $(INCLUDE) -c $< -o $@

# targets for unit tests of the CATE framework
include make_test_cate.mk

# targets for unit tests of algorithm implementations
include make_test_alg_linear.mk
include make_test_alg_bitvector.mk
include make_test_alg_hicuts.mk
include make_test_alg_tuples.mk

.PHONY: utest_all_algs
utest_all_algs: utest_linearsearch utest_bitvector utest_hicuts utest_tuples
	@echo "\nUnit tests of all algorithms completed.\n"
