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
#     V42 (V.42,LAPM,MNP error correction)
#
#	USING:
#     OPUS MAKE Uiltity
#
#  USEAGE: 
#       type 'make -f v42'
#
#

#
# Read global compiler directive file
#!include compiler.cfg

#
# V42 objects
#

V42 = v42.o v42_dat.o lapm_act.o lapm_bg.o lapm_brk.o lapm_ctl.o \
      lapm_fnc.o lapm_int.o lapm_lib.o lapm_que.o lapm_rej.o \
      lapm_rx.o lapm_sta.o lapm_tab.o lapm_tmr.o lapm_tst.o lapm_fg.o \
      lapm_tx.o lapm_xid.o lapm_dat.o mnp.o mnp_la.o \
      mnp_pack.o mnp_lib.o mnp_ln.o mnp_lna.o mnp_lr.o mnp_lrr.o \
      mnp_que.o mnp_comm.o mnp_bg.o mnp_lt.o mnp_dat.o mnp_ld.o 

ETC = v42_etc.o lapm_etc.o
ETCSTUB = v42_estb.o

OTHER = acu_stub.o

#
#  Tools Flags
#
LPATH  = data.v42
CFLGS  = $(CINCOPT)inc $(CINCOPTSEP)$(LPATH) $(CFLAGS)
VPATH  = inc $(LPATH)

#
#  Main Executable
#
v42code:    $(V42) 
            #@@ echo $(V42:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

etcstub:    $(ETCSTUB)
            #@@ echo $(ETCSTUB:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

etc:        $(ETC)
            #@@ echo $(ETC:S/.*/$(LOADOBJCMD) &$(MORE)$(RETURN)/) >> link.cmd

sys:        $(OTHER)

release: relinc reletc v42.mak $(V42:S/obj/c)
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

reletc: v42.mak $(ETC:S/obj/c)
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relinc: $(LPATH).*.h $(LPATH).*.edf 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

relstub: $(ETCSTUB:S/obj/c) v42.mak 
      #@@ echo $(**,S/.*/&$(MORE)$(RETURN)/) >> $(ZIPLIST)

#
#	V.42/MNP Source
#
v42.o:	v42.c  sys_def.h mnp.edf mnp_bg.edf dce_line.edf m10_if.edf \
		m10_def.h lapm_ede.h lapm_que.edf lapm_tx.edf
		$(CC) $(CFLGS) $(LPATH).v42.c 

mnp.o:	mnp.c sys_def.h v42.edf mnp_que.edf mnp_lib.edf mnp_af.edf \
		dce_line.edf dte.edf mnp_def.h mnp_stru.h btinit.edf \
		m10_def.h mnp_ede.h m10_if.edf mnp_comm.edf
		$(CC) $(CFLGS) $(LPATH).mnp.c 

mnp_bg.o:	mnp_bg.c sys_def.h v42.edf mnp.edf mnp_lib.edf mnp_af.edf \
		mnp_la.edf mnp_ld.edf mnp_ln.edf mnp_lna.edf mnp_lr.edf \
		mnp_que.edf mnp_lrr.edf mnp_lt.edf dce_line.edf dte.edf \
		mnp_def.h mnp_stru.h btinit.edf m10_def.h
		$(CC) $(CFLGS) $(LPATH).mnp_bg.c 

mnp_la.o:	mnp_la.c sys_def.h mnp_lib.edf mnp_def.h mnp_stru.h \
		m10_def.h mnp_que.edf mnp_ede.h
		$(CC) $(CFLGS) $(LPATH).mnp_la.c 

mnp_ld.o:	mnp_ld.c sys_def.h mnp_lib.edf mnp_def.h mnp_stru.h mnp_ede.h
		$(CC) $(CFLGS) $(LPATH).mnp_ld.c 

mnp_lib.o:	mnp_lib.c  sys_def.h v42.edf mnp_bg.edf mnp_af.edf \
		mnp_la.edf mnp_ld.edf mnp_ln.edf mnp_lna.edf mnp_lr.edf \
		mnp_que.edf mnp_lrr.edf mnp_lt.edf dce_line.edf dte.edf \
		mnp_def.h mnp_stru.h mnp_comm.edf m10_def.h m10_if.edf \
		mnp_ede.h
		$(CC) $(CFLGS) $(LPATH).mnp_lib.c 

mnp_ln.o:	mnp_ln.c  sys_def.h mnp_lib.edf mnp_def.h mnp_stru.h mnp_ede.h
		$(CC) $(CFLGS) $(LPATH).mnp_ln.c 

mnp_pack.o:	mnp_pack.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).mnp_pack.c 

mnp_lna.o:	mnp_lna.c  sys_def.h mnp_lib.edf mnp_def.h mnp_stru.h mnp_ede.h
		$(CC) $(CFLGS) $(LPATH).mnp_lna.c 

mnp_lr.o:	mnp_lr.c  sys_def.h mnp_lib.edf mnp_def.h mnp_stru.h \
		m10_def.h mnp_ede.h m10_if.edf mnp_comm.edf
		$(CC) $(CFLGS) $(LPATH).mnp_lr.c 

mnp_lrr.o:	mnp_lrr.c  sys_def.h mnp_lib.edf mnp_def.h mnp_stru.h mnp_ede.h
		$(CC) $(CFLGS) $(LPATH).mnp_lrr.c 

mnp_lt.o:	mnp_lt.c  sys_def.h mnp_lib.edf mnp_af.edf dte.edf mnp_def.h \
		mnp_stru.h m10_if.edf mnp_ede.h
		$(CC) $(CFLGS) $(LPATH).mnp_lt.c 

mnp_af.o:	mnp_af.c sys_def.h mnp_lib.edf dte.edf mnp_def.h mnp_stru.h \
                mnp_ede.h cl5_ede.h 
		$(CC) $(CFLGS) $(LPATH).mnp_af.c

mnp_no5.o:	mnp_no5.c sys_def.h mnp_lib.edf dte.edf mnp_def.h mnp_stru.h \
		mnp_ede.h m10_if.edf
		$(CC) $(CFLGS) $(LPATH).mnp_no5.c 

mnp_que.o:	mnp_que.c  sys_def.h v42.edf mnp_lib.edf mnp_bg.edf mnp_af.edf \
		mnp_la.edf mnp_ld.edf mnp_ln.edf mnp_lna.edf mnp_lr.edf \
		mnp_lrr.edf mnp_lt.edf dce_line.edf dte.edf m10_if.edf \
		mnp_def.h mnp_stru.h m10_def.h mnp_ede.h
		$(CC) $(CFLGS) $(LPATH).mnp_que.c 

mnp_comm.o:	mnp_comm.c sys_def.h mnp_def.h mnp_stru.h mnp_ede.h
		$(CC) $(CFLGS) $(LPATH).mnp_comm.c 

mnp_dat.o:	mnp_dat.c sys_def.h mnp_def.h mnp_stru.h
		$(CC) $(CFLGS) $(LPATH).mnp_dat.c 

v42_dat.o:	v42_dat.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).v42_dat.c 

cl5_dat.o:	cl5_dat.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).cl5_dat.c 

lapm_act.o:	lapm_act.c lapm_ede.h lapm_que.edf lapm_sta.edf lapm_ctl.edf \
		lapm_lib.edf lapm_rej.edf lapm_rx.edf lapm_tx.edf lapm_tmr.edf \
		lapm_xid.edf lapm_brk.edf lapm_fnc.edf dte.edf dce_line.edf v42.edf \
		sys_def.h
		$(CC) $(CFLGS) $(LPATH).lapm_act.c 

lapm_bg.o:	lapm_bg.c  sys_def.h lapm_ede.h lapm_que.edf lapm_sta.edf \
		lapm_ctl.edf lapm_rx.edf lapm_tx.edf lapm_tmr.edf lapm_lib.edf \
		lapm_fg.edf dce_line.edf v42.edf dte.edf btlapm.edf
		$(CC) $(CFLGS) $(LPATH).lapm_bg.c 

lapm_fg.o:	lapm_fg.c sys_def.h lapm_ede.h lapm_que.edf lapm_tmr.edf \
		lapm_brk.edf lapm_lib.edf lapm_tx.edf dce_line.edf v42.edf \
		dte.edf lapm_ctl.edf lapm_rej.edf
		$(CC) $(CFLGS) $(LPATH).lapm_fg.c 

lapm_brk.o:	lapm_brk.c  sys_def.h lapm_ede.h lapm_que.edf lapm_ctl.edf \
		lapm_lib.edf dte.edf
		$(CC) $(CFLGS) $(LPATH).lapm_brk.c 

lapm_ctl.o:	lapm_ctl.c  sys_def.h lapm_ede.h lapm_que.edf lapm_lib.edf \
		lapm_tmr.edf lapm_xid.edf
		$(CC) $(CFLGS)  $(LPATH).lapm_ctl.c 

lapm_fnc.o:	lapm_fnc.c  sys_def.h lapm_ede.h lapm_que.edf \
		lapm_sta.edf lapm_ctl.edf lapm_brk.edf lapm_tst.edf \
		lapm_lib.edf lapm_tmr.edf lapm_tx.edf dte.edf dce_line.edf \
		v42.edf btencode.edf btinit.edf
		$(CC) $(CFLGS) $(LPATH).lapm_fnc.c 

lapm_int.o:	lapm_int.c sys_def.h lapm_ede.h lapm_que.edf
		$(CC) $(CFLGS) $(LPATH).lapm_int.c

lapm_lib.o:	lapm_lib.c sys_def.h lapm_ede.h lapm_que.edf lapm_ctl.edf \
		lapm_tmr.edf dce_line.edf dte.edf btinit.edf lapm_fg.edf
		$(CC) $(CFLGS) $(LPATH).lapm_lib.c 

lapm_que.o:	lapm_que.c sys_def.h lapm_ede.h dce_line.edf
		$(CC) $(CFLGS) $(LPATH).lapm_que.c 

lapm_rej.o:	lapm_rej.c sys_def.h lapm_ede.h lapm_que.edf lapm_ctl.edf \
		dce_line.edf
		$(CC) $(CFLGS) $(LPATH).lapm_rej.c 

lapm_rx.o:	lapm_rx.c  sys_def.h lapm_ede.h lapm_que.edf \
		dce_line.edf lapm_brk.edf dte.edf lapm_lib.edf
		$(CC) $(CFLGS) $(LPATH).lapm_rx.c 

lapm_sta.o:	lapm_sta.c sys_def.h lapm_ede.h lapm_act.edf
		$(CC) $(CFLGS) $(LPATH).lapm_sta.c 

lapm_tab.o:	lapm_tab.c sys_def.h lapm_ede.h lapm_act.edf
		$(CC) $(CFLGS) $(LPATH).lapm_tab.c 

lapm_tmr.o:	lapm_tmr.c sys_def.h lapm_ede.h
		$(CC) $(CFLGS) $(LPATH).lapm_tmr.c 

lapm_tst.o:	lapm_tst.c sys_def.h lapm_ede.h lapm_que.edf lapm_ctl.edf
		$(CC) $(CFLGS) $(LPATH).lapm_tst.c 

lapm_tx.o:	lapm_tx.c sys_def.h lapm_ede.h lapm_que.edf \
		lapm_ctl.edf lapm_tmr.edf lapm_lib.edf dce_line.edf dte.edf \
		btencode.edf
		$(CC) $(CFLGS) $(LPATH).lapm_tx.c 

lapm_xid.o:	lapm_xid.c sys_def.h lapm_ede.h lapm_que.edf v42.edf btinit.edf
		$(CC) $(CFLGS) $(LPATH).lapm_xid.c 

btlapm.o:	btlapm.c sys_def.h lapm_ede.h lapm_que.edf \
		btdecode.edf btencode.edf dte.edf
		$(CC) $(CFLGS) $(LPATH).btlapm.c 

lapm_dat.o:	lapm_dat.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).lapm_dat.c 


#
#  V42 ETC source
#
v42_etc.o:	v42_etc.c v42_etc.h sys_def.h
		$(CC) $(CFLGS) $(LPATH).v42_etc.c

lapm_etc.o:	lapm_etc.c lapm_etc.h sys_def.h lapm_ede.h
		$(CC) $(CFLGS) $(LPATH).lapm_etc.c

v42_estb.o:	v42_estb.c sys_def.h
		$(CC) $(CFLGS) $(LPATH).v42_estb.c





# Dynamic dependencies:
o.v42_estb:	data.v42.c.v42_estb
o.v42_estb:	inc.h.sys_def
o.v42_estb:	inc.h.__config
o.v42_estb:	inc.h.risc_os
o.v42:	data.v42.c.v42
o.v42:	inc.h.sys_def
o.v42:	inc.h.__config
o.v42:	inc.h.risc_os
o.v42:	data.v42.edf.mnp_bg
o.v42:	data.v42.edf.mnp
o.v42:	inc.edf.dte
o.v42:	inc.edf.dce_line
o.v42:	data.v42.edf.m10_if
o.v42:	data.v42.h.m10_def
o.v42:	data.v42.h.lapm_ede
o.v42:	data.v42.edf.lapm_que
o.v42:	data.v42.edf.lapm_tx
o.v42:	data.v42.edf.v42_etc
o.v42_dat:	data.v42.c.v42_dat
o.v42_dat:	inc.h.sys_def
o.v42_dat:	inc.h.__config
o.v42_dat:	inc.h.risc_os
o.lapm_act:	data.v42.c.lapm_act
o.lapm_act:	inc.h.sys_def
o.lapm_act:	inc.h.__config
o.lapm_act:	inc.h.risc_os
o.lapm_act:	data.v42.h.lapm_ede
o.lapm_act:	data.v42.edf.lapm_que
o.lapm_act:	data.v42.edf.lapm_sta
o.lapm_act:	data.v42.edf.lapm_ctl
o.lapm_act:	data.v42.edf.lapm_lib
o.lapm_act:	data.v42.edf.lapm_rej
o.lapm_act:	data.v42.edf.lapm_rx
o.lapm_act:	data.v42.edf.lapm_tx
o.lapm_act:	data.v42.edf.lapm_tmr
o.lapm_act:	data.v42.edf.lapm_xid
o.lapm_act:	data.v42.edf.lapm_brk
o.lapm_act:	data.v42.edf.lapm_fnc
o.lapm_act:	inc.edf.dte
o.lapm_act:	inc.edf.dce_line
o.lapm_act:	data.v42.edf.v42
o.lapm_bg:	data.v42.c.lapm_bg
o.lapm_bg:	inc.h.sys_def
o.lapm_bg:	inc.h.__config
o.lapm_bg:	inc.h.risc_os
o.lapm_bg:	data.v42.h.lapm_ede
o.lapm_bg:	data.v42.edf.lapm_que
o.lapm_bg:	data.v42.edf.lapm_sta
o.lapm_bg:	data.v42.edf.lapm_ctl
o.lapm_bg:	data.v42.edf.lapm_rx
o.lapm_bg:	data.v42.edf.lapm_tx
o.lapm_bg:	data.v42.edf.lapm_tmr
o.lapm_bg:	data.v42.edf.lapm_lib
o.lapm_bg:	data.v42.edf.lapm_fg
o.lapm_bg:	inc.edf.dce_line
o.lapm_bg:	data.v42.edf.v42
o.lapm_bg:	inc.edf.dte
o.lapm_bg:	data.v42.edf.btlapm
o.lapm_brk:	data.v42.c.lapm_brk
o.lapm_brk:	inc.h.sys_def
o.lapm_brk:	inc.h.__config
o.lapm_brk:	inc.h.risc_os
o.lapm_brk:	data.v42.h.lapm_ede
o.lapm_brk:	data.v42.edf.lapm_que
o.lapm_brk:	data.v42.edf.lapm_ctl
o.lapm_brk:	data.v42.edf.lapm_lib
o.lapm_brk:	data.v42.edf.lapm_tmr
o.lapm_brk:	inc.edf.dte
o.lapm_ctl:	data.v42.c.lapm_ctl
o.lapm_ctl:	inc.h.sys_def
o.lapm_ctl:	inc.h.__config
o.lapm_ctl:	inc.h.risc_os
o.lapm_ctl:	data.v42.h.lapm_ede
o.lapm_ctl:	data.v42.edf.lapm_que
o.lapm_ctl:	data.v42.edf.lapm_lib
o.lapm_ctl:	data.v42.edf.lapm_tmr
o.lapm_ctl:	data.v42.edf.lapm_xid
o.lapm_fnc:	data.v42.c.lapm_fnc
o.lapm_fnc:	inc.h.sys_def
o.lapm_fnc:	inc.h.__config
o.lapm_fnc:	inc.h.risc_os
o.lapm_fnc:	data.v42.h.lapm_ede
o.lapm_fnc:	data.v42.edf.lapm_que
o.lapm_fnc:	data.v42.edf.lapm_sta
o.lapm_fnc:	data.v42.edf.lapm_ctl
o.lapm_fnc:	data.v42.edf.lapm_brk
o.lapm_fnc:	data.v42.edf.lapm_tst
o.lapm_fnc:	data.v42.edf.lapm_lib
o.lapm_fnc:	data.v42.edf.lapm_tmr
o.lapm_fnc:	data.v42.edf.lapm_tx
o.lapm_fnc:	inc.edf.dte
o.lapm_fnc:	inc.edf.dce_line
o.lapm_fnc:	data.v42.edf.v42
o.lapm_fnc:	data.v42.edf.btencode
o.lapm_fnc:	data.v42.edf.btinit
o.lapm_fnc:	data.v42.edf.lapm_etc
o.lapm_int:	data.v42.c.lapm_int
o.lapm_int:	inc.h.sys_def
o.lapm_int:	inc.h.__config
o.lapm_int:	inc.h.risc_os
o.lapm_int:	data.v42.h.lapm_ede
o.lapm_int:	data.v42.edf.lapm_que
o.lapm_lib:	data.v42.c.lapm_lib
o.lapm_lib:	inc.h.sys_def
o.lapm_lib:	inc.h.__config
o.lapm_lib:	inc.h.risc_os
o.lapm_lib:	data.v42.h.lapm_ede
o.lapm_lib:	data.v42.edf.lapm_que
o.lapm_lib:	data.v42.edf.lapm_fg
o.lapm_lib:	data.v42.edf.lapm_ctl
o.lapm_lib:	data.v42.edf.lapm_tmr
o.lapm_lib:	inc.edf.dce_line
o.lapm_lib:	inc.edf.dte
o.lapm_lib:	data.v42.edf.btinit
o.lapm_lib:	data.v42.edf.v42
o.lapm_que:	data.v42.c.lapm_que
o.lapm_que:	inc.h.sys_def
o.lapm_que:	inc.h.__config
o.lapm_que:	inc.h.risc_os
o.lapm_que:	data.v42.h.lapm_ede
o.lapm_que:	inc.edf.dce_line
o.lapm_rej:	data.v42.c.lapm_rej
o.lapm_rej:	inc.h.sys_def
o.lapm_rej:	inc.h.__config
o.lapm_rej:	inc.h.risc_os
o.lapm_rej:	data.v42.h.lapm_ede
o.lapm_rej:	data.v42.edf.lapm_que
o.lapm_rej:	data.v42.edf.lapm_ctl
o.lapm_rej:	inc.edf.dce_line
o.lapm_rx:	data.v42.c.lapm_rx
o.lapm_rx:	inc.h.sys_def
o.lapm_rx:	inc.h.__config
o.lapm_rx:	inc.h.risc_os
o.lapm_rx:	data.v42.h.lapm_ede
o.lapm_rx:	data.v42.edf.lapm_que
o.lapm_rx:	data.v42.edf.lapm_lib
o.lapm_rx:	inc.edf.dce_line
o.lapm_rx:	data.v42.edf.lapm_brk
o.lapm_rx:	inc.edf.dte
o.lapm_rx:	data.v42.edf.btdecode
o.lapm_sta:	data.v42.c.lapm_sta
o.lapm_sta:	inc.h.sys_def
o.lapm_sta:	inc.h.__config
o.lapm_sta:	inc.h.risc_os
o.lapm_sta:	data.v42.h.lapm_ede
o.lapm_sta:	data.v42.edf.lapm_act
o.lapm_tab:	data.v42.c.lapm_tab
o.lapm_tab:	inc.h.sys_def
o.lapm_tab:	inc.h.__config
o.lapm_tab:	inc.h.risc_os
o.lapm_tab:	data.v42.h.lapm_ede
o.lapm_tab:	data.v42.edf.lapm_act
o.lapm_tmr:	data.v42.c.lapm_tmr
o.lapm_tmr:	inc.h.sys_def
o.lapm_tmr:	inc.h.__config
o.lapm_tmr:	inc.h.risc_os
o.lapm_tmr:	data.v42.h.lapm_ede
o.lapm_tmr:	data.v42.edf.lapm_etc
o.lapm_tst:	data.v42.c.lapm_tst
o.lapm_tst:	inc.h.sys_def
o.lapm_tst:	inc.h.__config
o.lapm_tst:	inc.h.risc_os
o.lapm_tst:	data.v42.h.lapm_ede
o.lapm_tst:	data.v42.edf.lapm_que
o.lapm_tst:	data.v42.edf.lapm_ctl
o.lapm_fg:	data.v42.c.lapm_fg
o.lapm_fg:	inc.h.sys_def
o.lapm_fg:	inc.h.__config
o.lapm_fg:	inc.h.risc_os
o.lapm_fg:	data.v42.h.lapm_ede
o.lapm_fg:	data.v42.edf.lapm_ctl
o.lapm_fg:	data.v42.edf.lapm_que
o.lapm_fg:	data.v42.edf.lapm_tmr
o.lapm_fg:	data.v42.edf.lapm_brk
o.lapm_fg:	data.v42.edf.lapm_lib
o.lapm_fg:	data.v42.edf.lapm_rej
o.lapm_fg:	data.v42.edf.lapm_tx
o.lapm_fg:	inc.edf.dce_line
o.lapm_fg:	data.v42.edf.v42
o.lapm_fg:	inc.edf.dte
o.lapm_tx:	data.v42.c.lapm_tx
o.lapm_tx:	inc.h.sys_def
o.lapm_tx:	inc.h.__config
o.lapm_tx:	inc.h.risc_os
o.lapm_tx:	data.v42.h.lapm_ede
o.lapm_tx:	data.v42.edf.lapm_que
o.lapm_tx:	data.v42.edf.lapm_ctl
o.lapm_tx:	data.v42.edf.lapm_tmr
o.lapm_tx:	data.v42.edf.lapm_lib
o.lapm_tx:	inc.edf.dce_line
o.lapm_tx:	inc.edf.dte
o.lapm_tx:	data.v42.edf.btencode
o.lapm_xid:	data.v42.c.lapm_xid
o.lapm_xid:	inc.h.sys_def
o.lapm_xid:	inc.h.__config
o.lapm_xid:	inc.h.risc_os
o.lapm_xid:	data.v42.h.lapm_ede
o.lapm_xid:	data.v42.edf.lapm_que
o.lapm_xid:	data.v42.edf.v42
o.lapm_xid:	data.v42.edf.btinit
o.lapm_dat:	data.v42.c.lapm_dat
o.lapm_dat:	inc.h.sys_def
o.lapm_dat:	inc.h.__config
o.lapm_dat:	inc.h.risc_os
o.mnp:	data.v42.c.mnp
o.mnp:	inc.h.sys_def
o.mnp:	inc.h.__config
o.mnp:	inc.h.risc_os
o.mnp:	data.v42.EDF.MNP_QUE
o.mnp:	data.v42.EDF.MNP_LIB
o.mnp:	data.v42.EDF.MNP_AF
o.mnp:	inc.EDF.DCE_LINE
o.mnp:	inc.EDF.DTE
o.mnp:	data.v42.EDF.BTINIT
o.mnp:	data.v42.EDF.V42
o.mnp:	data.v42.H.MNP_DEF
o.mnp:	data.v42.H.MNP_STRU
o.mnp:	data.v42.H.MNP_EDE
o.mnp:	data.v42.EDF.M10_IF
o.mnp:	data.v42.EDF.MNP_COMM
o.mnp_la:	data.v42.c.mnp_la
o.mnp_la:	inc.h.sys_def
o.mnp_la:	inc.h.__config
o.mnp_la:	inc.h.risc_os
o.mnp_la:	data.v42.H.M10_DEF
o.mnp_la:	data.v42.EDF.MNP_LIB
o.mnp_la:	data.v42.EDF.MNP_QUE
o.mnp_la:	data.v42.H.MNP_DEF
o.mnp_la:	data.v42.H.MNP_STRU
o.mnp_la:	data.v42.H.MNP_EDE
o.mnp_pack:	data.v42.c.mnp_pack
o.mnp_pack:	inc.h.sys_def
o.mnp_pack:	inc.h.__config
o.mnp_pack:	inc.h.risc_os
o.mnp_lib:	data.v42.c.mnp_lib
o.mnp_lib:	inc.h.sys_def
o.mnp_lib:	inc.h.__config
o.mnp_lib:	inc.h.risc_os
o.mnp_lib:	data.v42.EDF.V42
o.mnp_lib:	data.v42.EDF.MNP_BG
o.mnp_lib:	data.v42.EDF.MNP_AF
o.mnp_lib:	data.v42.EDF.MNP_LA
o.mnp_lib:	data.v42.EDF.MNP_LD
o.mnp_lib:	data.v42.EDF.MNP_LN
o.mnp_lib:	data.v42.EDF.MNP_LNA
o.mnp_lib:	data.v42.EDF.MNP_LR
o.mnp_lib:	data.v42.EDF.MNP_QUE
o.mnp_lib:	data.v42.EDF.MNP_LRR
o.mnp_lib:	data.v42.EDF.MNP_LT
o.mnp_lib:	inc.EDF.DCE_LINE
o.mnp_lib:	inc.EDF.DTE
o.mnp_lib:	data.v42.H.MNP_DEF
o.mnp_lib:	data.v42.H.MNP_STRU
o.mnp_lib:	data.v42.EDF.MNP_COMM
o.mnp_lib:	data.v42.EDF.M10_IF
o.mnp_lib:	data.v42.H.MNP_EDE
o.mnp_ln:	data.v42.c.mnp_ln
o.mnp_ln:	inc.h.sys_def
o.mnp_ln:	inc.h.__config
o.mnp_ln:	inc.h.risc_os
o.mnp_ln:	data.v42.EDF.MNP_LIB
o.mnp_ln:	data.v42.H.MNP_DEF
o.mnp_ln:	data.v42.H.MNP_STRU
o.mnp_ln:	data.v42.H.MNP_EDE
o.mnp_lna:	data.v42.c.mnp_lna
o.mnp_lna:	inc.h.sys_def
o.mnp_lna:	inc.h.__config
o.mnp_lna:	inc.h.risc_os
o.mnp_lna:	data.v42.EDF.MNP_LIB
o.mnp_lna:	data.v42.H.MNP_DEF
o.mnp_lna:	data.v42.H.MNP_STRU
o.mnp_lna:	data.v42.H.MNP_EDE
o.mnp_lr:	data.v42.c.mnp_lr
o.mnp_lr:	inc.h.sys_def
o.mnp_lr:	inc.h.__config
o.mnp_lr:	inc.h.risc_os
o.mnp_lr:	data.v42.EDF.MNP_LIB
o.mnp_lr:	data.v42.H.MNP_DEF
o.mnp_lr:	data.v42.H.MNP_STRU
o.mnp_lr:	data.v42.H.MNP_EDE
o.mnp_lr:	data.v42.EDF.M10_IF
o.mnp_lr:	data.v42.EDF.MNP_COMM
o.mnp_lrr:	data.v42.c.mnp_lrr
o.mnp_lrr:	inc.h.sys_def
o.mnp_lrr:	inc.h.__config
o.mnp_lrr:	inc.h.risc_os
o.mnp_lrr:	data.v42.EDF.MNP_LIB
o.mnp_lrr:	data.v42.H.MNP_DEF
o.mnp_lrr:	data.v42.H.MNP_STRU
o.mnp_lrr:	data.v42.H.MNP_EDE
o.mnp_que:	data.v42.c.mnp_que
o.mnp_que:	inc.h.sys_def
o.mnp_que:	inc.h.__config
o.mnp_que:	inc.h.risc_os
o.mnp_que:	data.v42.EDF.V42
o.mnp_que:	data.v42.EDF.MNP_LIB
o.mnp_que:	data.v42.EDF.MNP_BG
o.mnp_que:	data.v42.EDF.MNP_AF
o.mnp_que:	data.v42.EDF.MNP_LA
o.mnp_que:	data.v42.EDF.MNP_LD
o.mnp_que:	data.v42.EDF.MNP_LN
o.mnp_que:	data.v42.EDF.MNP_LNA
o.mnp_que:	data.v42.EDF.MNP_LR
o.mnp_que:	data.v42.EDF.MNP_LRR
o.mnp_que:	data.v42.EDF.MNP_LT
o.mnp_que:	inc.EDF.DCE_LINE
o.mnp_que:	inc.EDF.DTE
o.mnp_que:	data.v42.EDF.M10_IF
o.mnp_que:	data.v42.H.MNP_DEF
o.mnp_que:	data.v42.H.MNP_STRU
o.mnp_que:	data.v42.H.MNP_EDE
o.mnp_que:	data.v42.EDF.BTINIT
o.mnp_comm:	data.v42.c.mnp_comm
o.mnp_comm:	inc.h.sys_def
o.mnp_comm:	inc.h.__config
o.mnp_comm:	inc.h.risc_os
o.mnp_comm:	data.v42.H.MNP_DEF
o.mnp_comm:	data.v42.H.MNP_STRU
o.mnp_comm:	data.v42.H.MNP_EDE
o.mnp_bg:	data.v42.c.mnp_bg
o.mnp_bg:	inc.H.SYS_DEF
o.mnp_bg:	inc.h.__config
o.mnp_bg:	inc.h.risc_os
o.mnp_bg:	data.v42.EDF.V42
o.mnp_bg:	data.v42.EDF.MNP
o.mnp_bg:	data.v42.EDF.MNP_LIB
o.mnp_bg:	data.v42.EDF.MNP_AF
o.mnp_bg:	data.v42.EDF.MNP_LA
o.mnp_bg:	data.v42.EDF.MNP_LD
o.mnp_bg:	data.v42.EDF.MNP_LN
o.mnp_bg:	data.v42.EDF.MNP_LNA
o.mnp_bg:	data.v42.EDF.MNP_LR
o.mnp_bg:	data.v42.EDF.MNP_QUE
o.mnp_bg:	data.v42.EDF.MNP_LRR
o.mnp_bg:	data.v42.EDF.MNP_LT
o.mnp_bg:	data.v42.EDF.M10_IF
o.mnp_bg:	inc.EDF.DCE_LINE
o.mnp_bg:	inc.EDF.DTE
o.mnp_bg:	data.v42.EDF.BTINIT
o.mnp_bg:	data.v42.H.MNP_DEF
o.mnp_bg:	data.v42.H.MNP_STRU
o.mnp_bg:	data.v42.H.M10_DEF
o.mnp_lt:	data.v42.c.mnp_lt
o.mnp_lt:	inc.h.sys_def
o.mnp_lt:	inc.h.__config
o.mnp_lt:	inc.h.risc_os
o.mnp_lt:	data.v42.EDF.MNP_LIB
o.mnp_lt:	data.v42.EDF.MNP_AF
o.mnp_lt:	inc.EDF.DTE
o.mnp_lt:	data.v42.EDF.M10_IF
o.mnp_lt:	data.v42.H.MNP_DEF
o.mnp_lt:	data.v42.H.MNP_STRU
o.mnp_lt:	data.v42.H.MNP_EDE
o.mnp_dat:	data.v42.c.mnp_dat
o.mnp_dat:	inc.h.sys_def
o.mnp_dat:	inc.h.__config
o.mnp_dat:	inc.h.risc_os
o.mnp_dat:	data.v42.h.mnp_def
o.mnp_dat:	data.v42.h.mnp_stru
o.mnp_ld:	data.v42.c.mnp_ld
o.mnp_ld:	inc.h.sys_def
o.mnp_ld:	inc.h.__config
o.mnp_ld:	inc.h.risc_os
o.mnp_ld:	data.v42.EDF.MNP_LIB
o.mnp_ld:	data.v42.H.MNP_DEF
o.mnp_ld:	data.v42.H.MNP_STRU
o.mnp_ld:	data.v42.H.MNP_EDE
