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
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = ModeFiles
SOURCE    = s.ModeFiles
TDIR      = rm.${MACHINE}
TARGET    = ${TDIR}.ModeFiles
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

${TARGET}: ${SOURCE}
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
rm.Omega.ModeFiles: Hdr:ListOpts
rm.Omega.ModeFiles: Hdr:Macros
rm.Omega.ModeFiles: Hdr:System
rm.Omega.ModeFiles: Hdr:SWIs
rm.Omega.ModeFiles: Hdr:CPU.Generic26
rm.Omega.ModeFiles: Hdr:IO.GenericIO
rm.Omega.ModeFiles: Hdr:RISCOS
rm.Omega.ModeFiles: Hdr:ModHand
rm.Omega.ModeFiles: Hdr:Machine.<Machine>
rm.Omega.ModeFiles: Hdr:Machine.Machine
rm.Omega.ModeFiles: Hdr:Proc
rm.Omega.ModeFiles: Hdr:Services
rm.Omega.ModeFiles: Hdr:FSNumbers
rm.Omega.ModeFiles: Hdr:HighFSI
rm.Omega.ModeFiles: Hdr:NewErrors
rm.Omega.ModeFiles: Hdr:MsgTrans
rm.Omega.ModeFiles: Version
rm.Boca.ModeFiles: Hdr:ListOpts
rm.Boca.ModeFiles: Hdr:Macros
rm.Boca.ModeFiles: Hdr:System
rm.Boca.ModeFiles: Hdr:SWIs
rm.Boca.ModeFiles: Hdr:CPU.Generic26
rm.Boca.ModeFiles: Hdr:IO.GenericIO
rm.Boca.ModeFiles: Hdr:RISCOS
rm.Boca.ModeFiles: Hdr:ModHand
rm.Boca.ModeFiles: Hdr:Machine.<Machine>
rm.Boca.ModeFiles: Hdr:Machine.Machine
rm.Boca.ModeFiles: Hdr:Proc
rm.Boca.ModeFiles: Hdr:Services
rm.Boca.ModeFiles: Hdr:FSNumbers
rm.Boca.ModeFiles: Hdr:HighFSI
rm.Boca.ModeFiles: Hdr:NewErrors
rm.Boca.ModeFiles: Hdr:MsgTrans
rm.Boca.ModeFiles: Version
