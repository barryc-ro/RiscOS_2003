#	Project Post (769)
#	Acorn Risc Technologies
#	Copyright © 1997. All Rights Reserved.
#
#	FILE:		Makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.08 for RISC OS
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

#
# Paths
#
EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
EXPORT_PDIR     = <Lib$Dir>.Email
EXPORT_DIR	= ${EXPORT_PDIR}.EmailLib

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
COMPONENT	= EmailLib
TARGET		= EmailLib
TARGETD		= EmailLibd
TARGETZ		= EmailLibzm

# app or module definitions


# You may want to link with TinyStubs. if you do, set LD to tlink and
# uncomment this TINYSTUBS, and comment out the STUBS

TINYSTUBS=	#-DTINYSTUBS
STUBS=		C:o.Stubs

ROMCSTUBS=      RISC_OSLib:o.romcstubs
ABSSYM=         RISC_OSLib:o.AbsSym

C++LIB=		C:o.c++lib 

DEBUG=		#-DMEMCHECK #-DDEBUGLIB

CFLAGS=		-ffah -I${INCLUDES} ${DEBUG}\
		-Wp -depend !Depend -c ${THROWBACK} ${TINYSTUBS}

C++FLAGS=	-I${INCLUDES} ${DEBUG}\
		-Wp -depend !Depend -c ${THROWBACK} ${TINYSTUBS}		

ASMFLAGS=       

HDRS=		h.822parser body.h bodymulti.h contentT.h contentTE.h date.h elibbase.h entity.h \
		from.h header.h headers.h messageid.h MIMEentity.h MIMEimage.h MIMEmultip.h mimevers.h \
		parameter.h replyto.h resentdate.h rfc822msg.h to.h XNFdevice.o XNFdevicec.o \
		XNFfaxnum.h XNFfaxpage.h XNFmsgclas.h XNFphoneno.h XNFserial.h XNFsoftrev.h Xorganiz.h \
		subject.h base64.h ELib.h ELib_priv.h MIMEtext.h ELib_OS.h \
		@.qautils.h.list @.qautils.h.olist @.qautils.h.indxlist

OBJS=		o.822parser body.o bodymulti.o contentT.o contentTE.o date.o elibbase.o entity.o \
		from.o header.o headers.o messageid.o MIMEentity.o MIMEimage.o MIMEmultip.o mimevers.o \
		parameter.o replyto.o resentdate.o rfc822msg.o to.o XNFdevice.o XNFdevicec.o \
		XNFfaxnum.o XNFfaxpage.o XNFmsgclas.o XNFphoneno.o XNFserial.o XNFsoftrev.o Xorganiz.o \
		subject.o base64.o ELib.o ELib_priv.o MIMEtext.o ELib_OS.o

OBJSZ=		z.822parser z.body z.bodymulti z.contentT z.contentTE z.date z.elibbase z.entity \
		z.from z.header z.headers z.messageid z.MIMEentity z.MIMEimage z.MIMEmultip z.mimevers \
		z.parameter z.replyto z.resentdate z.rfc822msg z.to z.XNFdevice z.XNFdevicec \
		z.XNFfaxnum z.XNFfaxpage z.XNFmsgclas z.XNFphoneno z.XNFserial z.XNFsoftrev z.Xorganiz \
		z.subject z.base64 z.ELib z.ELib_priv z.MIMEtext z.ELib_OS

OBJSD =		od.822parser od.body od.bodymulti od.contentT od.contentTE od.date od.elibbase \
		od.entity od.from od.header od.headers od.messageid od.MIMEentity od.MIMEimage \
		od.MIMEmultip od.mimevers od.parameter od.replyto od.resentdate od.rfc822msg \
		od.to od.XNFdevice od.XNFdevicec od.XNFfaxnum od.XNFfaxpage od.XNFmsgclas \
		od.XNFphoneno od.XNFserial od.XNFsoftrev od.Xorganiz od.subject od.base64 od.ELib \
		od.ELib_priv od.MIMEtext od.ELib_OS

QAOBJS=		@.qautils.o.list @.qautils.o.olist @.qautils.o.indxlist

QAOBJSZ=	@.qautils.z.list @.qautils.z.olist @.qautils.z.indxlist


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
		
clean:		cleanqautils
		$(WIPE) ${TARGET}	$(WFLAGS)
		$(WIPE) ${TARGETZ}	$(WFLAGS)
		$(WIPE) ${TARGETD}	$(WFLAGS)
		$(WIPE) o.*		$(WFLAGS)
		$(WIPE) od.*		$(WFLAGS)
		$(WIPE) z.*		$(WFLAGS)
		@echo $(COMPONENT): cleaned out

export: export_${PHASE}

export_hdrs: ${HDRS} dirs
	${CP}	h.*		${EXPORT_DIR}.h.*		${CPFLAGS}
	${CP}	qautils.h.*	${EXPORT_DIR}.qautils.h.*	${CPFLAGS}
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
	${MKDIR} ${EXPORT_DIR}.qautils
	${MKDIR} ${EXPORT_DIR}.qautils.h


#
# Target 
#
${TARGET}:	$(OBJS) qautils local_dirs
		$(LIBFILE) -c -o $(TARGET) $(OBJS) $(QAOBJS)

${TARGETZ}:	$(OBJSZ) qautils local_dirs
		$(LIBFILE) -c -o $(TARGETZ) $(OBJSZ) $(QAOBJSZ)

${TARGETD}:	${OBJSD} qautils local_dirs
		$(LIBFILE) -c -o $(TARGETD) $(OBJSD) $(QAOBJS)

#
# Some rules that need explicitly declaring
#
qautils:
        Dir qautils
        Amu
        Up
        
cleanqautils:
        Dir qautils
	Amu -f Makefile clean
	Up
	
# Dynamic dependencies:
