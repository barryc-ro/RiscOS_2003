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
# 13-Nov-98  SMC	  Updated to latest MPEG player
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
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Export Paths for Messages module
#
RESAPP = <resource$dir>.Apps.${APP}

#
# Program specific options:
#
COMPONENT = MPEGPlayer
APP       = !MPEG
RDIR	= Resources
SOURCE	= s.main
TARGET	= rm.MPEGRes

RESFILES = \
  ${RDIR}.!Boot		\
  ${RDIR}.!Help		\
  ${RDIR}.!Run		\
  ${RDIR}.!RunImage	\
  ${RDIR}.!Version	\
  ${RDIR}.DefaultUI	\
  ${RDIR}.icons


#
# Generic rules:
#

all: ${RESFILES}
	@echo ${COMPONENT}: built

${TARGET}: ${SOURCE} ${RESFILES}
	${AS} ${ASFLAGS} ${SOURCE} ${TARGET}

install: ${RESFILES}
        ${MKDIR} ${INSTDIR}
        ${CP} VersionNum         ${INSTDIR}.VersionNum ${CPFLAGS}
        ${CP} ${RDIR}.!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} ${RDIR}.!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} ${RDIR}.!RunImage  ${INSTDIR}.!RunImage  ${CPFLAGS}
        ${CP} ${RDIR}.DefaultUI  ${INSTDIR}.DefaultUI  ${CPFLAGS}
        ${CP} ${RDIR}.icons      ${INSTDIR}.icons      ${CPFLAGS}
        ${CP} ${RDIR}.MPEGError  ${INSTDIR}.MPEGError  ${CPFLAGS}
        ${CP} ${RDIR}.MPEGResume ${INSTDIR}.MPEGResume ${CPFLAGS}
        ${CP} ${RDIR}.NPTResume  ${INSTDIR}.NPTResume  ${CPFLAGS}
        ${CP} ${RDIR}.PlayMPEG   ${INSTDIR}.PlayMPEG   ${CPFLAGS}
        ${CP} ${RDIR}.PlayMPEGF  ${INSTDIR}.PlayMPEGF  ${CPFLAGS}
        ${CP} ${RDIR}.WimpComm   ${INSTDIR}.WimpComm   ${CPFLAGS}
	@echo ${COMPONENT}: installed

resources: ${RESFILES} ${RDIR}.!RunROM
        ${MKDIR} ${RESAPP}
        ${CP} VersionNum         ${RESAPP}.VersionNum ${CPFLAGS}
        ${CP} ${RDIR}.!RunROM    ${RESAPP}.!Run       ${CPFLAGS}
        ${CP} ${RDIR}.!RunImage  ${RESAPP}.!RunImage  ${CPFLAGS}
        ${CP} ${RDIR}.DefaultUI  ${RESAPP}.DefaultUI  ${CPFLAGS}
        ${CP} ${RDIR}.icons      ${RESAPP}.icons      ${CPFLAGS}
        ${CP} ${RDIR}.MPEGError  ${RESAPP}.MPEGError  ${CPFLAGS}
        ${CP} ${RDIR}.MPEGResume ${RESAPP}.MPEGResume ${CPFLAGS}
        ${CP} ${RDIR}.NPTResume  ${RESAPP}.NPTResume  ${CPFLAGS}
        ${CP} ${RDIR}.PlayMPEG   ${RESAPP}.PlayMPEG   ${CPFLAGS}
        ${CP} ${RDIR}.PlayMPEGF  ${RESAPP}.PlayMPEGF  ${CPFLAGS}
        ${CP} ${RDIR}.WimpComm   ${RESAPP}.WimpComm   ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} !MPEG ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Dynamic dependencies:
