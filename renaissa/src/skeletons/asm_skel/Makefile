# Skeleton ASM Makefile

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
ASFLAGS = -depend !Depend ${THROWBACK} -Stamp -quit -module -Throwback -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = Skeleton
SOURCE     = s.module
TARGET     = rm.${COMPONENT}
EXPORTS    = ${EXP_HDR}.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

export: ${EXPORTS}
        ${CP} Hdr.${COMPONENT} $@ ${CPFLAGS}
        @echo ${COMPONENT}: export complete

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

all: ${TARGET}
        @echo ${COMPONENT}: standalone module built

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.${COMPONENT}: Hdr.${COMPONENT}
        ${CP} Hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.Skeleton: s.getall
rm.Skeleton: Hdr:ListOpts
rm.Skeleton: Hdr:Macros
rm.Skeleton: Hdr:System
rm.Skeleton: Hdr:SWIs
rm.Skeleton: Hdr:CPU.Generic26
rm.Skeleton: Hdr:IO.GenericIO
rm.Skeleton: Hdr:RISCOS
rm.Skeleton: Hdr:ModHand
rm.Skeleton: Hdr:Proc
rm.Skeleton: VersionASM
