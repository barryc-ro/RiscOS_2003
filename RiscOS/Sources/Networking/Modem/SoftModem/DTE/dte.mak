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
#     DOS DTE I/O
#
#	USING:
#     OPUS MAKE Uiltity
#
#  USEAGE: 
#       type 'make -f dte'
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
# DTE I/O objects
#

DTE_RISCOS = dte_riscos.o pkttrace.o


#
#  Tools Flags
#
LPATH  = dte
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)dp_crus $(CFLAGS)
VPATH  = inc $(LPATH)

#
#	Main Executable
#

dte_riscos:  $(DTE_RISCOS)
        #@@ echo $(DTE_RISCOS:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd


#
# DTE I/O source
#

dte_riscos.o:   dte_riscos.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).dte_riscos.c

pkttrace.o:     pkttrace.c sys_def.h 
		$(CC) $(CFLGS) $(LPATH).pkttrace.c


# Dynamic dependencies:
o.pkttrace:	dte.c.pkttrace
o.pkttrace:	dp_crus.h.icd_msgs
o.dte_riscos:	dte.c.dte_riscos
o.dte_riscos:	inc.h.sys_def
o.dte_riscos:	inc.h.__config
o.dte_riscos:	inc.h.risc_os
o.dte_riscos:	inc.h.acu_def
o.dte_riscos:	inc.h.acu_pro
o.dte_riscos:	inc.h.acu_mem
o.dte_riscos:	inc.edf.dce_line
o.dte_riscos:	inc.h.mt_pro
o.dte_riscos:	dp_crus.h.mt_coext
o.dte_riscos:	dp_crus.h.dsp_drv
o.dte_riscos:	inc.h.tracemac
o.dte_riscos:	dp_crus.h.dosapp
o.dte_riscos:	C:h.swis
o.dte_riscos:	C:h.kernel
o.dte_riscos:	dte.^.riscos.h.devicefs
