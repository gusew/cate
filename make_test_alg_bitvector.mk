# listing of all relevant object files
TEST_ALG_BV		= $(OBJ_MEM) $(OBJ_CHRONO) $(OBJ_DATA) \
	$(CATE_OBJ_DIR)Base.o \
	$(ALG_OBJ_DIR)Data5tpl.o \
	$(ALG_OBJ_DIR)Converter5tpl.o \
	$(ALG_OBJ_DIR)DataBitvector.o \
	$(ALG_OBJ_DIR)Bitvector5tpl.o \
	$(TEST_OBJ_DIR)AlgTestFixtures.o \
	$(TEST_OBJ_DIR)AlgDataBitvector.o \
	$(TEST_OBJ_DIR)AlgBitvector5tpl.o

.PHONY: utest_bitvector

# build test suite for the algorithm
utest_bitvector: $(TEST_EXEC_DIR)$(APPNAME)_utest_bitvector
	./run_test.sh $< -v

$(TEST_EXEC_DIR)$(APPNAME)_utest_bitvector: $(TEST_BUILD_SO) $(INCLUDE)/libunittest build_gmp $(TEST_ALG_BV)
	$(MKDIR)
	$(CC) -pthread $(TEST_BUILD_SO) $(TEST_ALG_BV) -o $@ $(GMP_BUILD_XXA) $(GMP_BUILD_A)

