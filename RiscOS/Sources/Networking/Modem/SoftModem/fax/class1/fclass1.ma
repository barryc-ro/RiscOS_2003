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
#           type 'make -f fclass1'       to add Class1
#           type 'make -f fclass1 stub'  to add Class1 stubs
#

#
# Read global compiler directive file
#!include compiler.cfg

#
#  FAX Class 1 Objects
#
FAX1 =  f1x00cm.o f1x00cl.o f1x00lib.o f1x00mem.o f1x00ov.o

FAX1STUB = f1xstb.o


#                             
#  Tools flags
#
LPATH  = fax.class1
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CFLAGS)
VPATH  = inc $(LPATH)

#
#  Main executable
#
class1: $(FAX1) 
      #@@ echo $(FAX1:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

stub: $(FAX1STUB) 
       #@@ echo $(FAX1STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: fclass1.mak $(FAX1:S/obj/c) $(LPATH).*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(FAX1STUB:S/obj/c) fclass1.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#  FAX Class 1 Source
#

f1x00cm.o:    f1x00cm.c sys_def.h f1x_def.h f1x_mem.h f1x_pro.h
                $(CC) $(CFLGS) $(LPATH).f1x00cm.c
f1x00cl.o:    f1x00cl.c sys_def.h f1x_def.h f1x_mem.h f1x_pro.h
                $(CC) $(CFLGS) $(LPATH).f1x00cl.c
f1x00lib.o:   f1x00lib.c sys_def.h f1x_def.h f1x_mem.h f1x_pro.h
                $(CC) $(CFLGS) $(LPATH).f1x00lib.c
f1x00mem.o:   f1x00mem.c sys_def.h f1x_def.h
                $(CC) $(CFLGS) $(LPATH).f1x00mem.c
f1x00ov.o:    f1x00ov.c sys_def.h f1x_def.h
                $(CC) $(CFLGS) $(LPATH).f1x00ov.c


#
#  FAX Class 1 stub source
#

f1xstb.o:     f1xstb.c sys_def.h
                $(CC) $(CFLGS) $(LPATH).f1xstb.c


# Dynamic dependencies:
o.f1x00cm:	fax.class1.c.f1x00cm
o.f1x00cm:	inc.h.sys_def
o.f1x00cm:	inc.h.__config
o.f1x00cm:	inc.h.risc_os
o.f1x00cm:	fax.class1.h.f1x_def
o.f1x00cm:	fax.class1.h.f1x_mem
o.f1x00cm:	fax.class1.h.f1x_pro
o.f1x00cl:	fax.class1.c.f1x00cl
o.f1x00cl:	inc.h.sys_def
o.f1x00cl:	inc.h.__config
o.f1x00cl:	inc.h.risc_os
o.f1x00cl:	fax.class1.h.f1x_def
o.f1x00cl:	fax.class1.h.f1x_mem
o.f1x00cl:	fax.class1.h.f1x_pro
o.f1x00lib:	fax.class1.c.f1x00lib
o.f1x00lib:	inc.h.sys_def
o.f1x00lib:	inc.h.__config
o.f1x00lib:	inc.h.risc_os
o.f1x00lib:	fax.class1.h.f1x_def
o.f1x00lib:	fax.class1.h.f1x_mem
o.f1x00lib:	fax.class1.h.f1x_pro
o.f1x00mem:	fax.class1.c.f1x00mem
o.f1x00mem:	inc.h.sys_def
o.f1x00mem:	inc.h.__config
o.f1x00mem:	inc.h.risc_os
o.f1x00mem:	fax.class1.h.f1x_def
o.f1x00ov:	fax.class1.c.f1x00ov
o.f1x00ov:	inc.h.sys_def
o.f1x00ov:	inc.h.__config
o.f1x00ov:	inc.h.risc_os
o.f1x00ov:	fax.class1.h.f1x_def
