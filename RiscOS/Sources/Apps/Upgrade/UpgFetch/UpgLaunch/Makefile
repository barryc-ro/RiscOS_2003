# Makefile for UpgLaunch

#
# Paths
#
EXP_HDR   = <export$dir>
RESDIR    = <Resource$Dir>.Resources.Test

#
# Generic options
#
include Makefiles:StdTools

INCLUDES  = -IC: -ITCPIPLibs:

AFLAGS    = -depend !Depend -Stamp -quit
CFLAGS    = -c -depend !Depend -Wp ${INCLUDES} -D__BORLANDC__=0 -DSYS${SYSTEM}=1 -DMACH${MACHINE}=1 ${DFLAGS} -DCOMPAT_INET4 ${EXT_FLAGS}
CPFLAGS   = ~cfr~v
WFLAGS    = ~c~v

#
# Libraries
#
LIBDIR    = <Lib$Dir>

CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ZLIB      = ${LIBDIR}.zlib.o.zlib

#
# Include files
#

INCLUDES  = -IC: -ITCPIPLibs:

# Program specific options:
#
COMPONENT = UpgLaunch
TARGET    = UpgLaunch
INSTAPP   = ${INSTDIR}.Apps
OM        = o

OBJS      =	${OM}.Dirs		\
		${OM}.Main		\
		${OM}.Utils

.SUFFIXES: .o .s .c .c++ .cmhg

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET}
	@echo ${COMPONENT}: all complete

clean:
	${WIPE}		o.*		${WFLAGS}
	${RM}				${TARGET}
	@echo ${COMPONENT}: cleaned

clean_all: clean
	${RM} ${TARGET}
	${WIPE} o		${WFLAGS}r
	@echo Stripping dependencies from makefile...
	${STRIP}
	@echo ${COMPONENT}: cleaned all

install: ${TARGET}
	${MKDIR} ${INSTAPP}
	${CP} ${TARGET}           ${INSTAPP}.${TARGET}  ${CPFLAGS}
	@echo ${COMPONENT}: installed locally

dirs:
	${MKDIR} o

#
# Target
#
${TARGET}: ${OBJS} dirs
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${CLIB} ${ZLIB}
	@echo ${COMPONENT}: built


# Dynamic dependencies:
