# Makefile for FileCore
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
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
ASFLAGS = -throwback -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = FileCore
SOURCE    = s.FileCore
TARGET    = rm.FileCore
EXPORTS   = ${EXP_HDR}.FileCore \
            ${EXP_HDR}.MultiFS

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
        ${RM} s.TokenHelp
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE} s.TokenHelp
        ${AS} ${ASFLAGS} ${SOURCE}

s.TokenHelp: Hdr:Tokens HelpText
        tokenise Hdr:Tokens HelpText $@

${EXP_HDR}.FileCore: hdr.FileCore
        ${CP} hdr.FileCore $@ ${CPFLAGS}

${EXP_HDR}.MultiFS: hdr.MultiFS
        ${CP} hdr.MultiFS $@ ${CPFLAGS}

# Dynamic dependencies:
