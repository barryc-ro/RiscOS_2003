# Makefile for JavaScript test application
# ========================================
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

CCFLAGS		= -c -depend !Depend -ffah -throwback -Wp ${INCLUDES} ${UFLAGS}
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
JSLIB		= C:JSLib.o.JSLib
NSPRLIB		= C:NSPRLib.o.NSPRLib

LIBS = \
 ${CLIB} \
 ${INETLIB} \
 ${JSLIB} \
 ${NSPRLIB}

LIBSD = \
 ${CLIB} \
 ${INETLIB} \
 ${JSLIB} \
 ${NSPRLIB}

# -----------------------------------------------------------------------------
# Include files
#

INCLUDES	= -IC:,TCPIPLibs:,C:NSPRLib,C:JSLib

# -----------------------------------------------------------------------------
# Compile-time options
#

UFLAGS		= -UXP_UNIX  -UXP_OS2  -UXP_MAC
DFLAGS		= -DCOMPAT_INET4  -DRISCOS  -DNSPR20  -DJSFILE
DDFLAGS		= -DCOMPAT_INET4  -DRISCOS  -DNSPR20  -DJSFILE  -DTRACE

# -----------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= !TstImage
COMPONENTD	= !TstImageD
SOURCE		= ...
TARGET		= ${COMPONENT}
TARGETD		= ${COMPONENTD}

OBJS = \
 o.JSTests

OBJSD = \
 od.svcprint \
 od.JSTests

# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .od .s .c

.c.o:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CCFLAGS} ${DFLAGS} -o $@ $<

.c.od:;		@echo
		@echo Compiling $<
		@echo ${DDFLAGS}
		@${CC} ${CCFLAGS} ${DDFLAGS} -g -o $@ $<

.s.o:;		@echo
		@echo Assembling $<
		@echo ${ASFLAGS}
		@${AS} ${ASFLAGS} -o $@ $<

# -----------------------------------------------------------------------------
# Build the application:
#

all:	${TARGET} dirs
	@echo ${COMPONENT}: Application built (disc, non-debug)

all_d:	${TARGETD} dirs
	@echo ${COMPONENTD}: Application built (disc, debug)

# -----------------------------------------------------------------------------
# Build rules:
#

clean:
	${WIPE} o  ${WFLAGS}
	${WIPE} od ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGETD}
	${RM} local_dirs
	@echo
	@echo ${COMPONENT}: Cleaned

local_dirs:
	${MKDIR} o
	${MKDIR} od
	${TOUCH} local_dirs

dirs:
	${MKDIR} o
	${MKDIR} od

# -----------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} local_dirs
	@echo
	${LD} -o ${COMPONENT} ${OBJS} ${LIBS}

${TARGETD}: ${OBJSD} local_dirs
	@echo
	${LD} -debug -o ${COMPONENTD} ${OBJSD} ${LIBSD}

# -----------------------------------------------------------------------------
# Dynamic dependencies:
