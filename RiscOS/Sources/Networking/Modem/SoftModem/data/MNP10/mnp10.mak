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
#     MNP10 - (MNP robust data error correction)
#
#	USING:
#     OPUS MAKE Uiltity
#
#  USEAGE: 
#     (requires a custom data pump driver - dp10*.*)
#     type 'make -f mnp10'
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
# MNP10 objects
#
M10 =	m10_fnc.o m10_fffb.o m10_lm.o m10_lk.o m10_pref.o\
	   m10_lib.o m10_dat.o m10_btlz.o m10_if.o mnp_if.o

M10STUB = m10_stub.o

M10INC = m10_ede.h $(LPATH).*.edf

#
#  Tools Flags
#
LPATH  = data.mnp10
HPATH  = data.v42
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(HPATH) $(CFLAGS)
VPATH  = inc $(LPATH) $(HPATH)

#
# Main Executable
#
m10code:    $(M10)
            #@@ echo $(M10:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

m10stub:    $(M10STUB)
            #@@ echo $(M10STUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

release: mnp10.mak $(M10:S/obj/c) $(M10INC)
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(M10STUB:S/obj/c) mnp10.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#	MNP10 source
#
m10_stub.o:	m10_stub.c sys_def.h v42.edf mnp_que.edf m10_def.h mnp_def.h
		$(CC) $(CFLGS) $(LPATH).m10_stub.c 

m10_btlz.o:	m10_btlz.c  sys_def.h btdecode.edf btencode.edf \
			    dte.edf mnp_if.edf mnp_def.h mnp_stru.h
		$(CC) $(CFLGS) $(LPATH).m10_btlz.c 

m10_if.o:	m10_if.c sys_def.h m10_fnc.edf m10_btlz.edf m10_lk.edf \
		m10_pref.edf
		$(CC) $(CFLGS) $(LPATH).m10_if.c 

mnp_if.o:	mnp_if.c sys_def.h mnp_lib.edf mnp_la.edf mnp_lr.edf \
		mnp_lrr.edf mnp_lt.edf mnp_que.edf mnp_comm.edf mnp.edf \
		mnp_af.edf
		$(CC) $(CFLGS) $(LPATH).mnp_if.c 

m10_dat.o:	m10_dat.c sys_def.h mnp_stru.h
		$(CC) $(CFLGS) $(LPATH).m10_dat.c 

m10_fnc.o:	m10_fnc.c sys_def.h m10_def.h m10_lm.edf m10_fffb.edf \
		dp10.edf mnp_def.h v42.edf mnp_if.edf m10_lk.edf m10_lib.edf \
		btinit.edf
		$(CC) $(CFLGS) $(LPATH).m10_fnc.c 

m10_fffb.o:	m10_fffb.c sys_def.h m10_def.h dp10.edf v42.edf mnp_if.edf \
		m10_lib.edf m10_lm.edf
		$(CC) $(CFLGS) $(LPATH).m10_fffb.c 

m10_lm.o:	m10_lm.c sys_def.h m10_def.h mnp_def.h mnp_stru.h mnp_if.edf \
		m10_fnc.edf
		$(CC) $(CFLGS) $(LPATH).m10_lm.c 

m10_lk.o:	m10_lk.c sys_def.h mnp_def.h mnp_stru.h mnp_if.edf
		$(CC) $(CFLGS) $(LPATH).m10_lk.c 

m10_lib.o:	m10_lib.c sys_def.h dp10.edf m10_def.h m10_lm.edf m10_fffb.edf \
		mnp_if.edf
		$(CC) $(CFLGS) $(LPATH).m10_lib.c 

m10_pref.o:	m10_pref.c sys_def.h mnp_def.h m10_def.h mnp_stru.h mnp_ede.h \
		m10_fnc.edf mnp_if.edf dce_line.edf dte.edf
		$(CC) $(CFLGS) $(LPATH).m10_pref.c 


# Dynamic dependencies:
o.m10_stub:	data.mnp10.c.m10_stub
o.m10_stub:	inc.h.sys_def
o.m10_stub:	inc.h.__config
o.m10_stub:	inc.h.risc_os
o.m10_stub:	inc.edf.v42
o.m10_stub:	data.v42.edf.mnp_que
o.m10_stub:	data.v42.h.m10_def
o.m10_stub:	data.v42.h.mnp_def
o.m10_stub:	data.v42.edf.btinit
