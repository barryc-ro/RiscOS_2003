# -*-Text-*-
#
# Copyright (c) 1995 Acorn Computers Ltd., Cambridge, England
#
# Makefile for Freeway module
#
# :RCS Log discontinued:
# Revision 1.3  95/01/09  14:13:24  kwelton
# Added ifswrap.c into the list of C source files, remade dependencies
# (again).
# 
# Revision 1.2  95/01/09  13:40:01  kwelton
# Now use system versions of "kernel.h" and "swis.h", "_swis.h" no longer
# exists - remade dependencies.
# 
# Revision 1.1  95/01/04  19:18:55  kwelton
# Initial revision
# 
#
# Manually adapted for use under RISC iX
#

CC=             cc
AS=             objasm ${ASFLAGS}
LIB=            libfile
RM=             remove
WIPE=           -wipe
CPFLAGS=        ~cfr~v
WFLAGS=         ~c~v

CFLAGS=         -zM -zps1 -ffah -IC: ${DFLAGS}
DFLAGS=         -DLOOKUPERR
ASFLAGS=        -IHdr:Global.,Hdr:Interface. -Stamp -quit

CFILES=         _Args.c _Close.c _File.c _Func.c _Free.c _GBPB.c \
                _GetBytes.c _Open.c _PutBytes.c command.c debug.c \
                esc.c fixsf.c fswrap.c ifswrap.c leaf.c lookup.c \
                noimage.c privw.c readwidth.c stricmp.c strins.c \
                stampinfo.c ticker.c visdelay.c ukerrs.c

FSOBJS=         _Args.o _Close.o _File.o _Func.o _GBPB.o command.o \
                _GetBytes.o _Open.o _PutBytes.o Interface.o _Free.o \
                cstart.o ticker.o dropper.o privw.o

FSOBJS1=        stricmp.o strins.o readwidth.o stampinfo.o esc.o \
                debug.o noimage.o fixsf.o callcnpv.o callinsv.o callmore.o \
                callremv.o visdelay.o fswrap.o lookup.o robase.o asw.o \
                fwupcall.o leaf.o

IMOBJS=         Imterface.o ifswrap.o

#
# Rule patterns
#
.c.o:;      ${CC} -depend !Depend -c ${CFLAGS} -o $@ $<
.s.o:;      ${AS} -depend !Depend ${AFLAGS} $< $@

all:            fslib.o imlib.o ukerrs.o

fslib.o:        ${FSOBJS} ${FSOBJS1}
                ${LIB} -co $@ ${FSOBJS}
                ${LIB} -io $@ ${FSOBJS1}

clean:
        ${WIPE} o.* ${WFLAGS}
#---------------------------------------------------------------------------
# Dynamic dependencies:
