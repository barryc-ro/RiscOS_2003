# File:    Makefile
# Purpose: Makefile for DCILib class library
# Author:  Ben Laughton
#
# History:
#
# 1999-08-02: BAL
# Created




# ------------------------------------------------------------------------------
# Generic options
#

RM		= remove
STRIPDEPEND	= stripdepnd




# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= DCILib
MODULE_TARGET	= DCILib
DEBUG_TARGET	= DCILibd




# ------------------------------------------------------------------------------
# Build rules
#

all:
	@amu -f MainMake $(MFLAGS) TARGET=$(MODULE_TARGET) EXT=o
	@echo $(COMPONENT): build complete

debug:
	@amu -f MainMake $(MFLAGS) TARGET=$(DEBUG_TARGET) EXTRAFLAGS="-DDEBUGLIB" EXT=od
	@echo $(COMPONENT): debug build complete

preprocess:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@echo $(COMPONENT): preprocessing complete

export: export_$(PHASE)

export_hdrs:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=AvoidAMUBug
	@echo $(COMPONENT): export complete (hdrs)

export_libs:
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=o
	@echo $(COMPONENT): export complete (libs)

clean:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@$(RM) $(MODULE_TARGET)
	@$(RM) $(DEBUG_TARGET)
	@$(STRIPDEPEND) MainMake
	@echo $(COMPONENT): cleaned
