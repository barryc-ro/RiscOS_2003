# Makefile for PowerOn
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 29-Mar-96 Rich Buckley  Created
# 25-Feb-96 Neil Turton   Stolen and adapted  :-)
# 12-Sep-97 Ant Skelton	  encore une fois
# 06-Jan-00 Simon Forrest Added support for StripDepnd tool

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
STRIP   = stripdepnd
ASFLAGS = -depend !Depend -Stamp -quit -module -Esc -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = PowerOn
SOURCE     = s.poweron
TARGET     = rm.${COMPONENT}

#
# Generic rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: standalone module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${STRIP}
        @echo ${COMPONENT}: cleaned

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

resources:
	@echo ${COMPONENT}: I don't have any resources

${TARGET}: ${SOURCE}
	${MKDIR} rm
        ${AS} ${ASFLAGS} ${SOURCE} ${THROWBACK}
        
# Dynamic dependencies:
