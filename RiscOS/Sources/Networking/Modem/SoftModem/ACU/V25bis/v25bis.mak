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
#           type 'make -f v25bis'      to add various ACU configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

ACU_V25BIS =   v200id.o v200ps.o v200pr.o v200hn.o v200on.o \
               v200mem.o v200lib.o v200ts.o v200v24.o v200sta0.o \
               v200sta1.o v200cmd.o
ACU_V25BIS_STUB = v25_stub.o 

#
#  Tools flags
#
LPATH  = acu.v25bis
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)acu.acu $(CFLAGS)
VPATH  = inc acu.acu $(LPATH)
                    
v25bis:    $(ACU_V25BIS)
            #@@ echo $(ACU_V25BIS:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

!v25bis:    $(ACU_V25BIS_STUB)
            #@@ echo $(ACU_V25BIS_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

#release: v25bis.mak $(ACU_V25BIS:S/obj/c) $(LPATH).*.h
release: v25bis.mak $(ACU_V25BIS:S/obj/c)
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(ACU_V25BIS_STUB:S/obj/c) v25bis.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#  ACU V.25bis Driver Source Code
#
v200id.o:	   v200id.c sys_def.h v25_mem.h ee_def.h acu_def.h v25_def.h \
               v25_pro.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH).v200id.c

v200ps.o:	   v200ps.c sys_def.h v25_mem.h acu_def.h v25_def.h \
               acu_pro.h mt_pro.h v25_pro.h
               $(CC) $(CFLGS) $(LPATH).v200ps.c

v200pr.o:	   v200pr.c sys_def.h v25_mem.h acu_def.h   v25_def.h\
               v25_pro.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH).v200pr.c

v200hn.o:	   v200hn.c sys_def.h v25_mem.h acu_def.h   v25_def.h\
               v25_pro.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH).v200hn.c

v200on.o:	   v200on.c sys_def.h v25_mem.h acu_def.h v25_def.h  \
               v25_pro.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH).v200on.c

v200lib.o:	v200lib.c sys_def.h v25_mem.h ee_def.h acu_def.h v25_def.h\
               acu_pro.h mt_pro.h v25_pro.h
               $(CC) $(CFLGS) $(LPATH).v200lib.c

v200ts.o:	   v200ts.c sys_def.h acu_def.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH).v200ts.c

v200v24.o:	v200v24.c sys_def.h acu_def.h acu_pro.h mt_pro.h v25_pro.h
               $(CC) $(CFLGS) $(LPATH).v200v24.c

v200sta0.o:	v200sta0.c sys_def.h acu_def.h acu_pro.h fp_pro.h v25_pro.h
               $(CC) $(CFLGS) $(LPATH).v200sta0.c

v200sta1.o:	v200sta1.c sys_def.h acu_def.h acu_pro.h fp_pro.h v25_pro.h
               $(CC) $(CFLGS) $(LPATH).v200sta1.c

v200mem.o:	v200mem.c sys_def.h v25_def.h acu_def.h ee_def.h v25_mem.h\
               acu_pro.h mt_pro.h v25_pro.h
               $(CC) $(CFLGS) $(LPATH).v200mem.c

v200cmd.o:	v200cmd.c sys_def.h v25_mem.h ee_def.h acu_def.h v25_def.h  \
               mt_pro.h v25_pro.h acu_pro.h
               $(CC) $(CFLGS) $(LPATH).v200cmd.c

#
#  ACU V25bis Source Code Stub
#
v25_stub.o:	v25_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH).v25_stub.c


# Dynamic dependencies:
o.v25_stub:	acu.v25bis.c.v25_stub
o.v25_stub:	inc.h.sys_def
o.v25_stub:	inc.h.__config
o.v25_stub:	inc.h.risc_os
o.v25_stub:	inc.h.v25_def
