#
#     Copyright, 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995.
#     All Rights Reserved by:
#        RSA
#        7701 Six Forks Road
#        Suite 201
#        Raleigh, NC  27615
#        (919) 846-7171
#
#    This document contains material confidential to RSA. Its contents
#    must not be revealed, used or disclosed to anyone or company without
#    written permission by RSA. The information contained herein is solely
#    for the use of RSA.
#
#	MAKE FILE FOR:
#     RISC OS port
#
#  USEAGE: 
#       type 'make -f riscos'
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
#  RISC OS soft modem objects
#
SOFTMOD = riscosasm.o devicefs.o module.o modulehdr.o svcprint.o


#
#  Tools Flags
#
LPATH  = riscos
AFLGS  = $(AFLAGS)
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)dp_crus $(CFLAGS)
VPATH  = inc $(LPATH)
                    
#
#  Main Executable
#
riscos:   $(SOFTMOD)
      #@@ echo $(PARSER:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: riscos.mak $(PARSER:S/obj/c) $(LPATH).*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

# 
# RISC OS Soft Modem Source
#
riscosasm.o: $(LPATH).riscosasm.s
               $(AS) $(AFLGS) $(LPATH).riscosasm.s
               
rotest.o:    $(LPATH).rotest.c
               $(CC) $(CFLGS) $(LPATH).rotest.c

module.o:    $(LPATH).module.c $(LPATH).modulehdr.h
               $(CC) $(CFLGS) $(LPATH).module.c

devicefs.o:  $(LPATH).devicefs.c
               $(CC) $(CFLGS) $(LPATH).devicefs.c

svcprint.o:  $(LPATH).svcprint.c
               $(CC) $(CFLGS) $(LPATH).svcprint.c

modulehdr.o:  $(LPATH).cmhg.modulehdr
               CMHG -o o.modulehdr $(LPATH).cmhg.modulehdr

$(LPATH).modulehdr.h:  $(LPATH).cmhg.modulehdr
               CMHG -d $(LPATH).h.modulehdr $(LPATH).cmhg.modulehdr
               
pr00at.o:    pr00at.c par_def.h par_mem.h par_pro.h sys_def.h
               $(CC) $(CFLGS) $(LPATH).pr00at.c

pr00lib.o:   pr00lib.c par_def.h par_mem.h sys_def.h
               $(CC) $(CFLGS) $(LPATH).pr00lib.c

pr00mem.o:   pr00mem.c par_def.h sys_def.h
               $(CC) $(CFLGS) $(LPATH).pr00mem.c

parsetab.o:  parsetab.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH).parsetab.c



# Dynamic dependencies:
o.rotest:	riscos.c.rotest
o.rotest:	inc.h.sys_def
o.rotest:	inc.h.__config
o.rotest:	inc.h.risc_os
o.rotest:	inc.edf.v42
o.riscosasm: riscos.s.riscosasm
o.riscosasm: Hdr:ListOpts
o.riscosasm: Hdr:Macros
o.riscosasm: Hdr:System
o.riscosasm: Hdr:SWIs
o.riscosasm: Hdr:CPU.Generic26
o.riscosasm: Hdr:IO.GenericIO
o.riscosasm: Hdr:RISCOS
o.svcprint:	riscos.c.svcprint
o.svcprint:	C:h.kernel
o.svcprint:	C:h.swis
o.svcprint:	riscos.h.svcprint
o.devicefs:	riscos.c.devicefs
o.devicefs:	inc.h.sys_def
o.devicefs:	inc.h.__config
o.devicefs:	inc.h.risc_os
o.devicefs:	inc.h.acu_def
o.devicefs:	inc.h.acu_mem
o.devicefs:	inc.edf.dce_line
o.devicefs:	riscos.^.dp_crus.h.dsp_drv
o.devicefs:	riscos.^.DCE_crus.h.iohw_equ
o.devicefs:	inc.h.mt_pro
o.devicefs:	dp_crus.h.dosapp
o.devicefs:	C:h.kernel
o.devicefs:	C:h.swis
o.devicefs:	riscos.h.devicefs
o.devicefs:	riscos.h.svcprint
o.module:	riscos.c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	C:h.Podule
o.module:	inc.h.sys_def
o.module:	inc.h.__config
o.module:	inc.h.risc_os
o.module:	dp_crus.h.dosapp
o.module:	dp_crus.h.mt_voice
o.module:	riscos.h.devicefs
o.module:	riscos.h.modulehdr
