include makefile.vars
include version.vars
#-----------------------------------------------------------------------
# run compiled and linked executable of main program
run: $(BUILD)$(APPNAME) copy_lua copy_html
	$(BUILD)$(APPNAME)

# link all compiled object files to main program
$(BUILD)$(APPNAME): $(LUALIB) $(INCLUDE)/liblua $(GMPLIB) $(INCLUDE)/gmpxx.h $(INCLUDE)/gmp.h build_dir build_recursive
	$(CC) $(CFLAGS) $(OBJFILES) -o $@ $(LUALIB) $(GMPLIBXX) $(GMPLIB) -ldl

# create directory for compiled files
build_dir:
	@mkdir -p $(BUILD)

# call make recursively for each src-subfolder
build_recursive:
	make -C $(SRCDIR) BASE=`pwd` build

#-----------------------------------------------------------------------
# compile and build lua-lib (recursively)
$(LUALIB):
	make -C $(LUALIBDIR) linux

# create a softlink to lua-headerfiles
$(INCLUDE)/liblua:
	-ln -s ../$(LUALIBDIR). $(INCLUDE)/liblua

copy_lua: $(CP_LUA)
	-cp $(CP_LUA) $(BUILD)

#-----------------------------------------------------------------------
# copy necessary html-files for evaluation creation
copy_html: $(CP_HTML)
	-cp $(CP_HTML) $(BUILD)
#-----------------------------------------------------------------------
# configure and build gmp-library (recursively)
$(GMPLIB):
	@mkdir -p $(GMPLIBDIR)build
	cd $(GMPLIBDIR); ./configure --enable-cxx --prefix `pwd`/build/ CFLAGS=-fPIC
	make -C $(GMPLIBDIR)
	make -C $(GMPLIBDIR) install

# create a softlink to gmp-headerfiles
$(INCLUDE)/gmpxx.h:
	-ln -s ../$(GMPLIBDIR)/build/include/gmpxx.h $(INCLUDE)/gmpxx.h

$(INCLUDE)/gmp.h:
	-ln -s ../$(GMPLIBDIR)/build/include/gmp.h $(INCLUDE)/gmp.h

#-----------------------------------------------------------------------

# run compiled and linked executable with all unittests
utest: build_test_dir $(BUILD)test/$(APPNAME)_unittest
	./run_test.sh $(BUILD)test/$(APPNAME)_unittest -v

utest_linearsearch: build_test_dir $(BUILD)test/$(APPNAME)_unittest_linearsearch
	./run_test.sh $(BUILD)test/$(APPNAME)_unittest_linearsearch -v

utest_bitvector: build_test_dir $(BUILD)test/$(APPNAME)_unittest_bitvector
	./run_test.sh $(BUILD)test/$(APPNAME)_unittest_bitvector -v

utest_hicuts: build_test_dir $(BUILD)test/$(APPNAME)_unittest_hicuts
	./run_test.sh $(BUILD)test/$(APPNAME)_unittest_hicuts -v

utest_tuples: build_test_dir $(BUILD)test/$(APPNAME)_unittest_tuples
	./run_test.sh $(BUILD)test/$(APPNAME)_unittest_tuples -v

# create directory for compilated unittest-files
build_test_dir:
	@mkdir -p $(BUILD)test

# link all compilated object files for unittest to one executable
$(BUILD)test/$(APPNAME)_unittest: $(UNITTLIB) $(INCLUDE)/libunittest $(INCLUDE)/liblua $(GMPLIB) $(INCLUDE)/gmpxx.h $(INCLUDE)/gmp.h build_recursive build_test_recursive
	$(CC) -pthread $(UNITTLIB) $(UNITTOBJS) -o $@ $(GMPLIBXX) $(GMPLIB)

$(BUILD)test/$(APPNAME)_unittest_linearsearch: $(UNITTLIB) $(INCLUDE)/libunittest $(INCLUDE)/liblua $(GMPLIB) $(INCLUDE)/gmpxx.h $(INCLUDE)/gmp.h build_recursive build_test_recursive
	$(CC) -pthread $(UNITTLIB) $(UNITTALG1) -o $@ $(GMPLIBXX) $(GMPLIB)

$(BUILD)test/$(APPNAME)_unittest_bitvector: $(UNITTLIB) $(INCLUDE)/libunittest $(INCLUDE)/liblua $(GMPLIB) $(INCLUDE)/gmpxx.h $(INCLUDE)/gmp.h build_recursive build_test_recursive
	$(CC) -pthread $(UNITTLIB) $(UNITTALG2) -o $@ $(GMPLIBXX) $(GMPLIB)

$(BUILD)test/$(APPNAME)_unittest_hicuts: $(UNITTLIB) $(INCLUDE)/libunittest $(INCLUDE)/liblua $(GMPLIB) $(INCLUDE)/gmpxx.h $(INCLUDE)/gmp.h build_recursive build_test_recursive
	$(CC) -pthread $(UNITTLIB) $(UNITTALG3) -o $@ $(GMPLIBXX) $(GMPLIB)

$(BUILD)test/$(APPNAME)_unittest_tuples: $(UNITTLIB) $(INCLUDE)/libunittest $(INCLUDE)/liblua $(GMPLIB) $(INCLUDE)/gmpxx.h $(INCLUDE)/gmp.h build_recursive build_test_recursive
	$(CC) -pthread $(UNITTLIB) $(UNITTALG4) -o $@ $(GMPLIBXX) $(GMPLIB)

# compile and build a shared library file for unittesting (recursively)
$(UNITTLIB):
	make -C $(UNITTLIBDIR)

# create a softlink to headerfiles of library for unittesting
$(INCLUDE)/libunittest:
	-ln -s ../$(UNITTLIBDIR)include/libunittest $(INCLUDE)/libunittest

# call make recursively for test-subfolder
build_test_recursive:
	make -C $(UNITTDIR) BASE=`pwd` build
#-----------------------------------------------------------------------
valtest: $(BUILD)$(APPNAME)
	valgrind $(VALFLAGS) $(BUILD)$(APPNAME)
#-----------------------------------------------------------------------
# export all required files for executing cate in another directory
export: $(EXECFILES) build_export_dir
	-cp $(EXECFILES) $(EXPORT)
	@echo "All required program files were copied to '"$(EXPORT)"'."

build_export_dir:
	@mkdir -p $(EXPORT)
#-----------------------------------------------------------------------
clean:
	-@rm -r $(BUILD)
	-@rm -f $(INCLUDE)/libunittest
	-@rm -f $(INCLUDE)/liblua
	-@rm -f $(INCLUDE)/gmpxx.h
	-@rm -f $(INCLUDE)/gmp.h

cleanlib:
	make -C $(UNITTLIBDIR) clean
	make -C $(LUALIBDIR) clean
	make -C $(GMPLIBDIR) clean
	-@rm -fr $(GMPLIBDIR)build

