# ResEd master Makefile
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 05-Jun-98  SNB    Created
#

COMPONENT = ResEd
EXPDIR = Export

TGT_MAIN = aif.!RunImage
TGT_MENU = aif.menu
TGT_WIND = aif.window
TGT_MISC = aif.misc

COMMONLIB = sources.common.o.commonlib


# These are passed to the sub-makefiles so that they can locate their
# Templates files and !WinEdit (for header file regeneration)
TEMP_BASE = ^.^.Resources.!${COMPONENT}
WINEDIT = WINEDIT=^.^.Resources.!WinEdit CFLAGS="-depend !Depend"

# Sub-Makefiles rely on TARGET macro being not empty - clean target needs
# to have some dummy setting to avoid errors.
DUMMYTARGET = TARGET=foo

#
# Generic options:
#
CHMOD	= access
MKDIR	= cdir
CC	= cc
CMHG	= cmhg
CP	= copy
LD	= link
RM	= remove
SQZ	= squeeze
WIPE	= -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

export:
	cdir aif
	dir sources.common
	-amu ${MFLAGS} ${MAKECMDGOALS}
	dir ^.^
        dir sources.shell
        -amu TARGET=^.^.${TGT_MAIN} ${MFLAGS} ${MAKECMDGOALS} ${WINEDIT} TEMPLATES=${TEMP_BASE}.Templates
        dir ^.^
        dir sources.misc
        -amu TARGET=^.^.${TGT_MISC} ${MFLAGS} ${MAKECMDGOALS} ${WINEDIT} TEMPLATES=${TEMP_BASE}.CSE.!Misc.Templates
        dir ^.^
        dir sources.window
        -amu TARGET=^.^.${TGT_WIND} ${MFLAGS} ${MAKECMDGOALS} ${WINEDIT} TEMPLATES=${TEMP_BASE}.CSE.!Window.Templates
        dir ^.^
        dir sources.menu
        -amu TARGET=^.^.${TGT_MENU} ${MFLAGS} ${MAKECMDGOALS} ${WINEDIT} TEMPLATES=${TEMP_BASE}.CSE.!Menu.Templates
        dir ^.^
	${MKDIR} ${EXPDIR}
	${MKDIR} ${EXPDIR}.!${COMPONENT}
	${CP} Resources.!${COMPONENT}.* ${EXPDIR}.!${COMPONENT}.* ${CPFLAGS}
	${SQZ} ${TGT_MENU} ${EXPDIR}.!${COMPONENT}.CSE.!Menu.!RunImage ${SQZFLAGS}
	${SQZ} ${TGT_WIND} ${EXPDIR}.!${COMPONENT}.CSE.!Window.!RunImage ${SQZFLAGS}
	${SQZ} ${TGT_MISC} ${EXPDIR}.!${COMPONENT}.CSE.!Misc.!RunImage ${SQZFLAGS}
	${SQZ} ${TGT_MAIN} ${EXPDIR}.!${COMPONENT}.!RunImage ${SQZFLAGS}
        @echo ${COMPONENT}: exported

clean:
        ${WIPE} Export ${WFLAGS}
        ${WIPE} aif ${WFLAGS}
	dir sources.menu
        -amu clean ${MFLAGS} ${DUMMYTARGET}
        dir ^.^
	dir sources.misc
        -amu clean ${MFLAGS} ${DUMMYTARGET}
        dir ^.^
	dir sources.window
        -amu clean ${MFLAGS} ${DUMMYTARGET}
        dir ^.^
	dir sources.common
        -amu clean ${MFLAGS} ${DUMMYTARGET}
        dir ^.^
	dir sources.shell
        -amu clean ${MFLAGS} ${DUMMYTARGET}
        dir ^.^
        @echo ${COMPONENT: cleaned
