# Makefile for PDumper (Printer Dumpers)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 27-May-94  AMcC         Created.
# 03-Nov-94  MJS          Added EscapeP2 dumper
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Program specific options:
#
COMPONENT = PDumpers
EXPORTS   = ${EXP_HDR}.PDumper
DUMPERS   = DotMatrix \
            ImageWriter \
            LaserJet \
            EscapeP2

#
# Generic options:
#
AS      = aasm
CP      = copy
MAKE    = @amu ${MFLAGS}
MKDIR   = cdir
DIR     = @dir
RM      = remove
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Generic rules:
#
all: ${DUMPERS}
        @echo ${COMPONENT}: all built (disc)

rom: ${DUMPERS}
        @echo ${COMPONENT}: all built (rom)

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: ${DUMPERS}
        Set Alias$CPFD $(CP) rm.%0 $(INSTDIR).%0 $(CPFLAGS)
        Set Alias$CPFDL CPFD %0|MAccess $(INSTDIR).%0 LR/r
        |
        CPFDL PDumperDM
        CPFDL PDumperIW
        CPFDL PDumperLJ
        CPFDL PDumperE2
        |
        Unset Alias$CPFDL
        Unset Alias$CPFD
        @echo ${COMPONENT}: installed (disc)

install_rom: ${DUMPERS}
        ${CP} rm.PDumperDM ${INSTDIR}.PDumperDM ${CPFLAGS}
        ${CP} rm.PDumperIW ${INSTDIR}.PDumperIW ${CPFLAGS}
        ${CP} rm.PDumperLJ ${INSTDIR}.PDumperLJ ${CPFLAGS}
        ${CP} rm.PDumperE2 ${INSTDIR}.PDumperE2 ${CPFLAGS}
        @echo ${COMPONENT}: installed (rom)

clean:
        ${WIPE} rm.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${EXP_HDR}.PDumper: hdr.PDumper
        ${CP} hdr.PDumper $@ ${CPFLAGS}

#----------------------------------------------------------------------
# Supported bit image printers
#----------------------------------------------------------------------

DotMatrix:
        ${DIR} PDumperDM
        ${MAKE} ^.rm.PDumperDM
        @back

ImageWriter:
        ${DIR} PDumperIW
        ${MAKE} ^.rm.PDumperIW
        @back

LaserJet:
        ${DIR} PDumperLJ
        ${MAKE} ^.rm.PDumperLJ
        @back

EscapeP2:
        ${DIR} PDumperE2
        ${MAKE} ^.rm.PDumperE2
        @back

24Pin:
        ${DIR} PDumper24
        ${MAKE} ^.rm.PDumper24
        @back

Sprite:
        ${DIR} PDumperSP
        ${MAKE} ^.rm.PDumperSP
        @back

# Dynamic dependencies:
