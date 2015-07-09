ALG_LIN_HPPS_DIR	= $(ALG_HPPS_DIR)linear/
ALG_LIN_SRCS_DIR	= $(ALG_SRCS_DIR)linear/

# dependencies for different flavors of Linear Search
ALG_LIN_OBJ	= $(ALG_OBJ_BASIC) $(ALG_OBJ_TPL) $(ALG_OBJ_DIR)LinearSearch%tpl.o

# general target for LinearSearch*tpl.o
$(ALG_OBJ_DIR)%.o: $(ALG_LIN_SRCS_DIR)%.cpp $(ALG_LIN_HPPS_DIR)%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

# general target for LinearSearch*tpl.so
$(ALG_LIB_DIR)LinearSearch%tpl.so: $(ALG_LIN_OBJ)
	$(MKDIR)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(INCLUDE) $^ -o $@ $(GMP_BUILD_XXA) $(GMP_BUILD_A)

