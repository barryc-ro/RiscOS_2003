# Project:   ROMinfo

#
# Generic options:
#
MKDIR   = mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
ROMinfoRESS = squash
STRIP   = stripdepnd

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
WRAPPER   = s.ModuleWrap

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Include files
#
INCLUDES = -IC:

#
# Program specific options:
#
COMPONENT   = ROMinfo
APPLICATION = ROMinfo
TARGET      = ROMinfo
LIBDIR	    = <Lib$Dir>
LIBINSTDIR  = ${INSTDIR}.Library.Build
INSTAPP	    = ${INSTDIR}.Apps.!ROMInfo
OBJS        = o.ROMinfo

#
# Build for application:
#
ROMinfo:	${OBJS} ${CLIB} dirs
		${LD} -o $@ ${OBJS} ${CLIB}

dirs:
        ${MKDIR} o

#
# build an application:
#
all:	${APPLICATION}
	@echo ${APPLICATION} built

clean:
        ${RM} ${TARGET}
        ${WIPE} ${OBJS} ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${STRIP}

install_tool: ${TARGET}
	${CP} ${TARGET} ${LIBINSTDIR}.${TARGET} ${CPFLAGS}
	@echo ${COMPONENT}: tool and docs installed in library

install: install_tool install_dirs
	${CP} LocalRes:!Boot ${INSTAPP}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${INSTAPP}.!Run ${CPFLAGS}
	${CP} LocalRes:!Help ${INSTAPP}.!Help ${CPFLAGS}
	${CP} LocalRes:!Sprites ${INSTAPP}.!Sprites ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTAPP}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Messages ${INSTAPP}.Messages ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTAPP}.Templates ${CPFLAGS}
	${CC} -C++ -E >${INSTAPP}.Desc LocalRes:Desc
	@echo ${COMPONENT}: FrontEnd app install in '${INSTAPP}'


install_dirs:
	${MKDIR} ${INSTAPP}
	${MKDIR} ${LIBINSTDIR}
	${MKDIR} ${LIBDIR}.Docs

# Dynamic dependencies:
