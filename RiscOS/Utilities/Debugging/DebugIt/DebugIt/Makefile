# Makefile for TestMod
# ====================
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
WFLAGS		= ~c~v

DFLAGS		= -fn -DDEBUGLIB
DDFLAGS		= -fn -DDEBUGLIB

# -----------------------------------------------------------------------------
# Libraries
#

LIBDIR		= <Lib$Dir>
CLIB		= CLIB:o.stubs
CPPLIB		= CPPLib:o.c++lib
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_AbsSym
WRAPPER		= RISC_OSLib:s.ModuleWrap

DEBUGLIB	= ${LIBDIR}.DebugLib.o.debuglibzm
MEMCHECKLIB	= MemCheck:o.MC_M

LIBS = \
 ${DEBUGLIB} \
 TCPIPLibs:o.socklib5 \
 TCPIPLibs:o.inetlib \
 ${MEMCHECKLIB} \

LIBSD = \
 ${DEBUGLIB} \
 TCPIPLibs:o.socklib5 \
 TCPIPLibs:o.inetlib \
 ${MEMCHECKLIB} \


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES = -IC:


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT 	= DebugIt
TARGET		= aof.DebugIt
TARGET_D	= aof.DebugItD
TARGETSA	= rm.DebugIt
TARGETSA_D	= rm.DebugItD

OBJS = \
 o.Main \
 o.debugitHDR\
 

OBJSD = \
 o.Main \
 o.debugitHDR\


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

${TARGETSA}: ${OBJS} ${LIBS} ${CLIB} o.dirs
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}

${TARGETSA_D}: ${OBJSD} ${LIBSD} ${CLIB} o.dirs
        ${LD} -o $@ -module ${OBJSD} ${LIBSD} ${CLIB}

# ------------------------------------------------------------------------------
# Final link for the ROM Image (re-linked at ROM Image build time)
#

${TARGET}: ${OBJS} ${LIBS} ${ROMCSTUBS} o.dirs
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

${TARGET_D}: ${OBJSD} ${LIBSD} ${ROMCSTUBS} o.dirs
        ${LD} -o $@ -aof ${OBJSD} ${LIBSD} ${ROMCSTUBS}

# ------------------------------------------------------------------------------
# Final link for the ROM Image (using given base address)
#

rom_link:
        ${LD} -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM} \
              > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
