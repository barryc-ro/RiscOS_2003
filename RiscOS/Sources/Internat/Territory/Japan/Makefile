# Makefile for Japan Territory
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
COMPONENT = Japan
SOURCE    = s.Japan
TARGET    = rm.Japan

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
rm.Japan: Hdr:ListOpts
rm.Japan: Hdr:Macros
rm.Japan: Hdr:System
rm.Japan: Hdr:SWIs
rm.Japan: Hdr:CPU.Generic26
rm.Japan: Hdr:IO.GenericIO
rm.Japan: Hdr:RISCOS
rm.Japan: Hdr:ModHand
rm.Japan: Hdr:Services
rm.Japan: Hdr:FSNumbers
rm.Japan: Hdr:NewErrors
rm.Japan: Hdr:Variables
rm.Japan: Hdr:Proc
rm.Japan: Hdr:Messages
rm.Japan: Hdr:CMOS
rm.Japan: Hdr:MsgTrans
rm.Japan: Hdr:Territory
rm.Japan: Hdr:Countries
rm.Japan: Hdr:NdrDebug
rm.Japan: Version
rm.Japan: s.ModHead
rm.Japan: s.Entries
rm.Japan: s.TimeZones
rm.Japan: s.Symbols
rm.Japan: s.Calendar
rm.Japan: s.Tables
rm.Japan: s.DateTime
rm.Japan: s.Transform
rm.Japan: s.tail
