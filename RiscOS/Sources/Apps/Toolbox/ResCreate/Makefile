# Makefile for ResCreate
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 06-Nov-98  SNB    Created
#
.SUFFIXES: .c .o

#
# Component specific options:
#
COMPONENT = ResCreate

EXPDIR = Export
TARGET = aif.!RunImage

#
# Generic options:
#
CHMOD	= access
MKDIR	= cdir
CC	= cc
CMHG	= cmhg
CP	= copy
LD	= link
RM	= remove
SQZ	= squeeze
WIPE	= -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

SQZFLAGS = -f
CFLAGS	 = -c -depend !Depend ${THROWBACK} ${INCLUDES} -ffah ${DFLAGS}
DFLAGS	 = -D${SYSTEM}
INCLUDES = -IC:tboxlibs,C:


# !${COMPONENT} Makefile

LIBS = C:tboxlibs.o.toolboxlib C:tboxlibs.o.eventlib C:tboxlibs.o.wimplib C:o.Stubs

CFILES = alloc.c debug.c error.c main.c message.c dbox.c

OFILES = alloc.o debug.o error.o main.o message.o dbox.o

FILES = LocalRes:!Help LocalRes:!Run LocalRes:!Sprites \
	LocalRes:!Sprites22 LocalRes:Messages LocalRes:Res

#
# Main rules:
#
#
all: ${TARGET} o.dirs
	@echo ${COMPONENT}: !RunImage built

export: install
	${SQZ} ${TARGET} ${EXPDIR}.!${COMPONENT}.!RunImage ${SQZFLAGS}
	Access ${EXPDIR}.!${COMPONENT}.!RunImage RW/R
        @echo *** ${COMPONENT} installation complete

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} aif ${WFLAGS}
	${WIPE} ${EXPDIR} ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${TARGET}: ${OFILES} ${LIBS}
	${mkdir} aif
	${LD} ${LDFLAGS} -o $@ ${OFILES} ${LIBS}
	Access $@ RW/R

.c.o:
        ${CC} $(CFLAGS) -o $@ $<

install: $(FILES) all
	${MKDIR} ${EXPDIR}
        cdir $(EXPDIR).!${COMPONENT}
        copy LocalRes:!Help $(EXPDIR).!${COMPONENT}.!Help $(CPFLAGS)
        copy LocalRes:!Run $(EXPDIR).!${COMPONENT}.!Run $(CPFLAGS)
        copy LocalRes:!Sprites $(EXPDIR).!${COMPONENT}.!Sprites $(CPFLAGS)
        copy LocalRes:!Sprites22 $(EXPDIR).!${COMPONENT}.!Sprites22 $(CPFLAGS)
        copy LocalRes:Res $(EXPDIR).!${COMPONENT}.Res $(CPFLAGS)
        copy LocalRes:Messages $(EXPDIR).!${COMPONENT}.Messages $(CPFLAGS)

o.dirs:
	${MKDIR} o
	create o.dirs

# Dynamic dependencies:
