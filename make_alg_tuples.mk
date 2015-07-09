ALG_TUP_HPPS_DIR	= $(ALG_HPPS_DIR)tuples/
ALG_TUP_SRCS_DIR	= $(ALG_SRCS_DIR)tuples/

# dependencies for different flavors of Linear Search
ALG_TUP_OBJ	= $(ALG_OBJ_BASIC) $(ALG_OBJ_TPL) \
	$(ALG_OBJ_DIR)DataTupleSpace.o $(ALG_OBJ_DIR)DataTupleSpace%tpl.o $(ALG_OBJ_DIR)TupleSpace%tpl.o

# general target for TupleSpace*tpl.o and DataTupleSpace*
$(ALG_OBJ_DIR)%.o: $(ALG_TUP_SRCS_DIR)%.cpp $(ALG_TUP_HPPS_DIR)%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

# Specific targets for TupleSpace*tpl.so
$(ALG_LIB_DIR)TupleSpace%tpl.so: $(ALG_TUP_OBJ)
	$(MKDIR)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(INCLUDE) $^ -o $@ $(GMP_BUILD_XXA) $(GMP_BUILD_A)

