#
#          Makefile for MULTILINK PPP and BACP
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -I^.^.par -I^.^.device -Ic: -I^.services -I^.mppp -I^.cc  -I^.dl -I^.v120 -I^.ph 

LIBFILES = 



# ----------------------------------------------------------------- #
# Various Macros
# ----------------------------------------------------------------- #



CC = cc
NOTMODULE_CFLAGS = $(INCLUDES) -c -Throwback -depend !Depend $(VARIANT_CFLAGS)
MODULE_CFLAGS = $(NOTMODULE_CFLAGS) -zM 
# -fc enables the limited pcc option (allows characters after endif) 
UNIX_CFLAGS = -fc

LINK = link
NOTMODULE_LINKFLAGS = -AOF
MODULE_LINKFLAGS = $(NOTMODULE_LINKFLAGS) 

CMHG = cmhg
CMHGFLAGS =

OBJASM = objasm 

AR = libfile
AR_FLAGS = -c -o

WIPE = -wipe
WIPEFLAGS = F~C~R~V

MODSQZ = modsqz -fv

# ----------------------------------------------------------------- #
# Rules for making things - do not change.
# ----------------------------------------------------------------- #
	   
.SUFFIXES:	.o .c .cmhg .s
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.c.o:;		$(CC) $(MODULE_CFLAGS) $<
.s.o:;		$(OBJASM) -from $< -to $@

# ----------------------------------------------------------------- #
# Targets to be compiled
# ----------------------------------------------------------------- #

TARGETS =   bacp.o\
	bacp_sta.o\
	bap.o\
	bap_sta.o\
	lcp.o\
	lcp_sta.o\
	mlcp.o\
	mlcp_sta.o\
	mp.o\
	mp_proc.o\
	mp_queue.o\
	mppp.o\
	mppp_acc.o\
	mppp_cc.o\
	mppp_cp.o\
	mppp_dbg.o\
	mppp_dl.o\
	mppp_fr.o\
	mppp_ph.o\
	mppp_rcv.o\
	mppp_snd.o\
	mppp_sta.o\
	mppp_uti.o\
	mppp_x25.o

#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.mppp:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.bacp:	c.bacp
o.bacp:	h.mppp
o.bacp:	^.services.h.services
o.bacp:	^.^.par.h.serv_par
o.bacp:	^.^.par.h.consts
o.bacp:	^.^.par.h._product
o.bacp:	c:h.string
o.bacp:	^.^.par.h.serv_por
o.bacp:	c:sys.h.types
o.bacp:	c:h.kernel
o.bacp:	c:h.swis
o.bacp:	h.mppp_int
o.bacp:	^.^.par.h.mppp_par
o.bacp:	h.mppp_dbg
o.bacp_sta:	c.bacp_sta
o.bacp_sta:	h.mppp
o.bacp_sta:	^.services.h.services
o.bacp_sta:	^.^.par.h.serv_par
o.bacp_sta:	^.^.par.h.consts
o.bacp_sta:	^.^.par.h._product
o.bacp_sta:	c:h.string
o.bacp_sta:	^.^.par.h.serv_por
o.bacp_sta:	c:sys.h.types
o.bacp_sta:	c:h.kernel
o.bacp_sta:	c:h.swis
o.bacp_sta:	h.mppp_int
o.bacp_sta:	^.^.par.h.mppp_par
o.bacp_sta:	h.mppp_dbg
o.bap:	c.bap
o.bap:	h.mppp
o.bap:	^.services.h.services
o.bap:	^.^.par.h.serv_par
o.bap:	^.^.par.h.consts
o.bap:	^.^.par.h._product
o.bap:	c:h.string
o.bap:	^.^.par.h.serv_por
o.bap:	c:sys.h.types
o.bap:	c:h.kernel
o.bap:	c:h.swis
o.bap:	h.mppp_int
o.bap:	^.^.par.h.mppp_par
o.bap:	h.mppp_dbg
o.bap_sta:	c.bap_sta
o.bap_sta:	h.mppp
o.bap_sta:	^.services.h.services
o.bap_sta:	^.^.par.h.serv_par
o.bap_sta:	^.^.par.h.consts
o.bap_sta:	^.^.par.h._product
o.bap_sta:	c:h.string
o.bap_sta:	^.^.par.h.serv_por
o.bap_sta:	c:sys.h.types
o.bap_sta:	c:h.kernel
o.bap_sta:	c:h.swis
o.bap_sta:	h.mppp_int
o.bap_sta:	^.^.par.h.mppp_par
o.bap_sta:	h.mppp_dbg
o.lcp:	c.lcp
o.lcp:	h.mppp
o.lcp:	^.services.h.services
o.lcp:	^.^.par.h.serv_par
o.lcp:	^.^.par.h.consts
o.lcp:	^.^.par.h._product
o.lcp:	c:h.string
o.lcp:	^.^.par.h.serv_por
o.lcp:	c:sys.h.types
o.lcp:	c:h.kernel
o.lcp:	c:h.swis
o.lcp:	h.mppp_int
o.lcp:	^.^.par.h.mppp_par
o.lcp:	h.mppp_dbg
o.lcp:	h.mppp_ext
o.lcp:	^.services.h.ctx_ext
o.lcp:	h.lcp
o.lcp_sta:	c.lcp_sta
o.lcp_sta:	h.mppp
o.lcp_sta:	^.services.h.services
o.lcp_sta:	^.^.par.h.serv_par
o.lcp_sta:	^.^.par.h.consts
o.lcp_sta:	^.^.par.h._product
o.lcp_sta:	c:h.string
o.lcp_sta:	^.^.par.h.serv_por
o.lcp_sta:	c:sys.h.types
o.lcp_sta:	c:h.kernel
o.lcp_sta:	c:h.swis
o.lcp_sta:	h.mppp_int
o.lcp_sta:	^.^.par.h.mppp_par
o.lcp_sta:	h.mppp_dbg
o.lcp_sta:	h.mppp_ext
o.lcp_sta:	^.services.h.ctx_ext
o.lcp_sta:	h.lcp
o.mlcp:	c.mlcp
o.mlcp:	h.mppp
o.mlcp:	^.services.h.services
o.mlcp:	^.^.par.h.serv_par
o.mlcp:	^.^.par.h.consts
o.mlcp:	^.^.par.h._product
o.mlcp:	c:h.string
o.mlcp:	^.^.par.h.serv_por
o.mlcp:	c:sys.h.types
o.mlcp:	c:h.kernel
o.mlcp:	c:h.swis
o.mlcp:	h.mppp_int
o.mlcp:	^.^.par.h.mppp_par
o.mlcp:	h.mppp_dbg
o.mlcp_sta:	c.mlcp_sta
o.mlcp_sta:	h.mppp
o.mlcp_sta:	^.services.h.services
o.mlcp_sta:	^.^.par.h.serv_par
o.mlcp_sta:	^.^.par.h.consts
o.mlcp_sta:	^.^.par.h._product
o.mlcp_sta:	c:h.string
o.mlcp_sta:	^.^.par.h.serv_por
o.mlcp_sta:	c:sys.h.types
o.mlcp_sta:	c:h.kernel
o.mlcp_sta:	c:h.swis
o.mlcp_sta:	h.mppp_int
o.mlcp_sta:	^.^.par.h.mppp_par
o.mlcp_sta:	h.mppp_dbg
o.mp:	c.mp
o.mp:	h.mppp
o.mp:	^.services.h.services
o.mp:	^.^.par.h.serv_par
o.mp:	^.^.par.h.consts
o.mp:	^.^.par.h._product
o.mp:	c:h.string
o.mp:	^.^.par.h.serv_por
o.mp:	c:sys.h.types
o.mp:	c:h.kernel
o.mp:	c:h.swis
o.mp:	h.mppp_int
o.mp:	^.^.par.h.mppp_par
o.mp:	h.mppp_dbg
o.mp:	h.mppp_ext
o.mp:	^.services.h.ctx_ext
o.mp:	h.mp
o.mp_proc:	c.mp_proc
o.mp_proc:	h.mppp
o.mp_proc:	^.services.h.services
o.mp_proc:	^.^.par.h.serv_par
o.mp_proc:	^.^.par.h.consts
o.mp_proc:	^.^.par.h._product
o.mp_proc:	c:h.string
o.mp_proc:	^.^.par.h.serv_por
o.mp_proc:	c:sys.h.types
o.mp_proc:	c:h.kernel
o.mp_proc:	c:h.swis
o.mp_proc:	h.mppp_int
o.mp_proc:	^.^.par.h.mppp_par
o.mp_proc:	h.mppp_dbg
o.mp_proc:	h.mppp_ext
o.mp_proc:	^.services.h.ctx_ext
o.mp_proc:	^.services.h.it
o.mp_proc:	^.^.par.h.it_par
o.mp_proc:	h.mp
o.mp_queue:	c.mp_queue
o.mp_queue:	h.mppp
o.mp_queue:	^.services.h.services
o.mp_queue:	^.^.par.h.serv_par
o.mp_queue:	^.^.par.h.consts
o.mp_queue:	^.^.par.h._product
o.mp_queue:	c:h.string
o.mp_queue:	^.^.par.h.serv_por
o.mp_queue:	c:sys.h.types
o.mp_queue:	c:h.kernel
o.mp_queue:	c:h.swis
o.mp_queue:	h.mppp_int
o.mp_queue:	^.^.par.h.mppp_par
o.mp_queue:	h.mppp_dbg
o.mp_queue:	h.mppp_ext
o.mp_queue:	^.services.h.ctx_ext
o.mp_queue:	^.services.h.it
o.mp_queue:	^.^.par.h.it_par
o.mp_queue:	h.mp
o.mppp:	c.mppp
o.mppp:	h.mppp
o.mppp:	^.services.h.services
o.mppp:	^.^.par.h.serv_par
o.mppp:	^.^.par.h.consts
o.mppp:	^.^.par.h._product
o.mppp:	c:h.string
o.mppp:	^.^.par.h.serv_por
o.mppp:	c:sys.h.types
o.mppp:	c:h.kernel
o.mppp:	c:h.swis
o.mppp:	h.mppp_int
o.mppp:	^.^.par.h.mppp_par
o.mppp:	h.mppp_dbg
o.mppp:	h.lcp
o.mppp:	h.mlcp
o.mppp:	h.mp
o.mppp_acc:	c.mppp_acc
o.mppp_acc:	h.mppp
o.mppp_acc:	^.services.h.services
o.mppp_acc:	^.^.par.h.serv_par
o.mppp_acc:	^.^.par.h.consts
o.mppp_acc:	^.^.par.h._product
o.mppp_acc:	c:h.string
o.mppp_acc:	^.^.par.h.serv_por
o.mppp_acc:	c:sys.h.types
o.mppp_acc:	c:h.kernel
o.mppp_acc:	c:h.swis
o.mppp_acc:	h.mppp_int
o.mppp_acc:	^.^.par.h.mppp_par
o.mppp_acc:	h.mppp_dbg
o.mppp_acc:	h.mppp_ext
o.mppp_acc:	^.services.h.ctx_ext
o.mppp_cc:	c.mppp_cc
o.mppp_cc:	h.mppp
o.mppp_cc:	^.services.h.services
o.mppp_cc:	^.^.par.h.serv_par
o.mppp_cc:	^.^.par.h.consts
o.mppp_cc:	^.^.par.h._product
o.mppp_cc:	c:h.string
o.mppp_cc:	^.^.par.h.serv_por
o.mppp_cc:	c:sys.h.types
o.mppp_cc:	c:h.kernel
o.mppp_cc:	c:h.swis
o.mppp_cc:	h.mppp_int
o.mppp_cc:	^.^.par.h.mppp_par
o.mppp_cc:	h.mppp_dbg
o.mppp_cc:	h.mppp_ext
o.mppp_cc:	^.services.h.ctx_ext
o.mppp_cc:	^.cc.h.cc_int
o.mppp_cc:	^.^.par.h.cc_par
o.mppp_cc:	^.cc.h.acu_int
o.mppp_cp:	c.mppp_cp
o.mppp_cp:	h.mppp
o.mppp_cp:	^.services.h.services
o.mppp_cp:	^.^.par.h.serv_par
o.mppp_cp:	^.^.par.h.consts
o.mppp_cp:	^.^.par.h._product
o.mppp_cp:	c:h.string
o.mppp_cp:	^.^.par.h.serv_por
o.mppp_cp:	c:sys.h.types
o.mppp_cp:	c:h.kernel
o.mppp_cp:	c:h.swis
o.mppp_cp:	h.mppp_int
o.mppp_cp:	^.^.par.h.mppp_par
o.mppp_cp:	h.mppp_dbg
o.mppp_cp:	h.mppp_ext
o.mppp_cp:	^.services.h.ctx_ext
o.mppp_cp:	h.lcp
o.mppp_cp:	h.mp
o.mppp_dbg:	c.mppp_dbg
o.mppp_dbg:	h.mppp
o.mppp_dbg:	^.services.h.services
o.mppp_dbg:	^.^.par.h.serv_par
o.mppp_dbg:	^.^.par.h.consts
o.mppp_dbg:	^.^.par.h._product
o.mppp_dbg:	c:h.string
o.mppp_dbg:	^.^.par.h.serv_por
o.mppp_dbg:	c:sys.h.types
o.mppp_dbg:	c:h.kernel
o.mppp_dbg:	c:h.swis
o.mppp_dbg:	h.mppp_int
o.mppp_dbg:	^.^.par.h.mppp_par
o.mppp_dbg:	h.mppp_dbg
o.mppp_dl:	c.mppp_dl
o.mppp_dl:	h.mppp
o.mppp_dl:	^.services.h.services
o.mppp_dl:	^.^.par.h.serv_par
o.mppp_dl:	^.^.par.h.consts
o.mppp_dl:	^.^.par.h._product
o.mppp_dl:	c:h.string
o.mppp_dl:	^.^.par.h.serv_por
o.mppp_dl:	c:sys.h.types
o.mppp_dl:	c:h.kernel
o.mppp_dl:	c:h.swis
o.mppp_dl:	h.mppp_int
o.mppp_dl:	^.^.par.h.mppp_par
o.mppp_dl:	h.mppp_dbg
o.mppp_dl:	h.mppp_ext
o.mppp_dl:	^.services.h.ctx_ext
o.mppp_fr:	c.mppp_fr
o.mppp_fr:	h.mppp
o.mppp_fr:	^.services.h.services
o.mppp_fr:	^.^.par.h.serv_par
o.mppp_fr:	^.^.par.h.consts
o.mppp_fr:	^.^.par.h._product
o.mppp_fr:	c:h.string
o.mppp_fr:	^.^.par.h.serv_por
o.mppp_fr:	c:sys.h.types
o.mppp_fr:	c:h.kernel
o.mppp_fr:	c:h.swis
o.mppp_fr:	h.mppp_int
o.mppp_fr:	^.^.par.h.mppp_par
o.mppp_fr:	h.mppp_dbg
o.mppp_fr:	h.mppp_ext
o.mppp_fr:	^.services.h.ctx_ext
o.mppp_ph:	c.mppp_ph
o.mppp_ph:	h.mppp
o.mppp_ph:	^.services.h.services
o.mppp_ph:	^.^.par.h.serv_par
o.mppp_ph:	^.^.par.h.consts
o.mppp_ph:	^.^.par.h._product
o.mppp_ph:	c:h.string
o.mppp_ph:	^.^.par.h.serv_por
o.mppp_ph:	c:sys.h.types
o.mppp_ph:	c:h.kernel
o.mppp_ph:	c:h.swis
o.mppp_ph:	h.mppp_int
o.mppp_ph:	^.^.par.h.mppp_par
o.mppp_ph:	h.mppp_dbg
o.mppp_ph:	h.mppp_ext
o.mppp_ph:	^.services.h.ctx_ext
o.mppp_ph:	^.ph.h.ph_int
o.mppp_ph:	^.^.par.h.ph_par
o.mppp_rcv:	c.mppp_rcv
o.mppp_rcv:	h.mppp
o.mppp_rcv:	^.services.h.services
o.mppp_rcv:	^.^.par.h.serv_par
o.mppp_rcv:	^.^.par.h.consts
o.mppp_rcv:	^.^.par.h._product
o.mppp_rcv:	c:h.string
o.mppp_rcv:	^.^.par.h.serv_por
o.mppp_rcv:	c:sys.h.types
o.mppp_rcv:	c:h.kernel
o.mppp_rcv:	c:h.swis
o.mppp_rcv:	h.mppp_int
o.mppp_rcv:	^.^.par.h.mppp_par
o.mppp_rcv:	h.mppp_dbg
o.mppp_rcv:	h.mppp_ext
o.mppp_rcv:	^.services.h.ctx_ext
o.mppp_rcv:	h.mp
o.mppp_snd:	c.mppp_snd
o.mppp_snd:	h.mppp
o.mppp_snd:	^.services.h.services
o.mppp_snd:	^.^.par.h.serv_par
o.mppp_snd:	^.^.par.h.consts
o.mppp_snd:	^.^.par.h._product
o.mppp_snd:	c:h.string
o.mppp_snd:	^.^.par.h.serv_por
o.mppp_snd:	c:sys.h.types
o.mppp_snd:	c:h.kernel
o.mppp_snd:	c:h.swis
o.mppp_snd:	h.mppp_int
o.mppp_snd:	^.^.par.h.mppp_par
o.mppp_snd:	h.mppp_dbg
o.mppp_snd:	h.mppp_ext
o.mppp_snd:	^.services.h.ctx_ext
o.mppp_snd:	h.mp
o.mppp_snd:	^.ph.h.ph_int
o.mppp_snd:	^.^.par.h.ph_par
o.mppp_snd:	^.cc.h.cc_int
o.mppp_snd:	^.^.par.h.cc_par
o.mppp_snd:	^.cc.h.acu_int
o.mppp_sta:	c.mppp_sta
o.mppp_sta:	h.mppp
o.mppp_sta:	^.services.h.services
o.mppp_sta:	^.^.par.h.serv_par
o.mppp_sta:	^.^.par.h.consts
o.mppp_sta:	^.^.par.h._product
o.mppp_sta:	c:h.string
o.mppp_sta:	^.^.par.h.serv_por
o.mppp_sta:	c:sys.h.types
o.mppp_sta:	c:h.kernel
o.mppp_sta:	c:h.swis
o.mppp_sta:	h.mppp_int
o.mppp_sta:	^.^.par.h.mppp_par
o.mppp_sta:	h.mppp_dbg
o.mppp_sta:	h.mppp_ext
o.mppp_sta:	^.services.h.ctx_ext
o.mppp_sta:	h.mp
o.mppp_sta:	^.cc.h.cc_int
o.mppp_sta:	^.^.par.h.cc_par
o.mppp_sta:	^.cc.h.acu_int
o.mppp_uti:	c.mppp_uti
o.mppp_uti:	h.mppp
o.mppp_uti:	^.services.h.services
o.mppp_uti:	^.^.par.h.serv_par
o.mppp_uti:	^.^.par.h.consts
o.mppp_uti:	^.^.par.h._product
o.mppp_uti:	c:h.string
o.mppp_uti:	^.^.par.h.serv_por
o.mppp_uti:	c:sys.h.types
o.mppp_uti:	c:h.kernel
o.mppp_uti:	c:h.swis
o.mppp_uti:	h.mppp_int
o.mppp_uti:	^.^.par.h.mppp_par
o.mppp_uti:	h.mppp_dbg
o.mppp_uti:	h.mppp_ext
o.mppp_uti:	^.services.h.ctx_ext
o.mppp_x25:	c.mppp_x25
o.mppp_x25:	h.mppp
o.mppp_x25:	^.services.h.services
o.mppp_x25:	^.^.par.h.serv_par
o.mppp_x25:	^.^.par.h.consts
o.mppp_x25:	^.^.par.h._product
o.mppp_x25:	c:h.string
o.mppp_x25:	^.^.par.h.serv_por
o.mppp_x25:	c:sys.h.types
o.mppp_x25:	c:h.kernel
o.mppp_x25:	c:h.swis
o.mppp_x25:	h.mppp_int
o.mppp_x25:	^.^.par.h.mppp_par
o.mppp_x25:	h.mppp_dbg
o.mppp_x25:	h.mppp_ext
o.mppp_x25:	^.services.h.ctx_ext
