# Makefile for BrowserMsg Messages Module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-Feb-95  AMcC         Created.
# 30-Oct-95  RWB	  Modified from ADFSRom module

#
# Generic options:
#
AS       = aasm
CP       = copy
MKDIR    = cdir
MODGEN   = modgen
MODSQZ   = modsqz
RM       = remove
SCANRES  = scanres
WIPE     = -wipe
MAKE     = amu

CFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
TOUCH   = create
WFLAGS  = ~cr~v

#
# Program specific options:
#
COMPONENT   = BrowserMsg
TARGET      = rm.BrowserMsg
MODULE_HELP = "BrowserRom Messages"
SOURCE      = s.Messages
RESDIR      = Resources.${LOCALE}.${SYSTEM}
UTILS       = Utils
DATA        = Data.${LOCALE}.${SYSTEM}
RESDATA     = ${Data}.Data

#
# Utility programs:
#
SCANRES = ${UTILS}.ScanRes

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${WIPE} Data.${LOCALE} ${WFLAGS}
        ${WIPE} Resources.${LOCALE} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

dirs:
	${MKDIR} Data
	${MKDIR} Data.${LOCALE}
	${MKDIR} Data.${LOCALE}.${SYSTEM}

${TARGET}: ${SOURCE} ${SCANRES} dirs
        ${SCANRES} ${RESDIR} ${RESDATA}
        set resdata ${RESDATA}
        ${AS} ${ASFLAGS} -To $@ -From ${SOURCE}
        unset resdata

${SCANRES}:
        Dir ${UTILS}
        ${MAKE}
        Up

# Dynamic dependencies:
rm.ADFSMsgs: Hdr:ListOpts
rm.ADFSMsgs: Hdr:Macros
rm.ADFSMsgs: Hdr:System
rm.ADFSMsgs: Hdr:SWIs
rm.ADFSMsgs: Hdr:CPU.Generic26
rm.ADFSMsgs: Hdr:IO.GenericIO
rm.ADFSMsgs: Hdr:RISCOS
rm.ADFSMsgs: Hdr:ModHand
rm.ADFSMsgs: Hdr:Services
rm.ADFSMsgs: Hdr:Proc
rm.ADFSMsgs: Hdr:ResourceFS
rm.ADFSMsgs: Version
rm.ADFSMsgs: Hdr:NDRDebug
rm.BrowserMsg: Hdr:ListOpts
rm.BrowserMsg: Hdr:Macros
rm.BrowserMsg: Hdr:System
rm.BrowserMsg: Hdr:SWIs
rm.BrowserMsg: Hdr:CPU.Generic26
rm.BrowserMsg: Hdr:IO.GenericIO
rm.BrowserMsg: Hdr:RISCOS
rm.BrowserMsg: Hdr:ModHand
rm.BrowserMsg: Hdr:Services
rm.BrowserMsg: Hdr:Proc
rm.BrowserMsg: Hdr:ResourceFS
rm.BrowserMsg: Version
rm.BrowserMsg: Hdr:NDRDebug
