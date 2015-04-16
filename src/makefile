include $(BASE)/makefile.vars
include $(BASE)/version.vars

build: $(BASE)/$(BUILD)main.o 
	make -C ./data/ BASE=$(BASE) build
	make -C ./algorithms/ BASE=$(BASE) build
	make -C ./core/ BASE=$(BASE) build
	make -C ./memory/ BASE=$(BASE) build
	make -C ./frontend/ BASE=$(BASE) build
	
$(BASE)/$(BUILD)%.o: %.cpp
	$(CC) $(CFLAGS) $(VERSION_FLAGS) -I $(BASE)/$(INCLUDE) -c $< -o $@
