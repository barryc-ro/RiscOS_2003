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
COMPONENT = UpgCreate
TARGET    = UpgCreate
APP       = !${COMPONENT}
INSTAPP   = ${INSTDIR}.Apps.${APP}
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

clean_all:
	${RM} ${TARGET}
	${WIPE} o		${WFLAGS}r
	${WIPE} Dirs		${WFLAGS}
	${WIPE} Targets		${WFLAGS}r
	@echo
	@echo ${COMPONENT}: cleaned all

resources: ${TARGET}
	${MKDIR} ${RESDIR}.^.Apps.${APP}
	${CP} LocalRes:!Boot      ${RESDIR}.^.Apps.${APP}.!Boot        ${CPFLAGS}
	${CP} LocalRes:!Run       ${RESDIR}.^.Apps.${APP}.!Run         ${CPFLAGS}
	${CP} LocalRes:!Help      ${RESDIR}.^.Apps.${APP}.!Help        ${CPFLAGS}
	${CP} LocalRes:!Sprites   ${RESDIR}.^.Apps.${APP}.!Sprites     ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${RESDIR}.^.Apps.${APP}.!Sprites22   ${CPFLAGS}
	${CP} LocalRes:Messages   ${RESDIR}.^.Apps.${APP}.Messages     ${CPFLAGS}
	${CP} LocalRes:Templates  ${RESDIR}.^.Apps.${APP}.Templates    ${CPFLAGS}
	${CP} ${TARGET}           ${RESDIR}.^.Apps.${APP}.${TARGET}    ${CPFLAGS}
	@echo Putting version and date into Desc file...
	@${GETVERSION} UpgFetchTool$MajorVersion UpgFetchTool$FullVersion "" "" "" UpgFetchTool$AppDateFour
	@Set UpgFetchTool$FullVersion s/Module_FullVersion/<UpgFetchTool$FullVersion>/g
	@do ${SED} "<UpgFetchTool$FullVersion>" > ${RESDIR}.^.Apps.${APP}.DescTemp < LocalRes:Desc
	@Set UpgFetchTool$AppDateFour s/Module_ApplicationDate4/<UpgFetchTool$AppDateFour>/g
	@do ${SED} "<UpgFetchTool$AppDateFour>" > ${RESDIR}.^.Apps.${APP}.Desc < ${RESDIR}.^.Apps.${APP}.DescTemp
	@${RM} ${RESDIR}.^.Apps.${APP}.DescTemp
	@echo ${COMPONENT}: all complete

install: ${TARGET}
	${MKDIR} ${INSTAPP}
	${CP} LocalRes:!Boot      ${INSTAPP}.!Boot        ${CPFLAGS}
	${CP} LocalRes:!Run       ${INSTAPP}.!Run         ${CPFLAGS}
	${CP} LocalRes:!Help      ${INSTAPP}.!Help        ${CPFLAGS}
	${CP} LocalRes:!Sprites   ${INSTAPP}.!Sprites     ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTAPP}.!Sprites22   ${CPFLAGS}
	${CP} LocalRes:Messages   ${INSTAPP}.Messages     ${CPFLAGS}
	${CP} LocalRes:Templates  ${INSTAPP}.Templates    ${CPFLAGS}
	${CP} ${TARGET}           ${INSTAPP}.${TARGET}    ${CPFLAGS}
	@echo Putting version and date into Desc file...
	@${GETVERSION} UpgFetchTool$MajorVersion UpgFetchTool$FullVersion "" "" "" UpgFetchTool$AppDateFour
	@Set UpgFetchTool$FullVersion s/Module_FullVersion/<UpgFetchTool$FullVersion>/g
	@do ${SED} "<UpgFetchTool$FullVersion>" > ${INSTAPP}.DescTemp < LocalRes:Desc
	@Set UpgFetchTool$AppDateFour s/Module_ApplicationDate4/<UpgFetchTool$AppDateFour>/g
	@do ${SED} "<UpgFetchTool$AppDateFour>" > ${INSTAPP}.Desc < ${INSTAPP}.DescTemp
	@${RM} ${INSTAPP}.DescTemp
	@echo ${COMPONENT}: all complete

dirs:
	${MKDIR} o
	Create Dirs 0
	Stamp Dirs

#
# Target
#
${TARGET}: ${OBJS} dirs
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${CLIB} ${ZLIB}
	@echo ${COMPONENT}: built


# Dynamic dependencies:
