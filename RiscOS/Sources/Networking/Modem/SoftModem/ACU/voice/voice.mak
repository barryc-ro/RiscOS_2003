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
#           type 'make -f voice'      to add various ACU configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

# Digital Voice (Rockwell and IS-101) ACU Driver code
ACU_VOICE      = ac00vc1.o ac00vc2.o ac00cmvc.o ac00cmv1.o ac00cmv2.o
ACU_VOICE_STUB = vce_stub.o

#
#  Tools flags
#
LPATH  = acu.voice
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)acu.acu $(CFLAGS)
VPATH  = inc acu.acu $(LPATH)
                    
# Compile ACU digital Voice driver
voice:      $(ACU_VOICE)
            #@@ echo $(ACU_VOICE:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

!voice:     $(ACU_VOICE_STUB)
            #@@ echo $(ACU_VOICE_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

#release: voice.mak $(ACU_VOICE:S/obj/c) $(LPATH).*.h
release: voice.mak $(ACU_VOICE:S/obj/c)
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(ACU_VOICE_STUB:S/obj/c) voice.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#  ACU Digital Voice Driver Source Code
#
ac00cmvc.o:  ac00cmvc.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vce.h \
               vce_pro.h mt_pro.h fp_pro.h par_def.h par_pro.h 
               $(CC) $(CFLGS) $(LPATH).ac00cmvc.c

ac00cmv1.o:  ac00cmv1.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vce.h \
               vce_pro.h fp_pro.h par_def.h par_pro.h 
               $(CC) $(CFLGS) $(LPATH).ac00cmv1.c

ac00cmv2.o:  ac00cmv2.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vce.h \
               vce_pro.h fp_pro.h par_def.h par_pro.h 
               $(CC) $(CFLGS) $(LPATH).ac00cmv2.c

ac00vc1.o:	ac00vc1.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vce.h\
               vce_pro.h fp_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00vc1.c

ac00vc2.o:	ac00vc2.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vce.h\
               vce_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00vc2.c
#
#  ACU Digital Voice Driver Source Code Stub
#
vce_stub.o:  vce_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH).vce_stub.c


# Dynamic dependencies:
o.vce_stub:	acu.voice.c.vce_stub
o.vce_stub:	inc.h.sys_def
o.vce_stub:	inc.h.__config
o.vce_stub:	inc.h.risc_os
