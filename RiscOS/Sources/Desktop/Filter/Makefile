# Makefile for Filter Manager
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
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = FilterMgr
SOURCE    = s.FilterMgr
TARGET    = rm.FilterMgr
EXPORTS   = ${EXP_HDR}.Filter

MERGEDRESOURCE = MergedMsgs

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
        ${RM} ${MERGEDRESOURCE}
        @echo ${COMPONENT}: cleaned

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common: ${MERGEDRESOURCE}
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	print Resources.${LOCALE}.CmdHelp { >> ${RESDIR}.${COMPONENT}.Messages }

${MERGEDRESOURCE}: Resources.${LOCALE}.Messages Resources.${LOCALE}.CmdHelp
	${CP} Resources.${LOCALE}.Messages ${MERGEDRESOURCE} ${CPFLAGS}
	print Resources.${LOCALE}.CmdHelp { >> ${MERGEDRESOURCE} }

${TARGET}: ${SOURCE} ${MERGEDRESOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.Filter: hdr.Filter
        ${CP} hdr.Filter $@ ${CPFLAGS}

# Dynamic dependencies:
