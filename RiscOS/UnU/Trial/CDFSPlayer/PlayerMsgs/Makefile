# Makefile for CDFSPlayer Messages Module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-Feb-95  AMcC         Created.
#

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
COMPONENT   = PlayerMsgs
TARGET      = rm.PlayerMsgs
MODULE_HELP = "CDFSPlayer Messages"
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
rm.PlayerMsgs: Hdr:ListOpts
rm.PlayerMsgs: Hdr:Macros
rm.PlayerMsgs: Hdr:System
rm.PlayerMsgs: Hdr:SWIs
rm.PlayerMsgs: Hdr:CPU.Generic26
rm.PlayerMsgs: Hdr:IO.GenericIO
rm.PlayerMsgs: Hdr:RISCOS
rm.PlayerMsgs: Hdr:ModHand
rm.PlayerMsgs: Hdr:Services
rm.PlayerMsgs: Hdr:Proc
rm.PlayerMsgs: Hdr:ResourceFS
rm.PlayerMsgs: Version
rm.PlayerMsgs: Hdr:NDRDebug
