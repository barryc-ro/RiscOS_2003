#
#          Makefile for Omnitel PH_B driver entity
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -Ic: -I^.^.par -I^.services -I^.dl -I^.ph

LIBFILES = 



# ----------------------------------------------------------------- #
# Various Macros
# ----------------------------------------------------------------- #



CC = cc
NOTMODULE_CFLAGS = $(INCLUDES) -c -Throwback -depend !Depend  $(VARIANT_CFLAGS) $(VARIANT_CFLAGS)
# no error for invalid casts 
NOCAST_CFLAGS = -ecf
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
.c.o:;		$(CC) $(MODULE_CFLAGS) $(NOCAST_CFLAGS) $<
.s.o:;		$(OBJASM) -from $< -to $@

# ----------------------------------------------------------------- #
# Targets to be compiled
# ----------------------------------------------------------------- #

TARGETS = mph_b.o \
 ph_b_it.o \
 hscx_drv.o \
 hscx_it.o \
 ph_b.o
 
#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.ph_b:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.mph_b:	c.mph_b
o.mph_b:	h.ph_b
o.mph_b:	^.services.h.services
o.mph_b:	^.^.par.h.serv_par
o.mph_b:	^.^.par.h.consts
o.mph_b:	^.^.par.h._product
o.mph_b:	c:h.string
o.mph_b:	^.^.par.h.serv_por
o.mph_b:	c:sys.h.types
o.mph_b:	c:h.kernel
o.mph_b:	c:h.swis
o.mph_b:	h.ph_b_int
o.mph_b:	^.^.par.h.ph_b_par
o.mph_b:	^.services.h.it
o.mph_b:	^.^.par.h.it_par
o.mph_b:	^.dl.h.dl_int
o.mph_b:	^.^.par.h.dl_par
o.mph_b:	^.services.h.it
o.mph_b:	h.def
o.mph_b:	h.basic
o.ph_b_it:	c.ph_b_it
o.ph_b_it:	h.ph_b
o.ph_b_it:	^.services.h.services
o.ph_b_it:	^.^.par.h.serv_par
o.ph_b_it:	^.^.par.h.consts
o.ph_b_it:	^.^.par.h._product
o.ph_b_it:	c:h.string
o.ph_b_it:	^.^.par.h.serv_por
o.ph_b_it:	c:sys.h.types
o.ph_b_it:	c:h.kernel
o.ph_b_it:	c:h.swis
o.ph_b_it:	h.ph_b_int
o.ph_b_it:	^.^.par.h.ph_b_par
o.ph_b_it:	^.services.h.it
o.ph_b_it:	^.^.par.h.it_par
o.ph_b_it:	^.dl.h.dl_int
o.ph_b_it:	^.^.par.h.dl_par
o.ph_b_it:	^.services.h.it
o.ph_b_it:	h.def
o.ph_b_it:	h.basic
o.ph_b_it:	^.services.h.it
o.hscx_drv:	c.hscx_drv
o.hscx_drv:	h.ph_b
o.hscx_drv:	^.services.h.services
o.hscx_drv:	^.^.par.h.serv_par
o.hscx_drv:	^.^.par.h.consts
o.hscx_drv:	^.^.par.h._product
o.hscx_drv:	c:h.string
o.hscx_drv:	^.^.par.h.serv_por
o.hscx_drv:	c:sys.h.types
o.hscx_drv:	c:h.kernel
o.hscx_drv:	c:h.swis
o.hscx_drv:	h.ph_b_int
o.hscx_drv:	^.^.par.h.ph_b_par
o.hscx_drv:	^.services.h.it
o.hscx_drv:	^.^.par.h.it_par
o.hscx_drv:	^.dl.h.dl_int
o.hscx_drv:	^.^.par.h.dl_par
o.hscx_drv:	^.services.h.it
o.hscx_drv:	h.def
o.hscx_drv:	h.basic
o.hscx_it:	c.hscx_it
o.hscx_it:	h.ph_b
o.hscx_it:	^.services.h.services
o.hscx_it:	^.^.par.h.serv_par
o.hscx_it:	^.^.par.h.consts
o.hscx_it:	^.^.par.h._product
o.hscx_it:	c:h.string
o.hscx_it:	^.^.par.h.serv_por
o.hscx_it:	c:sys.h.types
o.hscx_it:	c:h.kernel
o.hscx_it:	c:h.swis
o.hscx_it:	h.ph_b_int
o.hscx_it:	^.^.par.h.ph_b_par
o.hscx_it:	^.services.h.it
o.hscx_it:	^.^.par.h.it_par
o.hscx_it:	^.dl.h.dl_int
o.hscx_it:	^.^.par.h.dl_par
o.hscx_it:	^.services.h.it
o.hscx_it:	h.def
o.hscx_it:	h.basic
o.ph_b:	c.ph_b
o.ph_b:	h.ph_b
o.ph_b:	^.services.h.services
o.ph_b:	^.^.par.h.serv_par
o.ph_b:	^.^.par.h.consts
o.ph_b:	^.^.par.h._product
o.ph_b:	c:h.string
o.ph_b:	^.^.par.h.serv_por
o.ph_b:	c:sys.h.types
o.ph_b:	c:h.kernel
o.ph_b:	c:h.swis
o.ph_b:	h.ph_b_int
o.ph_b:	^.^.par.h.ph_b_par
o.ph_b:	^.services.h.it
o.ph_b:	^.^.par.h.it_par
o.ph_b:	^.dl.h.dl_int
o.ph_b:	^.^.par.h.dl_par
o.ph_b:	^.services.h.it
o.ph_b:	h.def
o.ph_b:	h.basic
