# Makefile for WimpRes module.
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
#
# Date		Version	Author	Description
# ----		-------	------	-----------
# 07/Sep/2001	1.00	SAR	Created.
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

#
# Program specific options:
#
COMPONENT  = WimpRes
SOURCE	   = Hdr.GetAll
TARGET	   = rm.${COMPONENT}
OBJECT	   = o.${COMPONENT}
VIAFILE    = Resources.ViaFile
VERSIONVAR = WimpRes$Version
VERSION    = <WimpRes$Version>
PLSCRIPT   = Build:GetVersion

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
        Set WimpRes$Path ^.^.^.^.Desktop.Wimp.Resources.<Locale>.,^.^.^.^.Desktop.Wimp.Resources.UK.
	${MOD} ${TARGET}u ${COMPONENT} ${COMPONENT} ${VERSION} -via ${VIAFILE}
	${SQUEEZE} ${TARGET}u ${TARGET}
	Unset WimpRes$Path
	Access $@ rw/r

versionvar:
	${PL} ${PLSCRIPT} ${VERSIONVAR}

# Dynamic dependencies:
