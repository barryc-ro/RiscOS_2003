# Makefile for IRKB4
#

COMPONENT = IRKB4



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
WIPE    = -wipe

CFLAGS  = -depend !Depend ${INCLUDES} -c -zM -ffah ${DFLAGS} -DIR_CLIENT
#DFLAGS  = -DDEBUG=3
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
INCLUDES= -IC:

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
TARGET    = aof.${COMPONENT}
OBJS      = module.o driver.o ir_api.o header.o veneers.o \
            keyboard.o irqs.o


dirs:
        ${MKDIR} o
        ${MKDIR} aof
        ${MKDIR} rm
        ${MKDIR} linked
        ${MKDIR} map

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
	@echo ${COMPONENT}: rom module built

export: ${EXPORTS}
	@echo ${COMPONENT}: export complete

install_rom: ${TARGET}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} rm.* ${WFLAGS}
	${WIPE} linked.* ${WFLAGS}
	${WIPE} map.* ${WFLAGS}
	${RM} ${TARGET}
	@echo ${COMPONENT}: cleaned


#
# Stand-alone module
#
rm.${COMPONENT}: ${OBJS} ${CLIB} ${SOCKLIB} ${INETLIB} dirs
	${LD} -o $@ -module ${OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} dirs
	${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}



#
# Final link for the ROM Image (using given base address)
#
rom_link:
	${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
	truncate map.${COMPONENT} linked.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete



