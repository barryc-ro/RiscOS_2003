# Makefile for NCWriter
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 17-Jun-96  SMC          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link -v
RM      = remove
WIPE    = -wipe
COMPRESS = squash

#
# Include files
#
INCLUDES = -Irisc_oslib.,clib.,C:

#
# Libraries
#
CLIB      = C:o.stubs
RLIB      = risc_oslib.o.risc_oslib
DLIBS     = ${CLIB} ${RLIB}
PASSLIB   = Password.o.PasswdLib
DASHLIB   = Dashes.o.dashes
RLIBZ     = risc_oslib.oz.risc_oslib
PASSLIBZ  = Password.oz.PasswdLib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
WRAPPER   = RISCOSLIB:s.ModuleWrap

#
# Program specific options:
#
COMPONENT   = NCWriter
PRODUCT     = ${COMPONENT}
# No timeout
BOMBF       = 0
# No Acorn SP Scheme or Registration dialogue
ASPF        = 2
# Not a demo
DEMOF       = 0
TARG        = !NCWriter
TYPE        = Make
VERSION     = 1.01
NDISSUE     = ${TARG}
DEMISSUE    = !NCWDemo
ISSUE       = <issue>
DIR         = WP
STUBS       = o.stubs
OD          = WP.o
OZ          = WP.oz
TOOLS       = Library
CCC         = ${TOOLS}.cconv ${OD}
CCCZ        = ${TOOLS}.cconv ${OZ}
FILECAT     = ${TOOLS}.FileCat
TEMPLMERGE  = ${TOOLS}.TemplMerge
LN          = ${LD}
VPATH       = WP
TARGET      = aof.NCWriter
AFLAGS      = -depend !Depend -Stamp -quit
DFLAGS      = -DTABLES=1 -DTHESAURUS=0 -DDRAGDROP=1 -DFN=1 -DTIMEBOMB=$(BOMBF) -DACORNSP=$(ASPF) -DDEMO=$(DEMOF) -DTALK=0 -DWORDBANK=0 -DSIDEBAR=0 -DPRO=1 -DNETWP=1
CFLAGS      = $(INCLUDES) -Ospace -ffah -wn -c -depend !Depend
ZFLAGS      = -zM -zps1
CPFLAGS     = ~cfr~v
WFLAGS      = ~c~v
LDFLAGS     = -map -o ${TARG}.!RunImage
LNFLAGS     = ${LDFLAGS}
ASFLAGS     = ${AFLAGS}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}
RESAPP = <resource$dir>.Apps.${TARG}

OBJECTS     = $(OD).main \
$(OD).UExpressn \
$(OD).IExpressn \
$(OD).AlienFile \
$(OD).UWordOut \
$(OD).UPictEdit \
$(OD).UClipBrd \
$(OD).UTablEdit \
$(OD).UColEdit \
$(OD).UBodyEdit \
$(OD).UTextEdit \
$(OD).UTextComm \
$(OD).UIndex \
$(OD).UScaledWn \
$(OD).UPagesVew \
$(OD).URulerVew \
$(OD).TDStyles \
$(OD).TDStructu \
$(OD).TDViews \
$(OD).TDSelectn \
$(OD).TDDoc \
$(OD).TDDialog \
$(OD).URulerInt \
$(OD).UCommon \
$(OD).UPrinting \
$(OD).UPopup \
$(OD).UDialog \
$(OD).UApplicat \
$(OD).UDocument \
$(OD).UCommand \
$(OD).UControls \
$(OD).UHandler \
$(OD).UScroller \
$(OD).UWindow \
$(OD).UView \
$(OD).UMenu \
$(OD).UPrintHan \
$(OD).Support \
$(OD).ISupport \
$(OD).ASupport \
$(OD).Utilities \
$(OD).IUtilities \
$(OD).USaveDraw \
$(OD).UArray \
$(OD).UList \
$(OD).UObject \
$(OD).UMenuSetu \
$(OD).IMenuSetu \
$(OD).UMemory \
$(OD).UFailure \
$(OD).Toolbox \
$(OD).CToolbox \
$(OD).IToolbox \
$(OD).PackTempl \
$(OD).dbox \
$(OD).missing

OBJECTSZ    = $(OZ).main \
$(OZ).UExpressn \
$(OZ).IExpressn \
$(OZ).AlienFile \
$(OZ).UWordOut \
$(OZ).UPictEdit \
$(OZ).UClipBrd \
$(OZ).UTablEdit \
$(OZ).UColEdit \
$(OZ).UBodyEdit \
$(OZ).UTextEdit \
$(OZ).UTextComm \
$(OZ).UIndex \
$(OZ).UScaledWn \
$(OZ).UPagesVew \
$(OZ).URulerVew \
$(OZ).TDStyles \
$(OZ).TDStructu \
$(OZ).TDViews \
$(OZ).TDSelectn \
$(OZ).TDDoc \
$(OZ).TDDialog \
$(OZ).URulerInt \
$(OZ).UCommon \
$(OZ).UPrinting \
$(OZ).UPopup \
$(OZ).UDialog \
$(OZ).UApplicat \
$(OZ).UDocument \
$(OZ).UCommand \
$(OZ).UControls \
$(OZ).UHandler \
$(OZ).UScroller \
$(OZ).UWindow \
$(OZ).UView \
$(OZ).UMenu \
$(OZ).UPrintHan \
$(OZ).Support \
$(OZ).ISupport \
$(OZ).ASupport \
$(OZ).Utilities \
$(OZ).IUtilities \
$(OZ).USaveDraw \
$(OZ).UArray \
$(OZ).UList \
$(OZ).UObject \
$(OZ).UMenuSetu \
$(OZ).IMenuSetu \
$(OZ).UMemory \
$(OZ).UFailure \
$(OZ).Toolbox \
$(OZ).CToolbox \
$(OZ).IToolbox \
$(OZ).PackTempl \
$(OZ).dbox \
$(OZ).missing \
$(OZ).ModuleWrap

#
# Rule patterns
#
.SUFFIXES: .o .c

#
# build a !RunImage module:
#
all:    ${DIR}.!RunImage bomb asp demo

bomb:   $(DIR).BombFlag
        Run $?
        if <BombFlag> <> $(BOMBF) THEN Spool $?
        @if <BombFlag> <> $(BOMBF) THEN Echo SetEval BombFlag $(BOMBF)
        @if <BombFlag> <> $(BOMBF) THEN Spool
        Unset BombFlag

asp:    $(DIR).AspFlag
        Run $?
        if <AspFlag> <> $(ASPF) THEN Spool $?
        @if <AspFlag> <> $(ASPF) THEN Echo SetEval AspFlag $(ASPF)
        @if <AspFlag> <> $(ASPF) THEN Spool
        Unset AspFlag

demo:   $(DIR).DemoFlag
        Run $?
        if <DemoFlag> <> $(DEMOF) THEN Spool $?
        @if <DemoFlag> <> $(DEMOF) THEN Echo SetEval DemoFlag $(DEMOF)
        @if <DemoFlag> <> $(DEMOF) THEN Spool
        Unset DemoFlag

ramdisc:
        -create ram:temp
        -wipe ram:* ~cr~v
        ChangeDynamicArea -ramfssize 32K

${DIR}.!RunImage: ${OBJECTS} ${DLIBS} ramdisc
      set ProdName $(COMPONENT)
      if $(DEMOF) = 1 THEN seteval ProdVers "Demo" ELSE seteval ProdVers "$(VERSION)"
      seteval ProdVers ProdVers + " (" + ((Sys$Date RIGHT 6) LEFT 2) + "-" + (Sys$Date RIGHT 3) + "-" + (Sys$Year RIGHT 2) + ")"
      if $(DEMOF) = 1 THEN set issue $(DEMISSUE) ELSE set issue $(NDISSUE)
      echo Linking <ProdName> <ProdVers>
      -wipe $(TARG) ~C F R
      cdir $(TARG)
      $(LN) $(LNFLAGS) -via $(DIR).$(TYPE)Objs$(ASPF) $(DLIBS) { > $(DIR).BuildList }
      settype $(DIR).BuildList Text
      echo Squeezing <ProdName> <ProdVers>
      squeeze $(TARG).!RunImage

#
# Static dependencies:
#
${RLIB}:
        Dir RISC_OSLib
        Amu ${MFLAGS}
        Back

${PASSLIB}:
        Dir Password
        Amu ${MFLAGS}
        Back

${DASHLIB}:
        Dir Dashes
        Amu ${MFLAGS}
        Back

$(DIR).SP.SPLibrary: c.SProutines c.SPscheme h.SProutines $(DIR).SP.h.SPdefs $(DIR).SP.c.WindDef
      echo Making SPLibrary for NCWriter Single User
      run amu -fBuildSP SPDIR=$(DIR).SP SPlib

$(DIR).SP.!InitMDisc.!RunImage: c.InitMaster c.SProutines c.SPscheme h.SProutines $(DIR).SP.h.SPdefs
      echo Making !InitMDisc for NCWriter Single User
      run amu -fBuildSP SPDIR=$(DIR).SP SPinit

$(OD).main:   c.main
          $(CCC) main -none $(DFLAGS)

$(OD).UExpressn: c.UExpressn $(DIR).AspFlag $(DIR).DemoFlag
          $(CCC) UExpressn -none $(DFLAGS)

$(OD).IExpressn: c.IExpressn $(DIR).AspFlag $(DIR).DemoFlag
          $(CCC) IExpressn -none $(DFLAGS)

$(OD).AlienFile: c.AlienFile
          $(CCC) AlienFile -none $(DFLAGS)

$(OD).UWordOut: c.UWordOut
          $(CCC) UWordOut -none $(DFLAGS)

$(OD).USpellChk: c.USpellChk
          $(CCC) USpellChk -none $(DFLAGS)

$(OD).ESpellChk: c.ESpellChk
          $(CCC) ESpellChk -none $(DFLAGS)

$(OD).UPictEdit: c.UPictEdit
          $(CCC) UPictEdit -none $(DFLAGS)

$(OD).UTextComm: c.UTextComm
          $(CCC) UTextComm -none $(DFLAGS)

$(OD).UIndex: c.UIndex
          $(CCC) UIndex -none $(DFLAGS)

$(OD).UClipBrd: c.UClipBrd
          $(CCC) UClipBrd -none $(DFLAGS)

$(OD).UTablEdit: c.UTablEdit
          $(CCC) UTablEdit -none $(DFLAGS)

$(OD).UColEdit: c.UColEdit
          $(CCC) UColEdit -none $(DFLAGS)

$(OD).UBodyEdit: c.UBodyEdit
          $(CCC) UBodyEdit -none $(DFLAGS)

$(OD).UTextEdit: c.UTextEdit
          $(CCC) UTextEdit -none $(DFLAGS)

$(OD).UScaledWn: c.UScaledWn
          $(CCC) UScaledWn -none $(DFLAGS)

$(OD).UPagesVew: c.UPagesVew
          $(CCC) UPagesVew -none $(DFLAGS)

$(OD).URulerVew: c.URulerVew
          $(CCC) URulerVew -none $(DFLAGS)

$(OD).TDStyles: c.TDStyles
          $(CCC) TDStyles -none $(DFLAGS)

$(OD).TDStructu: c.TDStructu
          $(CCC) TDStructu -none $(DFLAGS)

$(OD).TDViews: c.TDViews
          $(CCC) TDViews -none $(DFLAGS)

$(OD).TDSelectn: c.TDSelectn
          $(CCC) TDSelectn -none $(DFLAGS)

$(OD).TDDoc: c.TDDoc
          $(CCC) TDDoc -none $(DFLAGS)

$(OD).TDDialog: c.TDDialog
          $(CCC) TDDialog -none $(DFLAGS)

$(OD).URulerInt: c.URulerInt
          $(CCC) URulerInt -none $(DFLAGS)

$(OD).UCommon:  c.UCommon
          $(CCC) UCommon -none $(DFLAGS)

$(OD).UPrinting: c.UPrinting $(DIR).DemoFlag
          $(CCC) UPrinting -none $(DFLAGS)

$(OD).UPopup: c.UPopup
          $(CCC) UPopup -none $(DFLAGS)

$(OD).UDialog: c.UDialog
          $(CCC) UDialog -none $(DFLAGS)

$(OD).UMenu:  c.UMenu
          $(CCC) UMenu -none $(DFLAGS)

$(OD).UApplicat: c.UApplicat $(DIR).AspFlag $(DIR).DemoFlag
          $(CCC) UApplicat -none $(DFLAGS)

$(OD).UDocument: c.UDocument $(DIR).DemoFlag
          $(CCC) UDocument -none $(DFLAGS)

$(OD).UCommand: c.UCommand
          $(CCC) UCommand -none $(DFLAGS)

$(OD).UControls: c.UControls
          $(CCC) UControls -none $(DFLAGS)

$(OD).UHandler: c.UHandler
          $(CCC) UHandler -none $(DFLAGS)

$(OD).UScroller: c.UScroller
          $(CCC) UScroller -none $(DFLAGS)

$(OD).UWindow: c.UWindow
          $(CCC) UWindow -none $(DFLAGS)

$(OD).UView: c.UView
          $(CCC) UView -none $(DFLAGS)

$(OD).UPrintHan: c.UPrintHan
          $(CCC) UPrintHan -none $(DFLAGS)

$(OD).Support: c.Support $(DIR).DemoFlag
          $(CCC) Support -none $(DFLAGS)

$(OD).ISupport: c.ISupport
          $(CCC) ISupport -none $(DFLAGS)

$(OD).ASupport : ASupport.s
          $(AS) $(ASFLAGS) -f s.ASupport -t $(OD).ASupport

$(DIR).SP0.SPLibrary: c.URegister h.Toolbox
          $(CCC) URegister -none $(DFLAGS)
          copy $(DIR).o.URegister $(DIR).SP0.SPLibrary ~C F ~V

$(OD).Utilities: c.Utilities $(DIR).DemoFlag
          $(CCC) Utilities -none $(DFLAGS)

$(OD).IUtilities: c.IUtilities
          $(CCC) IUtilities -none $(DFLAGS)

$(OD).USaveDraw: c.USaveDraw
          $(CCC) USaveDraw -none $(DFLAGS)

$(OD).UArray:  c.UArray
          $(CCC) UArray -none $(DFLAGS)

$(OD).UList:  c.UList
          $(CCC) UList -none $(DFLAGS)

$(OD).UObject:  c.UObject
          $(CCC) UObject -none $(DFLAGS)

$(OD).UMenuSetu: c.UMenuSetu
          $(CCC) UMenuSetu -none $(DFLAGS)

$(OD).IMenuSetu: c.IMenuSetu
          $(CCC) IMenuSetu -none $(DFLAGS)

$(OD).UMemory: c.UMemory
          spool @.trace.UMemory
          $(CC) $(CFLAGS) $(DFLAGS) -o$(OD).UMemory UMemory
          spool

$(OD).UFailure: c.UFailure
          $(CCC) UFailure -none $(DFLAGS)

$(OD).Allocate: c.Allocate
          spool @.trace.Allocate
          $(CC) $(CFLAGS) -o$(OD).Allocate Allocate
          spool

$(OD).Toolbox: c.Toolbox
          $(CCC) Toolbox -none $(DFLAGS)

$(OD).IToolbox: c.IToolbox $(DIR).BombFlag
          $(CCC) IToolbox -none $(DFLAGS)

$(OD).CToolbox: c.CToolbox
          spool @.trace.CToolbox
          $(CC) $(CFLAGS) $(DFLAGS) -o$(OD).CToolbox CToolbox
          spool

$(OD).PackTempl: c.PackTempl
          $(CCC) PackTempl -none $(DFLAGS)

$(OD).dbox: c.dbox
          spool @.trace.dbox
          $(CC) $(CFLAGS) $(DFLAGS) -o$(OD).dbox dbox
          spool

$(OD).inithyph: inithyph.c
          $(CC) $(CFLAGS) $(DFLAGS) -o$(OD).inithyph inithyph

$(OD).missing: missing.s
          $(AS) $(ASFLAGS) -f s.missing -t $(OD).missing

# ROM rules
#
${RLIBZ}:
        Dir RISC_OSLib
        Amu ${MFLAGS}
        Back

${PASSLIBZ}:
        Dir Password
        Amu ${MFLAGS}
        Back

$(OZ).main:   c.main
          $(CCCZ) main -module $(DFLAGS) -DROM=1

$(OZ).UExpressn: c.UExpressn $(DIR).AspFlag $(DIR).DemoFlag
          $(CCCZ) UExpressn -module $(DFLAGS)

$(OZ).IExpressn: c.IExpressn $(DIR).AspFlag $(DIR).DemoFlag
          $(CCCZ) IExpressn -module $(DFLAGS)

$(OZ).AlienFile: c.AlienFile
          $(CCCZ) AlienFile -module $(DFLAGS)

$(OZ).UWordOut: c.UWordOut
          $(CCCZ) UWordOut -module $(DFLAGS)

$(OZ).USpellChk: c.USpellChk
          $(CCCZ) USpellChk -module $(DFLAGS)

$(OZ).ESpellChk: c.ESpellChk
          $(CCCZ) ESpellChk -module $(DFLAGS)

$(OZ).UPictEdit: c.UPictEdit
          $(CCCZ) UPictEdit -module $(DFLAGS)

$(OZ).UTextComm: c.UTextComm
          $(CCCZ) UTextComm -module $(DFLAGS)

$(OZ).UIndex: c.UIndex
          $(CCCZ) UIndex -module $(DFLAGS)

$(OZ).UClipBrd: c.UClipBrd
          $(CCCZ) UClipBrd -module $(DFLAGS)

$(OZ).UTablEdit: c.UTablEdit
          $(CCCZ) UTablEdit -module $(DFLAGS)

$(OZ).UColEdit: c.UColEdit
          $(CCCZ) UColEdit -module $(DFLAGS)

$(OZ).UBodyEdit: c.UBodyEdit
          $(CCCZ) UBodyEdit -module $(DFLAGS)

$(OZ).UTextEdit: c.UTextEdit
          $(CCCZ) UTextEdit -module $(DFLAGS)

$(OZ).UScaledWn: c.UScaledWn
          $(CCCZ) UScaledWn -module $(DFLAGS)

$(OZ).UPagesVew: c.UPagesVew
          $(CCCZ) UPagesVew -module $(DFLAGS)

$(OZ).URulerVew: c.URulerVew
          $(CCCZ) URulerVew -module $(DFLAGS)

$(OZ).TDStyles: c.TDStyles
          $(CCCZ) TDStyles -module $(DFLAGS)

$(OZ).TDStructu: c.TDStructu
          $(CCCZ) TDStructu -module $(DFLAGS)

$(OZ).TDViews: c.TDViews
          $(CCCZ) TDViews -module $(DFLAGS)

$(OZ).TDSelectn: c.TDSelectn
          $(CCCZ) TDSelectn -module $(DFLAGS)

$(OZ).TDDoc: c.TDDoc
          $(CCCZ) TDDoc -module $(DFLAGS)

$(OZ).TDDialog: c.TDDialog
          $(CCCZ) TDDialog -module $(DFLAGS)

$(OZ).URulerInt: c.URulerInt
          $(CCCZ) URulerInt -module $(DFLAGS)

$(OZ).UCommon:  c.UCommon
          $(CCCZ) UCommon -module $(DFLAGS)

$(OZ).UPrinting: c.UPrinting $(DIR).DemoFlag
          $(CCCZ) UPrinting -module $(DFLAGS)

$(OZ).UPopup: c.UPopup
          $(CCCZ) UPopup -module $(DFLAGS)

$(OZ).UDialog: c.UDialog
          $(CCCZ) UDialog -module $(DFLAGS)

$(OZ).UMenu:  c.UMenu
          $(CCCZ) UMenu -module $(DFLAGS)

$(OZ).UApplicat: c.UApplicat $(DIR).AspFlag $(DIR).DemoFlag
          $(CCCZ) UApplicat -module $(DFLAGS)

$(OZ).UDocument: c.UDocument $(DIR).DemoFlag
          $(CCCZ) UDocument -module $(DFLAGS)

$(OZ).UCommand: c.UCommand
          $(CCCZ) UCommand -module $(DFLAGS)

$(OZ).UControls: c.UControls
          $(CCCZ) UControls -module $(DFLAGS)

$(OZ).UHandler: c.UHandler
          $(CCCZ) UHandler -module $(DFLAGS)

$(OZ).UScroller: c.UScroller
          $(CCCZ) UScroller -module $(DFLAGS)

$(OZ).UWindow: c.UWindow
          $(CCCZ) UWindow -module $(DFLAGS)

$(OZ).UView: c.UView
          $(CCCZ) UView -module $(DFLAGS)

$(OZ).UPrintHan: c.UPrintHan
          $(CCCZ) UPrintHan -module $(DFLAGS)

$(OZ).Support: c.Support $(DIR).DemoFlag
          $(CCCZ) Support -module $(DFLAGS)

$(OZ).ISupport: c.ISupport
          $(CCCZ) ISupport -module $(DFLAGS)

$(OZ).ASupport : ASupport.s
          $(AS) $(ASFLAGS) -f s.ASupport -t $(OZ).ASupport

$(OZ).Utilities: c.Utilities $(DIR).DemoFlag
          $(CCCZ) Utilities -module $(DFLAGS)

$(OZ).IUtilities: c.IUtilities
          $(CCCZ) IUtilities -module $(DFLAGS)

$(OZ).USaveDraw: c.USaveDraw
          $(CCCZ) USaveDraw -module $(DFLAGS)

$(OZ).UArray:  c.UArray
          $(CCCZ) UArray -module $(DFLAGS)

$(OZ).UList:  c.UList
          $(CCCZ) UList -module $(DFLAGS)

$(OZ).UObject:  c.UObject
          $(CCCZ) UObject -module $(DFLAGS)

$(OZ).UMenuSetu: c.UMenuSetu
          $(CCCZ) UMenuSetu -module $(DFLAGS)

$(OZ).IMenuSetu: c.IMenuSetu
          $(CCCZ) IMenuSetu -module $(DFLAGS)

$(OZ).UMemory: c.UMemory
          spool @.trace.UMemory
          $(CC) $(CFLAGS) $(ZFLAGS) $(DFLAGS) -o$(OZ).UMemory UMemory
          spool

$(OZ).UFailure: c.UFailure
          $(CCCZ) UFailure -module $(DFLAGS)

$(OZ).Allocate: c.Allocate
          spool @.trace.Allocate
          $(CC) $(CFLAGS) $(ZFLAGS) -o$(OZ).Allocate Allocate
          spool

$(OZ).Toolbox: c.Toolbox
          $(CCCZ) Toolbox -module $(DFLAGS)

$(OZ).IToolbox: c.IToolbox $(DIR).BombFlag
          $(CCCZ) IToolbox -module $(DFLAGS)

