# Makefile for Territory Manager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 21-Nov-00  SBF          Added StripDepnd support.
# 26-Feb-01  SBF          StripDepnd support moved to MkClean
#

COMPONENT  = TerritoryManager
TARGET     = TerrMgr
HEADER1    = Territory
SPRITEFILE = z.SystemDisc

ASFLAGS    = -PD "SpriteDisc SETS \"${SPRITEFILE}\""

include StdTools
include AAsmModule

${SA_OBJECT} ${DBG_OBJECT} ${ROM_OBJECT}: ${SPRITEFILE}

${SPRITEFILE}: SystemDisc
        ${SQUASH} SystemDisc z.SystemDisc

clean ::
        ${RM} ${SPRITEFILE}


# Dynamic dependencies:
