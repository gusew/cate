# listing of all relevant object files
TEST_ALG_TS		= $(OBJ_MEM) $(OBJ_CHRONO) $(OBJ_DATA) \
	$(CATE_OBJ_DIR)Base.o \
	$(ALG_OBJ_DIR)Data5tpl.o \
	$(ALG_OBJ_DIR)Converter5tpl.o \
	$(ALG_OBJ_DIR)TupleSpace5tpl.o \
	$(TEST_OBJ_DIR)AlgTestFixtures.o \
	$(TEST_OBJ_DIR)AlgTupleSpace5tpl.o


.PHONY: utest_tuples

# build test suite for the algorithm
utest_tuples: $(TEST_EXEC_DIR)$(APPNAME)_utest_tuples
	./run_test.sh $< -v

$(TEST_EXEC_DIR)$(APPNAME)_utest_tuples: $(TEST_BUILD_SO) $(INCLUDE)/libunittest build_gmp $(TEST_ALG_TS)
	$(MKDIR)
	$(CC) -pthread $(TEST_BUILD_SO) $(TEST_ALG_TS) -o $@ $(GMP_BUILD_XXA) $(GMP_BUILD_A)

