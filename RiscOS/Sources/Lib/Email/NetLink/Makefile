# File:    Makefile
# Purpose: Makefile for NetLink class library
# Author:  Ben Laughton
#
# History:
#
# 1998-10-06: BAL
# Created, based on Mailbox makefile.
# 1998-11-03: RDW
# separate MemCheck and HierProf targets added



# ------------------------------------------------------------------------------
# Generic options
#

RM		= remove




# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= NetLink
NORMAL_TARGET	= $(COMPONENT)
MEMCHECK_TARGET	= $(COMPONENT)MC
HIERPROF_TARGET	= $(COMPONENT)HP
DEBUG_TARGET	= $(COMPONENT)d
MODULE_TARGET	= $(COMPONENT)zm




# ------------------------------------------------------------------------------
# Build rules
#

all:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(NORMAL_TARGET) EXT=o

rom:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(NORMAL_TARGET) EXT=o
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(MODULE_TARGET) EXT=z

debug:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(DEBUG_TARGET) EXT=od

preprocess:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=i
	@echo $(COMPONENT): preprocessing complete

export: export_$(PHASE)

export_hdrs:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=AvoidAMUBug
	@echo $(COMPONENT): export complete (hdrs)		$(CPFLAGS)

export_libs:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(NORMAL_TARGET) EXT=o
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(MODULE_TARGET) EXT=z
	@echo $(COMPONENT): export complete (libs)

export_libsMC:
	@amu -f MainMake $(MFLAGS) MemCheckCC=MemCheck.CC ${MAKECMDGOALS} TARGET=$(MEMCHECK_TARGET) EXT=oMC
	@echo $(COMPONENT): export complete (MemCheck libs)
	
export_libsHP:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(HIERPROF_TARGET) EXT=oHP
	@echo $(COMPONENT): export complete (HierProf libs)
	
install_rom:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(NORMAL_TARGET)
	@echo $(COMPONENT): rom module installed

clean:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=oMC
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=oHP
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=z
	@$(RM) $(MEMCHECK_TARGET)
	@$(RM) $(HIERPROF_TARGET)
	@$(RM) $(NORMAL_TARGET)
	@$(RM) $(DEBUG_TARGET)
	@$(RM) $(MODULE_TARGET)
	@echo $(COMPONENT): cleaned
