# Makefile for DiscSetup
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 23-Jun-98      BJGA   Derived from FontSetup makefile
# 26-Jun-98      BJGA   Added preprocess target
#

#
# Program specific options:
#
COMPONENT  = DiscSetup
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
PFLAGS  = -c -depend !Depend -E -C ${INCLUDES} -throwback
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
 ${LDIR}.Blank\
 ${RDIR}.CoSprite22\
 ${LDIR}.Messages\
 ${LDIR}.Res\

OBJS  = o.Main o.Settings o.ToolboxE o.WimpE o.WimpM
POBJS = p.Main p.Settings p.ToolboxE p.WimpE p.WimpM

#
# Rule patterns
#
.c.o:;  ${CC} ${CFLAGS} -o $@ $<
.c.p:;  ${CC} ${PFLAGS} $< > $@

#
# Main rules:
#
# Application
#
all: ${FILES}
        @echo ${COMPONENT}: All built (Disc)

preprocess: ${POBJS}
        @echo
        @echo Preprocessing complete

install: ${FILES}
        ${MKDIR} ${INSTDIR}
        |
        ${CP} ${RDIR}.!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} ${LDIR}.!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} ${LDIR}.!Run       ${INSTDIR}.!Run       ${CPFLAGS}
        ${CP} ${RDIR}.!RunImage  ${INSTDIR}.!RunImage  ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites22 ${INSTDIR}.!Sprites22 ${CPFLAGS}
        ${CP} ${LDIR}.Blank      ${INSTDIR}.Blank      ${CPFLAGS}
        ${CP} ${RDIR}.CoSprite22 ${INSTDIR}.CoSprite22 ${CPFLAGS}
        ${CP} ${LDIR}.Messages   ${INSTDIR}.Messages   ${CPFLAGS}
        ${CP} ${LDIR}.Res        ${INSTDIR}.Res        ${CPFLAGS}
        |
        ${ATTR} -directories +wr -ww ${INSTDIR}
        ${ATTR} -files +or +ow +wr -ww ${INSTDIR}
        |
        @echo ${COMPONENT}: All installed (Disc)

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} p ${WFLAGS}
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
