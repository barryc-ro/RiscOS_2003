# Makefile for BASIC
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 08-Jun-94  AMcC   Created.
# 08-Nov-94  AMcC   Updated to install BASIC64 (non ROM)

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
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
EXPORTS   = 
TARGETS   = rm.${COMPONENT}

#
# Generic rules:
#
all: rm.BASIC64
        @echo ${COMPONENT}: BASIC64 module built

rom: ${TARGETS}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: rm.BASIC64
        ${CP} rm.BASIC64 ${INSTDIR}.BASIC64 ${CPFLAGS}
        Access ${INSTDIR}.BASIC64 WR/r
        @echo ${COMPONENT}: BASIC64 installed (Disc)

install_rom: ${TARGETS}
        ${CP} rm.${COMPONENT} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} rm.BASIC64
        ${RM} rm.BASIC105
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

rm.BASIC105: ModuleAB
        ${AS} ${ASFLAGS} ModuleAB

rm.BASIC64: ModuleAB64
        ${AS} ${ASFLAGS} ModuleAB64

# Dynamic dependencies:
