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
# 07-Oct-96  TMD	  Added Hdr:Macrovisio
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
            ${EXP_HDR}.VSP \
            ${EXP_HDR}.Macrovisio

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
${EXP_HDR}.AV110: Hdr.AV110
        ${CP} Hdr.AV110 $@ ${CPFLAGS}

${EXP_HDR}.CL450: Hdr.CL450
        ${CP} Hdr.CL450 $@ ${CPFLAGS}

${EXP_HDR}.MPEGAudio: Hdr.MPEGAudio
        ${CP} Hdr.MPEGAudio $@ ${CPFLAGS}

${EXP_HDR}.MPEGCard: Hdr.MPEGCard
        ${CP} Hdr.MPEGCard $@ ${CPFLAGS}

${EXP_HDR}.MPEGCtrl: Hdr.MPEGCtrl
        ${CP} Hdr.MPEGCtrl $@ ${CPFLAGS}

${EXP_HDR}.MPEGVideo: Hdr.MPEGVideo
        ${CP} Hdr.MPEGVideo $@ ${CPFLAGS}

${EXP_HDR}.L64002: Hdr.L64002
        ${CP} Hdr.L64002 $@ ${CPFLAGS}

${EXP_HDR}.MPEG2Card: Hdr.MPEG2Card
        ${CP} Hdr.MPEG2Card $@ ${CPFLAGS}

${EXP_HDR}.DVBControl: Hdr.DVBControl
        ${CP} Hdr.DVBControl $@ ${CPFLAGS}

${EXP_HDR}.VSP: Hdr.VSP
	${CP} Hdr.VSP $@ ${CPFLAGS}

${EXP_HDR}.Macrovisio: Hdr.Macrovisio
	${CP} Hdr.Macrovisio $@ ${CPFLAGS}


# Dynamic dependencies:
