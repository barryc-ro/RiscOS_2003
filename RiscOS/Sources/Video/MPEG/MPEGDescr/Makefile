# Makefile for MPEGDescramble
#
# ************************************
# ***     C h a n g e   L i s t    ***
# ************************************
# Date         Name        Description
# ----         ----        -----------
# 23-Jan-2003  SAR         Created.
#

COMPONENT  = MPEGDescramble
TARGET    ?= MPEGDescr
ROM_SOURCE = s.Module
#
# DEBUG     = set to TRUE if you want a build with debugging code included
# BUF_USAGE = set to TRUE to include attemped fix for client-pull systems. May be faulty!
#
DEBUG     ?= FALSE
BUF_USAGE ?= FALSE
ASFLAGS    = -PD "DEBUG SETL {${DEBUG}}" -PD "BUF_USAGE SETL {${BUF_USAGE}}"

include StdTools
include AAsmModule

clean::
	${WIPE} rm ${WFLAGS}

# Dynamic dependencies:
