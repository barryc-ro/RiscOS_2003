# Makefile for Omnitel ISDN module 
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 3-Jul-97   PMS 	  New for ISDN MODULE
# 8-Jul-97   PMS          Runs Makelink to link o files
#
#
# Paul Skirrow
#
# Don't run make directly against this Makefile.  
# Use Build.Europe.Makefile or Build.US.Makefile instead.
#
# This makefile runs all of the makefiles in the different
# sub-directories and then runs Build.Link.Makefile to link
# the resulting o files if they have changed (ie there are
# three levels of nested makefile here).
#
# Note that you must only run one make job at once as it
# relies heavily on the current directory (this also means that
# you must not run any software that changes the current directory
# while this make job is running).
#
# Various command line options may be specified when running amu:
# SUFFIX=_	    append _ to all o directory references and to 
#		    the final ISDN module (ie produce ISDN_)
# DEBUG=-DD_ASSERT  produce a debug version (with the *ISDNlog command)
# CCOPTS=...	    specify additional cc options. This is used
#		    to produce a US version, by defining VARIANT_US.
#
# These options are all passed to subsequent amu commands that
# are invoked from this makefile.
#
# The first two options are normally used together to produce a debug
# version (ISDN_) using the o_ directories.
#
# Although this one makefile can be used to make different versions
# (the standard version, the debug one etc) it is still necessary
# to have two or or more makefiles in each sub-directory as the
# dynamic dependencies are stored in each makefile. It would
# not work properly if we switched from making the o version to
# making the o_ version (since the o_ dynamic dependencies would
# not be set) using the same dynamic dependencies in the makefile.
#
#
# AMU Note:
# There is a bug (undocumented feature?) in AMU that requires lines
# starting like $(TARGET): to have a space before the : as well as after.
#
#
# Generic options:
#
AS       = aasm
CP       = copy
MKDIR    = cdir
MODGEN   = modgen
MODSQZ   = modsqz
RM       = remove
SCANRES  = scanres
WIPE     = -wipe
MAKE     = amu

CFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
TOUCH   = create
WFLAGS  = ~cr~v

#
# Program specific options:
#
COMPONENT   = ISDN$(SUFFIX)
TARGET      = rm.ISDN$(SUFFIX)

#
# Generic rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: module built

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

#  clean:   See below for clean which cleans all subdirectories

#
# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -Ic: -I^.par  
LIBFILES = c:o.stubs  


# ----------------------------------------------------------------- #
# Various Macros
# ----------------------------------------------------------------- #

#  Object directory is o or o_
O = o${SUFFIX}

CC = cc
#NOTMODULE_CFLAGS = $(INCLUDES) -c -Throwback -depend !Depend ${DEBUG} $(VARIANT_CFLAGS)
MODULE_CFLAGS = $(NOTMODULE_CFLAGS) -zM 

LINK = link
NOTMODULE_LINKFLAGS =
MODULE_LINKFLAGS = -LIST Map$(SUFFIX) -MAP -module

CMHG = cmhg
CMHGFLAGS =

#  Run subsequent makefiles with the same parameters as this one
#  and use Makefile_ if necessary to make the o_ files.
MAKEDIR = amu -f Makefile${SUFFIX} $(MFLAGS)
MAKELINK = $(MAKE) -f Makelink $(MFLAGS)

CD = dir
OBJASM = objasm 
MODSQZ = modsqz -fv

# ----------------------------------------------------------------- #
# Rules for making things - do not change.
# ----------------------------------------------------------------- #
	   
.SUFFIXES :	.${O} .c .cmhg .s
.cmhg.${O} :;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.c.${O} :;	$(CC) $(MODULE_CFLAGS) $<
.s.${O} :;	$(OBJASM) -from $< -to $@


#	To make the ISDN module change into each sub-directory in turn and run
#	the makefile there, and then run the Makelink makefile 
#	to link the resulting object files if any have changed.
#	(Sadly we can't do a conditional link here and it is messy
#	to always do the link if the object files haven't changed).
#	Makelink simply performs this link operation if neceessary:
#	$(LINK) $(MODULE_LINKFLAGS) -o $@ $(TARGETS) $(LIBFILES)


$(TARGET) :	MAKE_SERVICES MAKE_INIT MAKE_DL MAKE_V120 MAKE_NS MAKE_ASN1\
		MAKE_CC MAKE_PH_D MAKE_PH_B MAKE_DTE MAKE_APPLI MAKE_MPPP\
		MAKE_PAR MAKE_MODULE MAKE_DEVICE
		$(MAKELINK)

#	If clean target is used we clean every directory:

clean:		MAKE_SERVICES MAKE_INIT MAKE_DL MAKE_V120 MAKE_NS MAKE_ASN1\
		MAKE_CC MAKE_PH_D MAKE_PH_B MAKE_DTE MAKE_APPLI MAKE_MPPP\
		MAKE_PAR MAKE_MODULE MAKE_DEVICE
		$(WIPE) ${O}.* $(WFLAGS)
		$(WIPE) $(TARGET) $(WFLAGS)
                $(WIPE) Map${SUFFIX} $(WFLAGS)
                
MAKE_SERVICES:
		$(CD) sources.services
		$(MAKEDIR)
		$(CD) ^.^
		
MAKE_INIT:		
		$(CD) sources.init
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_DL:	
		$(CD) sources.dl
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_V120:
		$(CD) sources.v120
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_NS:
		$(CD) sources.ns
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_ASN1:
		$(CD) sources.asn1
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_CC:
		$(CD) sources.cc
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_PH_D:
		$(CD) sources.ph_d
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_PH_B:
		$(CD) sources.ph_b
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_DTE:
		$(CD) sources.dte
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_APPLI:
		$(CD) sources.appli
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_MPPP:
		$(CD) sources.mppp
		$(MAKEDIR) 
		$(CD) ^.^

MAKE_PAR:
		$(CD) par
		$(MAKEDIR) 
		$(CD) ^

MAKE_MODULE:
		$(CD) module
		$(MAKEDIR) 
		$(CD) ^

MAKE_DEVICE:
		$(CD) device
		$(MAKEDIR) 
		$(CD) ^
# Dynamic dependencies:
