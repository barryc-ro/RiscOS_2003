#
#          Makefile for TEST APPLICATION 
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -I^.^.par -Ic: -I^.services 

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

TARGETS = asn1_dec.o\
 asn1_enc.o\
 asn1_prc.o\
 ber_dec.o\
 ber_enc.o

#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.asn1:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.asn1_dec:	c.asn1_dec
o.asn1_dec:	h.asn1
o.asn1_dec:	^.services.h.services
o.asn1_dec:	^.^.par.h.serv_par
o.asn1_dec:	^.^.par.h.consts
o.asn1_dec:	^.^.par.h._product
o.asn1_dec:	c:h.string
o.asn1_dec:	^.^.par.h.serv_por
o.asn1_dec:	c:sys.h.types
o.asn1_dec:	c:h.kernel
o.asn1_dec:	c:h.swis
o.asn1_dec:	h.asn1_int
o.asn1_dec:	^.^.par.h.asn1_par
o.asn1_dec:	^.^.par.ns.asn1_par
o.asn1_dec:	h.asn1_mac
o.asn1_dec:	h.asn1_ber
o.asn1_enc:	c.asn1_enc
o.asn1_enc:	h.asn1
o.asn1_enc:	^.services.h.services
o.asn1_enc:	^.^.par.h.serv_par
o.asn1_enc:	^.^.par.h.consts
o.asn1_enc:	^.^.par.h._product
o.asn1_enc:	c:h.string
o.asn1_enc:	^.^.par.h.serv_por
o.asn1_enc:	c:sys.h.types
o.asn1_enc:	c:h.kernel
o.asn1_enc:	c:h.swis
o.asn1_enc:	h.asn1_int
o.asn1_enc:	^.^.par.h.asn1_par
o.asn1_enc:	^.^.par.ns.asn1_par
o.asn1_enc:	h.asn1_mac
o.asn1_enc:	h.asn1_ber
o.asn1_enc:	h.asn1cons
o.asn1_prc:	c.asn1_prc
o.asn1_prc:	h.asn1
o.asn1_prc:	^.services.h.services
o.asn1_prc:	^.^.par.h.serv_par
o.asn1_prc:	^.^.par.h.consts
o.asn1_prc:	^.^.par.h._product
o.asn1_prc:	c:h.string
o.asn1_prc:	^.^.par.h.serv_por
o.asn1_prc:	c:sys.h.types
o.asn1_prc:	c:h.kernel
o.asn1_prc:	c:h.swis
o.asn1_prc:	h.asn1_int
o.asn1_prc:	^.^.par.h.asn1_par
o.asn1_prc:	^.^.par.ns.asn1_par
o.asn1_prc:	h.asn1_mac
o.asn1_prc:	h.asn1_ber
o.ber_dec:	c.ber_dec
o.ber_dec:	h.asn1
o.ber_dec:	^.services.h.services
o.ber_dec:	^.^.par.h.serv_par
o.ber_dec:	^.^.par.h.consts
o.ber_dec:	^.^.par.h._product
o.ber_dec:	c:h.string
o.ber_dec:	^.^.par.h.serv_por
o.ber_dec:	c:sys.h.types
o.ber_dec:	c:h.kernel
o.ber_dec:	c:h.swis
o.ber_dec:	h.asn1_int
o.ber_dec:	^.^.par.h.asn1_par
o.ber_dec:	^.^.par.ns.asn1_par
o.ber_dec:	h.asn1_mac
o.ber_dec:	h.asn1_ber
o.ber_enc:	c.ber_enc
o.ber_enc:	h.asn1
o.ber_enc:	^.services.h.services
o.ber_enc:	^.^.par.h.serv_par
o.ber_enc:	^.^.par.h.consts
o.ber_enc:	^.^.par.h._product
o.ber_enc:	c:h.string
o.ber_enc:	^.^.par.h.serv_por
o.ber_enc:	c:sys.h.types
o.ber_enc:	c:h.kernel
o.ber_enc:	c:h.swis
o.ber_enc:	h.asn1_int
o.ber_enc:	^.^.par.h.asn1_par
o.ber_enc:	^.^.par.ns.asn1_par
o.ber_enc:	h.asn1_mac
o.ber_enc:	h.asn1_ber
