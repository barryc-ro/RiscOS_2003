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
#           type 'make -f etc'      to add various ACU configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

ACU_ETC      = ac00etc.o ac00emem.o ac00elib.o ac00cmet.o
ACU_ETC_STUB = etc_stub.o

#
#  Tools flags
#
LPATH  = acu.etc.
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)acu.acu $(CFLAGS)
VPATH  = inc acu.acu $(LPATH)
                    
# Compile ACU's ETC sub-module
etc:        $(ACU_ETC)
            #@@ echo $(ACU_ETC:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

# Stu ACU's ETC sub-module
!etc:       $(ACU_ETC_STUB)
            #@@ echo $(ACU_ETC_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: etc.mak $(ACU_ETC:S/obj/c) $(LPATH)*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(ACU_ETC_STUB:S/obj/c) etc.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

# 
# ACU ETC Driver
#
ac00emem.o:  ac00emem.c sys_def.h acu_edef.h acu_emem.h \
               acu_def.h acu_mem.h
               $(CC) $(CFLGS) $(LPATH)ac00emem.c

ac00etc.o:   ac00etc.c sys_def.h acu_emem.h acu_edef.h mt_pro.h \
               acu_def.h acu_mem.h 
               $(CC) $(CFLGS) $(LPATH)ac00etc.c

ac00elib.o:  ac00elib.c sys_def.h acu_edef.h acu_emem.h \
               acu_def.h acu_mem.h
               $(CC) $(CFLGS) $(LPATH)ac00elib.c

ac00cmet.o:  ac00cmet.c sys_def.h acu_def.h acu_mem.h acu_edef.h acu_emem.h \
               par_def.h par_pro.h
               $(CC) $(CFLGS) $(LPATH)ac00cmet.c

# 
# ACU ETC Driver Stub
#
etc_stub.o:  etc_stub.c sys_def.h
               $(CC) $(CFLGS) $(LPATH)etc_stub.c


# Dynamic dependencies:
o.etc_stub:	acu.etc.c.etc_stub
o.etc_stub:	inc.h.sys_def
o.etc_stub:	inc.h.__config
o.etc_stub:	inc.h.risc_os
