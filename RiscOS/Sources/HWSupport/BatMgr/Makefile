# Makefile for Battery Manager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09-Aug-94  AMcC         Created.
# 03-Nov-94  AMcC         Updated resources rule

#
# Program specific options:
#
COMPONENT = BatMgr
APP       = !BatMgr
SOURCE    = s.BatMgr
TARGET    = rm.BatMgr
TARGETSA  = rm.BatMgrSA
RDIR      = Resources
LDIR      = ${RDIR}.${LOCALE}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}
RESAPP = <resource$dir>.Apps.${APP}

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

RESOURCES = \
 ${LDIR}.Messages \
 ${LDIR}.Templates \
 ${LDIR}.!Run \
 ${RDIR}.!Sprites \
 ${RDIR}.Sprites

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
        ${RM} ${TARGETSA}
        @echo ${COMPONENT}: cleaned

# Note: !Run and !Sprites NOT in Messages module (only required on portables)
resources:
        ${MKDIR} ${RESDIR}
        ${CP} ${LDIR}.Messages  ${RESDIR}.Messages  ${CPFLAGS}
        ${CP} ${LDIR}.Templates ${RESDIR}.Templates ${CPFLAGS}
        ${CP} ${RDIR}.Sprites   ${RESDIR}.Sprites   ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

#
# Makes a stand-alone version with messages bound in
#
standalone: ${TARGETSA}
        @echo ${COMPONENT}: standalone module built

${TARGETSA}: s.StandAlone ${RESOURCES}
        ${AS} ${ASFLAGS} s.StandAlone

# Dynamic dependencies:
