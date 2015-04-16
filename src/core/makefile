include $(BASE)/makefile.vars

HPPS	= $(BASE)/$(INCLUDE)/core/
OBJS	= $(BASE)/$(BUILD)Chronograph.o \
	$(BASE)/$(BUILD)ChronoManager.o \
	$(BASE)/$(BUILD)LuaInterpreter.o \
	$(BASE)/$(BUILD)LuaConfigurator.o \
	$(BASE)/$(BUILD)BenchmarkExecutor.o \
	$(BASE)/$(BUILD)RandomNumberGenerator.o \
	$(BASE)/$(BUILD)HeaderGenerator.o \
	$(BASE)/$(BUILD)Evaluator.o 

build: $(OBJS)
	@echo "core built"

$(BASE)/$(BUILD)%.o: %.cpp $(HPPS)%.hpp
	$(CC) $(CFLAGS) -I $(BASE)/$(INCLUDE) -c $< -o $@
