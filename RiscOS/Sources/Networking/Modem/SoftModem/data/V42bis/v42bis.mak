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
#     V42bis Data Compression
#
#	USING:
#     OPUS MAKE Uiltity
#
#  USEAGE: 
#       type 'make -f v42bis'         to add V.42bis
#       type 'make -f v42bis stub'    to stub V.42bis 
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
# V42bis objects
#

BTLZ = btlapm.o btdecode.o btencode.o btinit.o btlz_dat.o btdict.o
BTLZSTUB = btstub.o

BTLZINC = btlz_ede.h

#
#  Tools Flags
#
LPATH  = data.v42bis
HPATH  = data.v42
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CINCOPTSEP)$(HPATH) $(CFLAGS)
VPATH  = inc $(LPATH) $(HPATH)

#
#  Main Executable
#
btlzcode:   $(BTLZ)
            #@@ echo $(BTLZ:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

btlzstub:   $(BTLZSTUB)
            #@@ echo $(BTLZSTUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: v42bis.mak $(BTLZ:S/obj/c) $(BTLZINC)
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(BTLZSTUB:S/obj/c) v42bis.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)


#
#	BTLZ Source
#

btlapm.o:	btlapm.c sys_def.h lapm_ede.h lapm_que.edf \
		btdecode.edf btencode.edf dte.edf
		$(CC) $(CFLGS) $(LPATH).btlapm.c 

btlz_dat.o:	btlz_dat.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).btlz_dat.c 

btdict.o:	btdict.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).btdict.c 

btinit.o:	btinit.c sys_def.h btlz_ede.h btencode.edf btdecode.edf \
		lapm_ede.h
		$(CC) $(CFLGS) $(LPATH).btinit.c 

btencode.o:	btencode.c sys_def.h lapm_ede.h btlz_ede.h dte.edf
		$(CC) $(CFLGS) $(LPATH).btencode.c

btdecode.o:	btdecode.c sys_def.h btlz_ede.h lapm_ede.h btinit.edf \
      dte.edf lapm_fnc.edf v42.edf
		$(CC) $(CFLGS) $(LPATH).btdecode.c

btstub.o:	btstub.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).btstub.c 




# Dynamic dependencies:
o.btlapm:	data.v42bis.c.btlapm
o.btlapm:	inc.h.sys_def
o.btlapm:	inc.h.__config
o.btlapm:	inc.h.risc_os
o.btlapm:	data.v42bis.h.btlz_ede
o.btlapm:	data.v42.h.lapm_ede
o.btlapm:	data.v42.edf.lapm_que
o.btlapm:	data.v42.edf.btdecode
o.btlapm:	data.v42.edf.btencode
o.btlapm:	inc.edf.dte
o.btdecode:	data.v42bis.c.btdecode
o.btdecode:	inc.h.sys_def
o.btdecode:	inc.h.__config
o.btdecode:	inc.h.risc_os
o.btdecode:	data.v42bis.h.btlz_ede
o.btdecode:	data.v42.edf.btinit
o.btdecode:	inc.edf.dte
o.btdecode:	data.v42.edf.lapm_fnc
o.btdecode:	inc.edf.v42
o.btencode:	data.v42bis.c.btencode
o.btencode:	inc.h.sys_def
o.btencode:	inc.h.__config
o.btencode:	inc.h.risc_os
o.btencode:	data.v42bis.h.btlz_ede
o.btencode:	inc.edf.dte
o.btinit:	data.v42bis.c.btinit
o.btinit:	inc.h.sys_def
o.btinit:	inc.h.__config
o.btinit:	inc.h.risc_os
o.btinit:	data.v42bis.h.btlz_ede
o.btinit:	data.v42.h.lapm_ede
o.btinit:	data.v42.edf.BTEncode
o.btinit:	data.v42.edf.BTDecode
o.btlz_dat:	data.v42bis.c.btlz_dat
o.btlz_dat:	inc.h.sys_def
o.btlz_dat:	inc.h.__config
o.btlz_dat:	inc.h.risc_os
o.btdict:	data.v42bis.c.btdict
o.btdict:	inc.h.sys_def
o.btdict:	inc.h.__config
o.btdict:	inc.h.risc_os
