# File:    Makefile
# Purpose: Makefile for ATM25
# Author:  Ben Laughton
#
# History:
#
# 1999-03-12: BAL
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

COMPONENT	= ATM25
NORMAL_TARGET	= ATM25
MEMCHECK_TARGET	= ATM25mc
HIERPROF_TARGET	= ATM25hp
DEBUG_TARGET	= ATM25d

HDR = h.header




# ------------------------------------------------------------------------------
# Build rules
#

# Local

all:
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=o
	@echo $(COMPONENT): build complete

debug:	debug_$(TYPE)
	@echo $(COMPONENT): debug build complete

debug_:
	@amu -f MainMake $(MFLAGS) TARGET=$(DEBUG_TARGET) EXTRAFLAGS="-DDEBUGLIB" EXT=od

debug_NEC:
	@amu -f MainMake $(MFLAGS) TARGET=$(DEBUG_TARGET) EXTRAFLAGS="-DDEBUGLIB -DDHCP" EXTRAOBJS="od.dhcpboot" EXT=od

memcheck:
	@amu -f MainMake $(MFLAGS) TARGET=$(MEMCHECK_TARGET) EXTRAFLAGS="-DDEBUGLIB -DMemCheck_MEMCHECK" EXT=oMC
	@echo $(COMPONENT): memcheck build complete

preprocess:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@echo $(COMPONENT): preprocessing complete


# RISC OS ROM build rules

rom:
	@amu -f MainMake $(MFLAGS) EXTRAFLAGS=-DROM TARGET=$(NORMAL_TARGET) EXT=o
	@echo $(COMPONENT): build complete

export:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=AvoidAMUBug

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
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=oMC
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=oHP
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@$(WIPE) linked $(WFLAGS)
	@$(WIPE) map    $(WFLAGS)
	@$(WIPE) rm     $(WFLAGS)
	@$(WIPE) aof    $(WFLAGS)
	@$(RM) $(HDR)
	@$(STRIPDEPND) MainMake
	@echo $(COMPONENT): cleaned

# Dynamic dependencies:
