# Makefile for FrontEnd
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 17-Mar-99  SNB          Created

COMPONENT = FrontEnd
RAM_MODULE = rm.${COMPONENT}
ROM_MODULE = aof.${COMPONENT}
DBG_MODULE = rm.${COMPONENT}D

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
WIPE    = -wipe
CD	= dir
TOUCH	= stamp
AWK     = awk

CPFLAGS = ~cfr~v
WFLAGS = ~c~v

#
# Include files
#
INCLUDES=	-IC:

#
# Libraries
#
CLIB		= Clib:o.Stubs
RLIB            = RISC_OSLib:o.riscoslibm
FLEXLIB         = C:tboxlibs.o.flexlibzm
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym

LIBS            = ${FLEXLIB} ${RLIB}

DEPEND          = -depend !Depend ${THROWBACK}
DBGFLAGS        = -DTRACE
DEFINES         = -DTEST_RELEASE -DINCLUDES_AS_INCLUDES

ASFLAGS		= ${DEPEND}
CFLAGS		= -c -ffah -Wc -zM -zps0 ${INCLUDES} ${DEFINES} ${DEPEND}
CMHGFLAGS	= ${DEPEND} -p
LDFLAGS         = -rmf -c++

RAM_OBJS        = o.FEbkg  o.FEcmdline  o.FEdbox  o.FEexit  o.FEfileicon \
                  o.FEhelp  o.FEiconbar  o.FEinfo  o.FEinit  o.FEinterr  o.FEmem \
                  o.FEmenus  o.FEmsg  o.FEoptions  o.FEoutput  o.FEsaveas  o.FEtask \
                  o.FEtxtwin  o.frontend  o.gen  o.lexer  o.parser  o.slist  o.utils \
                  o.base o.FEHdr

ROM_OBJS        = or.FEbkg or.FEcmdline or.FEdbox or.FEexit or.FEfileicon \
                  or.FEhelp or.FEiconbar or.FEinfo or.FEinit or.FEinterr or.FEmem \
                  or.FEmenus or.FEmsg or.FEoptions or.FEoutput or.FEsaveas or.FEtask \
                  or.FEtxtwin or.frontend or.gen or.lexer or.parser or.slist or.utils \
                  o.base o.FEHdr

ROM_OBJS        = od.FEbkg od.FEcmdline od.FEdbox od.FEexit od.FEfileicon \
                  od.FEhelp od.FEiconbar od.FEinfo od.FEinit od.FEinterr od.FEmem \
                  od.FEmenus od.FEmsg od.FEoptions od.FEoutput od.FEsaveas od.FEtask \
                  od.FEtxtwin od.frontend od.gen od.lexer od.parser od.slist od.utils \
                  o.base o.FEHdr


#
# Rule patterns
#
.SUFFIXES:	.o .od .or .s .c .cmhg .h
.c.o:;		${CC} ${CFLAGS} -o $@ $<
.c.or:;		${CC} ${CFLAGS} -DROM -o $@ $<
.c.od:;		${CC} ${CFLAGS} ${DBGFLAGS} -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} -o $@ $< -d h.FEHdr
.cmhg.h:;	${CMHG} ${CMHGFLAGS} -d $@ $<
.cmhg.od:;	${CMHG} ${CMHGFLAGS} -o $@ $<
.s.o:;		${AS} ${ASFLAGS} -o $@ $<

FEHdr.h: o.FEHdr
	${TOUCH} $@
	
o.FEHdr: VersionNum

#
# Main rules:
#
#
all: ${RAM_MODULE} o._dirs
        @echo ${COMPONENT}: Module built (RAM)

ramtrace: ${DBG_MODULE} o._dirs
	@echo ${COMPONENT}: Module built (TRACE)

rom: ${ROM_MODULE} o._dirs
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources:
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} or ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} h.FEHdr ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        @echo ${COMPONENT}: cleaned

o._dirs:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} or
	create $@

export:
        @echo ${COMPONENT}: export complete


#
# Targets
#
${RAM_MODULE}:	${RAM_OBJS} ${CLIB} ${LIBS}
	${MKDIR} rm
	$(LD) -o $@ -rmf ${RAM_OBJS} ${CLIB} ${LIBS}
        ${MODSQZ} $@
        Access $@ RW/R

${ROM_MODULE}:	${ROM_OBJS} ${ROMCSTUBS} ${LIBS}
	${MKDIR} aof
	$(LD) -o $@ -aof ${ROM_OBJS} ${ROMCSTUBS} ${LIBS}

${DBG_MODULE}:	${DBG_OBJS} ${CLIB} ${LIBS}
	${MKDIR} rm
	$(LD) -o $@ ${LDFLAGS} ${DBG_OBJS} ${CLIB} ${LIBS}
        Access $@ RW/R

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


# Dynamic dependencies:
