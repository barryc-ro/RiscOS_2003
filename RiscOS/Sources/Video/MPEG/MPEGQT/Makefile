# Makefile for Tim's Lightspan code
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 30-Aug-94  AMcC         Added rom_link rule.

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG	= cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB     = CLIB:o.stubs

#
# Include files
#
INCLUDES = -IC:

#
# Program specific options:
#
COMPONENT = MPEGQT
TARGET = aof.MPEGQT
OBJS = mpegqtv.o mpegqt.o debug.o
EXPORTS = ${EXP_HDR}.${COMPONENT}

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@


#
# Test/development rules:
#
all:	rm.MPEGQT
        @echo ${COMPONENT}: module built

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

rm.MPEGQT: ${OBJS} ${CLIB}
	${LD} -o $@ -module ${OBJS} ${CLIB}

# Dynamic dependencies:
o.debug:	c.DEBUG
o.debug:	h.misc
o.debug:	h.debug
o.debug:	h.swicall
o.debug:	C:h.kernel
o.debug:	C:h.swis
o.mpegqt:	c.mpegqt
o.mpegqt:	t.mpegqt
o.mpegqt:	h.mpegqt
o.mpegqt:	h.swicall
o.mpegqt:	C:h.kernel
o.mpegqt:	C:h.swis
o.mpegqt:	h.debug
