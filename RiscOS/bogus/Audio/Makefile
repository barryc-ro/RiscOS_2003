# Makefile for SoundFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
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
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module ${THROWBACK}
CPFLAGS = ~cfr~v
WFLAGS   = ~c~v

#
# Program specific options:
#
COMPONENT = SoundFS
SOURCE    = s.SoundFS
TARGET    = aof.SoundFS
TARGETSA  = rm.SoundFS
EXPORTS   = ${EXP_HDR}.SoundFS

RESOURCES = Resources.UK.Messages

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.SoundFS

OBJS = fs.o txbuffer.o module.o error.o

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@
.cmhg.o:;    ${CMHG} -d @.h.module $< $@

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
        WIPE o.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.Buffer: hdr.Buffer
        ${CP} hdr.Buffer $@ ${CPFLAGS}

#
# Makes a stand-alone version with messages bound in
#
standalone: ${TARGETSA}
        @echo ${COMPONENT}: standalone module built


${TARGETSA}: ${RESOURCES} ${OBJS}
        ${LD} -rmf -o $@ ${OBJS} 


# Dynamic dependencies:
o.fs: s.fs
o.fs: Hdr:ListOpts
o.fs: Hdr:Macros
o.fs: Hdr:System
o.fs: Hdr:SWIs
o.fs: Hdr:CPU.Generic26
o.fs: Hdr:IO.GenericIO
o.fs: Hdr:RISCOS
o.fs: Hdr:ModHand
o.fs: Hdr:HostFS
o.fs: Hdr:NdrDebug
o.fs: Hdr:MsgTrans
o.fs: Hdr:ResourceFS
o.fs: Hdr:DeviceFS
o.fs: Hdr:Buffer
o.fs: a.debuga
o.fs: a.defs+ws
o.fs: a.fs
o.fs: a.swiextra
o.fs: a.error
o.txbuffer: s.txbuffer
o.txbuffer: Hdr:ListOpts
o.txbuffer: Hdr:Macros
o.txbuffer: Hdr:System
o.txbuffer: Hdr:SWIs
o.txbuffer: Hdr:CPU.Generic26
o.txbuffer: Hdr:IO.GenericIO
o.txbuffer: Hdr:RISCOS
o.txbuffer: Hdr:HostFS
o.txbuffer: a.swiextra
o.txbuffer: a.debuga
o.txbuffer: a.defs+ws
o.txbuffer: a.fs
o.module: s.module
o.module: Hdr:ListOpts
o.module: Hdr:Macros
o.module: Hdr:System
o.module: Hdr:SWIs
o.module: Hdr:CPU.Generic26
o.module: Hdr:IO.GenericIO
o.module: Hdr:RISCOS
o.module: Hdr:Services
o.module: Hdr:ModHand
o.module: Hdr:HostFS
o.module: Hdr:DDVMacros
o.module: Hdr:UpCall
o.module: Hdr:MsgTrans
o.module: Hdr:ResourceFS
o.module: Hdr:DeviceFS
o.module: a.debuga
o.module: a.Version
o.module: a.swiextra
o.module: a.defs+ws
o.module: a.fs
o.module: a.ResFiles
o.error: s.error
o.error: Hdr:ListOpts
o.error: Hdr:Macros
o.error: Hdr:System
o.error: Hdr:SWIs
o.error: Hdr:CPU.Generic26
o.error: Hdr:IO.GenericIO
o.error: Hdr:RISCOS
o.error: Hdr:ModHand
o.error: Hdr:HostFS
o.error: Hdr:NdrDebug
o.error: Hdr:MsgTrans
o.error: Hdr:ResourceFS
o.error: Hdr:DeviceFS
o.error: Hdr:Buffer
o.error: Hdr:Proc
o.error: a.defs+ws
o.error: a.error
