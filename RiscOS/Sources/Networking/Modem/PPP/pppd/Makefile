# Makefile for pppd
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date		Name	Description
# ----		----	-----------
# 1998-01-19	BAL	Created
#

#
# Generic options:
#
MKDIR =		cdir
AS =		objasm
CC =		${MEMCHECKCC} cc
CMHG =		cmhg
CP =		copy
LD =		link
RM =		remove
MODSQZ =	modsqz
WIPE =		-wipe

ASMFLAGS =	-depend !Depend ${THROWBACK}
CFLAGS =	-depend !Depend ${THROWBACK} ${INCLUDES} ${CBLOCKDRIVERS} -strict -Wp -ffah ${CCEXTRA} 
CPFLAGS =	~cfr~v
LDFLAGS =	-aof
WFLAGS =	~c~v

DFLAGS_COMMON = -DCOMPAT_INET4 -DDEBUGMAIN -DDEBUGCHAP -DNO_DRAND48 -DVJC
DFLAGS =	${DFLAGS_COMMON} -DNDEBUG
DDFLAGS =       ${DFLAGS_COMMON} -DDEBUG -DEBUGALL -DDEBUG_TML -DTrace_TRACE

#
# Include files
#
INCLUDES =	-ITCPIPLibs:,C:

#
# Program specific options:
#
TARGET =	o.pppd
DEBUGTARGET =	od.pppd


OBJS =\
 o.auth\
 o.chap\
 o.dea_crypt\
 o.dea_data\
 o.fsm\
 o.ipcp\
 o.lcp\
 o.magic\
 o.main\
 o.md5\
 o.minixlib\
 o.options\
 o.sys-riscos\
 o.upap\


OBJSD =\
 od.auth\
 od.chap\
 od.dea_crypt\
 od.dea_data\
 od.fsm\
 od.ipcp\
 od.lcp\
 od.magic\
 od.main\
 od.md5\
 od.minixlib\
 od.options\
 od.sys-riscos\
 od.upap\


OBJSCP =\
 cp.auth\
 cp.chap\
 cp.dea_crypt\
 cp.fsm\
 cp.ipcp\
 cp.lcp\
 cp.magic\
 cp.main\
 cp.md5\
 cp.minixlib\
 cp.options\
 cp.sys-riscos\
 cp.upap\


#
# Rule patterns
#
.SUFFIXES:	.o .od .cp .c .s
.c.o:;		${CC} ${CFLAGS} ${DFLAGS}       -c -zM -zps1 -o $@ $<
.c.od:;		${CC} ${CFLAGS} ${DDFLAGS}      -c -zM -zps1 -fn -o $@ $<
.c.cp:;		${CC} ${CFLAGS} ${DFLAGS}       -c -C -E $< >> $@
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<
.s.od:;		$(AS) ${ASMFLAGS} -o $@ $<

#
# Main rules:
#
ram: ${TARGET}

rom: ${TARGET}

debug: ${DEBUGTARGET}

analyse: ${OBJSCP}

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} od.* ${WFLAGS}
	${WIPE} cp.* ${WFLAGS}

#
# Target 
#
${TARGET}: ${OBJS}
	${LD} ${LDFLAGS} -o $@ ${OBJS}

#
# Debug target
#
${DEBUGTARGET}: ${OBJSD}
	${LD} ${LDFLAGS} -o $@ ${OBJSD}

# Dynamic dependencies:
