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
MKDIR   = CDir
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

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${WIPE} o.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

# ARM600
#
${TARGET}: o.fpe
        ${MKDIR} rm.${MACHINE}
        ${LD} -bin -o $@ o.fpe
        SetType $@ &FFA

o.fpe: s.fpemedusa
        ${AS} -I Hdr:t.^ ${ASFLAGS} -APCS 3/32bit -CPU ARM7M s.fpemedusa $@

# ARM2/3
#
#${TARGET}: o.fpe400
#        ${MKDIR} rm.${MACHINE}
#        ${LD} -bin -o $@ o.fpe400
#        SetType $@ &FFA
#
#o.fpe400: s.fpe400
#        ${AS} -I Hdr:t.^ ${ASFLAGS} -APCS 3/26bit s.fpe400 $@

${EXP_HDR}.FPEmulator: hdr.FPEmulator
        ${CP} hdr.FPEmulator $@ ${CPFLAGS}

# Dynamic dependencies:

