# Makefile for the Tar Extension Utility
# ======================================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date          Name    Description
# ====          ====    ===========
# 1999-01-27    DSC     Created

# ------------------------------------------------------------------------------
# Paths
#

EXP_C_H	= <CExport$Dir>.h
EXP_C_O	= <CExport$Dir>.o
LIBDIR	= <Lib$Dir>


# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT       = TarExtend
NORMAL_TARGET	= $(COMPONENT)
MEMCHECK_TARGET	= $(COMPONENT)MC
HIERPROF_TARGET	= $(COMPONENT)HP
DEBUG_TARGET	= $(COMPONENT)d


# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

# RESDIR          = $(RESDIR).$(RES_COMPONENT)
# RESAPP          = <Resource$Dir>.Apps.$(APP)


# -----------------------------------------------------------------------------
# Generic options
#

MKDIR           = cdir
AS              = objasm
CC              = $(MemCheckCC) cc
C++             = $(MemCheckCC) c++
CMHG            = cmhg
CP              = copy
LD              = link
LB		= libfile
RM              = remove
SQUEEZE         = squeeze
WIPE            = -wipe


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DDFLAGS         = $(DFLAGS) -fn
DDZFLAGS        = $(DZFLAGS) -fn


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS =  o.main \
        o.dbug \
        o.Recurse \
        o.RMalloc \
        o.CPathConv

# -----------------------------------------------------------------------------
# Libraries
#

CLIB		= CLib:o.stubs
ROMCSTUBS	= RISC_OSLib:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_abssym
WRAPPER		= RISC_OSLib:s.ModuleWrap

C++LIB          = $(LIBDIR).C++Lib.o.c++lib

EVENTLIB	= $(LIBDIR).tboxlibs.o.eventlib
TOOLBOXLIB	= $(LIBDIR).tboxlibs.o.toolboxlib
WIMPLIB		= $(LIBDIR).tboxlibs.o.wimplib

SOCKLIB		= $(LIBDIR).TCPIPLibs.o.socklib5
DEBUGLIB	= $(LIBDIR).DebugLib.o.debuglib
INETLIB		= $(LIBDIR).TCPIPLibs.o.inetlib
UNIXLIB		= $(LIBDIR).TCPIPLibs.o.unixlib

LIBS = \
 $(SOCKLIB)\
 $(CLIB)\
 $(C++LIB)\
 $(EVENTLIB)\
 $(TOOLBOXLIB)\
 $(WIMPLIB)\
 $(EXTRALIBS)
# $(ABSSYM)


# -----------------------------------------------------------------------------
# Build rules
#

all: 
	amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=o
        $(LD) $(LDFLAGS) -c++ -o targets.$(COMPONENT) $(OBJS) $(LIBS) -map > map.$(COMPONENT)
	@echo $(COMPONENT): build complete

debug: 
	@amu -f MainMake $(MFLAGS) TARGET=$(DEBUG_TARGET) EXT=od
	@echo $(COMPONENT): debug build complete

preprocess:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@echo $(COMPONENT): preprocessing complete

export: export_$(PHASE)
	
export_libsMC:
	@amu -f MainMake $(MFLAGS) MemCheckCC=MemCheck.CC TARGET=$(MEMCHECK_TARGET) EXT=oMC
	@echo $(COMPONENT): export complete (MemCheck libs)
	
export_libsHP:
	@amu -f MainMake $(MFLAGS) TARGET=$(HIERPROF_TARGET) EXT=oHP
	@echo $(COMPONENT): export complete (HierProf libs)

ram: 
	amu -f MainMake $(MFLAGS) TARGET=$(MODULE_TARGET) EXT=oz
	@echo $(COMPONENT): Module built (RAM)

installMC:
	amu -f MainMake $(MFLAGS) MemCheckCC=MemCheck.CC TARGET=$(MEMCHECK_TARGET) EXT=oMC
        $(LD) $(LDFLAGS) -c++ -o targets.$(COMPONENT) $(OBJS) $(LIBSMC) -map > map.$(COMPONENT)
        echo
        echo $(COMPONENT): $(TARGET) (MemCheckD) installed

clean: clean_common
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=oMC
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=oHP
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=oz
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=Targets
	@echo $(COMPONENT): cleaned

clean_common:
	@$(RM) $(NORMAL_TARGET)
	@$(RM) $(MEMCHECK_TARGET)
	@$(RM) $(HIERPROF_TARGET)
	@$(RM) $(DEBUG_TARGET)


# -----------------------------------------------------------------------------
# Development rules
#

clean_all: clean_common
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=oMC
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=oHP
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=od
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=oz
        @echo $(COMPONENT): Cleaned all
