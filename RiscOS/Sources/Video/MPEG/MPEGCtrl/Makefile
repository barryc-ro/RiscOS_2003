# Makefile for MPEGSystem
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Sep-94  JRH          Created.
# 13-Feb-95  TMD	  Added install option for making !M-Code21.
# 17-Mar-95  TMD	  Delete CP from all target.
# 05-May-95  TMD	  Add Doc.MPEGPlayer to installed list
# 10-Mar-97  TMD	  Add option to make system-only version
# 09-Dec-98  BJGA         Renamed MPEGCtrl to MPEGSystem
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
# Program specific options:
#
COMPONENT = MPEGSystem
SOURCE    = Hdr.GetAll
TARGET    = rm.MPEGSystem

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

all: ${TARGET}
        @echo ${COMPONENT}: disc module built

install: ${TARGET} Doc.MPEGPlayer
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        ${CP} Doc.MPEGPlayer ${INSTDIR}.^.MPEGPlayer ${CPFLAGS}
        @echo ${COMPONENT}: disc module installed

clean:
        ${RM} ${TARGET}
        -wipe rm.SysOnly ~cfr~v
        -stripdepnd
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

sysonly: rm.SysOnly.MPEGSystem
	@echo System-stream only version of MPEGSystem built

rm.SysOnly.MPEGSystem: ${SOURCE} Hdr.SysOnly
        ${MKDIR} rm.SysOnly
	${AS} ${ASFLAGS} -From Hdr.SysOnly -To $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
