# Makefile for JapanIME test program
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 02-Oct-98  KJB          Created
# 05-Oct-98  RSP          Added 'test' target and flags for cc, link and squeeze
# 06-Oct-98  RSP          Added !RunImage to 'clean'
# 09-Oct-98  KJB          Test program separated out

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir
AR	= libfile
TOUCH   = create

# defines for CHINA, TAIWAN, KOREA, TANKANJI_CNV, TANBUN, RENBUN1 and DEBUG added because
# VJE code does, for example, #IF CHINA == 1 which swamps the throwback with warnings
OPTIONS = -Dunix=1 -DSTRCT_CN=1 -DPROTO=1 -DROMDIC=1 -DCHINA=0 -DTAIWAN=0 -DKOREA=0 \
          -DTANKANJI_CNV=0 -DTANBUN=0 -DRENBUN1=0 -DDEBUG=0

CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

CCFLAGS = -g -depend !Depend -Wp -throwback -ffa ${INCLUDES} ${OPTIONS} ${SJISFONT}
LINKFLAGS = -MAP -o $@ 
SQUEEZEFLAGS = -f $@
INCLUDES = -I^.^,^.^.^.JapanIME.vjesrc,tbox:,C:,C:Unicode


all: !RunImage JapanIME IntIME Dictionary
        @echo all: complete

clean:
       ${RM} Dictionary
       ${RM} JapanIME
       ${RM} IntIME
       ${RM} !RunImage
       ${WIPE} o ${WFLAGS}
       @echo IMEtest: cleaned

!RunImage: o.Main o.VJEtest C:o.stubs tbox:o.toolboxlib tbox:o.eventlib tbox:o.wimplib C:Unicode.o.ucodelib
       ${LD} $(LINKDEBUG) $(LINKFLAGS) o.Main o.VJEtest C:o.stubs tbox:o.toolboxlib tbox:o.eventlib tbox:o.wimplib C:Unicode.o.ucodelib
       Squeeze $(SQUEEZEFLAGS) !RunImage

JapanIME: ^.^.^.JapanIME.rm.JapanIME
       $(CP) ^.^.^.JapanIME.rm.JapanIME JapanIME $(CPFLAGS)

IntIME: ^.^.rm.IntIME
       $(CP) ^.^.rm.IntIME IntIME $(CPFLAGS)

Dictionary: ^.^.^.JapanIME.Resources.Dictionary
       $(CP) ^.^.^.JapanIME.Resources.Dictionary Dictionary $(CPFLAGS)

o.Main:    c.Main o.dirs
        $(CC) $(CCFLAGS) -c c.Main -o o.Main
           
o.VJEtest: c.VJEtest o.dirs
        $(CC) $(CCFLAGS) -Wp -c c.VJEtest -o o.VJEtest

o.dirs:
        ${MKDIR} o
        ${TOUCH} o.dirs

#
# Dynamic dependencies:
