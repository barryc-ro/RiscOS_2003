ifneq ($(CLEARCASE_ROOT),)
   SRCHOME        = /vobs
else
   t              = $(subst /, ,$(shell pwd))
   SRCHOME        = /$(word 1,$t)/$(word 2,$t)/$(word 3,$t)
endif
SRC	  = $(SRCHOME)/mx/src

include $(SRC)/buildtools/defaults.mk

INCLUDE	  =	-I$(SRCHOME)/mx/pub \
		-I$(SRCHOME)/mx/inc \
		-I$(SRCHOME)/mx/src/inc \
		-I$(SRCHOME)/mx/src/ys/sosd/$(ARCH) \
		-I$(SRCHOME)/mx/src/mt/sosd/$(ARCH) \
		-I$(SRCHOME)/mx/src/mn \
		-I$(SRCHOME)/mx/src/mn/sosd/$(ARCH) \
		-I$(SRCHOME)/mx/src/mt

COMPONENT = Media Net

S0MZMAIN  = $(SRCHOME)/mx/src/mt/sosd/$(ARCH)/s0mzmain.c

MNSRC	  = $(SRC)/mn
MNOSD     = $(MNSRC)/sosd/$(ARCH)

$(SRCHOME)/mx/src/mesg/%.mot : $(MNSRC)/mesg/%.msg
	$(MSGC) -I $(SRCHOME)/mx/src/mesg -o $@ $<

vpath %.c       $(MNOSD) $(MNSRC)
vpath %.a       $(LIB)
vpath %.$(SHX)  $(LIB)

LIBMN	  = $(LIB)/libmn.a
LIBS      = $(LIBMN)
SHLIBS    = $(LIB)/libmn.$(SHX)
LOADLIBES = -lmn -lmt -lmn -lys

#Base objects
GENOBJS = mn.o mna.o mne.o mnm.o mnn.o mno.o mnt.o mntr.o mnts.o

# RPC objects
RPCOBJS = mnr.o mnrc.o mnrs.o mnx.o mznt.o mnb.o mzmgs.o

# Name server objects
NSOBJS  = mzn.o mznc.o

# Process server objects
SCOBJS = mnsc.o mnscc.o

# Utility objects
UTLOBJS = mnpsl.o

# OSD objects
OSDOBJS   = smnudp.o smni.o smng.o smnj.o
OSDOBJS  += $(SS_MN_OSDOBJS)

LIBOBJS   = $(GENOBJS) $(RPCOBJS) $(NSOBJS) $(SCOBJS) $(UTLOBJS) $(OSDOBJS)
LIBOBJS  += $(SS_MN_LIBOBJS)

OBJS	  = $(OBJ)/mnars.o $(OBJ)/mnrm.o $(OBJ)/mnls.o $(OBJ)/mnln.o \
	    $(OBJ)/mnscmd.o $(OBJ)/mnping.o $(OBJ)/mnps.o $(OBJ)/mnrid.o \
	    $(OBJ)/mnscs.o $(OBJ)/mzns.o $(OBJ)/s0mnars.o $(OBJ)/s0mnjoin.o \
	    $(OBJ)/s0mnln.o $(OBJ)/s0mnls.o $(OBJ)/s0mnping.o $(OBJ)/s0mnps.o \
	    $(OBJ)/s0mnrid.o $(OBJ)/s0mnrm.o $(OBJ)/s0mnscmd.o \
	    $(OBJ)/s0mnsc.o $(OBJ)/s0mznm.o $(OBJ)/mnjoin.o \
            $(OBJ)/mnscsop.o $(OBJ)/mznops.o
OBJS     += $(SS_MN_OBJS)

BINS      = $(BIN)/mnaddrsrv $(BIN)/mnrpcnmsrv $(BIN)/mnprocsrv $(BIN)/mnping \
            $(BIN)/mnls $(BIN)/mnps $(BIN)/mnrpcid $(BIN)/mnscmd $(BIN)/mnln \
            $(BIN)/mnrm $(BIN)/mnjoin
