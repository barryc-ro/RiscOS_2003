#
#   MUSKET MAKEFILE 
#   ---------------
#
#   Copyright 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997.
#   All Rights Reserved by:
#   Cirrus Logic, Inc.
#

GLOBAL_HDRS = sys_def.h acu_pro.h acu_mem.h acu_def.h       \
     mt_pro.h mt_codef.h mt_hwdef.h mt_hwpro.h              \
     mt_copro.h as_pro.h pp_def.h pp_mem.h                  \
     wl_pro.h wl_def.h wl_mem.h mt_coext.h eeprom.h         \
     mod_def.h v34_rate.h par_def.h vcv_cpro.h vcv_mem.h    \
     resp_cod.h ptt.h ee_def.h

IO = ac00ptt.o ptt00cfg.o dte.o acu.o dte_drv.o linelink.o  \
     lineasyn.o linematc.o linedat.o lineint.o linecrc.o    \
     linesync.o linelapm.o linedetc.o line_hw.o dsp_drv.o   \
     acuiomem.o faxhdlc.o faxdata.o faxlink.o faxdat.o      \
     rtstand.o sleep.o musket.o linev8.o powerup.o          \
     eeread.o eewrite.o pnp.o pcmcia.o overmgr.o ringint.o  \
     callerid.o dsppatch.o powerons.o seramd.o seratmel.o   \
     dlamd.o dlatmel.o linev80.o dte_v80.o print.o          \
     trnpatch.o compatch.o datpatch.o vcepatch.o            \
     bootx.o mdm_56k.o x2_page.o 

IO_S  = serial.o 

IO_P  = parallel.o

IO_VV = vcvhdlc.o vcvlink.o vcvdat.o 

# Packet Protocol 
#
PP     = pp00hdx.o pp00mem.o pp00io.o
PPSTUB = pp00stb.o

# Hayes Autosync
#
AUTOSYNC = as00mem.o as00if.o as00lib1.o as00mn.o \
           as01dte.o as01ri.o as01line.o asio_dat.o
AUTOSYNC_STUB = as_stub.o

# What to build (Platform specific)
#
TARGET_OBJS_P = $(PPSTUB) $(AUTOSYNC_STUB) $(IO) $(IO_P) $(IO_VV)
TARGET_OBJS_S = $(PPSTUB) $(AUTOSYNC_STUB) $(IO) $(IO_S) $(IO_VV)

TARGET_OBJS_P_V = $(PPSTUB) $(AUTOSYNC_STUB) $(IO) $(IO_P)
TARGET_OBJS_S_V = $(PPSTUB) $(AUTOSYNC_STUB) $(IO) $(IO_S)

# Local Flags
#
CFLAGS  = -APCS 3/NOSWST/NOFP -c -li -I.\ -Iio\ -Iv42\ -Imt_ctls1\ $(C_DBG_FLAGS)  
AFLAGS  = -LI $(A_DBG_FLAGS)
LFLAGS  = -map -REMOVE -ERRORS LINK.ERR -o bin -SPLIT  -via link.cmd 

CC      = $(COMP_TOOL)
AS      = $(ASM_TOOL)
LK      = $(LINK_TOOL)
LB      = $(TDIR)\lib\armlib.32l

%setenv TDIR      = c:\arm200
%setenv CDIR      = $(TDIR)\dos
%setenv COMP_TOOL = $(CDIR)\armcc
%setenv ASM_TOOL  = $(CDIR)\armasm
%setenv LINK_TOOL = $(CDIR)\armlink

#
#------------------------------------------------------------------------------
# help section:  "make" with no argument
#

help:
   
   @@echo  MUSKET MAKEFILE
   @@echo  --------------- 
   @@echo   
   @@echo  usage:  make Mxxxx {refer to makefile for available builds}
   @@echo 

#
#------------------------------------------------------------------------------
# makes all of the possible builds and saves results in .err file 
#

all:
   del *.o
   make -x m0010.err m0010
   del *.o
   make -x m0011.err m0011
   del *.o
   make -x m8014.err m8014
   del *.o
   make -x m8015.err m8015
   del *.o
   make -x m0015.err m0015
   del *.o
   make -x m0014.err m0014
   del *.o
   make -x m0019.err m0019
   del *.o
   make -x m001d.err m001d

# ----------------------------------------------------------------------------
# M0010: Serial, voice, no V.80, no VoiceView, TIES, internal DSP code,
#        and X2 resident
#

before_M0010:

   # create new bldflags.h file 
   attrib -r bldflags.h
   %echo \#define PATCHLESS_EXTERNAL       0 >  bldflags.h 
   %echo \#define DNLD_X2_FROM_HOST        0 >> bldflags.h
   %echo \#define DSP_SERIES             408 >> bldflags.h
   %echo \#define MDM_FIRMWARE_FEATURES "$@" >> bldflags.h

   # create new mod_def.h file
   attrib -r mod_def.h
   %echo \#define PARALLEL_MODE  0 >  mod_def.h
   %echo \#define SERIAL_MODE    1 >> mod_def.h
   %echo \#define PCMCIA_SPECIAL 0 >> mod_def.h

   %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 1" -PD "SERIAL_ON SETA 1"
   %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 

M0010: before_M0010 $(GLOBAL_HDRS) $(TARGET_OBJS_S_V) linktop

   %set TNAME  = $@.BIN
   make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
   make -f faxmf.arm hdrs fx1
   make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
   make -f mtmf.arm hdrs cutlass_1
   make -f vcmf.arm hdrs voice
   make -f v80mf.arm v80
   # use hard-coded link.cmd file to arrange objects manually 
   copy linknvv.ser link.cmd
   $(LK) $(LFLAGS) $(LB) > link.map
   copy bin\root $(TNAME)    
   %echo 
   %echo   Serial, voice, no V.80, no VoiceView, TIES, internal DSP code,
   %echo   and X2 resident(4M)
   %echo
 
# ----------------------------------------------------------------------------
# M0011: Parallel, voice, no V.80, no VoiceView, TIES, internal DSP code, 
#        and X2 resident(4M)
#

before_M0011:
 
   # create new bldflags.h file 
   attrib -r bldflags.h
   %echo \#define PATCHLESS_EXTERNAL       0 >  bldflags.h 
   %echo \#define DNLD_X2_FROM_HOST        0 >> bldflags.h
   %echo \#define DSP_SERIES             408 >> bldflags.h
   %echo \#define MDM_FIRMWARE_FEATURES "$@" >> bldflags.h

   # create new mod_def.h file
   attrib -r mod_def.h
   %echo \#define PARALLEL_MODE  1 >  mod_def.h
   %echo \#define SERIAL_MODE    0 >> mod_def.h
   %echo \#define PCMCIA_SPECIAL 0 >> mod_def.h
 
   %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 0"
   %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 

M0011: before_M0011 $(GLOBAL_HDRS) $(TARGET_OBJS_P_V) linktop

   %set TNAME  = $@.BIN
   % echo $(TARGET_OBJS_P_V:S/.*/ &$(RETURN)/) >> link.cmd
   make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
   make -f faxmf.arm hdrs fx1
   make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
   make -f mtmf.arm hdrs cutlass_1
   make -f vcmf.arm hdrs voice
   make -f v80mf.arm v80
   # use hard-coded link.cmd to arrange objs manually instead of generated one 
   copy linknvv.par link.cmd
   $(LK) $(LFLAGS) $(LB) > link.map
   copy bin\root $(TNAME)    
   %echo
   %echo   Parallel, voice, no V.80, no VoiceView, TIES, internal DSP code,
   %echo   and X2 resident(4M)
   %echo

# ----------------------------------------------------------------------------
# M0011dbg: Parallel, voice, no V.80, no VoiceView, TIES, internal DSP code, 
#           X2 resident(4M), and ARM debugger version based on M0011
#

before_M0011dbg:
   # create new bldflags.h file 
   attrib -r bldflags.h
   %echo \#define PATCHLESS_EXTERNAL       0 >  bldflags.h 
   %echo \#define DNLD_X2_FROM_HOST        0 >> bldflags.h
   %echo \#define DSP_SERIES             408 >> bldflags.h
   %echo \#define MDM_FIRMWARE_FEATURES "$@" >> bldflags.h

   # create new mod_def.h file
   attrib -r mod_def.h
   %echo \#define PARALLEL_MODE  1 >  mod_def.h
   %echo \#define SERIAL_MODE    0 >> mod_def.h
   %echo \#define PCMCIA_SPECIAL 0 >> mod_def.h
 
    %setenv A_DBG_FLAGS = -g -PD "DEBUG_SUPPORT SETA 1" -PD "SERIAL_ON SETA 1"
    %setenv C_DBG_FLAGS = -g 
    %set    LINK_BASE   = -d -BASE 0xa000 -DATA 0xff0000  -OV ovlymap  -V 

M0011dbg: before_M0011dbg $(GLOBAL_HDRS) $(TARGET_OBJS_P_V) linktop 

   %set TNAME  = $@.aof
   make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
   make -f faxmf.arm hdrs fx1
   make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
   make -f mtmf.arm hdrs cutlass_1
   make -f vcmf.arm hdrs voice
   make -f v80mf.arm v80
   @@ echo $(TARGET_OBJS_P_V:S/.*/ &$(RETURN)/) >> link.cmd
   $(LK) $(LFLAGS) $(LB) > link.map
   copy bin\root $(TNAME)    

   %echo
   %echo   Parallel, voice, no V.80, no VoiceView, TIES, internal DSP code,
   %echo   X2 resident(4M), and ARM debugger based on M0011 
   %echo


# ----------------------------------------------------------------------------
# M0019: Parallel, voice, no V.80, no VoiceView, TIES, internal DSP code, 
#        and X2 resident(4M), and PCMCIA
#

before_M0019:
   # create new bldflags.h file 
   attrib -r bldflags.h
   %echo \#define PATCHLESS_EXTERNAL       0 >  bldflags.h 
   %echo \#define DNLD_X2_FROM_HOST        0 >> bldflags.h
   %echo \#define DSP_SERIES             408 >> bldflags.h
   %echo \#define MDM_FIRMWARE_FEATURES "$@" >> bldflags.h

   # create new mod_def.h file
   attrib -r mod_def.h
   %echo \#define PARALLEL_MODE  1 >  mod_def.h
   %echo \#define SERIAL_MODE    0 >> mod_def.h
   %echo \#define PCMCIA_SPECIAL 1 >> mod_def.h
 
   %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 0"
   %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 

M0019: before_M0019 $(GLOBAL_HDRS) $(TARGET_OBJS_P_V) linktop

   %set TNAME  = $@.BIN
   % echo $(TARGET_OBJS_P_V:S/.*/ &$(RETURN)/) >> link.cmd
   make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
   make -f faxmf.arm hdrs fx1
   make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
   make -f mtmf.arm hdrs cutlass_1
   make -f vcmf.arm hdrs voice
   make -f v80mf.arm v80
   # use hard-coded link.cmd to arrange objs manually instead of generated one 
   copy linknvv.par link.cmd
   $(LK) $(LFLAGS) $(LB) > link.map
   copy bin\root $(TNAME)    
   %echo
   %echo   Parallel, voice, no V.80, no VoiceView, TIES, internal DSP code,
   %echo   and X2 resident(4M), and PCMCIA
   %echo


# ----------------------------------------------------------------------------
# M001D: Parallel, voice, no V.80, no VoiceView, TIES, internal DSP code, 
#        X2 downloadable(2M), and PCMCIA
#

before_M001D:
 
   # create new bldflags.h file 
   attrib -r bldflags.h
   %echo \#define PATCHLESS_EXTERNAL       0 >  bldflags.h 
   %echo \#define DNLD_X2_FROM_HOST        1 >> bldflags.h
   %echo \#define DSP_SERIES             408 >> bldflags.h
   %echo \#define MDM_FIRMWARE_FEATURES "$@" >> bldflags.h

   # create new mod_def.h file
   attrib -r mod_def.h
   %echo \#define PARALLEL_MODE  1 >  mod_def.h
   %echo \#define SERIAL_MODE    0 >> mod_def.h
   %echo \#define PCMCIA_SPECIAL 1 >> mod_def.h
 
   %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 0"
   %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 

M001D: before_M001D $(GLOBAL_HDRS) $(TARGET_OBJS_P_V) linktop

   %set TNAME  = $@.BIN
   % echo $(TARGET_OBJS_P_V:S/.*/ &$(RETURN)/) >> link.cmd
   make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
   make -f faxmf.arm hdrs fx1
   make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
   make -f mtmf.arm hdrs cutlass_1
   make -f vcmf.arm hdrs voice
   make -f v80mf.arm v80
   # use hard-coded link.cmd to arrange objs manually instead of generated one 
   copy linknvv.par link.cmd
   $(LK) $(LFLAGS) $(LB) > link.map
   copy bin\root $(TNAME)    
   %echo
   %echo   Parallel, voice, no V.80, no VoiceView, TIES, internal DSP code,
   %echo   X2 downloadable(2M), and PCMCIA
   %echo

# ----------------------------------------------------------------------------
# M0015: Parallel, voice, no V.80, no VoiceView, TIES, internal DSP code, 
#        and X2 downloadable
#
# NOTE: for x2 downloadable, still need to manually change
# io/pnp.s line 219-> 00 to 50, line 252 00->50, and line 887 94->44.
# need to automate this process too!
#

before_M0015:
 
   # create new bldflags.h file 
   attrib -r bldflags.h
   %echo \#define PATCHLESS_EXTERNAL       0 >  bldflags.h 
   %echo \#define DNLD_X2_FROM_HOST        1 >> bldflags.h
   %echo \#define DSP_SERIES             408 >> bldflags.h
   %echo \#define MDM_FIRMWARE_FEATURES "$@" >> bldflags.h

   # create new mod_def.h file
   attrib -r mod_def.h
   %echo \#define PARALLEL_MODE  1 >  mod_def.h
   %echo \#define SERIAL_MODE    0 >> mod_def.h
   %echo \#define PCMCIA_SPECIAL 0 >> mod_def.h
 
   %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 0"
   %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 

M0015: before_M0015 $(GLOBAL_HDRS) $(TARGET_OBJS_P_V) linktop

   %set TNAME  = $@.BIN
   % echo $(TARGET_OBJS_P_V:S/.*/ &$(RETURN)/) >> link.cmd
   make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
   make -f faxmf.arm hdrs fx1
   make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
   make -f mtmf.arm hdrs cutlass_1
   make -f vcmf.arm hdrs voice
   make -f v80mf.arm v80
   # use hard-coded link.cmd to arrange objs manually instead of generated one 
   copy linknvv.par link.cmd
   $(LK) $(LFLAGS) $(LB) > link.map
   copy bin\root $(TNAME)    
   %echo
   %echo   Parallel, voice, no V.80, no VoiceView, TIES, internal DSP code,
   %echo   and X2 downloadable
   %echo


# ----------------------------------------------------------------------------
# M0014: Serial, voice, no V.80, no VoiceView, TIES, internal DSP code, 
#        and X2 downloadable
#
# NOTE: for x2 downloadable, still need to manually change
# io/pnp.s line 219-> 00 to 50, line 252 00->50, and line 887 94->44.
# need to automate this process too!
#

before_M0014:
 
   # create new bldflags.h file 
   attrib -r bldflags.h
   %echo \#define PATCHLESS_EXTERNAL       0 >  bldflags.h 
   %echo \#define DNLD_X2_FROM_HOST        1 >> bldflags.h
   %echo \#define DSP_SERIES             408 >> bldflags.h
   %echo \#define MDM_FIRMWARE_FEATURES "$@" >> bldflags.h

   # create new mod_def.h file
   attrib -r mod_def.h
   %echo \#define PARALLEL_MODE  0 >  mod_def.h
   %echo \#define SERIAL_MODE    1 >> mod_def.h
   %echo \#define PCMCIA_SPECIAL 0 >> mod_def.h
 
   %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 1" -PD "SERIAL_ON SETA 1"
   %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 

M0014: before_M0014 $(GLOBAL_HDRS) $(TARGET_OBJS_S_V) linktop

   %set TNAME  = $@.BIN
   % echo $(TARGET_OBJS_S_V:S/.*/ &$(RETURN)/) >> link.cmd
   make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
   make -f faxmf.arm hdrs fx1
   make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
   make -f mtmf.arm hdrs cutlass_1
   make -f vcmf.arm hdrs voice
   make -f v80mf.arm v80
   # use hard-coded link.cmd to arrange objs manually instead of generated one 
   copy linknvv.ser link.cmd
   $(LK) $(LFLAGS) $(LB) > link.map
   copy bin\root $(TNAME)    
   %echo
   %echo   Serial, voice, no V.80, no VoiceView, TIES, internal DSP code,
   %echo   and X2 downloadable
   %echo


# ----------------------------------------------------------------------------
# M8015: Parallel, voice, no V.80, no VoiceView, TIES, external patchless
#        DSP code, and X2 downloadable
#

before_M8015:
 
   # create new bldflags.h file 
   attrib -r bldflags.h
   %echo \#define PATCHLESS_EXTERNAL       1 >  bldflags.h 
   %echo \#define DNLD_X2_FROM_HOST        1 >> bldflags.h
   %echo \#define DSP_SERIES             700 >> bldflags.h
   %echo \#define MDM_FIRMWARE_FEATURES "$@" >> bldflags.h

   # create new mod_def.h file
   attrib -r mod_def.h
   %echo \#define PARALLEL_MODE  1 >  mod_def.h
   %echo \#define SERIAL_MODE    0 >> mod_def.h
   %echo \#define PCMCIA_SPECIAL 0 >> mod_def.h
 
   %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 0"
   %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 

M8015: before_M8015 $(GLOBAL_HDRS) $(TARGET_OBJS_P_V) linktop

   %set TNAME  = $@.BIN
   % echo $(TARGET_OBJS_P_V:S/.*/ &$(RETURN)/) >> link.cmd
   make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
   make -f faxmf.arm hdrs fx1
   make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
   make -f mtmf.arm hdrs cutlass_1
   make -f vcmf.arm hdrs voice
   make -f v80mf.arm v80
   # use hard-coded link.cmd to arrange objs manually instead of generated one 
   copy linknvv.par link.cmd
   $(LK) $(LFLAGS) $(LB) > link.map
   copy bin\root $(TNAME)    
   %echo
   %echo   Parallel, voice, no V.80, no VoiceView, TIES, 
   %echo   external patchless DSP code, and X2 downloadable
   %echo

# ----------------------------------------------------------------------------
# M8014: Serial, voice, no V.80, no VoiceView, TIES, external patchless
#        DSP code, and X2 downloadable
#

before_M8014:
 
   # create new bldflags.h file 
   attrib -r bldflags.h
   %echo \#define PATCHLESS_EXTERNAL       1 >  bldflags.h 
   %echo \#define DNLD_X2_FROM_HOST        1 >> bldflags.h
   %echo \#define DSP_SERIES             700 >> bldflags.h
   %echo \#define MDM_FIRMWARE_FEATURES "$@" >> bldflags.h

   # create new mod_def.h file
   attrib -r mod_def.h
   %echo \#define PARALLEL_MODE  0 >  mod_def.h
   %echo \#define SERIAL_MODE    1 >> mod_def.h
   %echo \#define PCMCIA_SPECIAL 0 >> mod_def.h

   %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 1" -PD "SERIAL_ON SETA 1"
   %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 

M8014: before_M8014 $(GLOBAL_HDRS) $(TARGET_OBJS_S_V) linktop

   %set TNAME  = $@.BIN
   % echo $(TARGET_OBJS_S_V:S/.*/ &$(RETURN)/) >> link.cmd
   make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
   make -f faxmf.arm hdrs fx1
   make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
   make -f mtmf.arm hdrs cutlass_1
   make -f vcmf.arm hdrs voice
   make -f v80mf.arm v80
   # use hard-coded link.cmd to arrange objs manually instead of generated one 
   copy linknvv.ser link.cmd
   $(LK) $(LFLAGS) $(LB) > link.map
   copy bin\root $(TNAME)    
   %echo
   %echo   Serial, voice, no V.80, no VoiceView, TIES, 
   %echo   external patchless DSP code, and X2 downloadable
   %echo

#-----------------------------------------------------------------------------

linktop:
   @@ echo $(LINK_BASE) > link.cmd

hdrs: $(GLOBAL_HDRS)

#
# Assembler files
#

parallel.o : io\$*.s io\$*.h
        $(AS) $(AFLAGS) $< -o $@

powerup.o   : io\$*.s io\$*.h
        $(AS) $(AFLAGS) $< -o $@

rtstand.o  : io\$*.s 
        $(AS) $(AFLAGS) $< -o $@

main_dn.o  : io\$*.s io\$*.h
           $(AS) $(AFLAGS) $< -o $@

m_bt_dn.o  : io\$*.s io\$*.h
           $(AS) $(AFLAGS) $< -o $@

serial.o   : io\$*.s io\$*.h
        $(AS) $(AFLAGS) $< -o $@

cfg_spd.o  : io\$*.s
        $(AS) $(AFLAGS) $< -o $@

overmgr.o  : io\$*.s
        $(AS) $(AFLAGS) $< -o $@

pnp.o   : io\$*.s io\$*.h io\powerup.h
        $(AS) $(AFLAGS) $< -o $@

pcmcia.o   : io\$*.s 
        $(AS) $(AFLAGS) $< -o $@

dlatmel.o  : io\$*.s io\$*.h
           $(AS) $(AFLAGS) $< -o $@

dlamd.o  : io\$*.s io\$*.h
           $(AS) $(AFLAGS) $< -o $@

seratmel.o  : io\$*.s io\$*.h
           $(AS) $(AFLAGS) $< -o $@

seramd.o  : io\$*.s io\$*.h
           $(AS) $(AFLAGS) $< -o $@

# DSP PATCHES

mt_ctls1\trnpatch.c: mt_ctls1\trnpatch.l
     %chdir mt_ctls1
     pat2c trnpatch.l training_patch
     %chdir ..

mt_ctls1\datpatch.c: mt_ctls1\datpatch.l
     %chdir mt_ctls1
     pat2c datpatch.l data_patch
     %chdir ..

mt_ctls1\vcepatch.c: mt_ctls1\vcepatch.l
     %chdir mt_ctls1
     pat2c vcepatch.l voice_patch
     %chdir ..

mt_ctls1\compatch.c: mt_ctls1\compatch.l
     %chdir mt_ctls1
     pat2c compatch.l common_patch
     %chdir ..

mt_ctls1\powerons.c: mt_ctls1\powerons.l
     %chdir mt_ctls1
     pat2c powerons.l pwr_on_patch
     %chdir ..

trnpatch.o: mt_ctls1\trnpatch.c

compatch.o: mt_ctls1\compatch.c

datpatch.o: mt_ctls1\datpatch.c

vcepatch.o: mt_ctls1\vcepatch.c

powerons.o: mt_ctls1\powerons.c

io\x2_page.l: io\x2_page.zip
           %chdir io
           pkunzip x2_page
           touch x2_page.l
           %chdir ..

io\x2_page.c: io\x2_page.l
     %chdir io
     patx2 x2_page.l x2_page_array
     del ffile
     %chdir ..

x2_page.o: io\x2_page.c

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

acu.o:     acu_def.h acu_mem.h io\acuiomem.h io\dteequ.h sys_def.h mod_def.h

acuiomem.o: io\dteequ.h sys_def.h

as_stub.o: sys_def.h

bd_rate.o: sys_def.h

dte.o: acu_def.h acu_mem.h io\acuiomem.h io\dteequ.h sys_def.h v42\dte.edf

dte_drv.o: io\dte_drv.h sys_def.h mod_def.h

dte_stub.o: sys_def.h

faxdat.o: io\faxequ.h sys_def.h

faxdata.o: io\faxedf.h io\faxequ.h io\iohw_equ.h io\lineequ.h io\linelink.h \
     local.h mt_ctls1\dsp_drv.h sys_def.h

faxhdlc.o: io\faxedf.h io\faxequ.h io\iohw_equ.h io\lineequ.h io\linelink.h \
     local.h mt_ctls1\dsp_drv.h sys_def.h

faxlink.o: io\faxedf.h io\faxequ.h io\iohw_equ.h io\lineequ.h io\linelink.h \
     local.h mt_ctls1\dsp_drv.h icd_msgs.h mt_codef.h sys_def.h

lineasyn.o: io\iohw_equ.h io\lineedf.h io\lineequ.h io\linelink.h local.h \
     mt_ctls1\dsp_drv.h sys_def.h

linecrc.o: sys_def.h

linedat.o: io\lineequ.h sys_def.h

linedetc.o: io\iohw_equ.h io\lineedf.h io\lineequ.h local.h mt_ctls1\dsp_drv.h \
     icd_msgs.h mt_codef.h sys_def.h

lineint.o: io\faxedf.h io\faxequ.h io\iohw_equ.h io\lineedf.h io\lineequ.h \
     mt_ctls1\dsp_drv.h mt_ctls1\mt_v8v34.h mt_coext.h sys_def.h v42\dce_line.edf \
     v42\dte.edf

linelapm.o: io\iohw_equ.h io\lineedf.h io\lineequ.h io\linelink.h local.h \
     mt_ctls1\dsp_drv.h sys_def.h

linelink.o: io\iohw_equ.h io\lineedf.h io\lineequ.h mt_ctls1\cp_dsp.h \
     mt_ctls1\dsp_drv.h mt_ctls1\mt_macro.h mt_ctls1\mt_v8v34.h mt_codef.h mt_coext.h \
     sys_def.h

linematc.o: io\iohw_equ.h io\lineedf.h io\lineequ.h io\linelink.h local.h \
     mt_ctls1\cp_dsp.h mt_ctls1\dsp_drv.h mt_ctls1\mt_macro.h mt_codef.h sys_def.h

linesync.o: io\iohw_equ.h io\lineedf.h io\lineequ.h io\linelink.h local.h \
     mt_ctls1\dsp_drv.h sys_def.h

pp00stb.o: sys_def.h

sleep.o: sleep_if.h  acu_def.h acu_mem.h acu_pro.h sys_def.h

vcvdat.o: sys_def.h vcv_def.h

vcvdata.o: io\lineedf.h io\lineequ.h io\linepro.h io\vcvedf.h \ 
           sys_def.h vcv_def.h

vcvhdlc.o: io\lineedf.h io\lineequ.h io\linepro.h io\vcvedf.h sys_def.h \
     vcv_def.h

vcvlink.o: io\lineedf.h io\lineequ.h io\linepro.h io\vcvedf.h sys_def.h \
     vcv_def.h

musket.o: io\dte_drv.h mt_ctls1\cp_dsp.h mt_ctls1\mt_macro.h \
      mt_codef.h mt_coext.h mt_copro.h mt_hwdef.h mt_hwpro.h sys_def.h \
      tracemac.h resp_cod.h io\lineequ.h ee_def.h mt_ctls1\mt_gpio.h mod_def.h

eeread.o: sys_def.h eeprom.h

eewrite.o: sys_def.h eeprom.h

ringint.o: callerid.h

callerid.o: callerid.h

pnp.o: pnp.h powerup.h

ac00ptt.o: ptt.h

ptt00cfg.o: ptt.h

linev80.o: sys_def.h iohw_equ.h lineequ.h lineedf.h linelink.h \
       dsp_drv.h mt_macro.h mt_codef.h mt_coext.h cp_dsp.h  \
           mt_v8v34.h linev80.h

dte_v80.o: sys_def.h acuiomem.h acu_def.h acu_mem.h dteequ.h v42\dte.edf \
           v80_pro.h

print.o: sys_def.h

dsppatch.o: sys_def.h mt_pro.h cp_dsp.h dsp_drv.h mt_macro.h mt_codef.h \
            mt_coext.h dsppatch.h 

bootx.o:

mdm_56k.o:
