# Makefile for NHTwitter
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 31-Oct-95  TMD	  Created.
# 24-Feb-00  KJB          Adapted for multiple build variants.
#

COMPONENT  = NHTwitter
TYPE      ?= Standard
ROM_MODULE := rm.${MACHINE}.${TYPE}
ROM_OBJECT := o.${MACHINE}.${TYPE}
ASFLAGS    = -PD "Type SETS \"${TYPE}\""

include StdTools
include AAsmModule

# Dynamic dependencies:
