# Makefile for Task Manager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 18-Oct-94  AMcC         Templates copied to Resources as 'Templates' not 'Template3D'
#

COMPONENT = TaskManager
TARGET    = Switcher
HEADER1  = ${TARGET}

CMDHELP := special

include StdTools
include AAsmModule

resources-special:
        ${MKDIR} ${RESFSDIR}
        Set SwitcherRes$Path LocalUserIFRes:,LocalRes:
        ${CP} SwitcherRes:Messages  ${RESFSDIR}.Messages ${CPFLAGS}
        ${CP} SwitcherRes:Templates ${RESFSDIR}.Templates ${CPFLAGS}
        Unset SwitcherRes$Path
        @echo ${COMPONENT}: resource files copied


# Dynamic dependencies:
