# Makefile for deccf
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 01-Mar-01  SNB          Created
#

# Build option:
# OPTIONS = -DUSE32BIT     switches the default to 32-bit veneers

#
# Paths
#
COMPONENT = cmhg
TARGET    = cmhg

CFLAGS    = -Wpc -ffah -IC:CLX -DDDE -DUSE32BIT ${OPTIONS}
DIRS      = o._dirs

INSTAPP   = ${INSTDIR}.Apps.!CMHG
LIBDIR    = ${INSTDIR}.Library.Acorn

#
# Program specific options:
#
OBJS       = cmhg.o outaof.o throwback.o veneers.o veneers32.o
LIBS       = ${CLXLIB} ${CLIB}

include StdTools
include AppLibs

.SUFFIXES:
.SUFFIXES: .o .s .c
.c.o:; ${CC} ${CFLAGS} -o $@ $<
.s.o:; ${AS} ${ASLAGS} -o $@ $<

#
# build a the library:
#
all:    ${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed
        
install_tool: ${TARGET}
	${MKDIR} ${LIBDIR}.Docs
	${CP} ${TARGET} ${LIBDIR}.${TARGET} ${CPFLAGS}
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
        ${RM} ${COMPONENT}
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
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@
        ${CHMOD} a+rx $@


# Dynamic dependencies:
