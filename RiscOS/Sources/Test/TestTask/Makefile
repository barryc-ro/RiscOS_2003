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
	 	${OM}.get_t0		\
	 	${OM}.usb		
	 			
	 						
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
o.Main: c.Main C:h.swis C:h.kernel h.serial_lib C:h.kernel h.serial h.timer h.command h.debug h.support h.MPEG h.IR C:h.kernel
o.serial_lib: c.serial_lib C:h.kernel C:h.swis h.serial_lib C:h.kernel h.debug
o.timer: c.timer C:h.swis C:h.kernel h.timer
o.DRAMTest: c.DRAMTest C:h.kernel C:h.swis h.dramtest h.MemSrcNCOS h.debug h.Support C:h.stdlib
o.Support: c.Support C:h.swis C:h.kernel C:h.string h.Support h.debug C:h.math
o.Pop: c.Pop C:h.swis C:h.kernel h.debug
o.parallel: c.parallel h.debug h.Pop C:h.stdio C:h.string C:h.stdlib
o.mouse: c.mouse C:h.swis C:h.kernel h.debug
o.CPUType: s.CPUType Hdr:ListOpts Hdr:Macros Hdr:MEMM.ARM600 Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS
o.IOControl: s.IOControl Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:IO.IOMD
o.sound: c.sound C:h.stdio C:h.stdlib h.debug C:h.swis C:h.kernel
o.eeprom: c.eeprom C:h.swis C:h.kernel h.debug h.support C:h.string
o.PeeknPoke: s.PeeknPoke Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:IO.IOMD
o.CMOS: c.CMOS C:h.kernel C:h.swis
o.Graphics: c.Graphics C:h.swis C:h.kernel h.debug
o.RGBBanner: c.RGBBanner h.Graphics h.RGBBanner C:h.swis C:h.kernel h.debug
o.STBLED: c.STBLED C:h.swis C:h.kernel h.PeeknPoke
o.ethertest: c.ethertest TCPIPLibs:sys.h.types TCPIPLibs:sys.h.cdefs TCPIPLibs:machine.h.endian TCPIPLibs:machine.h.ansi TCPIPLibs:machine.h.types TCPIPLibs:sys.h.param TCPIPLibs:sys.h.types TCPIPLibs:sys.h.syslimits TCPIPLibs:sys.h.signal TCPIPLibs:machine.h.signal TCPIPLibs:sys.h.cdefs TCPIPLibs:machine.h.param TCPIPLibs:machine.h.limits TCPIPLibs:sys.h.errno C:h.kernel TCPIPLibs:sys.h.socket TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.ioctl TCPIPLibs:sys.h.ttycom TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.filio TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.sockio TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.cdefs TCPIPLibs:net.h.route TCPIPLibs:net.h.radix TCPIPLibs:net.h.if TCPIPLibs:sys.h.time TCPIPLibs:sys.h.cdefs TCPIPLibs:netinet.h.in TCPIPLibs:netinet.h.in_systm TCPIPLibs:netinet.h.ip TCPIPLibs:netinet.h.ip_icmp TCPIPLibs:netinet.h.udp TCPIPLibs:netinet.h.if_ether TCPIPLibs:net.h.ethernet TCPIPLibs:sys.h.cdefs TCPIPLibs:net.h.if_arp TCPIPLibs:sys.h.socket TCPIPLibs:arpa.h.inet TCPIPLibs:sys.h.cdefs TCPIPLibs:arpa.h.nameser TCPIPLibs:sys.h.param TCPIPLibs:sys.h.types TCPIPLibs:sys.h.cdefs TCPIPLibs:machine.h.endian TCPIPLibs:h.netdb TCPIPLibs:sys.h.cdefs TCPIPLibs:h.socklib TCPIPLibs:sys.h.types TCPIPLibs:sys.h.socket TCPIPLibs:sys.h.stat TCPIPLibs:sys.h.time TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.time TCPIPLibs:sys.h.uio TCPIPLibs:sys.h.cdefs C:h.kernel TCPIPLibs:h.inetlib TCPIPLibs:sys.h.types TCPIPLibs:netinet.h.in TCPIPLibs:h.unixlib C:h.swis TCPIPLibs:sys.h.dcistructs TCPIPLibs:sys.h.errno h.debug C:h.swis h.timer
o.ReadIR: s.ReadIR Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:IO.IOMD
o.modulator: c.modulator C:h.swis C:h.kernel h.debug C:h.stdio C:h.string
o.SCART: c.SCART C:h.swis C:h.kernel h.debug C:h.stdio
o.MPEG: c.MPEG C:h.swis C:h.kernel h.Debug h.Graphics
o.ROMChksum: s.ROMChksum Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:IO.IOMD
o.gen_ir: c.gen_ir TCPIPLibs:h.unixlib TCPIPLibs:sys.h.time TCPIPLibs:sys.h.cdefs C:h.swis C:h.kernel h.PeeknPoke h.support h.debug h.get_t0
o.Flash: c.Flash h.PeeknPoke h.debug C:h.stdlib C:h.kernel
o.get_t0: s.get_t0 Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:APCS.<APCS> Hdr:APCS.Common
o.versions: c.versions C:h.swis C:h.kernel h.debug h.CPUType h.IOControl C:h.stdio C:h.string
o.command: c.command C:h.swis C:h.kernel h.serial_lib C:h.kernel h.serial h.timer h.main h.debug h.support h.DRAMTest h.parallel h.mouse h.keyboard h.versions h.sound h.eeprom h.PS2 h.twiddle h.CMOS h.RGBBanner h.Graphics h.STBLED h.SerialLoop h.ethertest h.IR h.modulator h.SCART h.MPEG h.ROMCheck h.ID h.gen_ir h.Flash h.usb
o.usb: s.usb Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Proc Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:IO.IOMD
