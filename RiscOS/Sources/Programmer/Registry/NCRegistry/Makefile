# Makefile for NCRegistry
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 9-Aug-96	AJS	Created
#

#
# Paths
#
EXP_HDR = <export$dir>
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

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
CD	= dir
DEFMOD  = DefMod
AR	= LibFile

ASFLAGS = -depend !Depend -Stamp -quit -module

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
INCLUDES= -IC: 

#
# Different compile options - choose one
#

# Tiny stubs, no debugging
CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -DUSE_TINY -UENABLE_DEBUGGING -DINSECURE_SMARTCARD_OPS

# Tiny stubs, debugging (may be a bit dodgy)
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -DUSE_TINY -DENABLE_DEBUGGING -DINSECURE_SMARTCARD_OPS

# Shared C stubs, no debugging
#CFLAGS  = -c -depend !Depend ${INCLUDES} -zM -Throwback -UUSE_TINY -UENABLE_DEBUGGING -DINSECURE_SMARTCARD_OPS

# Shared C stubs, debugging
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -Throwback -UUSE_TINY -DENABLE_DEBUGGING -DINSECURE_SMARTCARD_OPS 



# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

#SOCKLIB	   = InetInc:^.^.libs.socklib.socklibzm
#INETLIB	   = InetInc:^.^.libs.inetlib.inetlibzm

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@

#
# Program specific options:
#
COMPONENT = NCRegistry
TARGET    = aof.NCRegistry
TARGETSA  = rm.NCRegistry
EXPORTS   = ${EXP_C_H}.sc_const ${EXP_C_H}.reg_scbb ${EXP_C_H}.s_card
OBJS      = module.o header.o reg_swis.o reg_events.o enum_ddi.o \
	reg_scbb.o reg_tags.o reg_cmds.o reg_status.o reg_utils.o \
	machine.o DBUG.o reg_spprt.o dynalloc.o reg_auth.o \
	BORDERDBUG.o kprintf.o

#
# Generic rules:
#
all: ${TARGETSA}
        @echo ${COMPONENT}: all complete

