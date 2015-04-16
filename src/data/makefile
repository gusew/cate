include $(BASE)/makefile.vars

HPPS	= $(BASE)/$(INCLUDE)/data/

build: $(BASE)/$(BUILD)RuleAtom.o \
	$(BASE)/$(BUILD)RuleSet.o \
	$(BASE)/$(BUILD)VarValue.o \
	$(BASE)/$(BUILD)Benchmark.o \
	$(BASE)/$(BUILD)RandomHeaderConfiguration.o \
	$(BASE)/$(BUILD)RandomDistribution.o \
	$(BASE)/$(BUILD)Configuration.o \
	$(BASE)/$(BUILD)Statistics.o \
	$(BASE)/$(BUILD)Results.o 
	@echo "data built"

$(BASE)/$(BUILD)%.o: %.cpp $(HPPS)%.hpp
	$(CC) $(CFLAGS) -I $(BASE)/$(INCLUDE) -c $< -o $@