$(OZ).CToolbox: c.CToolbox
          spool @.trace.CToolbox
          $(CC) $(CFLAGS) $(ZFLAGS) $(DFLAGS) -o$(OZ).CToolbox CToolbox
          spool

$(OZ).PackTempl: c.PackTempl
          $(CCCZ) PackTempl -module $(DFLAGS)

$(OZ).dbox: c.dbox
          spool @.trace.dbox
          $(CC) $(CFLAGS) $(ZFLAGS) $(DFLAGS) -o$(OZ).dbox dbox
          spool

$(OZ).inithyph: inithyph.c
          $(CC) $(CFLAGS) $(ZFLAGS) $(DFLAGS) -o$(OZ).inithyph inithyph

$(OZ).missing: missing.s
          $(AS) $(ASFLAGS) -f s.missing -t $(OZ).missing

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
	@echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
        dir Dashes
        amu $(MFLAGS)
        back
        dir Password
        amu $(MFLAGS)
        back
        dir RISC_OSLib
        amu $(MFLAGS)
        back
        $(WIPE) $(OD).* $(WFLAGS)
        $(WIPE) $(OZ).* $(WFLAGS)
        $(WIPE) trace.* $(WFLAGS)
        $(WIPE) map.* $(WFLAGS)
        $(WIPE) linked.* $(WFLAGS)
        $(WIPE) aof.* $(WFLAGS)
        $(WIPE) $(TARG).* $(WFLAGS)
        remove $(DIR).BuildList
	@echo ${COMPONENT}: cleaned

resources:
      ChangeDynamicArea -SpriteSize 32K
      if $(DEMOF) <> 1 THEN Set EW$Path Resources.Professnl.NCWriter.,Resources.Professnl.,Resources.NCWriter.,Resources.
      if $(DEMOF) = 1 THEN Set EW$Path Resources.Professnl.NCWriter.Demo.,Resources.Professnl.NCWriter.,Resources.Professnl.,Resources.NCWriter.,Resources.
      echo Building resources for <ProdName> <ProdVers>
      cdir $(RESAPP)
      cdir $(RESDIR)
   |
   | *** !Boot file ***
   |
      $(FILECAT) $(RESAPP).!Boot @.Resources.!Boot
      settype $(RESAPP).!Boot Obey
   |
   | *** !Help file ***
   |
      copy EW:!Help $(RESAPP).!Help ~C F ~V
   |
   | *** Register template ***
   |
      if ($(DEMOF) <> 1) AND ($(ASPF) = 0) then copy @.Resources.NCWriter.Register $(RESDIR).Register ~C F ~V
   |
   | *** !Run file ***
   |
      $(FILECAT) $(RESAPP).!Run @.Resources.!Run EW:!RunROM
      settype $(RESAPP).!Run Obey
   |
   | *** !Sprites file ***
   |
      SNew
      SLoad EW:!Sprites22
      SSave $(RESAPP).!Sprites
   |
   | *** !Sprites23 file *** (Do as for !Sprites !!!)
   |
   |  copy @.Resources.NCWriter.NonDemo.!Sprites23 $(TARG).!Sprites23 ~C F ~V
   |
   | *** Resources file ***
   |
      $(FILECAT) $(RESDIR).Resources EW:Resources EW:w EW:p EW:t EW:d EW:y EW:r
      Settype $(RESDIR).Resources Text
      Squash $(RESDIR).Resources
   |
   | *** Messages file (English text) ***
   |
      $(FILECAT) $(RESDIR).Messages EW:Messages EW:x EW:x1 EW:x2 EW:x3 EW:x4 EW:c EW:s EW:a1 \
        EW:a2 EW:a3 EW:l EW:b EW:e1 EW:BombRes$(BOMBF) EW:f EW:n EW:g EW:q EW:u EW:j EW:i1 EW:i2
      Settype $(RESDIR).Messages Text
      Squash $(RESDIR).Messages
   |
   | *** HelpMsgs file ***
   |
      $(FILECAT) $(RESDIR).HelpMsgs @.Resources.HelpMsgsNC @.Resources.Professnl.NCWriter.HelpMsgs
      Settype $(RESDIR).HelpMsgs Text
      Squash $(RESDIR).HelpMsgs
   |
   | *** MenuData file ***
   |
      $(FILECAT) $(RESDIR).MenuData @.Resources.MenuData @.Resources.Professnl.NCWriter.MenuData
      $(FILECAT) $(RESDIR).MenuData01 @.Resources.MenuData @.Resources.Professnl.NCWriter.MenuData01
   |
   | *** Templates file ***
   |
      $(TEMPLMERGE) $(RESDIR).Templates @.Resources.NCWriter.NCWrTempl EW:NCWrTempl
      Settype $(RESDIR).Templates Template
      Squash $(RESDIR).Templates
   |
   | *** Sprites file ***
   |
      SNew
      SLoad @.Resources.Ptrs22
      SMerge @.Resources.Professnl.NCWriter.NCSprite22
      SSave $(RESDIR).Sprites
      Squash $(RESDIR).Sprites
   |
   | *** Sprites22 file ***
   |
   |  SNew
   |  SLoad @.Resources.CommonSp22
   |  SMerge @.Resources.NCWrSpri22
   |  SMerge @.Resources.Professnl.PrSprite22
   |  SSave $(RESDIR).Sprites22
   |  Squash $(RESDIR).Sprites22
   |
   | *** Set access ***
   |
   |  Access $(RESAPP).!Run LR
   |  Access $(RESAPP).!Boot LR

install:
	${MKDIR} ${INSTDIR}
	@echo ${COMPONENT}: installed

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJECTSZ} ${RLIBZ} ${ROMSTUBS} ramdisc
        $(LD) -o $@ -aof -via $(DIR).MakeObjsZ $(RLIBZ) ${ROMSTUBS} { > $(DIR).BuildList }
        settype $(DIR).BuildList Text

#
# Final link for the ROM Image (using given base address)
#
rom_link:
	${MKDIR} linked
	${MKDIR} map
	${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
	truncate map.${COMPONENT} linked.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete


${OZ}.ModuleWrap: s.ModuleWrap s.AppName
	${AS} ${AFLAGS} s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
        ${CP} ${WRAPPER} $@ ${CPFLAGS}

# Dynamic dependencies:
WP.oz.dprintf:	c.dprintf
WP.oz.dprintf:	h.dprintf
WP.oz.dprintf:	clib.h.swis
WP.oz.dprintf:	clib.h.kernel
WP.oz.ModuleWrap: s.ModuleWrap
WP.oz.ModuleWrap: s.AppName
WP.oz.ModuleWrap: Hdr:ListOpts
WP.oz.ModuleWrap: Hdr:Macros
WP.oz.ModuleWrap: Hdr:System
WP.oz.ModuleWrap: Hdr:SWIs
WP.oz.ModuleWrap: Hdr:CPU.Generic26
WP.oz.ModuleWrap: Hdr:IO.GenericIO
WP.oz.ModuleWrap: Hdr:RISCOS
WP.oz.ModuleWrap: Hdr:ModHand
WP.oz.ModuleWrap: Hdr:EnvNumbers
WP.oz.ModuleWrap: Hdr:Proc
WP.oz.ModuleWrap: Hdr:FSNumbers
WP.oz.ModuleWrap: Hdr:NewErrors
WP.oz.ModuleWrap: Hdr:Services
WP.oz.ModuleWrap: Hdr:ResourceFS
WP.oz.ModuleWrap: Hdr:MsgTrans
WP.oz.ModuleWrap: Hdr:Variables
WP.oz.ModuleWrap: Hdr:UpCall
WP.oz.ModuleWrap: s.ResFiles
