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
ASFLAGS = -depend !Depend -Stamp -quit -module ${THROWBACK} -To ${TARGET} -From
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = ModeFiles
SOURCE    = s.ModeFiles
TDIR      = rm.${MACHINE}
TARGET    = ${TDIR}.MF${TYPE}
RDIR      = ${RESDIR}.Monitors
#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} ${TDIR} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RDIR}
        ${CP} Resources.${LOCALE}.${MACHINE} ${RDIR} ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TDIR}.MFPAL: ${SOURCE} s.PAL
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.PAL

${TDIR}.MFNTSC: ${SOURCE} s.NTSC
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.NTSC

${TDIR}.MF: ${SOURCE} s.ModeFiles
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.ModeFiles

# Dynamic dependencies:
rm.Funai.MFNTSC: s.ModeFiles
rm.Funai.MFNTSC: Hdr:ListOpts
rm.Funai.MFNTSC: Hdr:Macros
rm.Funai.MFNTSC: Hdr:System
rm.Funai.MFNTSC: Hdr:SWIs
rm.Funai.MFNTSC: Hdr:CPU.Generic26
rm.Funai.MFNTSC: Hdr:IO.GenericIO
rm.Funai.MFNTSC: Hdr:RISCOS
rm.Funai.MFNTSC: Hdr:ModHand
rm.Funai.MFNTSC: Hdr:Machine.<Machine>
rm.Funai.MFNTSC: Hdr:Machine.Machine
rm.Funai.MFNTSC: Hdr:IO.IOMDL
rm.Funai.MFNTSC: Hdr:Proc
rm.Funai.MFNTSC: Hdr:Services
rm.Funai.MFNTSC: Hdr:FSNumbers
rm.Funai.MFNTSC: Hdr:HighFSI
rm.Funai.MFNTSC: Hdr:NewErrors
rm.Funai.MFNTSC: Hdr:MsgTrans
rm.Funai.MFNTSC: VersionASM
rm.Funai.MFPAL: s.ModeFiles
rm.Funai.MFPAL: Hdr:ListOpts
rm.Funai.MFPAL: Hdr:Macros
rm.Funai.MFPAL: Hdr:System
rm.Funai.MFPAL: Hdr:SWIs
rm.Funai.MFPAL: Hdr:CPU.Generic26
rm.Funai.MFPAL: Hdr:IO.GenericIO
rm.Funai.MFPAL: Hdr:RISCOS
rm.Funai.MFPAL: Hdr:ModHand
rm.Funai.MFPAL: Hdr:Machine.<Machine>
rm.Funai.MFPAL: Hdr:Machine.Machine
rm.Funai.MFPAL: Hdr:IO.IOMDL
rm.Funai.MFPAL: Hdr:Proc
rm.Funai.MFPAL: Hdr:Services
rm.Funai.MFPAL: Hdr:FSNumbers
rm.Funai.MFPAL: Hdr:HighFSI
rm.Funai.MFPAL: Hdr:NewErrors
rm.Funai.MFPAL: Hdr:MsgTrans
rm.Funai.MFPAL: VersionASM
rm.Boca.MFNTSC: s.ModeFiles
rm.Boca.MFNTSC: Hdr:ListOpts
rm.Boca.MFNTSC: Hdr:Macros
rm.Boca.MFNTSC: Hdr:System
rm.Boca.MFNTSC: Hdr:SWIs
rm.Boca.MFNTSC: Hdr:CPU.Generic26
rm.Boca.MFNTSC: Hdr:IO.GenericIO
rm.Boca.MFNTSC: Hdr:RISCOS
rm.Boca.MFNTSC: Hdr:ModHand
rm.Boca.MFNTSC: Hdr:Machine.<Machine>
rm.Boca.MFNTSC: Hdr:Machine.Machine
rm.Boca.MFNTSC: Hdr:IO.IOMDL
rm.Boca.MFNTSC: Hdr:Proc
rm.Boca.MFNTSC: Hdr:Services
rm.Boca.MFNTSC: Hdr:FSNumbers
rm.Boca.MFNTSC: Hdr:HighFSI
rm.Boca.MFNTSC: Hdr:NewErrors
rm.Boca.MFNTSC: Hdr:MsgTrans
rm.Boca.MFNTSC: VersionASM
rm.Boca.MFPAL: s.ModeFiles
rm.Boca.MFPAL: Hdr:ListOpts
rm.Boca.MFPAL: Hdr:Macros
rm.Boca.MFPAL: Hdr:System
rm.Boca.MFPAL: Hdr:SWIs
rm.Boca.MFPAL: Hdr:CPU.Generic26
rm.Boca.MFPAL: Hdr:IO.GenericIO
rm.Boca.MFPAL: Hdr:RISCOS
rm.Boca.MFPAL: Hdr:ModHand
rm.Boca.MFPAL: Hdr:Machine.<Machine>
rm.Boca.MFPAL: Hdr:Machine.Machine
rm.Boca.MFPAL: Hdr:IO.IOMDL
rm.Boca.MFPAL: Hdr:Proc
rm.Boca.MFPAL: Hdr:Services
rm.Boca.MFPAL: Hdr:FSNumbers
rm.Boca.MFPAL: Hdr:HighFSI
rm.Boca.MFPAL: Hdr:NewErrors
rm.Boca.MFPAL: Hdr:MsgTrans
rm.Boca.MFPAL: VersionASM
rm.Funai.MF: Hdr:ListOpts
rm.Funai.MF: Hdr:Macros
rm.Funai.MF: Hdr:System
rm.Funai.MF: Hdr:SWIs
rm.Funai.MF: Hdr:CPU.Generic26
rm.Funai.MF: Hdr:IO.GenericIO
rm.Funai.MF: Hdr:RISCOS
rm.Funai.MF: Hdr:ModHand
rm.Funai.MF: Hdr:Machine.<Machine>
rm.Funai.MF: Hdr:Machine.Machine
rm.Funai.MF: Hdr:IO.IOMDL
rm.Funai.MF: Hdr:Proc
rm.Funai.MF: Hdr:Services
rm.Funai.MF: Hdr:FSNumbers
rm.Funai.MF: Hdr:HighFSI
rm.Funai.MF: Hdr:NewErrors
rm.Funai.MF: Hdr:MsgTrans
rm.Funai.MF: VersionASM
rm.Omega.MFNTSC: s.ModeFiles
rm.Omega.MFNTSC: Hdr:ListOpts
rm.Omega.MFNTSC: Hdr:Macros
rm.Omega.MFNTSC: Hdr:System
rm.Omega.MFNTSC: Hdr:SWIs
rm.Omega.MFNTSC: Hdr:CPU.Generic26
rm.Omega.MFNTSC: Hdr:IO.GenericIO
rm.Omega.MFNTSC: Hdr:RISCOS
rm.Omega.MFNTSC: Hdr:ModHand
rm.Omega.MFNTSC: Hdr:Machine.<Machine>
rm.Omega.MFNTSC: Hdr:Machine.Machine
rm.Omega.MFNTSC: Hdr:IO.IOMDL
rm.Omega.MFNTSC: Hdr:Proc
rm.Omega.MFNTSC: Hdr:Services
rm.Omega.MFNTSC: Hdr:FSNumbers
rm.Omega.MFNTSC: Hdr:HighFSI
rm.Omega.MFNTSC: Hdr:NewErrors
rm.Omega.MFNTSC: Hdr:MsgTrans
rm.Omega.MFNTSC: VersionASM
rm.Omega.MFPAL: s.ModeFiles
rm.Omega.MFPAL: Hdr:ListOpts
rm.Omega.MFPAL: Hdr:Macros
rm.Omega.MFPAL: Hdr:System
rm.Omega.MFPAL: Hdr:SWIs
rm.Omega.MFPAL: Hdr:CPU.Generic26
rm.Omega.MFPAL: Hdr:IO.GenericIO
rm.Omega.MFPAL: Hdr:RISCOS
rm.Omega.MFPAL: Hdr:ModHand
rm.Omega.MFPAL: Hdr:Machine.<Machine>
rm.Omega.MFPAL: Hdr:Machine.Machine
rm.Omega.MFPAL: Hdr:IO.IOMDL
rm.Omega.MFPAL: Hdr:Proc
rm.Omega.MFPAL: Hdr:Services
rm.Omega.MFPAL: Hdr:FSNumbers
rm.Omega.MFPAL: Hdr:HighFSI
rm.Omega.MFPAL: Hdr:NewErrors
rm.Omega.MFPAL: Hdr:MsgTrans
rm.Omega.MFPAL: VersionASM
