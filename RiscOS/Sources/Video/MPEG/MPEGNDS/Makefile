# Makefile for MPEG NDS
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
#
# Date		Version	Author	Description
# ----		-------	------	-----------
# 27 Jun 2001	1.00	SAR	Created.
#

#
# Paths:
#
EXP_HDR	= <Export$Dir>

#
# Generic options:
#
MKDIR	= mkdir -p
AS	= objasm
LD	= link
CP	= copy
RM	= remove
WIPE	= -wipe
SQUEEZE	= modsqz -v
CCFLAGS	= -c -depend !Depend -IC:
ASFLAGS	= -depend !Depend ${THROWBACK} -o ${OBJECT}
LDFLAGS	= -bin -o ${TARGET}
CPFLAGS	= ~cfr~v
WFLAGS	= ~cfr~v

#
# Program specific options:
#
COMPONENT = MPEGNDS
SOURCE	  = Hdr.GetAll
TARGET	  = rm.${COMPONENT}
OBJECT	  = o.${COMPONENT}

#
# Generic rules:
#

all: ${TARGET}
	@echo ${COMPONENT}: disc module built

install: ${TARGET} dirs
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: disc module installed

rom: ${TARGET} dirs
	@echo ${COMPONENT}: rom module built

ram: ${TARGET} dirs
	${SQUEEZE} ${TARGET}
	@echo ${COMPONENT}: ram module built

install_rom: ${TARGET}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${RM} ${TARGET}
	${WIPE} o ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	@echo ${COMPONENT}: cleaned

dirs:
	${MKDIR} rm

${TARGET}: ${OBJECT}
	${LD} ${LDFLAGS} ${OBJECT}
	SetType $@ Module
	Access $@ rw/r

${OBJECT}: ${SOURCE}
	${MKDIR} o
	${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
o.MPEGNDS: Hdr.GetAll Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:ModHand Hdr:Proc Hdr:MsgTrans Hdr:Services Hdr:MPEGCtrl VersionASM s.Debug s.DataStruct s.Module s.SWIs s.Descramble s.Support
