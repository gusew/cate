ALG_BV_HPPS_DIR	= $(ALG_HPPS_DIR)bitvector/
ALG_BV_SRCS_DIR	= $(ALG_SRCS_DIR)bitvector/

# dependencies for different flavors of Bit Vector
ALG_BV_OBJ	= $(ALG_OBJ_BASIC) $(ALG_OBJ_TPL) $(ALG_OBJ_DIR)DataBitvector.o $(ALG_OBJ_DIR)Bitvector%tpl.o

# general target for Bitvector*tpl.o and DataBitvector.o
$(ALG_OBJ_DIR)%.o: $(ALG_BV_SRCS_DIR)%.cpp $(ALG_BV_HPPS_DIR)%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

# general target for Bitvector*tpl.so
$(ALG_LIB_DIR)Bitvector%tpl.so: $(ALG_BV_OBJ)
	$(MKDIR)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(INCLUDE) $^ -o $@ $(GMP_BUILD_XXA) $(GMP_BUILD_A)

