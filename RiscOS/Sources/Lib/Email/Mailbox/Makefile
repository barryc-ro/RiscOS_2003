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
# 1998-10-24: RDW
#   MEMCHECK_TARGET added





# ------------------------------------------------------------------------------
# Generic options
#

RM		= remove
STRIPDEPND	= stripdepnd



# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Mailbox
NORMAL_TARGET	= Mailbox
MEMCHECK_TARGET	= MailboxMC
HIERPROF_TARGET	= MailboxHP
MODULE_TARGET	= Mailboxzm
DEBUG_TARGET	= Mailboxd




# ------------------------------------------------------------------------------
# Build rules
#

all:	export_libs export_libsMC export_libsHP preprocess
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug
	@echo $(COMPONENT): build complete

debug:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(DEBUG_TARGET) EXT=od
	@echo $(COMPONENT): debug build complete

preprocess:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=i
	@echo $(COMPONENT): preprocessing complete

export: export_$(PHASE)

export_hdrs:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=AvoidAMUBug
	@echo $(COMPONENT): export complete (hdrs)

export_libs:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(NORMAL_TARGET) EXT=o
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(MODULE_TARGET) EXT=z
	@echo $(COMPONENT): export complete (libs)
	
export_libsMC:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} MemCheckCC=MemCheck.CC TARGET=$(MEMCHECK_TARGET) EXT=oMC
	@echo $(COMPONENT): export complete (MemCheck libs)
	
export_libsHP:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(HIERPROF_TARGET) EXT=oHP
	@echo $(COMPONENT): export complete (HierProf libs)
	
clean:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=oMC
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=oHP
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=AvoidAMUBug EXT=z
	@$(RM) $(NORMAL_TARGET)
	@$(RM) $(MEMCHECK_TARGET)
	@$(RM) $(HIERPROF_TARGET)
	@$(RM) $(MODULE_TARGET)
	@$(RM) $(DEBUG_TARGET)
	@$(STRIPDEPND) MainMake
	@echo $(COMPONENT): cleaned
