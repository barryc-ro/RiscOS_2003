#
#         Example Makefile for Omnitel Services entity
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -Ic: -I^.sources.init -I^.device -I^.sources.cc -I^.sources.mppp -I^.sources.ph -I^.par -I^.sources.services 

LIBFILES = 



# ----------------------------------------------------------------- #
# Various Macros
# ----------------------------------------------------------------- #



CC = cc
NOTMODULE_CFLAGS = $(INCLUDES) -c -Throwback -depend !Depend $(VARIANT_CFLAGS)
MODULE_CFLAGS = $(NOTMODULE_CFLAGS) -zM 

LINK = link
NOTMODULE_LINKFLAGS = -AOF 
MODULE_LINKFLAGS = $(NOTMODULE_LINKFLAGS) -module 

CMHG = cmhg
CMHGFLAGS =

OBJASM = objasm 

AR = libfile
AR_FLAGS = -c 

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

TARGETS = module.o \
    	  header.o \
    	  pod_irq.o
    	  

^.o.module:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 

module.o:	module.c
		$(CC) $(MODULE_CFLAGS) module.c
		
header.o:	cmhg.header
		$(CMHG) $(CMHG_FLAGS) cmhg.header -o o.header
	


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.module:	c.module
o.module:	c:h.kernel
o.module:	c:h.swis
o.module:	h.module
o.module:	^.device.h.device
o.module:	^.device.h.isdnctrl
o.module:	^.sources.services.h.services
o.module:	^.par.h.serv_par
o.module:	^.par.h.consts
o.module:	^.par.h._product
o.module:	^.par.h.serv_por
o.module:	c:sys.h.types
o.module:	c:h.kernel
o.module:	c:h.swis
o.module:	^.sources.init.h.ob_int
o.module:	^.sources.init.h.ob_par
o.module:	^.sources.cc.h.cc_int
o.module:	^.par.h.cc_par
o.module:	^.sources.cc.h.acu_int
o.module:	^.par.h.ap_par
o.module:	^.par.h.ph_d_par
o.module:	^.sources.ph.h.ph_int
o.module:	^.par.h.ph_par
o.module:	^.device.h.dd_int
o.module:	^.sources.mppp.h.mppp_int
o.module:	^.par.h.mppp_par
