ALG_BUILD_DIR	= $(BUILD)algorithms/
ALG_OBJ_DIR		= $(ALG_BUILD_DIR)obj/
ALG_LIB_DIR		= $(ALG_BUILD_DIR)lib/
ALG_HPPS_DIR	= $(INCLUDE)/algorithms/
ALG_SRCS_DIR	= $(SRCDIR)algorithms/

# Makefile targets for all existing algorithms (defines ALG_LIST).
include make_algs_all.mk


.PHONY: build_algorithms
build_algorithms: $(ALG_LIST)
	@echo "\nAlgorithms build successfully.\n"

.PHONY: clean_algorithms
clean_algorithms:
	-@rm -rf $(ALG_BUILD_DIR)
	
