include $(BASE)/makefile.vars

HPPS	= $(BASE)/$(INCLUDE)/frontend/
OBJS	= $(BASE)/$(BUILD)FilesysHelper.o \
	$(BASE)/$(BUILD)Shell.o \
	$(BASE)/$(BUILD)Web.o \
	$(BASE)/$(BUILD)OutputResults.o

build: $(OBJS)
	@echo "frontend built"

$(BASE)/$(BUILD)%.o: %.cpp $(HPPS)%.hpp
	$(CC) $(CFLAGS) -I $(BASE)/$(INCLUDE) -c $< -o $@
