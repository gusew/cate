LUA_BUILD_DIR	= $(BUILD)liblua/
LUA_BUILD_A		= $(LUA_BUILD_DIR)liblua.a
LUA_LIB_DIR		= ./liblua/
LUA_LIB_A		= $(LUA_LIB_DIR)liblua.a


.PHONY: build_lua
build_lua: $(LUA_BUILD_A) $(INCLUDE)/liblua
	@echo "\nLua library build successfully.\n"

# copy static library to build dir
$(LUA_BUILD_A): $(LUA_LIB_A)
	$(MKDIR)
	cp $(LUA_LIB_A) $(LUA_BUILD_A)

# compile and build lua-lib (recursively)
$(LUA_LIB_A):
	make -C $(LUA_LIB_DIR) linux

# create a softlink to lua-headerfiles
$(INCLUDE)/liblua:
	-ln -s ../$(LUA_LIB_DIR). $@

.PHONY: clean_lua
clean_lua:
	-@rm -f $(INCLUDE)/liblua
	-@rm -rf $(LUA_BUILD_DIR)
#	make -C $(LUA_LIB_DIR) clean

