# Makefile for GenericCD Messages Module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 12-Sep-95  JRH          Created.
# 09-Oct-95  JRH          Made clean target work
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
COMPONENT   = GenCDMsgs
TARGET      = rm.GenCDMsgs
MODULE_HELP = "GenericCD Messages"
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
rm.GenCDMsgs: Hdr:ListOpts
rm.GenCDMsgs: Hdr:Macros
rm.GenCDMsgs: Hdr:System
rm.GenCDMsgs: Hdr:SWIs
rm.GenCDMsgs: Hdr:CPU.Generic26
rm.GenCDMsgs: Hdr:IO.GenericIO
rm.GenCDMsgs: Hdr:RISCOS
rm.GenCDMsgs: Hdr:ModHand
rm.GenCDMsgs: Hdr:Services
rm.GenCDMsgs: Hdr:Proc
rm.GenCDMsgs: Hdr:ResourceFS
rm.GenCDMsgs: Version
rm.GenCDMsgs: Hdr:NDRDebug
