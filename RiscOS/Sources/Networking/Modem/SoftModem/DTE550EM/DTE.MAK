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
!include compiler.cfg

#
# DTE I/O objects
#

DTEIO = terminal.obj serialio.obj pkttrace.obj dte.obj dtedat.obj \
        acu.obj sleep.obj

#
# AS I/O Objects
#
AS_IO = 
AS_IO_STUB = as_stub.obj


#
#  Tools Flags
#
LPATH  = dte550em
HPATH1 = dp_crus
HPATH2 = acu
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)$(HPATH1) $(CINCOPTSEP)$(HPATH2) $(CFLAGS)
VPATH  = inc;$(LPATH);$(HPATH1);$(HPATH2)

#
#	Main Executable
#

dte_io:  $(DTEIO)
        @@ echo $(DTEIO:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

as_stub:  $(AS_IO_STUB)
        @@ echo $(AS_IO_STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

#
# DTE I/O source
#

as_stub.obj:    as_stub.c sys_def.h
		$(CC) $(CFLGS) $(LPATH)\as_stub.c

acu.obj:        acu.c acuiomem.h dteequ.h sys_def.h
		$(CC) $(CFLGS) $(LPATH)\acu.c

dte.obj:        dte.c acuiomem.h dte.edf dteequ.h sys_def.h
		$(CC) $(CFLGS) $(LPATH)\dte.c

dtedat.obj:     dtedat.c dteequ.h sys_def.h
		$(CC) $(CFLGS) $(LPATH)\dtedat.c

sleep.obj:      sleep.c sys_def.h
		$(CC) $(CFLGS) $(LPATH)\sleep.c

terminal.obj:   terminal.c sys_def.h
		$(CC) $(CFLGS) $(LPATH)\terminal.c

serialio.obj:   serialio.c sys_def.h
		$(CC) $(CFLGS) $(LPATH)\serialio.c

pkttrace.obj:   pkttrace.c sys_def.h 
		$(CC) $(CFLGS) $(LPATH)\pkttrace.c


