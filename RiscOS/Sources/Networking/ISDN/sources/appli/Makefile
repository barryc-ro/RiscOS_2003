#
#          Makefile for TEST APPLICATION 
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -I^.^.par -I^.^.device -Ic: -I^.services -I^.cc -I^.mppp -I^.ns -I^.dte -I^.pdte -I^.dl -I^.v120 -I^.ph -I^.ph_d 

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

TARGETS = appli.o\
 appli_pp.o\
 user.o

#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.appli:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.appli:	c.appli
o.appli:	^.services.h.services
o.appli:	^.^.par.h.serv_par
o.appli:	^.^.par.h.consts
o.appli:	^.^.par.h._product
o.appli:	c:h.string
o.appli:	^.^.par.h.serv_por
o.appli:	c:sys.h.types
o.appli:	c:h.kernel
o.appli:	c:h.swis
o.appli:	^.cc.h.cc_int
o.appli:	^.^.par.h.cc_par
o.appli:	^.cc.h.acu_int
o.appli:	^.ns.h.isdn
o.appli:	^.v120.h.vs_int
o.appli:	^.v120.h.vs_unit
o.appli:	^.^.par.h.v120_par
o.appli:	^.dl.h.v120_int
o.appli:	h.ap_par
o.appli:	^.ph_d.h.ph_d_int
o.appli:	^.^.par.h.ph_d_par
o.appli:	^.ph.h.ph_int
o.appli:	^.^.par.h.ph_par
o.appli:	^.services.h.it
o.appli:	^.^.par.h.it_par
o.appli:	^.^.device.h.dd_int
o.appli:	^.mppp.h.mppp_int
o.appli:	^.^.par.h.mppp_par
o.appli:	c:h.stdio
o.appli:	c:h.kernel
o.appli:	c:h.swis
o.appli_pp:	c.appli_pp
o.appli_pp:	^.services.h.services
o.appli_pp:	^.^.par.h.serv_par
o.appli_pp:	^.^.par.h.consts
o.appli_pp:	^.^.par.h._product
o.appli_pp:	c:h.string
o.appli_pp:	^.^.par.h.serv_por
o.appli_pp:	c:sys.h.types
o.appli_pp:	c:h.kernel
o.appli_pp:	c:h.swis
o.appli_pp:	h.ap_par
o.appli_pp:	^.dte.h.dte
o.appli_pp:	^.services.h.services
o.appli_pp:	^.dte.h.dte_int
o.appli_pp:	^.^.par.h.dte_par
o.appli_pp:	^.pdte.h.pdte_int
o.appli_pp:	^.^.par.h.dte_par
o.appli_pp:	^.^.par.h.pdte_par
o.appli_pp:	^.^.par.h.dte_par
o.appli_pp:	^.dte.h.dte_ppp
o.appli_pp:	^.dte.h.dte_ch
o.appli_pp:	^.^.device.h.dd_int
o.appli_pp:	^.mppp.h.mppp_int
o.appli_pp:	^.^.par.h.mppp_par
o.user:	c.user
o.user:	^.services.h.services
o.user:	^.^.par.h.serv_par
o.user:	^.^.par.h.consts
o.user:	^.^.par.h._product
o.user:	c:h.string
o.user:	^.^.par.h.serv_por
o.user:	c:sys.h.types
o.user:	c:h.kernel
o.user:	c:h.swis
o.user:	^.cc.h.cc_int
o.user:	^.^.par.h.cc_par
o.user:	^.cc.h.acu_int
o.user:	^.v120.h.vs_int
o.user:	^.v120.h.vs_unit
o.user:	^.^.par.h.v120_par
o.user:	^.dl.h.v120_int
o.user:	^.ph.h.ph_int
o.user:	^.^.par.h.ph_par
o.user:	^.mppp.h.mppp_int
o.user:	^.^.par.h.mppp_par
o.user:	h.ap_par
o.user:	c:h.stdlib
