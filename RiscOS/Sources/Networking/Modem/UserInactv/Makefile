# Makefile for UserInactivity
# ===========================
#

# ------------------------------------------------------------------------------
# Paths
#

EXP_C_H = <CExport$dir>.h
RESDIR = <resource$dir>.Resources.${COMPONENT}

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
CFLAGS		= -c -depend !Depend -zM -zps1 -ffah ${INCLUDES} ${THROWBACK} ${DEBUG} ${OPTS}
CPFLAGS		= ~cfr~v
CMHGFLAGS	= -depend !Depend -p -throwback
WFLAGS		= ~c~v

DFLAGS		= 
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
UNIXLIB		= TCPIPLibs:o.UnixLib

LIBS =

LIBSD =\
 ${DEBUGLIB} \
 TCPIPLibs:o.socklib5\
 TCPIPLibs:o.inetlib\



# ------------------------------------------------------------------------------
# Include files
#

INCLUDES = -Itbox:,C:,TCPIPLibs: 


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT 	= UserInactv
TARGET		= aof.UserInactv
TARGET_D	= aof.UserInactD
TARGETSA	= rm.UserInactv
TARGETSA_D	= rm.UserInactD

OBJS =\
 o.Header\
 o.Module\
 

OBJSD =\
 o.Header\
 od.Module\

OBJSI =\
 o.Header\
 i.Module\

OBJSIO =\
 o.Header\
 io.Module\
 io.instlib\


EXPORTS =

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES:	.o .od .s .h .cmhg .c .z .i .inst .io 
.c.o:;		${CC} ${CFLAGS} ${DFLAGS} ${EXTRAFLAGS} -o $@ $<
.c.od:;		$(CC) ${CFLAGS} ${DDFLAGS} ${EXTRAFLAGS} -o $@ $<
.c.i:;		${CC} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.s.o:;		${AS} ${AFLAGS} -to $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;	${CMHG} ${CMHGFLAGS} -d $@ $<
.inst.io:;	${CC} ${CFLAGS} -C++ -W -o $@ $<

# ------------------------------------------------------------------------------
# Main rules
#

all: ${TARGETSA}
        @echo ${COMPONENT}: module built

preprocess: ${OBJSI} ${DIRS}
        @echo ${COMPONENT}: preprocess build complete

instrument: ${OBJSIO} ${DIRS} ${UNIXLIB}
        ${LD} -o rm.${COMPONENT} -module ${OBJSIO} ${LIBSD} ${EXTRALIBS} ${CLIB} ${UNIXLIB}
        @echo ${COMPONENT}: instrument build complete

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
	${MKDIR} i
	${MKDIR} io
	${MKDIR} inst
	${MKDIR} rm
	${MKDIR} linked
	${MKDIR} aof
	${MKDIR} map
	create ${DIRS}
	stamp ${DIRS}

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} i ${WFLAGS}
        ${WIPE} io ${WFLAGS}
        ${WIPE} inst ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} Docs.html ${WFLAGS}
        ${WIPE} Docs.PassFilter ${WFLAGS}
        ${WIPE} SHelpDocs ${WFLAGS}
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
        ${CP} linked.${COMPONENT} ${LINKDIR}.UserInactivity ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# ------------------------------------------------------------------------------
# Static dependencies
#

o.Module: h.Header
od.Module: h.Header
i.Module: h.Header
io.Module: h.Header

# Dynamic dependencies:
