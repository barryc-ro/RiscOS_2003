# Makefile for NSPRLib
# ====================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 28-Apr-98	ADH	Created.

# -----------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
NSPRLIBDIR	= ${LIBDIR}.NSPRLib

# -----------------------------------------------------------------------------
# Generic options:
#

MKDIR		= cdir
AS		= objasm
CP		= copy
CC		= cc
CM		= cmhg
RM		= remove
LD		= link
LB		= libfile
TOUCH		= create
WIPE		= -wipe

CCFLAGS		= -c -depend !Depend -ffah -throwback -Wp ${INCLUDES} ${DFLAGS} ${UFLAGS}
ASFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~cfr~v
LBFLAGS		= -c
WFLAGS		= ~CFR~V

# -----------------------------------------------------------------------------
# Libraries
#

CLIB		= CLIB:o.stubs
INETLIB		= TCPIPLibs:o.inetlib
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym

# -----------------------------------------------------------------------------
# Include files
#

INCLUDES	= -IC:,TCPIPLibs:,RISCOSconf,NSPR.lib.ds,NSPR.pr.include,NSPR.pr.include.private,NSPR.lib.libc.include,NSPR.pr.src.io
VPATH		= RISCOSconf NSPR.lib.ds NSPR.lib.libc.src NSPR.pr.src.malloc NSPR.pr.src.misc NSPR.pr.src.io

# -----------------------------------------------------------------------------
# Compile-time options
#

DFLAGS		= -DCOMPAT_INET4  -DRISCOS  -DNSPR20
UFLAGS		= -UXP_UNIX  -UXP_OS2  -UXP_MAC

# -----------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= NSPRLib
COMPONENTZ	= NSPRLibzm
SOURCE		= ...
TARGET		= ${COMPONENT}
TARGETZ		= ${COMPONENTZ}
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.NoThreads\
 o.strcat\
 o.strccmp\
 o.strchr\
 o.strcmp\
 o.strcpy\
 o.strcstr\
 o.strdup\
 o.strlen\
 o.strpbrk\
 o.strstr\
 o.plhash\
 o.plarena\
 o.prlog2\
 o.prdtoa\
 o.prlong\
 o.prprf\
 o.prmem\

OBJSZ =\
 z.NoThreads\
 z.strcat\
 z.strccmp\
 z.strchr\
 z.strcmp\
 z.strcpy\
 z.strcstr\
 z.strdup\
 z.strlen\
 z.strpbrk\
 z.strstr\
 z.plhash\
 z.plarena\
 z.prlog2\
 z.prdtoa\
 z.prlong\
 z.prprf\
 z.prmem\

# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .h .cmhg .c .z
.c.o:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CCFLAGS} -o $@ $<

.c.z:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CCFLAGS} -zM -o $@ $<

.s.o:;		@echo
		@echo Assembling $<
		@echo ${ASFLAGS}
		@${AS} ${ASFLAGS} -o $@ $<

.cmhg.o:;	@echo
		@echo Compiling $<
		@echo ${CMFLAGS}
		@${CM} ${CMFLAGS} -o $@ $<

.cmhg.h:;	@echo
		@echo Compiling $<
		@echo ${CMFLAGS}
		@${CM} ${CMFLAGS} -d $@ $<

# -----------------------------------------------------------------------------
# build the library:
#

all:		${COMPONENT} #${COMPONENTZ}

# -----------------------------------------------------------------------------
# RISC OS ROM build rules:
#

rom: ${TARGET} ${TARGETZ}
        @echo ${COMPONENT}: ROM module built

export: export_${PHASE}

install_rom: ${TARGET} ${TARGETZM}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ROM module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} z ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${TARGETZ}
        ${RM} local_dirs
        @echo
        @echo ${COMPONENT}: Cleaned

export_hdrs: dirs
	@echo
	${CP} NSPR.pr.include.h.*			${NSPRLIBDIR}.h.*		${CPFLAGS}
	${CP} NSPR.lib.ds.h.*				${NSPRLIBDIR}.h.*		${CPFLAGS}
	${CP} RISCOSconf.h.prcpucfg			${NSPRLIBDIR}.h.*		${CPFLAGS}
	${CP} NSPR.pr.include.obsolete.h.*		${NSPRLIBDIR}.obsolete.h.*	${CPFLAGS}
	${CP} NSPR.pr.include.md.h.prosdep		${NSPRLIBDIR}.md.h.*		${CPFLAGS}
	${CP} NSPR.pr.include.md.h._riscos		${NSPRLIBDIR}.md.h.*		${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Export complete (headers)

export_libs: ${TARGET} ${TARGETZ} dirs
	@echo
	${CP} ${TARGET}		${NSPRLIBDIR}.o.${TARGET}	${CPFLAGS}
	${CP} ${TARGETZ}	${NSPRLIBDIR}.o.${TARGETZ}	${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Export complete (library)

local_dirs:
	${MKDIR} o
	${MKDIR} z
	${TOUCH} local_dirs

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${LIBDIR}.NSPRLib
        ${MKDIR} ${LIBDIR}.NSPRLib.h
        ${MKDIR} ${LIBDIR}.NSPRLib.md
        ${MKDIR} ${LIBDIR}.NSPRLib.md.h
        ${MKDIR} ${LIBDIR}.NSPRLib.obsolete
        ${MKDIR} ${LIBDIR}.NSPRLib.obsolete.h
        ${MKDIR} ${LIBDIR}.NSPRLib.o

# -----------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} local_dirs
	@echo
	${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	@echo
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# -----------------------------------------------------------------------------
# Dynamic dependencies:
