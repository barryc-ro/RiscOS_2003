# Makefile for STBError
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 21/06/95  Rich Buckley  Copied from CatchErr
# 31/01/96  Rich	  Removed Throwback flag
#

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

#
# Program specific options:
#
COMPONENT  = Diagnostic
SOURCE     = s.Diagnostic
TARGET     = rm.Diagnostic

#
# Generic rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: module built
        
install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: 
        @echo ${COMPONENT}: export not required

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

        
# Dynamic dependencies:
rm.Diagnostic: Hdr:ListOpts
rm.Diagnostic: Hdr:Macros
rm.Diagnostic: Hdr:System
rm.Diagnostic: Hdr:SWIs
rm.Diagnostic: Hdr:CPU.Generic26
rm.Diagnostic: Hdr:IO.GenericIO
rm.Diagnostic: Hdr:RISCOS
rm.Diagnostic: Hdr:Machine.<Machine>
rm.Diagnostic: Hdr:IO.IOMDL
rm.Diagnostic: Hdr:Debug
rm.Diagnostic: Hdr:ModHand
rm.Diagnostic: Hdr:Symbols
rm.Diagnostic: Hdr:Proc
rm.Diagnostic: Hdr:STBState
rm.Diagnostic: Hdr:Services
rm.Diagnostic: Hdr:CMOS
rm.Diagnostic: Hdr:Font
rm.Diagnostic: Hdr:ColourTran
rm.Diagnostic: Hdr:Podule
rm.Diagnostic: Hdr:PoduleReg
rm.Diagnostic: Version
