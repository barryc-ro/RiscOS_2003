# Makefile for !AudioCmd
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name       Description
# ----         ----       -----------
#

COMPONENT = AudioCmd

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
INRESDIR	= LocalUserIFRes:Applet
INVSNFILE	= LocalUserIFRes:Version
INCHOICEDIR	= LocalUserIFRes:Choices
INPREDIR	= LocalUserIFRes:PreDesktop
INIMGDIR	= LocalUserIFRes:images
#INDESKDIR	= LocalUserIFRes:Desktop

#
# Destination paths and filenames for files and directories to be installed to:
#
OUTRESDIR	= $(INSTDIR).$(RES)
OUTVSNDIR	= $(INSTDIR).$(VERSN)
OUTVSNFILE	= $(OUTVSNDIR).$(COMPONENT)
OUTCHOICEDIR	= $(INSTDIR).$(CHOICES)
OUTPREDIR	= $(INSTDIR).$(PRE)
OUTIMGDIR	= $(OUTRESDIR).images
#OUTDESKDIR	= $(INSTDIR).$(DESK)

#
# Flags and translation macros:
#
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v
CHMODFLAGS	= +ow +or +wr -ww
TRANSFLAGS	= $(THROWBACK) -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"
#TRANSMAPPINGS	= TranslateRes:Mappings
#TRANSCHOICEDIR	= TranslateRes:Choices

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
	@echo $(COMPONENT): nothing to do.

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
	$(CP) $(INRESDIR).*     $(OUTRESDIR).*          $(CPFLAGS)
	$(CP) $(INVSNFILE)      $(OUTVSNFILE)           $(CPFLAGS)
	Do $(AWK) -- "/.define.*CMHG.*/" VersionNum >>$(OUTVSNFILE)
	$(CHMOD) $(INSTDIR)
	@echo $(COMPONENT): installed to $(INSTDIR)

translate: dirs
	@echo Translation Complete.

dirs:
	$(MKDIR) $(OUTRESDIR)
	$(MKDIR) $(OUTVSNDIR)

# Dynamic dependencies:
