# Makefile for TestTask
#
# 10/4/00 added McCabe instrumentation 
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
	 	${OM}.ID		\
	 	${OM}.gen_ir		\
	 	${OM}.Flash		\
	 	${OM}.get_t0		
	 			
	 						
OBJSI      = 	i.Main 		\
	 	i.serial  	\
	 	i.command	\
	 	i.serial_lib	\
	 	i.timer		\
	 	i.debug		\
	 	i.DRAMTest	\
	 	i.Support	\
	 	i.Pop           \
	 	i.parallel	\
	 	i.mouse		\
	 	i.keyboard	\
	 	i.versions	\
	 	i.sound         \
	 	i.eeprom	\
	 	i.PS2		\
	 	i.twiddle	\
	 	i.CMOS		\
	 	i.Graphics	\
	 	i.RGBBanner	\
	 	i.STBLED	\
	 	i.SerialLoop	\
	 	i.ethertest	\
	 	i.IR		\
	 	i.modulator     \
	 	i.SCART		\
	 	i.MPEG		\
	 	i.ID		\
	 	i.gen_ir	\
	 	i.Flash			
	 	
OBJSIO      = 	io.Main 	\
	 	io.serial  	\
	 	io.command	\
	 	io.serial_lib	\
	 	io.timer	\
	 	io.debug	\
	 	io.DRAMTest	\
	 	io.Support	\
	 	io.Pop          \
	 	io.parallel	\
	 	io.mouse	\
	 	io.keyboard	\
	 	io.versions	\
	 	io.sound        \
	 	io.eeprom	\
	 	io.PS2		\
	 	io.twiddle	\
	 	io.CMOS		\
	 	io.Graphics	\
	 	io.RGBBanner	\
	 	io.STBLED	\
	 	io.SerialLoop	\
	 	io.ethertest	\
	 	io.IR		\
	 	io.modulator    \
	 	io.SCART	\
	 	io.MPEG		\
	 	io.ID		\
	 	io.gen_ir	\
	 	io.Flash	\
	 	io.instlib 	\
	 	${OM}.ROMChksum	        \
	 	${OM}.CPUType		\
	 	${OM}.get_t0		\
	 	${OM}.IOControl         \
	 	${OM}.PeeknPoke		\
	 	${OM}.ReadIR		\
	 	${OM}.ROMChksum		\
	 	

	 			
.SUFFIXES: .o .s .c .c++ .i .cmhg .inst .io

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.inst.io:;  ${CC} ${CFLAGS} -C++ -W -o $@ $<
.c++.i:;    ${C++} ${C++FLAGS} -c -C -E $< >> $@
.c.i:;     ${CC} ${CFLAGS} -c -C -E $< >> $@

#
# Build
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE}	o.*		${WFLAGS}
        ${WIPE} i.*		${WFLAGS}
        ${WIPE} io.*		${WFLAGS}
        @echo ${COMPONENT}: cleaned

clean_all: clean
	${RM} ${TARGET}
	${RM} inst
	${RM} i
	${RM} io
	${WIPE}	${APP}		${WFLAGS}
	${WIPE} o		${WFLAGS}
        @echo Stripping dependencies from makefile...
        ${STRIP}
	@echo ${COMPONENT}: cleaned all

resources: ${TARGET}
        ${MKDIR} ${RESDIR}
	${CP} ${APP} ${RESDIR}.${APP} ${CPFLAGS}
        @echo ${COMPONENT}: all complete

install: ${TARGET}
         @echo ${COMPONENT}: installed

dirs:
	${MKDIR} o
	${MKDIR} i
	${MKDIR} inst
	${MKDIR} io

preprocess: ${OBJSI} dirs
	@echo ${COMPONENT}: preprocess build complete
	
instrument: ${OBJSIO} dirs
	${LD} ${LDFLAGS} -o $@ ${OBJSIO} ${CLIB} ${SOCK} ${INET} ${UNIX}
	@echo ${COMPONENT}: instrument build complete			
	
	
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
	@echo ${COMPONENT}: built
        

# Dynamic dependencies:
