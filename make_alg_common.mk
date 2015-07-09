ALG_COMM_HPPS_DIR	= $(ALG_HPPS_DIR)common/
ALG_COMM_SRCS_DIR	= $(ALG_SRCS_DIR)common/

ALG_OBJ_BASE		= $(CATE_OBJ_DIR)Base.o
ALG_OBJ_VARVALUE	= $(CATE_OBJ_DIR)VarValue.o
ALG_OBJ_BASIC		= $(ALG_OBJ_BASE) $(ALG_OBJ_VARVALUE)

ALG_OBJ_TPL			= $(ALG_OBJ_DIR)Data%tpl.o $(ALG_OBJ_DIR)Converter%tpl.o

# general target for common algorithm object files
$(ALG_OBJ_DIR)%.o: $(ALG_COMM_SRCS_DIR)%.cpp $(ALG_COMM_HPPS_DIR)%.hpp
	$(MKDIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

