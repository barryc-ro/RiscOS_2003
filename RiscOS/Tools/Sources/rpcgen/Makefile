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
LIBDIR  = ${INSTDIR}.Library.Acorn
INSTAPP = ${INSTDIR}.Apps.!RPCGen

#
# Generic options:
#
MKDIR   = mkdir -p
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
DFLAGS    = -DRISCOS -DTARGET_IS_RISCOS -Dunlink=remove

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

install_tool: ${TARGET}
	${CP} ${TARGET} ${LIBDIR}.${TARGET} ${CPFLAGS}
	@echo ${COMPONENT}: tool and docs installed in library

install: install_tool install_dirs
	|${CP} LocalRes:!Boot ${INSTAPP}.!Boot ${CPFLAGS}
	|${CP} LocalRes:!Run ${INSTAPP}.!Run ${CPFLAGS}
	|${CP} LocalRes:!Help ${INSTAPP}.!Help ${CPFLAGS}
	|${CP} LocalRes:!Setup ${INSTAPP}.!Setup ${CPFLAGS}
	|${CP} LocalRes:!Sprites ${INSTAPP}.!Sprites ${CPFLAGS}
	|${CP} LocalRes:!Sprites22 ${INSTAPP}.!Sprites22 ${CPFLAGS}
	|${CP} LocalRes:Messages ${INSTAPP}.Messages ${CPFLAGS}
	|${CP} LocalRes:Templates ${INSTAPP}.Templates ${CPFLAGS}
	|${CC} -C++ -E >${INSTAPP}.Desc LocalRes:Desc
	@|echo ${COMPONENT}: FrontEnd app install in '${INSTAPP}'

install_dirs:
	${MKDIR} ${INSTAPP}
	${MKDIR} ${LIBDIR}.Docs

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
