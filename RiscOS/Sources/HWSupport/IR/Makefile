# Makefile for IR
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 15-Sep-94  JRH          Created.
#

#
# Generic options:
#
MKDIR   = do mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
MODSQZ  = modsqz
RM      = remove
WIPE    = -wipe

CFLAGS  = -depend !Depend -c -zM -ffa -wp ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Program specific options:
#
COMPONENT = IR
TARGET    = aof.IR

DFLAGS    = -DRCMM

#OBJS      = o.Debug o.Module o.IRiface o.poduleirq
OBJS      = o.Debug o.Module o.IRiface o.asm

#
# Rule patterns
#
.c.o:;      ${CC} ${THROWBACK} ${CFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} -p ${DFLAGS} -d $@ $<
.cmhg.o:;   ${CMHG} -p ${DFLAGS} -o $@ $<
.s.o:;      ${AS} ${THROWBACK} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.IR
	@echo ${COMPONENT}: relocatable module built

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: rm.IR
        ${MKDIR} ${INSTDIR}
        ${CP} rm.IR ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ram module installed

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} h.iriface ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned


#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.IR:  rm.IRu
        ${MODSQZ} rm.IRu $@

rm.IRu: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB}

o.module: h.iriface

# Dynamic dependencies:
