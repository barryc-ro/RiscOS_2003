#
#          Makefile for DTE PPP 
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -I^.^.par -Ic: -I^.services -I^.pdte 

LIBFILES = 



# ----------------------------------------------------------------- #
# Various Macros
# ----------------------------------------------------------------- #



CC = cc
NOTMODULE_CFLAGS = $(INCLUDES) -c -Throwback -depend !Depend  $(VARIANT_CFLAGS)
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

TARGETS = dte_ppp.o

#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.dte:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.dte_ppp:	c.dte_ppp
o.dte_ppp:	^.services.h.services
o.dte_ppp:	^.^.par.h.serv_par
o.dte_ppp:	^.^.par.h.consts
o.dte_ppp:	^.^.par.h._product
o.dte_ppp:	c:h.string
o.dte_ppp:	^.^.par.h.serv_por
o.dte_ppp:	c:sys.h.types
o.dte_ppp:	c:h.kernel
o.dte_ppp:	c:h.swis
o.dte_ppp:	^.^.par.h.dte_par
o.dte_ppp:	h.dte
o.dte_ppp:	^.services.h.services
o.dte_ppp:	h.dte_int
o.dte_ppp:	^.^.par.h.dte_par
o.dte_ppp:	^.pdte.h.pdte_int
o.dte_ppp:	^.^.par.h.dte_par
o.dte_ppp:	^.^.par.h.pdte_par
o.dte_ppp:	h.dte_ch
o.dte_ppp:	h.dte_ppp
