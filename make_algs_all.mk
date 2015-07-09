# PLEASE ADD YOUR ALGORITHM IMPLEMENTATION TO THIS LIST:
ALG_LIST	= \
	$(ALG_LIB_DIR)LinearSearch2tpl.so \
	$(ALG_LIB_DIR)LinearSearch4tpl.so \
	$(ALG_LIB_DIR)LinearSearch5tpl.so \
	$(ALG_LIB_DIR)LinearSearch10tpl.so \
	$(ALG_LIB_DIR)Bitvector2tpl.so \
	$(ALG_LIB_DIR)Bitvector4tpl.so \
	$(ALG_LIB_DIR)Bitvector5tpl.so \
	$(ALG_LIB_DIR)Bitvector10tpl.so \
	$(ALG_LIB_DIR)HiCuts2tpl.so \
	$(ALG_LIB_DIR)HiCuts4tpl.so \
	$(ALG_LIB_DIR)HiCuts5tpl.so \
	$(ALG_LIB_DIR)HiCuts10tpl.so \
	$(ALG_LIB_DIR)TupleSpace2tpl.so \
	$(ALG_LIB_DIR)TupleSpace4tpl.so \
	$(ALG_LIB_DIR)TupleSpace5tpl.so \
	$(ALG_LIB_DIR)TupleSpace10tpl.so 

# Makefiles with targets for each algorithm implementation.
include make_alg_common.mk
include make_alg_linear.mk
include make_alg_bitvector.mk
include make_alg_hicuts.mk
include make_alg_tuples.mk
# PLEASE ADD YOUR MAKEFILE FOR YOUR CUSTOM ALGORITHM HERE

