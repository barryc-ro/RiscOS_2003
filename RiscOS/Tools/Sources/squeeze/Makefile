# Makefile for squeeze & xpand
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 01-Mar-01  SNB          Created
#

#
# Paths
#
COMPONENT = squeeze
TARGET    = squeeze
TARGET2   = xpand

CFLAGS    = -Wpc -ffah -IC:CLX -DDDE -DDEBUGGING=0
DIRS      = o._dirs

INSTAPP   = ${INSTDIR}.Apps.!Squeeze
LIBDIR    = ${INSTDIR}.Library.Acorn

#
# Program specific options:
#
OBJS       = squeeze.o unsqueeze.o unsqrm.o
XPANDOBJS  = xpand.o asmcall.o
LIBS       = ${CLXLIB} ${CLIB}

include Makefiles:StdTools
include Makefiles:AppLibs
include Makefiles:AppStdRule

#
# build a the library:
#
all:    ${TARGET} ${TARGET2}

#
# RISC OS ROM build rules:
#
rom: ${TARGET} ${TARGET2}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET} ${TARGET2}
        @echo ${COMPONENT}: rom module installed
        
install_tool: ${TARGET} ${TARGET2}
	${MKDIR} ${LIBDIR}.Docs
	${CP} ${TARGET} ${LIBDIR}.${TARGET} ${CPFLAGS}
	${CP} ${TARGET2} ${LIBDIR}.${TARGET2} ${CPFLAGS}
	@echo ${COMPONENT}: tool installed in library

install: install_resources install_tool
	@echo ${COMPONENT}: install complete

install_resources:
	${MKDIR} ${INSTAPP}
	${CP} LocalRes:!Boot ${INSTAPP}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${INSTAPP}.!Run ${CPFLAGS}
	${CP} LocalRes:!Help ${INSTAPP}.!Help ${CPFLAGS}
	${CP} LocalRes:!Sprites ${INSTAPP}.!Sprites ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTAPP}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Messages ${INSTAPP}.Messages ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTAPP}.Templates ${CPFLAGS}
	${CC} -C++ -E >${INSTAPP}.Desc LocalRes:Desc
	${TIDYDESC}  ${INSTAPP}.Desc ${INSTAPP}.Desc
	@echo ${COMPONENT}: FrontEnd app install in '${INSTAPP}'

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${TARGET2}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@
        ${CHMOD} a+rx $@

${TARGET2}: ${XPANDOBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ ${XPANDOBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@
        ${CHMOD} a+rx $@

# Dynamic dependencies:
