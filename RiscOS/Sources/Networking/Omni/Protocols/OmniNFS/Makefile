# File:    Makefile
# Purpose: Makefile for OmniNFS
# Author:  Ben Laughton
#
# History:
#
# 1999-11-29: BAL
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

COMPONENT	= OmniNFS
NORMAL_TARGET	= OmniNFS
MEMCHECK_TARGET	= OmniNFSmc
HIERPROF_TARGET	= OmniNFShp
DEBUG_TARGET	= OmniNFSd

NORMAL_HDR =	h.OmniNFSHdr
DEBUG_HDR =	h.OmniNFSDbg

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
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXTRA_A_FLAGS=$(A_DEBUG_OFF) EXTRA_C_FLAGS=$(C_DEBUG_OFF) HDR=$(NORMAL_HDR) EXT=o
	@echo $(COMPONENT): build complete

debug:
	@amu -f MainMake $(MFLAGS) TARGET=$(DEBUG_TARGET) EXTRA_A_FLAGS=$(A_DEBUG_ON) EXTRA_C_FLAGS="$(C_DEBUG_ON) -DDEBUGLIB" HDR=$(DEBUG_HDR) EXT=od
	@echo $(COMPONENT): debug build complete

memcheck:
	@amu -f MainMake $(MFLAGS) TARGET=$(MEMCHECK_TARGET) EXTRA_A_FLAGS=$(A_DEBUG_OFF) EXTRA_C_FLAGS="$(C_DEBUG_OFF) -DDEBUGLIB" HDR=$(NORMAL_HDR) EXT=oMC
	@echo $(COMPONENT): memcheck build complete

preprocess:
	@amu -f MainMake $(MFLAGS)  EXTRA_A_FLAGS=$(A_DEBUG_OFF) EXTRA_C_FLAGS=$(C_DEBUG_OFF) HDR=$(NORMAL_HDR) EXT=i
	@echo $(COMPONENT): preprocessing complete


# RISC OS ROM build rules

rom:
	@amu -f MainMake $(MFLAGS) EXTRAFLAGS=-DROM TARGET=$(NORMAL_TARGET) HDR=$(NORMAL_HDR) EXT=o
	@echo $(COMPONENT): build complete

export:
	@amu -f MainMake $(MFLAGS)  EXT=AvoidAMUBug

install:
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) HDR=$(NORMAL_HDR) EXT=o

install_rom:
	@amu -f MainMake $(MFLAGS) TARGET=$(ROM_TARGET) HDR=$(NORMAL_HDR) EXT=o

rom_link:
	@amu -f MainMake $(MFLAGS) TARGET=$(ROM_TARGET) HDR=$(NORMAL_HDR) EXT=o

resources: LocalRes:Messages
	@echo $(COMPONENT): resource files copied to Messages module

clean:
	@amu -f MainMake $(MFLAGS)  TARGET=Dummy HDR=$(NORMAL_HDR) EXT=o
	@amu -f MainMake $(MFLAGS)  TARGET=Dummy HDR=$(NORMAL_HDR) EXT=oMC
	@amu -f MainMake $(MFLAGS)  TARGET=Dummy HDR=$(NORMAL_HDR) EXT=oHP
	@amu -f MainMake $(MFLAGS)  TARGET=Dummy HDR=$(NORMAL_HDR) EXT=od
	@amu -f MainMake $(MFLAGS)  TARGET=Dummy HDR=$(NORMAL_HDR) EXT=i
	@$(WIPE) linked $(WFLAGS)
	@$(WIPE) map    $(WFLAGS)
	@$(WIPE) rm     $(WFLAGS)
	@$(WIPE) aof    $(WFLAGS)
	@$(RM) $(NORMAL_HDR)
	@$(RM) $(DEBUG_HDR)
	@$(STRIPDEPND) MainMake
	@echo $(COMPONENT): cleaned

# Dynamic dependencies:
