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
#           RSA Modem STUB Module 
#
#	   PLATFORM:
#
#     SOFTWARE TOOLS:
#           OPUS MAKE make utility
# 
#     USEAGE: 
#           type 'make -f stub'      to add various STUB configurations
#
#

#
# Read global compiler directive file
#!include compiler.cfg

# Platform specific Dipswitch code
DIPSWITCH = 
DIPSWITCH_STUB = ds00stub.o ds00mem.o 


# Platform specific Front Panel code
FRONT_PANEL = 
FRONT_PANEL_STUB = fp00stub.o 


SYSTEM_STUB =  acuiostb.o v42_stub.o mt_stub.o pag_stub.o fax_stub.o \
               as_stub.o vce_stub.o vv_stub.o wbiosstb.o audiostb.o

STUBS = $(DIPSWITCH_STUB) $(FRONT_PANEL_STUB)

#
#  Tools flags
#
LPATH  = platform.stub
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CFLAGS)
VPATH  = inc $(LPATH)
 
# Compile stubs
stubs:    $(STUBS)
            #@@ echo $(STUBS:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

#release: stub.mak $(STUBS:S/obj/c) $(LPATH).*.h
release: stub.mak $(STUBS:S/obj/c)
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#  Front Panel Driver Source Code
#
fp00stub.o:	fp00stub.c sys_def.h acu_def.h acu_pro.h fp_pro.h
               $(CC) $(CFLGS) $(LPATH).fp00stub.c

#
#  Dip Switch Driver Source Code
#
ds00mem.o:	ds00mem.c sys_def.h
               $(CC) $(CFLGS) $(LPATH).ds00mem.c

ds00stub.o:	ds00stub.c sys_def.h
               $(CC) $(CFLGS) $(LPATH).ds00stub.c


# Dynamic dependencies:
o.ds00stub:	platform.stub.c.ds00stub
o.ds00stub:	inc.h.sys_def
o.ds00stub:	inc.h.__config
o.ds00stub:	inc.h.risc_os
o.ds00mem:	platform.stub.c.ds00mem
o.ds00mem:	inc.h.sys_def
o.ds00mem:	inc.h.__config
o.ds00mem:	inc.h.risc_os
o.fp00stub:	platform.stub.c.fp00stub
o.fp00stub:	inc.h.sys_def
o.fp00stub:	inc.h.__config
o.fp00stub:	inc.h.risc_os
