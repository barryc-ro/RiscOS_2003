#
#     Copyright, 1988,1989,1990,1991,1992,1993,1994,1995.
#     All Rights Reserved by:
#        RSA
#        7701 Six Forks Road
#        Suite 120
#        Raleigh, NC  27615
#        (919) 846-7171
#
#     This document contains material confidential to RSA. Its contents
#     must not be revealed, used or disclosed to anyone or company without
#     written permission by RSA. The information contained herein is solely
#     for the use of RSA.
#
#	   EXAMPLE MAKE FILE FOR:
#           RSA Modem ACU Module 
#
#	   PLATFORM:
#
#     SOFTWARE TOOLS:
#           OPUS MAKE make utility
# 
#     USEAGE: 
#           type 'make -f acu'      to add various ACU configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

# PSTN data modem Automatic Calling Unit
ACU = ac00mstr.o ac00mem.o ac00que.o ac00lib1.o ac00lib2.o \
      ac00lib3.o ac00lib4.o ac00tab0.o ac00tab1.o ac00tab2.o \
      ac00sta0.o ac00sta1.o ac00v24.o ac00id.o ac00idat.o \
      ac00or.o ac00orat.o ac00ordl.o ac00orps.o \
      ac00an.o ac00anps.o ac00on.o ac00onat.o \
      ac00pr.o ac00pra1.o ac00hn.o ac00hnat.o ac00hnps.o \
      ac00rt.o ac00ts.o ac00tsrm.o ac00tsa1.o ac00tsa2.o \
      ac00cmat.o ac00cmdt.o ac00cman.o ac00cmav.o ac00cmaw.o \
      ac00cmex.o ac00cmts.o \
      ac00sys0.o ac00sys1.o ac00cust.o 

ACUIO_STUB =  acuiostb.o

SYSTEM_STUB =  v42_stub.o mt_stub.o pag_stub.o fax_stub.o \
               as_stub.o vce_stub.o vv_stub.o wbiosstb.o audiostb.o

#
#  Tools flags
#
LPATH  = acu.acu.
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CFLAGS)
VPATH  = inc $(LPATH)
                    
# Compile ACU
acu:    $(ACU)
        #@ echo $(ACU:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

acuiostb:    $(ACUIO_STUB)
        #@ echo $(ACUIO_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

# Compile and link system stubs
sys_stub:   $(SYSTEM_STUB) 
            #@ echo $(SYSTEM_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd           
            #@ echo $(LOADOBJCMD) C:\mcc68k\68000\mcc68kab.lib >> link.cmd
            #@ echo END >> link.cmd
  	    #     $(LK) -c link.cmd $(LFLAGS) -m > link.map

release: acu.mak $(ACU:S/obj/c) $(ACUIO_STUB:S/obj/c) $(LPATH)*.h
      #@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(SYSTEM_STUB:S/obj/c) acu.mak 
      #@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

# linker requirements 
link:
            @ echo CHIP 68000 > link.cmd
            @ echo LISTABS >> link.cmd
            @ echo SECT zerovars=$$800 >> link.cmd
            @ echo SECT startcode=$$400 >> link.cmd
            @ echo SECT code=$$400500 >> link.cmd
            @ echo SECT reset=$$000000 >> link.cmd
            @ echo public ????STACKTOP=$$0fffe >> link.cmd
            @ echo ORDER code,strings,const,vars >> link.cmd


#
#	ACU Dependent Modules
#

#
# (Core) ACU V.42 Driver Source Code
#
ac00mem.o:   ac00mem.c sys_def.h  acu_def.h
               $(CC) $(CFLGS) $(LPATH)ac00mem.c

ac00mstr.o:	ac00mstr.c ac00mstr.c sys_def.h acu_def.h acu_mem.h v25_def.h \
               v25_mem.h acu_pro.h mt_pro.h ll_pro.h v25_pro.h fp_pro.h \
               ds_pro.h acu_epro.h wl_pro.h 
               $(CC) $(CFLGS) $(LPATH)ac00mstr.c

ac00sys0.o:  ac00sys0.c acu_def.h sys_def.h mt_pro.h acu_pro.h acu_epro.h \
               dte.edf v42.edf dce_line.edf
               $(CC) $(CFLGS) $(LPATH)ac00sys0.c

ac00sys1.o:  ac00sys1.c acu_def.h sys_def.h mt_pro.h acu_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00sys1.c

ac00id.o:	   ac00id.c sys_def.h v25_mem.h acu_def.h v25_def.h acu_pro.h \
               mt_pro.h v25_pro.h wl_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00id.c

ac00idat.o:	ac00idat.c sys_def.h acu_def.h v25_pro.h acu_pro.h mt_pro.h \
               acu_vce.h 
	       $(CC) $(CFLGS) $(LPATH)ac00idat.c

ac00or.o:	   ac00or.c sys_def.h acu_def.h acu_pro.h ll_pro.h v25_pro.h \
               wl_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00or.c

ac00orat.o:	ac00orat.c sys_def.h acu_def.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00orat.c

ac00orps.o:	ac00orps.c sys_def.h v25_mem.h ds_def.h acu_def.h v25_def.h \
               ds_mem.h mt_pro.h acu_pro.h fp_pro.h fax_pro.h acu_epro.h \
                wl_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00orps.c

ac00ordl.o:	ac00ordl.c sys_def.h ee_def.h acu_def.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00ordl.c

ac00pr.o:    ac00pr.c sys_def.h acu_def.h acu_pro.h v25_pro.h wl_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00pr.c

ac00pra1.o:	ac00pra1.c sys_def.h acu_def.h acu_pro.h acu_mem.h mt_pro.h \
               fax_pro.h par_def.h par_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00pra1.c

ac00an.o: 	ac00an.c sys_def.h acu_def.h acu_pro.h v25_pro.h wl_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00an.c

ac00anps.o:	ac00anps.c sys_def.h acu_def.h ds_def.h ds_mem.h acu_pro.h \
               mt_pro.h fax_pro.h acu_epro.h wl_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00anps.c

ac00on.o:	   ac00on.c sys_def.h v25_mem.h acu_def.h v25_def.h v25_pro.h \
               mt_pro.h acu_pro.h ll_pro.h fax_pro.h wl_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00on.c

ac00onat.o:	ac00onat.c sys_def.h v25_def.h v25_mem.h acu_def.h mt_pro.h \
               acu_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00onat.c

ac00hn.o:	   ac00hn.c sys_def.h acu_def.h acu_pro.h ll_pro.h v25_pro.h \
               wl_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00hn.c

ac00hnat.o:	ac00hnat.c sys_def.h dce_line.edf acu_def.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00hnat.c

ac00hnps.o:	ac00hnps.c sys_def.h dce_line.edf acu_def.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00hnps.c

ac00rt.o:	   ac00rt.c sys_def.h v25_def.h v25_mem.h acu_def.h ll_pro.h \
               acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00rt.c

ac00lib1.o:	ac00lib1.c sys_def.h v25_mem.h acu_def.h v25_def.h ds_def.h \
               ds_mem.h ee_def.h acu_pro.h mt_pro.h v25_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00lib1.c

ac00lib2.o:	ac00lib2.c sys_def.h acu_def.h dte.edf dce_line.edf acu_pro.h \
               mt_pro.h v25_pro.h
	       $(CC) $(CFLGS) $(LPATH)ac00lib2.c

ac00ts.o:	   ac00ts.c sys_def.h acu_def.h acu_pro.h v25_pro.h fp_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00ts.c

ac00tsa1.o:	ac00tsa1.c sys_def.h acu_def.h acu_mem.h ds_def.h ds_mem.h \
               acu_pro.h mt_pro.h v42.edf dte.edf dce_line.edf 
               $(CC) $(CFLGS) $(LPATH)ac00tsa1.c

ac00tsa2.o:	ac00tsa2.c sys_def.h ds_mem.h acu_def.h dte.edf dce_line.edf \
               mt_pro.h acu_pro.h v25_mem.h v25_def.h
               $(CC) $(CFLGS) $(LPATH)ac00tsa2.c

ac00tsrm.o:	ac00tsrm.c sys_def.h ds_mem.h ds_def.h acu_def.h acu_pro.h \
               mt_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00tsrm.c

ac00que.o:	ac00que.c sys_def.h acu_def.h
               $(CC) $(CFLGS) $(LPATH)ac00que.c

ac00lib3.o:	ac00lib3.c sys_def.h ds_def.h ds_mem.h acu_def.h ee_def.h \
               acu_pro.h mt_pro.h ll_pro.h v25_pro.h acu_epro.h \
               par_def.h par_pro.h acu_vce.h
               $(CC) $(CFLGS) $(LPATH)ac00lib3.c

