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
#	MAKE FILE FOR:
#     68302 DECC DCE I/O
#
#	USING:
#     OPUS MAKE Uiltity
#
#  USEAGE: 
#       type 'make -f dcedata'
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
# Modem Task Objects 
#

MT_CUTLASS_II = mt00if10.o mt00mn10.o mt00dp10.o mt00io10.o \
                mt00cd10.o mt00hw10.o mt00fx10.o mt00rt10.o \
                mt00cust.o mt00ts10.o audio_if.o mt_voice.o \
                mt00sp10.o mt_stub.o dosapp.o dsp_drv.o \
                mt00dbug.o newpeek.o \
                dsppatch.o compatch.o datpatch.o trnpatch.o 

#
# VView Modem Task Objects 
#

MT_VCV = mt00vv10.o

#
# V8 Modem Task Objects 
#

MT_V8 = mt00v834.o

#
# DOS Modem Task Objects 
#

MT_DOS = host.o

#
#  Tools Flags
#
LPATH  = dp_crus
HPATH  = dce_crus
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)$(HPATH) $(CFLAGS)
VPATH  = inc $(LPATH) $(HPATH)

#
#	Main Executable
#

vcv: $(MT_VCV)
       #@@ echo $(MT_VCV:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

v8: $(MT_V8)
       #@@ echo $(MT_V8:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

cutlass_2: $(MT_CUTLASS_II)
       #@@ echo $(MT_CUTLASS_II:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

dos: $(MT_DOS)
       #@@ echo $(MT_DOS:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

clean:
			@@echo @echo off > delfile.bat
			@@echo $(MT_CUTLASS_II:S/.*/$(LOADOBJCMD)del &$(RETURN)/) >> delfile.bat
			@@echo $(MT_V8:S/.*/$(LOADOBJCMD)del &$(RETURN)/) >> delfile.bat
			@@echo $(MT_VCV:S/.*/$(LOADOBJCMD)del &$(RETURN)/) >> delfile.bat
			@@echo $(MT_DOS:S/.*/$(LOADOBJCMD)del &$(RETURN)/) >> delfile.bat
			delfile.bat
			del delfile.bat

#
# File dependancies
audio_if.o:   $(LPATH).audio_if.c assert.h audio_if.h mt_voice.h sys_def.h tracemac.h
		$(CC) $(CFLGS) $(LPATH).audio_if.c

dosapp.o:     $(LPATH).dosapp.c sys_def.h cp_dsp.h mt_codef.h mt_hwdef.h mt_copro.h \
                mt_hwpro.h mt_coext.h dsp_drv.h mt_macro.h tracemac.h \
                dosapp.h
		$(CC) $(CFLGS) $(LPATH).dosapp.c

mt00vv10.o:   $(LPATH).mt00vv10.c icd_msgs.h cp_dsp.h dsp_drv.h mt_macro.h \
	        mt_voice.h mt_codef.h mt_coext.h mt_copro.h sys_def.h \
                vcv_def.h vcv_mem.h
		$(CC) $(CFLGS) $(LPATH).mt00vv10 .c

mt_voice.o:   $(LPATH).mt_voice.c icd_msgs.h audio_if.h mt_voice.h mt_codef.h \
	        mt_coext.h mt_copro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt_voice.c

mt00v834.o:   $(LPATH).mt00v834.c iohw_equ.h lineedf.h lineequ.h \
                cp_dsp.h mt_macro.h mt_v8v34.h mt_codef.h mt_coext.h \
	        mt_copro.h mt_hwdef.h mt_hwpro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00v834.c

dsp_drv.o:    $(LPATH).dsp_drv.c dsp_drv.h mt_codef.h mt_coext.h mt_copro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).dsp_drv.c

host.o:       $(LPATH).host.c cp_dsp.h dsp_drv.h mt_macro.h mt_codef.h mt_coext.h \
	        mt_copro.h mt_hwdef.h mt_hwpro.h sys_def.h tracemac.h
		$(CC) $(CFLGS) $(LPATH).host.c

mt00cd10.o:   $(LPATH).mt00cd10.c cp_dsp.h mt_codef.h mt_hwdef.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00cd10.c

mt00cust.o:   $(LPATH).mt00cust.c cp_dsp.h mt_macro.h mt_codef.h mt_coext.h mt_copro.h \
	        mt_hwdef.h mt_hwpro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00cust.c

mt00dp10.o:   $(LPATH).mt00dp10.c cp_dsp.h mt_macro.h mt_codef.h mt_coext.h mt_copro.h \
	        mt_hwdef.h mt_hwpro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00dp10.c

mt00fx10.o:   $(LPATH).mt00fx10.c cp_dsp.h mt_macro.h mt_codef.h mt_coext.h mt_copro.h \
	        mt_hwdef.h mt_hwpro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00fx10.c

mt00hw10.o:   $(LPATH).mt00hw10.c cp_dsp.h mt_macro.h mt_codef.h mt_coext.h mt_copro.h \
	        mt_hwdef.h mt_hwpro.h sys_def.h tracemac.h
		$(CC) $(CFLGS) $(LPATH).mt00hw10.c

mt00if10.o:   $(LPATH).mt00if10.c cp_dsp.h mt_codef.h mt_coext.h mt_copro.h mt_hwpro.h \
	        sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00if10.c

mt00io10.o:   $(LPATH).mt00io10.c dsp_drv.h mt_codef.h mt_coext.h mt_copro.h mt_hwdef.h \
	        mt_hwpro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00io10.c

mt00mn10.o:   $(LPATH).mt00mn10.c cp_dsp.h mt_macro.h mt_codef.h mt_coext.h mt_copro.h \
	        mt_hwdef.h mt_hwpro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00mn10.c

mt00rt10.o:   $(LPATH).mt00rt10.c cp_dsp.h mt_macro.h mt_codef.h mt_coext.h mt_copro.h \
	        mt_hwdef.h mt_hwpro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00rt10.c

mt00sp10.o:   $(LPATH).mt00sp10.c cp_dsp.h dsp_drv.h mt_codef.h mt_coext.h mt_copro.h \
	        mt_hwdef.h mt_hwpro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00sp10.c

mt00ts10.o:   $(LPATH).mt00ts10.c cp_dsp.h mt_macro.h mt_codef.h mt_coext.h mt_copro.h \
	        mt_hwdef.h mt_hwpro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt00ts10.c

mt_stub.o:    $(LPATH).mt_stub.c ee_def.h cp_dsp.h mt_macro.h mt_pro.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).mt_stub.c

dsppatch.o:   $(LPATH).dsppatch.c sys_def.h mt_pro.h cp_dsp.h mt_macro.h mt_codef.h
		$(CC) $(CFLGS) $(LPATH).dsppatch.c

compatch.o:   $(LPATH).compatch.c
		$(CC) $(CFLGS) $(LPATH).compatch.c

datpatch.o:   $(LPATH).datpatch.c
		$(CC) $(CFLGS) $(LPATH).datpatch.c

trnpatch.o:   $(LPATH).trnpatch.c
		$(CC) $(CFLGS) $(LPATH).trnpatch.c

mt00dbug.o:   $(LPATH).mt00dbug.c
		$(CC) $(CFLGS) -Iacu $(LPATH).mt00dbug.c

newpeek.o:    $(LPATH).newpeek.c
		$(CC) $(CFLGS) $(LPATH).newpeek.c


# Dynamic dependencies:
o.compatch:	dp_crus.c.compatch
o.datpatch:	dp_crus.c.datpatch
o.trnpatch:	dp_crus.c.trnpatch
o.host:	dp_crus.c.host
o.host:	inc.h.sys_def
o.host:	inc.h.__config
o.host:	inc.h.risc_os
o.host:	dp_crus.h.cp_dsp
o.host:	dp_crus.h.mt_codef
o.host:	dp_crus.h.mt_hwdef
o.host:	dp_crus.h.mt_copro
o.host:	dp_crus.h.mt_hwpro
o.host:	dp_crus.h.mt_coext
o.host:	dp_crus.h.dsp_drv
o.host:	dp_crus.h.mt_macro
o.host:	inc.h.tracemac
o.host:	dp_crus.h.mt_voice
o.mt00v834:	dp_crus.c.mt00v834
o.mt00v834:	inc.H.SYS_DEF
o.mt00v834:	inc.h.__config
o.mt00v834:	inc.h.risc_os
o.mt00v834:	dp_crus.H.MT_MACRO
o.mt00v834:	dp_crus.H.MT_COPRO
o.mt00v834:	dp_crus.H.MT_CODEF
o.mt00v834:	dp_crus.H.MT_HWDEF
o.mt00v834:	dp_crus.H.MT_HWPRO
o.mt00v834:	dp_crus.H.CP_DSP
o.mt00v834:	dp_crus.H.MT_V8V34
o.mt00v834:	inc.h.sys_def
o.mt00v834:	dp_crus.H.MT_COEXT
o.mt00v834:	dce_crus.H.IOHW_EQU
o.mt00v834:	dce_crus.h.lineequ
o.mt00v834:	dce_crus.h.lineedf
o.mt00if10:	dp_crus.c.mt00if10
o.mt00if10:	inc.H.SYS_DEF
o.mt00if10:	inc.h.__config
o.mt00if10:	inc.h.risc_os
o.mt00if10:	dp_crus.h.cp_dsp
o.mt00if10:	dp_crus.H.MT_CODEF
o.mt00if10:	dp_crus.H.MT_COEXT
o.mt00if10:	dp_crus.H.MT_HWPRO
o.mt00if10:	dp_crus.h.mt_copro
o.mt00mn10:	dp_crus.c.mt00mn10
o.mt00mn10:	inc.H.SYS_DEF
o.mt00mn10:	inc.h.__config
o.mt00mn10:	inc.h.risc_os
o.mt00mn10:	dp_crus.h.cp_dsp
o.mt00mn10:	dp_crus.H.MT_CODEF
o.mt00mn10:	dp_crus.H.MT_HWDEF
o.mt00mn10:	dp_crus.H.MT_COPRO
o.mt00mn10:	dp_crus.H.MT_HWPRO
o.mt00mn10:	dp_crus.H.MT_V8V34
o.mt00mn10:	inc.h.sys_def
o.mt00mn10:	dp_crus.H.MT_COEXT
o.mt00mn10:	dp_crus.h.mt_macro
o.mt00mn10:	dp_crus.h.dsppatch
o.mt00mn10:	dp_crus.h.dsp_drv
o.mt00dp10:	dp_crus.c.mt00dp10
o.mt00dp10:	inc.H.SYS_DEF
o.mt00dp10:	inc.h.__config
o.mt00dp10:	inc.h.risc_os
o.mt00dp10:	dp_crus.h.cp_dsp
o.mt00dp10:	dp_crus.H.MT_V8V34
o.mt00dp10:	inc.h.sys_def
o.mt00dp10:	dp_crus.H.MT_CODEF
o.mt00dp10:	dp_crus.H.MT_HWDEF
o.mt00dp10:	dp_crus.H.MT_COPRO
o.mt00dp10:	dp_crus.H.MT_HWPRO
o.mt00dp10:	dp_crus.H.MT_COEXT
o.mt00dp10:	dp_crus.h.mt_macro
o.mt00cd10:	dp_crus.c.mt00cd10
o.mt00cd10:	inc.H.SYS_DEF
o.mt00cd10:	inc.h.__config
o.mt00cd10:	inc.h.risc_os
o.mt00cd10:	dp_crus.H.MT_CODEF
o.mt00cd10:	dp_crus.H.MT_HWDEF
o.mt00cd10:	dp_crus.H.MT_V8V34
o.mt00cd10:	inc.h.sys_def
o.mt00cd10:	dp_crus.h.cp_dsp
o.mt00hw10:	dp_crus.c.mt00hw10
o.mt00hw10:	dp_crus.h.mod_def
o.mt00hw10:	inc.H.SYS_DEF
o.mt00hw10:	inc.h.__config
o.mt00hw10:	inc.h.risc_os
o.mt00hw10:	dp_crus.h.cp_dsp
o.mt00hw10:	dp_crus.H.MT_CODEF
o.mt00hw10:	dp_crus.H.MT_HWDEF
o.mt00hw10:	dp_crus.H.MT_COPRO
o.mt00hw10:	dp_crus.H.MT_HWPRO
o.mt00hw10:	dp_crus.H.MT_COEXT
o.mt00hw10:	dp_crus.h.mt_macro
o.mt00hw10:	inc.h.tracemac
o.mt00fx10:	dp_crus.c.mt00fx10
o.mt00fx10:	inc.H.SYS_DEF
o.mt00fx10:	inc.h.__config
o.mt00fx10:	inc.h.risc_os
o.mt00fx10:	dp_crus.h.cp_dsp
o.mt00fx10:	dp_crus.H.MT_COPRO
o.mt00fx10:	dp_crus.H.MT_CODEF
o.mt00fx10:	dp_crus.H.MT_COEXT
o.mt00fx10:	dp_crus.H.MT_HWPRO
o.mt00fx10:	dp_crus.H.MT_HWDEF
o.mt00fx10:	dp_crus.h.mt_macro
o.mt00rt10:	dp_crus.c.mt00rt10
o.mt00rt10:	inc.H.SYS_DEF
o.mt00rt10:	inc.h.__config
o.mt00rt10:	inc.h.risc_os
o.mt00rt10:	dp_crus.h.cp_dsp
o.mt00rt10:	dp_crus.H.MT_COPRO
o.mt00rt10:	dp_crus.H.MT_CODEF
o.mt00rt10:	dp_crus.H.MT_COEXT
o.mt00rt10:	dp_crus.H.MT_HWDEF
o.mt00rt10:	dp_crus.H.MT_HWPRO
o.mt00rt10:	dp_crus.h.mt_macro
o.mt00cust:	dp_crus.c.mt00cust
o.mt00cust:	inc.H.SYS_DEF
o.mt00cust:	inc.h.__config
o.mt00cust:	inc.h.risc_os
o.mt00cust:	dp_crus.h.cp_dsp
o.mt00cust:	dp_crus.H.MT_CODEF
o.mt00cust:	dp_crus.H.MT_HWDEF
o.mt00cust:	dp_crus.H.MT_COPRO
o.mt00cust:	dp_crus.H.MT_HWPRO
o.mt00cust:	dp_crus.H.MT_COEXT
o.mt00cust:	dp_crus.h.mt_macro
o.mt00cust:	dp_crus.h.dsp_drv
o.mt00cust:	dp_crus.h.dsppatch
o.mt00ts10:	dp_crus.c.mt00ts10
o.mt00ts10:	inc.H.SYS_DEF
o.mt00ts10:	inc.h.__config
o.mt00ts10:	inc.h.risc_os
o.mt00ts10:	dp_crus.h.cp_dsp
o.mt00ts10:	dp_crus.H.MT_CODEF
o.mt00ts10:	dp_crus.H.MT_HWDEF
o.mt00ts10:	dp_crus.H.MT_COPRO
o.mt00ts10:	dp_crus.H.MT_HWPRO
o.mt00ts10:	dp_crus.H.MT_COEXT
o.mt00ts10:	dp_crus.h.mt_macro
o.audio_if:	dp_crus.c.audio_if
o.audio_if:	inc.h.sys_def
o.audio_if:	inc.h.__config
o.audio_if:	inc.h.risc_os
o.audio_if:	inc.h.audio_if
o.audio_if:	inc.h.assert
o.audio_if:	inc.h.tracemac
o.audio_if:	dp_crus.h.mt_voice
o.mt_voice:	dp_crus.c.mt_voice
o.mt_voice:	inc.H.SYS_DEF
o.mt_voice:	inc.h.__config
o.mt_voice:	inc.h.risc_os
o.mt_voice:	dp_crus.H.MT_CODEF
o.mt_voice:	dp_crus.H.MT_COEXT
o.mt_voice:	dp_crus.H.MT_COPRO
o.mt_voice:	inc.h.audio_if
o.mt_voice:	dp_crus.h.cp_dsp
o.mt_voice:	dp_crus.h.mt_voice
o.mt_voice:	inc.h.audio_if
o.mt00sp10:	dp_crus.c.mt00sp10
o.mt00sp10:	inc.h.sys_def
o.mt00sp10:	inc.h.__config
o.mt00sp10:	inc.h.risc_os
o.mt00sp10:	dp_crus.h.cp_dsp
o.mt00sp10:	dp_crus.H.MT_CODEF
o.mt00sp10:	dp_crus.H.MT_HWDEF
o.mt00sp10:	dp_crus.H.MT_COPRO
o.mt00sp10:	dp_crus.H.MT_HWPRO
o.mt00sp10:	dp_crus.H.MT_COEXT
o.mt00sp10:	dp_crus.H.DSP_DRV
o.mt00sp10:	inc.h.tracemac
o.mt_stub:	dp_crus.c.mt_stub
o.mt_stub:	inc.H.SYS_DEF
o.mt_stub:	inc.h.__config
o.mt_stub:	inc.h.risc_os
o.mt_stub:	inc.h.ee_def
o.mt_stub:	dp_crus.h.cp_dsp
o.mt_stub:	inc.h.mt_pro
o.mt_stub:	dp_crus.h.mt_macro
o.dsp_drv:	dp_crus.c.dsp_drv
o.dsp_drv:	inc.h.sys_def
o.dsp_drv:	inc.h.__config
o.dsp_drv:	inc.h.risc_os
o.dsp_drv:	dp_crus.h.dsp_drv
o.dsp_drv:	dp_crus.h.dsppatch
o.dsp_drv:	dp_crus.h.mt_coext
o.dsp_drv:	dp_crus.h.mt_codef
o.dsp_drv:	dp_crus.h.mt_copro
o.dsp_drv:	dp_crus.h.icd_msgs
o.dsp_drv:	dp_crus.h.vbdtregs
o.dsp_drv:	dp_crus.h.mt_codef
o.mt00dbug:	dp_crus.c.mt00dbug
o.mt00dbug:	inc.h.sys_def
o.mt00dbug:	inc.h.__config
o.mt00dbug:	inc.h.risc_os
o.mt00dbug:	dp_crus.h.mt_codef
o.mt00dbug:	dp_crus.h.mt_coext
o.mt00dbug:	dp_crus.h.mt_copro
o.mt00dbug:	dp_crus.h.cp_dsp
o.mt00dbug:	inc.h.acu_pro
o.newpeek:	dp_crus.c.newpeek
o.newpeek:	inc.h.sys_def
o.newpeek:	inc.h.__config
o.newpeek:	inc.h.risc_os
o.newpeek:	dp_crus.h.cut_type
o.newpeek:	dp_crus.h.icd_msgs
o.dsppatch:	dp_crus.c.dsppatch
o.dsppatch:	inc.h.sys_def
o.dsppatch:	inc.h.__config
o.dsppatch:	inc.h.risc_os
o.dsppatch:	inc.h.mt_pro
o.dsppatch:	dp_crus.h.cp_dsp
o.dsppatch:	dp_crus.h.dsp_drv
o.dsppatch:	dp_crus.h.mt_macro
o.dsppatch:	dp_crus.h.mt_codef
o.dsppatch:	dp_crus.h.mt_coext
o.dsppatch:	dp_crus.h.dsppatch
o.dosapp:	dp_crus.c.dosapp
o.dosapp:	inc.h.sys_def
o.dosapp:	inc.h.__config
o.dosapp:	inc.h.risc_os
o.dosapp:	dp_crus.h.cp_dsp
o.dosapp:	dp_crus.h.mt_codef
o.dosapp:	dp_crus.h.mt_hwdef
o.dosapp:	dp_crus.h.mt_copro
o.dosapp:	dp_crus.h.mt_hwpro
o.dosapp:	dp_crus.h.mt_coext
o.dosapp:	dp_crus.h.dsp_drv
o.dosapp:	dp_crus.h.dsppatch
o.dosapp:	dp_crus.h.mt_macro
o.dosapp:	dp_crus.h.mt_voice
o.dosapp:	inc.h.tracemac
o.dosapp:	dp_crus.h.dosapp
o.dosapp:	C:h.kernel
o.mt00io10:	dp_crus.c.mt00io10
o.mt00io10:	inc.H.SYS_DEF
o.mt00io10:	inc.h.__config
o.mt00io10:	inc.h.risc_os
o.mt00io10:	dp_crus.h.cp_dsp
o.mt00io10:	dp_crus.H.MT_CODEF
o.mt00io10:	dp_crus.H.MT_HWDEF
o.mt00io10:	dp_crus.H.MT_COPRO
o.mt00io10:	dp_crus.H.MT_HWPRO
o.mt00io10:	dp_crus.H.MT_COEXT
o.mt00io10:	dp_crus.H.DSP_DRV
o.mt00io10:	dp_crus.h.mt_macro
