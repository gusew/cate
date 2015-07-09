# listing of all relevant object files
TEST_ALG_HIC	= $(OBJ_MEM) $(OBJ_CHRONO) $(OBJ_DATA) \
	$(CATE_OBJ_DIR)Base.o \
	$(ALG_OBJ_DIR)Data5tpl.o \
	$(ALG_OBJ_DIR)Converter5tpl.o \
	$(ALG_OBJ_DIR)DataHiCuts.o \
	$(ALG_OBJ_DIR)DataHiCuts5tpl.o \
	$(ALG_OBJ_DIR)HiCuts5tpl.o \
	$(TEST_OBJ_DIR)AlgTestFixtures.o \
	$(TEST_OBJ_DIR)AlgDataHiCuts5tpl.o \
	$(TEST_OBJ_DIR)AlgHiCuts5tpl.o


.PHONY: utest_hicuts

# build test suite for the algorithm
utest_hicuts: $(TEST_EXEC_DIR)$(APPNAME)_utest_hicuts $(TEST_RUNNER)
	$(TEST_RUNNER) $< -v

$(TEST_EXEC_DIR)$(APPNAME)_utest_hicuts: $(TEST_BUILD_SO) $(INCLUDE)/libunittest build_gmp $(TEST_ALG_HIC)
	$(MKDIR)
	$(CC) -pthread $(TEST_BUILD_SO) $(TEST_ALG_HIC) -o $@ $(GMP_BUILD_XXA) $(GMP_BUILD_A)

