#
#          Makefile for Omnitel Call Control entity
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -Ic: -I^.^.par -I^.services -I^.ns -I^.dl -I^.ph

LIBFILES = 



# ----------------------------------------------------------------- #
# Various Macros
# ----------------------------------------------------------------- #



CC = cc
NOTMODULE_CFLAGS = $(INCLUDES) -c -Throwback -depend !Depend $(VARIANT_CFLAGS)
MODULE_CFLAGS = $(NOTMODULE_CFLAGS) -zM -ff 

LINK = link
NOTMODULE_LINKFLAGS = -AOF -V
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

TARGETS = cc.o\
 cc_proc.o\
 cc_accss.o\
 cc_rcv.o\
 cc_snd.o\
 cc_switc.o\
 cc_state.o\
 cc_a11.o\
 cc_a17.o\
 cc_at4.o\
 cc_at5.o\
 cc_at9.o\
 cc_au1.o\
 cc_bt2.o\
 cc_bv1.o\
 cc_bip.o\
 cc_dms.o\
 cc_ets.o\
 cc_e10.o\
 cc_hkt.o\
 cc_kdd.o\
 cc_ni1.o\
 cc_ni2.o\
 cc_ntt.o\
 cc_qsi.o\
 cc_swd.o\
 cc_tr6.o\
 cc_vn2.o\
 cc_vn3.o\
 cc_vn6.o

 
#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.cc:   $(TARGETS)
	    $(LINK) $(NOTMODULE_LINKFLAGS) -o $@ -VIA Target1 -VIA Target2
	    		


clean:;	    @$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.cc:	c.cc
o.cc:	h.cc
o.cc:	^.services.h.services
o.cc:	^.^.par.h.serv_par
o.cc:	^.^.par.h.consts
o.cc:	^.^.par.h._product
o.cc:	c:h.string
o.cc:	^.^.par.h.serv_por
o.cc:	c:sys.h.types
o.cc:	c:h.kernel
o.cc:	c:h.swis
o.cc:	^.ns.h.ns_int
o.cc:	^.^.par.h.ns_par
o.cc:	^.ns.ets.net_par
o.cc:	^.ns.vn6.net_par
o.cc:	^.ns.ntt.net_par
o.cc:	h.cc_int
o.cc:	^.^.par.h.cc_par
o.cc:	h.acu_int
o.cc:	^.ns.h.isdn
o.cc:	^.ph.h.ph_int
o.cc:	^.^.par.h.ph_par
o.cc:	h.cc_fct
o.cc_proc:	c.cc_proc
o.cc_proc:	h.cc
o.cc_proc:	^.services.h.services
o.cc_proc:	^.^.par.h.serv_par
o.cc_proc:	^.^.par.h.consts
o.cc_proc:	^.^.par.h._product
o.cc_proc:	c:h.string
o.cc_proc:	^.^.par.h.serv_por
o.cc_proc:	c:sys.h.types
o.cc_proc:	c:h.kernel
o.cc_proc:	c:h.swis
o.cc_proc:	^.ns.h.ns_int
o.cc_proc:	^.^.par.h.ns_par
o.cc_proc:	^.ns.ets.net_par
o.cc_proc:	^.ns.vn6.net_par
o.cc_proc:	^.ns.ntt.net_par
o.cc_proc:	h.cc_int
o.cc_proc:	^.^.par.h.cc_par
o.cc_proc:	h.acu_int
o.cc_proc:	^.ns.h.isdn
o.cc_proc:	h.cc_ie
o.cc_proc:	^.services.h.ctx_ext
o.cc_proc:	^.ph.h.ph_int
o.cc_proc:	^.^.par.h.ph_par
o.cc_proc:	h.cc_prof
o.cc_proc:	h.cc_fct
o.cc_accss:	c.cc_accss
o.cc_accss:	h.cc
o.cc_accss:	^.services.h.services
o.cc_accss:	^.^.par.h.serv_par
o.cc_accss:	^.^.par.h.consts
o.cc_accss:	^.^.par.h._product
o.cc_accss:	c:h.string
o.cc_accss:	^.^.par.h.serv_por
o.cc_accss:	c:sys.h.types
o.cc_accss:	c:h.kernel
o.cc_accss:	c:h.swis
o.cc_accss:	^.ns.h.ns_int
o.cc_accss:	^.^.par.h.ns_par
o.cc_accss:	^.ns.ets.net_par
o.cc_accss:	^.ns.vn6.net_par
o.cc_accss:	^.ns.ntt.net_par
o.cc_accss:	h.cc_int
o.cc_accss:	^.^.par.h.cc_par
o.cc_accss:	h.acu_int
o.cc_accss:	^.ns.h.isdn
o.cc_rcv:	c.cc_rcv
o.cc_rcv:	h.cc
o.cc_rcv:	^.services.h.services
o.cc_rcv:	^.^.par.h.serv_par
o.cc_rcv:	^.^.par.h.consts
o.cc_rcv:	^.^.par.h._product
o.cc_rcv:	c:h.string
o.cc_rcv:	^.^.par.h.serv_por
o.cc_rcv:	c:sys.h.types
o.cc_rcv:	c:h.kernel
o.cc_rcv:	c:h.swis
o.cc_rcv:	^.ns.h.ns_int
o.cc_rcv:	^.^.par.h.ns_par
o.cc_rcv:	^.ns.ets.net_par
o.cc_rcv:	^.ns.vn6.net_par
o.cc_rcv:	^.ns.ntt.net_par
o.cc_rcv:	h.cc_int
o.cc_rcv:	^.^.par.h.cc_par
o.cc_rcv:	h.acu_int
o.cc_rcv:	^.ns.h.isdn
o.cc_rcv:	h.cc_ie
o.cc_rcv:	^.services.h.ctx_ext
o.cc_rcv:	h.cc_undef
o.cc_rcv:	^.ns.h.ns_mac
o.cc_rcv:	h.cc_undef
o.cc_snd:	c.cc_snd
o.cc_snd:	h.cc
o.cc_snd:	^.services.h.services
o.cc_snd:	^.^.par.h.serv_par
o.cc_snd:	^.^.par.h.consts
o.cc_snd:	^.^.par.h._product
o.cc_snd:	c:h.string
o.cc_snd:	^.^.par.h.serv_por
o.cc_snd:	c:sys.h.types
o.cc_snd:	c:h.kernel
o.cc_snd:	c:h.swis
o.cc_snd:	^.ns.h.ns_int
o.cc_snd:	^.^.par.h.ns_par
o.cc_snd:	^.ns.ets.net_par
o.cc_snd:	^.ns.vn6.net_par
o.cc_snd:	^.ns.ntt.net_par
o.cc_snd:	h.cc_int
o.cc_snd:	^.^.par.h.cc_par
o.cc_snd:	h.acu_int
o.cc_snd:	^.ns.h.isdn
o.cc_snd:	^.services.h.ctx_ext
o.cc_snd:	^.ph.h.ph_int
o.cc_snd:	^.^.par.h.ph_par
o.cc_switc:	c.cc_switc
o.cc_switc:	h.cc
o.cc_switc:	^.services.h.services
o.cc_switc:	^.^.par.h.serv_par
o.cc_switc:	^.^.par.h.consts
o.cc_switc:	^.^.par.h._product
o.cc_switc:	c:h.string
o.cc_switc:	^.^.par.h.serv_por
o.cc_switc:	c:sys.h.types
o.cc_switc:	c:h.kernel
o.cc_switc:	c:h.swis
o.cc_switc:	^.ns.h.ns_int
o.cc_switc:	^.^.par.h.ns_par
o.cc_switc:	^.ns.ets.net_par
o.cc_switc:	^.ns.vn6.net_par
o.cc_switc:	^.ns.ntt.net_par
o.cc_switc:	h.cc_int
o.cc_switc:	^.^.par.h.cc_par
o.cc_switc:	h.acu_int
o.cc_switc:	^.ns.h.isdn
o.cc_switc:	h.cc_ie
o.cc_switc:	^.services.h.ctx_ext
o.cc_switc:	^.ph.h.ph_int
o.cc_switc:	^.^.par.h.ph_par
o.cc_switc:	h.cc_undef
o.cc_switc:	^.ns.h.ns_mac
o.cc_switc:	h.cc_undef
o.cc_state:	c.cc_state
o.cc_state:	h.cc
o.cc_state:	^.services.h.services
o.cc_state:	^.^.par.h.serv_par
o.cc_state:	^.^.par.h.consts
o.cc_state:	^.^.par.h._product
o.cc_state:	c:h.string
o.cc_state:	^.^.par.h.serv_por
o.cc_state:	c:sys.h.types
o.cc_state:	c:h.kernel
o.cc_state:	c:h.swis
o.cc_state:	^.ns.h.ns_int
o.cc_state:	^.^.par.h.ns_par
o.cc_state:	^.ns.ets.net_par
o.cc_state:	^.ns.vn6.net_par
o.cc_state:	^.ns.ntt.net_par
o.cc_state:	h.cc_int
o.cc_state:	^.^.par.h.cc_par
o.cc_state:	h.acu_int
o.cc_state:	^.ns.h.isdn
o.cc_state:	h.cc_ie
o.cc_state:	^.services.h.ctx_ext
o.cc_state:	^.ph.h.ph_int
o.cc_state:	^.^.par.h.ph_par
o.cc_state:	h.cc_undef
o.cc_state:	^.ns.h.ns_mac
o.cc_state:	h.cc_undef
o.cc_a11:	c.cc_a11
o.cc_a11:	h.cc
o.cc_a11:	^.services.h.services
o.cc_a11:	^.^.par.h.serv_par
o.cc_a11:	^.^.par.h.consts
o.cc_a11:	^.^.par.h._product
o.cc_a11:	c:h.string
o.cc_a11:	^.^.par.h.serv_por
o.cc_a11:	c:sys.h.types
o.cc_a11:	c:h.kernel
o.cc_a11:	c:h.swis
o.cc_a11:	^.ns.h.ns_int
o.cc_a11:	^.^.par.h.ns_par
o.cc_a11:	^.ns.ets.net_par
o.cc_a11:	^.ns.vn6.net_par
o.cc_a11:	^.ns.ntt.net_par
o.cc_a11:	h.cc_int
o.cc_a11:	^.^.par.h.cc_par
o.cc_a11:	h.acu_int
o.cc_a11:	^.ns.h.isdn
o.cc_a17:	c.cc_a17
o.cc_a17:	h.cc
o.cc_a17:	^.services.h.services
o.cc_a17:	^.^.par.h.serv_par
o.cc_a17:	^.^.par.h.consts
o.cc_a17:	^.^.par.h._product
o.cc_a17:	c:h.string
o.cc_a17:	^.^.par.h.serv_por
o.cc_a17:	c:sys.h.types
o.cc_a17:	c:h.kernel
o.cc_a17:	c:h.swis
o.cc_a17:	^.ns.h.ns_int
o.cc_a17:	^.^.par.h.ns_par
o.cc_a17:	^.ns.ets.net_par
o.cc_a17:	^.ns.vn6.net_par
o.cc_a17:	^.ns.ntt.net_par
o.cc_a17:	h.cc_int
o.cc_a17:	^.^.par.h.cc_par
o.cc_a17:	h.acu_int
o.cc_a17:	^.ns.h.isdn
o.cc_at4:	c.cc_at4
o.cc_at4:	h.cc
o.cc_at4:	^.services.h.services
o.cc_at4:	^.^.par.h.serv_par
o.cc_at4:	^.^.par.h.consts
o.cc_at4:	^.^.par.h._product
o.cc_at4:	c:h.string
o.cc_at4:	^.^.par.h.serv_por
o.cc_at4:	c:sys.h.types
o.cc_at4:	c:h.kernel
o.cc_at4:	c:h.swis
o.cc_at4:	^.ns.h.ns_int
o.cc_at4:	^.^.par.h.ns_par
o.cc_at4:	^.ns.ets.net_par
o.cc_at4:	^.ns.vn6.net_par
o.cc_at4:	^.ns.ntt.net_par
o.cc_at4:	h.cc_int
o.cc_at4:	^.^.par.h.cc_par
o.cc_at4:	h.acu_int
o.cc_at4:	^.ns.h.isdn
o.cc_at5:	c.cc_at5
o.cc_at5:	h.cc
o.cc_at5:	^.services.h.services
o.cc_at5:	^.^.par.h.serv_par
o.cc_at5:	^.^.par.h.consts
o.cc_at5:	^.^.par.h._product
o.cc_at5:	c:h.string
o.cc_at5:	^.^.par.h.serv_por
o.cc_at5:	c:sys.h.types
o.cc_at5:	c:h.kernel
o.cc_at5:	c:h.swis
o.cc_at5:	^.ns.h.ns_int
o.cc_at5:	^.^.par.h.ns_par
o.cc_at5:	^.ns.ets.net_par
o.cc_at5:	^.ns.vn6.net_par
o.cc_at5:	^.ns.ntt.net_par
o.cc_at5:	h.cc_int
o.cc_at5:	^.^.par.h.cc_par
o.cc_at5:	h.acu_int
o.cc_at5:	^.ns.h.isdn
o.cc_at9:	c.cc_at9
o.cc_at9:	h.cc
o.cc_at9:	^.services.h.services
o.cc_at9:	^.^.par.h.serv_par
o.cc_at9:	^.^.par.h.consts
o.cc_at9:	^.^.par.h._product
o.cc_at9:	c:h.string
o.cc_at9:	^.^.par.h.serv_por
o.cc_at9:	c:sys.h.types
o.cc_at9:	c:h.kernel
o.cc_at9:	c:h.swis
o.cc_at9:	^.ns.h.ns_int
o.cc_at9:	^.^.par.h.ns_par
o.cc_at9:	^.ns.ets.net_par
o.cc_at9:	^.ns.vn6.net_par
o.cc_at9:	^.ns.ntt.net_par
o.cc_at9:	h.cc_int
o.cc_at9:	^.^.par.h.cc_par
o.cc_at9:	h.acu_int
o.cc_at9:	^.ns.h.isdn
o.cc_au1:	c.cc_au1
o.cc_au1:	h.cc
o.cc_au1:	^.services.h.services
o.cc_au1:	^.^.par.h.serv_par
o.cc_au1:	^.^.par.h.consts
o.cc_au1:	^.^.par.h._product
o.cc_au1:	c:h.string
o.cc_au1:	^.^.par.h.serv_por
o.cc_au1:	c:sys.h.types
o.cc_au1:	c:h.kernel
o.cc_au1:	c:h.swis
o.cc_au1:	^.ns.h.ns_int
o.cc_au1:	^.^.par.h.ns_par
o.cc_au1:	^.ns.ets.net_par
o.cc_au1:	^.ns.vn6.net_par
o.cc_au1:	^.ns.ntt.net_par
o.cc_au1:	h.cc_int
o.cc_au1:	^.^.par.h.cc_par
o.cc_au1:	h.acu_int
o.cc_au1:	^.ns.h.isdn
o.cc_bt2:	c.cc_bt2
o.cc_bt2:	h.cc
o.cc_bt2:	^.services.h.services
o.cc_bt2:	^.^.par.h.serv_par
o.cc_bt2:	^.^.par.h.consts
o.cc_bt2:	^.^.par.h._product
o.cc_bt2:	c:h.string
o.cc_bt2:	^.^.par.h.serv_por
o.cc_bt2:	c:sys.h.types
o.cc_bt2:	c:h.kernel
o.cc_bt2:	c:h.swis
o.cc_bt2:	^.ns.h.ns_int
o.cc_bt2:	^.^.par.h.ns_par
o.cc_bt2:	^.ns.ets.net_par
o.cc_bt2:	^.ns.vn6.net_par
o.cc_bt2:	^.ns.ntt.net_par
o.cc_bt2:	h.cc_int
o.cc_bt2:	^.^.par.h.cc_par
o.cc_bt2:	h.acu_int
o.cc_bt2:	^.ns.h.isdn
o.cc_bv1:	c.cc_bv1
o.cc_bv1:	h.cc
o.cc_bv1:	^.services.h.services
o.cc_bv1:	^.^.par.h.serv_par
o.cc_bv1:	^.^.par.h.consts
o.cc_bv1:	^.^.par.h._product
o.cc_bv1:	c:h.string
o.cc_bv1:	^.^.par.h.serv_por
o.cc_bv1:	c:sys.h.types
o.cc_bv1:	c:h.kernel
o.cc_bv1:	c:h.swis
o.cc_bv1:	^.ns.h.ns_int
o.cc_bv1:	^.^.par.h.ns_par
o.cc_bv1:	^.ns.ets.net_par
o.cc_bv1:	^.ns.vn6.net_par
o.cc_bv1:	^.ns.ntt.net_par
o.cc_bv1:	h.cc_int
o.cc_bv1:	^.^.par.h.cc_par
o.cc_bv1:	h.acu_int
o.cc_bv1:	^.ns.h.isdn
o.cc_bip:	c.cc_bip
o.cc_bip:	h.cc
o.cc_bip:	^.services.h.services
o.cc_bip:	^.^.par.h.serv_par
o.cc_bip:	^.^.par.h.consts
o.cc_bip:	^.^.par.h._product
o.cc_bip:	c:h.string
o.cc_bip:	^.^.par.h.serv_por
o.cc_bip:	c:sys.h.types
o.cc_bip:	c:h.kernel
o.cc_bip:	c:h.swis
o.cc_bip:	^.ns.h.ns_int
o.cc_bip:	^.^.par.h.ns_par
o.cc_bip:	^.ns.ets.net_par
o.cc_bip:	^.ns.vn6.net_par
o.cc_bip:	^.ns.ntt.net_par
o.cc_bip:	h.cc_int
o.cc_bip:	^.^.par.h.cc_par
o.cc_bip:	h.acu_int
o.cc_bip:	^.ns.h.isdn
o.cc_dms:	c.cc_dms
o.cc_dms:	h.cc
o.cc_dms:	^.services.h.services
o.cc_dms:	^.^.par.h.serv_par
o.cc_dms:	^.^.par.h.consts
o.cc_dms:	^.^.par.h._product
o.cc_dms:	c:h.string
o.cc_dms:	^.^.par.h.serv_por
o.cc_dms:	c:sys.h.types
o.cc_dms:	c:h.kernel
o.cc_dms:	c:h.swis
o.cc_dms:	^.ns.h.ns_int
o.cc_dms:	^.^.par.h.ns_par
o.cc_dms:	^.ns.ets.net_par
o.cc_dms:	^.ns.vn6.net_par
o.cc_dms:	^.ns.ntt.net_par
o.cc_dms:	h.cc_int
o.cc_dms:	^.^.par.h.cc_par
o.cc_dms:	h.acu_int
o.cc_dms:	^.ns.h.isdn
o.cc_ets:	c.cc_ets
o.cc_ets:	h.cc
o.cc_ets:	^.services.h.services
o.cc_ets:	^.^.par.h.serv_par
o.cc_ets:	^.^.par.h.consts
o.cc_ets:	^.^.par.h._product
o.cc_ets:	c:h.string
o.cc_ets:	^.^.par.h.serv_por
o.cc_ets:	c:sys.h.types
o.cc_ets:	c:h.kernel
o.cc_ets:	c:h.swis
o.cc_ets:	^.ns.h.ns_int
o.cc_ets:	^.^.par.h.ns_par
o.cc_ets:	^.ns.ets.net_par
o.cc_ets:	^.ns.vn6.net_par
o.cc_ets:	^.ns.ntt.net_par
o.cc_ets:	h.cc_int
o.cc_ets:	^.^.par.h.cc_par
o.cc_ets:	h.acu_int
o.cc_ets:	^.ns.h.isdn
o.cc_ets:	^.services.h.ctx_ext
o.cc_ets:	^.ph.h.ph_int
o.cc_ets:	^.^.par.h.ph_par
o.cc_ets:	h.cc_undef
o.cc_ets:	^.ns.h.ns_mac
o.cc_ets:	^.ns.ets.net_mac
o.cc_ets:	h.cc_ie
o.cc_ets:	h.cc_fct
o.cc_ets:	h.cc_build
o.cc_ets:	ets.cc_fct
o.cc_ets:	ets.cc_proc
o.cc_ets:	ets.cc_serv
o.cc_ets:	ets.cc_build
o.cc_e10:	c.cc_e10
o.cc_e10:	h.cc
o.cc_e10:	^.services.h.services
o.cc_e10:	^.^.par.h.serv_par
o.cc_e10:	^.^.par.h.consts
o.cc_e10:	^.^.par.h._product
o.cc_e10:	c:h.string
o.cc_e10:	^.^.par.h.serv_por
o.cc_e10:	c:sys.h.types
o.cc_e10:	c:h.kernel
o.cc_e10:	c:h.swis
o.cc_e10:	^.ns.h.ns_int
o.cc_e10:	^.^.par.h.ns_par
o.cc_e10:	^.ns.ets.net_par
o.cc_e10:	^.ns.vn6.net_par
o.cc_e10:	^.ns.ntt.net_par
o.cc_e10:	h.cc_int
o.cc_e10:	^.^.par.h.cc_par
o.cc_e10:	h.acu_int
o.cc_e10:	^.ns.h.isdn
o.cc_hkt:	c.cc_hkt
o.cc_hkt:	h.cc
o.cc_hkt:	^.services.h.services
o.cc_hkt:	^.^.par.h.serv_par
o.cc_hkt:	^.^.par.h.consts
o.cc_hkt:	^.^.par.h._product
o.cc_hkt:	c:h.string
o.cc_hkt:	^.^.par.h.serv_por
o.cc_hkt:	c:sys.h.types
o.cc_hkt:	c:h.kernel
o.cc_hkt:	c:h.swis
o.cc_hkt:	^.ns.h.ns_int
o.cc_hkt:	^.^.par.h.ns_par
o.cc_hkt:	^.ns.ets.net_par
o.cc_hkt:	^.ns.vn6.net_par
o.cc_hkt:	^.ns.ntt.net_par
o.cc_hkt:	h.cc_int
o.cc_hkt:	^.^.par.h.cc_par
o.cc_hkt:	h.acu_int
o.cc_hkt:	^.ns.h.isdn
o.cc_kdd:	c.cc_kdd
o.cc_kdd:	h.cc
o.cc_kdd:	^.services.h.services
o.cc_kdd:	^.^.par.h.serv_par
o.cc_kdd:	^.^.par.h.consts
o.cc_kdd:	^.^.par.h._product
o.cc_kdd:	c:h.string
o.cc_kdd:	^.^.par.h.serv_por
o.cc_kdd:	c:sys.h.types
o.cc_kdd:	c:h.kernel
o.cc_kdd:	c:h.swis
o.cc_kdd:	^.ns.h.ns_int
o.cc_kdd:	^.^.par.h.ns_par
o.cc_kdd:	^.ns.ets.net_par
o.cc_kdd:	^.ns.vn6.net_par
o.cc_kdd:	^.ns.ntt.net_par
o.cc_kdd:	h.cc_int
o.cc_kdd:	^.^.par.h.cc_par
o.cc_kdd:	h.acu_int
o.cc_kdd:	^.ns.h.isdn
o.cc_ni1:	c.cc_ni1
o.cc_ni1:	h.cc
o.cc_ni1:	^.services.h.services
o.cc_ni1:	^.^.par.h.serv_par
o.cc_ni1:	^.^.par.h.consts
o.cc_ni1:	^.^.par.h._product
o.cc_ni1:	c:h.string
o.cc_ni1:	^.^.par.h.serv_por
o.cc_ni1:	c:sys.h.types
o.cc_ni1:	c:h.kernel
o.cc_ni1:	c:h.swis
o.cc_ni1:	^.ns.h.ns_int
o.cc_ni1:	^.^.par.h.ns_par
o.cc_ni1:	^.ns.ets.net_par
o.cc_ni1:	^.ns.vn6.net_par
o.cc_ni1:	^.ns.ntt.net_par
o.cc_ni1:	h.cc_int
o.cc_ni1:	^.^.par.h.cc_par
o.cc_ni1:	h.acu_int
o.cc_ni1:	^.ns.h.isdn
o.cc_ni2:	c.cc_ni2
o.cc_ni2:	h.cc
o.cc_ni2:	^.services.h.services
o.cc_ni2:	^.^.par.h.serv_par
o.cc_ni2:	^.^.par.h.consts
o.cc_ni2:	^.^.par.h._product
o.cc_ni2:	c:h.string
o.cc_ni2:	^.^.par.h.serv_por
o.cc_ni2:	c:sys.h.types
o.cc_ni2:	c:h.kernel
o.cc_ni2:	c:h.swis
o.cc_ni2:	^.ns.h.ns_int
o.cc_ni2:	^.^.par.h.ns_par
o.cc_ni2:	^.ns.ets.net_par
o.cc_ni2:	^.ns.vn6.net_par
o.cc_ni2:	^.ns.ntt.net_par
o.cc_ni2:	h.cc_int
o.cc_ni2:	^.^.par.h.cc_par
o.cc_ni2:	h.acu_int
o.cc_ni2:	^.ns.h.isdn
o.cc_ntt:	c.cc_ntt
o.cc_ntt:	h.cc
o.cc_ntt:	^.services.h.services
o.cc_ntt:	^.^.par.h.serv_par
o.cc_ntt:	^.^.par.h.consts
o.cc_ntt:	^.^.par.h._product
o.cc_ntt:	c:h.string
o.cc_ntt:	^.^.par.h.serv_por
o.cc_ntt:	c:sys.h.types
o.cc_ntt:	c:h.kernel
o.cc_ntt:	c:h.swis
o.cc_ntt:	^.ns.h.ns_int
o.cc_ntt:	^.^.par.h.ns_par
o.cc_ntt:	^.ns.ets.net_par
o.cc_ntt:	^.ns.vn6.net_par
o.cc_ntt:	^.ns.ntt.net_par
o.cc_ntt:	h.cc_int
o.cc_ntt:	^.^.par.h.cc_par
o.cc_ntt:	h.acu_int
o.cc_ntt:	^.ns.h.isdn
o.cc_ntt:	^.services.h.ctx_ext
o.cc_ntt:	^.ph.h.ph_int
o.cc_ntt:	^.^.par.h.ph_par
o.cc_ntt:	h.cc_undef
o.cc_ntt:	^.ns.h.ns_mac
o.cc_ntt:	^.ns.ntt.net_mac
o.cc_ntt:	h.cc_ie
o.cc_ntt:	h.cc_fct
o.cc_ntt:	h.cc_build
o.cc_ntt:	ntt.cc_fct
o.cc_ntt:	ntt.cc_proc
o.cc_ntt:	ntt.cc_serv
o.cc_ntt:	ntt.cc_build
o.cc_qsi:	c.cc_qsi
o.cc_qsi:	h.cc
o.cc_qsi:	^.services.h.services
o.cc_qsi:	^.^.par.h.serv_par
o.cc_qsi:	^.^.par.h.consts
o.cc_qsi:	^.^.par.h._product
o.cc_qsi:	c:h.string
o.cc_qsi:	^.^.par.h.serv_por
o.cc_qsi:	c:sys.h.types
o.cc_qsi:	c:h.kernel
o.cc_qsi:	c:h.swis
o.cc_qsi:	^.ns.h.ns_int
o.cc_qsi:	^.^.par.h.ns_par
o.cc_qsi:	^.ns.ets.net_par
o.cc_qsi:	^.ns.vn6.net_par
o.cc_qsi:	^.ns.ntt.net_par
o.cc_qsi:	h.cc_int
o.cc_qsi:	^.^.par.h.cc_par
o.cc_qsi:	h.acu_int
o.cc_qsi:	^.ns.h.isdn
o.cc_swd:	c.cc_swd
o.cc_swd:	h.cc
o.cc_swd:	^.services.h.services
o.cc_swd:	^.^.par.h.serv_par
o.cc_swd:	^.^.par.h.consts
o.cc_swd:	^.^.par.h._product
o.cc_swd:	c:h.string
o.cc_swd:	^.^.par.h.serv_por
o.cc_swd:	c:sys.h.types
o.cc_swd:	c:h.kernel
o.cc_swd:	c:h.swis
o.cc_swd:	^.ns.h.ns_int
o.cc_swd:	^.^.par.h.ns_par
o.cc_swd:	^.ns.ets.net_par
o.cc_swd:	^.ns.vn6.net_par
o.cc_swd:	^.ns.ntt.net_par
o.cc_swd:	h.cc_int
o.cc_swd:	^.^.par.h.cc_par
o.cc_swd:	h.acu_int
o.cc_swd:	^.ns.h.isdn
o.cc_tr6:	c.cc_tr6
o.cc_tr6:	h.cc
o.cc_tr6:	^.services.h.services
o.cc_tr6:	^.^.par.h.serv_par
o.cc_tr6:	^.^.par.h.consts
o.cc_tr6:	^.^.par.h._product
o.cc_tr6:	c:h.string
o.cc_tr6:	^.^.par.h.serv_por
o.cc_tr6:	c:sys.h.types
o.cc_tr6:	c:h.kernel
o.cc_tr6:	c:h.swis
o.cc_tr6:	^.ns.h.ns_int
o.cc_tr6:	^.^.par.h.ns_par
o.cc_tr6:	^.ns.ets.net_par
o.cc_tr6:	^.ns.vn6.net_par
o.cc_tr6:	^.ns.ntt.net_par
o.cc_tr6:	h.cc_int
o.cc_tr6:	^.^.par.h.cc_par
o.cc_tr6:	h.acu_int
o.cc_tr6:	^.ns.h.isdn
o.cc_vn2:	c.cc_vn2
o.cc_vn2:	h.cc
o.cc_vn2:	^.services.h.services
o.cc_vn2:	^.^.par.h.serv_par
o.cc_vn2:	^.^.par.h.consts
o.cc_vn2:	^.^.par.h._product
o.cc_vn2:	c:h.string
o.cc_vn2:	^.^.par.h.serv_por
o.cc_vn2:	c:sys.h.types
o.cc_vn2:	c:h.kernel
o.cc_vn2:	c:h.swis
o.cc_vn2:	^.ns.h.ns_int
o.cc_vn2:	^.^.par.h.ns_par
o.cc_vn2:	^.ns.ets.net_par
o.cc_vn2:	^.ns.vn6.net_par
o.cc_vn2:	^.ns.ntt.net_par
o.cc_vn2:	h.cc_int
o.cc_vn2:	^.^.par.h.cc_par
o.cc_vn2:	h.acu_int
o.cc_vn2:	^.ns.h.isdn
o.cc_vn3:	c.cc_vn3
o.cc_vn3:	h.cc
o.cc_vn3:	^.services.h.services
o.cc_vn3:	^.^.par.h.serv_par
o.cc_vn3:	^.^.par.h.consts
o.cc_vn3:	^.^.par.h._product
o.cc_vn3:	c:h.string
o.cc_vn3:	^.^.par.h.serv_por
o.cc_vn3:	c:sys.h.types
o.cc_vn3:	c:h.kernel
o.cc_vn3:	c:h.swis
o.cc_vn3:	^.ns.h.ns_int
o.cc_vn3:	^.^.par.h.ns_par
o.cc_vn3:	^.ns.ets.net_par
o.cc_vn3:	^.ns.vn6.net_par
o.cc_vn3:	^.ns.ntt.net_par
o.cc_vn3:	h.cc_int
o.cc_vn3:	^.^.par.h.cc_par
o.cc_vn3:	h.acu_int
o.cc_vn3:	^.ns.h.isdn
o.cc_vn6:	c.cc_vn6
o.cc_vn6:	h.cc
o.cc_vn6:	^.services.h.services
o.cc_vn6:	^.^.par.h.serv_par
o.cc_vn6:	^.^.par.h.consts
o.cc_vn6:	^.^.par.h._product
o.cc_vn6:	c:h.string
o.cc_vn6:	^.^.par.h.serv_por
o.cc_vn6:	c:sys.h.types
o.cc_vn6:	c:h.kernel
o.cc_vn6:	c:h.swis
o.cc_vn6:	^.ns.h.ns_int
o.cc_vn6:	^.^.par.h.ns_par
o.cc_vn6:	^.ns.ets.net_par
o.cc_vn6:	^.ns.vn6.net_par
o.cc_vn6:	^.ns.ntt.net_par
o.cc_vn6:	h.cc_int
o.cc_vn6:	^.^.par.h.cc_par
o.cc_vn6:	h.acu_int
o.cc_vn6:	^.ns.h.isdn
o.cc_vn6:	^.services.h.ctx_ext
o.cc_vn6:	^.ph.h.ph_int
o.cc_vn6:	^.^.par.h.ph_par
o.cc_vn6:	h.cc_undef
o.cc_vn6:	^.ns.h.ns_mac
o.cc_vn6:	^.ns.vn6.net_mac
o.cc_vn6:	h.cc_ie
o.cc_vn6:	h.cc_fct
o.cc_vn6:	h.cc_build
o.cc_vn6:	vn6.cc_fct
o.cc_vn6:	vn6.cc_proc
o.cc_vn6:	vn6.cc_serv
o.cc_vn6:	vn6.cc_build
