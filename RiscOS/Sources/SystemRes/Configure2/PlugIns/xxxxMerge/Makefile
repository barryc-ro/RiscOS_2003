# Makefile for BootMerge / FontMerge / SysMerge
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 29-May-98      BJGA   Derived from FontSetup makefile
# 10-Jun-98      BJGA   Added preprocess-only option
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

SOBJS = oS.Main oS.Merge oS.ToolboxE oS.WimpE oS.WimpM
FOBJS = oF.Main oF.Merge oF.ToolboxE oF.WimpE oF.WimpM
BOBJS = oB.Main oB.Merge oB.ToolboxE oB.WimpE oB.WimpM
POBJS = pS.Main pS.Merge pS.ToolboxE pS.WimpE pS.WimpM \
        pF.Main pF.Merge pF.ToolboxE pF.WimpE pF.WimpM \
        pB.Main pB.Merge pB.ToolboxE pB.WimpE pB.WimpM \


#
# Rule patterns
#
.SUFFIXES: .c .oB .oF .oS .pB .pF .pS
.c.oB:;  ${CC} ${CFLAGS} -DAPP=1 -o $@ $<
.c.oF:;  ${CC} ${CFLAGS} -DAPP=2 -o $@ $<
.c.oS:;  ${CC} ${CFLAGS} -DAPP=3 -o $@ $<
.c.pB:;  ${CC} ${PFLAGS} -DAPP=1 $< > $@
.c.pF:;  ${CC} ${PFLAGS} -DAPP=2 $< > $@
.c.pS:;  ${CC} ${PFLAGS} -DAPP=3 $< > $@

#
# Main rules:
#
# Application
#
all:        Resources.SysMerge.!RunImage \
            Resources.FontMerge.!RunImage \
            Resources.BootMerge.!RunImage
        @echo All built (Disc)

preprocess: ${POBJS}
        @echo
        @echo Preprocessing complete

install:    Resources.SysMerge.!RunImage \
            Resources.FontMerge.!RunImage \
            Resources.BootMerge.!RunImage
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
        ${WIPE} oB.* ${WFLAGS}
        ${WIPE} oF.* ${WFLAGS}
        ${WIPE} oS.* ${WFLAGS}
        ${WIPE} pB ${WFLAGS}
        ${WIPE} pF ${WFLAGS}
        ${WIPE} pS ${WFLAGS}
        ${RM} Resources.BootMerge.!RunImage
        ${RM} Resources.FontMerge.!RunImage
        ${RM} Resources.SysMerge.!RunImage
        @echo All cleaned

#
# Static dependencies:
#
Resources.BootMerge.!RunImage: ${BOBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${LD} ${LDFLAGS} -o $@ ${BOBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${SQUEEZE} $@

Resources.FontMerge.!RunImage: ${FOBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${LD} ${LDFLAGS} -o $@ ${FOBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${SQUEEZE} $@

Resources.SysMerge.!RunImage:  ${SOBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${LD} ${LDFLAGS} -o $@ ${SOBJS} ${CONLIB} ${ELIB} ${TLIB} ${WLIB} ${CLIB}
        ${SQUEEZE} $@

#---------------------------------------------------------------------------
# Dynamic dependencies:
