# Makefile for TestTask
#
# Paths
#
EXP_HDR = <export$dir>
RESDIR  = <Resource$Dir>.Resources.Test
#
# Generic options:
#
MKDIR   = cdir
#AS      = aasm
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
STRIP   = -stripdepnd
CD	= dir
CRUNCH  = bascrunch
INCLUDES= -IC: -ITCPIPLibs:

#AFLAGS  = -depend !Depend -Stamp -quit -To $@ -From
AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -Wp ${INCLUDES} -DSYS${SYSTEM}=1 -DMACH${MACHINE}=1 ${DFLAGS} -DCOMPAT_INET4 ${EXT_FLAGS}
#LDFLAGS =
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#DFLAGS  = ${DEBUG}
BFLAGS  = 31

## Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
SOCK       = TCPIPLibs:o.socklib
INET       = TCPIPLibs:o.inetlib
UNIX       = TCPIPLibs:o.unixlib


#
# Include files
#



# Program specific options:
#
COMPONENT = TestTask
TARGET    = !RunImage
APP	  = !${COMPONENT}
OM        = o

OBJS      = 	${OM}.Main 		\
	 	${OM}.serial  		\
	 	${OM}.command		\
	 	${OM}.serial_lib	\
	 	${OM}.timer		\
	 	${OM}.debug		\
	 	${OM}.DRAMTest		\
	 	${OM}.Support		\
	 	${OM}.Pop               \
	 	${OM}.parallel		\
	 	${OM}.mouse		\
	 	${OM}.keyboard		\
	 	${OM}.CPUType		\
	 	${OM}.versions		\
	 	${OM}.IOControl         \
	 	${OM}.sound             \
	 	${OM}.eeprom		\
	 	${OM}.PS2		\
	 	${OM}.PeeknPoke		\
	 	${OM}.twiddle		\
	 	${OM}.CMOS		\
	 	${OM}.Graphics		\
	 	${OM}.RGBBanner		\
	 	${OM}.STBLED		\
	 	${OM}.SerialLoop	\
	 	${OM}.ethertest		\
	 	${OM}.ReadIR		\
	 	${OM}.IR		\
	 	${OM}.modulator         \
	 	${OM}.SCART		\
	 	${OM}.MPEG		\
	 	${OM}.ROMChksum		\
	 	${OM}.ID
	 						
	 			
.SUFFIXES: .${OM}

#
# Rule patterns
#
.c.${OM}:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.${OM}:;   ${CMHG} -o $@ $<
.s.${OM}:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE}	o.*		${WFLAGS}
        @echo ${COMPONENT}: cleaned

clean_all: clean
	${RM} ${TARGET}
	${WIPE}	${APP}		${WFLAGS}
	${WIPE} o		${WFLAGS}
	${WIPE} rm		${WFLAGS}
        @echo Stripping dependencies from makefile...
        ${STRIP}
	@echo ${COMPONENT}: cleaned all

resources: ${TARGET}
        @echo ${COMPONENT}: all complete

install: ${TARGET}
         @echo ${COMPONENT}: installed

dirs:
	${MKDIR} o
	${MKDIR} rm
	
#
# Target
#
${TARGET}: ${OBJS} dirs
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${CLIB} ${SOCK} ${INET} ${UNIX}
        squeeze $@
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	Run b.PreTestSrc
	${CP} Resources.* ${APP}.* ${CPFLAGS}
	${MKDIR} ${RESDIR}
	${CP} ${APP} ${RESDIR}.${APP} ${CPFLAGS}
	@echo ${COMPONENT}: built & installed
        

# Dynamic dependencies:
