GMP_BUILD_DIR	= $(BUILD)libgmp/
GMP_BUILD_LIB	= $(GMP_BUILD_DIR)lib/
GMP_BUILD_A		= $(GMP_BUILD_LIB)libgmp.a
GMP_BUILD_XXA	= $(GMP_BUILD_LIB)libgmpxx.a 
GMP_LIB_DIR		= ./libgmp/

.PHONY: build_gmp
build_gmp: $(GMP_BUILD_LIB) $(INCLUDE)/gmpxx.h $(INCLUDE)/gmp.h
	@echo "\nGMP library build successfully.\n"

# compile and build static libraries
$(GMP_BUILD_LIB): $(GMP_LIB_DIR)config.m4
	$(MKDIR)
	make -C $(GMP_LIB_DIR)
	make -C $(GMP_LIB_DIR) install

# configure GMP
$(GMP_LIB_DIR)config.m4: 
	cd $(GMP_LIB_DIR); ./configure --enable-cxx --prefix `pwd`/../$(GMP_BUILD_DIR) CFLAGS=-fPIC

# create softlinks to GMP header files
$(INCLUDE)/gmpxx.h:
	-ln -s ../$(GMP_BUILD_DIR)include/gmpxx.h $@

$(INCLUDE)/gmp.h:
	-ln -s ../$(GMP_BUILD_DIR)include/gmp.h $@


.PHONY: clean_gmp
clean_gmp:
	-@rm -f $(INCLUDE)/gmpxx.h
	-@rm -f $(INCLUDE)/gmp.h
	-@rm -rf $(GMP_BUILD_DIR)
#	make -C $(GMP_LIB_DIR) clean
	
