# Makefile for Configure Installer module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 15-Dec-97      BJGA   Created
# 17-Dec-97      BJGA   Tidied up
# 20-Jan-98      BJGA   Added objasm support
# 06-Apr-98      BJGA   Replaced Common directory with ConfigLib
#

#
# Program specific options:
#
COMPONENT  = Installer
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
RAM_MODULE = rm.${COMPONENT}
INSTDIR    = ${INSTDIR}

#
# Generic options:
#
MKDIR   = cdir
CC      = cc
ATTR    = -attr
OBJASM  = objasm
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

CFLAGS   = -c -depend !Depend -ffah -gflv ${INCLUDES} -throwback -zM
INCLUDES = -IC:,<Lib$Dir>.ConfigLib.
OFLAGS   = -depend !Depend -throwback

#
# Libraries
#
CLIB   = CLIB:o.stubs
CONLIB = <Lib$Dir>.ConfigLib.o.ConfigLib

OBJS = CheckSys.o Dispatch.o File.o FileOp.o Housekeep.o Log.o Merge.o Mess.o Modver.o OSVar.o   ResMess.o ModHdr.o

#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} -o $@ $<
.s.o:;    ${OBJASM} ${OFLAGS} -o $@ $<

#
# Main rules:
#
#
all: ${RAM_MODULE}
        @echo ${COMPONENT}: Module built (RAM)

install: ${RAM_MODULE}
        ${MKDIR} ${INSTDIR}
        |
        ${CP} ${RAM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        |
        ${ATTR} -files +or +ow +wr -ww ${INSTDIR}.${COMPONENT}
        |
        @echo ${COMPONENT}: All installed (Disc)

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${RM} h.ModHdr
        @echo ${COMPONENT}: cleaned

${RAM_MODULE}: ${OBJS} ${CONLIB} ${CLIB}
	${MKDIR} rm
        ${LD} -o $@ -module ${OBJS} ${CONLIB} ${CLIB}
        access $@ RW/R

ModHdr.h: cmhg.ModHdr
	${CMHG} -o o.ModHdr cmhg.ModHdr h.ModHdr

ModHdr.o: cmhg.ModHdr
	${CMHG} -o o.ModHdr cmhg.ModHdr h.ModHdr

ResMess.o: ${LDIR}.Messages
	ResGen resourcesfiles o.ResMess ${LDIR}.Messages Resources.Installer.Messages

#---------------------------------------------------------------------------
# Dynamic dependencies:
