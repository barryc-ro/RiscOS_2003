# Project:   ROMinfo

#
# Libraries
#
include Makefiles:AppLibs

#
# Include files
#
CINCLUDES = -IC:
CFLAGS    = -ffa -fussy

#
# Generic options:
#
include StdTools

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<


#
# Program specific options:
#
COMPONENT   = ROMinfo
APPLICATION = ROMinfo
TARGET      = ROMinfo
LIBINSTDIR  = ${INSTDIR}.Library.Build
INSTAPP     = ${INSTDIR}.Apps.!ROMInfo
OBJS        = o.ROMinfo

#
# Build for application:
#
${APPLICATION}: ${OBJS} ${CLIB} dirs
        ${LD} -o $@ ${OBJS} ${CLIB}
        ${SQZ} $@
        

dirs:
        ${MKDIR} o

#
# build an application:
#
all:	${APPLICATION}
	@echo ${APPLICATION}: application built

clean:
        ${RM} ${TARGET}
        ${WIPE} ${OBJS} ${WFLAGS}
        ${WIPE} o ${WFLAGS}
	@echo ${APPLICATION}: cleaned

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

# Dynamic dependencies:
