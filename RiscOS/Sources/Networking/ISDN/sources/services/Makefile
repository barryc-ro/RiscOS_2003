#
#         Example Makefile for Omnitel Services entity
#

# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = -Ic: -I^.^.PAR -I^.ns -I^.dl  

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

TARGETS = com_proc.o \
 services.o \
 to_proc.o \
 it_proc.o \
 sm_ram.o

#Example of a single target with it's own include path
#com_proc.o:	com_proc.c
#		$(CC) -Iinclude_path com_proc.c


^.^.o.services:	$(TARGETS)
		$(LINK) $(NOTMODULE_LINKFLAGS) -o $@ $(TARGETS) 
		


clean:;		@$(WIPE) o.* $(WIPEFLAGS)

					   

# Dynamic dependencies:
o.com_proc:	c.com_proc
o.com_proc:	h.services
o.com_proc:	^.^.PAR.h.serv_par
o.com_proc:	^.^.PAR.h.consts
o.com_proc:	^.^.PAR.h._product
o.com_proc:	c:h.string
o.com_proc:	^.^.PAR.h.serv_por
o.com_proc:	c:sys.h.types
o.com_proc:	c:h.kernel
o.com_proc:	c:h.swis
o.com_proc:	h.it
o.com_proc:	^.^.PAR.h.it_par
o.com_proc:	h.ctx_ext
o.services:	c.services
o.services:	h.services
o.services:	^.^.PAR.h.serv_par
o.services:	^.^.PAR.h.consts
o.services:	^.^.PAR.h._product
o.services:	c:h.string
o.services:	^.^.PAR.h.serv_por
o.services:	c:sys.h.types
o.services:	c:h.kernel
o.services:	c:h.swis
o.services:	h.ctx_ext
o.services:	c:sys.h.types
o.services:	c:sys.h.time
o.services:	c:sys.h.time
o.services:	c:h.errno
o.services:	c:h.stdlib
o.services:	c:h.kernel
o.services:	c:h.swis
o.to_proc:	c.to_proc
o.to_proc:	h.services
o.to_proc:	^.^.PAR.h.serv_par
o.to_proc:	^.^.PAR.h.consts
o.to_proc:	^.^.PAR.h._product
o.to_proc:	c:h.string
o.to_proc:	^.^.PAR.h.serv_por
o.to_proc:	c:sys.h.types
o.to_proc:	c:h.kernel
o.to_proc:	c:h.swis
o.to_proc:	h.ctx_ext
o.to_proc:	h.it
o.to_proc:	^.^.PAR.h.it_par
o.it_proc:	c.it_proc
o.it_proc:	h.services
o.it_proc:	^.^.PAR.h.serv_par
o.it_proc:	^.^.PAR.h.consts
o.it_proc:	^.^.PAR.h._product
o.it_proc:	c:h.string
o.it_proc:	^.^.PAR.h.serv_por
o.it_proc:	c:sys.h.types
o.it_proc:	c:h.kernel
o.it_proc:	c:h.swis
o.it_proc:	h.it
o.it_proc:	^.^.PAR.h.it_par
o.sm_ram:	c.sm_ram
o.sm_ram:	h.services
o.sm_ram:	^.^.PAR.h.serv_par
o.sm_ram:	^.^.PAR.h.consts
o.sm_ram:	^.^.PAR.h._product
o.sm_ram:	c:h.string
o.sm_ram:	^.^.PAR.h.serv_por
o.sm_ram:	c:sys.h.types
o.sm_ram:	c:h.kernel
o.sm_ram:	c:h.swis
