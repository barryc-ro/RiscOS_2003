# Makefile for UpgCreate

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
CFLAGS    = -c -depend !Depend -Wp ${INCLUDES} -DSYS${SYSTEM}=1 -DMACH${MACHINE}=1 ${DFLAGS} -DCOMPAT_INET4 ${EXT_FLAGS}
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
COMPONENT = UpgCreate
TARGET    = UpgCreate
APP       = !${COMPONENT}
INSTAPP   = ${INSTDIR}.Apps.${APP}
INSTLOCAL = Targets.${APP}
OM        = o

OBJS      =	${OM}.compress 		\
		${OM}.config		\
		${OM}.crc32		\
		${OM}.dirs		\
		${OM}.main		\
		${OM}.utils

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
	${WIPE} ${APP}		${WFLAGS}
	${WIPE} o		${WFLAGS}r
	${WIPE} Targets		${WFLAGS}r
	@echo Stripping dependencies from makefile...
	${STRIP}
	@echo ${COMPONENT}: cleaned all

resources: ${TARGET} dirs
	${MKDIR} ${RESDIR}
	${CP} ${APP} ${RESDIR}.${APP} ${CPFLAGS}
	@echo ${COMPONENT}: all complete

local: ${TARGET}
	${MKDIR} ${INSTLOCAL}
	${CP} LocalRes:!Boot      ${INSTLOCAL}.!Boot      ${CPFLAGS}
	${CP} LocalRes:!Run       ${INSTLOCAL}.!Run       ${CPFLAGS}
	${CP} LocalRes:!Help      ${INSTLOCAL}.!Help      ${CPFLAGS}
	${CP} LocalRes:!Sprites   ${INSTLOCAL}.!Sprites   ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTLOCAL}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Messages   ${INSTLOCAL}.Messages   ${CPFLAGS}
	${CP} LocalRes:Templates  ${INSTLOCAL}.Templates  ${CPFLAGS}
	${CP} ${TARGET}           ${INSTLOCAL}.${TARGET}  ${CPFLAGS}
	@echo Putting version and date into Desc file...
	@${GETVERSION} UpgFetchTool$MajorVersion UpgFetchTool$FullVersion "" "" "" UpgFetchTool$AppDateFour
	@Set UpgFetchTool$FullVersion s/Module_FullVersion/<UpgFetchTool$FullVersion>/g
	@do ${SED} "<UpgFetchTool$FullVersion>" > ${INSTLOCAL}.DescTemp < LocalRes:Desc
	@Set UpgFetchTool$AppDateFour s/Module_ApplicationDate4/<UpgFetchTool$AppDateFour>/g
	@do ${SED} "<UpgFetchTool$AppDateFour>" > ${INSTLOCAL}.Desc < ${INSTLOCAL}.DescTemp
	@${RM} ${INSTLOCAL}.DescTemp
	@echo ${COMPONENT}: installed locally

install: ${TARGET}
	${MKDIR} ${INSTAPP}
	${CP} LocalRes:!Boot      ${INSTAPP}.!Boot      ${CPFLAGS}
	${CP} LocalRes:!Run       ${INSTAPP}.!Run       ${CPFLAGS}
	${CP} LocalRes:!Help      ${INSTAPP}.!Help      ${CPFLAGS}
	${CP} LocalRes:!Sprites   ${INSTAPP}.!Sprites   ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTAPP}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Messages   ${INSTAPP}.Messages   ${CPFLAGS}
	${CP} LocalRes:Templates  ${INSTAPP}.Templates  ${CPFLAGS}
	${CP} ${TARGET}           ${INSTAPP}.${TARGET}  ${CPFLAGS}
	@echo Putting version and date into Desc file...
	@${GETVERSION} UpgFetchTool$MajorVersion UpgFetchTool$FullVersion "" "" "" UpgFetchTool$AppDateFour
	@Set UpgFetchTool$FullVersion s/Module_FullVersion/<UpgFetchTool$FullVersion>/g
	@do ${SED} "<UpgFetchTool$FullVersion>" > ${INSTAPP}.DescTemp < LocalRes:Desc
	@Set UpgFetchTool$AppDateFour s/Module_ApplicationDate4/<UpgFetchTool$AppDateFour>/g
	@do ${SED} "<UpgFetchTool$AppDateFour>" > ${INSTAPP}.Desc < ${INSTAPP}.DescTemp
	@${RM} ${INSTAPP}.DescTemp
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
