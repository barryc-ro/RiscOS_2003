# Makefile for USA Territory
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
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
COMPONENT = USA
SOURCE    = s.USA
TARGET    = rm.USA

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export:
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
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
rm.USA: Hdr:ListOpts
rm.USA: Hdr:Macros
rm.USA: Hdr:System
rm.USA: Hdr:SWIs
rm.USA: Hdr:CPU.Generic26
rm.USA: Hdr:IO.GenericIO
rm.USA: Hdr:RISCOS
rm.USA: Hdr:ModHand
rm.USA: Hdr:Services
rm.USA: Hdr:FSNumbers
rm.USA: Hdr:NewErrors
rm.USA: Hdr:Variables
rm.USA: Hdr:Proc
rm.USA: Hdr:Messages
rm.USA: Hdr:CMOS
rm.USA: Hdr:MsgTrans
rm.USA: Hdr:Territory
rm.USA: Hdr:NdrDebug
rm.USA: Version
rm.USA: s.ModHead
rm.USA: s.Entries
rm.USA: s.TimeZones
rm.USA: s.Symbols
rm.USA: s.Calendar
rm.USA: s.Tables
rm.USA: s.DateTime
rm.USA: s.Transform
rm.USA: s.tail
