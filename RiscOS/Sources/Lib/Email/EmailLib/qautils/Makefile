#	Project Post (769)
#	Acorn Risc Technologies
#	Copyright © 1997. All Rights Reserved.
#
#	FILE:		Makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.05 for RISC OS
#
#	OVERVIEW
#	========
#	Makefile for QAUtils
#
#       IMPORTANT!	This needs Ben Laughton's fixed c++ frontend which allows
#			output to a directory other than o.
#	
#	HISTORY
#	=======
#	19/09/97	0.01	RDW	Initial version
#	27/11/97	0.02	RDW	Added MemCheck support for a MemCheck build
#	17/12/97	0.03	RDW	merged in BALs changes so we export
#					correctly
#	18/02/98	0.04	RDW	throwback now from define
#	24/03/98	0.05	RDW	local_dirs didn't create l dir. does now

#
# Paths
#
EXP_HDR = <export$dir>

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
COMPONENT = QAUtils
TARGET = l.QAUtils
TARGETZ = l.QAUtilsz

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

OBJS=		list.o olist.o indxlist.o

OBJSZ=		z.list z.olist z.indxlist

#
# Rule patterns
#
.SUFFIXES:	.o .z .s .c .c++
.c.o:;		$(MemCheckCC) $(CC) $(CFLAGS) -o $@ $<
.c.z:;		$(MemCheckCC) $(CC) $(CFLAGS) -zm -zps1 -o $@ $<
.c++.o:;	$(MemCheckCC) $(C++) $(C++FLAGS) -o $@ $<
.c++.z:;	$(MemCheckCC) $(C++) $(C++FLAGS) -zm -zps1 -o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<

all: 		${TARGET} ${TARGETZ}
        	@echo ${COMPONENT}: all complete

clean:		
		$(WIPE) o.*		$(WFLAGS)
		$(WIPE) z.*		$(WFLAGS)
		$(WIPE) l.*		$(WFLAGS)
                @echo ${COMPONENT}: cleaned out

local_dirs:
	${MKDIR} o
	${MKDIR} z
	${MKDIR} l

#
# Target 
#
${TARGET}:	$(OBJS) local_dirs
		$(LIBFILE) -c -o $(TARGET) $(OBJS)

${TARGETZ}:	$(OBJSZ) local_dirs
		$(LIBFILE) -c -o $(TARGETZ) $(OBJSZ)

# Dynamic dependencies:
o.list:	c++.list
o.list:	h.list
o.olist:	c++.olist
o.olist:	h.olist
o.olist:	h.indxlist
o.olist:	h.list
o.indxlist:	c++.indxlist
o.indxlist:	h.indxlist
o.indxlist:	h.list
o.list:	c++.list
o.list:	h.list
o.olist:	c++.olist
o.olist:	h.olist
o.olist:	h.indxlist
o.olist:	h.list
o.indxlist:	c++.indxlist
o.indxlist:	h.indxlist
o.indxlist:	h.list
