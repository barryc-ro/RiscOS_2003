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

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ram module installed

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	TokenCheck LocalRes:CmdHelp
	print LocalRes:CmdHelp { >> ${RESDIR}.${COMPONENT}.Messages }

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

CheckIt: s.CheckIt
        ${AS} ${ASFLAGS} s.CheckIt
        SetType CheckIt Utility

# Dynamic dependencies:
