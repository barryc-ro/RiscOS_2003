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
C_EXP_HDR = <lib$dir>.MPEG

#
# Generic options:
#
MKDIR   = mkdir -p
AS      = objasm
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
PLFLAGS = -swi
WIPE    = -wipe
WFLAGS  = FR~C~V
CAT     = print
LIBFILE = libfile

AFLAGS  = -depend !Depend $(THROWBACK)
LIBFILEFLAGS = -c -o $@

PERL    = perl Build:Hdr2H ${PLFLAGS}

#
# Rule patterns:
#

.SUFFIXES: .o .s
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

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
            ${EXP_HDR}.MPEG2bCard \
            ${EXP_HDR}.MPEG2cCard \
            ${EXP_HDR}.DVBControl \
            ${EXP_HDR}.VSP \
            ${EXP_HDR}.Macrovisio \
            ${EXP_HDR}.UpCallMPEG \
            ${C_EXP_HDR}.h.AV110 \
            ${C_EXP_HDR}.h.CL450 \
            ${C_EXP_HDR}.h.MPEGAudio \
            ${C_EXP_HDR}.h.MPEGCard \
            ${C_EXP_HDR}.h.MPEGCtrl \
            ${C_EXP_HDR}.h.MPEGVideo \
            ${C_EXP_HDR}.h.L64002 \
            ${C_EXP_HDR}.h.MPEG2Card \
            ${C_EXP_HDR}.h.MPEG2bCard \
            ${C_EXP_HDR}.h.MPEG2cCard \
            ${C_EXP_HDR}.h.DVBControl \
            ${C_EXP_HDR}.h.VSP \
            ${C_EXP_HDR}.h.Macrovisio \
            ${C_EXP_HDR}.h.UpCallMPEG \
            ${C_EXP_HDR}.h.TTextSubs \
            ${C_EXP_HDR}.h.mpegtime \

MPEGLIB =   MPEG.o.MPEG
LIBOBJS =   o.mpegtime \

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

export_: export_libs export_hdrs
        @|

export_hdrs: ${EXPORTS} Dirs
        @echo ${COMPONENT}: export hdrs complete

export_libs: ${MPEGLIB} Dirs
        ${CP} ${MPEGLIB} <Lib$Dir>.${MPEGLIB} ${CPFLAGS}
        @echo ${COMPONENT}: export libs complete

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} MPEG ${WFLAGS}
        @echo ${COMPONENT}: cleaned

Dirs:
        ${MKDIR} o
        ${MKDIR} MPEG.h
        ${MKDIR} MPEG.o
        ${MKDIR} <lib$dir>.MPEG.h
        ${MKDIR} <lib$dir>.MPEG.o

${MPEGLIB}: ${LIBOBJS}
        ${LIBFILE} ${LIBFILEFLAGS} ${LIBOBJS}

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

${EXP_HDR}.MPEG2bCard: hdr.MPEG2bCard
        ${CP} hdr.MPEG2bCard $@ ${CPFLAGS}

${EXP_HDR}.MPEG2cCard: hdr.MPEG2cCard
        ${CP} hdr.MPEG2cCard $@ ${CPFLAGS}

${EXP_HDR}.DVBControl: hdr.DVBControl
        ${CP} hdr.DVBControl $@ ${CPFLAGS}

${EXP_HDR}.VSP: hdr.VSP
	${CP} hdr.VSP $@ ${CPFLAGS}

${EXP_HDR}.Macrovisio: Hdr.Macrovisio
	${CP} hdr.Macrovisio $@ ${CPFLAGS}

${EXP_HDR}.UpCallMPEG: Hdr.UpCallMPEG
	${CP} hdr.UpCallMPEG $@ ${CPFLAGS}

${C_EXP_HDR}.h.AV110: hdr.AV110
	${PERL} Hdr.AV110 $@

${C_EXP_HDR}.h.CL450: hdr.CL450
	${PERL} Hdr.CL450 $@

${C_EXP_HDR}.h.MPEGAudio: MPEG.h.MPEGAudio h.MPEGAudio
        ${CP} h.MPEGAudio $@ ${CPFLAGS}
        ${CAT} MPEG.h.MPEGAudio { >> $@ }

MPEG.h.MPEGAudio:
        ${PERL} Hdr.MPEGAudio $@

${C_EXP_HDR}.h.MPEGCard: hdr.MPEGCard
	${PERL} Hdr.MPEGCard $@

${C_EXP_HDR}.h.MPEGCtrl: MPEG.h.MPEGCtrl h.MPEGCtrl
        ${CP} h.MPEGCtrl $@ ${CPFLAGS}
        ${CAT} MPEG.h.MPEGCtrl { >> $@ }

MPEG.h.MPEGCtrl:
        ${PERL} Hdr.MPEGCtrl $@

${C_EXP_HDR}.h.MPEGVideo: MPEG.h.MPEGVideo h.MPEGVideo
        ${CP} h.MPEGVideo $@ ${CPFLAGS}
        ${CAT} MPEG.h.MPEGVideo { >> $@ }

MPEG.h.MPEGVideo:
        ${PERL} Hdr.MPEGVideo $@

${C_EXP_HDR}.h.L64002: hdr.L64002
	${PERL} Hdr.L64002 $@

${C_EXP_HDR}.h.MPEG2Card: hdr.MPEG2Card
	${PERL} Hdr.MPEG2Card $@

${C_EXP_HDR}.h.MPEG2bCard: hdr.MPEG2bCard
	${PERL} Hdr.MPEG2bCard $@

${C_EXP_HDR}.h.MPEG2cCard: hdr.MPEG2cCard
	${PERL} Hdr.MPEG2cCard $@

${C_EXP_HDR}.h.DVBControl: hdr.DVBControl
	${PERL} Hdr.DVBControl $@

${C_EXP_HDR}.h.VSP: hdr.VSP
	${PERL} Hdr.VSP $@

${C_EXP_HDR}.h.Macrovisio: Hdr.Macrovisio
	${PERL} Hdr.Macrovisio $@

${C_EXP_HDR}.h.UpCallMPEG: Hdr.UpCallMPEG
	${PERL} Hdr.UpCallMPEG $@

${C_EXP_HDR}.h.TTextSubs: h.TTextSubs
        ${CP} h.TTextSubs $@ ${CPFLAGS}

${C_EXP_HDR}.h.mpegtime: h.mpegtime
        ${CP} h.mpegtime $@ ${CPFLAGS}


# Dynamic dependencies:
