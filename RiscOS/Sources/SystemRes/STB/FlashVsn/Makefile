# Makefile for FlashVsn
#
# ************************************
# ***     C h a n g e   L i s t    ***
# ************************************
# Date         Name        Description
# ----         ----        -----------
# 24-Jan-2002  SAR         Created.
#

COMPONENT = FlashVsn
TARGET    = ${COMPONENT}

ASFLAGS  = -pd "FlashVar SETS \"${VAR}\"" -pd "FlashVer SETS \"${VER}\""

include StdTools
include AAsmModule

clean::
	${WIPE} rm

# Dynamic dependencies:
