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
# Setup flags and linker base options
#



# Global Header Files required 
GLOBAL_HDR_FILES = sys_def.h acu_pro.h acu_mem.h acu_def.h \
                   mt_pro.h mt_codef.h mt_hwdef.h mt_hwpro.h \
                   mt_copro.h as_pro.h pp_def.h pp_mem.h \
                   wl_pro.h wl_def.h wl_mem.h mt_coext.h eeprom.h \
                   mod_def.h v34_rate.h par_def.h vcv_cpro.h vcv_mem.h \
                   fab.h resp_cod.h dsp_cod.h ptt.h ee_def.h

# Platform I/O
IOP_V = ac00ptt.o ptt00cfg.o dte.o acu.o dte_drv.o linelink.o lineasyn.o \
     linematc.o linedat.o lineint.o linecrc.o linesync.o \
     linelapm.o linedetc.o line_hw.o dsp_drv.o acuiomem.o \
     faxhdlc.o faxdata.o faxlink.o faxdat.o \
     riscosasm.o voc_ctls.o \
     musket.o linev8.o modulehdr.o riscos.o \
     ringint.o callerid.o dsppatch.o patch_l.o vcepatch.o pwronptc.o 






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
DFLAGS  = -DPODULEBASED #-DDEBUG -DAPP
CFLAGS  = -depend !Depend -zps -zM -c -li -I. -Iio -Iv42 -Imt_ctls1 $(DFLAGS) $(C_DBG_FLAGS)
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

VPATH=ACU AS IO MT_Ctls1 PP Voice
 
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
        ${WIPE} map.* ${WFLAGS}
        ${WIPE} o.* ${WFLAGS}
	${WIPE} Syms $(WFLAGS)
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${TARGET} ${ROMCSTUBS}
	${MKDIR} aof
        ${LD} -aof -o $@ $(LFLAGS) $(ROMCSTUBS)

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM} \
              -map > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


#
#    Musket w/ Cirrus Cutlass I data pump for EMULATOR No wireless code
#
${TARGET}:    $(GLOBAL_HDR_FILES) $(TARGET_OBJS_P_V) $(STARTUP) $(PAR_OBJ) linktop
	    $(MKDIR) o
	    ${MKDIR} rm
            amu -f acumf/arm hdrs acucore !etc !v25bis !wireless !voice fax !voiceview
            amu -f faxmf/arm hdrs fx1
            amu -f v42mf/arm hdrs v42code btlzcode etcstub m10stub cl5code
            amu -f mtmf/arm hdrs cutlass_1
            $(LD) -rmf -o $@ $(LFLAGS) $(LB)
	    $(MODSQZ) $@



linktop:;         @echo Making it

hdrs: $(GLOBAL_HDR_FILES)


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
               CMHG -o o.modulehdr io/modulehdr.cmhg

modulehdr.h:  io/modulehdr.cmhg
               CMHG -d h.modulehdr io/modulehdr.cmhg




# Dynamic dependencies:
o.vcvhdlc:	IO.c.vcvhdlc
o.vcvhdlc:	h.sys_def
o.vcvhdlc:	IO.h.iohw_equ
o.vcvhdlc:	IO.h.lineequ
o.vcvhdlc:	h.vcv_def
o.vcvhdlc:	IO.h.vcvedf
o.vcvhdlc:	IO.h.linelink
o.vcvhdlc:	h.local
o.vcvhdlc:	mt_ctls1.h.dsp_drv
o.vcvlink:	IO.c.vcvlink
o.vcvlink:	h.sys_def
o.vcvlink:	IO.h.iohw_equ
o.vcvlink:	IO.h.lineequ
o.vcvlink:	h.vcv_def
o.vcvlink:	IO.h.vcvedf
o.vcvlink:	h.local
o.vcvlink:	h.icd_msgs
o.vcvlink:	mt_ctls1.h.dsp_drv
o.vcvlink:	h.mt_codef
o.vcvlink:	IO.h.linelink
o.vcvdat:	IO.c.vcvdat
o.vcvdat:	h.sys_def
o.vcvdat:	h.vcv_def
o.pkttrace:	IO.c.pkttrace
o.pkttrace:	h.icd_msgs
o.ringint:	IO.c.ringint
o.ringint:	h.sys_def
o.ringint:	h.acu_def
o.ringint:	IO.h.callerid
o.ringint:	h.ptt
o.callerid:	IO.c.callerid
o.callerid:	IO.h.callerid
o.callerid:	h.sys_def
o.callerid:	h.acu_def
o.callerid:	IO.h.iohw_equ
o.callerid:	mt_ctls1.h.mt_macro
o.dsppatch:	IO.c.dsppatch
o.dsppatch:	h.sys_def
o.dsppatch:	h.mt_pro
o.dsppatch:	mt_ctls1.h.cp_dsp
o.dsppatch:	h.fab
o.dsppatch:	h.dsp_cod
o.dsppatch:	mt_ctls1.h.mt_macro
o.dsppatch:	h.mt_codef
o.patch_l:	IO.c.patch_l
o.vcepatch:	IO.c.vcepatch
o.pwronptc:	IO.c.pwronptc
o.pp00stb:	PP.c.pp00stb
o.pp00stb:	h.sys_def
o.as_stub:	AS.c.as_stub
o.as_stub:	H.SYS_DEF
o.ac00ptt:	ACU.c.ac00ptt
o.ac00ptt:	h.sys_def
o.ac00ptt:	h.acu_def
o.ac00ptt:	h.acu_mem
o.ac00ptt:	h.mt_pro
o.ac00ptt:	h.acu_pro
o.ac00ptt:	h.ee_def
o.ac00ptt:	h.ptt
o.ptt00cfg:	ACU.c.ptt00cfg
o.ptt00cfg:	h.sys_def
o.ptt00cfg:	h.acu_def
o.ptt00cfg:	h.acu_mem
o.ptt00cfg:	h.mt_pro
o.ptt00cfg:	h.ee_def
o.ptt00cfg:	h.ptt
o.dte:	IO.c.dte
o.dte:	h.sys_def
o.dte:	IO.h.acuiomem
o.dte:	h.acu_def
o.dte:	h.acu_mem
o.dte:	IO.h.dteequ
o.dte:	v42.edf.dte
o.dte:	h.par_def
o.dte:	h.riscos
o.dte:	C:h.swis
o.dte:	C:h.kernel
o.acu:	IO.c.acu
o.acu:	h.mod_def
o.acu:	h.sys_def
o.acu:	IO.h.acuiomem
o.acu:	h.acu_def
o.acu:	IO.h.dte_drv
o.acu:	h.acu_mem
o.acu:	IO.h.dteequ
o.acu:	mt_ctls1.h.dsp_drv
o.dte_drv:	IO.c.dte_drv
o.dte_drv:	h.mod_def
o.dte_drv:	h.sys_def
o.dte_drv:	IO.h.dte_drv
o.dte_drv:	IO.h.dteequ
o.dte_drv:	h.acu_def
o.dte_drv:	h.acu_mem
o.dte_drv:	IO.h.acuiomem
o.linelink:	IO.c.linelink
o.linelink:	h.sys_def
o.linelink:	IO.h.iohw_equ
o.linelink:	IO.h.lineequ
o.linelink:	IO.h.lineedf
o.linelink:	mt_ctls1.h.dsp_drv
o.linelink:	mt_ctls1.h.mt_macro
o.linelink:	h.mt_codef
o.linelink:	h.mt_coext
o.linelink:	mt_ctls1.h.cp_dsp
o.linelink:	mt_ctls1.h.mt_v8v34
o.linelink:	h.sys_def
o.lineasyn:	IO.c.lineasyn
o.lineasyn:	h.sys_def
o.lineasyn:	IO.h.lineequ
o.lineasyn:	IO.h.lineedf
o.lineasyn:	IO.h.iohw_equ
o.lineasyn:	IO.h.linelink
o.lineasyn:	h.local
o.lineasyn:	mt_ctls1.h.dsp_drv
o.linematc:	IO.c.linematc
o.linematc:	h.sys_def
o.linematc:	IO.h.lineequ
o.linematc:	IO.h.lineedf
o.linematc:	IO.h.iohw_equ
o.linematc:	IO.h.linelink
o.linematc:	mt_ctls1.h.dsp_drv
o.linematc:	h.local
o.linematc:	mt_ctls1.h.cp_dsp
o.linematc:	h.mt_codef
o.linematc:	mt_ctls1.h.mt_macro
o.linedat:	IO.c.linedat
o.linedat:	h.sys_def
o.linedat:	IO.h.lineequ
o.lineint:	IO.c.lineint
o.lineint:	h.sys_def
o.lineint:	IO.h.lineequ
o.lineint:	IO.h.lineedf
o.lineint:	IO.h.faxequ
o.lineint:	IO.h.faxedf
o.lineint:	IO.h.iohw_equ
o.lineint:	mt_ctls1.h.dsp_drv
o.lineint:	v42.edf.dce_line
o.lineint:	v42.edf.dte
o.lineint:	mt_ctls1.h.mt_v8v34
o.lineint:	h.sys_def
o.lineint:	h.mt_coext
o.lineint:	IO.h.dteequ
o.linecrc:	IO.c.linecrc
o.linecrc:	h.sys_def
o.linesync:	IO.c.linesync
o.linesync:	h.sys_def
o.linesync:	IO.h.iohw_equ
o.linesync:	IO.h.lineequ
o.linesync:	IO.h.lineedf
o.linesync:	IO.h.linelink
o.linesync:	h.local
o.linesync:	mt_ctls1.h.dsp_drv
o.linelapm:	IO.c.linelapm
o.linelapm:	h.sys_def
o.linelapm:	IO.h.iohw_equ
o.linelapm:	IO.h.lineequ
o.linelapm:	IO.h.lineedf
o.linelapm:	IO.h.linelink
o.linelapm:	h.local
o.linelapm:	mt_ctls1.h.dsp_drv
o.linedetc:	IO.c.linedetc
o.linedetc:	h.sys_def
o.linedetc:	IO.h.lineequ
o.linedetc:	IO.h.lineedf
o.linedetc:	IO.h.iohw_equ
o.linedetc:	h.local
o.linedetc:	mt_ctls1.h.dsp_drv
o.linedetc:	h.icd_msgs
o.linedetc:	h.mt_codef
o.line_hw:	IO.c.line_hw
o.dsp_drv:	MT_Ctls1.c.dsp_drv
o.dsp_drv:	h.sys_def
o.dsp_drv:	MT_Ctls1.h.dsp_drv
o.dsp_drv:	h.mt_coext
o.dsp_drv:	h.mt_codef
o.dsp_drv:	h.mt_copro
o.dsp_drv:	h.local
o.dsp_drv:	h.icd_msgs
o.dsp_drv:	h.mt_codef
o.acuiomem:	IO.c.acuiomem
o.acuiomem:	h.sys_def
o.acuiomem:	IO.h.dteequ
o.faxhdlc:	IO.c.faxhdlc
o.faxhdlc:	h.sys_def
o.faxhdlc:	IO.h.iohw_equ
o.faxhdlc:	IO.h.lineequ
o.faxhdlc:	IO.h.faxequ
o.faxhdlc:	IO.h.faxedf
o.faxhdlc:	IO.h.linelink
o.faxhdlc:	h.local
o.faxhdlc:	mt_ctls1.h.dsp_drv
o.faxdata:	IO.c.faxdata
o.faxdata:	h.sys_def
o.faxdata:	IO.h.iohw_equ
o.faxdata:	IO.h.lineequ
o.faxdata:	IO.h.faxequ
o.faxdata:	IO.h.faxedf
o.faxdata:	IO.h.linelink
o.faxdata:	h.local
o.faxdata:	mt_ctls1.h.dsp_drv
o.faxlink:	IO.c.faxlink
o.faxlink:	h.sys_def
o.faxlink:	IO.h.iohw_equ
o.faxlink:	IO.h.lineequ
o.faxlink:	IO.h.faxequ
o.faxlink:	IO.h.faxedf
o.faxlink:	h.local
o.faxlink:	h.icd_msgs
o.faxlink:	mt_ctls1.h.dsp_drv
o.faxlink:	h.mt_codef
o.faxlink:	IO.h.linelink
o.faxdat:	IO.c.faxdat
o.faxdat:	h.sys_def
o.faxdat:	IO.h.faxequ
o.voc_ctls:	Voice.c.voc_ctls
o.voc_ctls:	h.sys_def
o.voc_ctls:	h.dv_def
o.voc_ctls:	h.dv_mem
o.voc_ctls:	h.dv_pro
o.voc_ctls:	Voice.^.io.h.linepro
o.voc_ctls:	h.dsp_cod
o.voc_ctls:	h.vce_def
o.voc_ctls:	Voice.^.mt_ctls1.h.cp_dsp
o.voc_ctls:	Voice.^.mt_ctls1.h.mt_macro
o.voc_ctls:	h.vce_cpro
o.voc_ctls:	Voice.h.vc00mem
o.voc_ctls:	Voice.h.voc_icd
o.voc_ctls:	Voice.h.func_mac
o.voc_ctls:	h.fab
o.musket:	MT_Ctls1.c.musket
o.musket:	h.mod_def
o.musket:	H.SYS_DEF
o.musket:	MT_Ctls1.h.cp_dsp
o.musket:	H.MT_CODEF
o.musket:	H.MT_HWDEF
o.musket:	H.MT_COPRO
o.musket:	H.MT_HWPRO
o.musket:	H.MT_COEXT
o.musket:	MT_Ctls1.h.mt_macro
o.musket:	io.h.dte_drv
o.musket:	h.tracemac
o.musket:	h.resp_cod
o.musket:	h.mod_def
o.musket:	h.fab
o.musket:	h.dsp_cod
o.musket:	h.acu_pro
o.musket:	io.h.lineequ
o.musket:	MT_Ctls1.h.mt_gpio
o.musket:	h.ee_def
o.musket:	MT_Ctls1.h.mt_voice
o.musket:	MT_Ctls1.h.dsp_drv
o.musket:	h.riscos
o.musket:	C:h.swis
o.musket:	C:h.kernel
o.linev8:	IO.c.linev8
o.linev8:	h.sys_def
o.linev8:	IO.h.lineequ
o.linev8:	IO.h.lineedf
o.linev8:	IO.h.iohw_equ
o.linev8:	h.local
o.linev8:	mt_ctls1.h.dsp_drv
o.linev8:	h.mt_coext
o.linev8:	mt_ctls1.h.mt_v8v34
o.linev8:	h.sys_def
o.riscos:	IO.c.riscos
o.riscos:	C:h.kernel
o.riscos:	C:h.swis
o.riscos:	C:h.Podule
o.riscos:	h.sys_def
o.riscos:	h.acu_def
o.riscos:	h.acu_mem
o.riscos:	mt_ctls1.h.mt_voice
o.riscos:	IO.h.iohw_equ
o.riscos:	mt_ctls1.h.dsp_drv
o.riscos:	h.ee_def
o.riscos:	h.riscos
o.riscos:	C:h.swis
o.riscos:	IO.h.tracemac
o.riscos:	h.modulehdr
