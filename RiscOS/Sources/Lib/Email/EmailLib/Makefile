#	Project Post (769)
#	Acorn Risc Technologies
#	Copyright © 1997. All Rights Reserved.
#
#	FILE:		Makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.15 for RISC OS
#
#	OVERVIEW
#	========
#	Makefile for EmailLib - Internet Email message parsing and creation library
#
#       IMPORTANT!	This needs Ben Laughton's fixed c++ frontend which allows
#			output to a directory other than o.
#	
#	HISTORY
#	=======
#	19/09/97	0.01	RDW	Initial version
#	27/10/97	0.02	RDW	subject.o added to OBJS
#	24/11/97	0.03	RDW	MIMEtext.o added to OBJS
#	27/11/97	0.04	RDW	Added MemCheck support for a MemCheck build
#	04/12/97	0.05	RDW	Added ELib_OS.o to OBJS
#	17/12/97	0.06	RDW	merged in BALs changes so we export correctly
#	09/01/98	0.07	RDW	merged in BALs debug build target
#	18/02/98	0.08	RDW	Split NetFax NFE code from generic emaillibrary
#	20/02/98	0.09	RDW	added XNFrtnrcpt to OBJS, HDRS, OBJSZ, OBJSD
#	25/02/98	0.10	RDW	Commented out DEBUG so it can be passed from
#					amu commandline
#	27/05/98	0.11	RDW	-ffa added to C++FLAGS
#	28/08/98	0.12	RDW	clean removes o, od and z dirs.
#	28/09/98	0.13	RDW	mailbox object added to o, od and z
#					cc object added to o, od and z
#	05/10/98	0.14	RDW	C++LIB added to INCLUDES
#					quotedprintable added to OBJS, HDRS, OBSSZ, OBJSD
#					-zps1 removed from module build so library will work in
#					module wrapped application
#	24/10/98	0.15		better MemCheck build support added.  MemCheck output is
#					EmailLibMC
#					Makefile system changed over to Ben's cunning
#					2 level makefile



# ------------------------------------------------------------------------------
# Generic options
#

RM		= remove


# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= EmailLib
NORMAL_TARGET	= $(COMPONENT)
MEMCHECK_TARGET	= $(COMPONENT)MC
HIERPROF_TARGET	= $(COMPONENT)HP
MODULE_TARGET	= $(COMPONENT)zm
DEBUG_TARGET	= $(COMPONENT)d



# ------------------------------------------------------------------------------
# Build rules
#

all:
	@amu -f MainMake $(MFLAGS) ${MAKECMDGOALS} TARGET=$(NORMAL_TARGET) EXT=o
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
	@echo $(COMPONENT): cleaned
