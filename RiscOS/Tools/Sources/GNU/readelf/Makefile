# Makefile for GNU readelf (from binutils)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 08-Nov00  SNB          Created.
#

#
# Paths
#
TINSTDIR= ${INSTDIR}.Library.GNU
INSTAPP = ${INSTDIR}.Apps.!ReadELF

#
# Include files
#
CINCLUDES += -Iinclude -Ibfd -IC:TCPIPLibs
CFLAGS    += -W

#
# Generic options:
#
include Makefiles:StdTools
include Makefiles:StdRules

#
# Libraries
#
include Makefiles:AppLibs

DIRS=o._dirs
VERSION=o._version

#
# Program specific options:
#
COMPONENT  = readelf
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 


OBJS       = readelf.o version.o getopt.o getopt1.o

LIBS       = ${CLIB}

#
# build a the library:
#
all:       ${TARGET}
	@echo ${COMPONENT} built.

install_tool: all
        ${MKDIR} ${TINSTDIR}
	${CP} ${TARGET} ${TINSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo Installation of tool ${COMPONENT} complete

install: install_tool
        ${MKDIR} ${INSTAPP}
	${CP} LocalRes:!Boot ${INSTAPP}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${INSTAPP}.!Run ${CPFLAGS}
	${CP} LocalRes:!Help ${INSTAPP}.!Help ${CPFLAGS}
	${CP} LocalRes:!Sprites ${INSTAPP}.!Sprites ${CPFLAGS}
	${CP} LocalRes:Messages ${INSTAPP}.Messages ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTAPP}.Templates ${CPFLAGS}
	${CC} -C++ -E >${INSTAPP}.Desc LocalRes:Desc
	${TIDYDESC}  ${INSTAPP}.Desc ${INSTAPP}.Desc
	@echo ${COMPONENT}: FrontEnd app install in '${INSTAPP}'


#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

${DIRS}:
	${MKDIR} o
	create $@

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@

# Dynamic dependencies:
