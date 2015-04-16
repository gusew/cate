include $(BASE)/makefile.vars

HPPS	= $(BASE)/$(INCLUDE)/algorithms/
OBJS	= $(BASE)/$(BUILD)AlgFactory.o 

BASIC	= $(BASE)/$(BUILD)Base.o \
	$(BASE)/$(BUILD)VarValue.o

OTPL2	= $(BASE)/$(BUILD)Data2tpl.o \
	$(BASE)/$(BUILD)Converter2tpl.o

OTPL4	= $(BASE)/$(BUILD)Data4tpl.o \
	$(BASE)/$(BUILD)Converter4tpl.o 

OTPL5	= $(BASE)/$(BUILD)Data5tpl.o \
	$(BASE)/$(BUILD)Converter5tpl.o

OTPL10	= $(BASE)/$(BUILD)Data10tpl.o \
	$(BASE)/$(BUILD)Converter10tpl.o

LIBS	= $(BASE)/$(BUILD)LinearSearch5tpl.so \
	$(BASE)/$(BUILD)LinearSearch2tpl.so \
	$(BASE)/$(BUILD)LinearSearch4tpl.so \
	$(BASE)/$(BUILD)LinearSearch10tpl.so \
	$(BASE)/$(BUILD)Bitvector5tpl.so \
	$(BASE)/$(BUILD)Bitvector2tpl.so \
	$(BASE)/$(BUILD)Bitvector4tpl.so \
	$(BASE)/$(BUILD)Bitvector10tpl.so \
	$(BASE)/$(BUILD)HiCuts5tpl.so \
	$(BASE)/$(BUILD)HiCuts2tpl.so \
	$(BASE)/$(BUILD)HiCuts4tpl.so \
	$(BASE)/$(BUILD)HiCuts10tpl.so \
	$(BASE)/$(BUILD)TupleSpace2tpl.so \
	$(BASE)/$(BUILD)TupleSpace4tpl.so \
	$(BASE)/$(BUILD)TupleSpace5tpl.so \
	$(BASE)/$(BUILD)TupleSpace10tpl.so 
	
LIN5_OBJ	= $(OTPL5) $(BASIC) \
	$(BASE)/$(BUILD)LinearSearch5tpl.o

LIN2_OBJ	= $(OTPL2) $(BASIC) \
	$(BASE)/$(BUILD)LinearSearch2tpl.o

LIN4_OBJ	= $(OTPL4) $(BASIC) \
	$(BASE)/$(BUILD)LinearSearch4tpl.o

LIN10_OBJ	= $(OTPL10) $(BASIC) \
	$(BASE)/$(BUILD)LinearSearch10tpl.o

BV5_OBJ = $(OTPL5) $(BASIC) \
	$(BASE)/$(BUILD)DataBitvector.o \
	$(BASE)/$(BUILD)Bitvector5tpl.o

BV2_OBJ = $(OTPL2) $(BASIC) \
	$(BASE)/$(BUILD)DataBitvector.o \
	$(BASE)/$(BUILD)Bitvector2tpl.o

BV4_OBJ = $(OTPL4) $(BASIC) \
	$(BASE)/$(BUILD)DataBitvector.o \
	$(BASE)/$(BUILD)Bitvector4tpl.o

BV10_OBJ = $(OTPL10) $(BASIC) \
	$(BASE)/$(BUILD)DataBitvector.o \
	$(BASE)/$(BUILD)Bitvector10tpl.o

HI5_OBJ = $(OTPL5) $(BASIC) \
	$(BASE)/$(BUILD)DataHiCuts.o \
	$(BASE)/$(BUILD)DataHiCuts5tpl.o \
	$(BASE)/$(BUILD)HiCuts5tpl.o

HI2_OBJ = $(OTPL2) $(BASIC) \
	$(BASE)/$(BUILD)DataHiCuts.o \
	$(BASE)/$(BUILD)DataHiCuts2tpl.o \
	$(BASE)/$(BUILD)HiCuts2tpl.o

HI4_OBJ = $(OTPL4) $(BASIC) \
	$(BASE)/$(BUILD)DataHiCuts.o \
	$(BASE)/$(BUILD)DataHiCuts4tpl.o \
	$(BASE)/$(BUILD)HiCuts4tpl.o

HI10_OBJ = $(OTPL10) $(BASIC) \
	$(BASE)/$(BUILD)DataHiCuts.o \
	$(BASE)/$(BUILD)DataHiCuts10tpl.o \
	$(BASE)/$(BUILD)HiCuts10tpl.o

TS2_OBJ	= $(OTPL2) $(BASIC) \
	$(BASE)/$(BUILD)DataTupleSpace.o \
	$(BASE)/$(BUILD)DataTupleSpace2tpl.o \
	$(BASE)/$(BUILD)TupleSpace2tpl.o

TS4_OBJ	= $(OTPL4) $(BASIC) \
	$(BASE)/$(BUILD)DataTupleSpace.o \
	$(BASE)/$(BUILD)DataTupleSpace4tpl.o \
	$(BASE)/$(BUILD)TupleSpace4tpl.o

TS5_OBJ	= $(OTPL5) $(BASIC) \
	$(BASE)/$(BUILD)DataTupleSpace.o \
	$(BASE)/$(BUILD)DataTupleSpace5tpl.o \
	$(BASE)/$(BUILD)TupleSpace5tpl.o

TS10_OBJ	= $(OTPL10) $(BASIC) \
	$(BASE)/$(BUILD)DataTupleSpace.o \
	$(BASE)/$(BUILD)DataTupleSpace10tpl.o \
	$(BASE)/$(BUILD)TupleSpace10tpl.o

build: $(OBJS) $(LIBS)
	@echo "algorithms built"

$(BASE)/$(BUILD)%.o: %.cpp $(HPPS)%.hpp
	$(CC) $(CFLAGS) -I $(BASE)/$(INCLUDE) -c $< -o $@

$(BASE)/$(BUILD)LinearSearch5tpl.so: $(LIN5_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(LIN5_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)LinearSearch2tpl.so: $(LIN2_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(LIN2_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)LinearSearch4tpl.so: $(LIN4_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(LIN4_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)LinearSearch10tpl.so: $(LIN10_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(LIN10_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)Bitvector2tpl.so: $(BV2_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(BV2_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)Bitvector5tpl.so: $(BV5_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(BV5_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)Bitvector4tpl.so: $(BV4_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(BV4_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)Bitvector10tpl.so: $(BV10_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(BV10_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)HiCuts5tpl.so: $(HI5_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(HI5_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)HiCuts2tpl.so: $(HI2_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(HI2_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)HiCuts4tpl.so: $(HI4_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(HI4_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)HiCuts10tpl.so: $(HI10_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(HI10_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)TupleSpace2tpl.so: $(TS2_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(TS2_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)TupleSpace4tpl.so: $(TS4_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(TS4_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)TupleSpace5tpl.so: $(TS5_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(TS5_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

$(BASE)/$(BUILD)TupleSpace10tpl.so: $(TS10_OBJ)
	$(CC) $(CFLAGS) $(CSOFLAGS) -I $(BASE)/$(INCLUDE) $(TS10_OBJ) -o $@ $(BASE)/$(GMPLIBXX) $(BASE)/$(GMPLIB)

