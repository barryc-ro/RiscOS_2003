# Makefile for FlashWrite
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
CMHGFLAGS	= -depend !Depend -p -throwback ${DFLAGS}
WFLAGS		= ~c~v

DFLAGS		= 
DDFLAGS		= -fn -DDEBUGLIB

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

LIBS = \

LIBSD = \
 ${DEBUGLIB} \
 TCPIPLibs:o.socklib5 \
 TCPIPLibs:o.inetlib \


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES = -IC:


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT 	= FlashWrite
TARGET		= aof.FlashWri
TARGET_D	= aof.FlashWriD
TARGETSA	= rm.FlashWri
TARGETSA_D	= rm.FlashWriD

OBJS = \
 o.calls \
 o.Header \
 o.error \
 o.Module \
 o.FlashProg \
 

OBJSD = \
 o.calls \
 o.Header \
 od.error \
 od.Module \
 od.FlashProg \
 

EXPORTS =

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES:	.o .od .s .h .cmhg .c .z
.c.o:;		${CC} ${CFLAGS} ${DFLAGS} ${EXTRAFLAGS} -o $@ $<
.c.od:;		$(CC) ${CFLAGS} ${DDFLAGS} ${EXTRAFLAGS} -o $@ $<
.s.o:;		${AS} ${AFLAGS} -to $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} -o $@ $<

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

o.dirs:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} rm
	${MKDIR} linked
	${MKDIR} aof
	${MKDIR} map
	create o.dirs
	stamp o.dirs

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied


# ------------------------------------------------------------------------------
# Stand-alone module
#

${TARGETSA}: ${OBJS} ${LIBS} ${CLIB} ${EXTRALIBS} o.dirs
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${EXTRALIBS} ${CLIB}

${TARGETSA_D}: ${OBJSD} ${LIBSD} ${EXTRALIBS} ${CLIB} o.dirs
        ${LD} -o $@ -module ${OBJSD} ${LIBSD} ${EXTRALIBS} ${CLIB}

# ------------------------------------------------------------------------------
# Final link for the ROM Image (re-linked at ROM Image build time)
#

${TARGET}: ${OBJS} ${LIBS} ${EXTRALIBS} ${ROMCSTUBS} o.dirs 
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${EXTRALIBS} ${ROMCSTUBS}

${TARGET_D}: ${OBJSD} ${LIBSD} ${EXTRALIBS} ${ROMCSTUBS} o.dirs
        ${LD} -o $@ -aof ${OBJSD} ${LIBSD} ${EXTRALIBS} ${ROMCSTUBS}

# ------------------------------------------------------------------------------
# Final link for the ROM Image (using given base address)
#

rom_link:
        ${LD} -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM} \
              > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
