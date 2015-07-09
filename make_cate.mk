CATE_OBJ_DIR	= $(BUILD)obj/
CATE_EXEC_DIR	= $(BUILD)cate/
# TODO make both versions of cate: memory and time
CATE_EXEC		= $(CATE_EXEC_DIR)$(APPNAME)

# define necessary object files
OBJ_MEM		= \
	$(CATE_OBJ_DIR)MemTrace.o \
	$(CATE_OBJ_DIR)MemTraceData.o \
	$(CATE_OBJ_DIR)MemManager.o \
	$(CATE_OBJ_DIR)Registry.o \
	$(CATE_OBJ_DIR)MemTraceRegistry.o \
	$(CATE_OBJ_DIR)MemSnapshot.o \

OBJ_CHRONO	= \
	$(CATE_OBJ_DIR)ChronoManager.o \
	$(CATE_OBJ_DIR)Chronograph.o

OBJ_DATA	= \
	$(CATE_OBJ_DIR)VarValue.o \
	$(CATE_OBJ_DIR)RuleSet.o \
	$(CATE_OBJ_DIR)RuleAtom.o \
	$(CATE_OBJ_DIR)Benchmark.o \
	$(CATE_OBJ_DIR)Configuration.o \
	$(CATE_OBJ_DIR)Statistics.o \
	$(CATE_OBJ_DIR)Results.o

OBJ_RNDGEN	= \
	$(CATE_OBJ_DIR)RandomNumberGenerator.o \
	$(CATE_OBJ_DIR)HeaderGenerator.o \
	$(CATE_OBJ_DIR)RandomHeaderConfiguration.o

# all object files for main-program
OBJFILES	= $(OBJ_MEM) $(OBJ_CHRONO) $(OBJ_DATA) $(OBJ_RNDGEN) \
	$(CATE_OBJ_DIR)main.o \
	$(CATE_OBJ_DIR)AlgFactory.o \
	$(CATE_OBJ_DIR)LuaInterpreter.o \
	$(CATE_OBJ_DIR)LuaConfigurator.o \
	$(CATE_OBJ_DIR)Shell.o \
	$(CATE_OBJ_DIR)Web.o \
	$(CATE_OBJ_DIR)FilesysHelper.o \
	$(CATE_OBJ_DIR)BenchmarkExecutor.o \
	$(CATE_OBJ_DIR)OutputResults.o \
	$(CATE_OBJ_DIR)Evaluator.o

# define all files to copy to executable directory
CP_LUA	= \
	$(SRCDIR)lua/preprocess.lua \
	$(SRCDIR)lua/postprocess.lua

CP_HTML	= \
	$(SRCDIR)html/jqplot.js \
	$(SRCDIR)html/jqplot.css

#-----------------------------------------------------------------------

.PHONY: build_cate 
build_cate: $(CATE_EXEC) copy_files
	@echo "\nCATE executable build successfully.\n"

$(CATE_EXEC): build_libs $(OBJFILES) 
	$(MKDIR)
	$(CC) $(CFLAGS) $(OBJFILES) -o $@ $(LUA_LIB_A) $(GMP_BUILD_XXA) $(GMP_BUILD_A) -ldl

# general targets for compiling the object files
$(CATE_OBJ_DIR)%.o: $(SRCDIR)generics/%.cpp $(INCLUDE)/generics/%.hpp 
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(CATE_OBJ_DIR)%.o: $(SRCDIR)core/%.cpp $(INCLUDE)/core/%.hpp 
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(CATE_OBJ_DIR)%.o: $(SRCDIR)data/%.cpp $(INCLUDE)/data/%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(CATE_OBJ_DIR)%.o: $(SRCDIR)configuration/%.cpp $(INCLUDE)/configuration/%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(CATE_OBJ_DIR)%.o: $(SRCDIR)frontend/%.cpp $(INCLUDE)/frontend/%.hpp 
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(CATE_OBJ_DIR)%.o: $(SRCDIR)memory/%.cpp $(INCLUDE)/memory/%.hpp 
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(CATE_OBJ_DIR)%.o: $(SRCDIR)%.cpp 
	$(MKDIR)
	$(CC) $(CFLAGS) $(VERSION_FLAGS) -I $(INCLUDE) -c $< -o $@

#-----------------------------------------------------------------------

.PHONY: copy_files copy_lua copy_html
# copy static files
copy_files: copy_lua copy_html
	@echo "\nStatic files copied successfully.\n"

# copy Lua pre- and postprocess files
copy_lua: $(CP_LUA)
	@mkdir -p $(CATE_EXEC_DIR)
	cp $^ $(CATE_EXEC_DIR)

# copy jqPlot files for evaluation summary
copy_html: $(CP_HTML)
	@mkdir -p $(CATE_EXEC_DIR)
	cp $^ $(CATE_EXEC_DIR)

#-----------------------------------------------------------------------

.PHONY: clean_cate
clean_cate:
	-@rm -rf $(CATE_EXEC_DIR)
	-@rm -rf $(CATE_OBJ_DIR)

