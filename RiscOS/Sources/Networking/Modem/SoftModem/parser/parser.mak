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
#     PARSER (AT command parser)
#
#	USING:
#     OPUS MAKE Uiltity
#
#  USEAGE: 
#       type 'make -f parser'
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
#  AT Command Parser objects
#
PARSER = pr00if.o pr00mem.o pr00lib.o pr00at.o 


#
#  Tools Flags
#
LPATH  = parser
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CFLAGS)
VPATH  = inc $(LPATH)
                    
#
#  Main Executable
#
pr:   $(PARSER)
      #@@ echo $(PARSER:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: parser.mak $(PARSER:S/obj/c) $(LPATH).*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

# 
# AT Command Parser Source
#
pr00if.o:    pr00if.c sys_def.h
               $(CC) $(CFLGS) $(LPATH).pr00if.c

pr00at.o:    pr00at.c par_def.h par_mem.h par_pro.h sys_def.h
               $(CC) $(CFLGS) $(LPATH).pr00at.c

pr00lib.o:   pr00lib.c par_def.h par_mem.h sys_def.h
               $(CC) $(CFLGS) $(LPATH).pr00lib.c

pr00mem.o:   pr00mem.c par_def.h sys_def.h
               $(CC) $(CFLGS) $(LPATH).pr00mem.c

parsetab.o:  parsetab.c sys_def.h 
               $(CC) $(CFLGS) $(LPATH).parsetab.c



# Dynamic dependencies:
o.pr00if:	parser.c.pr00if
o.pr00if:	inc.h.sys_def
o.pr00if:	inc.h.__config
o.pr00if:	inc.h.risc_os
o.pr00if:	inc.h.par_def
o.pr00if:	inc.h.par_pro
o.pr00mem:	parser.c.pr00mem
o.pr00mem:	inc.h.sys_def
o.pr00mem:	inc.h.__config
o.pr00mem:	inc.h.risc_os
o.pr00mem:	inc.h.par_def
o.pr00lib:	parser.c.pr00lib
o.pr00lib:	inc.h.sys_def
o.pr00lib:	inc.h.__config
o.pr00lib:	inc.h.risc_os
o.pr00lib:	inc.h.par_def
o.pr00lib:	parser.h.par_mem
o.pr00at:	parser.c.pr00at
o.pr00at:	inc.h.sys_def
o.pr00at:	inc.h.__config
o.pr00at:	inc.h.risc_os
o.pr00at:	inc.h.par_def
o.pr00at:	parser.h.par_mem
