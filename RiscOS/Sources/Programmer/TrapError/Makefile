# Makefile for TrapError
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
#
# Date		Version	Author	Description
# ----		-------	------	-----------
# 13-Apr2001    2001	SAR	Created.
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
WIPE	= wipe
SQUEEZE	= modsqz -v
CCFLAGS	= -c -depend !Depend -IC:
ASFLAGS	= -depend !Depend ${THROWBACK} -o ${OBJECT}
LDFLAGS	= -bin -o ${TARGET}
CPFLAGS	= ~cfr~v
WFLAGS	= ~cfr~v

#
# Program specific options:
#
COMPONENT = TrapError
SOURCE	  = Hdr.GetAll
TARGET	  = rm.TrapError
OBJECT	  = o.TrapError

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
	IfThere o  Then ${WIPE} o ${WFLAGS}
	IfThere rm Then ${WIPE} rm ${WFLAGS}
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
