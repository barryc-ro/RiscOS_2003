# File:    Makefile
# Purpose: Makefile for Omni
# Author:  Ben Laughton
#
# History:
#
# 2000-03-02: BAL
# Created.




# ------------------------------------------------------------------------------
# Paths
#

# Export path for Messages module
RESDIR		= <resource$dir>.Resources2.$(COMPONENT)



# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
CP		= copy
RM		= remove
WIPE		= -wipe
STRIPDEPND	= stripdepnd



# ------------------------------------------------------------------------------
# Flags
#

CPFLAGS		= ~cfr~v
WFLAGS		= ~c~vr




# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Omni
NORMAL_TARGET	= Omni
MEMCHECK_TARGET	= Omnimc
HIERPROF_TARGET	= Omnihp
DEBUG_TARGET	= Omnid

HDR = h.header

A_DEBUG_ON	= "-pd \"DEBUG SETL {TRUE}\""
A_DEBUG_OFF	= "-pd \"DEBUG SETL {FALSE}\""

# C debugging and DebugLib do not map 1<->1 since DebugLib is used for other
# stuff as well.
C_DEBUG_ON	= -DDEBUG -DNDEBUG
C_DEBUG_OFF	= -DNDEBUG




# ------------------------------------------------------------------------------
# Build rules
#

# Local

all:
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXTRA_A_FLAGS=$(A_DEBUG_OFF) EXTRA_C_FLAGS=$(C_DEBUG_OFF) EXT=o
	@echo $(COMPONENT): build complete

debug:	debug_$(TYPE)
	@echo $(COMPONENT): debug build complete

debug_:
	@amu -f MainMake $(MFLAGS) TARGET=$(DEBUG_TARGET) EXTRA_A_FLAGS=$(A_DEBUG_ON) EXTRA_C_FLAGS="$(C_DEBUG_ON) -DDEBUGLIB" EXT=od

debug_NEC:
	@amu -f MainMake $(MFLAGS) TARGET=$(DEBUG_TARGET) EXTRA_A_FLAGS=$(A_DEBUG_ON) EXTRA_C_FLAGS="$(C_DEBUG_ON) -DDEBUGLIB -DDHCP" EXTRAOBJS="od.dhcpboot" EXT=od

memcheck:
	@amu -f MainMake $(MFLAGS) TARGET=$(MEMCHECK_TARGET) EXTRA_A_FLAGS=$(A_DEBUG_OFF) EXTRA_C_FLAGS="$(C_DEBUG_OFF) -DDEBUGLIB -DMemCheck_MEMCHECK" EXT=oMC
	@echo $(COMPONENT): memcheck build complete

preprocess:
	@amu -f MainMake $(MFLAGS)  EXTRA_A_FLAGS=$(A_DEBUG_OFF) EXTRA_C_FLAGS=$(C_DEBUG_OFF) EXT=i
	@echo $(COMPONENT): preprocessing complete


# RISC OS ROM build rules

rom:
	@amu -f MainMake $(MFLAGS) EXTRAFLAGS=-DROM TARGET=$(NORMAL_TARGET) EXT=o
	@echo $(COMPONENT): build complete

export:
	@amu -f MainMake $(MFLAGS)  EXT=AvoidAMUBug

install:
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=o

install_rom:
	@amu -f MainMake $(MFLAGS) TARGET=$(ROM_TARGET) EXT=o

rom_link:
	@amu -f MainMake $(MFLAGS) TARGET=$(ROM_TARGET) EXT=o

resources: LocalRes:Messages
	$(MKDIR) $(RESDIR)
	TokenCheck LocalRes:Messages
	$(CP) LocalRes:Messages $(RESDIR).Messages $(CPFLAGS)
	@echo $(COMPONENT): resource files copied to Messages module

clean:
	@amu -f MainMake $(MFLAGS)  TARGET=Dummy EXT=o
	@amu -f MainMake $(MFLAGS)  TARGET=Dummy EXT=oMC
	@amu -f MainMake $(MFLAGS)  TARGET=Dummy EXT=oHP
	@amu -f MainMake $(MFLAGS)  TARGET=Dummy EXT=od
	@amu -f MainMake $(MFLAGS)  TARGET=Dummy EXT=i
	@$(WIPE) linked $(WFLAGS)
	@$(WIPE) map    $(WFLAGS)
	@$(WIPE) rm     $(WFLAGS)
	@$(WIPE) aof    $(WFLAGS)
	@$(RM) $(HDR)
	@$(STRIPDEPND) MainMake
	@echo $(COMPONENT): cleaned

# Dynamic dependencies:
