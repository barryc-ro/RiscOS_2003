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
#           type 'make -f fax_qc'        to add Qualtiy Checking
#           type 'make -f fax_qc stub'   to add Qualtiy Checking
#
#

#
# Read global compiler directive file
!include compiler.cfg

#
#  FAX Class 2/2.0 Quailty Checking Objects (ROM based tables) 
#
FAX2QC =  f2x00qc.obj f2x00cq.obj
       
FAX2QCSTUB = f2xqcstb.obj


#
#  Tools Flags
#
LPATH  = fax\qc
HPATH  = fax\class2
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(HPATH) $(CFLAGS)
VPATH  = inc;$(LPATH);$(HPATH)

#
#  Main executable
#
qc:      $(FAX2QC) 
         @@ echo $(FAX2QC:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

stub:    $(FAX2QCSTUB) 
         @@ echo $(FAX2QCSTUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

#release: $(FAX2QC:S/obj/c) fax_qc.mak  $(LPATH)\*.h
release: $(FAX2QC:S/obj/c) fax_qc.mak
      @@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(FAX2QCSTUB:S/obj/c) fax_qc.mak 
      @@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)


#
#  FAX Class 2/2.0 quality checking module Source
#

f2x00qc.obj:    f2x00qc.c sys_def.h f2x_def.h
                $(CC) $(CFLGS) $(LPATH)\f2x00qc.c
f2x00cq.obj:    f2x00cq.c sys_def.h f2x_def.h
                $(CC) $(CFLGS) $(LPATH)\f2x00cq.c

#
#  FAX Class 2/2.0 QC stubs
#

f2xqcstb.obj:   f2xqcstb.c sys_def.h
                $(CC) $(CFLGS) $(LPATH)\f2xqcstb.c


