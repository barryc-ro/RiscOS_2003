#	Project Daytona-SW (757)
#	Acorn Risc Technologies
#	Copyright © 1997. All Rights Reserved.
#
#	FILE:		Makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.05 for RISC OS
#
#	OVERVIEW
#	========
#	Makefile for NFELib test harness
#	
#	HISTORY
#	=======
#	19/09/97	0.01	RDW	Initial version
#	27/11/97	0.02	RDW	Fixed NFEDecode target
#	27/11/97	0.03	RDW	Added MemCheck support for a MemCheck build
#	20/02/98	0.04	RDW	Cleaned up to work in build env. correctly
#	25/02/98	0.05	RDW	Commented out DEBUG and DBUGOBJS so it can
#					be passed from amu commandline

#
# Paths
#
EXP_HDR = <export$dir>
LIBDIR = <Lib$Dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm -tb
CC      = cc
C++     = c++
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir
LIBFILE = libfile
MemCheckCC	= #MemCheck.CC

CPFLAGS = ~cfr~v
WFLAGS = ~c~v

#
# Include files
#
INCLUDES=	C:

#
# Program specific options:
#
ENCCOMP = NFEencode
DECCOMP = NFEdecode
TARGETE = NFEencode
TARGETD = NFEdecode
NFELIB = ${LIBDIR}.NetFax.NFELib.o.NFELib
EMAILLIB = ${LIBDIR}.Email.EmailLib.o.EmailLib

# app or module definitions


# You may want to link with TinyStubs. if you do, set LD to tlink and
# uncomment this TINYSTUBS, and comment out the STUBS

TINYSTUBS=	#-DTINYSTUBS
STUBS=		C:o.Stubs

ROMCSTUBS=      RISC_OSLib:o.romcstubs
ABSSYM=         RISC_OSLib:o.AbsSym

C++LIB=		CPP:o.c++lib 

#DEBUG=		#-DMEMCHECK #-DDEBUGLIB

CFLAGS=		-ffah -I${INCLUDES} ${DEBUG}\
		-Wp -depend !Depend -c -throwback ${TINYSTUBS}

C++FLAGS=	-I${INCLUDES} ${DEBUG}\
		-Wp -depend !Depend -c -throwback ${TINYSTUBS}		


#DBUGOBJS=       #MemCheck:o.MC #tml_podule.o debuglib.o trace_PL.o# 

ASMFLAGS=       

EOBJS=		o.tstNFELibc
DOBJS=		o.tstNFELDec
DEOBJS=		od.tstNFELibc
DDOBJS=		od.tstNFELDec

#
# Rule patterns
#
.SUFFIXES:	.o .s .c .od
.c.od:;		$(MemCheckCC) $(CC) $(CFLAGS) -g -o $@ $<
.c.o:;		$(MemCheckCC) $(CC) $(CFLAGS) -o $@ $<
.c++.o:;	$(MemCheckCC) $(C++) $(C++FLAGS) -o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<

encode:		${EOBJS} local_dirs
		$(LD) -aif -C++ -o NFEencode $(NFELIB) $(EMAILLIB) ${EOBJS} $(OBJS) $(C++LIB) $(DBUGOBJS) $(STUBS)
        	@echo ${TARGETE}: complete
decode:		${DOBJS} local_dirs
		$(LD) -aif -C++ -o NFEdecode $(NFELIB) $(EMAILLIB) ${DOBJS} $(OBJS) $(C++LIB) $(DBUGOBJS) $(STUBS)
        	@echo ${TARGETD}: complete
edebug:		${DEOBJS} local_dirs
		$(LD) -d -aif -C++ -o NFEencode $(NFELIB)d $(EMAILLIB)d ${DEOBJS} $(OBJS) $(C++LIB) $(DBUGOBJS) $(STUBS)
        	@echo ${TARGETE}: complete
ddebug:		${DDOBJS} local_dirs
		$(LD) -d -aif -C++ -o NFEdecode $(NFELIB)d $(EMAILLIB)d ${DDOBJS} $(OBJS) $(C++LIB) $(DBUGOBJS) $(STUBS)
        	@echo ${TARGETD}: complete
all: 		encode decode
        	@echo ${TARGETL}: all complete

local_dirs:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} z


clean:
		$(WIPE) $(EOBJS)	$(WFLAGS)
		$(WIPE) $(DOBJS)	$(WFLAGS)
		$(WIPE) o.*		$(WFLAGS)
		$(WIPE) od.*		$(WFLAGS)
		$(WIPE) z.*		$(WFLAGS)
		$(WIPE) $(TARGETE)	$(WFLAGS)
		$(WIPE) $(TARGETD)	$(WFLAGS)
		@echo $(TARGET): cleaned out

#
# Target 
#

#
# Some rules that need explicitly declaring
#
	
# Dynamic dependencies:
