# Makefile for BootApps / BootBoot / BootRun
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 29-May-98      BJGA   Derived from BootMerge/FontMerge/SysMerge makefile
#

#
# Program specific options:
#
APP        = !${COMPONENT}
RDIR       = Resources.${COMPONENT}
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

CFLAGS  = -c -depend !Depend -ffa -gflv ${INCLUDES} -throwback
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
 ${RDIR}.CoSprite22\
 ${LDIR}.Messages\
 ${LDIR}.Res\

ROBJS = oR.List oR.Main oR.ToolboxE oR.WimpE oR.WimpM
BOBJS = oB.List oB.Main oB.ToolboxE oB.WimpE oB.WimpM
AOBJS = oA.List oA.Main oA.ToolboxE oA.WimpE oA.WimpM
POBJS = pR.List pR.Main pR.ToolboxE pR.WimpE pR.WimpM \
        pB.List pB.Main pB.ToolboxE pB.WimpE pB.WimpM \
        pA.List pA.Main pA.ToolboxE pA.WimpE pA.WimpM \


#
# Rule patterns
#
.SUFFIXES: .c .oA .oB .oR .pA .pB .pR
.c.oA:;  ${CC} ${CFLAGS} -DAPP=1 -o $@ $<
.c.oB:;  ${CC} ${CFLAGS} -DAPP=2 -o $@ $<
.c.oR:;  ${CC} ${CFLAGS} -DAPP=3 -o $@ $<
.c.pA:;  ${CC} ${PFLAGS} -DAPP=1 $< > $@
.c.pB:;  ${CC} ${PFLAGS} -DAPP=2 $< > $@
.c.pR:;  ${CC} ${PFLAGS} -DAPP=3 $< > $@

#
# Main rules:
#
# Application
#
all:        Resources.BootRun.!RunImage \
            Resources.BootBoot.!RunImage \
            Resources.BootApps.!RunImage
        @echo All built (Disc)

preprocess: ${POBJS}
        @echo
        @echo Preprocessing complete

install:    Resources.BootRun.!RunImage \
            Resources.BootBoot.!RunImage \
            Resources.BootApps.!RunImage
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
        |
        ${ATTR} -directories +wr -ww ${INSTDIR}
        ${ATTR} -files +or +ow +wr -ww ${INSTDIR}
        |
        @echo ${COMPONENT}: All installed (Disc)

clean:
        ${WIPE} oA.* ${WFLAGS}
        ${WIPE} oB.* ${WFLAGS}
        ${WIPE} oR.* ${WFLAGS}
        ${WIPE} pA ${WFLAGS}
        ${WIPE} pB ${WFLAGS}
        ${WIPE} pR ${WFLAGS}
        ${RM} Resources.BootApps.!RunImage
        ${RM} Resources.BootBoot.!RunImage
        ${RM} Resources.BootRun.!RunImage
        @echo All cleaned

#
# Static dependencies:
#
Resources.BootApps.!RunImage: ${AOBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${LD} ${LDFLAGS} -o $@ ${AOBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${SQUEEZE} $@

Resources.BootBoot.!RunImage: ${BOBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${LD} ${LDFLAGS} -o $@ ${BOBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${SQUEEZE} $@

Resources.BootRun.!RunImage:  ${ROBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${LD} ${LDFLAGS} -o $@ ${ROBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${SQUEEZE} $@

#---------------------------------------------------------------------------
# Dynamic dependencies:
