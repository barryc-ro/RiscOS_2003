# File:    Makefile
# Purpose: Makefile for Mailbox class library
# Author:  Ben Laughton
#
# History:
#
# 1998-08-28: BAL
#   Created.
# 1998-09-28: RDW
#   install_rom target removed, not required.
#   rom target removed, not required.
#   all target changed to make RAM version for development builds
#   Mailboxz target added to export 
# 1998-10-06: BAL
#   Moved all screen output indicating various parts of build complete from
#   sub-makefile into this one (e.g. to prevent "export (libs) completed"
#   being echoed twice).
#   Separated out export of headers and library object to prevent headers
#   being exported twice.
#   Aliased off the various different targets.





# ------------------------------------------------------------------------------
# Generic options
#

RM		= remove




# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Mailbox
NORMAL_TARGET	= Mailbox
MODULE_TARGET	= Mailboxzm
DEBUG_TARGET	= Mailboxd




# ------------------------------------------------------------------------------
# Build rules
#

all:
	@amu -f MainMake $(MFLAGS) TARGET=$(NORMAL_TARGET) EXT=o
	@echo $(COMPONENT): build complete

debug:
	@amu -f MainMake $(MFLAGS) TARGET=$(DEBUG_TARGET) EXT=od
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
	@amu -f MainMake $(MFLAGS) TARGET=$(MODULE_TARGET) EXT=z
	@echo $(COMPONENT): export complete (libs)
	
clean:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=z
	@$(RM) $(NORMAL_TARGET)
	@$(RM) $(MODULE_TARGET)
	@$(RM) $(DEBUG_TARGET)
	@echo $(COMPONENT): cleaned
