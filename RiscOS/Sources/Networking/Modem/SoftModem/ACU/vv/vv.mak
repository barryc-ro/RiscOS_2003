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
#           type 'make -f vv'      to add various ACU configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

ACU_VOICEVIEW      = ac00mmvv.o ac00vcv.o ac00cmvv.o ac00anvv.o \
                     ac00idvv.o ac00prvv.o ac00orvv.o ac00hnvv.o \
                     ac00onvv.o
ACU_VOICEVIEW_STUB = vv_stub.o

#
#  Tools flags
#
LPATH  = acu.vv
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)acu.acu $(CFLAGS)
VPATH  = inc acu.acu $(LPATH)
                    
# ACU voiceview driver
vv:        $(ACU_VOICEVIEW)
            #@@ echo $(ACU_VOICEVIEW:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

!vv:        $(ACU_VOICEVIEW_STUB)
            #@@ echo $(ACU_VOICEVIEW_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: vv.mak $(ACU_VOICEVIEW:S/obj/c) $(LPATH).*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(ACU_VOICEVIEW_STUB:S/obj/c) vv.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#  ACU Voice View Driver Source Code
#
ac00mmvv.o:	ac00mmvv.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH).ac00mmvv.c

ac00vcv.o:	ac00vcv.c sys_def.h acu_def.h acu_mem.h acu_pro.h mt_pro.h \
               acu_vcv.h acu_mmvv.h vv_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00vcv.c

ac00cmvv.o:  ac00cmvv.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vcv.h \
               acu_mmvv.h par_def.h par_pro.h vv_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00cmvv.c

ac00anvv.o:	ac00anvv.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vcv.h \
               mt_pro.h vv_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00anvv.c

ac00idvv.o:	ac00idvv.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vcv.h \
               mt_pro.h vv_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00idvv.c

ac00prvv.o:	ac00prvv.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vcv.h 
               $(CC) $(CFLGS) $(LPATH).ac00prvv.c

ac00orvv.o:	ac00orvv.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vcv.h \
               mt_pro.h vv_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00orvv.c

ac00onvv.o:	ac00onvv.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vcv.h \
               mt_pro.h vv_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00onvv.c

ac00hnvv.o:	ac00hnvv.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vcv.h \
               mt_pro.h 
               $(CC) $(CFLGS) $(LPATH).ac00hnvv.c

#
#  ACU Voice View Driver Source Code Stub
#
vv_stub.o:	vv_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH).vv_stub.c


# Dynamic dependencies:
o.vv_stub:	acu.vv.c.vv_stub
o.vv_stub:	inc.h.sys_def
o.vv_stub:	inc.h.__config
o.vv_stub:	inc.h.risc_os
