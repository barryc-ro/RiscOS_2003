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
#           RSA Modem FAX Module 
#
#	   PLATFORM:
#
#     SOFTWARE TOOLS:
#           OPUS MAKE make utility
# 
#     USEAGE: 
#           type 'make -f fax_if' for build instructions
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
#  FAX Interface objects 
#
FAX = fx00if.o 

SYSTEM_STUBS = sys_stub.o

#
#  Tools flags
#
LPATH  = fax.if
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CFLAGS)
VPATH  = inc $(LPATH)

#
#  Main Executable
#
fax_if: $(FAX) 
        #@@ echo $(FAX:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

sys_stub: $(SYSTEM_STUBS)
        #@@ echo $(SYSTEM_STUBS:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

#release: $(FAX:S/obj/c) $(SYSTEM_STUBS:S/obj/c) fax_if.mak  $(LPATH).*.h
release: $(FAX:S/obj/c) $(SYSTEM_STUBS:S/obj/c) fax_if.mak
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)


#
#  FAX module source
#

fx00if.o: fx00if.c sys_def.h
            $(CC) $(CFLGS) $(LPATH).fx00if.c

#
#  System Stub compiles
#

sys_stub.o: sys_stub.c sys_def.h
            $(CC) $(CFLGS) $(LPATH).sys_stub.c



# Dynamic dependencies:
o.fx00if:	fax.if.c.fx00if
o.fx00if:	inc.h.sys_def
o.fx00if:	inc.h.__config
o.fx00if:	inc.h.risc_os
