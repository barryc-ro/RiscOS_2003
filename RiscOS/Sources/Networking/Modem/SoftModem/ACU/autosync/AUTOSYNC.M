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
#           type 'make -f autosync'      to add various ACU configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

# Hayes Autosync ACU Driver code
ACU_AUTOSYNC = ac00as.o
ACU_AS_STUB  = as_stub.o

#
#  Tools flags
#
LPATH  = acu.autosync.
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)acu.acu $(CFLAGS)
VPATH  = inc acu.acu $(LPATH)
                    
# Compile ACU AutoSync driver
autosync:   $(ACU_AUTOSYNC)
            #@@ echo $(ACU_AUTOSYNC:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

!autosync:  $(ACU_AS_STUB)
            #@@ echo $(ACU_AS_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: autosync.mak $(ACU_AUTOSYNC:S/obj/c) $(LPATH)*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(ACU_AS_STUB:S/obj/c) autosync.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#  ACU Hayes Autosync Driver Source Code
#
ac00as.o:    as_pro.h ac00as.c sys_def.h acu_def.h acu_pro.h acu_as.h \
               mt_pro.h  
               $(CC) $(CFLGS) $(LPATH)ac00as.c

#
#  ACU Hayes Autosync Driver Source Code Stub
#
as_stub.o:  as_stub.c sys_def.h
               $(CC) $(CFLGS) $(LPATH)as_stub.c


# Dynamic dependencies:
o.ac00as:	acu.autosync.c.ac00as
o.ac00as:	inc.h.sys_def
o.ac00as:	inc.h.__config
o.ac00as:	inc.h.acu_def
o.ac00as:	inc.h.acu_pro
o.ac00as:	acu.autosync.h.acu_as
o.ac00as:	inc.h.mt_pro
o.ac00as:	inc.h.fp_pro
o.ac00as:	inc.h.as_pro
o.ac00as:	inc.h.acu_mem
o.as_stub:	acu.autosync.c.as_stub
o.as_stub:	inc.h.sys_def
o.as_stub:	inc.h.__config
o.as_stub:	inc.h.risc_os
