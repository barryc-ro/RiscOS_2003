# Makefile for PassFilter
# =======================
#

# ------------------------------------------------------------------------------
# Paths
#

EXP_C_H = <CExport$dir>.h

# ------------------------------------------------------------------------------
# Generic options:
#

MKDIR		= cdir
AS		= objasm
CC		= ${MemCheckCC} cc
CMHG		= cmhg
CP		= copy
LD		= link #${MemCheckLD} link
RM		= remove
MODSQZ		= modsqz
RESGEN		= resgen
WIPE    	= -wipe

AFLAGS		= -depend !Depend -Stamp -quit ${THROWBACK}
CFLAGS		= -c -depend !Depend -zM -zps1 -ffah ${INCLUDES} ${THROWBACK} 
CPFLAGS		= ~cfr~v
CMHGFLAGS	= -depend !Depend -p -throwback
WFLAGS		= ~c~v

#DFLAGS		= -DPASSWORD=${PASSWORD} -DGOTO_URL=${GOTO_URL} -DENABLE_URL=${ENABLE_URL}
DFLAGS		= "-DPASSWORD=\"${PASSWORD}\"" "-DGOTO_URL=\"${GOTO_URL}\"" "-DENABLE_URL=\"${ENABLE_URL}\""
DDFLAGS		= ${DFLAGS} -fn -DDEBUGLIB

DIRS		= o._dirs

# -----------------------------------------------------------------------------
# Libraries
#

LIBDIR		= <Lib$Dir>
CLIB		= CLIB:o.stubs
CPPLIB		= CPPLib:o.c++lib
RLIB		= ${LIBDIR}.RISC_OSLIB.o.risc_oslib
RSTUBS		= ${LIBDIR}.RISC_OSLIB.o.rstubs
ROMSTUBS	= ${LIBDIR}.RISC_OSLIB.o.romstubs
ROMCSTUBS	= ${LIBDIR}.RISC_OSLIB.o.romcstubs
ABSSYM		= ${LIBDIR}.RISC_OSLib.o.c_AbsSym
WRAPPER		= ${LIBDIR}.RISC_OSLib.s.ModuleWrap

DEBUGLIB	= ${LIBDIR}.DebugLib.o.debuglibzm
MEMCHECKLIB	= MemCheck:o.MC_M

LIBS =\

LIBSD =\
 ${DEBUGLIB}\
 TCPIPLibs:o.socklib5\
 TCPIPLibs:o.inetlib\


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES = -Itbox:,C:


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT 	= PassFilter
TARGET		= aof.PassFilter
TARGET_D	= aof.PassFilteD
TARGETSA	= rm.PassFilter
TARGETSA_D	= rm.PassFilteD

OBJS =\
 o.Header\
 o.Module\
 o.Filter\
 

OBJSD =\
 o.Header\
 od.Module\
 od.Filter\
 

EXPORTS =

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES:	.o .od .s .h .cmhg .c .z
.c.o:;		${CC} ${CFLAGS} ${DFLAGS} ${EXTRAFLAGS} -o $@ $<
.c.od:;		$(CC) ${CFLAGS} ${DDFLAGS} ${EXTRAFLAGS} -o $@ $<
.s.o:;		${AS} ${AFLAGS} -to $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;	${CMHG} ${CMHGFLAGS} -d $@ $<

# ------------------------------------------------------------------------------
# Main rules
#

all: ${TARGETSA}
        @echo ${COMPONENT}: module built

debug: $(TARGETSA_D)
        @echo ${COMPONENT}: debug module built

normal: ${TARGETSA}
        @echo ${COMPONENT}: normal module built

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: ${TARGETSA}
        ${CP} ${TARGETSA} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

${DIRS}:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} rm
	${MKDIR} linked
	${MKDIR} aof
	${MKDIR} map
	create ${DIRS}
	stamp ${DIRS}

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${RM} h.Header
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied


# ------------------------------------------------------------------------------
# Stand-alone module
#

${TARGETSA}: ${OBJS} ${LIBS} ${CLIB} ${EXTRALIBS} ${DIRS}
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${EXTRALIBS} ${CLIB}

${TARGETSA_D}: ${OBJSD} ${LIBSD} ${EXTRALIBS} ${CLIB} ${DIRS}
        ${LD} -o $@ -module ${OBJSD} ${LIBSD} ${EXTRALIBS} ${CLIB}


# ------------------------------------------------------------------------------
# Final link for the ROM Image (re-linked at ROM Image build time)
#

${TARGET}: ${OBJS} ${LIBS} ${EXTRALIBS} ${ROMCSTUBS} ${DIRS} 
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${EXTRALIBS} ${ROMCSTUBS}

${TARGET_D}: ${OBJSD} ${LIBSD} ${EXTRALIBS} ${ROMCSTUBS} ${DIRS}
        ${LD} -o $@ -aof ${OBJSD} ${LIBSD} ${EXTRALIBS} ${ROMCSTUBS}

# ------------------------------------------------------------------------------
# Final link for the ROM Image (using given base address)
#

rom_link:
        ${LD} -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM} \
              > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# ------------------------------------------------------------------------------
# Static dependencies
#

o.Header: h.Header

# Dynamic dependencies:
o.Module:	c.Module
o.Module:	C:h.kernel
o.Module:	C:h.swis
o.Module:	C:Debuglib.h.Debuglib
o.Module:	h.Module
o.Module:	h.Filter
o.Module:	h.Header
o.Filter:	c.Filter
o.Filter:	C:h.kernel
o.Filter:	C:h.swis
o.Filter:	tbox:h.wimp
o.Filter:	C:DebugLib.h.DebugLib
o.Filter:	h.Module
o.Filter:	h.Filter
o.Filter:	h.Header
o.Header:	cmhg.Header
o.Header:	VersionNum
od.Module:	c.Module
od.Module:	C:h.kernel
od.Module:	C:h.swis
od.Module:	C:Debuglib.h.Debuglib
od.Module:	h.Module
od.Module:	h.Filter
od.Module:	h.Header
od.Filter:	c.Filter
od.Filter:	C:h.kernel
od.Filter:	C:h.swis
od.Filter:	tbox:h.wimp
od.Filter:	C:DebugLib.h.DebugLib
od.Filter:	h.Module
od.Filter:	h.Filter
od.Filter:	h.Header
