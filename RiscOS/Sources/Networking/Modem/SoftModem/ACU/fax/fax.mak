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
#           type 'make -f fax'      to add various ACU configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

# FAX Class 1,2,2.0,8,80 ACU Driver code
ACU_FAX      = ac00fx1.o ac00fx2.o ac00cmfx.o ac00cmf1.o ac00cmf2.o \
               ac00cm20.o
ACU_FAX_STUB = fax_stub.o 

#
#  Tools flags
#
LPATH  = acu.fax
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)acu.acu $(CFLAGS)
VPATH  = inc acu.acu $(LPATH)
                    
# Compile ACU FAX driver
fax:        $(ACU_FAX)
            #@@ echo $(ACU_FAX:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

!fax:       $(ACU_FAX_STUB)
            #@@ echo $(ACU_FAX_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: fax.mak $(ACU_FAX:S/obj/c) $(LPATH)\*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(ACU_FAX_STUB:S/obj/c) fax.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#  ACU FAX Driver Source Code
#
ac00fx1.o:	ac00fx1.c sys_def.h acu_def.h acu_pro.h acu_fax.h fax_pro.h \
               mt_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00fx1.c

ac00fx2.o:	ac00fx2.c sys_def.h acu_def.h acu_pro.h acu_fax.h fax_pro.h \
               mt_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00fx2.c

ac00cmfx.o:  ac00cmfx.c sys_def.h acu_def.h acu_mem.h acu_pro.h acu_vce.h \
               par_def.h par_pro.h
               $(CC) $(CFLGS) $(LPATH).ac00cmfx.c

ac00cmf1.o:  ac00cmf1.c sys_def.h acu_def.h acu_mem.h acu_pro.h par_def.h 
               $(CC) $(CFLGS) $(LPATH).ac00cmf1.c

ac00cmf2.o:  ac00cmf2.c sys_def.h acu_def.h acu_mem.h acu_pro.h par_def.h
               $(CC) $(CFLGS) $(LPATH).ac00cmf2.c

ac00cm20.o:  ac00ãm20.c sys_def.h acu_def.h acu_mem.h acu_pro.h par_def.h
               $(CC) $(CFLGS) $(LPATH).ac00cm20.c

#
#  ACU FAX Driver Source Code Stub
#
fax_stub.o:	fax_stub.c sys_def.h
               $(CC) $(CFLGS) $(LPATH)\fax_stub.c


# Dynamic dependencies:
o.ac00fx1:	acu.fax.c.ac00fx1
o.ac00fx1:	inc.h.sys_def
o.ac00fx1:	inc.h.__config
o.ac00fx1:	inc.h.risc_os
o.ac00fx1:	inc.h.acu_def
o.ac00fx1:	inc.h.acu_mem
o.ac00fx1:	inc.h.acu_pro
o.ac00fx1:	acu.fax.h.acu_fax
o.ac00fx1:	inc.h.fax_pro
o.ac00fx1:	inc.h.mt_pro
o.ac00fx2:	acu.fax.c.ac00fx2
o.ac00fx2:	inc.h.sys_def
o.ac00fx2:	inc.h.__config
o.ac00fx2:	inc.h.risc_os
o.ac00fx2:	inc.h.acu_def
o.ac00fx2:	inc.h.acu_mem
o.ac00fx2:	inc.h.acu_pro
o.ac00fx2:	acu.fax.h.acu_fax
o.ac00fx2:	inc.h.fax_pro
o.ac00fx2:	inc.h.mt_pro
o.ac00cmfx:	acu.fax.c.ac00cmfx
o.ac00cmfx:	inc.h.sys_def
o.ac00cmfx:	inc.h.__config
o.ac00cmfx:	inc.h.risc_os
o.ac00cmfx:	inc.h.acu_def
o.ac00cmfx:	acu.fax.h.acu_fax
o.ac00cmfx:	inc.h.acu_mem
o.ac00cmfx:	inc.h.acu_pro
o.ac00cmfx:	acu.acu.h.acu_vce
o.ac00cmfx:	acu.acu.h.acu_vcv
o.ac00cmfx:	inc.h.mt_pro
o.ac00cmfx:	inc.h.fp_pro
o.ac00cmfx:	inc.h.fax_pro
o.ac00cmfx:	inc.h.par_def
o.ac00cmfx:	inc.h.par_pro
o.ac00cmf1:	acu.fax.c.ac00cmf1
o.ac00cmf1:	inc.h.sys_def
o.ac00cmf1:	inc.h.__config
o.ac00cmf1:	inc.h.risc_os
o.ac00cmf1:	inc.h.acu_def
o.ac00cmf1:	acu.fax.h.acu_fax
o.ac00cmf1:	inc.h.fax_pro
o.ac00cmf1:	inc.h.par_def
o.ac00cmf1:	inc.h.par_pro
o.ac00cmf2:	acu.fax.c.ac00cmf2
o.ac00cmf2:	inc.h.sys_def
o.ac00cmf2:	inc.h.__config
o.ac00cmf2:	inc.h.risc_os
o.ac00cmf2:	inc.h.acu_def
o.ac00cmf2:	acu.fax.h.acu_fax
o.ac00cmf2:	inc.h.fax_pro
o.ac00cmf2:	inc.h.par_def
o.ac00cmf2:	inc.h.par_pro
o.ac00cm20:	acu.fax.c.ac00cm20
o.ac00cm20:	inc.h.sys_def
o.ac00cm20:	inc.h.__config
o.ac00cm20:	inc.h.risc_os
o.ac00cm20:	inc.h.acu_def
o.ac00cm20:	acu.fax.h.acu_fax
o.ac00cm20:	inc.h.fax_pro
o.ac00cm20:	inc.h.par_def
o.ac00cm20:	inc.h.par_pro
