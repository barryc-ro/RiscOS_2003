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
#           type 'make -f fclass20'      to add Class 2.0
#           type 'make -f fclass20 stub' to add Class 2.0 stubs
#
#

#
# Read global compiler directive file
!include compiler.cfg

#
#  FAX Class 2.0 Objects
#
FAX20 = f20x00cm.obj

FAX20STUB = f20xstb.obj


#
#  Tools flags
#
LPATH  = fax\class20
HPATH  = fax\class2
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(HPATH) $(CFLAGS)
VPATH  = inc;$(LPATH);$(HPATH)

#
#  Main executable
#
f20: $(FAX20) 
        @@ echo $(FAX20:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

stub: $(FAX20STUB) 
        @@ echo $(FAX20STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

#release: $(FAX20:S/obj/c) fclass20.mak  $(LPATH)\*.h
release: $(FAX20:S/obj/c) fclass20.mak
      @@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(FAX20STUB:S/obj/c) fclass20.mak 
      @@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)


#
#  FAX Class 2.0 Source
#

f20x00cm.obj:   f20x00cm.c sys_def.h f2x_def.h f2x_mem.h f2x_pro.h
                $(CC) $(CFLGS) $(LPATH)\f20x00cm.c

#
#  FAX Class 2.0 stub source
#

f20xstb.obj:    f20xstb.c sys_def.h
                $(CC) $(CFLGS) $(LPATH)\f20xstb.c


