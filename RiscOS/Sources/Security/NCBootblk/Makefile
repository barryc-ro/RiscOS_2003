# Makefile for NCBootblock
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 7-Apr-97	AJS	Created
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
# AUTHENTICATE_SIGNATURES will cause NCBootblock to reject inauthentic smartcard data
#

# Tiny stubs, no debugging
CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM ${THROWBACK} -DUSE_TINY -UREMOTE_DEBUG -UENABLE_DEBUGGING \
		-DAUTHENTICATE_SIGNATURES

# Tiny stubs, debugging (may be a bit dodgy)
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM ${THROWBACK} -DUSE_TINY -UREMOTE_DEBUG -DREMOTE_DEBUG -DENABLE_DEBUGGING \
		-DAUTHENTICATE_SIGNATURES

# Shared C stubs, no debugging
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM ${THROWBACK} -UUSE_TINY -UREMOTE_DEBUG -UENABLE_DEBUGGING \
		-DAUTHENTICATE_SIGNATURES
 
# Shared C stubs, debugging
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM ${THROWBACK} -UUSE_TINY -DREMOTE_DEBUG -DENABLE_DEBUGGING \
		-DAUTHENTICATE_SIGNATURES



# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
DBUGLIB   = C:o.DBUGzm 

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
COMPONENT = NCBootBlk
TARGET    = aof.NCBootBlk
TARGETSA  = rm.NCBootblk
EXPORTS   = ${EXP_C_H}.smcard ${EXP_C_H}.DBUG
OBJS      = module.o header.o reg_swis.o reg_events.o enum_ddi.o \
	reg_scbb.o reg_cmds.o reg_status.o reg_utils.o \
	machine.o dynalloc.o reg_auth.o reg_index.o \
	asm_utils.o reg_files.o\
	BORDERDBUG.o kprintf.o \
	reg_sign.o reg_tags.o


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

${TARGETSA}: ${OBJS} ${CLIB} ${DBUGLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${DBUGLIB}

#
# Exported interface headers
#
${EXP_C_H}.smcard:	h.smcard
	${CP} h.smcard $@ ${CPFLAGS}

${EXP_C_H}.DBUG:	h.DBUG
	${CP} h.DBUG $@ ${CPFLAGS}


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
o.module:	h.reg_scbb
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
o.reg_swis:	h.reg_scbb
o.reg_swis:	h.reg_status
o.reg_swis:	h.enum_ddi
o.reg_swis:	h.dci4
o.reg_swis:	h.machine
o.reg_swis:	h.reg_auth
o.reg_swis:	h.reg_files
o.reg_swis:	h.reg_utils
o.reg_swis:	h.reg_index
o.reg_swis:	h.reg_sign
o.reg_swis:	H.DBUG
o.reg_swis:	C:h.scinter
o.reg_swis:	h.smcard
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
o.reg_scbb:	h.reg_scbb
o.reg_scbb:	h.reg_status
o.reg_scbb:	h.reg_index
o.reg_scbb:	h.enum_ddi
o.reg_scbb:	h.dci4
o.reg_scbb:	h.dynalloc
o.reg_scbb:	h.DBUG
o.reg_cmds:	c.reg_cmds
o.reg_cmds:	C:h.swis
o.reg_cmds:	C:h.kernel
o.reg_cmds:	C:h.kernel
o.reg_cmds:	h.kprintf
o.reg_cmds:	h.module
o.reg_cmds:	h.reg_cmds
o.reg_cmds:	h.reg_scbb
o.reg_cmds:	h.enum_ddi
o.reg_cmds:	h.dci4
o.reg_cmds:	h.reg_status
o.reg_cmds:	h.enum_ddi
o.reg_cmds:	h.dci4
o.reg_cmds:	h.machine
o.reg_cmds:	h.reg_auth
o.reg_cmds:	h.reg_files
o.reg_cmds:	h.reg_index
o.reg_cmds:	h.DBUG
o.reg_status:	c.reg_status
o.reg_status:	C:h.kernel
o.reg_status:	C:h.swis
o.reg_status:	h.reg_status
o.reg_status:	h.reg_utils
o.reg_status:	h.DBUG
o.reg_utils:	c.reg_utils
o.reg_utils:	C:h.kernel
o.reg_utils:	C:h.swis
o.machine:	c.machine
o.machine:	C:h.kernel
o.machine:	C:h.swis
o.machine:	h.machine
o.machine:	h.module
o.machine:	h.reg_events
o.machine:	h.reg_status
o.machine:	C:h.scinter
o.machine:	h.reg_scbb
o.machine:	h.reg_auth
o.machine:	h.reg_files
o.machine:	h.reg_index
o.machine:	h.reg_utils
o.machine:	h.reg_sign
o.machine:	h.DBUG
o.dynalloc:	c.dynalloc
o.dynalloc:	C:h.kernel
o.dynalloc:	C:h.swis
o.dynalloc:	h.dynalloc
o.reg_auth:	c.reg_auth
o.reg_auth:	C:h.kernel
o.reg_auth:	C:h.swis
o.reg_auth:	h.machine
o.reg_auth:	h.reg_auth
o.reg_index:	c.reg_index
o.reg_index:	h.reg_index
o.reg_index:	h.reg_scbb
o.reg_index:	h.reg_status
o.reg_index:	h.DBUG
o.reg_index:	h.reg_utils
o.reg_index:	h.smcard
o.asm_utils: s.asm_utils
o.asm_utils: s.registers
o.reg_files:	c.reg_files
o.reg_files:	C:h.kernel
o.reg_files:	C:h.swis
o.reg_files:	h.machine
o.reg_files:	h.reg_files
o.reg_files:	h.reg_auth
o.reg_files:	h.reg_utils
o.reg_files:	h.DBUG
o.BORDERDBUG: s.BORDERDBUG
o.BORDERDBUG: s.registers
o.kprintf:	c.kprintf
o.kprintf:	C:h.kernel
o.kprintf:	C:h.swis
o.kprintf:	h.kprintf
o.reg_sign:	c.reg_sign
o.reg_sign:	C:h.kernel
o.reg_sign:	C:h.swis
o.reg_sign:	h.reg_sign
o.reg_sign:	h.reg_tags
o.reg_sign:	h.smcard
o.reg_sign:	h.DBUG
o.reg_tags:	c.reg_tags
o.reg_tags:	h.reg_tags
o.reg_tags:	h.reg_utils
o.reg_tags:	h.DBUG
