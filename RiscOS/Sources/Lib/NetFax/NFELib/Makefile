#	Project Daytona (757)
#	Acorn Computers Ltd.
#	Copyright © 1997. All Rights Reserved.
#
#	FILE:		Makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.11 for RISC OS
#
#	OVERVIEW
#	========
#	Makefile for NFELib - NetFax specific Email operations library.  Uses EmailLib
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
#	25/02/98	0.09	RDW	Commented out DEBUG so it can be passed from
#					amu commandline
#					create_ent, extract_hdr and utils objects added
#	25/02/98	0.10	RDW	Fixed so only NFELib.h is exported.
#	20/03/98	0.11	RDW	MEMCHECK var. removed so it can be set from
#					amu commandline

#
# Paths
#
EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
EXPORT_PDIR     = <Lib$Dir>.NetFax
EXPORT_DIR	= ${EXPORT_PDIR}.NFELib

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

CPFLAGS = ~cfr~v
WFLAGS = ~c~v

#
# Include files
#
INCLUDES=	C:

#
# Program specific options:
#
COMPONENT	= NFELib
TARGET		= NFELib
TARGETD		= NFELibd
TARGETZ		= NFELibzm

# app or module definitions


# You may want to link with TinyStubs. if you do, set LD to tlink and
# uncomment this TINYSTUBS, and comment out the STUBS

TINYSTUBS=	#-DTINYSTUBS
STUBS=		C:o.Stubs

ROMCSTUBS=      RISC_OSLib:o.romcstubs
ABSSYM=         RISC_OSLib:o.AbsSym

C++LIB=		C:o.c++lib 

#DEBUG=		#-DMEMCHECK #-DDEBUGLIB

CFLAGS=		-ffah -I${INCLUDES} ${DEBUG}\
		-Wp -depend !Depend -c ${THROWBACK} ${TINYSTUBS}

C++FLAGS=	-I${INCLUDES} ${DEBUG}\
		-Wp -depend !Depend -c ${THROWBACK} ${TINYSTUBS}		

ASMFLAGS=       

HDRS=		h.NFELib

OBJS=		o.NFELib o.create_ent o.extracthdr o.utils

OBJSZ=		z.NFELib z.create_ent z.extracthdr z.utils

OBJSD =		od.NFELib od.create_ent od.extracthdr od.utils

#
# Rule patterns
#
.SUFFIXES:	.o .od .z .s .c .c++
.c.o:;		$(MemCheckCC) $(CC) $(CFLAGS) -o $@ $<
.c.od:;		$(MemCheckCC) $(CC) $(CFLAGS) -g -o $@ $<
.c.z:;		$(MemCheckCC) $(CC) $(CFLAGS) -zm -zps1 -o $@ $<
.c++.o:;	$(MemCheckCC) $(C++) $(C++FLAGS) -o $@ $<
.c++.od:;	$(MemCheckCC) $(C++) $(C++FLAGS) +g -o $@ $<
.c++.z:;	$(MemCheckCC) $(C++) $(C++FLAGS) -zm -zps1 -o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<

all: 		library
        	@echo ${COMPONENT}: all complete
        	
library:	${TARGET} ${TARGETZ}
		@echo ${COMPONENT}: library complete

debug: 		${TARGETD}
		${CP}	${TARGETD}	${EXPORT_DIR}.o.${TARGETD}	${CPFLAGS}
		@echo ${COMPONENT}: debug build complete
		
clean:
		$(WIPE) ${TARGET}	$(WFLAGS)
		$(WIPE) ${TARGETZ}	$(WFLAGS)
		$(WIPE) ${TARGETD}	$(WFLAGS)
		$(WIPE) o.*		$(WFLAGS)
		$(WIPE) od.*		$(WFLAGS)
		$(WIPE) z.*		$(WFLAGS)
		@echo $(COMPONENT): cleaned out

export: export_${PHASE}

export_hdrs: ${HDRS} dirs
	${CP}	h.NFELib	${EXPORT_DIR}.h.NFELib		${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} dirs
	${CP}	${TARGET}	${EXPORT_DIR}.o.${TARGET}	${CPFLAGS}
	${CP}	${TARGETZ}	${EXPORT_DIR}.o.${TARGETZ}	${CPFLAGS}
	@echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} z

dirs:
	${MKDIR} ${LIBDIR}
	${MKDIR} ${EXPORT_PDIR}
	${MKDIR} ${EXPORT_DIR}
	${MKDIR} ${EXPORT_DIR}.h
	${MKDIR} ${EXPORT_DIR}.o


#
# Target 
#
${TARGET}:	$(OBJS) local_dirs
		$(LIBFILE) -c -o $(TARGET) $(OBJS) $(QAOBJS)

${TARGETZ}:	$(OBJSZ) local_dirs
		$(LIBFILE) -c -o $(TARGETZ) $(OBJSZ) $(QAOBJSZ)

${TARGETD}:	${OBJSD} local_dirs
		$(LIBFILE) -c -o $(TARGETD) $(OBJSD) $(QAOBJS)

# Dynamic dependencies:
