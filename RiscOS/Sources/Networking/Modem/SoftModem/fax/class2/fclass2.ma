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
#           type 'make -f fclass2'       to add Class 2
#           type 'make -f fclass2 stub'  to add Class 2 stubs
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
#  FAX Class 2 Objects
#
FAX2 =  f2x00cm.o f2x00t30.o f2x00pct.o f2x00pcr.o\
        f2x00lib.o f2x00mem.o f2x00ov.o f2x00ct.o f2x00cct.o\
        f2x00ccr.o  f2x00pg.o

FAX2STUB = f2xstb.o


#
#  Tools flags
#
LPATH  = fax.class2
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CFLAGS)
VPATH  = inc $(LPATH)

#
#  Main executable
#
fx2: $(FAX2) 
        #@@ echo $(FAX2:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

stub: $(FAX2STUB)
        #@@ echo $(FAX2STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: $(FAX2:S/obj/c) fclass2.mak  $(LPATH).*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(FAX2STUB:S/obj/c) fclass2.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#  FAX Class 2 Source
#

f2x00cm.o:    f2x00cm.c sys_def.h f2x_def.h f2x_mem.h f2x_pro.h
                $(CC) $(CFLGS) $(LPATH).f2x00cm.c
f2x00t30.o:   f2x00t30.c sys_def.h f2x_def.h f2x_mem.h f2x_pro.h  f2x_ecd.h
                $(CC) $(CFLGS) $(LPATH).f2x00t30.c
f2x00pct.o:   f2x00pct.c sys_def.h f2x_def.h f2x_mem.h f2x_pro.h
                $(CC) $(CFLGS) $(LPATH).f2x00pct.c
f2x00pcr.o:   f2x00pcr.c sys_def.h f2x_def.h f2x_mem.h f2x_pro.h
                $(CC) $(CFLGS) $(LPATH).f2x00pcr.c
f2x00lib.o:   f2x00lib.c sys_def.h f2x_def.h f2x_mem.h f2x_pro.h
                $(CC) $(CFLGS) $(LPATH).f2x00lib.c
f2x00mem.o:   f2x00mem.c sys_def.h f2x_def.h f2x_mem.h f2x_pro.h
                $(CC) $(CFLGS) $(LPATH).f2x00mem.c
f2x00ov.o:    f2x00ov.c sys_def.h f2x_def.h
                $(CC) $(CFLGS) $(LPATH).f2x00ov.c
f2x00ct.o:    f2x00ct.c sys_def.h f2x_def.h
                $(CC) $(CFLGS) $(LPATH).f2x00ct.c
f2x00cct.o:   f2x00cct.c sys_def.h f2x_def.h
                $(CC) $(CFLGS) $(LPATH).f2x00cct.c
f2x00ccr.o:   f2x00ccr.c sys_def.h f2x_def.h
                $(CC) $(CFLGS) $(LPATH).f2x00ccr.c
f2x00pg.o:    f2x00pg.c sys_def.h
                $(CC) $(CFLGS) $(LPATH).f2x00pg.c

#
#  FAX Class 2 stub source
#

f2xstb.o:     f2xstb.c sys_def.h
                $(CC) $(CFLGS) $(LPATH).f2xstb.c


# Dynamic dependencies:
o.f2xstb:	fax.class2.c.f2xstb
o.f2xstb:	inc.h.sys_def
o.f2xstb:	inc.h.__config
o.f2xstb:	inc.h.risc_os
