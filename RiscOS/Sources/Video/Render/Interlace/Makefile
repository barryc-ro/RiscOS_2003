# Makefile for Interlace
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Sep-94  JRH          Created.
# 14-Feb-95  TMD          Re-integrated versions
# 21-Feb-95  TMD	  Made modes 50,51,55 in a single binary
# 17-Mar-95  JRH	  Stopped all target from doing an install
# 17-Oct-95  TMD	  Removed obsolete variants 't','u','v' from disc build
# 18-Jul-96  TMD	  Added 'y' variant for STB1 MPEG2
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
COMPONENT = Interlace
ROMTARGET =	rm.${MACHINE}.Interlace

DISCTARGETS =	rm.Interls \
		rm.Interlw \
		rm.Interlx \
		rm.Interly

INSTROMTARGET = ${INSTDIR}.${COMPONENT}

INSTALLTARGETS = ${INSTDIR}.Interls \
		 ${INSTDIR}.Interlw \
		 ${INSTDIR}.Interlx \
		 ${INSTDIR}.Interly

.SUFFIXES: .Hdr .rm

#
# Generic rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom modules built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTROMTARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom modules installed

all: ${DISCTARGETS}
	@echo ${COMPONENT}: disc modules built

install: ${INSTALLTARGETS}
	@echo ${COMPONENT}: disc modules installed

${INSTDIR}.Interls: rm.Interls
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interlt: rm.Interlt
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interlu: rm.Interlu
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interlv: rm.Interlv
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interlw: rm.Interlw
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interly: rm.Interly
	${CP} $? $@ ${CPFLAGS}


#
# ROM variants
#
rm.Omega.Interlace: rm.Interlx
        ${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}

rm.Falcon.Interlace: rm.Interlw
        ${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}

rm.Peregrine.Interlace: rm.Interly
	${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}
	
.Hdr.rm:; ${AS} ${ASFLAGS} -From $< -To $@

