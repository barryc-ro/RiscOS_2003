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
#           type 'make -f leasdln'      to add various ACU configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

# Leased Line ACU Driver code
ACU_LEASED_LINE = ll00lib.o ll00id.o ll00pr.o ll00or.o ll00an.o \
                  ll00on.o ll00sta0.o ll00sta1.o ll00hn.o ll00cmd.o
ACU_LL_STUB     = ll_stub.o


#
#  Tools flags
#
LPATH  = acu.leasedln
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)acu.acu $(CFLAGS)
VPATH  = inc acu.acu $(LPATH)
 
# Compile ACU Leased Line Support

# Compile Leased Line code
leasedln:   $(ACU_LEASED_LINE)
            #@@ echo $(ACU_LEASED_LINE:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

!leasedln:   $(ACU_LL_STUB)
            #@@ echo $(ACU_LL_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: leasedln.mak $(ACU_LEASED_LINE:S/obj/c) $(LPATH).*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(ACU_LL_STUB:S/obj/c) leasedln.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#  ACU Leased Line Driver Source Code
#
ll00lib.o:	ll00lib.c sys_def.h acu_pro.h acu_def.h
               $(CC) $(CFLGS) $(LPATH).ll00lib.c

ll00id.o:	   ll00id.c sys_def.h acu_def.h ll_mem.h ll_pro.h
               $(CC) $(CFLGS) $(LPATH).ll00id.c

ll00pr.o:	   ll00pr.c sys_def.h acu_def.h ll_mem.h ll_pro.h
               $(CC) $(CFLGS) $(LPATH).ll00pr.c

ll00or.o:	   ll00or.c sys_def.h acu_def.h ll_mem.h ds_def.h ds_mem.h\
               acu_pro.h mt_pro.h ll_pro.h acu_epro.h
               $(CC) $(CFLGS) $(LPATH).ll00or.c

ll00an.o:	   ll00an.c sys_def.h acu_def.h ll_mem.h ds_def.h ds_mem.h\
               acu_pro.h mt_pro.h ll_pro.h acu_epro.h
               $(CC) $(CFLGS) $(LPATH).ll00an.c

ll00on.o:	   ll00on.c sys_def.h acu_def.h ll_mem.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH).ll00on.c

ll00hn.o:	   ll00hn.c sys_def.h acu_def.h ll_mem.h acu_pro.h mt_pro.h
               $(CC) $(CFLGS) $(LPATH).ll00hn.c

ll00cmd.o:	ll00cmd.c sys_def.h acu_def.h ll_pro.h
               $(CC) $(CFLGS) $(LPATH).ll00cmd.c

ll00sta0.o:	ll00sta0.c sys_def.h acu_def.h acu_pro.h ll_pro.h fp_pro.h
               $(CC) $(CFLGS) $(LPATH).ll00sta0.c

ll00sta1.o:	ll00sta1.c sys_def.h acu_def.h acu_pro.h ll_pro.h fp_pro.h
               $(CC) $(CFLGS) $(LPATH).ll00sta1.c


#
#  ACU Leased Line Driver Source Code Stub
#
ll_stub.o:  ll_stub.c sys_def.h
               $(CC) $(CFLGS) $(LPATH).ll_stub.c


# Dynamic dependencies:
o.ll_stub:	acu.leasedln.c.ll_stub
o.ll_stub:	inc.h.sys_def
o.ll_stub:	inc.h.__config
o.ll_stub:	inc.h.risc_os
