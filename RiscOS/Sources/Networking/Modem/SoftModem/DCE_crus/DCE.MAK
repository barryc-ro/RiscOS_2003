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
#     Cutlass DCE I/O
#
#	USING:
#     OPUS MAKE Uiltity
#
#  USEAGE: 
#       type 'make -f dce'
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
# Data DCE I/O objects
#

DATAIO = linecrc.o linelink.o lineasyn.o linedetc.o linelapm.o \
         linematc.o linesync.o linedat.o linev8.o lineint.o

#
# FAX DCE I/O objects
#
FAXDCEIO = faxdat.o faxdata.o faxhdlc.o faxlink.o

#
#  Tools Flags
#
LPATH  = dce_crus
HPATH  = dp_crus
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)$(HPATH) $(CFLAGS)
VPATH  = inc $(LPATH) $(HPATH)

#
#	Main Executable
#

data_io:  $(DATAIO)
        #@@ echo $(DATAIO:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

dce_fax_io:  $(FAXDCEIO)
        #@@ echo $(FAXDCEIO:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

#
# Data DCE I/O source
#
lineasyn.o:   lineasyn.c iohw_equ.h lineedf.h lineequ.h linelink.h \
	        dsp_drv.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).lineasyn.c

linecrc.o:    linecrc.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).linecrc.c

linedat.o:    linedat.c lineequ.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).linedat.c

linedetc.o:   linedetc.c icd_msgs.h iohw_equ.h lineedf.h lineequ.h \
	        dsp_drv.h mt_codef.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).linedetc.c

lineint.o:    lineint.c faxedf.h faxequ.h iohw_equ.h lineedf.h \
	        lineequ.h dsp_drv.h mt_v8v34.h mt_coext.h sys_def.h \
	        dce_line.edf dte.edf
		$(CC) $(CFLGS) $(LPATH).lineint.c

linelapm.o:   linelapm.c iohw_equ.h lineedf.h lineequ.h linelink.h \
	        dsp_drv.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).linelapm.c

linelink.o:   linelink.c icd_msgs.h iohw_equ.h lineedf.h lineequ.h cp_dsp.h \
	        dsp_drv.h mt_macro.h mt_v8v34.h mt_codef.h mt_coext.h \
	        sys_def.h
		$(CC) $(CFLGS) $(LPATH).linelink.c

linematc.o:   linematc.c icd_msgs.h iohw_equ.h lineedf.h lineequ.h \
	        linelink.h cp_dsp.h dsp_drv.h mt_macro.h \
	        mt_codef.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).linematc.c

linesync.o:   linesync.c iohw_equ.h lineedf.h lineequ.h linelink.h \
	        dsp_drv.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).linesync.c

linev8.o:     linev8.c iohw_equ.h lineedf.h lineequ.h dsp_drv.h \
	        mt_v8v34.h mt_coext.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).linev8.c

#
# FAX DCE I/O source
#
faxdat.o:     faxdat.c faxequ.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).faxdat.c

faxdata.o:    faxdata.c faxedf.h faxequ.h iohw_equ.h lineequ.h linelink.h \
	        dsp_drv.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).faxdata.c

faxhdlc.o:    faxhdlc.c faxedf.h faxequ.h iohw_equ.h lineequ.h linelink.h \
	        dsp_drv.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).faxhdlc.c

faxlink.o:    faxlink.c icd_msgs.h faxedf.h faxequ.h iohw_equ.h lineequ.h \
	        linelink.h dsp_drv.h mt_codef.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).faxlink.c

# Dynamic dependencies:
o.faxdat:	dce_crus.c.faxdat
o.faxdat:	inc.h.sys_def
o.faxdat:	inc.h.__config
o.faxdat:	inc.h.risc_os
o.faxdat:	dce_crus.h.faxequ
o.faxdata:	dce_crus.c.faxdata
o.faxdata:	inc.h.sys_def
o.faxdata:	inc.h.__config
o.faxdata:	inc.h.risc_os
o.faxdata:	dce_crus.h.iohw_equ
o.faxdata:	dce_crus.h.lineequ
o.faxdata:	dce_crus.h.faxequ
o.faxdata:	dce_crus.h.faxedf
o.faxdata:	dce_crus.h.linelink
o.faxdata:	dp_crus.h.dsp_drv
o.faxhdlc:	dce_crus.c.faxhdlc
o.faxhdlc:	inc.h.sys_def
o.faxhdlc:	inc.h.__config
o.faxhdlc:	inc.h.risc_os
o.faxhdlc:	dce_crus.h.iohw_equ
o.faxhdlc:	dce_crus.h.lineequ
o.faxhdlc:	dce_crus.h.faxequ
o.faxhdlc:	dce_crus.h.faxedf
o.faxhdlc:	dce_crus.h.linelink
o.faxhdlc:	dp_crus.h.dsp_drv
o.faxlink:	dce_crus.c.faxlink
o.faxlink:	inc.h.sys_def
o.faxlink:	inc.h.__config
o.faxlink:	inc.h.risc_os
o.faxlink:	dce_crus.h.iohw_equ
o.faxlink:	dce_crus.h.lineequ
o.faxlink:	dce_crus.h.faxequ
o.faxlink:	dce_crus.h.faxedf
o.faxlink:	dp_crus.h.icd_msgs
o.faxlink:	dp_crus.h.dsp_drv
o.faxlink:	dp_crus.h.mt_codef
o.faxlink:	dce_crus.h.linelink
o.linecrc:	dce_crus.c.linecrc
o.linecrc:	inc.h.sys_def
o.linecrc:	inc.h.__config
o.linecrc:	inc.h.risc_os
o.linelink:	dce_crus.c.linelink
o.linelink:	inc.h.sys_def
o.linelink:	inc.h.__config
o.linelink:	inc.h.risc_os
o.linelink:	dce_crus.h.iohw_equ
o.linelink:	dce_crus.h.lineequ
o.linelink:	dce_crus.h.lineedf
o.linelink:	dp_crus.h.dsp_drv
o.linelink:	dp_crus.h.mt_macro
o.linelink:	dp_crus.h.mt_codef
o.linelink:	dp_crus.h.mt_coext
o.linelink:	dp_crus.h.cp_dsp
o.linelink:	dp_crus.h.mt_v8v34
o.linelink:	inc.h.sys_def
o.lineasyn:	dce_crus.c.lineasyn
o.lineasyn:	inc.h.sys_def
o.lineasyn:	inc.h.__config
o.lineasyn:	inc.h.risc_os
o.lineasyn:	dce_crus.h.lineequ
o.lineasyn:	dce_crus.h.lineedf
o.lineasyn:	dce_crus.h.iohw_equ
o.lineasyn:	dce_crus.h.linelink
o.lineasyn:	dp_crus.h.dsp_drv
o.linedetc:	dce_crus.c.linedetc
o.linedetc:	inc.h.sys_def
o.linedetc:	inc.h.__config
o.linedetc:	inc.h.risc_os
o.linedetc:	dce_crus.h.lineequ
o.linedetc:	dce_crus.h.lineedf
o.linedetc:	dce_crus.h.iohw_equ
o.linedetc:	dp_crus.h.dsp_drv
o.linedetc:	dp_crus.h.icd_msgs
o.linedetc:	dp_crus.h.mt_codef
o.linelapm:	dce_crus.c.linelapm
o.linelapm:	inc.h.sys_def
o.linelapm:	inc.h.__config
o.linelapm:	inc.h.risc_os
o.linelapm:	dce_crus.h.iohw_equ
o.linelapm:	dce_crus.h.lineequ
o.linelapm:	dce_crus.h.lineedf
o.linelapm:	dce_crus.h.linelink
o.linelapm:	dp_crus.h.dsp_drv
o.linematc:	dce_crus.c.linematc
o.linematc:	inc.h.sys_def
o.linematc:	inc.h.__config
o.linematc:	inc.h.risc_os
o.linematc:	dce_crus.h.lineequ
o.linematc:	dce_crus.h.lineedf
o.linematc:	dce_crus.h.iohw_equ
o.linematc:	dce_crus.h.linelink
o.linematc:	dp_crus.h.dsp_drv
o.linematc:	dp_crus.h.cp_dsp
o.linematc:	dp_crus.h.mt_codef
o.linematc:	dp_crus.h.mt_macro
o.linesync:	dce_crus.c.linesync
o.linesync:	inc.h.sys_def
o.linesync:	inc.h.__config
o.linesync:	inc.h.risc_os
o.linesync:	dce_crus.h.iohw_equ
o.linesync:	dce_crus.h.lineequ
o.linesync:	dce_crus.h.lineedf
o.linesync:	dce_crus.h.linelink
o.linesync:	dp_crus.h.dsp_drv
o.linedat:	dce_crus.c.linedat
o.linedat:	inc.h.sys_def
o.linedat:	inc.h.__config
o.linedat:	inc.h.risc_os
o.linedat:	dce_crus.h.lineequ
o.linev8:	dce_crus.c.linev8
o.linev8:	inc.h.sys_def
o.linev8:	inc.h.__config
o.linev8:	inc.h.risc_os
o.linev8:	dce_crus.h.lineequ
o.linev8:	dce_crus.h.lineedf
o.linev8:	dce_crus.h.iohw_equ
o.linev8:	dp_crus.h.dsp_drv
o.linev8:	dp_crus.h.mt_coext
o.linev8:	dp_crus.h.mt_v8v34
o.linev8:	inc.h.sys_def
o.lineint:	dce_crus.c.lineint
o.lineint:	inc.h.sys_def
o.lineint:	inc.h.__config
o.lineint:	inc.h.risc_os
o.lineint:	dce_crus.h.lineequ
o.lineint:	dce_crus.h.lineedf
o.lineint:	dce_crus.h.faxequ
o.lineint:	dce_crus.h.faxedf
o.lineint:	dce_crus.h.iohw_equ
o.lineint:	dp_crus.h.dsp_drv
o.lineint:	inc.edf.dce_line
o.lineint:	inc.edf.dte
o.lineint:	dp_crus.h.mt_v8v34
o.lineint:	inc.h.sys_def
o.lineint:	dp_crus.h.mt_coext
