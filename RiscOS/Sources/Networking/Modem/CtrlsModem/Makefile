#
#     Copyright, 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995.
#     All Rights Reserved by:
#        RSA
#        7701 Six Forks Road
#        Suite 201
#        Raleigh, NC  27615
#        (919) 846-7171
#
#    This document contains material confidential to RSA. Its contents
#    must not be revealed, used or disclosed to anyone or company without
#    written permission by RSA. The information contained herein is solely
#    for the use of RSA.
#
#    EXAMPLE MAKE FILE FOR Musket:
#           - V.32bis Cutlass I
#           - V.42/V.42bis Modem
#           - FAX Class 1, 2
#           - ARM I/O
#
#    (Using OPUS MAKE and Microtec Research Inc. Software Development Tools)
#
#       'makel arm' to create arm.abs
#



#
# Component specific options:
#
COMPONENT  = CtrlsModem
ROM_MODULE = aof.CtrlsModem
TARGET     = rm.CtrlsModem
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym

#
# Setup flags and linker base options
#



# Global Header Files required 
GLOBAL_HDRS = sys_def.h acu_pro.h acu_mem.h acu_def.h       \
     mt_pro.h mt_codef.h mt_hwdef.h mt_hwpro.h              \
     mt_copro.h as_pro.h pp_def.h pp_mem.h                  \
     wl_pro.h wl_def.h wl_mem.h mt_coext.h eeprom.h         \
     mod_def.h v34_rate.h par_def.h vcv_cpro.h vcv_mem.h    \
     resp_cod.h ptt.h ee_def.h

# Platform I/O
IOP_V = ac00ptt.o ptt00cfg.o dte.o acu.o dte_drv.o linelink.o lineasyn.o \
     linematc.o linedat.o lineint.o linecrc.o linesync.o \
     linelapm.o linedetc.o line_hw.o dsp_drv.o acuiomem.o \
     faxhdlc.o faxdata.o faxlink.o faxdat.o \
     riscosasm.o voc_ctls.o \
     musket.o linev8.o modulehdr.o riscos.o \
     linev80.o dte_v80.o print.o \
     ringint.o callerid.o dsppatch.o powerons.o \
     trnpatch.o compatch.o datpatch.o vcepatch.o \
     #pkttrace.o






# Platform Startup Control
           
# Packet Protocol 
PP    = pp00hdx.o pp00mem.o   pp00io.o
PPSTUB = pp00stb.o

# Hayes Autosync
AUTOSYNC = as00mem.o as00if.o as00lib1.o as00mn.o \
                 as01dte.o as01ri.o as01line.o asio_dat.o
AUTOSYNC_STUB = as_stub.o

# What to build (Platform specific)
TARGET_OBJS_P_V = $(PPSTUB) $(AUTOSYNC_STUB) $(IOP_V)

# Local Flags
#DFLAGS  = -3/32bit
DFLAGS  = -DPODULEBASED #-DDEBUG #-DAPP
CFLAGS  = -depend !Depend -ff -zps -zM -c -li -I. -Iio -Iv42 -Imt_ctls1 $(DFLAGS) $(C_DBG_FLAGS) $(OPTS)
#CFLAGS  = -zps1 -c -li -I. -Iio -Iv42 -Imt_ctls1 $(DFLAGS) $(C_DBG_FLAGS)
AFLAGS  = -LI $(A_DBG_FLAGS)
#LFLAGS  = -map -ERRORS LINK.ERR -o $(TNAME) -via link/cmd 
#LFLAGS  = -map -ERRORS LINK.ERR -o bin  -via link/cmd 
LFLAGS  = -via link/cmd -sym Syms

CC      = CC
AS      = ObjAsm
LD      = Link
LB      = C:o.Stubs
MODSQZ  = ModSqz
MKDIR   = cdir
WIPE    = -Wipe
RM      = Remove
CP      = copy

CPFLAGS  = ~cfr~v
WFLAGS = ~cfr~v

VPATH=ACU ACU2 AS IO MT_Ctls1 PP Voice
 
#
# Rule patterns
#
.SUFFIXES: .o .c
.c.o:;	$(CC) $(CFLAGS) -o $@ $<


#
# Main rules:
#
all: ${TARGET}
	@echo ${COMPONENT}: Module built (Relocatable)

rom: ${ROM_MODULE}
	${MKDIR} aof
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources:
        @echo ${COMPONENT}: No resources

clean:
        ${RM} ${ROM_MODULE}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} o.* ${WFLAGS}
	${WIPE} Syms $(WFLAGS)
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${TARGET} ${ROMCSTUBS}
	${MKDIR} aof
        ${LD} -aof -o $@ $(LFLAGS) $(ROMCSTUBS)

# final link for ROM Image (using given base address)
rom_link: ${ROM_MODULE}
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


#
#    Musket w/ Cirrus Cutlass I data pump for EMULATOR No wireless code
#
${TARGET}:    $(GLOBAL_HDRS) $(TARGET_OBJS_P_V) $(STARTUP) $(PAR_OBJ)
	    $(MKDIR) o
	    ${MKDIR} rm
            amu -f acumf/arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
            amu -f faxmf/arm hdrs fx1
            amu -f v42mf/arm hdrs v42code btlzcode etcstub m10stub cl5code
            amu -f mtmf/arm hdrs cutlass_1
	    amu -f vcmf/arm hdrs voice
	    amu -f v80mf/arm v80
            $(LD) -rmf -o $@ $(LFLAGS) $(LB)
	    |$(MODSQZ) $@



linktop:;         @echo Making it

hdrs: $(GLOBAL_HDRS)


#
# Assembler files
#
parallel.o : io/parallel.s io/parallel.h
                $(AS) $(AFLAGS) $< -o parallel.o

#powerup.o   : io/powerup.s io/pwrup_h.s
powerup.o   : io/powerup.s io/powerup.h
                $(AS) $(AFLAGS) $< -o powerup.o

rtstand.o  : io/rtstand.s 
                $(AS) $(AFLAGS) $< -o rtstand.o

main_dn.o  : io/main_dn.s io/main_dn.h
                $(AS) $(AFLAGS) $< -o main_dn.o

m_bt_dn.o  : io/m_bt_dn.s io/m_bt_dn.h
                $(AS) $(AFLAGS) $< -o m_bt_dn.o

serial.o   : io/serial.s io/serial.h
                $(AS) $(AFLAGS) $< -o serial.o

cfg_spd.o  : io/cfg_spd.s
                $(AS) $(AFLAGS) $< -o cfg_spd.o

overmgr.o  : io/overmgr.s
                $(AS) $(AFLAGS) $< -o overmgr.o

pnp.o   : io/pnp.s io/pnp.h io/powerup.h
                $(AS) $(AFLAGS) $< -o pnp.o

pcmcia.o   : io/pcmcia.s 
                $(AS) $(AFLAGS) $< -o pcmcia.o

riscosasm.o   : io/riscosasm.s 
                $(AS) $(AFLAGS) io/riscosasm.s -o riscosasm.o

modulehdr.o:  io/modulehdr.cmhg
               CMHG -p -o o.modulehdr io/modulehdr.cmhg

modulehdr.h:  io/modulehdr.cmhg
               CMHG -p -d h.modulehdr io/modulehdr.cmhg


riscos.o:   modulehdr.h


# Dynamic dependencies:
