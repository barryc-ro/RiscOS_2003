# Makefile for MPEG exports
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Sep-94  JRH          Created.
# 01-Aug-95  JRH          Changed export directory
# 02-Aug-95  TMD	  Added Hdr:VSP
#

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2

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
COMPONENT = MPEGHdrs
EXPORTS   = ${EXP_HDR}.AV110 \
            ${EXP_HDR}.CL450 \
            ${EXP_HDR}.MPEGAudio \
            ${EXP_HDR}.MPEGCard \
            ${EXP_HDR}.MPEGCtrl \
            ${EXP_HDR}.MPEGVideo \
            ${EXP_HDR}.L64002 \
            ${EXP_HDR}.MPEG2Card \
            ${EXP_HDR}.DVBControl \
            ${EXP_HDR}.VSP

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        @echo ${COMPONENT}: cleaned

#
# Exported interface headers
#
${EXP_HDR}.AV110: hdr.AV110
        ${CP} hdr.AV110 $@ ${CPFLAGS}

${EXP_HDR}.CL450: hdr.CL450
        ${CP} hdr.CL450 $@ ${CPFLAGS}

${EXP_HDR}.MPEGAudio: hdr.MPEGAudio
        ${CP} hdr.MPEGAudio $@ ${CPFLAGS}

${EXP_HDR}.MPEGCard: hdr.MPEGCard
        ${CP} hdr.MPEGCard $@ ${CPFLAGS}

${EXP_HDR}.MPEGCtrl: hdr.MPEGCtrl
        ${CP} hdr.MPEGCtrl $@ ${CPFLAGS}

${EXP_HDR}.MPEGVideo: hdr.MPEGVideo
        ${CP} hdr.MPEGVideo $@ ${CPFLAGS}

${EXP_HDR}.L64002: hdr.L64002
        ${CP} hdr.L64002 $@ ${CPFLAGS}

${EXP_HDR}.MPEG2Card: hdr.MPEG2Card
        ${CP} hdr.MPEG2Card $@ ${CPFLAGS}

${EXP_HDR}.DVBControl: hdr.DVBControl
        ${CP} hdr.DVBControl $@ ${CPFLAGS}

${EXP_HDR}.VSP: hdr.VSP
	${CP} hdr.VSP $@ ${CPFLAGS}


# Dynamic dependencies:
