include $(BASE)/makefile.vars

OBJS	= $(BASE)/$(BUILD)test/RuleSet.o \
	$(BASE)/$(BUILD)test/AlgTestFixtures.o \
	$(BASE)/$(BUILD)test/AlgData5tpl.o \
	$(BASE)/$(BUILD)test/AlgLinearSearch5tpl.o \
	$(BASE)/$(BUILD)test/VarValue.o \
	$(BASE)/$(BUILD)test/MemSnapshot.o \
	$(BASE)/$(BUILD)test/MemManager.o \
	$(BASE)/$(BUILD)test/MemTrace.o \
	$(BASE)/$(BUILD)test/ChronoManager.o \
	$(BASE)/$(BUILD)test/FilesysHelper.o \
	$(BASE)/$(BUILD)test/LuaConfigurator.o \
	$(BASE)/$(BUILD)test/HeaderGenerator.o \
	$(BASE)/$(BUILD)test/RandomNumberGenerator.o \
	$(BASE)/$(BUILD)test/AlgDataBitvector.o \
	$(BASE)/$(BUILD)test/AlgBitvector5tpl.o \
	$(BASE)/$(BUILD)test/Statistics.o \
	$(BASE)/$(BUILD)test/AlgDataHiCuts5tpl.o \
	$(BASE)/$(BUILD)test/AlgHiCuts5tpl.o \
	$(BASE)/$(BUILD)test/AlgTupleSpace5tpl.o 

build: $(OBJS)
	@echo "unittests built"

$(BASE)/$(BUILD)test/%.o: %.cpp
	$(CC) $(UNITTFLAGS) -I $(BASE)/$(INCLUDE) -c $< -o $@
