# Makefile for ModeFiles
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 01-Feb-96  JRH          Created.
#

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To ${TARGET} -From
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = ModeFiles
SOURCE    = s.ModeFiles
TDIR      = rm.${MACHINE}
TARGET    = ${TDIR}.${TYPE}
RDIR      = ${RESDIR}.Monitors
#
# Generic rules:
#
rom: rom_${TYPE}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: rom_${TYPE}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} ${TDIR} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RDIR}
        ${CP} Resources.${LOCALE}.${MACHINE} ${RDIR} ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

rom_PAL: ${SOURCE} s.PAL
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.PAL

rom_NTSC: ${SOURCE} s.NTSC
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.NTSC

# Dynamic dependencies:
rm.Boca.PAL: s.ModeFiles
rm.Boca.PAL: Hdr:ListOpts
rm.Boca.PAL: Hdr:Macros
rm.Boca.PAL: Hdr:System
rm.Boca.PAL: Hdr:SWIs
rm.Boca.PAL: Hdr:CPU.Generic26
rm.Boca.PAL: Hdr:IO.GenericIO
rm.Boca.PAL: Hdr:RISCOS
rm.Boca.PAL: Hdr:ModHand
rm.Boca.PAL: Hdr:Machine.<Machine>
rm.Boca.PAL: Hdr:Machine.Machine
rm.Boca.PAL: Hdr:Proc
rm.Boca.PAL: Hdr:Services
rm.Boca.PAL: Hdr:FSNumbers
rm.Boca.PAL: Hdr:HighFSI
rm.Boca.PAL: Hdr:NewErrors
rm.Boca.PAL: Hdr:MsgTrans
rm.Boca.PAL: Version
rm.Boca.NTSC: s.ModeFiles
rm.Boca.NTSC: Hdr:ListOpts
rm.Boca.NTSC: Hdr:Macros
rm.Boca.NTSC: Hdr:System
rm.Boca.NTSC: Hdr:SWIs
rm.Boca.NTSC: Hdr:CPU.Generic26
rm.Boca.NTSC: Hdr:IO.GenericIO
rm.Boca.NTSC: Hdr:RISCOS
rm.Boca.NTSC: Hdr:ModHand
rm.Boca.NTSC: Hdr:Machine.<Machine>
rm.Boca.NTSC: Hdr:Machine.Machine
rm.Boca.NTSC: Hdr:Proc
rm.Boca.NTSC: Hdr:Services
rm.Boca.NTSC: Hdr:FSNumbers
rm.Boca.NTSC: Hdr:HighFSI
rm.Boca.NTSC: Hdr:NewErrors
rm.Boca.NTSC: Hdr:MsgTrans
rm.Boca.NTSC: Version
rm.Omega.PAL: s.ModeFiles
rm.Omega.PAL: Hdr:ListOpts
rm.Omega.PAL: Hdr:Macros
rm.Omega.PAL: Hdr:System
rm.Omega.PAL: Hdr:SWIs
rm.Omega.PAL: Hdr:CPU.Generic26
rm.Omega.PAL: Hdr:IO.GenericIO
rm.Omega.PAL: Hdr:RISCOS
rm.Omega.PAL: Hdr:ModHand
rm.Omega.PAL: Hdr:Machine.<Machine>
rm.Omega.PAL: Hdr:Machine.Machine
rm.Omega.PAL: Hdr:Proc
rm.Omega.PAL: Hdr:Services
rm.Omega.PAL: Hdr:FSNumbers
rm.Omega.PAL: Hdr:HighFSI
rm.Omega.PAL: Hdr:NewErrors
rm.Omega.PAL: Hdr:MsgTrans
rm.Omega.PAL: Version
rm.Omega.NTSC: s.ModeFiles
rm.Omega.NTSC: Hdr:ListOpts
rm.Omega.NTSC: Hdr:Macros
rm.Omega.NTSC: Hdr:System
rm.Omega.NTSC: Hdr:SWIs
rm.Omega.NTSC: Hdr:CPU.Generic26
rm.Omega.NTSC: Hdr:IO.GenericIO
rm.Omega.NTSC: Hdr:RISCOS
rm.Omega.NTSC: Hdr:ModHand
rm.Omega.NTSC: Hdr:Machine.<Machine>
rm.Omega.NTSC: Hdr:Machine.Machine
rm.Omega.NTSC: Hdr:Proc
rm.Omega.NTSC: Hdr:Services
rm.Omega.NTSC: Hdr:FSNumbers
rm.Omega.NTSC: Hdr:HighFSI
rm.Omega.NTSC: Hdr:NewErrors
rm.Omega.NTSC: Hdr:MsgTrans
rm.Omega.NTSC: Version
