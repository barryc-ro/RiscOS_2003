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
#           type 'make -f phonebk'      to add various ACU configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

ACU_PHONEBOOK      = ac00phbk.o pb00mn.o pb00mem.o pb00lib.o
ACU_PHONEBOOK_STUB = pb_stub.o



#
#  Tools flags
#
LPATH  = acu.phonebk
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)acu.acu $(CFLAGS)
VPATH  = inc acu.acu $(LPATH)
                    
phonebk:    $(ACU_PHONEBOOK)
            #@@ echo $(ACU_PHONEBOOK:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

!phonebk:    $(ACU_PHONEBOOK_STUB)
            #@@ echo $(ACU_PHONEBOOK_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: phonebk.mak $(ACU_PHONEBOOK:S/obj/c) $(LPATH).*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(ACU_PHONEBOOK_STUB:S/obj/c) phonebk.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
# ACU Phonebook Submodule
#
ac00phbk.o:  ac00phbk.c sys_def.h acu_def.h acu_mem.h acu_pro.h v25_def.h \
               ee_def.h par_def.h par_pro.h phbk_def.h phbk_pro.h phbk_mem.h
               $(CC) $(CFLGS) $(LPATH).ac00phbk.c

pb00mn.o:    pb00mn.c sys_def.h acu_def.h acu_mem.h acu_pro.h ee_def.h \
               v25_def.h phbk_def.h phbk_pro.h phbk_mem.h
               $(CC) $(CFLGS) $(LPATH).pb00mn.c

pb00mem.o:   pb00mem.c sys_def.h acu_def.h acu_def.h acu_mem.h acu_pro.h \
               phbk_def.h phbk_pro.h phbk_mem.h
               $(CC) $(CFLGS) $(LPATH).pb00mem.c

pb00lib.o:   pb00lib.c sys_def.h acu_def.h acu_mem.h acu_pro.h ee_def.h \
               v25_def.h phbk_def.h phbk_pro.h phbk_mem.h
               $(CC) $(CFLGS) $(LPATH).pb00lib.c

#
# ACU Phonebook Submodule Stub
#
pb_stub.o:  pb_stub.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH).pb_stub.c


# Dynamic dependencies:
o.pb_stub:	acu.phonebk.c.pb_stub
o.pb_stub:	inc.h.sys_def
o.pb_stub:	inc.h.__config
o.pb_stub:	inc.h.risc_os