ac00lib4.o:	ac00lib4.c sys_def.h dce_line.edf dte.edf acu_def.h \
               acu_pro.h mt_pro.h v25_pro.h fp_pro.h ll_pro.h acu_epro.h 
               $(CC) $(CFLGS) $(LPATH)ac00lib4.c

ac00cmat.o:	ac00cmat.c sys_def.h acu_def.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00cmat.c

ac00cmdt.o:  ac00cmdt.c sys_def.h acu_pro.h par_def.h par_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00cmdt.c

ac00cman.o:	ac00cman.c sys_def.h ee_def.h acu_def.h acu_pro.h mt_pro.h \
               v25_pro.h ll_pro.h acu_epro.h
               $(CC) $(CFLGS) $(LPATH)ac00cman.c

ac00cmav.o:	ac00cmav.c sys_def.h acu_def.h ee_def.h acu_pro.h mt_pro.h \
               acu_epro.h
               $(CC) $(CFLGS) $(LPATH)ac00cmav.c

ac00cmaw.o:	ac00cmaw.c sys_def.h acu_def.h ee_def.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00cmaw.c

ac00cmex.o:	ac00cmex.c sys_def.h acu_def.h acu_pro.h mt_pro.h acu_epro.h
               $(CC) $(CFLGS) $(LPATH)ac00cmex.c

ac00cmts.o:	ac00cmts.c dce_line.edf dte.edf sys_def.h acu_def.h acu_pro.h \
               mt_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00cmts.c

ac00tab0.o:	ac00tab0.c sys_def.h acu_def.h 
               $(CC) $(CFLGS) $(LPATH)ac00tab0.c

ac00tab1.o:	ac00tab1.c sys_def.h acu_def.h acu_edef.h
               $(CC) $(CFLGS) $(LPATH)ac00tab1.c

ac00tab2.o:	ac00tab2.c sys_def.h acu_def.h acu_edef.h
               $(CC) $(CFLGS) $(LPATH)ac00tab2.c

ac00v24.o:	ac00v24.c sys_def.h acu_def.h acu_pro.h mt_pro.h ll_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00v24.c

ac00sta0.o:	ac00sta0.c sys_def.h acu_def.h acu_pro.h fp_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00sta0.c

ac00sta1.o:	ac00sta1.c sys_def.h acu_def.h acu_pro.h fp_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00sta1.c

ac00cust.o:  ac00cust.c sys_def.h acu_def.h acu_pro.h acu_mem.h
               $(CC) $(CFLGS) $(LPATH)ac00cust.c

#
#	System Dependent Modules
#
pag_stub.o:	pag_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH)pag_stub.c

v42_stub.o:	v42_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH)v42_stub.c

v25_stub.o:	v25_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH)v25_stub.c

fax_stub.o:	fax_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH)fax_stub.c
                                 
wbiosstb.o:  wbiosstb.c sys_def.h wbios.h
               $(CC) $(CFLGS) $(LPATH)wbiosstb.c

audiostb.o:  audiostb.c sys_def.h audio_if.h 
               $(CC) $(CFLGS) $(LPATH)audiostb.c

vce_stub.o:	vce_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH)vce_stub.c
                                 
as_stub.o:	as_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH)as_stub.c
                                 
mt_stub.o:	mt_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH)mt_stub.c

acuiostb.o:	acuiostb.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH)acuiostb.c

vv_stub.o:   vv_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH)vv_stub.c


# Dynamic dependencies:
o.ac00mstr:	acu.acu.c.ac00mstr
o.ac00mstr:	inc.h.sys_def
o.ac00mstr:	inc.h.__config
o.ac00mstr:	inc.h.risc_os
o.ac00mstr:	inc.h.acu_def
o.ac00mstr:	inc.h.acu_mem
o.ac00mstr:	inc.h.v25_def
o.ac00mstr:	acu.acu.h.v25_mem
o.ac00mstr:	inc.h.acu_pro
o.ac00mstr:	inc.h.mt_pro
o.ac00mstr:	acu.acu.h.ll_pro
o.ac00mstr:	acu.acu.h.v25_pro
o.ac00mstr:	inc.h.fp_pro
o.ac00mstr:	inc.h.ds_pro
o.ac00mstr:	acu.acu.h.acu_epro
o.ac00mstr:	acu.acu.h.wl_pro
o.ac00mem:	acu.acu.c.ac00mem
o.ac00mem:	inc.h.sys_def
o.ac00mem:	inc.h.__config
o.ac00mem:	inc.h.risc_os
o.ac00mem:	inc.h.acu_def
o.ac00que:	acu.acu.c.ac00que
o.ac00que:	inc.h.sys_def
o.ac00que:	inc.h.__config
o.ac00que:	inc.h.risc_os
o.ac00que:	inc.h.acu_def
o.ac00que:	inc.h.acu_mem
o.ac00lib1:	acu.acu.c.ac00lib1
o.ac00lib1:	inc.h.sys_def
o.ac00lib1:	inc.h.__config
o.ac00lib1:	inc.h.risc_os
o.ac00lib1:	inc.h.acu_def
o.ac00lib1:	inc.h.acu_mem
o.ac00lib1:	inc.h.ds_def
o.ac00lib1:	inc.h.ds_mem
o.ac00lib1:	inc.h.ee_def
o.ac00lib1:	inc.h.acu_pro
o.ac00lib1:	inc.h.mt_pro
o.ac00lib2:	acu.acu.c.ac00lib2
o.ac00lib2:	inc.h.sys_def
o.ac00lib2:	inc.h.__config
o.ac00lib2:	inc.h.risc_os
o.ac00lib2:	inc.h.acu_def
o.ac00lib2:	inc.h.acu_mem
o.ac00lib2:	inc.h.acu_pro
o.ac00lib2:	inc.h.mt_pro
o.ac00lib2:	acu.acu.h.v25_pro
o.ac00lib2:	inc.edf.dte
o.ac00lib2:	inc.edf.dce_line
o.ac00lib3:	acu.acu.c.ac00lib3
o.ac00lib3:	inc.h.sys_def
o.ac00lib3:	inc.h.__config
o.ac00lib3:	inc.h.risc_os
o.ac00lib3:	inc.h.acu_def
o.ac00lib3:	inc.h.acu_mem
o.ac00lib3:	inc.h.ds_def
o.ac00lib3:	inc.h.ds_mem
o.ac00lib3:	inc.h.ee_def
o.ac00lib3:	inc.h.acu_pro
o.ac00lib3:	inc.h.mt_pro
o.ac00lib3:	acu.acu.h.v25_pro
o.ac00lib3:	acu.acu.h.ll_pro
o.ac00lib3:	acu.acu.h.acu_vce
o.ac00lib3:	acu.acu.h.acu_epro
o.ac00lib3:	acu.acu.h.wl_pro
o.ac00lib3:	inc.h.par_def
o.ac00lib3:	inc.h.par_pro
o.ac00lib4:	acu.acu.c.ac00lib4
o.ac00lib4:	inc.h.sys_def
o.ac00lib4:	inc.h.__config
o.ac00lib4:	inc.h.risc_os
o.ac00lib4:	inc.h.acu_def
o.ac00lib4:	inc.h.acu_mem
o.ac00lib4:	inc.h.acu_pro
o.ac00lib4:	inc.h.mt_pro
o.ac00lib4:	acu.acu.h.v25_pro
o.ac00lib4:	acu.acu.h.ll_pro
o.ac00lib4:	inc.h.fp_pro
o.ac00lib4:	acu.acu.h.acu_vce
o.ac00lib4:	acu.acu.h.acu_epro
o.ac00tab0:	acu.acu.c.ac00tab0
o.ac00tab0:	inc.h.sys_def
o.ac00tab0:	inc.h.__config
o.ac00tab0:	inc.h.risc_os
o.ac00tab0:	inc.h.acu_def
o.ac00tab0:	inc.h.acu_mem
o.ac00tab1:	acu.acu.c.ac00tab1
o.ac00tab1:	inc.h.sys_def
o.ac00tab1:	inc.h.__config
o.ac00tab1:	inc.h.risc_os
o.ac00tab1:	inc.h.acu_def
o.ac00tab1:	inc.h.acu_mem
o.ac00tab1:	acu.acu.h.acu_edef
o.ac00tab2:	acu.acu.c.ac00tab2
o.ac00tab2:	inc.h.sys_def
o.ac00tab2:	inc.h.__config
o.ac00tab2:	inc.h.risc_os
o.ac00tab2:	inc.h.acu_def
o.ac00tab2:	inc.h.acu_mem
o.ac00sta0:	acu.acu.c.ac00sta0
o.ac00sta0:	inc.h.sys_def
o.ac00sta0:	inc.h.__config
o.ac00sta0:	inc.h.risc_os
o.ac00sta0:	inc.h.acu_def
o.ac00sta0:	inc.h.acu_mem
o.ac00sta0:	inc.h.acu_pro
o.ac00sta0:	inc.h.fp_pro
o.ac00sta1:	acu.acu.c.ac00sta1
o.ac00sta1:	inc.h.sys_def
o.ac00sta1:	inc.h.__config
o.ac00sta1:	inc.h.risc_os
o.ac00sta1:	inc.h.acu_def
o.ac00sta1:	inc.h.acu_mem
o.ac00sta1:	inc.h.acu_pro
o.ac00sta1:	inc.h.fp_pro
o.ac00v24:	acu.acu.c.ac00v24
o.ac00v24:	inc.h.sys_def
o.ac00v24:	inc.h.__config
o.ac00v24:	inc.h.risc_os
o.ac00v24:	inc.h.acu_def
o.ac00v24:	inc.h.acu_mem
o.ac00v24:	inc.h.acu_pro
o.ac00v24:	inc.h.mt_pro
o.ac00v24:	acu.acu.h.ll_pro
o.ac00id:	acu.acu.c.ac00id
o.ac00id:	inc.h.sys_def
o.ac00id:	inc.h.__config
o.ac00id:	inc.h.risc_os
o.ac00id:	inc.h.acu_def
o.ac00id:	inc.h.acu_mem
o.ac00id:	inc.h.v25_def
o.ac00id:	acu.acu.h.v25_mem
o.ac00id:	inc.h.acu_pro
o.ac00id:	inc.h.mt_pro
o.ac00id:	acu.acu.h.ll_pro
o.ac00id:	acu.acu.h.v25_pro
o.ac00id:	acu.acu.h.wl_pro
o.ac00idat:	acu.acu.c.ac00idat
o.ac00idat:	inc.h.sys_def
o.ac00idat:	inc.h.__config
o.ac00idat:	inc.h.risc_os
o.ac00idat:	inc.h.acu_def
o.ac00idat:	inc.h.acu_mem
o.ac00idat:	inc.h.acu_pro
o.ac00idat:	inc.h.mt_pro
o.ac00idat:	acu.acu.h.v25_pro
o.ac00idat:	acu.acu.h.ll_pro
o.ac00idat:	acu.acu.h.acu_vce
o.ac00idat:	acu.acu.h.wl_pro
o.ac00or:	acu.acu.c.ac00or
o.ac00or:	inc.h.sys_def
o.ac00or:	inc.h.__config
o.ac00or:	inc.h.risc_os
o.ac00or:	inc.h.acu_def
o.ac00or:	inc.h.acu_mem
o.ac00or:	inc.h.acu_pro
o.ac00or:	acu.acu.h.ll_pro
o.ac00or:	acu.acu.h.v25_pro
o.ac00or:	acu.acu.h.wl_pro
o.ac00orat:	acu.acu.c.ac00orat
o.ac00orat:	inc.h.sys_def
o.ac00orat:	inc.h.__config
o.ac00orat:	inc.h.risc_os
o.ac00orat:	inc.h.acu_def
o.ac00orat:	inc.h.acu_mem
o.ac00orat:	inc.h.acu_pro
o.ac00orat:	inc.h.mt_pro
o.ac00ordl:	acu.acu.c.ac00ordl
o.ac00ordl:	inc.h.sys_def
o.ac00ordl:	inc.h.__config
o.ac00ordl:	inc.h.risc_os
o.ac00ordl:	inc.h.acu_def
o.ac00ordl:	inc.h.acu_mem
o.ac00ordl:	inc.h.ee_def
o.ac00ordl:	inc.h.acu_pro
o.ac00ordl:	inc.h.mt_pro
o.ac00orps:	acu.acu.c.ac00orps
o.ac00orps:	inc.h.sys_def
o.ac00orps:	inc.h.__config
o.ac00orps:	inc.h.risc_os
o.ac00orps:	inc.h.acu_def
o.ac00orps:	inc.h.acu_mem
o.ac00orps:	inc.h.v25_def
o.ac00orps:	acu.acu.h.v25_mem
o.ac00orps:	inc.h.ds_def
o.ac00orps:	inc.h.ds_mem
o.ac00orps:	inc.h.acu_pro
o.ac00orps:	inc.h.mt_pro
o.ac00orps:	acu.acu.h.acu_epro
o.ac00orps:	acu.acu.h.wl_pro
o.ac00an:	acu.acu.c.ac00an
o.ac00an:	inc.h.sys_def
o.ac00an:	inc.h.__config
o.ac00an:	inc.h.risc_os
o.ac00an:	inc.h.acu_def
o.ac00an:	inc.h.acu_mem
o.ac00an:	inc.h.acu_pro
o.ac00an:	acu.acu.h.v25_pro
o.ac00an:	acu.acu.h.ll_pro
o.ac00an:	acu.acu.h.wl_pro
o.ac00anps:	acu.acu.c.ac00anps
o.ac00anps:	inc.h.sys_def
o.ac00anps:	inc.h.__config
o.ac00anps:	inc.h.risc_os
o.ac00anps:	inc.h.acu_def
o.ac00anps:	inc.h.acu_mem
o.ac00anps:	inc.h.ds_def
o.ac00anps:	inc.h.ds_mem
o.ac00anps:	inc.h.acu_pro
o.ac00anps:	inc.h.mt_pro
o.ac00anps:	acu.acu.h.acu_epro
o.ac00anps:	acu.acu.h.wl_pro
o.ac00on:	acu.acu.c.ac00on
o.ac00on:	inc.h.sys_def
o.ac00on:	inc.h.__config
o.ac00on:	inc.h.risc_os
o.ac00on:	inc.h.acu_def
o.ac00on:	inc.h.acu_mem
o.ac00on:	inc.h.acu_pro
o.ac00on:	acu.acu.h.ll_pro
o.ac00on:	acu.acu.h.v25_pro
o.ac00on:	inc.h.mt_pro
o.ac00on:	acu.acu.h.wl_pro
o.ac00onat:	acu.acu.c.ac00onat
o.ac00onat:	inc.h.sys_def
o.ac00onat:	inc.h.__config
o.ac00onat:	inc.h.risc_os
o.ac00onat:	inc.h.acu_def
o.ac00onat:	inc.h.acu_mem
o.ac00onat:	inc.h.v25_def
o.ac00onat:	acu.acu.h.v25_mem
o.ac00onat:	inc.h.acu_pro
o.ac00onat:	inc.h.mt_pro
o.ac00pr:	acu.acu.c.ac00pr
o.ac00pr:	inc.h.sys_def
o.ac00pr:	inc.h.__config
o.ac00pr:	inc.h.risc_os
o.ac00pr:	inc.h.acu_def
o.ac00pr:	inc.h.acu_mem
o.ac00pr:	inc.h.acu_pro
o.ac00pr:	acu.acu.h.v25_pro
o.ac00pr:	acu.acu.h.ll_pro
o.ac00pr:	acu.acu.h.wl_pro
o.ac00pra1:	acu.acu.c.ac00pra1
o.ac00pra1:	inc.h.sys_def
o.ac00pra1:	inc.h.__config
o.ac00pra1:	inc.h.risc_os
o.ac00pra1:	inc.h.acu_def
o.ac00pra1:	inc.h.acu_mem
o.ac00pra1:	inc.h.acu_pro
o.ac00pra1:	inc.h.mt_pro
o.ac00pra1:	acu.acu.h.acu_vce
o.ac00pra1:	inc.h.par_def
o.ac00pra1:	inc.h.par_pro
o.ac00hn:	acu.acu.c.ac00hn
o.ac00hn:	inc.h.sys_def
o.ac00hn:	inc.h.__config
o.ac00hn:	inc.h.risc_os
o.ac00hn:	inc.h.acu_def
o.ac00hn:	inc.h.acu_mem
o.ac00hn:	inc.h.acu_pro
o.ac00hn:	acu.acu.h.ll_pro
o.ac00hn:	acu.acu.h.v25_pro
o.ac00hn:	acu.acu.h.wl_pro
o.ac00hnat:	acu.acu.c.ac00hnat
o.ac00hnat:	inc.h.sys_def
o.ac00hnat:	inc.h.__config
o.ac00hnat:	inc.h.risc_os
o.ac00hnat:	inc.h.acu_def
o.ac00hnat:	inc.h.acu_mem
o.ac00hnat:	inc.h.acu_pro
o.ac00hnat:	inc.h.mt_pro
o.ac00hnps:	acu.acu.c.ac00hnps
o.ac00hnps:	inc.h.sys_def
o.ac00hnps:	inc.h.__config
o.ac00hnps:	inc.h.risc_os
o.ac00hnps:	inc.h.acu_def
o.ac00hnps:	inc.h.acu_mem
o.ac00hnps:	inc.h.acu_pro
o.ac00hnps:	inc.h.mt_pro
o.ac00hnps:	inc.edf.dce_line
o.ac00rt:	acu.acu.c.ac00rt
o.ac00rt:	inc.h.sys_def
o.ac00rt:	inc.h.__config
o.ac00rt:	inc.h.risc_os
o.ac00rt:	inc.h.acu_def
o.ac00rt:	inc.h.acu_mem
o.ac00rt:	inc.h.v25_def
o.ac00rt:	acu.acu.h.v25_mem
o.ac00rt:	inc.h.acu_pro
o.ac00rt:	inc.h.mt_pro
o.ac00rt:	acu.acu.h.ll_pro
o.ac00ts:	acu.acu.c.ac00ts
o.ac00ts:	inc.h.sys_def
o.ac00ts:	inc.h.__config
o.ac00ts:	inc.h.risc_os
o.ac00ts:	inc.h.acu_def
o.ac00ts:	inc.h.acu_mem
o.ac00ts:	inc.h.acu_pro
o.ac00ts:	acu.acu.h.v25_pro
o.ac00ts:	inc.h.fp_pro
o.ac00tsrm:	acu.acu.c.ac00tsrm
o.ac00tsrm:	inc.h.sys_def
o.ac00tsrm:	inc.h.__config
o.ac00tsrm:	inc.h.risc_os
o.ac00tsrm:	inc.h.acu_def
o.ac00tsrm:	inc.h.acu_mem
o.ac00tsrm:	inc.h.ds_def
o.ac00tsrm:	inc.h.ds_mem
o.ac00tsrm:	inc.h.acu_pro
o.ac00tsrm:	inc.h.mt_pro
o.ac00tsrm:	inc.edf.v42
o.ac00tsrm:	inc.edf.dte
o.ac00tsrm:	inc.edf.dce_line
o.ac00tsa1:	acu.acu.c.ac00tsa1
o.ac00tsa1:	inc.h.sys_def
o.ac00tsa1:	inc.h.__config
o.ac00tsa1:	inc.h.risc_os
o.ac00tsa1:	inc.h.acu_def
o.ac00tsa1:	inc.h.acu_mem
o.ac00tsa1:	inc.h.ds_def
o.ac00tsa1:	inc.h.ds_mem
o.ac00tsa1:	inc.h.v25_def
o.ac00tsa1:	acu.acu.h.v25_mem
o.ac00tsa1:	inc.h.acu_pro
o.ac00tsa1:	inc.h.mt_pro
o.ac00tsa1:	inc.edf.v42
o.ac00tsa1:	inc.edf.dte
o.ac00tsa1:	inc.edf.dce_line
o.ac00tsa2:	acu.acu.c.ac00tsa2
o.ac00tsa2:	inc.h.sys_def
o.ac00tsa2:	inc.h.__config
o.ac00tsa2:	inc.h.risc_os
o.ac00tsa2:	inc.h.acu_def
o.ac00tsa2:	inc.h.acu_mem
o.ac00tsa2:	inc.h.ds_def
o.ac00tsa2:	inc.h.ds_mem
o.ac00tsa2:	inc.h.v25_def
o.ac00tsa2:	acu.acu.h.v25_mem
o.ac00tsa2:	inc.h.acu_pro
o.ac00tsa2:	inc.h.mt_pro
o.ac00tsa2:	inc.edf.v42
o.ac00tsa2:	inc.edf.dte
o.ac00tsa2:	inc.edf.dce_line
o.ac00cmat:	acu.acu.c.ac00cmat
o.ac00cmat:	inc.h.sys_def
o.ac00cmat:	inc.h.__config
o.ac00cmat:	inc.h.risc_os
o.ac00cmat:	inc.h.ee_def
o.ac00cmat:	inc.h.acu_def
o.ac00cmat:	inc.h.acu_mem
o.ac00cmat:	inc.h.acu_pro
o.ac00cmat:	acu.acu.h.acu_vce
o.ac00cmat:	inc.h.mt_pro
o.ac00cmdt:	acu.acu.c.ac00cmdt
o.ac00cmdt:	inc.h.sys_def
o.ac00cmdt:	inc.h.__config
o.ac00cmdt:	inc.h.risc_os
o.ac00cmdt:	inc.h.acu_def
o.ac00cmdt:	inc.h.acu_pro
o.ac00cmdt:	inc.h.par_def
o.ac00cmdt:	inc.h.par_pro
o.ac00cman:	acu.acu.c.ac00cman
o.ac00cman:	inc.h.sys_def
o.ac00cman:	inc.h.__config
o.ac00cman:	inc.h.risc_os
o.ac00cman:	inc.h.acu_def
o.ac00cman:	inc.h.acu_mem
o.ac00cman:	inc.h.ee_def
o.ac00cman:	inc.h.acu_pro
o.ac00cman:	inc.h.mt_pro
o.ac00cman:	acu.acu.h.v25_pro
o.ac00cman:	acu.acu.h.ll_pro
o.ac00cman:	acu.acu.h.acu_vce
o.ac00cman:	acu.acu.h.acu_epro
o.ac00cmav:	acu.acu.c.ac00cmav
o.ac00cmav:	inc.h.sys_def
o.ac00cmav:	inc.h.__config
o.ac00cmav:	inc.h.risc_os
o.ac00cmav:	inc.h.acu_def
o.ac00cmav:	inc.h.acu_mem
o.ac00cmav:	inc.h.ee_def
o.ac00cmav:	inc.h.acu_pro
o.ac00cmav:	inc.h.mt_pro
o.ac00cmaw:	acu.acu.c.ac00cmaw
o.ac00cmaw:	inc.h.sys_def
o.ac00cmaw:	inc.h.__config
o.ac00cmaw:	inc.h.risc_os
o.ac00cmaw:	inc.h.acu_def
o.ac00cmaw:	inc.h.acu_mem
o.ac00cmaw:	inc.h.ee_def
o.ac00cmaw:	inc.h.acu_pro
o.ac00cmaw:	inc.h.mt_pro
o.ac00cmex:	acu.acu.c.ac00cmex
o.ac00cmex:	inc.h.sys_def
o.ac00cmex:	inc.h.__config
o.ac00cmex:	inc.h.risc_os
o.ac00cmex:	inc.h.acu_def
o.ac00cmex:	inc.h.acu_mem
o.ac00cmex:	inc.h.acu_pro
o.ac00cmex:	acu.acu.h.ll_pro
o.ac00cmex:	acu.acu.h.v25_pro
o.ac00cmex:	inc.h.mt_pro
o.ac00cmts:	acu.acu.c.ac00cmts
o.ac00cmts:	inc.h.sys_def
o.ac00cmts:	inc.h.__config
o.ac00cmts:	inc.h.risc_os
o.ac00cmts:	inc.h.acu_def
o.ac00cmts:	inc.h.acu_mem
o.ac00cmts:	inc.h.acu_pro
o.ac00cmts:	inc.h.mt_pro
o.ac00sys0:	acu.acu.c.ac00sys0
o.ac00sys0:	inc.h.sys_def
o.ac00sys0:	inc.h.__config
o.ac00sys0:	inc.h.risc_os
o.ac00sys0:	inc.h.acu_def
o.ac00sys0:	inc.h.acu_mem
o.ac00sys0:	inc.h.acu_pro
o.ac00sys0:	inc.h.mt_pro
o.ac00sys0:	inc.edf.v42
o.ac00sys0:	inc.edf.dce_line
o.ac00sys0:	inc.edf.dte
o.ac00sys0:	acu.acu.h.acu_epro
o.ac00sys1:	acu.acu.c.ac00sys1
o.ac00sys1:	inc.h.sys_def
o.ac00sys1:	inc.h.__config
o.ac00sys1:	inc.h.risc_os
o.ac00sys1:	inc.h.acu_def
o.ac00sys1:	inc.h.acu_mem
o.ac00cust:	acu.acu.c.ac00cust
o.ac00cust:	inc.h.sys_def
o.ac00cust:	inc.h.__config
o.ac00cust:	inc.h.risc_os
o.ac00cust:	inc.h.acu_def
o.ac00cust:	inc.h.acu_mem
o.ac00cust:	inc.h.acu_pro
o.ac00cust:	inc.h.par_def
o.ac00cust:	inc.h.par_pro
o.ac00cust:	inc.^.dp_crus.h.mt_voice
o.ac00cust:	inc.^.dp_crus.h.mt_gpio
o.ac00cust:	inc.^.dp_crus.h.mt_coext
o.ac00cust:	inc.^.dp_crus.h.mt_codef
o.ac00cust:	inc.^.dp_crus.h.mt_copro
o.ac00cust:	inc.^.dp_crus.h.dsp_drv
