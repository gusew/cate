ALG_HIC_HPPS_DIR	= $(ALG_HPPS_DIR)hicuts/
ALG_HIC_SRCS_DIR	= $(ALG_SRCS_DIR)hicuts/

# dependencies for different flavors of Linear Search
ALG_HIC_OBJ	= $(ALG_OBJ_BASIC) $(ALG_OBJ_TPL) \
	$(ALG_OBJ_DIR)DataHiCuts.o $(ALG_OBJ_DIR)DataHiCuts%tpl.o $(ALG_OBJ_DIR)HiCuts%tpl.o

# general target for HiCuts*tpl.o
$(ALG_OBJ_DIR)%.o: $(ALG_HIC_SRCS_DIR)%.cpp $(ALG_HIC_HPPS_DIR)%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

# general target for HiCuts*tpl.so
$(ALG_LIB_DIR)HiCuts%tpl.so: $(ALG_HIC_OBJ)
	$(MKDIR)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(INCLUDE) $^ -o $@ $(GMP_BUILD_XXA) $(GMP_BUILD_A)

