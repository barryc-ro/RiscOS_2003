# Makefile for Podule {Expansion cards}
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 28-Jun-94  AMcC         Added extra development rules.
# 30-Aug-94  AMcC         Added resources: target
# 16-Jan-96  JRH          Makes different targets for different machines
# 11-Jan-00  PMS          Converted to use objasm instead of aasm so that we
#                         can pass in assembly-time constants to control
#                         fake podule header for Funai 5 Ethernet NC
#                         which doesn't have a podule ROM. Paul Skirrow.
# 13-Jan-00  PMS          Creates/deletes o dir, (ie don't keep o in cvs)
# 13-Jan-00  PMS          Passes Options from MakeFile to ObjAsm 
# 06-Jun-00  SNB          Recreated using makefile fragments

COMPONENT = Podule

#
# Program specific options:
#
CHEADER1  = ${COMPONENT}
HEADER1   = ${COMPONENT}

TOKHELPSRC = ${TOKENSOURCE}
HELPSRC    = HelpTexts
ROM_SOURCE = GetAll.s

ASFLAGS    = ${OPTIONS}

include StdTools
include AAsmModule

#
# Generic rules:
#
clean::
        ${WIPE} o ${WFLAGS}
        @echo ${COMPONENT}: cleaned 

# Dynamic dependencies:
