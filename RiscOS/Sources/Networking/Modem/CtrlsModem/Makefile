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
# Setup flags and linker base options
#





.BEFORE .MAKE:
        %if %make(sdbg5)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o

		copy serl_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.vv ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy ext_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h

                %setenv A_DBG_FLAGS =  -g -PD "DEBUG_SUPPORT SETA 1" -PD "SERIAL_ON SETA 1"
                %setenv C_DBG_FLAGS =  -g -errors $$*.err
                #set    LINK_BASE = -d -base 0xa000 -data 0xff0000 -V
                %set LINK_BASE = -d -BASE 0xa000 -DATA 0xff0000  -OV ovlymap  -V 
                %set    TNAME = serdbg5.aof

        %elseif %make(pdbg5)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o

		copy prll_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.vv ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy ext_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h

                %setenv A_DBG_FLAGS =  -g -PD "DEBUG_SUPPORT SETA 1" -PD "SERIAL_ON SETA 0"
                %setenv C_DBG_FLAGS =  -g -errors $$*.err
                #set    LINK_BASE = -d -base 0xa000 -data 0xff0000 -V
                %set LINK_BASE = -d -BASE 0xa000 -DATA 0xff0000  -OV ovlymap  -V 

                %set    TNAME = pardbg5.aof

        %elseif %make(sndb5)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o

		copy serl_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.vv ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy ext_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h

                %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 1"
                %setenv C_DBG_FLAGS = -errors $$*.err
                #set LINK_BASE = -base 0x0000 -bin -data 0xff0000 -V
                %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 
                %set TNAME  = sndb5v.bin

	%elseif %make(pndb5)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o

		copy prll_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.vv ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy ext_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h

                %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 0"
                %setenv C_DBG_FLAGS = -errors $$*.err
                #set LINK_BASE = -base 0x0000 -bin -data 0xff0000 -V
                %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 
                %set TNAME  = pnd5v.bin

        %elseif %make(sndb5i)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o
		del voc_ctls.o

		copy serl_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.vv ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy int_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h

                %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 1"
                %setenv C_DBG_FLAGS = -errors $$*.err
                #set LINK_BASE = -base 0x0000 -bin -data 0xff0000 -V
                %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 
                %set TNAME  = sndb5vi.bin

	%elseif %make(pndb5i)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o
		del voc_ctls.o

		copy prll_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.vv ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy int_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h

                %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 0"
                %setenv C_DBG_FLAGS = -errors $$*.err
                #set LINK_BASE = -base 0x0000 -bin -data 0xff0000 -V
                %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 
                %set TNAME  = pndb5vi.bin

       %elseif %make(sdbg5-v)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o

		copy serl_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.v42 ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy ext_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h

                %setenv A_DBG_FLAGS =  -g -PD "DEBUG_SUPPORT SETA 1" -PD "SERIAL_ON SETA 1"
                %setenv C_DBG_FLAGS =  -g -errors $$*.err
                #set    LINK_BASE = -d -base 0xa000 -data 0xff0000 -V
                %set LINK_BASE = -d -BASE 0xa000 -DATA 0xff0000  -OV ovlymap  -V 
                %set    TNAME = sdb5nvv.aof

        %elseif %make(pdbg5-v)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o

		copy prll_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.v42 ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy ext_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

                %setenv A_DBG_FLAGS =  -g -PD "DEBUG_SUPPORT SETA 1" -PD "SERIAL_ON SETA 0"
                %setenv C_DBG_FLAGS =  -g -errors $$*.err
                #set    LINK_BASE = -d -base 0xa000 -data 0xff0000 -V
                %set LINK_BASE = -d -BASE 0xa000 -DATA 0xff0000  -OV ovlymap  -V 

                %set    TNAME = pdb5nvv.aof

        %elseif %make(sndb5-v)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o

		copy serl_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.v42 ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy ext_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h

                %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0"     -PD "SERIAL_ON SETA 1"
                %setenv C_DBG_FLAGS = -errors $$*.err
                #set LINK_BASE = -base 0x0000 -bin -data 0xff0000 -V
                %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 
                %set TNAME  = sndb5nvv.bin

	%elseif %make(pndb5-v)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o

		copy prll_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.v42 ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy ext_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h
	
                %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 0"
                %setenv C_DBG_FLAGS = -errors $$*.err
                #set LINK_BASE = -base 0x0000 -bin -data 0xff0000 -V
                %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 
                %set TNAME  = pndb5nvv.bin
 


        %elseif %make(sndb5i-v)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o
		#
		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o
		del voc_ctls.o

		copy serl_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.v42 ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy int_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h

                %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 1"
                %setenv C_DBG_FLAGS = -errors $$*.err
                #set LINK_BASE = -base 0x0000 -bin -data 0xff0000 -V
                %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 
                %set TNAME  = sndb5nvi.bin

	%elseif %make(pndb5i-v)
#these are the two file that have the serial and parallel mode switch
		del patch_l.o
		del acu.o
		del dte_drv.o
		del musket.o
		del ac00mstr.o
		del ac00lib2.o

		del powerup.o
		del ac00idat.o

		del ac00anps.o
		del ac00orps.o
		del ac00tsa1.o

		del ac00cmat.o
		del dsppatch.o
		del pwronptc.o
		del voc_ctls.o

		copy prll_mod.h mod_def.h
		copy cut_j5.h fab.h
		copy ovlymap.v42 ovlymap		
		copy io\patch_l5.l io\patch_l.l
		pat2c.exe io\patch_l.l patch
		pat2c.exe io\vcepatch.l vcepatch

		copy int_dspc.h dsp_cod.h
		copy io\pwron_j5.c io\pwronptc.c

		touch io\pwronptc.c
		touch fab.h
		touch mod_def.h
		touch dsp_cod.h


                %setenv A_DBG_FLAGS = -PD "DEBUG_SUPPORT SETA 0" -PD "SERIAL_ON SETA 0"
                %setenv C_DBG_FLAGS = -errors $$*.err
                #set LINK_BASE = -base 0x0000 -bin -data 0xff0000 -V
                %set LINK_BASE =  -BASE 0x0000 -DATA 0xff0000  -bin  -OV ovlymap  -V 
                %set TNAME  = pndb5nvi.bin
 
        %endif


# Global Header Files required 
#added mt_coext.h mt_hwdef.h, mt_hwpro.h, mt_copro.h by ramn on 1/30/96
GLOBAL_HDR_FILES = sys_def.h acu_pro.h acu_mem.h acu_def.h \
		   mt_pro.h mt_codef.h mt_hwdef.h mt_hwpro.h \
		   mt_copro.h as_pro.h pp_def.h pp_mem.h \
		   wl_pro.h wl_def.h wl_mem.h mt_coext.h eeprom.h \
		   mod_def.h v34_rate.h par_def.h vcv_cpro.h vcv_mem.h \
		   fab.h resp_cod.h dsp_cod.h ptt.h ee_def.h

# Platform I/O
IOP =ac00ptt.o ptt00cfg.o dte.o acu.o dte_drv.o linelink.o lineasyn.o \
     linematc.o linedat.o lineint.o linecrc.o linesync.o \
     linelapm.o linedetc.o line_hw.o dsp_drv.o acuiomem.o \
     faxhdlc.o faxdata.o faxlink.o faxdat.o \
     vcvhdlc.o vcvlink.o vcvdat.o \
     rtstand.o sleep.o  parallel.o \
     musket.o linev8.o powerup.o \
     # main_dn.o m_bt_dn.o eeread.o eewrite.o pnp.o pcmcia.o \
     eeread.o eewrite.o pnp.o pcmcia.o\
     overmgr.o ringint.o callerid.o dsppatch.o patch_l.o vcepatch.o pwronptc.o 
     


IOS = ac00ptt.o ptt00cfg.o dte.o acu.o dte_drv.o linelink.o lineasyn.o \
     linematc.o linedat.o lineint.o linecrc.o linesync.o \
     linelapm.o linedetc.o line_hw.o dsp_drv.o acuiomem.o \
     faxhdlc.o faxdata.o faxlink.o faxdat.o \
     vcvhdlc.o vcvlink.o vcvdat.o \
     rtstand.o sleep.o serial.o \
     musket.o linev8.o powerup.o \
     # main_dn.o m_bt_dn.o eeread.o eewrite.o  pnp.o pcmcia.o \
     eeread.o eewrite.o  pnp.o pcmcia.o \
     ringint.o overmgr.o callerid.o dsppatch.o patch_l.o vcepatch.o pwronptc.o 






IOP_V = ac00ptt.o ptt00cfg.o dte.o acu.o dte_drv.o linelink.o lineasyn.o \
     linematc.o linedat.o lineint.o linecrc.o linesync.o \
     linelapm.o linedetc.o line_hw.o dsp_drv.o acuiomem.o \
     faxhdlc.o faxdata.o faxlink.o faxdat.o \
    # vcvhdlc.o vcvlink.o vcvdat.o \
     rtstand.o sleep.o  parallel.o pnp.o pcmcia.o \
     musket.o linev8.o powerup.o \
     # main_dn.o m_bt_dn.o eeread.o eewrite.o \
     eeread.o eewrite.o \
     overmgr.o ringint.o callerid.o dsppatch.o patch_l.o vcepatch.o pwronptc.o 
     

IOS_V = ac00ptt.o ptt00cfg.o dte.o acu.o dte_drv.o linelink.o lineasyn.o \
     linematc.o linedat.o lineint.o linecrc.o linesync.o \
     linelapm.o linedetc.o line_hw.o dsp_drv.o acuiomem.o \
     faxhdlc.o faxdata.o faxlink.o faxdat.o \
    # vcvhdlc.o vcvlink.o vcvdat.o \
     rtstand.o sleep.o serial.o \
     musket.o linev8.o powerup.o \
     # main_dn.o m_bt_dn.o eeread.o eewrite.o  pnp.o pcmcia.o\
     eeread.o eewrite.o  pnp.o pcmcia.o\
     ringint.o overmgr.o callerid.o dsppatch.o patch_l.o vcepatch.o pwronptc.o 






# linetest.o
#       vcvlink.o vcvdata.o vcvhdlc.o vcvdat.o

# Platform Startup Control
	   
# Packet Protocol 
PP    = pp00hdx.o pp00mem.o   pp00io.o
PPSTUB = pp00stb.o

# Hayes Autosync
AUTOSYNC = as00mem.o as00if.o as00lib1.o as00mn.o \
		 as01dte.o as01ri.o as01line.o asio_dat.o
AUTOSYNC_STUB = as_stub.o

# What to build (Platform specific)
TARGET_OBJS_P = $(PPSTUB) $(AUTOSYNC_STUB) $(IOP)
TARGET_OBJS_S = $(PPSTUB) $(AUTOSYNC_STUB) $(IOS)

TARGET_OBJS_P_V = $(PPSTUB) $(AUTOSYNC_STUB) $(IOP_V)
TARGET_OBJS_S_V = $(PPSTUB) $(AUTOSYNC_STUB) $(IOS_V)

#ramn 1/30/96
#PLATFORM_OBJS = $(MODE_OBJ)
#PAR_OBJ = parallel.o							   

# Local Flags
#DFLAGS  = -3/32bit
DFLAGS  = 
CFLAGS  = -APCS 3/NOSWST/NOFP -c -li -I.\ -Iio\ -Iv42\ -Imt_ctls1\ $(DFLAGS) $(C_DBG_FLAGS)
#CFLAGS  = -zps1 -c -li -I.\ -Iio\ -Iv42\ -Imt_ctls1\ $(DFLAGS) $(C_DBG_FLAGS)
AFLAGS  = -LI $(A_DBG_FLAGS)
#LFLAGS  = -map -ERRORS LINK.ERR -o $(TNAME) -via link.cmd 
#LFLAGS  = -map -ERRORS LINK.ERR -o bin  -via link.cmd 
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


#        %setenv TDIR      = c:\arm
##       %setenv TDIR      = h:\arm
#        %setenv CDIR      = $(TDIR)\pc386
#        %setenv COMP_TOOL = $(CDIR)\armcc
#        %setenv ASM_TOOL  = $(CDIR)\armasm
#        %setenv LINK_TOOL = $(CDIR)\armlink

#
#    HELP!
#
help:
	   @@echo - - - - - - - - - MAKEFILE FOR Musket (ARM) PROJECT - - - - - - - - - -
	   @@echo 
	   @@echo Given below is table for building the MUSKET code with various options
	   @@echo 
	   @@echo sdbg5 -- Serial, with Debug, for J5, with Voice view and Ties mode
	   @@echo sdbg6 -- Serial, with Debug, for C6, with Voice view and Ties mode
	   @@echo sndb5 -- Serial, with No Debug, for J5, with Voice view and Ties mode
	   @@echo sndb6 -- Serial, with NO Debug, for C6, with Voice view and Ties mode
	   @@echo sndb5i-- Execute Internal DSP code, Serial, with No Debug, for J5, 
	   @@echo	   with Voice view and Ties mode
	   @@echo 
	   @@echo pdbg5 -- Parallel, with Debug, for J5, with Voice view and Ties mode
	   @@echo pdbg6 -- Parallel, with Debug, for C6, with Voice view and Ties mode
	   @@echo pndb5 -- Parallel, with NO Debug, for J5, with Voice view and Ties mode
	   @@echo pndb6 -- Parallel, with NO Debug, for C6, with Voice view and Ties mode
	   @@echo pndb5i-- Execute Internal DSP code ,Parallel, with NO Debug, for J5, 
	   @@echo	   with Voice view and Ties mode
	   @@echo	
	   @@echo 
	   @@echo GIVEN BELOW ARE MAKE OPTIONS THAT WILL BUILD CODE WITHOUT VOICE VIEW
	   @@echo 
	   @@echo sdbg5-v -- Serial, with Debug, for J5, without Voice view and with Ties mode
	   @@echo sdbg6-v -- Serial, with Debug, for C6, without Voice view and with Ties mode
	   @@echo sndb5-v -- Serial, with No Debug, for J5, without Voice view and with Ties mode
	   @@echo sndb6-v -- Serial, with NO Debug, for C6, without Voice view and with Ties mode
	   @@echo sndb5i-v-- Excute Internal DSP code ,Serial, with No Debug, for J5, 
	   @@echo	     without Voice view and with Ties mode
	   @@echo 
	   @@echo pdbg5-v -- Parallel, with Debug, for J5, without Voice view and with Ties mode
	   @@echo pdbg6-v -- Parallel, with Debug, for C6, without Voice view and with Ties mode
	   @@echo pndb5-v -- Parallel, with NO Debug, for J5, without Voice view and with Ties mode
	   @@echo pndb6-v -- Parallel, with NO Debug, for C6, without Voice view and with Ties mode
	   @@echo pndb5i-v-- Execute Internal DSP code, Parallel, with NO Debug, for J5, 
	   @@echo	     without Voice view and with Ties mode
	   @@echo 

#
#    Musket w/ Cirrus Cutlass I data pump for EMULATOR No wireless code
#
sdbg5:    $(GLOBAL_HDR_FILES) $(TARGET_OBJS_S) $(STARTUP) $(PLATFORM_OBJS) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
            make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_S:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	

pdbg5:    $(GLOBAL_HDR_FILES) $(TARGET_OBJS_P) $(STARTUP) $(PLATFORM_OBJS) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
            make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_P:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	

sndb5i:    $(GLOBAL_HDR_FILES) $(TARGET_OBJS_S) $(STARTUP) $(PLATFORM_OBJS) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
            make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_S:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    copy link.ser link.cmd	
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	

sndb5:    $(GLOBAL_HDR_FILES) $(TARGET_OBJS_S) $(STARTUP) $(PLATFORM_OBJS) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
            make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_S:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	

pndb5i:    $(GLOBAL_HDR_FILES) $(TARGET_OBJS_P) $(STARTUP) $(PAR_OBJ) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
            make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_P:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    copy link.par link.cmd	
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	

pndb5:    $(GLOBAL_HDR_FILES) $(TARGET_OBJS_P) $(STARTUP) $(PAR_OBJ) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
            make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_P:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	

sdbg5-v:  $(GLOBAL_HDR_FILES) $(TARGET_OBJS_S_V) $(STARTUP) $(PLATFORM_OBJS) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
           # make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_S_V:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	

pdbg5-v:  $(GLOBAL_HDR_FILES) $(TARGET_OBJS_P_V) $(STARTUP) $(PLATFORM_OBJS) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
           # make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_P_V:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	

sndb5i-v:   $(GLOBAL_HDR_FILES) $(TARGET_OBJS_S_V) $(STARTUP) $(PLATFORM_OBJS) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
           # make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_S_V:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	
	
sndb5-v:    $(GLOBAL_HDR_FILES) $(TARGET_OBJS_S_V) $(STARTUP) $(PLATFORM_OBJS) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
           # make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_S_V:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	

pndb5i-v:    $(GLOBAL_HDR_FILES) $(TARGET_OBJS_P_V) $(STARTUP) $(PAR_OBJ) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
           # make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_P_V:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	


pndb5-v:    $(GLOBAL_HDR_FILES) $(TARGET_OBJS_P_V) $(STARTUP) $(PAR_OBJ) linktop
	    make -f acumf.arm hdrs acucore !etc !v25bis !wireless voice fax !voiceview
	    make -f faxmf.arm hdrs fx1
	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5code
#	    make -f v42mf.arm hdrs v42code btlzcode etcstub m10stub cl5stub
	    make -f mtmf.arm hdrs cutlass_1
            make -f vcmf.arm hdrs voice
           # make -f vcvmf.arm hdrs vcv
	    @@ echo $(TARGET_OBJS_P_V:S/.*/ &$(RETURN)/) >> link.cmd
	    @@ echo $(PLATFORM_OBJS:S/.*/ &$(RETURN)/) >> link.cmd
	    $(LK) $(LFLAGS) $(LB) > link.map
	    copy bin\root $(TNAME)	


#
#    Link data that comes first
#
linktop:
			 @@ echo $(LINK_BASE) > link.cmd

hdrs: $(GLOBAL_HDR_FILES)

#
#    Include file dependents
#
sys_def.h:     sys02def.h
		copy $< sys_def.h


acu_pro.h:     ac00pro.h
		copy $< acu_pro.h

acu_mem.h:     ac00mem.h
		copy $< acu_mem.h

par_def.h :     pr00def.h 
	   copy $< par_def.h 


acu_def.h:     ac00def.h
		copy $< acu_def.h

mt_pro.h:      mt00pro.h
		copy $< mt_pro.h

mt_codef.h:    mt00cd10.h
		copy $< mt_codef.h

#added by ramn on 1/30/96
mt_hwdef.h:     mt00hd10.h    
		copy $< mt_hwdef.h

mt_copro.h:     mt00cp10.h    
		copy $< mt_copro.h

mt_hwpro.h:     mt00hp10.h    
		copy $< mt_hwpro.h
#added till here by ramn 

as_pro.h:      as00pro.h
	       	 copy $< as_pro.h

wl_pro.h:      wl00pro.h
	       	 copy $< wl_pro.h

wl_def.h:      wl00def.h
	       	 copy $< wl_def.h

wl_mem.h:      wl00mem.h
	       	 copy $< wl_mem.h

pp_def.h:      pp00def.h
	       	 copy $< pp_def.h

pp_mem.h:      pp00mem.h
	         copy $< pp_mem.h

#added by ramn on 1/30/96
mt_coext.h: mt00ce10.h
		copy $< mt_coext.h

eeprom.h: eeprom.h
		copy $< eeprom.h

mod_def.h: mod_def.h
	   copy $< mod_def.h

v34_rate.h: v34_rate.h
		copy $< v34_rate.h

ee_def.h:       ee00def.h
		copy $< ee_def.h

vcv_cpro.h:  vv00cpro.h
	   copy $< vcv_cpro.h

vcv_mem.h:   vv00mem.h 
	   copy $< vcv_mem.h

vcv_def.h:      vv00def.h 
	        copy $< vcv_def.h

fab.h: fab.h
       copy $< fab.h

resp_cod.h: resp_cod.h
	    copy $< resp_cod.h


dsp_cod.h: dsp_cod.h
	   copy $< dsp_cod.h


ptt.h:     ptt00def.h
		copy $< ptt.h




#
# Assembler files
#
parallel.o : io\parallel.s io\parallel.h
		$(AS) $(AFLAGS) $< -o parallel.o

#powerup.o   : io\powerup.s io\pwrup_h.s
powerup.o   : io\powerup.s io\powerup.h
		$(AS) $(AFLAGS) $< -o powerup.o

rtstand.o  : io\rtstand.s 
		$(AS) $(AFLAGS) $< -o rtstand.o

main_dn.o  : io\main_dn.s io\main_dn.h
	   	$(AS) $(AFLAGS) $< -o main_dn.o

m_bt_dn.o  : io\m_bt_dn.s io\m_bt_dn.h
	   	$(AS) $(AFLAGS) $< -o m_bt_dn.o

serial.o   : io\serial.s io\serial.h
		$(AS) $(AFLAGS) $< -o serial.o

cfg_spd.o  : io\cfg_spd.s
		$(AS) $(AFLAGS) $< -o cfg_spd.o

overmgr.o  : io\overmgr.s
		$(AS) $(AFLAGS) $< -o overmgr.o

pnp.o   : io\pnp.s io\pnp.h io\powerup.h
		$(AS) $(AFLAGS) $< -o pnp.o

pcmcia.o   : io\pcmcia.s 
		$(AS) $(AFLAGS) $< -o pcmcia.o


### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

#mod_def.h added by ramn on 2/6/96
acu.o:	 acu_def.h acu_mem.h io\acuiomem.h io\dteequ.h sys_def.h mod_def.h

acuiomem.o: io\dteequ.h sys_def.h

as_stub.o: sys_def.h

bd_rate.o: sys_def.h

#direc.o: acu_def.h acu_mem.h io\acuiomem.h io\dteequ.h io\iohw_equ.h \
	 mt_ctls1\dsp_drv.h sys_def.h v42\dce_line.edf v42\dte.edf

dte.o:	 acu_def.h acu_mem.h io\acuiomem.h io\dteequ.h sys_def.h \
	 v42\dte.edf

#mod_def.h added by ramn on 2/6/96
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

#ramn added resp_cod.h dependency
#mod_def.h added by ramn on 2/6/96
musket.o: io\dte_drv.h mt_ctls1\cp_dsp.h mt_ctls1\mt_macro.h \
	  mt_codef.h mt_coext.h mt_copro.h mt_hwdef.h mt_hwpro.h sys_def.h \
	  tracemac.h resp_cod.h io\lineequ.h ee_def.h mt_ctls1\mt_gpio.h mod_def.h

eeread.o: sys_def.h eeprom.h

eewrite.o: sys_def.h eeprom.h

ringint.o: callerid.h

callerid.o: callerid.h

dsppatch.o: sys_def.h cp_dsp.h mt_pro.h fab.h mt_macro.h mt_codef.h dsp_cod.h

pnp.o: pnp.h powerup.h

pwronptc.o :

vcepatch.o:

ac00ptt.o: ptt.h

ptt00cfg.o: ptt.h

