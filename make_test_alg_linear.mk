# listing of all relevant object files
TEST_ALG_LIN	= $(OBJ_MEM) $(OBJ_CHRONO) $(OBJ_DATA) \
	$(CATE_OBJ_DIR)Base.o \
	$(ALG_OBJ_DIR)Data5tpl.o \
	$(ALG_OBJ_DIR)Converter5tpl.o \
	$(ALG_OBJ_DIR)LinearSearch5tpl.o \
	$(TEST_OBJ_DIR)AlgTestFixtures.o \
	$(TEST_OBJ_DIR)AlgLinearSearch5tpl.o

.PHONY: utest_linearsearch

# build test suite for the algorithm
utest_linearsearch: $(TEST_EXEC_DIR)$(APPNAME)_utest_linearsearch $(TEST_RUNNER)
	$(TEST_RUNNER) $< -v

$(TEST_EXEC_DIR)$(APPNAME)_utest_linearsearch: $(TEST_BUILD_SO) $(INCLUDE)/libunittest build_gmp $(TEST_ALG_LIN)
	$(MKDIR)
	$(CC) -pthread $(TEST_BUILD_SO) $(TEST_ALG_LIN) -o $@ $(GMP_BUILD_XXA) $(GMP_BUILD_A)

