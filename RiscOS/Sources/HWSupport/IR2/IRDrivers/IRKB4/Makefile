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




# Dynamic dependencies:
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.utils
o.module:	h.ir_api
o.module:	h.module
o.module:	h.keyboard
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.utils
o.module:	h.ir_api
o.module:	h.module
o.module:	h.keyboard
o.driver:	c.driver
o.driver:	C:h.kernel
o.driver:	C:h.swis
o.driver:	h.utils
o.driver:	h.ir_api
o.driver:	h.veneers
o.driver:	h.irqs
o.driver:	h.keyboard
o.driver:	h.module
o.ir_api:	c.ir_api
o.ir_api:	C:h.kernel
o.ir_api:	C:h.swis
o.ir_api:	h.utils
o.ir_api:	h.ir_api
o.ir_api:	h.veneers
o.keyboard:	c.keyboard
o.keyboard:	C:h.kernel
o.keyboard:	C:h.swis
o.keyboard:	h.utils
o.keyboard:	h.ir_api
o.keyboard:	h.veneers
o.keyboard:	h.irqs
o.keyboard:	h.keyboard
o.keyboard:	h.module
o.keyboard:	c.keyboard
o.keyboard:	C:h.kernel
o.keyboard:	C:h.swis
o.keyboard:	h.utils
o.keyboard:	h.ir_api
o.keyboard:	h.veneers
o.keyboard:	h.irqs
o.keyboard:	h.keyboard
o.keyboard:	h.module
