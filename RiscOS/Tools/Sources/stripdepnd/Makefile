# Makefile for stripdepnd
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 17-Nov-98  SNB          Created

COMPONENT = stripdepnd
RAM_MODULE = aif.${COMPONENT}

#
# Paths
#
EXP_HDR = <export$dir>

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.${COMPONENT}
RESFSDIR   = Resources.${COMPONENT}

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
SQUEEZE = squeeze
WIPE    = -wipe
CD	= dir
TOUCH	= stamp

CPFLAGS = ~cfr~v
WFLAGS = ~c~v
SQZFLAGS = -f

#
# Include files
#
INCLUDES=	-IC:

#
# Libraries
#
CLIB		= Clib:o.Stubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym

DEPEND		= -depend !Depend
ASFLAGS		= ${DEPEND} ${THROWBACK}
CFLAGS		= -c -ffah ${INCLUDES} ${DEPEND} ${THROWBACK}
LDFLAGS		= -aif

RAM_OBJS	= o.stripdepnd

#
# Rule patterns
#
.c.o:;		${CC} ${CFLAGS} -o $@ $<
.s.o:;		${AS} ${ASFLAGS} -o $@ $<

#
# Main rules:
#
#
all: ${RAM_MODULE} o.dirs
        @echo ${COMPONENT}: Application built

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} aif ${WFLAGS}
        @echo ${COMPONENT}: cleaned

o.dirs:
	${MKDIR} o
	create $@

export:
        @echo ${COMPONENT}: export complete

#
# Targets
#
${RAM_MODULE}:	${RAM_OBJS} ${CLIB}
	${MKDIR} aif
	${LD} ${LDFLAGS} -o $@ ${RAM_OBJS} ${CLIB}
	${SQUEEZE} ${SQZFLAGS} $@
        Access $@ RW/R


# Dynamic dependencies:
