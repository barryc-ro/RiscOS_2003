#
#         Example Makefile for Omnitel Parameters and System Managememt entity
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #


INCLUDES = -I^.sources.services -Ic: -I^.sources.cc -I^.sources.ns -I^.sources.dl -I^.sources.v120 -I^.sources.ph -I^.sources.ph_b -I^.sources.ph_d  -I^.sources.mppp

LIBFILES = 



# ----------------------------------------------------------------- #
# Various Macros
# ----------------------------------------------------------------- #



CC = cc
NOTMODULE_CFLAGS = $(INCLUDES) -c -Throwback -depend !Depend $(VARIANT_CFLAGS)
MODULE_CFLAGS = $(NOTMODULE_CFLAGS) -zM 

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

TARGETS = sm.o	\
    	  cfg_ph_b.o \
    	  cfg_ph_d.o 

#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.o.par:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.sm:	c.sm
o.sm:	^.sources.services.h.sm
o.sm:	^.sources.services.h.services
o.sm:	h.serv_par
o.sm:	h.consts
o.sm:	h._product
o.sm:	c:h.string
o.sm:	h.serv_por
o.sm:	c:sys.h.types
o.sm:	c:h.kernel
o.sm:	c:h.swis
o.sm:	h.sm_par
o.sm:	h.config
o.sm:	h.conf_srv
o.sm:	^.sources.cc.h.cc_int
o.sm:	h.cc_par
o.sm:	^.sources.cc.h.acu_int
o.sm:	h.ap_par
o.sm:	h.conf_cc
o.sm:	h.upd_cc
o.sm:	^.sources.ns.h.ns_int
o.sm:	h.ns_par
o.sm:	^.sources.ns.ets.net_par
o.sm:	^.sources.ns.vn6.net_par
o.sm:	^.sources.ns.ntt.net_par
o.sm:	h.conf_ns
o.sm:	h.upd_ns
o.sm:	h.conf_vs
o.sm:	^.sources.v120.h.vs_unit
o.sm:	h.v120_par
o.sm:	^.sources.dl.h.v120_int
o.sm:	^.sources.dl.h.dl_int
o.sm:	h.dl_par
o.sm:	h.v120_par
o.sm:	h.conf_dl
o.sm:	^.sources.ph_b.h.ph_b_int
o.sm:	h.ph_b_par
o.sm:	^.sources.services.h.it
o.sm:	h.it_par
o.sm:	^.sources.ph_d.h.ph_d_int
o.sm:	h.ph_d_par
o.sm:	^.sources.ph.h.ph_int
o.sm:	h.ph_par
o.sm:	^.sources.services.h.it
o.sm:	h.conf_it
o.sm:	^.sources.mppp.h.mppp_int
o.sm:	h.mppp_par
o.sm:	^.sources.ph.h.ph_int
o.sm:	h.conf_ppp
o.sm:	h.upd_cc
o.sm:	h.upd_ns
o.sm:	h.upd_ph_d
o.cfg_ph_b:	c.cfg_ph_b
o.cfg_ph_b:	^.sources.services.h.services
o.cfg_ph_b:	h.serv_par
o.cfg_ph_b:	h.consts
o.cfg_ph_b:	h._product
o.cfg_ph_b:	c:h.string
o.cfg_ph_b:	h.serv_por
o.cfg_ph_b:	c:sys.h.types
o.cfg_ph_b:	c:h.kernel
o.cfg_ph_b:	c:h.swis
o.cfg_ph_b:	^.sources.ph.h.ph_int
o.cfg_ph_b:	h.ph_par
o.cfg_ph_b:	^.sources.ph_b.h.ph_b_int
o.cfg_ph_b:	h.ph_b_par
o.cfg_ph_b:	^.sources.services.h.it
o.cfg_ph_b:	h.it_par
o.cfg_ph_d:	c.cfg_ph_d
o.cfg_ph_d:	^.sources.services.h.services
o.cfg_ph_d:	h.serv_par
o.cfg_ph_d:	h.consts
o.cfg_ph_d:	h._product
o.cfg_ph_d:	c:h.string
o.cfg_ph_d:	h.serv_por
o.cfg_ph_d:	c:sys.h.types
o.cfg_ph_d:	c:h.kernel
o.cfg_ph_d:	c:h.swis
o.cfg_ph_d:	h.conf_srv
o.cfg_ph_d:	^.sources.ph_d.h.ph_d_int
o.cfg_ph_d:	h.ph_d_par
o.cfg_ph_d:	^.sources.ph.h.ph_int
o.cfg_ph_d:	h.ph_par
o.cfg_ph_d:	^.sources.services.h.it
o.cfg_ph_d:	h.it_par
o.cfg_ph_d:	h.upd_ph_d
