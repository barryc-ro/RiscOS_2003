#
#         Example Makefile for Omnitel Network Signalling  entity
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -Ic: -I^.^.par -I^.services -I^.dl -I^.ph -I^.asn1

LIBFILES = 



# ----------------------------------------------------------------- #
# Various Macros
# ----------------------------------------------------------------- #



CC = cc
NOTMODULE_CFLAGS = $(INCLUDES) -c -Throwback -depend !Depend $(VARIANT_CFLAGS)
MODULE_CFLAGS = $(NOTMODULE_CFLAGS) -zM -ff

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

TARGETS = mns.o\
 ns.o\
 ns_accss.o\
 ns_err.o\
 ns_ie_1.o\
 ns_ie_2.o\
 ns_ie_3.o\
 ns_ie_4.o\
 ns_ie_5.o\
 ns_ie_6.o\
 ns_ie_i.o\
 ns_ie_ss.o\
 ns_rcv.o\
 ns_snd.o\
 ns_stat1.o\
 ns_stat2.o\
 ns_stat3.o\
 ns_switc.o\
 ns_ssdu1.o\
 ns_ssdu3.o\
 ns_ssdu2.o\
 ns_spdu.o\
 ns_proc.o\
 ns_int.o\
 pic_nsie.o\
 pic_ssdu.o

#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.ns:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ -VIA Target 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.mns:	c.mns
o.mns:	h.mns
o.mns:	^.services.h.services
o.mns:	^.^.par.h.serv_par
o.mns:	^.^.par.h.consts
o.mns:	^.^.par.h._product
o.mns:	c:h.string
o.mns:	^.^.par.h.serv_por
o.mns:	c:sys.h.types
o.mns:	c:h.kernel
o.mns:	c:h.swis
o.mns:	h.ns_int
o.mns:	^.^.par.h.ns_par
o.mns:	ets.net_par
o.mns:	vn6.net_par
o.mns:	ntt.net_par
o.mns:	h.isdn
o.mns:	^.dl.h.dl_int
o.mns:	^.^.par.h.dl_par
o.ns:	c.ns
o.ns:	h.ns
o.ns:	^.services.h.services
o.ns:	^.^.par.h.serv_par
o.ns:	^.^.par.h.consts
o.ns:	^.^.par.h._product
o.ns:	c:h.string
o.ns:	^.^.par.h.serv_por
o.ns:	c:sys.h.types
o.ns:	c:h.kernel
o.ns:	c:h.swis
o.ns:	h.ns_int
o.ns:	^.^.par.h.ns_par
o.ns:	ets.net_par
o.ns:	vn6.net_par
o.ns:	ntt.net_par
o.ns:	^.dl.h.dl_int
o.ns:	^.^.par.h.dl_par
o.ns:	h.ns_i_s
o.ns:	h.isdn
o.ns:	ets.net_indx
o.ns:	ets.net_tim
o.ns:	vn6.net_indx
o.ns:	vn6.net_tim
o.ns:	ntt.net_indx
o.ns:	ntt.net_tim
o.ns:	h.ns_fct
o.ns:	ets.net_fct
o.ns:	ntt.net_fct
o.ns:	vn6.net_fct
o.ns_accss:	c.ns_accss
o.ns_accss:	h.ns
o.ns_accss:	^.services.h.services
o.ns_accss:	^.^.par.h.serv_par
o.ns_accss:	^.^.par.h.consts
o.ns_accss:	^.^.par.h._product
o.ns_accss:	c:h.string
o.ns_accss:	^.^.par.h.serv_por
o.ns_accss:	c:sys.h.types
o.ns_accss:	c:h.kernel
o.ns_accss:	c:h.swis
o.ns_accss:	h.ns_int
o.ns_accss:	^.^.par.h.ns_par
o.ns_accss:	ets.net_par
o.ns_accss:	vn6.net_par
o.ns_accss:	ntt.net_par
o.ns_accss:	^.dl.h.dl_int
o.ns_accss:	^.^.par.h.dl_par
o.ns_accss:	h.ns_i_s
o.ns_accss:	h.isdn
o.ns_accss:	^.services.h.ctx_ext
o.ns_err:	c.ns_err
o.ns_err:	h.ns
o.ns_err:	^.services.h.services
o.ns_err:	^.^.par.h.serv_par
o.ns_err:	^.^.par.h.consts
o.ns_err:	^.^.par.h._product
o.ns_err:	c:h.string
o.ns_err:	^.^.par.h.serv_por
o.ns_err:	c:sys.h.types
o.ns_err:	c:h.kernel
o.ns_err:	c:h.swis
o.ns_err:	h.ns_int
o.ns_err:	^.^.par.h.ns_par
o.ns_err:	ets.net_par
o.ns_err:	vn6.net_par
o.ns_err:	ntt.net_par
o.ns_err:	^.dl.h.dl_int
o.ns_err:	^.^.par.h.dl_par
o.ns_err:	h.ns_i_s
o.ns_err:	h.isdn
o.ns_err:	^.services.h.ctx_ext
o.ns_err:	ets.net_err
o.ns_err:	vn6.net_err
o.ns_err:	ntt.net_err
o.ns_ie_1:	c.ns_ie_1
o.ns_ie_1:	h.ns
o.ns_ie_1:	^.services.h.services
o.ns_ie_1:	^.^.par.h.serv_par
o.ns_ie_1:	^.^.par.h.consts
o.ns_ie_1:	^.^.par.h._product
o.ns_ie_1:	c:h.string
o.ns_ie_1:	^.^.par.h.serv_por
o.ns_ie_1:	c:sys.h.types
o.ns_ie_1:	c:h.kernel
o.ns_ie_1:	c:h.swis
o.ns_ie_1:	h.ns_int
o.ns_ie_1:	^.^.par.h.ns_par
o.ns_ie_1:	ets.net_par
o.ns_ie_1:	vn6.net_par
o.ns_ie_1:	ntt.net_par
o.ns_ie_1:	^.dl.h.dl_int
o.ns_ie_1:	^.^.par.h.dl_par
o.ns_ie_1:	h.ns_i_s
o.ns_ie_1:	h.isdn
o.ns_ie_1:	^.services.h.ctx_ext
o.ns_ie_1:	h.ns_ie
o.ns_ie_2:	c.ns_ie_2
o.ns_ie_2:	h.ns
o.ns_ie_2:	^.services.h.services
o.ns_ie_2:	^.^.par.h.serv_par
o.ns_ie_2:	^.^.par.h.consts
o.ns_ie_2:	^.^.par.h._product
o.ns_ie_2:	c:h.string
o.ns_ie_2:	^.^.par.h.serv_por
o.ns_ie_2:	c:sys.h.types
o.ns_ie_2:	c:h.kernel
o.ns_ie_2:	c:h.swis
o.ns_ie_2:	h.ns_int
o.ns_ie_2:	^.^.par.h.ns_par
o.ns_ie_2:	ets.net_par
o.ns_ie_2:	vn6.net_par
o.ns_ie_2:	ntt.net_par
o.ns_ie_2:	^.dl.h.dl_int
o.ns_ie_2:	^.^.par.h.dl_par
o.ns_ie_2:	h.ns_i_s
o.ns_ie_2:	h.isdn
o.ns_ie_2:	^.services.h.ctx_ext
o.ns_ie_2:	h.ns_ie
o.ns_ie_3:	c.ns_ie_3
o.ns_ie_3:	h.ns
o.ns_ie_3:	^.services.h.services
o.ns_ie_3:	^.^.par.h.serv_par
o.ns_ie_3:	^.^.par.h.consts
o.ns_ie_3:	^.^.par.h._product
o.ns_ie_3:	c:h.string
o.ns_ie_3:	^.^.par.h.serv_por
o.ns_ie_3:	c:sys.h.types
o.ns_ie_3:	c:h.kernel
o.ns_ie_3:	c:h.swis
o.ns_ie_3:	h.ns_int
o.ns_ie_3:	^.^.par.h.ns_par
o.ns_ie_3:	ets.net_par
o.ns_ie_3:	vn6.net_par
o.ns_ie_3:	ntt.net_par
o.ns_ie_3:	^.dl.h.dl_int
o.ns_ie_3:	^.^.par.h.dl_par
o.ns_ie_3:	h.ns_i_s
o.ns_ie_3:	h.isdn
o.ns_ie_3:	^.services.h.ctx_ext
o.ns_ie_3:	h.ns_ie
o.ns_ie_4:	c.ns_ie_4
o.ns_ie_4:	h.ns
o.ns_ie_4:	^.services.h.services
o.ns_ie_4:	^.^.par.h.serv_par
o.ns_ie_4:	^.^.par.h.consts
o.ns_ie_4:	^.^.par.h._product
o.ns_ie_4:	c:h.string
o.ns_ie_4:	^.^.par.h.serv_por
o.ns_ie_4:	c:sys.h.types
o.ns_ie_4:	c:h.kernel
o.ns_ie_4:	c:h.swis
o.ns_ie_4:	h.ns_int
o.ns_ie_4:	^.^.par.h.ns_par
o.ns_ie_4:	ets.net_par
o.ns_ie_4:	vn6.net_par
o.ns_ie_4:	ntt.net_par
o.ns_ie_4:	^.dl.h.dl_int
o.ns_ie_4:	^.^.par.h.dl_par
o.ns_ie_4:	h.ns_i_s
o.ns_ie_4:	h.isdn
o.ns_ie_4:	^.services.h.ctx_ext
o.ns_ie_4:	h.ns_ie
o.ns_ie_4:	ntt.net_indx
o.ns_ie_4:	ntt.net_ie
o.ns_ie_4:	ntt.mp_ie
o.ns_ie_4:	ntt.tab_ie
o.ns_ie_4:	ntt.tab_ie
o.ns_ie_5:	c.ns_ie_5
o.ns_ie_5:	h.ns
o.ns_ie_5:	^.services.h.services
o.ns_ie_5:	^.^.par.h.serv_par
o.ns_ie_5:	^.^.par.h.consts
o.ns_ie_5:	^.^.par.h._product
o.ns_ie_5:	c:h.string
o.ns_ie_5:	^.^.par.h.serv_por
o.ns_ie_5:	c:sys.h.types
o.ns_ie_5:	c:h.kernel
o.ns_ie_5:	c:h.swis
o.ns_ie_5:	h.ns_int
o.ns_ie_5:	^.^.par.h.ns_par
o.ns_ie_5:	ets.net_par
o.ns_ie_5:	vn6.net_par
o.ns_ie_5:	ntt.net_par
o.ns_ie_5:	^.dl.h.dl_int
o.ns_ie_5:	^.^.par.h.dl_par
o.ns_ie_5:	h.ns_i_s
o.ns_ie_5:	h.isdn
o.ns_ie_5:	^.services.h.ctx_ext
o.ns_ie_5:	h.ns_ie
o.ns_ie_6:	c.ns_ie_6
o.ns_ie_6:	h.ns
o.ns_ie_6:	^.services.h.services
o.ns_ie_6:	^.^.par.h.serv_par
o.ns_ie_6:	^.^.par.h.consts
o.ns_ie_6:	^.^.par.h._product
o.ns_ie_6:	c:h.string
o.ns_ie_6:	^.^.par.h.serv_por
o.ns_ie_6:	c:sys.h.types
o.ns_ie_6:	c:h.kernel
o.ns_ie_6:	c:h.swis
o.ns_ie_6:	h.ns_int
o.ns_ie_6:	^.^.par.h.ns_par
o.ns_ie_6:	ets.net_par
o.ns_ie_6:	vn6.net_par
o.ns_ie_6:	ntt.net_par
o.ns_ie_6:	^.dl.h.dl_int
o.ns_ie_6:	^.^.par.h.dl_par
o.ns_ie_6:	h.ns_i_s
o.ns_ie_6:	h.isdn
o.ns_ie_6:	^.services.h.ctx_ext
o.ns_ie_6:	h.ns_ie
o.ns_ie_6:	vn6.net_indx
o.ns_ie_6:	vn6.net_ie
o.ns_ie_6:	vn6.mp_ie
o.ns_ie_6:	vn6.tab_ie
o.ns_ie_6:	vn6.tab_ie
o.ns_ie_6:	ets.net_indx
o.ns_ie_6:	ets.net_ie
o.ns_ie_6:	ets.mp_ie
o.ns_ie_6:	ets.tab_ie
o.ns_ie_6:	ets.tab_ie
o.ns_ie_i:	c.ns_ie_i
o.ns_ie_i:	h.ns
o.ns_ie_i:	^.services.h.services
o.ns_ie_i:	^.^.par.h.serv_par
o.ns_ie_i:	^.^.par.h.consts
o.ns_ie_i:	^.^.par.h._product
o.ns_ie_i:	c:h.string
o.ns_ie_i:	^.^.par.h.serv_por
o.ns_ie_i:	c:sys.h.types
o.ns_ie_i:	c:h.kernel
o.ns_ie_i:	c:h.swis
o.ns_ie_i:	h.ns_int
o.ns_ie_i:	^.^.par.h.ns_par
o.ns_ie_i:	ets.net_par
o.ns_ie_i:	vn6.net_par
o.ns_ie_i:	ntt.net_par
o.ns_ie_i:	^.dl.h.dl_int
o.ns_ie_i:	^.^.par.h.dl_par
o.ns_ie_i:	h.ns_i_s
o.ns_ie_i:	h.isdn
o.ns_ie_i:	h.ns_ie_i
o.ns_ie_ss:	c.ns_ie_ss
o.ns_ie_ss:	h.ns
o.ns_ie_ss:	^.services.h.services
o.ns_ie_ss:	^.^.par.h.serv_par
o.ns_ie_ss:	^.^.par.h.consts
o.ns_ie_ss:	^.^.par.h._product
o.ns_ie_ss:	c:h.string
o.ns_ie_ss:	^.^.par.h.serv_por
o.ns_ie_ss:	c:sys.h.types
o.ns_ie_ss:	c:h.kernel
o.ns_ie_ss:	c:h.swis
o.ns_ie_ss:	h.ns_int
o.ns_ie_ss:	^.^.par.h.ns_par
o.ns_ie_ss:	ets.net_par
o.ns_ie_ss:	vn6.net_par
o.ns_ie_ss:	ntt.net_par
o.ns_ie_ss:	^.dl.h.dl_int
o.ns_ie_ss:	^.^.par.h.dl_par
o.ns_ie_ss:	h.ns_i_s
o.ns_rcv:	c.ns_rcv
o.ns_rcv:	h.ns
o.ns_rcv:	^.services.h.services
o.ns_rcv:	^.^.par.h.serv_par
o.ns_rcv:	^.^.par.h.consts
o.ns_rcv:	^.^.par.h._product
o.ns_rcv:	c:h.string
o.ns_rcv:	^.^.par.h.serv_por
o.ns_rcv:	c:sys.h.types
o.ns_rcv:	c:h.kernel
o.ns_rcv:	c:h.swis
o.ns_rcv:	h.ns_int
o.ns_rcv:	^.^.par.h.ns_par
o.ns_rcv:	ets.net_par
o.ns_rcv:	vn6.net_par
o.ns_rcv:	ntt.net_par
o.ns_rcv:	^.dl.h.dl_int
o.ns_rcv:	^.^.par.h.dl_par
o.ns_rcv:	h.ns_i_s
o.ns_rcv:	h.isdn
o.ns_rcv:	^.services.h.ctx_ext
o.ns_snd:	c.ns_snd
o.ns_snd:	h.ns
o.ns_snd:	^.services.h.services
o.ns_snd:	^.^.par.h.serv_par
o.ns_snd:	^.^.par.h.consts
o.ns_snd:	^.^.par.h._product
o.ns_snd:	c:h.string
o.ns_snd:	^.^.par.h.serv_por
o.ns_snd:	c:sys.h.types
o.ns_snd:	c:h.kernel
o.ns_snd:	c:h.swis
o.ns_snd:	h.ns_int
o.ns_snd:	^.^.par.h.ns_par
o.ns_snd:	ets.net_par
o.ns_snd:	vn6.net_par
o.ns_snd:	ntt.net_par
o.ns_snd:	^.dl.h.dl_int
o.ns_snd:	^.^.par.h.dl_par
o.ns_snd:	h.ns_i_s
o.ns_snd:	h.isdn
o.ns_snd:	^.services.h.ctx_ext
o.ns_stat1:	c.ns_stat1
o.ns_stat1:	h.ns
o.ns_stat1:	^.services.h.services
o.ns_stat1:	^.^.par.h.serv_par
o.ns_stat1:	^.^.par.h.consts
o.ns_stat1:	^.^.par.h._product
o.ns_stat1:	c:h.string
o.ns_stat1:	^.^.par.h.serv_por
o.ns_stat1:	c:sys.h.types
o.ns_stat1:	c:h.kernel
o.ns_stat1:	c:h.swis
o.ns_stat1:	h.ns_int
o.ns_stat1:	^.^.par.h.ns_par
o.ns_stat1:	ets.net_par
o.ns_stat1:	vn6.net_par
o.ns_stat1:	ntt.net_par
o.ns_stat1:	^.dl.h.dl_int
o.ns_stat1:	^.^.par.h.dl_par
o.ns_stat1:	h.ns_i_s
o.ns_stat1:	h.isdn
o.ns_stat1:	^.services.h.ctx_ext
o.ns_stat2:	c.ns_stat2
o.ns_stat2:	h.ns
o.ns_stat2:	^.services.h.services
o.ns_stat2:	^.^.par.h.serv_par
o.ns_stat2:	^.^.par.h.consts
o.ns_stat2:	^.^.par.h._product
o.ns_stat2:	c:h.string
o.ns_stat2:	^.^.par.h.serv_por
o.ns_stat2:	c:sys.h.types
o.ns_stat2:	c:h.kernel
o.ns_stat2:	c:h.swis
o.ns_stat2:	h.ns_int
o.ns_stat2:	^.^.par.h.ns_par
o.ns_stat2:	ets.net_par
o.ns_stat2:	vn6.net_par
o.ns_stat2:	ntt.net_par
o.ns_stat2:	^.dl.h.dl_int
o.ns_stat2:	^.^.par.h.dl_par
o.ns_stat2:	h.ns_i_s
o.ns_stat2:	h.isdn
o.ns_stat2:	^.services.h.ctx_ext
o.ns_stat2:	ets.net_indx
o.ns_stat2:	ets.net_stat
o.ns_stat2:	vn6.net_indx
o.ns_stat2:	vn6.net_stat
o.ns_stat3:	c.ns_stat3
o.ns_stat3:	h.ns
o.ns_stat3:	^.services.h.services
o.ns_stat3:	^.^.par.h.serv_par
o.ns_stat3:	^.^.par.h.consts
o.ns_stat3:	^.^.par.h._product
o.ns_stat3:	c:h.string
o.ns_stat3:	^.^.par.h.serv_por
o.ns_stat3:	c:sys.h.types
o.ns_stat3:	c:h.kernel
o.ns_stat3:	c:h.swis
o.ns_stat3:	h.ns_int
o.ns_stat3:	^.^.par.h.ns_par
o.ns_stat3:	ets.net_par
o.ns_stat3:	vn6.net_par
o.ns_stat3:	ntt.net_par
o.ns_stat3:	^.dl.h.dl_int
o.ns_stat3:	^.^.par.h.dl_par
o.ns_stat3:	h.ns_i_s
o.ns_stat3:	h.isdn
o.ns_stat3:	^.services.h.ctx_ext
o.ns_stat3:	ntt.net_indx
o.ns_stat3:	ntt.net_stat
o.ns_switc:	c.ns_switc
o.ns_switc:	h.ns
o.ns_switc:	^.services.h.services
o.ns_switc:	^.^.par.h.serv_par
o.ns_switc:	^.^.par.h.consts
o.ns_switc:	^.^.par.h._product
o.ns_switc:	c:h.string
o.ns_switc:	^.^.par.h.serv_por
o.ns_switc:	c:sys.h.types
o.ns_switc:	c:h.kernel
o.ns_switc:	c:h.swis
o.ns_switc:	h.ns_int
o.ns_switc:	^.^.par.h.ns_par
o.ns_switc:	ets.net_par
o.ns_switc:	vn6.net_par
o.ns_switc:	ntt.net_par
o.ns_switc:	^.dl.h.dl_int
o.ns_switc:	^.^.par.h.dl_par
o.ns_switc:	h.ns_i_s
o.ns_switc:	h.isdn
o.ns_switc:	^.services.h.ctx_ext
o.ns_switc:	ets.net_indx
o.ns_switc:	ets.net_swit
o.ns_switc:	vn6.net_indx
o.ns_switc:	vn6.net_swit
o.ns_switc:	ntt.net_indx
o.ns_switc:	ntt.net_swit
o.ns_ssdu1:	c.ns_ssdu1
o.ns_ssdu1:	h.ns
o.ns_ssdu1:	^.services.h.services
o.ns_ssdu1:	^.^.par.h.serv_par
o.ns_ssdu1:	^.^.par.h.consts
o.ns_ssdu1:	^.^.par.h._product
o.ns_ssdu1:	c:h.string
o.ns_ssdu1:	^.^.par.h.serv_por
o.ns_ssdu1:	c:sys.h.types
o.ns_ssdu1:	c:h.kernel
o.ns_ssdu1:	c:h.swis
o.ns_ssdu1:	h.ns_int
o.ns_ssdu1:	^.^.par.h.ns_par
o.ns_ssdu1:	ets.net_par
o.ns_ssdu1:	vn6.net_par
o.ns_ssdu1:	ntt.net_par
o.ns_ssdu1:	^.dl.h.dl_int
o.ns_ssdu1:	^.^.par.h.dl_par
o.ns_ssdu1:	h.ns_i_s
o.ns_ssdu1:	h.isdn
o.ns_ssdu1:	^.services.h.ctx_ext
o.ns_ssdu1:	h.ssdu_ml
o.ns_ssdu1:	ets.net_indx
o.ns_ssdu1:	ets.net_ssdu
o.ns_ssdu1:	ets.mp_ssdu
o.ns_ssdu1:	ets.tab_ssdu
o.ns_ssdu1:	ets.tab_ssdu
o.ns_ssdu3:	c.ns_ssdu3
o.ns_ssdu3:	h.ns
o.ns_ssdu3:	^.services.h.services
o.ns_ssdu3:	^.^.par.h.serv_par
o.ns_ssdu3:	^.^.par.h.consts
o.ns_ssdu3:	^.^.par.h._product
o.ns_ssdu3:	c:h.string
o.ns_ssdu3:	^.^.par.h.serv_por
o.ns_ssdu3:	c:sys.h.types
o.ns_ssdu3:	c:h.kernel
o.ns_ssdu3:	c:h.swis
o.ns_ssdu3:	h.ns_int
o.ns_ssdu3:	^.^.par.h.ns_par
o.ns_ssdu3:	ets.net_par
o.ns_ssdu3:	vn6.net_par
o.ns_ssdu3:	ntt.net_par
o.ns_ssdu3:	^.dl.h.dl_int
o.ns_ssdu3:	^.^.par.h.dl_par
o.ns_ssdu3:	h.ns_i_s
o.ns_ssdu3:	h.isdn
o.ns_ssdu3:	^.services.h.ctx_ext
o.ns_ssdu3:	h.ssdu_ml
o.ns_ssdu2:	c.ns_ssdu2
o.ns_ssdu2:	h.ns
o.ns_ssdu2:	^.services.h.services
o.ns_ssdu2:	^.^.par.h.serv_par
o.ns_ssdu2:	^.^.par.h.consts
o.ns_ssdu2:	^.^.par.h._product
o.ns_ssdu2:	c:h.string
o.ns_ssdu2:	^.^.par.h.serv_por
o.ns_ssdu2:	c:sys.h.types
o.ns_ssdu2:	c:h.kernel
o.ns_ssdu2:	c:h.swis
o.ns_ssdu2:	h.ns_int
o.ns_ssdu2:	^.^.par.h.ns_par
o.ns_ssdu2:	ets.net_par
o.ns_ssdu2:	vn6.net_par
o.ns_ssdu2:	ntt.net_par
o.ns_ssdu2:	^.dl.h.dl_int
o.ns_ssdu2:	^.^.par.h.dl_par
o.ns_ssdu2:	h.ns_i_s
o.ns_ssdu2:	h.isdn
o.ns_ssdu2:	^.services.h.ctx_ext
o.ns_ssdu2:	h.ns_mac
o.ns_ssdu2:	vn6.net_mac
o.ns_ssdu2:	ntt.net_mac
o.ns_ssdu2:	h.ssdu_ml
o.ns_ssdu2:	vn6.net_indx
o.ns_ssdu2:	vn6.net_ssdu
o.ns_ssdu2:	vn6.mp_ssdu
o.ns_ssdu2:	vn6.tab_ssdu
o.ns_ssdu2:	vn6.tab_ssdu
o.ns_ssdu2:	ntt.net_indx
o.ns_ssdu2:	ntt.net_ssdu
o.ns_ssdu2:	ntt.mp_ssdu
o.ns_ssdu2:	ntt.tab_ssdu
o.ns_ssdu2:	ntt.tab_ssdu
o.ns_spdu:	c.ns_spdu
o.ns_spdu:	h.ns
o.ns_spdu:	^.services.h.services
o.ns_spdu:	^.^.par.h.serv_par
o.ns_spdu:	^.^.par.h.consts
o.ns_spdu:	^.^.par.h._product
o.ns_spdu:	c:h.string
o.ns_spdu:	^.^.par.h.serv_por
o.ns_spdu:	c:sys.h.types
o.ns_spdu:	c:h.kernel
o.ns_spdu:	c:h.swis
o.ns_spdu:	h.ns_int
o.ns_spdu:	^.^.par.h.ns_par
o.ns_spdu:	ets.net_par
o.ns_spdu:	vn6.net_par
o.ns_spdu:	ntt.net_par
o.ns_spdu:	^.dl.h.dl_int
o.ns_spdu:	^.^.par.h.dl_par
o.ns_spdu:	h.ns_i_s
o.ns_spdu:	h.isdn
o.ns_spdu:	^.services.h.ctx_ext
o.ns_spdu:	ets.net_spdu
o.ns_spdu:	ets.tab_spdu
o.ns_spdu:	ets.tab_spdu
o.ns_spdu:	vn6.net_spdu
o.ns_spdu:	vn6.tab_spdu
o.ns_spdu:	vn6.tab_spdu
o.ns_spdu:	ntt.net_spdu
o.ns_spdu:	ntt.tab_spdu
o.ns_spdu:	ntt.tab_spdu
o.ns_proc:	c.ns_proc
o.ns_proc:	h.ns
o.ns_proc:	^.services.h.services
o.ns_proc:	^.^.par.h.serv_par
o.ns_proc:	^.^.par.h.consts
o.ns_proc:	^.^.par.h._product
o.ns_proc:	c:h.string
o.ns_proc:	^.^.par.h.serv_por
o.ns_proc:	c:sys.h.types
o.ns_proc:	c:h.kernel
o.ns_proc:	c:h.swis
o.ns_proc:	h.ns_int
o.ns_proc:	^.^.par.h.ns_par
o.ns_proc:	ets.net_par
o.ns_proc:	vn6.net_par
o.ns_proc:	ntt.net_par
o.ns_proc:	^.dl.h.dl_int
o.ns_proc:	^.^.par.h.dl_par
o.ns_proc:	h.ns_i_s
o.ns_proc:	h.isdn
o.ns_proc:	^.services.h.ctx_ext
o.ns_int:	c.ns_int
o.ns_int:	^.services.h.services
o.ns_int:	^.^.par.h.serv_par
o.ns_int:	^.^.par.h.consts
o.ns_int:	^.^.par.h._product
o.ns_int:	c:h.string
o.ns_int:	^.^.par.h.serv_por
o.ns_int:	c:sys.h.types
o.ns_int:	c:h.kernel
o.ns_int:	c:h.swis
o.ns_int:	h.ns_int
o.ns_int:	^.^.par.h.ns_par
o.ns_int:	ets.net_par
o.ns_int:	vn6.net_par
o.ns_int:	ntt.net_par
o.ns_int:	h.ns_mac
o.ns_int:	ets.net_mac
o.ns_int:	vn6.net_mac
o.ns_int:	ntt.net_mac
o.ns_int:	h.isdn
o.ns_int:	^.services.h.ctx_ext
o.pic_nsie:	c.pic_nsie
o.pic_nsie:	h.ns
o.pic_nsie:	^.services.h.services
o.pic_nsie:	^.^.par.h.serv_par
o.pic_nsie:	^.^.par.h.consts
o.pic_nsie:	^.^.par.h._product
o.pic_nsie:	c:h.string
o.pic_nsie:	^.^.par.h.serv_por
o.pic_nsie:	c:sys.h.types
o.pic_nsie:	c:h.kernel
o.pic_nsie:	c:h.swis
o.pic_nsie:	h.ns_int
o.pic_nsie:	^.^.par.h.ns_par
o.pic_nsie:	ets.net_par
o.pic_nsie:	vn6.net_par
o.pic_nsie:	ntt.net_par
o.pic_nsie:	^.dl.h.dl_int
o.pic_nsie:	^.^.par.h.dl_par
o.pic_nsie:	h.ns_i_s
o.pic_nsie:	h.ns_ie
o.pic_nsie:	h.pic_nsie
o.pic_nsie:	ets.net_indx
o.pic_nsie:	ets.mp_ie
o.pic_nsie:	vn6.net_indx
o.pic_nsie:	vn6.mp_ie
o.pic_nsie:	ntt.net_indx
o.pic_nsie:	ntt.mp_ie
o.pic_nsie:	ets.mp_ie
o.pic_nsie:	vn6.mp_ie
o.pic_nsie:	ntt.mp_ie
o.pic_ssdu:	c.pic_ssdu
o.pic_ssdu:	h.ns
o.pic_ssdu:	^.services.h.services
o.pic_ssdu:	^.^.par.h.serv_par
o.pic_ssdu:	^.^.par.h.consts
o.pic_ssdu:	^.^.par.h._product
o.pic_ssdu:	c:h.string
o.pic_ssdu:	^.^.par.h.serv_por
o.pic_ssdu:	c:sys.h.types
o.pic_ssdu:	c:h.kernel
o.pic_ssdu:	c:h.swis
o.pic_ssdu:	h.ns_int
o.pic_ssdu:	^.^.par.h.ns_par
o.pic_ssdu:	ets.net_par
o.pic_ssdu:	vn6.net_par
o.pic_ssdu:	ntt.net_par
o.pic_ssdu:	^.dl.h.dl_int
o.pic_ssdu:	^.^.par.h.dl_par
o.pic_ssdu:	h.ns_i_s
o.pic_ssdu:	h.ssdu_ml
o.pic_ssdu:	h.pic_ssdu
o.pic_ssdu:	ets.net_indx
o.pic_ssdu:	ets.mp_ssdu
o.pic_ssdu:	vn6.net_indx
o.pic_ssdu:	vn6.mp_ssdu
o.pic_ssdu:	ntt.net_indx
o.pic_ssdu:	ntt.mp_ssdu
o.pic_ssdu:	ets.mp_ssdu
o.pic_ssdu:	vn6.mp_ssdu
o.pic_ssdu:	ntt.mp_ssdu