# Dynamic dependencies:
rm.Interlt: Hdr.GetAll
rm.Interlt: Hdr:ListOpts
rm.Interlt: Hdr:Macros
rm.Interlt: Hdr:System
rm.Interlt: Hdr:SWIs
rm.Interlt: Hdr:CPU.Generic26
rm.Interlt: Hdr:IO.GenericIO
rm.Interlt: Hdr:RISCOS
rm.Interlt: Hdr:Machine.<Machine>
rm.Interlt: Hdr:Machine.<Machine>
rm.Interlt: Hdr:IO.IOMDL
rm.Interlt: Hdr:PublicWS
rm.Interlt: Hdr.GrubbyWS
rm.Interlt: Hdr:CMOS
rm.Interlt: Hdr:ModHand
rm.Interlt: Hdr:Services
rm.Interlt: Hdr:Proc
rm.Interlt: Hdr:FSNumbers
rm.Interlt: Hdr:NewErrors
rm.Interlt: Hdr:MsgTrans
rm.Interlt: Hdr:Debug
rm.Interlt: Hdr:VduExt
rm.Interlt: Hdr:Podule
rm.Interlt: Hdr:PoduleReg
rm.Interlt: Hdr:MPEGCard
rm.Interlt: Hdr:MEMM.ARM600
rm.Interlt: Hdr.Modes
rm.Interlt: s.Common
rm.Interlu: Hdr.GetAll
rm.Interlu: Hdr:ListOpts
rm.Interlu: Hdr:Macros
rm.Interlu: Hdr:System
rm.Interlu: Hdr:SWIs
rm.Interlu: Hdr:CPU.Generic26
rm.Interlu: Hdr:IO.GenericIO
rm.Interlu: Hdr:RISCOS
rm.Interlu: Hdr:Machine.<Machine>
rm.Interlu: Hdr:Machine.<Machine>
rm.Interlu: Hdr:IO.IOMDL
rm.Interlu: Hdr:PublicWS
rm.Interlu: Hdr.GrubbyWS
rm.Interlu: Hdr:CMOS
rm.Interlu: Hdr:ModHand
rm.Interlu: Hdr:Services
rm.Interlu: Hdr:Proc
rm.Interlu: Hdr:FSNumbers
rm.Interlu: Hdr:NewErrors
rm.Interlu: Hdr:MsgTrans
rm.Interlu: Hdr:Debug
rm.Interlu: Hdr:VduExt
rm.Interlu: Hdr:Podule
rm.Interlu: Hdr:PoduleReg
rm.Interlu: Hdr:MPEGCard
rm.Interlu: Hdr:MEMM.ARM600
rm.Interlu: Hdr.Modes
rm.Interlu: s.Common
rm.Interlv: Hdr.GetAll
rm.Interlv: Hdr:ListOpts
rm.Interlv: Hdr:Macros
rm.Interlv: Hdr:System
rm.Interlv: Hdr:SWIs
rm.Interlv: Hdr:CPU.Generic26
rm.Interlv: Hdr:IO.GenericIO
rm.Interlv: Hdr:RISCOS
rm.Interlv: Hdr:Machine.<Machine>
rm.Interlv: Hdr:Machine.<Machine>
rm.Interlv: Hdr:IO.IOMDL
rm.Interlv: Hdr:PublicWS
rm.Interlv: Hdr.GrubbyWS
rm.Interlv: Hdr:CMOS
rm.Interlv: Hdr:ModHand
rm.Interlv: Hdr:Services
rm.Interlv: Hdr:Proc
rm.Interlv: Hdr:FSNumbers
rm.Interlv: Hdr:NewErrors
rm.Interlv: Hdr:MsgTrans
rm.Interlv: Hdr:Debug
rm.Interlv: Hdr:VduExt
rm.Interlv: Hdr:Podule
rm.Interlv: Hdr:PoduleReg
rm.Interlv: Hdr:MPEGCard
rm.Interlv: Hdr:MEMM.ARM600
rm.Interlv: Hdr.Modes
rm.Interlv: s.Common
rm.Interls: Hdr.GetAll
rm.Interls: Hdr:ListOpts
rm.Interls: Hdr:Macros
rm.Interls: Hdr:System
rm.Interls: Hdr:SWIs
rm.Interls: Hdr:CPU.Generic26
rm.Interls: Hdr:IO.GenericIO
rm.Interls: Hdr:RISCOS
rm.Interls: Hdr:Machine.<Machine>
rm.Interls: Hdr:Machine.<Machine>
rm.Interls: Hdr:IO.IOMDL
rm.Interls: Hdr:PublicWS
rm.Interls: Hdr.GrubbyWS
rm.Interls: Hdr:CMOS
rm.Interls: Hdr:ModHand
rm.Interls: Hdr:Services
rm.Interls: Hdr:Proc
rm.Interls: Hdr:FSNumbers
rm.Interls: Hdr:NewErrors
rm.Interls: Hdr:MsgTrans
rm.Interls: Hdr:Debug
rm.Interls: Hdr:VduExt
rm.Interls: Hdr:Podule
rm.Interls: Hdr:PoduleReg
rm.Interls: Hdr:MEMM.ARM600
rm.Interls: Hdr.Modes
rm.Interls: s.Common
rm.Interlw: Hdr.GetAll
rm.Interlw: Hdr:ListOpts
rm.Interlw: Hdr:Macros
rm.Interlw: Hdr:System
rm.Interlw: Hdr:SWIs
rm.Interlw: Hdr:CPU.Generic26
rm.Interlw: Hdr:IO.GenericIO
rm.Interlw: Hdr:RISCOS
rm.Interlw: Hdr:Machine.<Machine>
rm.Interlw: Hdr:Machine.<Machine>
rm.Interlw: Hdr:IO.IOMDL
rm.Interlw: Hdr:PublicWS
rm.Interlw: Hdr.GrubbyWS
rm.Interlw: Hdr:CMOS
rm.Interlw: Hdr:ModHand
rm.Interlw: Hdr:Services
rm.Interlw: Hdr:Proc
rm.Interlw: Hdr:FSNumbers
rm.Interlw: Hdr:NewErrors
rm.Interlw: Hdr:MsgTrans
rm.Interlw: Hdr:Debug
rm.Interlw: Hdr:VduExt
rm.Interlw: Hdr:Podule
rm.Interlw: Hdr:PoduleReg
rm.Interlw: Hdr:MPEGCard
rm.Interlw: Hdr:MEMM.ARM600
rm.Interlw: Hdr.Modes
rm.Interlw: s.Common
rm.Interlx: Hdr.GetAll
rm.Interlx: Hdr:ListOpts
rm.Interlx: Hdr:Macros
rm.Interlx: Hdr:System
rm.Interlx: Hdr:SWIs
rm.Interlx: Hdr:CPU.Generic26
rm.Interlx: Hdr:IO.GenericIO
rm.Interlx: Hdr:RISCOS
rm.Interlx: Hdr:Machine.<Machine>
rm.Interlx: Hdr:Machine.<Machine>
rm.Interlx: Hdr:IO.IOMDL
rm.Interlx: Hdr:PublicWS
rm.Interlx: Hdr.GrubbyWS
rm.Interlx: Hdr:CMOS
rm.Interlx: Hdr:ModHand
rm.Interlx: Hdr:Services
rm.Interlx: Hdr:Proc
rm.Interlx: Hdr:FSNumbers
rm.Interlx: Hdr:NewErrors
rm.Interlx: Hdr:MsgTrans
rm.Interlx: Hdr:Debug
rm.Interlx: Hdr:VduExt
rm.Interlx: Hdr:Podule
rm.Interlx: Hdr:PoduleReg
rm.Interlx: Hdr:MPEGCard
rm.Interlx: Hdr:MEMM.ARM600
rm.Interlx: Hdr.Modes
rm.Interlx: s.Common
rm.Interly: Hdr.GetAll
rm.Interly: Hdr:ListOpts
rm.Interly: Hdr:Macros
rm.Interly: Hdr:System
rm.Interly: Hdr:SWIs
rm.Interly: Hdr:CPU.Generic26
rm.Interly: Hdr:IO.GenericIO
rm.Interly: Hdr:RISCOS
rm.Interly: Hdr:Machine.<Machine>
rm.Interly: Hdr:Machine.<Machine>
rm.Interly: Hdr:IO.IOMDL
rm.Interly: Hdr:PublicWS
rm.Interly: Hdr.GrubbyWS
rm.Interly: Hdr:CMOS
rm.Interly: Hdr:ModHand
rm.Interly: Hdr:Services
rm.Interly: Hdr:Proc
rm.Interly: Hdr:FSNumbers
rm.Interly: Hdr:NewErrors
rm.Interly: Hdr:MsgTrans
rm.Interly: Hdr:Debug
rm.Interly: Hdr:VduExt
rm.Interly: Hdr:Podule
rm.Interly: Hdr:PoduleReg
rm.Interly: Hdr:MPEG2Card
rm.Interly: Hdr:MEMM.ARM600
rm.Interly: Hdr.Modes
rm.Interly: s.Common
