#
# makefile for the Bush TV demo
#

include Makefiles:StdTools

ROM_MODULE = rm.${COMPONENT}
VIAFILE    = filelist

rom: ${ROM_MODULE}
	@echo ${COMPONENT}: rom module built

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${TARGET} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${WIPE} rm ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${ROM_MODULE}:
	${MKDIR} rm
	perl ${GETVERSION} ${COMPONENT}$$Version ${COMPONENT}$$Full ${COMPONENT}$$Date
	do ${MODGEN} -date "<${COMPONENT}$$Date>" ${ROM_MODULE} ${COMPONENT} ${COMPONENT} <${COMPONENT}$$Version> -via ${VIAFILE}
