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
#           type 'make -f wireless'      to add various ACU configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

# Wireless Modem Driver code
ACU_WIRELESS =      wl00if.o wl00lib.o wl00mem.o wl00cmd.o wl00sta0.o \
                    wl00sta1.o wl00id.o wl00pr.o wl00or.o wl00orps.o \
                    wl00an.o wl00on.o wl00hn.o ac00phon.o wl00cmpc.o \
                    wl00pin.o wl00tab.o wl00fx1.o ac00slep.o
ACU_WIRELESS_STUB = wl_stub.o

#
#  Tools flags
#
LPATH  = acu.wireless
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)acu.acu $(CFLAGS)
VPATH  = inc acu.acu $(LPATH)
                    
# ACU wireless driver
wireless:   $(ACU_WIRELESS)
            #@@ echo $(ACU_WIRELESS:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

# Stub ACU wireless driver
!wireless:   $(ACU_WIRELESS_STUB)
            #@@ echo $(ACU_WIRELESS_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: wireless.mak $(ACU_WIRELESS:S/obj/c) $(LPATH).*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(ACU_WIRELESS_STUB:S/obj/c) wireless.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

# 
# ACU Wireless Modem Driver
#
ac00phon.o:  ac00phon.c sys_def.h acu_def.h acu_mem.h acu_pro.h fp_pro.h \
               wl_pro.h 
               $(CC) $(CFLGS) $(LPATH).ac00phon.c

ac00slep.o:  ac00slep.c sys_def.h  acu_def.h acu_pro.h acu_mem.h
               $(CC) $(CFLGS) $(LPATH).ac00slep.c

wl00if.o:    wl00if.c sys_def.h acu_def.h wl_def.h wl_mem.h wl_pro.h \
               wbiosmsg.h audio_if.h 
               $(CC) $(CFLGS) $(LPATH).wl00if.c

wl00lib.o:   wl00lib.c sys_def.h acu_def.h v25_def.h ee_def.h acu_mem.h \
               mt_pro.h wl_def.h wl_mem.h wbios.h wbiosmsg.h audio_if.h
               $(CC) $(CFLGS) $(LPATH).wl00lib.c

wl00or.o:    wl00or.c sys_def.h acu_def.h acu_mem.h acu_pro.h mt_pro.h \
               wl_def.h wl_pro.h wl_mem.h audio_if.h 
               $(CC) $(CFLGS) $(LPATH).wl00or.c

wl00orps.o:  wl00orps.c sys_def.h acu_def.h acu_mem.h v25_def.h v25_mem.h \
               ee_def.h acu_pro.h mt_pro.h wl_def.h wl_mem.h wl_pro.h \
               audio_if.h wbios.h 
               $(CC) $(CFLGS) $(LPATH).wl00orps.c

wl00on.o:    wl00on.c sys_def.h acu_def.h acu_mem.h v25_def.h v25_mem.h \
               acu_pro.h mt_pro.h wl_def.h wl_pro.h 
               $(CC) $(CFLGS) $(LPATH).wl00on.c

wl00hn.o:    wl00hn.c sys_def.h acu_def.h acu_mem.h acu_pro.h wl_def.h \
               wl_mem.h wl_pro.h 
               $(CC) $(CFLGS) $(LPATH).wl00hn.c

wl00id.o:    wl00id.c sys_def.h acu_def.h acu_mem.h acu_pro.h mt_pro.h \
               wl_def.h wl_mem.h wl_pro.h 
               $(CC) $(CFLGS) $(LPATH).wl00id.c

wl00pr.o:    wl00pr.c sys_def.h acu_def.h acu_mem.h acu_pro.h wl_def.h \
               wl_pro.h audio_if.h par_def.h par_pro.h 
               $(CC) $(CFLGS) $(LPATH).wl00pr.c

wl00an.o:    wl00an.c sys_def.h acu_def.h acu_mem.h acu_pro.h mt_pro.h \
               acu_epro.h wl_pro.h wl_def.h 
               $(CC) $(CFLGS) $(LPATH).wl00an.c

wl00mem.o:   wl00mem.c sys_def.h wl_def.h 
               $(CC) $(CFLGS) $(LPATH).wl00mem.c

wl00sta0.o:  wl00sta0.c sys_def.h acu_def.h acu_mem.h acu_pro.h fp_pro.h \
               wl_pro.h 
               $(CC) $(CFLGS) $(LPATH).wl00sta0.c

wl00sta1.o:  wl00sta1.c sys_def.h acu_def.h acu_mem.h acu_pro.h fp_pro.h \
               wl_pro.h 
               $(CC) $(CFLGS) $(LPATH).wl00sta1.c

wl00cmd.o:   wl00cmd.c sys_def.h acu_def.h acu_pro.h acu_mem.h par_def.h \
               par_pro.h wl_def.h wl_mem.h audio_if.h wbios.h 
               $(CC) $(CFLGS) $(LPATH).wl00cmd.c

wl00cmpc.o:  wl00cmpc.c sys_def.h acu_def.h acu_pro.h acu_mem.h par_def.h \
               par_pro.h wl_def.h wl_mem.h wbios.h 
               $(CC) $(CFLGS) $(LPATH).wl00cmpc.c

wl00pin.o:   wl00pin.c sys_def.h acu_def.h acu_pro.h acu_mem.h \
               wl_def.h wl_mem.h wl_pro.h dte.edf
               $(CC) $(CFLGS) $(LPATH).wl00pin.c

wl00tab.o:   wl00tab.c sys_def.h acu_def.h 
               $(CC) $(CFLGS) $(LPATH).wl00tab.c

wl00fx1.o:	wl00fx1.c sys_def.h acu_def.h acu_pro.h acu_fax.h fax_pro.h \
               mt_pro.h wl_def.h wl_mem.h
               $(CC) $(CFLGS) $(LPATH).wl00fx1.c

# 
# ACU Wireless Modem Driver Stub
#
wl_stub.o:  wl_stub.c sys_def.h
               $(CC) $(CFLGS) $(LPATH).wl_stub.c 


# Dynamic dependencies:
o.wl_stub:	acu.wireless.c.wl_stub
o.wl_stub:	inc.h.sys_def
o.wl_stub:	inc.h.__config
o.wl_stub:	inc.h.risc_os
