# Makefile for !KeyEvents
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name       Description
# ----         ----       -----------
# 19-Sep-2000  SAR        Created.
#

COMPONENT = KeyEvents

#
# Generic options:
#
AWK		= awk
MKDIR		= mkdir -p
AS		= objasm
CC		= cc
CMHG		= cmhg
CP		= copy
LD		= link
RM		= remove
MODSQZ		= modsqz
WIPE		= -wipe
CD		= dir
TOUCH		= create
CHMOD		= attr $(CHMODFLAGS)
TRANSLATE	= translate

#
# Paths and filenames for files and directories within this component:
#
INAPPDIR	= Resources.App
INMAPDIR	= Resources.Mappings

#
# Destination paths and filenames for files and directories to be installed to:
#
OUTAPPDIR	= $(INSTDIR).!$(COMPONENT)
OUTMAPDIR	= $(OUTAPPDIR).Mappings

#
# Flags and translation macros:
#
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v
CHMODFLAGS	= +ow +or +wr -ww
TRANSFLAGS	= $(THROWBACK) -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"
TRANSMAPPINGS	= TranslateRes:Mappings
TRANSCHOICEDIR	= TranslateRes:Choices

#
# Rule patterns:
#
.SUFFIXES:	.o .s .c .h .cmhg
.c.o:;		$(CC) $(CFLAGS) -o $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.cmhg.h:;	$(CMHG) $(CMHGFLAGS) -d $@ $<
.s.o:;		$(AS) $(AFLAGS) $< $@

#
all: install
	@echo $(COMPONENT): all complete

#
# RISC OS ROM build rules:
#
clean:
	$(WIPE) $(OUTAPPDIR) $(WFLAGS)
	@echo $(COMPONENT): cleaned.

export_hdrs:
	@echo $(COMPONENT): nothing to do.

export_libs:
	@echo $(COMPONENT): nothing to do.

resources:
	@echo $(COMPONENT): nothing to do.

rom:
	@echo $(COMPONENT): nothing to do.

install_rom:
	@echo $(COMPONENT): nothing to do.

rom_link:
	@echo $(COMPONENT): nothing to do.

#
# RISC OS disc build rules:
#
install: translate dirs
	$(CP) $(INAPPDIR).*     $(OUTAPPDIR).*      $(CPFLAGS)
	$(CP) $(INMAPDIR).*     $(OUTMAPDIR).*      $(CPFLAGS)
	$(CHMOD) $(OUTAPPDIR)
	@echo $(COMPONENT): installed as $(OUTAPPDIR)

translate: dirs
	@echo Translation Complete.

dirs:
	$(MKDIR) $(OUTAPPDIR)
	$(MKDIR) $(OUTMAPDIR)

# Dynamic dependencies:
