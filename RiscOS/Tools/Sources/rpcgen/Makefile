# Makefile for rpcgen
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-Nov-98  SNB          Created.
#

#
# Paths
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
CC      = cc
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
TOUCH   = create
WIPE    = -wipe
SQZ     = squeeze

CCFLAGS = -c -depend !Depend -ffa -Wp ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V
LDFLAGS =
SQZFLAGS= -f

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
CLXLIB    = C:CLX.o.clxlib

#
# Include files
#
INCLUDES  = -ITCPIPLibs:,C:CLX,C:
DFLAGS    = -DRISCOS -Dunlink=remove

#
# Program specific options:
#
COMPONENT  = rpcgen
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 


OBJS=	   o.rpc_main o.rpc_hout o.rpc_cout o.rpc_parse \
      	   o.rpc_scan o.rpc_util o.rpc_svcout o.rpc_clntout \
	   o.rpc_tblout o.rpc_sample

LIBS=	${CLXLIB} ${CLIB}

#
# Rule patterns
#
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<

#
# build the tool:
#
all:	${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${COMPONENT}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

o._dirs:
	${MKDIR} o
	create $@

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} o._dirs
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@
        Access $@ RW/R


# Static dependencies:

# Dynamic dependencies:
