# Makefile for LoadCMOS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name       Description
# ----           ----       -----------
# 19th Jan 1998  BJGA       Created.
#                           Note, an attribute conflict during link is to be expected.
#

#
# Program-specific options:
#
COMPONENT = LoadCMOS
RDIR      = Resources
TARGET    = ${RDIR}.!RunImage

OBJS      = o.main o.util
LIBS      = OSLib:o.OSLib
INCLUDES  = -IOS:

#
# Generic options:
#
CC        = cc
CFLAGS    = -c -depend !Depend -throwback -ffah -apcs 3/noswst
OBJASM    = objasm
OFLAGS    = -depend !Depend -throwback -apcs 3/noswst
LINK      = link
LFLAGS    = -o $@ -rmf
SETTYPE   = settype
ACCESS    = access
ACCFLAGS  = WR/R

MKDIR     = cdir
CP        = copy
CPFLAGS   = ~cfr~v
WIPE      = wipe
WFLAGS    = ~c~v
RM        = remove

#
#General rules
#
.SUFFIXES: .o .c .s

.c.o:
   ${CC} ${CFLAGS} ${INCLUDES} -o $@ $<

.s.o:
   ${OBJASM} ${OFLAGS} -o $@ $<

#
# External targets:
#
all: ${TARGET}
        @echo ${COMPONENT}: make complete

install: all
	${MKDIR} ${INSTDIR}.${COMPONENT}
	${CP} ${RDIR}.!Run                 ${INSTDIR}.LoadCMOS.!Run       ${CPFLAGS}
	${CP} ${TARGET}                    ${INSTDIR}.LoadCMOS.LoadCMOS   ${CPFLAGS}
	${CP} ${RDIR}.${LOCALE}.Messages   ${INSTDIR}.LoadCMOS.Messages   ${CPFLAGS}
	|
        @echo ${COMPONENT}: installation complete

export:
        @echo ${COMPONENT}: export complete

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: clean complete

#
# Internal targets:
#
${TARGET}: ${OBJS} ${LIBS}
        ${LINK} ${LFLAGS} ${OBJS} ${LIBS}
        ${SETTYPE} ${TARGET} Utility
        ${ACCESS} ${TARGET} ${ACCFLAGS}

#
# Dynamic dependencies:
