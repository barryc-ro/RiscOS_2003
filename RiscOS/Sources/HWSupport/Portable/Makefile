# Makefile for Portable (Morris based)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09-Mar-01  SNB          Recreated
#

COMPONENT = Portable
HEADER1   = Portable
ROM_SOURCE = GetAll.s
RESOURCEEXTRA = resources_lcd

include StdTools
include AAsmModule

resources_lcd:
        ${MKDIR} ${RESFSDIR}
        ${CP} LocalRes:LCD       ${RESFSDIR}.LCD       ${CPFLAGS}
        ${CP} LocalRes:SelectLCD ${RESFSDIR}.SelectLCD ${CPFLAGS}
        @echo ${COMPONENT}: extra resource files copied to Messages module

# Dynamic dependencies:
