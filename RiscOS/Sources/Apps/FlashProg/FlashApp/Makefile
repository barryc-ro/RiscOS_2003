# Makefile for !Flash App Module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-Oct-96  JRH          Created.
#

#
# Generic options:
#
AS       = aasm
CP       = copy
MKDIR    = cdir
MODGEN   = modgen
MODSQZ   = modsqz
RM       = remove
SCANRES  = scanres
WIPE     = -wipe
MAKE     = amu

CFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
TOUCH   = create
WFLAGS  = ~cr~v

#
# Program specific options:
#
COMPONENT   = FlashApp
TARGET      = rm.FlashApp
MODULE_HELP = "FlashApp"
MODULE_VER  = 0.11
SOURCE      = s.Messages
RDIR        = Resources
LDIR        = ${RDIR}.${LOCALE}

#
# Generic rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: module built

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: List
        ${MODGEN} ${TARGET} ${COMPONENT} ${MODULE_HELP} ${MODULE_VER} -via List
	${MODSQZ} ${TARGET}

# Dynamic dependencies:
