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
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = ModeFiles
SOURCE    = s.ModeFiles
TARGET    = rm.ModeFiles
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
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RDIR}
        ${CP} Resources.${LOCALE}.PALTV   ${RDIR}.PALTV   ${CPFLAGS}
        ${CP} Resources.${LOCALE}.NTSCTV  ${RDIR}.NTSCTV  ${CPFLAGS}
        ${CP} Resources.${LOCALE}.VGA     ${RDIR}.VGA     ${CPFLAGS}
        ${CP} Resources.${LOCALE}.VGAPlus ${RDIR}.VGAPlus ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
rm.ModeFiles: Hdr:ListOpts
rm.ModeFiles: Hdr:Macros
rm.ModeFiles: Hdr:System
rm.ModeFiles: Hdr:SWIs
rm.ModeFiles: Hdr:CPU.Generic26
rm.ModeFiles: Hdr:IO.GenericIO
rm.ModeFiles: Hdr:RISCOS
rm.ModeFiles: Hdr:ModHand
rm.ModeFiles: Hdr:Machine.<Machine>
rm.ModeFiles: Hdr:Proc
rm.ModeFiles: Hdr:Services
rm.ModeFiles: Hdr:FSNumbers
rm.ModeFiles: Hdr:HighFSI
rm.ModeFiles: Hdr:NewErrors
rm.ModeFiles: Hdr:MsgTrans
rm.ModeFiles: Version
