# Makefile for PDumper (Printer Dumpers)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 27-May-94  AMcC         Created.
# 03-Nov-94  MJS          Added EscapeP2 dumper
# 24-Jul-95  MJS          Added CanonX dumper
#

#
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
            EscapeP2 \
            CanonX

#
# Generic options:
#
CP      = copy
DIR     = @dir
WIPE    = -wipe
MAKE   ?= amu
SUBMAKE = @${MAKE} ${MFLAGS} ${MAKECMDGOALS}

STRIPDEPEND = stripdepnd

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
        CPFDL PDumperCX
        |
        Unset Alias$CPFDL
        Unset Alias$CPFD
        @echo ${COMPONENT}: installed (disc)

install_rom: ${DUMPERS}
        ${CP} rm.PDumperDM ${INSTDIR}.PDumperDM ${CPFLAGS}
        ${CP} rm.PDumperIW ${INSTDIR}.PDumperIW ${CPFLAGS}
        ${CP} rm.PDumperLJ ${INSTDIR}.PDumperLJ ${CPFLAGS}
        ${CP} rm.PDumperE2 ${INSTDIR}.PDumperE2 ${CPFLAGS}
        ${CP} rm.PDumperCX ${INSTDIR}.PDumperCX ${CPFLAGS}
        @echo ${COMPONENT}: installed (rom)

clean:
        ${WIPE} rm.* ${WFLAGS}
        ${STRIPDEPEND} PDumper24.Makefile
        ${STRIPDEPEND} PDumperCX.Makefile
        ${STRIPDEPEND} PDumperDM.Makefile
        ${STRIPDEPEND} PDumperE2.Makefile
        ${STRIPDEPEND} PDumperIW.Makefile
        ${STRIPDEPEND} PDumperLJ.Makefile
        ${STRIPDEPEND} PDumperSP.Makefile
        @echo ${COMPONENT}: cleaned

${EXP_HDR}.PDumper: hdr.PDumper
        ${CP} hdr.PDumper $@ ${CPFLAGS}

#----------------------------------------------------------------------
# Supported bit image printers
#----------------------------------------------------------------------

DotMatrix:
        ${DIR} PDumperDM
        ${SUBMAKE} ^.rm.PDumperDM
        @back

ImageWriter:
        ${DIR} PDumperIW
        ${SUBMAKE} ^.rm.PDumperIW
        @back

LaserJet:
        ${DIR} PDumperLJ
        ${SUBMAKE} ^.rm.PDumperLJ
        @back

EscapeP2:
        ${DIR} PDumperE2
        ${SUBMAKE} ^.rm.PDumperE2
        @back

CanonX:
        ${DIR} PDumperCX
        ${SUBMAKE} ^.rm.PDumperCX
        @back

24Pin:
        ${DIR} PDumper24
        ${SUBMAKE} ^.rm.PDumper24
        @back

Sprite:
        ${DIR} PDumperSP
        ${SUBMAKE} ^.rm.PDumperSP
        @back

# Dynamic dependencies:
