# Makefile for STBError
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 12/5/95  Rich Buckley   Newish
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
ASFLAGS = -depend !Depend -Stamp -quit -module -Throwback -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = CatchErr
SOURCE     = s.CatchErr
TARGET     = rm.CatchErr

#
# Generic rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: module built
        
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
rm.STBError: Hdr:ListOpts
rm.STBError: Hdr:Macros
rm.STBError: Hdr:System
rm.STBError: Hdr:SWIs
rm.STBError: Hdr:CPU.Generic26
rm.STBError: Hdr:IO.GenericIO
rm.STBError: Hdr:RISCOS
rm.STBError: Hdr:Machine.<Machine>
rm.STBError: Hdr:ModHand
rm.STBError: Hdr:Symbols
rm.STBError: Hdr:Proc
rm.STBError: Version
rm.CatchErr: Hdr:ListOpts
rm.CatchErr: Hdr:Macros
rm.CatchErr: Hdr:System
rm.CatchErr: Hdr:SWIs
rm.CatchErr: Hdr:CPU.Generic26
rm.CatchErr: Hdr:IO.GenericIO
rm.CatchErr: Hdr:RISCOS
rm.CatchErr: Hdr:Machine.<Machine>
rm.CatchErr: Hdr:ModHand
rm.CatchErr: Hdr:Symbols
rm.CatchErr: Hdr:Proc
rm.CatchErr: Version
