# Makefile for CacheFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 19-Apr-96  NRaine       Created from PipeFS MakeFile.
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
COMPONENT = CacheFS
SOURCE    = s.CacheFS
TARGET    = rm.CacheFS

#
# Generic rules:
#
rom:    ${TARGET}
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

CheckIt: s.CheckIt
        ${AS} ${ASFLAGS} s.CheckIt
        SetType CheckIt Utility

# Dynamic dependencies:
rm.CacheFS: s.Declare
rm.CacheFS: Hdr:ListOpts
rm.CacheFS: Hdr:Macros
rm.CacheFS: Hdr:System
rm.CacheFS: Hdr:SWIs
rm.CacheFS: Hdr:CPU.Generic26
rm.CacheFS: Hdr:IO.GenericIO
rm.CacheFS: Hdr:RISCOS
rm.CacheFS: Hdr:ModHand
rm.CacheFS: Hdr:Services
rm.CacheFS: Hdr:CMOS
rm.CacheFS: Hdr:Proc
rm.CacheFS: Hdr:FSNumbers
rm.CacheFS: Hdr:HighFSI
rm.CacheFS: Hdr:LowFSi
rm.CacheFS: Hdr:NewErrors
rm.CacheFS: Hdr:Variables
rm.CacheFS: Hdr:Wimp
rm.CacheFS: Hdr:UpCall
rm.CacheFS: Hdr:MsgTrans
rm.CacheFS: Version
rm.CacheFS: Hdr:Trace
rm.CacheFS: s.MyMacros
rm.CacheFS: s.MoveBytes
rm.CacheFS: s.Test
