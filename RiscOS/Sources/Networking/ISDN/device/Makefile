#
#         Example Makefile for Omnitel Parameters and System Managememt entity
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #


INCLUDES = -I^.par -I^.sources.services -I^.sources.ns -Ic: -I^.sources.ph -I^.sources.cc -I^.sources.appli  -I^.sources.V120 -I^.sources.dl -I^.sources.init

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

TARGETS = device.o \
    	  handler.o \
    	 driver.o \
    	 isdnctrl.o \
    	 macros.o

#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.o.device:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.device:	c.device
o.device:	c:h.kernel
o.device:	c:h.swis
o.device:	h.device
o.device:	h.deviceFS
o.device:	h.handler
o.device:	h.isdnctrl
o.handler:	c.handler
o.handler:	c:h.kernel
o.handler:	c:h.swis
o.handler:	h.testmode
o.handler:	^.sources.services.h.services
o.handler:	^.par.h.serv_par
o.handler:	^.par.h.consts
o.handler:	^.par.h._product
o.handler:	^.par.h.serv_por
o.handler:	c:sys.h.types
o.handler:	c:h.kernel
o.handler:	c:h.swis
o.handler:	^.par.h.conf_srv
o.handler:	^.par.h.ap_par
o.handler:	^.sources.ph.h.ph_int
o.handler:	^.par.h.ph_par
o.handler:	^.sources.services.h.it
o.handler:	^.par.h.it_par
o.handler:	h.dd_int
o.handler:	^.sources.init.h.ob_int
o.handler:	^.sources.init.h.ob_par
o.handler:	h.deviceFS
o.handler:	h.device
o.handler:	h.handler
o.handler:	h.driver
o.handler:	h.isdnctrl
o.isdnctrl:	c.isdnctrl
o.isdnctrl:	c:h.kernel
o.isdnctrl:	c:h.swis
o.isdnctrl:	h.testmode
o.isdnctrl:	^.sources.services.h.services
o.isdnctrl:	^.par.h.serv_par
o.isdnctrl:	^.par.h.consts
o.isdnctrl:	^.par.h._product
o.isdnctrl:	^.par.h.serv_por
o.isdnctrl:	c:sys.h.types
o.isdnctrl:	c:h.kernel
o.isdnctrl:	c:h.swis
o.isdnctrl:	^.par.h.conf_srv
o.isdnctrl:	^.sources.cc.h.cc_int
o.isdnctrl:	^.par.h.cc_par
o.isdnctrl:	^.sources.cc.h.acu_int
o.isdnctrl:	^.sources.V120.h.vs_int
o.isdnctrl:	^.sources.V120.h.vs_unit
o.isdnctrl:	^.par.h.v120_par
o.isdnctrl:	^.sources.dl.h.v120_int
o.isdnctrl:	^.par.h.ap_par
o.isdnctrl:	^.sources.ph.h.ph_int
o.isdnctrl:	^.par.h.ph_par
o.isdnctrl:	^.par.h.ap_par
o.isdnctrl:	^.par.h.consts
o.isdnctrl:	h.device
o.isdnctrl:	h.handler
o.isdnctrl:	h.isdnctrl
o.isdnctrl:	h.driver
o.isdnctrl:	h.dd_int