rom: ${TARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${TARGETSA}: ${OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}

#
# Exported interface headers
#
${EXP_C_H}.reg_scbb:	h.reg_scbb
	${CP} h.reg_scbb $@ ${CPFLAGS}

${EXP_C_H}.sc_const:	h.sc_const
	${CP} h.sc_const $@ ${CPFLAGS}

${EXP_C_H}.s_card:	h.s_card
	${CP} h.s_card $@ ${CPFLAGS}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete



# Dynamic dependencies:
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.dci4
o.module:	h.machine
o.module:	h.reg_events
o.module:	h.module
o.module:	h.reg_swis
o.module:	h.reg_cmds
o.module:	h.s_card
o.module:	h.sc_const
o.module:	h.reg_scbb
o.module:	h.reg_spprt
o.module:	C:h.regss_defs
o.module:	h.enum_ddi
o.module:	h.dci4
o.module:	h.dynalloc
o.module:	h.DBUG
o.reg_swis:	c.reg_swis
o.reg_swis:	C:h.swis
o.reg_swis:	C:h.kernel
o.reg_swis:	C:h.kernel
o.reg_swis:	h.module
o.reg_swis:	h.reg_swis
o.reg_swis:	h.reg_tags
o.reg_swis:	h.s_card
o.reg_swis:	h.sc_const
o.reg_swis:	C:h.regss_defs
o.reg_swis:	h.reg_scbb
o.reg_swis:	h.reg_status
o.reg_swis:	h.enum_ddi
o.reg_swis:	h.dci4
o.reg_swis:	h.machine
o.reg_swis:	h.reg_spprt
o.reg_swis:	C:h.regss_defs
o.reg_swis:	h.reg_auth
o.reg_swis:	H.DBUG
o.reg_swis:	C:h.scinter
o.reg_events:	c.reg_events
o.reg_events:	C:h.kernel
o.reg_events:	C:h.swis
o.reg_events:	h.machine
o.reg_events:	h.module
o.reg_events:	h.reg_events
o.reg_events:	C:h.scinter
o.enum_ddi:	c.enum_ddi
o.enum_ddi:	C:h.kernel
o.enum_ddi:	C:h.swis
o.enum_ddi:	h.module
o.enum_ddi:	h.enum_ddi
o.enum_ddi:	h.dci4
o.enum_ddi:	h.dynalloc
o.enum_ddi:	H.DBUG
o.enum_ddi:	h.kprintf
o.reg_scbb:	c.reg_scbb
o.reg_scbb:	h.s_card
o.reg_scbb:	h.sc_const
o.reg_scbb:	h.reg_scbb
o.reg_scbb:	h.reg_status
o.reg_scbb:	h.enum_ddi
o.reg_scbb:	h.dci4
o.reg_scbb:	h.DBUG
o.reg_tags:	c.reg_tags
o.reg_tags:	h.s_card
o.reg_tags:	h.sc_const
o.reg_tags:	h.reg_scbb
o.reg_tags:	h.reg_tags
o.reg_tags:	h.s_card
o.reg_tags:	C:h.regss_defs
o.reg_cmds:	c.reg_cmds
o.reg_cmds:	C:h.swis
o.reg_cmds:	C:h.kernel
o.reg_cmds:	C:h.kernel
o.reg_cmds:	h.kprintf
o.reg_cmds:	h.module
o.reg_cmds:	h.reg_cmds
o.reg_cmds:	h.reg_tags
o.reg_cmds:	h.s_card
o.reg_cmds:	h.sc_const
o.reg_cmds:	C:h.regss_defs
o.reg_cmds:	h.reg_scbb
o.reg_cmds:	h.enum_ddi
o.reg_cmds:	h.dci4
o.reg_cmds:	h.reg_status
o.reg_cmds:	h.enum_ddi
o.reg_cmds:	h.dci4
o.reg_cmds:	h.reg_spprt
o.reg_cmds:	C:h.regss_defs
o.reg_cmds:	h.machine
o.reg_status:	c.reg_status
o.reg_status:	C:h.kernel
o.reg_status:	C:h.swis
o.reg_status:	h.reg_status
o.reg_status:	h.reg_utils
o.reg_status:	h.DBUG
o.reg_utils: s.reg_utils
o.reg_utils: s.registers
o.machine:	c.machine
o.machine:	C:h.kernel
o.machine:	C:h.swis
o.machine:	h.machine
o.machine:	h.module
o.machine:	h.reg_events
o.machine:	h.reg_status
o.machine:	C:h.scinter
o.machine:	h.s_card
o.machine:	h.sc_const
o.machine:	h.reg_scbb
o.machine:	h.reg_spprt
o.machine:	C:h.regss_defs
o.machine:	h.reg_auth
o.machine:	h.DBUG
o.DBUG:	c.DBUG
o.DBUG:	H.DBUG
o.reg_spprt:	c.reg_spprt
o.reg_spprt:	C:h.kernel
o.reg_spprt:	C:h.swis
o.reg_spprt:	h.reg_spprt
o.reg_spprt:	C:h.regss_defs
o.reg_spprt:	h.DBUG
o.dynalloc:	c.dynalloc
o.dynalloc:	C:h.kernel
o.dynalloc:	C:h.swis
o.dynalloc:	h.dynalloc
o.reg_auth:	c.reg_auth
o.reg_auth:	C:h.kernel
o.reg_auth:	C:h.swis
o.reg_auth:	h.machine
o.reg_auth:	h.reg_auth
o.BORDERDBUG: s.BORDERDBUG
o.BORDERDBUG: s.registers
o.kprintf:	c.kprintf
o.kprintf:	C:h.kernel
o.kprintf:	C:h.swis
o.kprintf:	h.kprintf
