# listing of all dependencies for each part of CATE test suite
TEST_SET_1	= $(OBJ_DATA) \
	$(TEST_OBJ_DIR)RuleSet.o 

TEST_SET_2	= $(OBJ_DATA) \
	$(TEST_OBJ_DIR)VarValue.o 

TEST_SET_3	= $(OBJ_MEM) \
	$(ALG_OBJ_DIR)Data5tpl.o \
	$(TEST_OBJ_DIR)AlgTestFixtures.o \
	$(TEST_OBJ_DIR)AlgData5tpl.o

TEST_SET_4	= $(OBJ_MEM) \
	$(TEST_OBJ_DIR)MemSnapshot.o \
	$(TEST_OBJ_DIR)MemManager.o \
	$(TEST_OBJ_DIR)MemTrace.o 

TEST_SET_5	= $(OBJ_CHRONO) \
	$(TEST_OBJ_DIR)ChronoManager.o

TEST_SET_6	= $(CATE_OBJ_DIR)FilesysHelper.o \
	$(TEST_OBJ_DIR)FilesysHelper.o

TEST_SET_7	= $(OBJ_DATA) \
	$(CATE_OBJ_DIR)LuaConfigurator.o \
	$(TEST_OBJ_DIR)LuaConfigurator.o

TEST_SET_8	= $(OBJ_RNDGEN) \
	$(TEST_OBJ_DIR)HeaderGenerator.o \
	$(TEST_OBJ_DIR)RandomNumberGenerator.o 

TEST_SET_9	= $(OBJ_DATA) \
	$(CATE_OBJ_DIR)Statistics.o \
	$(TEST_OBJ_DIR)Statistics.o

TEST_SET_10	= $(OBJ_LOGTAG) \
	$(TEST_OBJ_DIR)LogTagManager.o


# all object files for unit tests (algorithms excluded)
TEST_OBJS	= $(sort $(TEST_SET_1) $(TEST_SET_2) $(TEST_SET_3) $(TEST_SET_4) $(TEST_SET_5) $(TEST_SET_6) $(TEST_SET_7) $(TEST_SET_8) $(TEST_SET_9) $(TEST_SET_10))


.PHONY: utest 

# run compiled and linked executable with all unittests
utest: $(TEST_EXEC_DIR)$(APPNAME)_unittests $(TEST_RUNNER)
	$(TEST_RUNNER) $< -v

# build complete test suite for CATE
$(TEST_EXEC_DIR)$(APPNAME)_unittests: $(TEST_BUILD_SO) $(INCLUDE)/libunittest build_gmp $(TEST_OBJS)
	$(MKDIR)
	$(CC) -pthread $(TEST_BUILD_SO) $(TEST_OBJS) -o $@ $(GMP_BUILD_XXA) $(GMP_BUILD_A)

.PHONY: clean_utest
clean_utest:
	-@rm -rf $(BUILD)libunittest/
	-@rm -f $(INCLUDE)libunittest
	-@rm -rf $(TEST_EXEC_DIR)

