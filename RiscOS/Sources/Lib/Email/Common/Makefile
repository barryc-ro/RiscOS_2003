# File:    Makefile
# Purpose: Makefile for Common Email library
# Author:  Richard Warren
#
# History:
#
# 1998-10-22: RDW
#   Created from Mailbox Library makefile by Ben Laughton


# ------------------------------------------------------------------------------
# Generic options
#

RM		= remove


# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Common
NORMAL_TARGET	= $(COMPONENT)
MEMCHECK_TARGET	= $(COMPONENT)MC
HIERPROF_TARGET	= $(COMPONENT)HP
MODULE_TARGET	= $(COMPONENT)zm
DEBUG_TARGET	= $(COMPONENT)d




# ------------------------------------------------------------------------------
# Build rules
#

all:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(NORMAL_TARGET) EXT=o
	@echo $(COMPONENT): build complete

debug:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(DEBUG_TARGET) EXT=od
	@echo $(COMPONENT): debug build complete

preprocess:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=i
	@echo $(COMPONENT): preprocessing complete

export: export_$(PHASE)

export_hdrs:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=AvoidAMUBug
	@echo $(COMPONENT): export complete (hdrs)

export_libs:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(NORMAL_TARGET) EXT=o
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(MODULE_TARGET) EXT=z
	@echo $(COMPONENT): export complete (libs)

export_libsMC:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) MemCheckCC=MemCheck.CC TARGET=$(MEMCHECK_TARGET) EXT=oMC
	@echo $(COMPONENT): export complete (MemCheck libs)
	
export_libsHP:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(HIERPROF_TARGET) EXT=oHP
	@echo $(COMPONENT): export complete (HierProf libs)
	
clean:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=oMC
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=oHP
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=z
	@$(RM) $(NORMAL_TARGET)
	@$(RM) $(HIERPROF_TARGET)
	@$(RM) $(MEMCHECK_TARGET)
	@$(RM) $(NORMAL_TARGET)
	@$(RM) $(MODULE_TARGET)
	@$(RM) $(DEBUG_TARGET)
	@echo $(COMPONENT): cleaned
