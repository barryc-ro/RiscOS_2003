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
# 23-Oct-98  BJGA    Changed to use objasm, so that we can override the
#                    choice of options file using an argument to amu
# 03-Aug-99  BJGA    No longer produces build errors if some resources are absent
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
LD      = link
CP      = copy
RM      = remove
WIPE    = -wipe
ASFLAGS = -NOWarn -depend !Depend ${THROWBACK} ${OPTS} -cpu 3 -o $@
LDFLAGS = -rmf -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = Wimp
SOURCE    = s.Wimp
OBJECT    = o.Wimp
TARGET    = rm.Wimp
OPTS      = -PreDefine "Options SETS \"${OPTIONS}\""
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
        ${WIPE} o ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        IfThere LocalRes:<UserIF>.Messages  Then ${CP} LocalRes:<UserIF>.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS} Else IfThere LocalRes:Messages Then ${CP} LocalRes:Messages ${RESDIR}.${COMPONENT}.Messages ${CPFLAGS}
        IfThere LocalRes:<UserIF>.Templates Then ${CP} LocalRes:<UserIF>.Templates ${RESDIR}.${COMPONENT}.Templates ${CPFLAGS}
        IfThere LocalRes:<UserIF>.Sprites   Then ${CP} LocalRes:<UserIF>.Sprites   ${RESDIR}.${COMPONENT}.Sprites   ${CPFLAGS}
        IfThere LocalRes:<UserIF>.Sprites22 Then ${CP} LocalRes:<UserIF>.Sprites22 ${RESDIR}.${COMPONENT}.Sprites22 ${CPFLAGS}
        IfThere LocalRes:<UserIF>.TileN     Then ${CP} LocalRes:<UserIF>.TileN     ${RESDIR}.${COMPONENT}.TileN     ${CPFLAGS}
        IfThere LocalRes:<UserIF>.TileV     Then ${CP} LocalRes:<UserIF>.TileV     ${RESDIR}.${COMPONENT}.TileV     ${CPFLAGS}
        IfThere LocalRes:<UserIF>.Tools     Then ${CP} LocalRes:<UserIF>.Tools     ${RESDIR}.${COMPONENT}.Tools     ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	IfThere LocalRes:CmdHelp Then Print LocalRes:CmdHelp { >> ${RESDIR}.${COMPONENT}.Messages }

BBETYPE = wimp
bbe-wimp: bbe-generic-resources-get-alias
        BBE_Export_File_In_Dir Resources.${LOCALE} Messages
        BBE_Export_File_In_Dir Resources.${LOCALE} CmdHelp
        BBE_Export_Dir Resources.${LOCALE}.${USERIF}

${OBJECT}: ${SOURCE}
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
        ${LD} ${LDFLAGS} ${OBJECT}
        Access $@ rw/r

${EXP_HDR}.Wimp: hdr.Wimp
        ${CP} hdr.Wimp $@ ${CPFLAGS}

${EXP_HDR}.WimpSpace: hdr.WimpSpace
        ${CP} hdr.WimpSpace $@ ${CPFLAGS}

# Dynamic dependencies:
