#
#          Makefile for OS & Omnitel Init functions
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -I^.^.par -Ic: -I^.services -I^.^.device -I^.mppp  -I^.cc -I^.ns -I^.dl -I^.v120 -I^.ph -I^.ph_d 

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
.c.o:;		$(CC) $(MODULE_CFLAGS) $(UNIX_CFLAGS) $<
.s.o:;		$(OBJASM) -from $< -to $@

# ----------------------------------------------------------------- #
# Targets to be compiled
# ----------------------------------------------------------------- #

TARGETS = ob_init.o \
 os_acces.o 


#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.init:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.ob_init:	c.ob_init
o.ob_init:	^.services.h.services
o.ob_init:	^.^.par.h.serv_par
o.ob_init:	^.^.par.h.consts
o.ob_init:	^.^.par.h._product
o.ob_init:	c:h.string
o.ob_init:	^.^.par.h.serv_por
o.ob_init:	c:sys.h.types
o.ob_init:	c:h.kernel
o.ob_init:	c:h.swis
o.ob_init:	h.ob_int
o.ob_init:	h.ob_par
o.ob_init:	^.services.h.it
o.ob_init:	^.^.par.h.it_par
o.ob_init:	^.^.device.h.dd_int
o.ob_init:	^.ph_d.h.ph_d_int
o.ob_init:	^.^.par.h.ph_d_par
o.ob_init:	^.ph.h.ph_int
o.ob_init:	^.^.par.h.ph_par
o.ob_init:	^.services.h.it
o.ob_init:	^.^.par.h.conf_ob
o.ob_init:	^.services.h.services
o.ob_init:	^.ph.h.ph_int
o.ob_init:	^.dl.h.dl_int
o.ob_init:	^.^.par.h.dl_par
o.ob_init:	^.v120.h.vs_int
o.ob_init:	^.v120.h.vs_unit
o.ob_init:	^.^.par.h.v120_par
o.ob_init:	^.dl.h.v120_int
o.ob_init:	^.ns.h.ns_int
o.ob_init:	^.^.par.h.ns_par
o.ob_init:	^.ns.ets.net_par
o.ob_init:	^.ns.vn6.net_par
o.ob_init:	^.ns.ntt.net_par
o.ob_init:	^.cc.h.cc_int
o.ob_init:	^.^.par.h.cc_par
o.ob_init:	^.mppp.h.mppp_int
o.ob_init:	^.^.par.h.mppp_par
o.ob_init:	c:h.string
o.os_acces:	c.os_acces
o.os_acces:	^.services.h.services
o.os_acces:	^.^.par.h.serv_par
o.os_acces:	^.^.par.h.consts
o.os_acces:	^.^.par.h._product
o.os_acces:	c:h.string
o.os_acces:	^.^.par.h.serv_por
o.os_acces:	c:sys.h.types
o.os_acces:	c:h.kernel
o.os_acces:	c:h.swis
o.os_acces:	^.^.par.h.conf_srv
o.os_acces:	^.services.h.ctx_ext
o.os_acces:	h.ob_int
o.os_acces:	h.ob_par
o.os_acces:	c:h.string
o.os_acces:	c:h.kernel
o.os_acces:	c:h.swis
