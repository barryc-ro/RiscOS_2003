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
#           type 'make -f fax_ecm' to compile ECM sub-module
#           type 'make -f fax_ecm stub' to compile ECM sub-module stubs
#
#

#
# Read global compiler directive file
!include compiler.cfg

#
#  FAX Class 2/2.0 Error Correcting Mode Source 
#
FAX2ECM = f2x00cec.obj f2x00ecm.obj f2x00b0m.obj\
          f2x00b1m.obj f2x00b2m.obj f2x00b3m.obj\
          f2x00emm.obj f2x00etx.obj f2x00bmn.obj f2x00cbm.obj\
          f2x00erd.obj f2x00erx.obj f2x00elb.obj

FAX2ECMSTUB = f2xecstb.obj


#
#  Tools Flags
#
LPATH  = fax\ecm
HPATH  = fax\class2
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)$(HPATH) $(CFLAGS)
VPATH  = inc;$(LPATH);$(HPATH)

#
#  Main executable
#
ecm: $(FAX2ECM) 
        @@ echo $(FAX2ECM:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

stub: $(FAX2ECMSTUB) 
        @@ echo $(FAX2ECMSTUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: $(FAX2ECM:S/obj/c) fax_ecm.mak  $(LPATH)\*.h
      @@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(FAX2ECMSTUB:S/obj/c) fax_ecm.mak 
      @@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)


#
#  FAX Class 2/2.0 ECM source
#

f2x00cbm.obj:   f2x00cbm.c sys_def.h f2x_bmd.h f2x_ecp.h
                $(CC) $(CFLGS) $(LPATH)\f2x00cbm.c

f2x00b0m.obj:   f2x00b0m.c sys_def.h f2x_bmd.h
                $(CC) $(CFLGS) $(LPATH)\f2x00b0m.c

f2x00b1m.obj:   f2x00b1m.c sys_def.h f2x_bmd.h
                $(CC) $(CFLGS) $(LPATH)\f2x00b1m.c

f2x00b2m.obj:   f2x00b2m.c sys_def.h f2x_bmd.h
                $(CC) $(CFLGS) $(LPATH)\f2x00b2m.c

f2x00b3m.obj:   f2x00b3m.c sys_def.h f2x_bmd.h
                $(CC) $(CFLGS) $(LPATH)\f2x00b3m.c

f2x00bmn.obj:   f2x00bmn.c sys_def.h f2x_emm.h f2x_ecd.h f2x_bmd.h f2x_ecp.h
                $(CC) $(CFLGS) $(LPATH)\f2x00bmn.c

f2x00cec.obj:   f2x00cec.c sys_def.h f2x_def.h f2x_ecd.h
                $(CC) $(CFLGS) $(LPATH)\f2x00cec.c

f2x00ecm.obj:   f2x00ecm.c sys_def.h f2x_def.h f2x_mem.h f2x_pro.h f2x_emm.h\
                f2x_ecd.h f2x_bmd.h f2x_ecp.h 
                $(CC) $(CFLGS) $(LPATH)\f2x00ecm.c

f2x00emm.obj:   f2x00emm.c sys_def.h f2x_ecd.h f2x_def.h f2x_bmd.h
                $(CC) $(CFLGS) $(LPATH)\f2x00emm.c

f2x00etx.obj:   f2x00etx.c sys_def.h f2x_ecp.h f2x_emm.h f2x_pro.h\
                f2x_mem.h f2x_bmd.h
                $(CC) $(CFLGS) $(LPATH)\f2x00etx.c

f2x00erd.obj:   f2x00erd.c sys_def.h f2x_ecd.h f2x_emm.h f2x_bmd.h f2x_pro.h\
                f2x_def.h f2x_mem.h f2x_ecp.h
                $(CC) $(CFLGS) $(LPATH)\f2x00erd.c

f2x00erx.obj:   f2x00erx.c sys_def.h f2x_emm.h f2x_ecd.h f2x_bmd.h f2x_ecp.h\
                f2x_pro.h f2x_def.h f2x_mem.h
                $(CC) $(CFLGS) $(LPATH)\f2x00erx.c

f2x00elb.obj:   f2x00elb.c sys_def.h f2x_def.h f2x_mem.h f2x_pro.h f2x_emm.h\
                f2x_ecd.h f2x_bmd.h f2x_ecp.h
                $(CC) $(CFLGS) $(LPATH)\f2x00elb.c


#
#  FAX Class 2/2.0 ECM stub
#

f2xecstb.obj:   f2xecstb.c sys_def.h
                $(CC) $(CFLGS) $(LPATH)\f2xecstb.c

