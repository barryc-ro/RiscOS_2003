# Makefile for UpgFile
#
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

## Libraries
#
LIBDIR		= <Lib$Dir>

CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ZLIB	   = ${LIBDIR}.zlib.o.zlib

#
# Include files
#

INCLUDES= -IC: -ITCPIPLibs: 

# Program specific options:
#
COMPONENT = UpgCreate
TARGET    = UpgCreate
APP	  = !${COMPONENT}
OM        = o

OBJS      = 	${OM}.main		\
		${OM}.compress 		\
		${OM}.crc32		\
		${OM}.config		\
	 			
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
        ${WIPE}	o.*		${WFLAGS}
        ${RM}	{TARGET}
        @echo ${COMPONENT}: cleaned

clean_all: clean
	${RM} ${TARGET}
	${WIPE}	${APP}		${WFLAGS}
	${WIPE} o		${WFLAGS}
        @echo Stripping dependencies from makefile...
        ${STRIP}
	@echo ${COMPONENT}: cleaned all

resources: ${TARGET} dirs
        ${MKDIR} ${RESDIR}
	${CP} ${APP} ${RESDIR}.${APP} ${CPFLAGS}
        @echo ${COMPONENT}: all complete

install: ${TARGET}
         @echo ${COMPONENT}: installed

dirs:
	${MKDIR} o

			
#
# Target
#
${TARGET}: ${OBJS} dirs
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${CLIB} ${ZLIB}
	@echo ${COMPONENT}: built
        

# Dynamic dependencies:
