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
#     68302 FAX DTE I/O
#
#	USING:
#     OPUS MAKE Uiltity
#
#  USEAGE: 
#       type 'make -f pp'
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
#  PP objects
#
PACKET_PROTCOL = pp00hdx.o pp00mem.o

PP_STB = pp_stub.o

#
#  Tools Flags
#
LPATH  = pp
CFLGS  = $(CINCOPT)inc $(CFLAGS)
VPATH  = inc $(LPATH)


#
#  Main Executable
#
pp_stub:   $(PP_STB)
           #@@ echo $(PP_STB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

pp:        $(PACKET_PROTCOL)
           #@@ echo $(PACKET_PROTCOL:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: pp.mak $(PACKET_PROTCOL:S/obj/c) $(LPATH).*.h
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(PP_STB:S/obj/c) pp.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)



#
#  PP Source
#

pp00hdx.o: 	pp00hdx.c sys_def.h pp_mem.h pp_def.h
		$(CC) $(CFLGS) $(LPATH).pp00hdx.c

pp00mem.o:	pp00mem.c sys_def.h pp_def.h
		$(CC) $(CFLGS) $(LPATH).pp00mem.c

pp_stub.o:	pp_stub.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).pp_stub.c

# Dynamic dependencies:
o.pp_stub:	pp.c.pp_stub
o.pp_stub:	inc.h.sys_def
o.pp_stub:	inc.h.__config
o.pp_stub:	inc.h.risc_os
