# Makefile for BootSetup
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 22-Jun-98      BJGA   Derived from FontSetup makefile
#

#
# Program specific options:
#
COMPONENT  = BootSetup
APP        = !${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
INSTDIR    = ${INSTDIR}.${APP}

#
# Generic options:
#
MKDIR   = cdir
CC      = cc
ATTR    = -attr
CP      = copy
LD      = link
RM      = remove
SQUEEZE = squeeze
WIPE    = -wipe

CFLAGS  =  -c -depend !Depend -ffa -gflv ${INCLUDES} -throwback
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB = C:o.stubs
ELIB = Tbox:o.eventlib
TLIB = Tbox:o.toolboxlib
WLIB = Tbox:o.wimplib
CONLIB = <Lib$Dir>.ConfigLib.o.ConfigLib

#
# Include files
#
INCLUDES = -ITbox:,C:,<Lib$Dir>.ConfigLib.

FILES=\
 ${LDIR}.!Help\
 ${RDIR}.!Boot\
 ${LDIR}.!Run\
 ${RDIR}.!RunImage\
 ${RDIR}.!Sprites22\
 ${RDIR}.CoSprite22\
 ${LDIR}.Messages\
 ${LDIR}.Res\
 ${RDIR}.Sprites22\

OBJS = Main.o ToolboxE.o WimpE.o WimpM.o

#
# Rule patterns
#
.c.o:;  ${CC} ${CFLAGS} -o $@ $<

#
# Main rules:
#
# Application
#
all: ${FILES}
        @echo ${COMPONENT}: All built (Disc)

install: ${FILES}
        ${MKDIR} ${INSTDIR}
        |
        ${CP} ${RDIR}.!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} ${LDIR}.!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} ${LDIR}.!Run       ${INSTDIR}.!Run       ${CPFLAGS}
        ${CP} ${RDIR}.!RunImage  ${INSTDIR}.!RunImage  ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites22 ${INSTDIR}.!Sprites22 ${CPFLAGS}
        ${CP} ${RDIR}.CoSprite22 ${INSTDIR}.CoSprite22 ${CPFLAGS}
        ${CP} ${LDIR}.Messages   ${INSTDIR}.Messages   ${CPFLAGS}
        ${CP} ${LDIR}.Res        ${INSTDIR}.Res        ${CPFLAGS}
        ${CP} ${RDIR}.Sprites22  ${INSTDIR}.Sprites22  ${CPFLAGS}
        |
        ${ATTR} -directories +wr -ww ${INSTDIR}
        ${ATTR} -files +or +ow +wr -ww ${INSTDIR}
        |
        @echo ${COMPONENT}: All installed (Disc)

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${RDIR}.!RunImage
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#
${RDIR}.!RunImage: ${OBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${SQUEEZE} $@

#---------------------------------------------------------------------------
# Dynamic dependencies:
