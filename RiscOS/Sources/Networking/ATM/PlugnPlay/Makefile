# File:    Makefile
# Purpose: Makefile for PlugnPlay
# Author:  Ben Laughton
#
# History:
#
# 1999-01-08: BAL
# Created.




# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CC		= $(MemCheckCC) cc
CMHG		= cmhg
CP		= copy
LD		= $(MemCheckLD) link
RM		= remove
MODSQZ		= modsqz
RESGEN		= resgen
WIPE		= -wipe
DATA2AOF	= datatoaof

ASMFLAGS	= -depend !Depend -Stamp -quit $(THROWBACK)
CFLAGS		= -depend !Depend -c -strict -wps -zM -zps1 -ffah $(INCLUDES) $(THROWBACK) $(EXTRACFLAGS)
CMHGFLAGS	= -depend !Depend -p $(THROWBACK) $(DFLAGS)
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v
DATA2AOFFLAGS	= -L _length -S -P -DP

DFLAGS		= 




# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -IC:




# ------------------------------------------------------------------------------
# Libraries
#

CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM 		= RISC_OSLib:o.AbsSym




# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= PlugnPlay
RAMTARGET	= rm.PnP
ROMTARGET	= aof.PnP

OBJS = \
 o.ATMimage\
 o.ATMinit\
 o.atmio\
 o.convert\
 o.error\
 o.MicroDelay\
 o.modhdr\
 o.module\
 o.pnp\
 o.pnpswis\
 o.utils
 

HDR		= h.modhdr




# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES:	.o .h .s .c .cmhg .x
.c.o:;		$(CC) $(CFLAGS) $(DFLAGS) -o $@ $<
.s.o:;		$(AS) $(ASMFLAGS) -o $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) $< -o $@
.cmhg.h:;	$(CMHG) $(CMHGFLAGS) $< -d $@
.x.o:;		$(DATA2AOF) -f $< -o $@ $(DATA2AOFFLAGS)




# ------------------------------------------------------------------------------
# Build rules
#

ram: $(RAMTARGET)
	@echo $(COMPONENT): all complete

rom: $(ROMTARGET)
	@echo $(COMPONENT): rom module built

export:
	@echo $(COMPONENT): export complete

install_rom: $(ROMTARGET)
	$(CP) $(ROMTARGET) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
	@echo $(COMPONENT): rom module installed

clean:
	$(WIPE) aof.* $(WFLAGS)
	$(RM) linked.$(COMPONENT)
	$(RM) map.$(COMPONENT)
	$(WIPE) o.* $(WFLAGS)
	$(WIPE) rm.* $(WFLAGS)
	$(RM) $(HDR)
	$(RM) $(RAMTARGET)
	$(RM) $(ROMTARGET)
	$(RM) syms
	@echo $(COMPONENT): cleaned

resources:
        @echo $(COMPONENT): resource files copied

o.dirs:
	@$(MKDIR) aof
	@$(MKDIR) linked
	@$(MKDIR) map
	@$(MKDIR) o
	@$(MKDIR) rm
	create o.dirs
	stamp o.dirs


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#


# Target 

$(RAMTARGET): $(OBJS) $(HDR) $(LIBS) $(CLIB) o.dirs
	$(LD) -rmf -symbols @.syms -o $@ $(OBJS) $(LIBS) $(CLIB)


# ROM Target (re-linked at ROM Image build time)

$(ROMTARGET): $(OBJS) $(HDR) $(LIBS) $(ROMCSTUBS) o.dirs
	$(LD) -o $@ -aof $(OBJS) $(LIBS) $(ROMCSTUBS)


# Final link for the ROM Image (using given base address)

rom_link:
	$(MKDIR) linked
	$(MKDIR) map
	$(LD) -o linked.$(COMPONENT) -map -rmf -base $(ADDRESS) $(ROMTARGET) $(ABSSYM) > map.$(COMPONENT)
	$(CP) linked.$(COMPONENT) $(LINKDIR).$(COMPONENT) $(CPFLAGS)
	@echo $(COMPONENT): rom_link complete




# ------------------------------------------------------------------------------
# Dynamic dependencies:
