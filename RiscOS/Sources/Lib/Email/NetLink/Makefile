# File:    Makefile
# Purpose: Makefile for NetLink class library
# Author:  Ben Laughton
#
# History:
#
# 1998-10-06: BAL
# Created, based on Mailbox makefile.




# ------------------------------------------------------------------------------
# Generic options
#

RM		= remove




# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= NetLink
NORMAL_TARGET	= NetLink
DEBUG_TARGET	= NetLinkd
MODULE_TARGET	= NetLinkzm




# ------------------------------------------------------------------------------
# Build rules
#

all:
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=o

rom:
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=$(MODULE_TARGET) EXT=z

debug:
	@amu -f MainMake $(MFLAGS) TARGET=$(DEBUG_TARGET) EXT=od

preprocess:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@echo $(COMPONENT): preprocessing complete

export: export_$(PHASE)

export_hdrs:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=AvoidAMUBug
	@echo $(COMPONENT): export complete (hdrs)		$(CPFLAGS)

export_libs:
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=$(MODULE_TARGET) EXT=z
	@echo $(COMPONENT): export complete (libs)

install_rom:
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET)
	@echo $(COMPONENT): rom module installed

clean:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=z
	@$(RM) $(NORMAL_TARGET)
	@$(RM) $(DEBUG_TARGET)
	@$(RM) $(MODULE_TARGET)
	@echo $(COMPONENT): cleaned
