#
#          Makefile for Omnitel PH_D driver entity
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
NOTMODULE_CFLAGS = $(INCLUDES) -c -Throwback -depend !Depend -ecf $(VARIANT_CFLAGS)
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

TARGETS = mph.o \
 mph_accs.o \
 mph_it.o \
 mph_u.o \
 ph_d.o \
 ph_accss.o \
 ph_d_it.o \
 sbc_drv.o \
 arcofi.o \
 arcofisp.o \
 icc_drv.o \
 iec_drv.o \
 isac_it.o \
 mon_drv.o \
 mon1_drv.o
     

#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.ph_d:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.mph:	c.mph
o.mph:	h.ph_d
o.mph:	^.services.h.services
o.mph:	^.^.par.h.serv_par
o.mph:	^.^.par.h.consts
o.mph:	^.^.par.h._product
o.mph:	c:h.string
o.mph:	^.^.par.h.serv_por
o.mph:	c:sys.h.types
o.mph:	c:h.kernel
o.mph:	c:h.swis
o.mph:	h.ph_d_int
o.mph:	^.^.par.h.ph_d_par
o.mph:	^.ph.h.ph_int
o.mph:	^.^.par.h.ph_par
o.mph:	^.services.h.it
o.mph:	^.^.par.h.it_par
o.mph:	^.services.h.it
o.mph:	h.def
o.mph:	h.basic
o.mph:	^.services.h.it
o.mph:	h.PSB21910
o.mph:	h.ph_d
o.mph_accs:	c.mph_accs
o.mph_accs:	h.ph_d
o.mph_accs:	^.services.h.services
o.mph_accs:	^.^.par.h.serv_par
o.mph_accs:	^.^.par.h.consts
o.mph_accs:	^.^.par.h._product
o.mph_accs:	c:h.string
o.mph_accs:	^.^.par.h.serv_por
o.mph_accs:	c:sys.h.types
o.mph_accs:	c:h.kernel
o.mph_accs:	c:h.swis
o.mph_accs:	h.ph_d_int
o.mph_accs:	^.^.par.h.ph_d_par
o.mph_accs:	^.ph.h.ph_int
o.mph_accs:	^.^.par.h.ph_par
o.mph_accs:	^.services.h.it
o.mph_accs:	^.^.par.h.it_par
o.mph_accs:	^.services.h.it
o.mph_accs:	h.def
o.mph_accs:	h.basic
o.mph_accs:	^.services.h.it
o.mph_accs:	h.PSB21910
o.mph_accs:	h.ph_d
o.mph_it:	c.mph_it
o.mph_it:	h.ph_d
o.mph_it:	^.services.h.services
o.mph_it:	^.^.par.h.serv_par
o.mph_it:	^.^.par.h.consts
o.mph_it:	^.^.par.h._product
o.mph_it:	c:h.string
o.mph_it:	^.^.par.h.serv_por
o.mph_it:	c:sys.h.types
o.mph_it:	c:h.kernel
o.mph_it:	c:h.swis
o.mph_it:	h.ph_d_int
o.mph_it:	^.^.par.h.ph_d_par
o.mph_it:	^.ph.h.ph_int
o.mph_it:	^.^.par.h.ph_par
o.mph_it:	^.services.h.it
o.mph_it:	^.^.par.h.it_par
o.mph_it:	^.services.h.it
o.mph_it:	h.def
o.mph_it:	h.basic
o.mph_it:	^.services.h.it
o.mph_it:	h.PSB21910
o.mph_it:	h.ph_d
o.mph_u:	c.mph_u
o.mph_u:	h.ph_d
o.mph_u:	^.services.h.services
o.mph_u:	^.^.par.h.serv_par
o.mph_u:	^.^.par.h.consts
o.mph_u:	^.^.par.h._product
o.mph_u:	c:h.string
o.mph_u:	^.^.par.h.serv_por
o.mph_u:	c:sys.h.types
o.mph_u:	c:h.kernel
o.mph_u:	c:h.swis
o.mph_u:	h.ph_d_int
o.mph_u:	^.^.par.h.ph_d_par
o.mph_u:	^.ph.h.ph_int
o.mph_u:	^.^.par.h.ph_par
o.mph_u:	^.services.h.it
o.mph_u:	^.^.par.h.it_par
o.mph_u:	^.services.h.it
o.mph_u:	h.def
o.mph_u:	h.basic
o.mph_u:	^.services.h.it
o.mph_u:	h.PSB21910
o.mph_u:	h.ph_d
o.ph_d:	c.ph_d
o.ph_d:	h.ph_d
o.ph_d:	^.services.h.services
o.ph_d:	^.^.par.h.serv_par
o.ph_d:	^.^.par.h.consts
o.ph_d:	^.^.par.h._product
o.ph_d:	c:h.string
o.ph_d:	^.^.par.h.serv_por
o.ph_d:	c:sys.h.types
o.ph_d:	c:h.kernel
o.ph_d:	c:h.swis
o.ph_d:	h.ph_d_int
o.ph_d:	^.^.par.h.ph_d_par
o.ph_d:	^.ph.h.ph_int
o.ph_d:	^.^.par.h.ph_par
o.ph_d:	^.services.h.it
o.ph_d:	^.^.par.h.it_par
o.ph_d:	^.services.h.it
o.ph_d:	h.def
o.ph_d:	h.basic
o.ph_d:	^.services.h.it
o.ph_d:	^.dl.h.dl_int
o.ph_d:	^.^.par.h.dl_par
o.ph_d:	^.services.h.ctx_ext
o.ph_accss:	c.ph_accss
o.ph_accss:	h.ph_d
o.ph_accss:	^.services.h.services
o.ph_accss:	^.^.par.h.serv_par
o.ph_accss:	^.^.par.h.consts
o.ph_accss:	^.^.par.h._product
o.ph_accss:	c:h.string
o.ph_accss:	^.^.par.h.serv_por
o.ph_accss:	c:sys.h.types
o.ph_accss:	c:h.kernel
o.ph_accss:	c:h.swis
o.ph_accss:	h.ph_d_int
o.ph_accss:	^.^.par.h.ph_d_par
o.ph_accss:	^.ph.h.ph_int
o.ph_accss:	^.^.par.h.ph_par
o.ph_accss:	^.services.h.it
o.ph_accss:	^.^.par.h.it_par
o.ph_accss:	^.services.h.it
o.ph_accss:	h.def
o.ph_accss:	h.basic
o.ph_accss:	^.services.h.it
o.ph_d_it:	c.ph_d_it
o.ph_d_it:	h.ph_d
o.ph_d_it:	^.services.h.services
o.ph_d_it:	^.^.par.h.serv_par
o.ph_d_it:	^.^.par.h.consts
o.ph_d_it:	^.^.par.h._product
o.ph_d_it:	c:h.string
o.ph_d_it:	^.^.par.h.serv_por
o.ph_d_it:	c:sys.h.types
o.ph_d_it:	c:h.kernel
o.ph_d_it:	c:h.swis
o.ph_d_it:	h.ph_d_int
o.ph_d_it:	^.^.par.h.ph_d_par
o.ph_d_it:	^.ph.h.ph_int
o.ph_d_it:	^.^.par.h.ph_par
o.ph_d_it:	^.services.h.it
o.ph_d_it:	^.^.par.h.it_par
o.ph_d_it:	^.services.h.it
o.ph_d_it:	h.def
o.ph_d_it:	h.basic
o.ph_d_it:	^.services.h.it
o.sbc_drv:	c.sbc_drv
o.sbc_drv:	h.ph_d
o.sbc_drv:	^.services.h.services
o.sbc_drv:	^.^.par.h.serv_par
o.sbc_drv:	^.^.par.h.consts
o.sbc_drv:	^.^.par.h._product
o.sbc_drv:	c:h.string
o.sbc_drv:	^.^.par.h.serv_por
o.sbc_drv:	c:sys.h.types
o.sbc_drv:	c:h.kernel
o.sbc_drv:	c:h.swis
o.sbc_drv:	h.ph_d_int
o.sbc_drv:	^.^.par.h.ph_d_par
o.sbc_drv:	^.ph.h.ph_int
o.sbc_drv:	^.^.par.h.ph_par
o.sbc_drv:	^.services.h.it
o.sbc_drv:	^.^.par.h.it_par
o.sbc_drv:	^.services.h.it
o.sbc_drv:	h.def
o.sbc_drv:	h.basic
o.sbc_drv:	^.services.h.it
o.arcofi:	c.arcofi
o.arcofi:	h.ph_d
o.arcofi:	^.services.h.services
o.arcofi:	^.^.par.h.serv_par
o.arcofi:	^.^.par.h.consts
o.arcofi:	^.^.par.h._product
o.arcofi:	c:h.string
o.arcofi:	^.^.par.h.serv_por
o.arcofi:	c:sys.h.types
o.arcofi:	c:h.kernel
o.arcofi:	c:h.swis
o.arcofi:	h.ph_d_int
o.arcofi:	^.^.par.h.ph_d_par
o.arcofi:	^.ph.h.ph_int
o.arcofi:	^.^.par.h.ph_par
o.arcofi:	^.services.h.it
o.arcofi:	^.^.par.h.it_par
o.arcofi:	^.services.h.it
o.arcofi:	h.def
o.arcofi:	h.basic
o.arcofi:	^.services.h.it
o.arcofisp:	c.arcofisp
o.arcofisp:	h.ph_d
o.arcofisp:	^.services.h.services
o.arcofisp:	^.^.par.h.serv_par
o.arcofisp:	^.^.par.h.consts
o.arcofisp:	^.^.par.h._product
o.arcofisp:	c:h.string
o.arcofisp:	^.^.par.h.serv_por
o.arcofisp:	c:sys.h.types
o.arcofisp:	c:h.kernel
o.arcofisp:	c:h.swis
o.arcofisp:	h.ph_d_int
o.arcofisp:	^.^.par.h.ph_d_par
o.arcofisp:	^.ph.h.ph_int
o.arcofisp:	^.^.par.h.ph_par
o.arcofisp:	^.services.h.it
o.arcofisp:	^.^.par.h.it_par
o.arcofisp:	^.services.h.it
o.arcofisp:	h.def
o.arcofisp:	h.basic
o.arcofisp:	^.services.h.it
o.arcofisp:	h.arcofi
o.icc_drv:	c.icc_drv
o.icc_drv:	h.ph_d
o.icc_drv:	^.services.h.services
o.icc_drv:	^.^.par.h.serv_par
o.icc_drv:	^.^.par.h.consts
o.icc_drv:	^.^.par.h._product
o.icc_drv:	c:h.string
o.icc_drv:	^.^.par.h.serv_por
o.icc_drv:	c:sys.h.types
o.icc_drv:	c:h.kernel
o.icc_drv:	c:h.swis
o.icc_drv:	h.ph_d_int
o.icc_drv:	^.^.par.h.ph_d_par
o.icc_drv:	^.ph.h.ph_int
o.icc_drv:	^.^.par.h.ph_par
o.icc_drv:	^.services.h.it
o.icc_drv:	^.^.par.h.it_par
o.icc_drv:	^.services.h.it
o.icc_drv:	h.def
o.icc_drv:	h.basic
o.icc_drv:	^.services.h.it
o.iec_drv:	c.iec_drv
o.iec_drv:	h.ph_d
o.iec_drv:	^.services.h.services
o.iec_drv:	^.^.par.h.serv_par
o.iec_drv:	^.^.par.h.consts
o.iec_drv:	^.^.par.h._product
o.iec_drv:	c:h.string
o.iec_drv:	^.^.par.h.serv_por
o.iec_drv:	c:sys.h.types
o.iec_drv:	c:h.kernel
o.iec_drv:	c:h.swis
o.iec_drv:	h.ph_d_int
o.iec_drv:	^.^.par.h.ph_d_par
o.iec_drv:	^.ph.h.ph_int
o.iec_drv:	^.^.par.h.ph_par
o.iec_drv:	^.services.h.it
o.iec_drv:	^.^.par.h.it_par
o.iec_drv:	^.services.h.it
o.iec_drv:	h.def
o.iec_drv:	h.basic
o.iec_drv:	^.services.h.it
o.iec_drv:	h.PSB21910
o.iec_drv:	h.ph_d
o.isac_it:	c.isac_it
o.isac_it:	h.ph_d
o.isac_it:	^.services.h.services
o.isac_it:	^.^.par.h.serv_par
o.isac_it:	^.^.par.h.consts
o.isac_it:	^.^.par.h._product
o.isac_it:	c:h.string
o.isac_it:	^.^.par.h.serv_por
o.isac_it:	c:sys.h.types
o.isac_it:	c:h.kernel
o.isac_it:	c:h.swis
o.isac_it:	h.ph_d_int
o.isac_it:	^.^.par.h.ph_d_par
o.isac_it:	^.ph.h.ph_int
o.isac_it:	^.^.par.h.ph_par
o.isac_it:	^.services.h.it
o.isac_it:	^.^.par.h.it_par
o.isac_it:	^.services.h.it
o.isac_it:	h.def
o.isac_it:	h.basic
o.isac_it:	^.services.h.it
o.mon_drv:	c.mon_drv
o.mon_drv:	h.ph_d
o.mon_drv:	^.services.h.services
o.mon_drv:	^.^.par.h.serv_par
o.mon_drv:	^.^.par.h.consts
o.mon_drv:	^.^.par.h._product
o.mon_drv:	c:h.string
o.mon_drv:	^.^.par.h.serv_por
o.mon_drv:	c:sys.h.types
o.mon_drv:	c:h.kernel
o.mon_drv:	c:h.swis
o.mon_drv:	h.ph_d_int
o.mon_drv:	^.^.par.h.ph_d_par
o.mon_drv:	^.ph.h.ph_int
o.mon_drv:	^.^.par.h.ph_par
o.mon_drv:	^.services.h.it
o.mon_drv:	^.^.par.h.it_par
o.mon_drv:	^.services.h.it
o.mon_drv:	h.def
o.mon_drv:	h.basic
o.mon_drv:	^.services.h.it
o.mon_drv:	h.PSB21910
o.mon_drv:	h.ph_d
o.mon_drv:	h.PSB21910
o.mon1_drv:	c.mon1_drv
o.mon1_drv:	h.ph_d
o.mon1_drv:	^.services.h.services
o.mon1_drv:	^.^.par.h.serv_par
o.mon1_drv:	^.^.par.h.consts
o.mon1_drv:	^.^.par.h._product
o.mon1_drv:	c:h.string
o.mon1_drv:	^.^.par.h.serv_por
o.mon1_drv:	c:sys.h.types
o.mon1_drv:	c:h.kernel
o.mon1_drv:	c:h.swis
o.mon1_drv:	h.ph_d_int
o.mon1_drv:	^.^.par.h.ph_d_par
o.mon1_drv:	^.ph.h.ph_int
o.mon1_drv:	^.^.par.h.ph_par
o.mon1_drv:	^.services.h.it
o.mon1_drv:	^.^.par.h.it_par
o.mon1_drv:	^.services.h.it
o.mon1_drv:	h.def
o.mon1_drv:	h.basic
o.mon1_drv:	^.services.h.it
o.mon1_drv:	h.PSB21910
o.mon1_drv:	h.ph_d
