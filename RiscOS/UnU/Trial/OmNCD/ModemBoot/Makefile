# Makefile for CMOSreset
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 10-Mar-95  JRH          Created.
# 30-Jan-96  PSW	  Edit for ModemBoot module.
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
CC	  = cc
LINK	  = link
MKDIR     = cdir
AS        = aasm 
CP        = copy
RM        = remove
MODGEN    = modgen
MODSQZ    = modsqz
CCFLAGS   = -c -depend !Depend -IC:
LINKFLAGS = -aif -v -c++ -o $@ 
ASFLAGS   = -depend !Depend -Stamp -quit -module
CPFLAGS   = ~cfr~v
LIBS	  = C:o.stubs

#
# Program specific options:
#
COMPONENT   = BootMod
VERSION     = 0.01
TARGET      = rm.BootMod
MODULE_HELP = "Modem Boot Support"
FILELIST    = BootFiles
FILES	    = o.files

#
# Generic rules:
#
all: rom

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: make_list
	make_list @.Files ${FILELIST}
        ${MODGEN} $@ ${COMPONENT} ${MODULE_HELP} ${VERSION} -via ${FILELIST}
        ${MODSQZ} ${TARGET}
        
${FILELIST} : make_list

# { > Log }

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

#
# Extra Development rules:
#
install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ${TARGET} installed

maketime:
        MakeTime

version:
        Version

make_list: ${FILES}
	${LINK} ${LINKFLAGS} ${FILES} ${LIBS}

o.files: 
	${CC} ${CCFLAGS} -o o.files c.files
	
# Dynamic dependencies:

o.files:	c.files
o.files:	C:h.kernel
