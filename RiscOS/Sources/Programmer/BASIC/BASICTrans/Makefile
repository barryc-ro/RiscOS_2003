# Makefile for BASICTrans
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date        Name   	Description
# ----        ----   	-----------
# 08-Jun-94   AMcC   	Created.
# 08-Nov-94   AMcC   	Updated to install BASIC64 (non ROM)
# 27 Jun 1997 RWB	Split two targets into seperate makefiles

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
CD	= dir
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = BASICTrans
EXPORT    = ^.hdr.${COMPONENT}
TARGET    = rm.${COMPONENT}
SOURCE    = TransSrc

#
# Generic rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: module built

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORT} ${EXP_HDR}.${COMPONENT}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
	${RM} ${TARGET}
        ${RM} rm
        @echo ${COMPONENT}: cleaned

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.BASIC
        ${CP} ^.Resources.${LOCALE}.Messages ${RESDIR}.BASIC.Messages ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	print ^.Resources.${LOCALE}.CmdHelp { >> ${RESDIR}.BASIC.Messages }

${TARGET}: ^.${SOURCE}
	${MKDIR} rm
	${CD} ^
        ${AS} ${ASFLAGS} -To ${COMPONENT}.${TARGET} -From ${SOURCE}
        ${CD} ${COMPONENT}

${EXP_HDR}.${COMPONENT}: ${EXPORT}
        ${CP} ${EXPORT} $@ ${CPFLAGS}

${EXPORT}:
        ${CP} $@ ${EXP_HDR}.${COMPONENT} ${CPFLAGS}

# Dynamic dependencies:
BASICTrans.rm.BASICTrans: Hdr:ListOpts
BASICTrans.rm.BASICTrans: Hdr:Macros
BASICTrans.rm.BASICTrans: Hdr:System
BASICTrans.rm.BASICTrans: Hdr:SWIs
BASICTrans.rm.BASICTrans: Hdr:CPU.Generic26
BASICTrans.rm.BASICTrans: Hdr:IO.GenericIO
BASICTrans.rm.BASICTrans: Hdr:RISCOS
BASICTrans.rm.BASICTrans: Hdr:CMOS
BASICTrans.rm.BASICTrans: Hdr:ModHand
BASICTrans.rm.BASICTrans: Hdr:Services
BASICTrans.rm.BASICTrans: Hdr:Proc
BASICTrans.rm.BASICTrans: Hdr:FSNumbers
BASICTrans.rm.BASICTrans: Hdr:NewErrors
BASICTrans.rm.BASICTrans: Hdr:MsgTrans
