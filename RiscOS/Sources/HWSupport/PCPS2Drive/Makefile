# Makefile for PS2 Driver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-Jun-94  AMcC         Created.
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
COMPONENT = PS2Driver
SOURCE    = s.GetAll
TARGET    = rm.${MACHINE}.PS2Driver

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
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE}
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
rm.Morris.PS2Driver: Hdr:ListOpts
rm.Morris.PS2Driver: Hdr:Macros
rm.Morris.PS2Driver: Hdr:System
rm.Morris.PS2Driver: Hdr:SWIs
rm.Morris.PS2Driver: Hdr:CPU.Generic26
rm.Morris.PS2Driver: Hdr:IO.GenericIO
rm.Morris.PS2Driver: Hdr:RISCOS
rm.Morris.PS2Driver: Hdr:Machine.<Machine>
rm.Morris.PS2Driver: Hdr:IO.IOMDL
rm.Morris.PS2Driver: Hdr:DevNos
rm.Morris.PS2Driver: Hdr:KEYV
rm.Morris.PS2Driver: Hdr:ModHand
rm.Morris.PS2Driver: Hdr:Services
rm.Morris.PS2Driver: Hdr:MsgTrans
rm.Morris.PS2Driver: Hdr:ResourceFS
rm.Morris.PS2Driver: Hdr:Buffer
rm.Morris.PS2Driver: Hdr:FSNumbers
rm.Morris.PS2Driver: Hdr:CMOS
rm.Morris.PS2Driver: Hdr:HostFS
rm.Morris.PS2Driver: Hdr:NdrDebug
rm.Morris.PS2Driver: Hdr:DDVMacros
rm.Morris.PS2Driver: Hdr:Proc
rm.Morris.PS2Driver: Hdr:Pointer
rm.Morris.PS2Driver: Hdr:Debug
rm.Morris.PS2Driver: Hdr:Portable
rm.Morris.PS2Driver: Version
rm.Morris.PS2Driver: s.Front
rm.Morris.PS2Driver: s.Macros
rm.Morris.PS2Driver: s.ModHead
rm.Morris.PS2Driver: s.ResFiles
rm.Morris.PS2Driver: s.PS2Driver
rm.Morris.PS2Driver: s.Tail
rm.Phoebe.PS2Driver: Hdr:ListOpts
rm.Phoebe.PS2Driver: Hdr:Macros
rm.Phoebe.PS2Driver: Hdr:System
rm.Phoebe.PS2Driver: Hdr:SWIs
rm.Phoebe.PS2Driver: Hdr:CPU.Generic26
rm.Phoebe.PS2Driver: Hdr:IO.GenericIO
rm.Phoebe.PS2Driver: Hdr:RISCOS
rm.Phoebe.PS2Driver: Hdr:Machine.<Machine>
rm.Phoebe.PS2Driver: Hdr:IO.IOMDL
rm.Phoebe.PS2Driver: Hdr:DevNos
rm.Phoebe.PS2Driver: Hdr:KEYV
rm.Phoebe.PS2Driver: Hdr:ModHand
rm.Phoebe.PS2Driver: Hdr:Services
rm.Phoebe.PS2Driver: Hdr:MsgTrans
rm.Phoebe.PS2Driver: Hdr:ResourceFS
rm.Phoebe.PS2Driver: Hdr:Buffer
rm.Phoebe.PS2Driver: Hdr:FSNumbers
rm.Phoebe.PS2Driver: Hdr:CMOS
rm.Phoebe.PS2Driver: Hdr:HostFS
rm.Phoebe.PS2Driver: Hdr:NdrDebug
rm.Phoebe.PS2Driver: Hdr:DDVMacros
rm.Phoebe.PS2Driver: Hdr:Proc
rm.Phoebe.PS2Driver: Hdr:Pointer
rm.Phoebe.PS2Driver: Hdr:Debug
rm.Phoebe.PS2Driver: Hdr:Portable
rm.Phoebe.PS2Driver: Version
rm.Phoebe.PS2Driver: s.Front
rm.Phoebe.PS2Driver: s.Macros
rm.Phoebe.PS2Driver: s.ModHead
rm.Phoebe.PS2Driver: s.ResFiles
rm.Phoebe.PS2Driver: s.PS2Driver
rm.Phoebe.PS2Driver: s.Tail
