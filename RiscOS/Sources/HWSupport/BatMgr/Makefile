# Makefile for Battery Manager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 07-Mar-01  SNB          Recreated.
#

#
# Program specific options:
#
COMPONENT  = BatMgr
APP        = !${COMPONENT}

# Pass OPTIONS as batmgr_app to include !BatMgr in ResourceFS
RESOURCEEXTRA = batmgr_resources ${OPTIONS}

include Makefiles:StdTools
include Makefiles:AAsmModule

# Note: !Run and !Sprites NOT in Messages module (only required on portables)
batmgr_resources:
        ${MKDIR} ${RESFSDIR}
        ${CP} LocalRes:Templates ${RESFSDIR}.Templates ${CPFLAGS}
        ${CP} Resources.Sprites  ${RESFSDIR}.Sprites   ${CPFLAGS}
        @echo ${COMPONENT}: extra resource files copied to Messages module

batmgr_app:
        ${MKDIR} ${RESAPPDIR}
        ${CP} LocalRes:!Run      ${RESAPPDIR}.!Run ${CPFLAGS}
        ${CP} Resources.!Sprites ${RESAPPDIR}.!Sprites ${CPFLAGS}
        @echo ${COMPONENT}: application frontend copied to Messages module

# Dynamic dependencies:
