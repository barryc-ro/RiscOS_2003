# Makefile for unmodsqz
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 03-Nov-98  SNB          Created

COMPONENT = unmodsqz
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
INCLUDES=	-IC: -IC:CLX

#
# Libraries
#
CLIB		= Clib:o.Stubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym
CLXLIB          = C:CLX.o.CLXlib

DEPEND		= -depend !Depend
ASFLAGS		= ${DEPEND} ${THROWBACK}
CFLAGS		= -c -ffah ${INCLUDES} ${DEPEND} ${THROWBACK}
LDFLAGS		= -aif

RAM_OBJS	= o.unsqueezec o.unmodsqz

#
# Rule patterns
#
.c.o:;		${CC} ${CFLAGS} -DTARGET_IS_RISCOS -o $@ $<
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
	${LD} ${LDFLAGS} -o $@ ${RAM_OBJS} ${CLXLIB} ${CLIB}
	${SQUEEZE} ${SQZFLAGS} $@
        Access $@ RW/R

# Dynamic dependencies:
