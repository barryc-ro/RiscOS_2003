# Makefile for EMC Messages Module
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
COMPONENT   = EMCMsgs
TARGET      = rm.EMCMsgs
MODULE_HELP = "EMC Messages"
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
all: ${TARGET}
        @echo ${COMPONENT}: rom module built

install: ${TARGET}
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
rm.EMCMsgs: Hdr:ListOpts
rm.EMCMsgs: Hdr:Macros
rm.EMCMsgs: Hdr:System
rm.EMCMsgs: Hdr:SWIs
rm.EMCMsgs: Hdr:CPU.Generic26
rm.EMCMsgs: Hdr:IO.GenericIO
rm.EMCMsgs: Hdr:RISCOS
rm.EMCMsgs: Hdr:ModHand
rm.EMCMsgs: Hdr:Services
rm.EMCMsgs: Hdr:Proc
rm.EMCMsgs: Hdr:ResourceFS
rm.EMCMsgs: Version
rm.EMCMsgs: Hdr:NDRDebug