BINS     += $(SS_MN_BINS)

MSGDIR	= $(SRC)/mesg
MSGS	= $(MSGDIR)/OMNMN.mot

compile  : $(OBJS)

lib	 : $(LIBS)

share    : $(SHLIBS)

link	 : $(BINS) $(MSGS)

resource : $(MSGS)

cleanobj :
	rm -f $(LIBOBJS) $(OBJS)

cleanlib :
	rm -f $(LIBS) $(SHLIBS)

cleanbin :
	rm -f $(BINS)

clean::
	rm -f $(MSGS)

$(LIBMN) : $(LIBMN)($(LIBOBJS))
	$(RANLIB) $@

$(OBJ)/%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

$(BIN)/%: $(OBJ)/%.o
	$(LINK.o) $+ $(LDLIBS) -o $@

# If you're statically linking, you'll need to relink when libs change
ifndef SHX
$(BINS): $(LOADLIBES)
endif

$(OBJ)/s0mnars.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mnars -DDEFAULT_MTL_TOOL=MtlLogTool -c $? -o $@

$(BIN)/mnaddrsrv : $(OBJ)/s0mnars.o $(OBJ)/mnars.o
	$(LINK.o) -L$(LIB) $(OBJ)/s0mnars.o $(OBJ)/mnars.o $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/s0mnjoin.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mnjoin -c $? -o $@

$(BIN)/mnjoin : $(OBJ)/s0mnjoin.o $(OBJ)/mnjoin.o $(LIBDEP)
	$(LINK.o) -L$(LIB) $+ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/s0mnln.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mnln -c $? -o $@

$(BIN)/mnln : $(OBJ)/s0mnln.o $(OBJ)/mnln.o $(LIBDEP)
	$(LINK.o) -L$(LIB) $+ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/s0mnls.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mnls -c $? -o $@

$(BIN)/mnls : $(OBJ)/s0mnls.o $(OBJ)/mnls.o $(LIBDEP)
	$(LINK.o) -L$(LIB) $+ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/s0mnping.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mnping -c $? -o $@

$(BIN)/mnping : $(OBJ)/s0mnping.o $(OBJ)/mnping.o $(LIBDEP)
	$(LINK.o) -L$(LIB) $+ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/s0mnps.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mnps -c $? -o $@

$(BIN)/mnps : $(OBJ)/s0mnps.o $(OBJ)/mnps.o $(LIBDEP)
	$(LINK.o) -L$(LIB) $+ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/s0mnrid.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mnridMain -c $? -o $@

$(BIN)/mnrpcid : $(OBJ)/s0mnrid.o $(OBJ)/mnrid.o $(LIBDEP)
	$(LINK.o) -L$(LIB) $+ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/s0mnrm.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mnrm -c $? -o $@

$(BIN)/mnrm : $(OBJ)/s0mnrm.o $(OBJ)/mnrm.o $(LIBDEP)
	$(LINK.o) -L$(LIB) $+ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/s0mnscmd.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mnscmd -c $? -o $@

$(BIN)/mnscmd : $(OBJ)/s0mnscmd.o $(OBJ)/mnscmd.o $(LIBDEP)
	$(LINK.o) -L$(LIB) $+ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/s0mnsc.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mnscMain -DDEFAULT_MTL_TOOL=MtlLogTool -c $? -o $@

$(BIN)/mnprocsrv : $(OBJ)/s0mnsc.o $(OBJ)/mnscs.o $(OBJ)/mnscsop.o $(LIBDEP)
	$(LINK.o) -L$(LIB) $+ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ)/s0mznm.o : $(S0MZMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=mznMain -DDEFAULT_MTL_TOOL=MtlLogTool -c $? -o $@

$(BIN)/mnrpcnmsrv : $(OBJ)/s0mznm.o $(OBJ)/mzns.o $(OBJ)/mznops.o $(LIBDEP)
	$(LINK.o) -L$(LIB) $+ $(LOADLIBES) $(LDLIBS) -o $@

