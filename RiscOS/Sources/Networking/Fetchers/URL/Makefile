# Makefile for URL
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 10-Feb-98  SNB    Created
# 06-Oct-98  SNB    Updated to use LocalRes, link 5.08
#

#
# Component specific options:
#
COMPONENT  = URL
ROM_MODULE = aof.${COMPONENT}
RAM_MODULE = rm.${COMPONENT}
DBG_MODULE = rm.${COMPONENT}-tr


RESFSDIR   = Resources.URL.${COMPONENT}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.URL
EXP_HDR = <cexport$dir>.Interface

#
# Generic options:
#
CFLAGS    = ${INCLUDES} -zM -Wp -ffah -zps1 ${DFLAGS}
CDEFINES  = -D${SYSTEM} -DCOMPAT_INET4 -UTML -UDIRECTSWI
CINCLUDES = -IC: -IC:TCPIPLibs
ROMFLAGS  = -DROM
DBGFLAGS  = -DDEBUGLIB

include Makefiles:StdTools

#
# Libraries
#
include Makefiles:ModuleLibs

OBJS =\
 o.directswi\
 o.enumerate\
 o.generic\
 o.geturl\
 o.module\
 o.parseurl\
 o.protocol\
 o.proxy\
 o.readdata\
 o.register\
 o.status\
 o.stop\
 o.utils\
 o.URLHdr

RAM_OBJS =\
 o.directswi\
 o.enumerate\
 o.generic\
 o.geturl\
 o.moduleRAM\
 o.parseurl\
 o.protocol\
 o.proxy\
 o.readdata\
 o.register\
 o.status\
 o.stop\
 o.utils\
 o.URLHdr

DBG_OBJS =\
 o.directswi\
 od.enumerate\
 od.generic\
 od.geturl\
 od.moduleRAM\
 od.parseurl\
 od.protocol\
 od.proxy\
 od.readdata\
 od.register\
 od.status\
 od.stop\
 od.utils\
 o.URLHdr

MSGSF=o.msgs


PNGS=\
  LocalRes:images.internal-gopher-binary\
  LocalRes:images.internal-gopher-image\
  LocalRes:images.internal-gopher-index\
  LocalRes:images.internal-gopher-menu\
  LocalRes:images.internal-gopher-menu-grey\
  LocalRes:images.internal-gopher-movie\
  LocalRes:images.internal-gopher-sound\
  LocalRes:images.internal-gopher-telnet\
  LocalRes:images.internal-gopher-text\
  LocalRes:images.internal-gopher-unknown\
  LocalRes:images.internal-gopher-unknown-grey

#
# Rule patterns
#
.SUFFIXES: .c .s .o .od .i .oi .inst
.c.o:;    ${CC} ${CFLAGS} ${ROMFLAGS} -o $@ $<
.c.i:;    ${CC} ${CFLAGS} ${ROMFLAGS} -C -E $< >$@
.s.o:;    ${AS} ${ASFLAGS} -o $@ $<
.c.od:;   ${CC} ${CFLAGS} ${ROMFLAGS} ${DBGFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:; ${CMHG} ${CMHGFLAGS} -d $@ $<

od.protocol protocol.o: URLHdr.h


#
# Main rules:
#
#
all: ${RAM_MODULE} o.dirs
	@echo ${COMPONENT}: Module built (RAM)

install: ${RAM_MODULE} o.dirs
	${MKDIR} ${INSTDIR}
        ${CP} ${RAM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: Module installed (Disc)

ramtrace: ${DBG_MODULE} o.dirs
	@echo ${COMPONENT}: Module built (RAM - trace)

rom: ${ROM_MODULE} o.dirs
	@echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: Module installed (ROM)

resources: LocalRes:Messages LocalRes:ResList ${PNGS}
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck LocalRes:Messages
	${CP} LocalRes:* ${RESDIR}.${COMPONENT}.* ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied to Messages module

export: export_${PHASE}

export_libs:
	@echo ${COMPONENT}: export complete

export_hdrs: ${EXP_HDR}.h.${COMPONENT}
	@echo ${COMPONENT}: export complete

${EXP_HDR}.h.${COMPONENT}: h.URLHdr
	awk -- "/.define URL\_00/,/endif/" h.URLHdr { > $@ }
	@SetType $@ Text

o.dirs:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} i
	${MKDIR} oi
	${MKDIR} inst
	${TOUCH} $@

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} od ${WFLAGS}
	${WIPE} i ${WFLAGS}
	${WIPE} oi ${WFLAGS}
	${WIPE} inst ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${WIPE} aof ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${WIPE} h.${COMPONENT}Hdr ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${ROMCSTUBS}
	${MKDIR} aof
	${LD} -o $@ -aof ${OBJS} ${NET4LIBS} ${ROMCSTUBS}

# final link for ROM Image (using given base address)
rom_link: o.dirs
	${MKDIR} linked
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

${MSGSF}: LocalRes:Messages LocalRes:ResList ${PNGS}
	TokenCheck LocalRes:Messages
	resgen messages_file ${MSGSF} -via LocalRes:ResList

${RAM_MODULE}: ${RAM_OBJS} ${MSGSF} ${NET4LIBS} ${CLIB}
	${mkdir} rm
	${LD} -o $@ -module ${RAM_OBJS} ${MSGSF} ${NET4LIBS} ${CLIB}
	${MODSQZ} $@
	${CHMOD} u+w,a+r $@

${DBG_MODULE}: ${DBG_OBJS} ${MSGSF} ${NET4LIBS} ${CLIB} ${DEBUGLIB}
	${mkdir} rm
	${LD} -o $@ -module ${DBG_OBJS} ${MSGSF} ${DEBUGLIB} ${NET4LIBS} ${CLIB}
	${CHMOD} u+w,a+r $@

o.moduleRAM: module.c
	$(CC) ${CFLAGS} -o $@ module.c

od.moduleRAM: module.c
	$(CC) ${CFLAGS} ${DBGFLAGS} -o $@ module.c


#---------------------------------------------------------------------------
# Dynamic dependencies:
