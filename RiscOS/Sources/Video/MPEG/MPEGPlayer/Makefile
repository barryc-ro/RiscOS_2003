# Makefile for M-Code21
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 13-Feb-95  TMD	  Created (with help from JRH)
# 11-Apr-95  TMD	  Updated to include DefaultUI, icons
# 05-May-95  TMD	  Added RelNote to installed files
# 11-Oct-95  TMD	  Added stuff to make MPEGRes module
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
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v

#
# Export Paths for Messages module
#
RESAPP = <resource$dir>.Apps.${APP}

#
# Program specific options:
#
COMPONENT = MPEGPlayer
APP       = !M-Code21
RDIR	= Resources
SOURCE	= s.main
TARGET	= rm.MPEGRes

RESFILES = \
  ${RDIR}.!RunImage	\
  ${RDIR}.DefaultUI	\
  ${RDIR}.icons


#
# Generic rules:
#

all: ${RESFILES} ${RDIR}.!Run ${RDIR}.SprExt080 Doc.RelNote ${TARGET}
	@echo ${COMPONENT}: built

${TARGET}: ${SOURCE} ${RESFILES}
	${AS} ${ASFLAGS} ${SOURCE} ${TARGET}

install: ${RDIR}.!Run ${RDIR}.!RunImage ${RDIR}.DefaultUI ${RDIR}.icons ${RDIR}.SprExt080 ${TARGET}
        ${CP} ${RDIR}.!Run      ${INSTDIR}.!Run      ${CPFLAGS}
        ${CP} ${RDIR}.!RunImage ${INSTDIR}.!RunImage ${CPFLAGS}
        ${CP} ${RDIR}.DefaultUI ${INSTDIR}.DefaultUI ${CPFLAGS}
        ${CP} ${RDIR}.icons     ${INSTDIR}.icons     ${CPFLAGS}
        ${CP} ${RDIR}.SprExt080 ${INSTDIR}.SprExt080 ${CPFLAGS}
        ${CP} Doc.RelNote       ${INSTDIR}.^.RelNote ${CPFLAGS}
        ${CP} ${TARGET}		${INSTDIR}.MPEGRes   ${CPFLAGS}
        SetType ${INSTDIR}.SprExt080 Module
	@echo ${COMPONENT}: installed

resources: ${RESFILES} ${RDIR}.!RunROM
        ${MKDIR} ${RESAPP}
        ${CP} ${RDIR}.!RunROM   ${RESAPP}.!Run      ${CPFLAGS}
        ${CP} ${RDIR}.!RunImage ${RESAPP}.!RunImage ${CPFLAGS}
        ${CP} ${RDIR}.DefaultUI ${RESAPP}.DefaultUI ${CPFLAGS}
        ${CP} ${RDIR}.icons     ${RESAPP}.icons     ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

# Dynamic dependencies:
rm.MPEGRes: Hdr:ListOpts
rm.MPEGRes: Hdr:Macros
rm.MPEGRes: Hdr:System
rm.MPEGRes: Hdr:SWIs
rm.MPEGRes: Hdr:CPU.Generic26
rm.MPEGRes: Hdr:IO.GenericIO
rm.MPEGRes: Hdr:RISCOS
rm.MPEGRes: Hdr:Machine.<Machine>
rm.MPEGRes: Hdr:Services
rm.MPEGRes: Hdr:Proc
rm.MPEGRes: Hdr:ResourceFS
rm.MPEGRes: s.version
