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
!include compiler.cfg

#
#  Released Include files 
#
INCS = a68302.h acu_def.h acu_mem.h acu_pro.h as_pro.h assert.h \
       audio_if.h dce_line.edf ds_def.h ds_mem.h ds_pro.h dte.edf \
       dteinc.asm dv_pro.h ee_def.h fax_pro.h faxioext.inc fp_pro.h \
       lineequ.asm ms_dos.h mt_pro.h par_def.h par_pro.h sys_def.h \
       tracemac.h v42.edf vce_pro.h wbios.h wbiosmsg.h


#                             
#  Tools flags
#
VPATH  = inc $(LPATH)

#
#  release builds 
#
release: $(INCS)
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

