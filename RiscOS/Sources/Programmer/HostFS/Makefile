# Makefile for TML_HostFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 26-May-94  AMcC         Created.
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = TML_HostFS
SOURCE    = s.${COMPONENT}
TARGET    = rm.${COMPONENT}
EXPORTS   = ${EXP_HDR}.HostFS \
            ${EXP_HDR}.HostDebug

#
# Generic rules:
#
all: DumpFile SpewChars ${TARGET}
        @echo ${COMPONENT}: all complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

clean:
        ${RM} ${TARGET}
        ${RM} DumpFile
        ${RM} SpewChars
        @echo ${COMPONENT}: cleaned

DumpFile: s.DumpFile
        ${AS} ${ASFLAGS} -To $@ -From s.DumpFile
        SetType $@ Absolute

SpewChars: s.SpewChars
        ${AS} ${ASFLAGS} -To $@ -From s.SpewChars
        SetType $@ Absolute

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} -module -To $@ -From ${SOURCE}

${EXP_HDR}.HostFS: hdr.HostFS
        ${CP} hdr.HostFS $@ ${CPFLAGS}

${EXP_HDR}.HostDebug: hdr.HostDebug
        ${CP} hdr.HostDebug $@ ${CPFLAGS}

# Dynamic dependencies:
