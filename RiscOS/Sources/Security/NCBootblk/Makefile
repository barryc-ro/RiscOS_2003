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
		-UAUTHENTICATE_SIGNATURES

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
