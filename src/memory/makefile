include $(BASE)/makefile.vars

HPPS	= $(BASE)/$(INCLUDE)/memory/
OBJS	= $(BASE)/$(BUILD)MemManager.o \
	$(BASE)/$(BUILD)MemTraceRegistry.o \
	$(BASE)/$(BUILD)MemTrace.o \
	$(BASE)/$(BUILD)MemTraceData.o \
	$(BASE)/$(BUILD)Registry.o \
	$(BASE)/$(BUILD)MemSnapshot.o

build: $(OBJS) 
	@echo "memory built"

$(BASE)/$(BUILD)%.o: %.cpp $(HPPS)%.hpp
	$(CC) $(CFLAGS) -I $(BASE)/$(INCLUDE) -c $< -o $@
