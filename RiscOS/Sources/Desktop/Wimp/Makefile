# Makefile for Wimp
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name    Description
# ----       ----    -----------
# 25-May-94  AMcC    Created.
# 18-Oct-94  AMcC    Template3D now copied to Resources as 'Templates'
#                    Now correctly copies Tools3D to Resources:...Tools
# 26-Jun-95  JRH     Now copies STB (ie hacked-down) versions of Tools,
#                    Templates and Sprites. Doesn't copy Sprites22
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
COMPONENT = Wimp
SOURCE    = s.Wimp
TARGET    = rm.Wimp
EXPORTS   = ${EXP_HDR}.Wimp \
            ${EXP_HDR}.WimpSpace

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
        @echo ${COMPONENT}: cleaned

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages ${RESDIR}.${COMPONENT}.Messages ${CPFLAGS}
        ${CP} Resources.${LOCALE}.<System>.Templates ${RESDIR}.${COMPONENT}.Templates ${CPFLAGS}
        ${CP} Resources.${LOCALE}.<System>.Sprites ${RESDIR}.${COMPONENT}.Sprites ${CPFLAGS}
       -${CP} Resources.${LOCALE}.<System>.Sprites22 ${RESDIR}.${COMPONENT}.Sprites22 ${CPFLAGS}
       -${CP} Resources.${LOCALE}.<System>.TileN ${RESDIR}.${COMPONENT}.TileN ${CPFLAGS}
       -${CP} Resources.${LOCALE}.<System>.TileV ${RESDIR}.${COMPONENT}.TileV ${CPFLAGS}
        ${CP} Resources.${LOCALE}.<System>.Tools ${RESDIR}.${COMPONENT}.Tools ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	print Resources.${LOCALE}.CmdHelp { >> ${RESDIR}.${COMPONENT}.Messages }

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.Wimp: hdr.Wimp
        ${CP} hdr.Wimp $@ ${CPFLAGS}

${EXP_HDR}.WimpSpace: hdr.WimpSpace
        ${CP} hdr.WimpSpace $@ ${CPFLAGS}

# Dynamic dependencies:
