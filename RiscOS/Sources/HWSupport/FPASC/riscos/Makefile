# Makefile for Floating Point Accelerator Support Code (FPASC)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name     Description
# ----       ----     -----------
# 25-May-94  AMcC     Created.
# 17-Aug-94  AMcC     Removed Specific machine names.
#                     If building for old series machines, comment out A600
#                     target and uncomment ARM2/3 target.
# 28-Jan-97  KJB      If building for a kernel that is capable of operating
#                     in a 32-mode with interrupts enabled, you can change
#                     the APCS to 32bit for a more efficient emulator

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = do mkdir -p
AS      = ObjAsm
CP      = Copy
LD      = Link
RM      = Remove
WIPE    = -Wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Program specific options:
#
COMPONENT = FPEmulator
TARGET    = rm.${MACHINE}.FPEmulator
EXPORTS   = ${EXP_HDR}.FPEmulator

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: ${TARGET}
        ${MKDIR} ${INSTDIR}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ram module installed

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${WIPE} o.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: o.fpe
        ${MKDIR} rm.${MACHINE}
        ${LD} -rmf -o $@ o.fpe

o.fpe: s.${System}
        ${AS} -I Hdr:t.^ ${ASFLAGS} -APCS ${FPE_APCS} -CPU ARM7M s.${System} $@

${EXP_HDR}.FPEmulator: hdr.FPEmulator
        ${CP} hdr.FPEmulator $@ ${CPFLAGS}

# Dynamic dependencies:
