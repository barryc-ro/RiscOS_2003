# Makefile for FilterRes module.
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
#
# Date		Version	Author	Description
# ----		-------	------	-----------
# 04/Oct/2001	0.01	SAR	Created.
#

#
# Paths:
#
EXP_HDR	= <Export$Dir>

#
# Generic options:
#
MKDIR	 = mkdir -p
AS	 = objasm
LD	 = link
PL       = perl
CP	 = copy
RM	 = remove
WIPE	 = -wipe
MOD      = do modgen
SQUEEZE	 = modsqz -v
CCFLAGS	 = -c -depend !Depend -IC:
ASFLAGS	 = -depend !Depend ${THROWBACK} -o ${OBJECT}
LDFLAGS	 = -bin -o ${TARGET}
CPFLAGS	 = ~cfr~v
WFLAGS	 = ~cfr~v
CREATE   = create
TYPE     = settype
CAT      = print

#
# Program specific options:
#
COMPONENT  = FilterRes
SOURCE	   = Hdr.GetAll
TARGET	   = rm.${COMPONENT}
OBJECT	   = o.${COMPONENT}
VERSIONVAR = FilterRes$Version
VERSION    = <FilterRes$Version>
PLSCRIPT   = Build:GetVersion
INMSG      = rm.Messages
OUTMSG     = Resources.FilterMgr.Messages

#
# Generic rules:
#

all: ${TARGET}
	@echo ${COMPONENT}: disc module built

install: ${TARGET}
	${MKDIR} ${INSTDIR}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: disc module installed

rom: ${TARGET}
	@echo ${COMPONENT}: rom module built

ram: ${TARGET}
	${SQUEEZE} ${TARGET}
	@echo ${COMPONENT}: ram module built

install_rom: ${TARGET}
	${MKDIR} ${INSTDIR}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${RM} ${TARGET}
	${WIPE} o ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	@echo ${COMPONENT}: cleaned

dirs:
	${MKDIR} rm

${TARGET}: versionvar dirs
        ${CREATE} ${INMSG}
        ${CAT} ^.^.^.^.Desktop.Filter.Resources.${Locale}.CmdHelp  { >> ${INMSG} }
        ${CAT} ^.^.^.^.Desktop.Filter.Resources.${Locale}.Messages { >> ${INMSG} }
        ${TYPE} ${INMSG} Text
	${MOD} ${TARGET} ${COMPONENT} ${COMPONENT} ${VERSION} ${INMSG} ${OUTMSG}
	Access $@ rw/r

versionvar:
	${PL} ${PLSCRIPT} ${VERSIONVAR}

# Dynamic dependencies:
