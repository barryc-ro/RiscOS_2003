# Makefile for IPConfig
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 07-Oct-98  SNB          Created

COMPONENT = IPConfig
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

CPFLAGS = ~cfr~v
WFLAGS = ~c~v

#
# Include files
#
INCLUDES=	-ITCPIPLibs:,C:

#
# Libraries
#
CLIB		= Clib:o.Stubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym
INETLIB		= TCPIPLibs:o.inetlibzm
SOCKLIB		= TCPIPLibs:o.socklibzm

DEPEND		= -depend !Depend
DBGFLAGS        = -DTRACE
DEFINES         = -DUSECMOS -DCOMPAT_INET4
CENV		= ${DEPEND} ${THROWBACK}

ASFLAGS		= ${DEPEND} ${THROWBACK}
CFLAGS		= -c -ffah -zM -zps1 -Wp ${INCLUDES} ${DEFINES} ${CENV}
CMHGFLAGS	= ${DEPEND} ${THROWBACK} -p

ROM_OBJS	= o.iicpoke or.module or.ifrconfig or.editconfig or.utils   \
		  or.lineedit or.Strncmp_ci o.ipconfhdr
RAM_OBJS	= o.iicpoke o.module  o.ifrconfig  o.editconfig  o.utils   \
		  o.lineedit  o.Strncmp_ci  o.ipconfhdr  ${RESF}
DBG_OBJS	= o.iicpoke od.module od.ifrconfig od.editconfig od.utils  \
		  od.lineedit od.Strncmp_ci od.ipconfhdr ${RESF}

RESF		= o.msgs

#
# Rule patterns
#
.SUFFIXES:	.o .od .or .s .c .cmhg .h
.c.o:;		${CC} ${CFLAGS} -o $@ $<
.c.or:;		${CC} ${CFLAGS} -DROM -o $@ $<
.c.od:;		${CC} ${CFLAGS} ${DBGFLAGS} -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} -o $@ $< -d h.IPConfHdr
.cmhg.h:;	${CMHG} ${CMHGFLAGS} -d $@ $<
.cmhg.od:;	${CMHG} ${CMHGFLAGS} -o $@ $< -d h.IPConfHdrD ${DBGFLAGS}
.s.o:;		${AS} ${ASFLAGS} -o $@ $<

IPConfHdr.h: o.IPConfHdr
	${TOUCH} $@
	
IPConfHdrD.h: od.IPConfHdr
	${TOUCH} $@
	
o.IPConfHdr: VersionNum

od.IPConfHdr: VersionNum

#
# Main rules:
#
#
all: ${RAM_MODULE} o.dirs
        @echo ${COMPONENT}: Module built (RAM)

ramtrace: ${DBG_MODULE} o.dirs
	@echo ${COMPONENT}: Module built (TRACE)

rom: ${ROM_MODULE} o.dirs
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources: LocalRes:Messages
        TokenCheck LocalRes:Messages
        ${MKDIR} ${RESDIR}
        ${CP} LocalRes:Messages ${RESDIR}.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} or ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} h.IPConfHdr* ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        @echo ${COMPONENT}: cleaned

o.dirs:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} or
	create $@

export:
        @echo ${COMPONENT}: export complete


#
# Targets
#
${RAM_MODULE}:	${RAM_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
	${MKDIR} rm
	$(LD) -o $@ -rmf ${RAM_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        ${MODSQZ} $@
        Access $@ RW/R

${ROM_MODULE}:	${ROM_OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}
	${MKDIR} aof
	$(LD) -o $@ -aof ${ROM_OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}

${DBG_MODULE}:	${DBG_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
	${MKDIR} rm
	$(LD) -o $@ -rmf ${DBG_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        Access $@ RW/R

${RESF}: LocalRes:Messages
	TokenCheck LocalRes:Messages
	resgen messages_file ${RESF} LocalRes:Messages ${RESFSDIR}.Messages

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


# Dynamic dependencies:
