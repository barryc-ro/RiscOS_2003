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
ASFLAGS = -ABSolute -NOWarn -depend !Depend ${THROWBACK} ${OPTS} -o $@
LDFLAGS = -bin -o $@
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
        ${CP} LocalRes:<UserIF>.Messages ${RESDIR}.${COMPONENT}.Messages ${CPFLAGS}
        ${CP} LocalRes:<UserIF>.Templates ${RESDIR}.${COMPONENT}.Templates ${CPFLAGS}
        ${CP} LocalRes:<UserIF>.Sprites ${RESDIR}.${COMPONENT}.Sprites ${CPFLAGS}
       -${CP} LocalRes:<UserIF>.Sprites22 ${RESDIR}.${COMPONENT}.Sprites22 ${CPFLAGS}
       -${CP} LocalRes:<UserIF>.TileN ${RESDIR}.${COMPONENT}.TileN ${CPFLAGS}
       -${CP} LocalRes:<UserIF>.TileV ${RESDIR}.${COMPONENT}.TileV ${CPFLAGS}
        ${CP} LocalRes:<UserIF>.Tools ${RESDIR}.${COMPONENT}.Tools ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	print LocalRes:<UserIF>.CmdHelp { >> ${RESDIR}.${COMPONENT}.Messages }

${OBJECT}: ${SOURCE}
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ Module
        Access $@ rw/r

${EXP_HDR}.Wimp: hdr.Wimp
        ${CP} hdr.Wimp $@ ${CPFLAGS}

${EXP_HDR}.WimpSpace: hdr.WimpSpace
        ${CP} hdr.WimpSpace $@ ${CPFLAGS}

# Dynamic dependencies:
