# Makefile for International module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 21-Nov-00  SBF          Added StripDepnd support.
# 15-Mar-01  SNB          Recreated.
#

COMPONENT   = International
TARGET      = Inter
ROM_SOURCE  = InterMod.s
RESFSDIR    = ${RESDIR}.Internatio
HELPSRC     = s.IntHelp
TOKENSOURCE = s.IntHelpTok
TOKHELPSRC  = s.IntHelpTok

include StdTools
include AAsmModule

clean::
        ${RM} s.IntHelpDat
        ${RM} ${HELPSRC}
        @echo ${COMPONENT}: cleaned

s.IntHelp: IntHelpSrc IntHelpBody
        ${AS} -ABS ${ASFLAGS} -o s.IntHelpDat -From IntHelpSrc
        ${LD} -bin -o $@ s.IntHelpDat

# Dynamic dependencies:
