# Makefile for g723_1 audio codec and g723_1 library
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 05 Mar 2001   BJGA            Created
#

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR           = mkdir -p
AS              = objasm
CP              = copy
CC              = cc
RM              = remove
LD              = link
WIPE            = -wipe
LIBFILE         = libfile

CFLAGS          = -c -zps1 -ffah -depend !Depend $(THROWBACK) $(INCLUDES)
AFLAGS          = -depend !Depend $(THROWBACK)
CPFLAGS         = ~CFR~V
LDFLAGS         = -rmf -o $@
WFLAGS          = ~CFR~V
LIBFILEFLAGS    = -c -o $@

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = g723_1 audio codec
LIBCOMPONENT    = g723_1 library
TARGETS         = Play		\
                  Decode	\
                  Encode
G723_1LIB       = g723_1
LIBOBJS         = o.basop	\
		  o.coder	\
	  	  o.cod_cng	\
	  	  o.decod	\
	  	  o.dec_cng	\
	  	  o.exc_lbc	\
	  	  o.lpc		\
	  	  o.lsp		\
	  	  o.tab_lbc	\
	  	  o.tame	\
	  	  o.util_lbc	\
	  	  o.vad		\
	  	  o.util_cng	\
		  o.abasop	\
		  o.aexc_lbc	\
		  o.alpc	\
		  o.alsp	\
		  o.atame	\
		  o.autil_lpc
PLAYLIB         = C:ARSndLib.o.Snd16Play
PLAYX2LIB       = C:ARSndLib.o.Snd16Playx2
COMPLIB         = C:ARSndLib.o.g723_1

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGETS) local_dirs
        @echo $(COMPONENT): all complete

clean:
        $(WIPE) o $(WFLAGS)
        $(RM) Play
        $(RM) Decode
        $(RM) Encode
        $(RM) $(G723_1LIB)
        @echo $(COMPONENT) and $(LIBCOMPONENT): cleaned

export: export_$(PHASE)

export_hdrs:
        @echo $(LIBCOMPONENT): export complete (hdrs)

#        $(MKDIR) <Lib$Dir>.ARSndLib.h
#        $(CP) h.g723_1 <Lib$Dir>.ARSndLib.h.g723_1 $(CPFLAGS)

export_libs: $(G723_1LIB) local_dirs
        $(MKDIR) <Lib$Dir>.ARSndLib.o
        $(CP) $(G723_1LIB) <Lib$Dir>.ARSndLib.o.$(G723_1LIB) $(CPFLAGS)
        @echo $(LIBCOMPONENT): export complete (libs)

local_dirs:
        $(MKDIR) o

Play: o.Play o.wrapper o.miniClib $(COMPLIB) $(PLAYLIB) 
        $(LD) $(LDFLAGS) -Map -List !Map o.wrapper o.Play $(COMPLIB) $(PLAYLIB) o.miniClib
        /bas.MapCount !Map $@ -PadFile
        SetType $@ Data

Decode: o.DecdHdr o.Decode o.decoder o.miniClib $(COMPLIB) $(PLAYLIB) 
        $(LD) $(LDFLAGS) -Map -List !Map o.DecdHdr o.Decode o.decoder $(COMPLIB) o.miniClib
        /bas.MapCount !Map $@ -PadFile
        SetType $@ Data

Encode: o.EncdHdr o.Encode o.encoder o.miniClib $(COMPLIB) $(PLAYLIB) 
        $(LD) $(LDFLAGS) -Map -List !Map o.EncdHdr o.Encode o.encoder $(COMPLIB) o.miniClib
        /bas.MapCount !Map $@ -PadFile
        SetType $@ Data

$(G723_1LIB): $(LIBOBJS)
        $(LIBFILE) $(LIBFILEFLAGS) $(LIBOBJS)

# Explicit dependencies
o.decoder: s.DecdBody
o.encoder: s.EncdBody
o.play: s.PlayBody

# ------------------------------------------------------------------------------
# Dynamic dependencies:
