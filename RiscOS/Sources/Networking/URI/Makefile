# Makefile for URI
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date		Name	Description
# ----		----	-----------
# 1997-06-23	BAL	Created
#

#
# Paths
#
EXP_HDR	=	<export$dir>

#
# Generic options:
#
MKDIR =		cdir
AS =		objasm
CC =		cc
CMHG =		cmhg
CP =		copy
LD =		link
RM =		remove
MODSQZ =	modsqz
WIPE =		-wipe

ASMFLAGS =	-depend !Depend -Stamp -quit
CFLAGS =	-c -depend !Depend -throwback -zM ${INCLUDES} ${DFLAGS}
CMHGFLAGS =	-depend !Depend -p -throwback ${INCLUDES}
CPFLAGS =	~cfr~v
WFLAGS =	~c~v

#
# Libraries
#
CLIB =		CLIB:o.stubs
RLIB =		RISCOSLIB:o.risc_oslib
RSTUBS =	RISCOSLIB:o.rstubs
ROMSTUBS =	RISCOSLIB:o.romstubs
ROMCSTUBS =	RISCOSLIB:o.romcstubs
ABSSYM  =	RISC_OSLib:o.c_AbsSym
LIBS =		C:tboxlibs.o.eventlibm C:tboxlibs.o.wimplib

#
# Include files
#
INCLUDES =	-Itbox:,TCPIPLibs:,C:

#
# Program specific options:
#
COMPONENT =	URI
RAMTARGET =	rm.URI
ROMTARGET =	aof.URI
EXPORTS =	${EXP_DIR}.URI

#
# Export Paths for Messages module
#
#RESDIR = 	<resource$dir>.Resources2.URI


OBJS =\
 o.cstart \
 o.globals \
 o.ModHdr \
 o.module \
 o.ralloc \
 o.robase \
 o.task \
 o.debug \
 o.URIswis

HDRS =	h.ModHdr

#
# Rule patterns
#
.SUFFIXES:	.o .s .c .h
.c.o:;		${CC} ${CFLAGS} -o $@ $<
.s.o:;		${AS} ${ASMFLAGS} -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} $< -o $@ -d h.$*
.cmhg.h:;	${CMHG} ${CMHGFLAGS} $< -d $@ -o o.$*

#
# Main rules:
#
ram: ${RAMTARGET}
        @echo ${COMPONENT}: all complete

rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export:
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

#resources:
#        ${MKDIR} ${RESDIR}.^.^.URI
#        ${MKDIR} ${RESDIR}
#        ${CP} ${LDIR}.Messages ${RESDIR}.Messages ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} aof ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${WIPE} map ${WFLAGS}
	${WIPE} ${HDRS} ${WFLAGS}
	${RM} ${RAMTARGET}
	@echo ${COMPONENT}: cleaned

#
# Target
#
${RAMTARGET}: ${OBJS} ${HDRS} ${CLIB}
  	${MKDIR} rm
	${LD} -rmf -o $@ ${OBJS} ${LIBS} ${CLIB}
	${MODSQZ} $@

#
# ROM Target (re-linked at ROM Image build time)
#
${ROMTARGET}: ${OBJS} ${HDRS} ${ROMCSTUBS}
        ${MKDIR} aof
	${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
o.cstart: s.cstart
o.globals:	c.globals
o.globals:	h.URIdefs
o.globals:	h.ModHdr
o.globals:	C:h.kernel
o.globals:	h.globals
o.globals:	h.URI
o.globals:	C:h.kernel
o.globals:	h.ModHdr
o.ModHdr:	cmhg.ModHdr
o.ModHdr:	h.URIdefs
o.ModHdr:	tbox:h.wimp
o.ModHdr:	OS:h.os
o.ModHdr:	OS:h.types
o.module:	c.module
o.module:	C:h.kernel
o.module:	tbox:h.wimp
o.module:	tbox:h.wimplib
o.module:	C:h.swis
o.module:	OS:h.os
o.module:	OS:h.types
o.module:	h.ralloc
o.module:	h.globals
o.module:	h.URI
o.module:	C:h.kernel
o.module:	h.ModHdr
o.module:	h.URIdefs
o.module:	h.ModHdr
o.module:	h.URIdefs
o.module:	h.task
o.module:	h.URIswis
o.ralloc:	c.ralloc
o.ralloc:	C:h.kernel
o.ralloc:	C:h.swis
o.ralloc:	h.ralloc
o.ralloc:	h.globals
o.ralloc:	h.URI
o.ralloc:	C:h.kernel
o.ralloc:	h.ModHdr
o.ralloc:	h.URIdefs
o.robase: s.robase
o.task:	c.task
o.task:	C:h.kernel
o.task:	tbox:h.wimp
o.task:	tbox:h.wimplib
o.task:	C:h.swis
o.task:	h.URI
o.task:	C:h.kernel
o.task:	h.task
o.task:	h.globals
o.task:	h.ModHdr
o.task:	h.URIdefs
o.task:	h.URIswis
o.task:	h.ralloc
o.task:	h.module
o.debug:	c.debug
o.debug:	h.URIdefs
o.debug:	h.ModHdr
o.debug:	C:h.kernel
o.debug:	h.globals
o.debug:	h.URI
o.debug:	C:h.kernel
o.debug:	h.ModHdr
o.URIswis:	c.URIswis
o.URIswis:	C:h.kernel
o.URIswis:	C:h.swis
o.URIswis:	OS:h.os
o.URIswis:	OS:h.types
o.URIswis:	h.ralloc
o.URIswis:	h.globals
o.URIswis:	h.URI
o.URIswis:	C:h.kernel
o.URIswis:	h.ModHdr
o.URIswis:	h.URIdefs
o.URIswis:	h.URIswis
o.URIswis:	h.module
o.ModHdr:	cmhg.ModHdr
o.ModHdr:	h.URIdefs
o.ModHdr:	tbox:h.wimp
o.ModHdr:	OS:h.os
o.ModHdr:	OS:h.types
