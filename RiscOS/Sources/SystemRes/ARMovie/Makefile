# Makefile for ARMovie
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 30 Nov 2000   BJGA            Created
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

CFLAGS          = -c -depend !Depend $(THROWBACK) -ffah $(INCLUDES) $(DFLAGS)
AFLAGS          = -depend !Depend $(THROWBACK)
CPFLAGS         = ~CFR~V
LDFLAGS         = 
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:
DFLAGS          = 

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = ARMovie
ROOTDIR         = !$(COMPONENT)
ACCESSDIR       = $(ROOTDIR).MovingLine
AUD8DIR         = $(ROOTDIR).MovingLine
AUD16DIR        = $(ROOTDIR).MovingLine.Sound16
COLMAPDIR       = $(ROOTDIR).MovingLine.ColourMap
DOCSDIR         = $(ROOTDIR).Documents
FETCHDIR        = $(ROOTDIR).MovingLine.Fetchers
FILTERDIR       = $(ROOTDIR).Tools.Filters
FRONTENDDIR     = $(ROOTDIR).MovingLine
MSGSDIR         = $(ROOTDIR).MovingLine
PAINTDIR        = $(ROOTDIR).MovingLine.PaintCode
PALETTEDIR      = $(ROOTDIR).MovingLine
SETSNDDIR       = $(ROOTDIR).MovingLine
SHAPESDIR       = $(ROOTDIR).Shapes
TOOLSDIR        = $(ROOTDIR).Tools
TRAJDIR         = $(ROOTDIR).Trajectory
VIDDIR          = $(ROOTDIR)

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

#
# For what it's worth, this is how I'm deciding which video codec files to
# include in each build:
#
# * All files are included in encode+decode builds
# * For decode-only builds:
#     FastDec*, Dec2#xxx, Dec2# (RGB source): only if 32bpp modes supported
#     Dec2# (non-RGB source): not included
#     Dec8/Dec16: always included
#     DecompresB: included if 1-8bpp modes are supported
#     DecompresH: included if 4-16bpp modes are supported
#     Decompress: included if 8-32bpp modes are supported
#
# I've deduced this by a quick examination of Player; they don't tally exactly
# with the files included in the MultiMedia module, so beware!
#
# Ben Avison, 09 Jan 2001
#

all:
        @echo $(COMPONENT): all complete

export: export_$(PHASE)

export_hdrs:
        @Dir Sound16.adpcm
        @amu_machine export PHASE=hdrs
        @Up 2
        |
        @Dir Sound16.g721
        @amu_machine export PHASE=hdrs
        @Up 2
        |
        @Dir Sound16.g723_24
        @amu_machine export PHASE=hdrs
        @Up 2
        |
        @Dir Sound16.g723_40
        @amu_machine export PHASE=hdrs
        @Up 2
        |
        @Dir Sound16.g726_16
        @amu_machine export PHASE=hdrs
        @Up 2
        |
        @Dir Sound16.gsm
        @amu_machine export PHASE=hdrs
        @Up 2
        |
        @Dir Sound16.mpadec
        @amu_machine export PHASE=hdrs
        @Up 2
        |
        @Dir Sound16.mpaenc
        @amu_machine export PHASE=hdrs
        @Up 2
        |
        @echo $(COMPONENT): export complete (hdrs)

export_libs:
        @Dir Sound16.adpcm
        @amu_machine export PHASE=libs
        @Up 2
        |
        @Dir Sound16.g721
        @amu_machine export PHASE=libs
        @Up 2
        |
        @Dir Sound16.g723_24
        @amu_machine export PHASE=libs
        @Up 2
        |
        @Dir Sound16.g723_40
        @amu_machine export PHASE=libs
        @Up 2
        |
        @Dir Sound16.g726_16
        @amu_machine export PHASE=libs
        @Up 2
        |
        @Dir Sound16.gsm
        @amu_machine export PHASE=libs
        @Up 2
        |
        @Dir Sound16.mpadec
        @amu_machine export PHASE=libs
        @Up 2
        |
        @Dir Sound16.mpaenc
        @amu_machine export PHASE=libs
        @Up 2
        |
        @Dir Sound16.Type1
        @amu_machine export PHASE=libs
        @Up 2
        |
        @echo $(COMPONENT): export complete (libs)

install:
        @echo Starting ARMovie install phase
        @set Alias$MMD $(MKDIR) $(INSTDIR).%0
        @set Alias$CMF $(CP) %0 $(INSTDIR).%1 $(CPFLAGS)
        @setmacro Alias$If1bpp If "<Mk1bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If2bpp If "<Mk2bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If4bpp If "<Mk4bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If8bpp If "<Mk8bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If16bpp If "<Mk16bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If32bpp If "<Mk32bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If1˜8bpp If "<Mk1bpp>"="1" OR "<Mk2bpp>"="1" OR "<Mk4bpp>"="1" OR  "<Mk8bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If4˜16bpp If "<Mk4bpp>"="1" OR "<Mk8bpp>"="1" OR "<Mk16bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If8˜32bpp If "<Mk8bpp>"="1" OR "<Mk16bpp>"="1" OR "<Mk32bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @set Alias$MkARMovie /Options.!MkMovie %*0
        @set Alias$MkARSound /Options.!MkSound %*0
        @obey -v-c Options.$(BUILD)
        @unset Alias$MMD
        @unset Alias$CMF
        @unset Alias$MkARMovie
        @unset Alias$MkARSound
        @unset MkEncoders
        @unset Mk8bitSound
        @unset Mk1bpp
        @unset Mk2bpp
        @unset Mk4bpp
        @unset Mk8bpp
        @unset Mk16bpp
        @unset Mk32bpp
        @echo $(COMPONENT): installed

resources:
        @echo Starting ARMovie resources phase
        @RMEnsure ShrinkWrap 0.00 Error *** ShrinkWrap must be loaded ***
        @set Alias$MMD $(MKDIR) $(RESDIR).^.Apps.%0
        @set Alias$CMF ShrinkWrapZipCompress %0 $(RESDIR).^.Apps.%1
        @setmacro Alias$If1bpp If "<Mk1bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If2bpp If "<Mk2bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If4bpp If "<Mk4bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If8bpp If "<Mk8bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If16bpp If "<Mk16bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If32bpp If "<Mk32bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If1˜8bpp If "<Mk1bpp>"="1" OR "<Mk2bpp>"="1" OR "<Mk4bpp>"="1" OR  "<Mk8bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If4˜16bpp If "<Mk4bpp>"="1" OR "<Mk8bpp>"="1" OR "<Mk16bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @setmacro Alias$If8˜32bpp If "<Mk8bpp>"="1" OR "<Mk16bpp>"="1" OR "<Mk32bpp>"="1" OR "<MkEncoders>"="1" then %*0
        @set Alias$MkARMovie /Options.!MkMovie %*0
        @set Alias$MkARSound /Options.!MkSound %*0
        @obey -v-c Options.$(BUILD)
        @unset Alias$MMD
        @unset Alias$CMF
        @unset Alias$If1bpp
        @unset Alias$If2bpp
        @unset Alias$If4bpp
        @unset Alias$If8bpp
        @unset Alias$If16bpp
        @unset Alias$If32bpp
        @unset Alias$If1˜8bpp
        @unset Alias$If4˜16bpp
        @unset Alias$If8˜32bpp
        @unset Alias$MkARMovie
        @unset Alias$MkARSound
        @unset MkEncoders
        @unset Mk8bitSound
        @unset Mk1bpp
        @unset Mk2bpp
        @unset Mk4bpp
        @unset Mk8bpp
        @unset Mk16bpp
        @unset Mk32bpp
        @echo $(COMPONENT): resource files copied

clean:
        $(RM) !Help
        |
        $(RM) Player
        $(RM) crunched.Player
        |
        $(RM) Filters.DirectY
        $(RM) Filters.SharpenY
        $(RM) Filters.SMedian5
        $(RM) Filters.SMedian9
        $(RM) Filters.SmoothY5
        $(RM) Filters.SmoothY9
        $(RM) Filters.SmoothY91
        $(RM) Filters.SmoothY92
        $(RM) Filters.TClamp
        $(RM) Filters.TClamp2
        $(RM) Filters.TClamp4
        $(RM) Filters.TClamp8
        @Dir Tools.Extract
        @amu_machine clean
        @stripdepnd
        @Up 2
        @Dir Tools.Join
        @amu_machine clean
        @stripdepnd
        @Up 2
        @Dir Tools.Join4
        @amu_machine clean
        @stripdepnd
        @Up 2
        $(RM) crunched.CompLib
        |
        $(RM) Colour.8yuv11
        $(RM) Colour.8yuv22
        |
        $(WIPE) Resources.Shapes.Cubes2.Cube* $(WFLAGS)
        $(RM) Resources.Shapes.Cubes2.ReplayMap'
        $(WIPE) Resources.Shapes.Cubes3.Cube* $(WFLAGS)
        $(RM) Resources.Shapes.Cubes3.ReplayMap'
        $(RM) Resources.Shapes.Split.Split
        $(RM) Resources.Shapes.TestShape.Shape
        $(WIPE) Resources.Shapes.ZoomUp.Box* $(WFLAGS)
        $(WIPE) Resources.Shapes.ZoomUp.Scale* $(WFLAGS)
        $(RM) Resources.Trajectory.Circle        
        |
        @Dir Sound16.adpcm_1
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.adpcm
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.g721
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.g723_24
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.g723_40
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.g726_16
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.gsm
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.mpadec
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.mpaenc
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.mpeg-I
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.mpeg-II
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Sound16.Type1
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        $(RM) Video.MovingLine.DecompresB
        $(RM) Video.MovingLine.DecompresH
        $(RM) Video.MovingLine.Decompress
        |
        $(RM) Video.MovingLine.BatchComp
        |
        $(RM) Video.Decomp2.Dec16
        $(RM) Video.Decomp2.DecompresB
        $(RM) Video.Decomp2.Decompress
        |
        $(RM) Video.Decomp3.Dec16
        $(RM) Video.Decomp3.DecompresB
        $(RM) Video.Decomp3.Decompress
        |
        $(RM) Video.Decomp4.Dec8
        $(RM) Video.Decomp4.Decompress
        |
        $(RM) Video.Decomp5.Dec16
        $(RM) Video.Decomp5.DecompresB
        $(RM) Video.Decomp5.Decompress
        |
        $(RM) Video.Decomp6.Dec16
        $(RM) Video.Decomp6.DecompresB
        $(RM) Video.Decomp6.Decompress
        |
        $(RM) Video.Decomp7.DecompresB
        $(RM) Video.Decomp7.DecompresH
        $(RM) Video.Decomp7.Decompress
        |
        $(RM) Video.Decomp7.BatchComp
        |
        $(RM) Video.Decomp8.Dec21
        $(RM) Video.Decomp8.Dec24
        $(RM) Video.Decomp8.DecompresB
        $(RM) Video.Decomp8.DecompresH
        $(RM) Video.Decomp8.Decompress
        $(RM) Video.Decomp8.FastDec21
        $(RM) Video.Decomp8.FastDec24
        |
        $(RM) Video.Decomp9.Dec24
        $(RM) Video.Decomp9.DecompresB
        $(RM) Video.Decomp9.DecompresH
        $(RM) Video.Decomp9.Decompress
        $(RM) Video.Decomp9.FastDec21
        $(RM) Video.Decomp9.FastDec24
        $(RM) Video.Decomp9.FastDecYUV
        |
        $(RM) Video.Decomp10.Dec24
        $(RM) Video.Decomp10.DecompresB
        $(RM) Video.Decomp10.DecompresH
        $(RM) Video.Decomp10.Decompress
        $(RM) Video.Decomp10.FastDec21
        $(RM) Video.Decomp10.FastDec24
        $(RM) Video.Decomp10.FastDecYUV
        |
        $(RM) Video.Decomp11.Dec24
        $(RM) Video.Decomp11.DecompresB
        $(RM) Video.Decomp11.DecompresH
        $(RM) Video.Decomp11.Decompress
        |
        $(RM) Video.Decomp15.Uncomp15.DecompresB
        $(RM) Video.Decomp15.Uncomp15.DecompresH
        $(RM) Video.Decomp15.Uncomp15.Decompress
        $(RM) Video.Decomp15.Uncomp16.Dec16
        $(RM) Video.Decomp15.Uncomp16.DecompresB
        $(RM) Video.Decomp15.Uncomp16.Decompress
        |
        $(RM) Video.Decomp16.Dec24
        $(RM) Video.Decomp16.DecompresB
        $(RM) Video.Decomp16.DecompresH
        $(RM) Video.Decomp16.Decompress
        |
        $(RM) Video.Decomp17.Dec16
        $(RM) Video.Decomp17.DecompresB
        $(RM) Video.Decomp17.DecompresH
        $(RM) Video.Decomp17.Decompress
        |
        $(RM) Video.Decomp17.BatchComp
        |
        @Dir Video.Decomp18
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Video.Decomp18.BatchComp
        @amu_machine clean
        @stripdepnd
        @Up 3
        |
        $(RM) Video.Decomp19.Dec16
        $(RM) Video.Decomp19.DecompresB
        $(RM) Video.Decomp19.DecompresH
        $(RM) Video.Decomp19.Decompress
        |
        $(RM) Video.Decomp19.BatchComp
        |
        $(RM) Video.Decomp20.D4tab
        $(RM) Video.Decomp20.DecompresB
        $(RM) Video.Decomp20.DecompresH
        $(RM) Video.Decomp20.Decompress
        |
        $(RM) Video.Decomp20.BatchComp
        |
        $(RM) Video.Decomp21.Dec24
        $(RM) Video.Decomp21.DecompresB
        $(RM) Video.Decomp21.DecompresH
        $(RM) Video.Decomp21.Decompress
        $(RM) Video.Decomp21.FastDec21
        $(RM) Video.Decomp21.FastDec24
        $(RM) Video.Decomp21.FastDecYUV
        |
        $(RM) Video.Decomp22.D4tab
        $(RM) Video.Decomp22.Dec24
        $(RM) Video.Decomp22.DecompresB
        $(RM) Video.Decomp22.DecompresH
        $(RM) Video.Decomp22.Decompress
        |
        $(RM) Video.Decomp23.Dec16
        $(RM) Video.Decomp23.DecompresB
        $(RM) Video.Decomp23.Decompress
        |
        $(RM) Video.Decomp24.Dec16
        $(RM) Video.Decomp24.DecompresB
        $(RM) Video.Decomp24.Decompress
        |
        $(RM) Video.Decomp25.D4tab
        $(RM) Video.Decomp25.D4tabU
        $(RM) Video.Decomp25.Dec24
        $(RM) Video.Decomp25.DecompresH
        $(RM) Video.Decomp25.Decompress
        $(RM) Video.Decomp25.FastDecYUV
        |
        $(RM) Video.Decomp26.D4tab
        $(RM) Video.Decomp26.D4tabU
        $(RM) Video.Decomp26.Dec24
        $(RM) Video.Decomp26.DecompresH
        $(RM) Video.Decomp26.Decompress
        |
        $(RM) Video.Decomp27.D4tab
        $(RM) Video.Decomp27.D4tabU
        $(RM) Video.Decomp27.Dec24
        $(RM) Video.Decomp27.DecompresH
        $(RM) Video.Decomp27.Decompress
        |
        $(RM) Video.Decomp699.Decompress
        |
        @Dir Video.Decomp901
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @Dir Video.Decomp902
        @amu_machine clean
        @stripdepnd
        @Up 2
        |
        @echo $(COMPONENT): cleaned

Core_Decode:          Resources.!Boot \
                      Resources.!Run \
                      Player \
                      Resources.Access \
                      LocalRes:Messages \
                      bas.SetSndDir
        /bas.MakeHelp
        MMD $(ROOTDIR)
        CMF Resources.!Boot                        $(ROOTDIR).!Boot
        CMF !Help                                  $(ROOTDIR).!Help
        CMF Resources.!Run                         $(ROOTDIR).!Run
        CMF Player                                 $(ROOTDIR).Player
        MMD $(ACCESSDIR)
        CMF Resources.Access                       $(ACCESSDIR).Access
        MMD $(MSGSDIR)
        CMF LocalRes:Messages                      $(MSGSDIR).Messages
        MMD $(SETSNDDIR)
        CMF bas.SetSndDir                          $(SETSNDDIR).SetSndDir
Player: crunched.Player
        squish -nolist -warn 0 -keep crunch.Keep -from crunched.Player -to $@
crunched.Player: bas.Player
        $(MKDIR) crunched
        /crunch.Player
        BASIC

Core_Encode:          Filters.DirectY \
                      Filters.SharpenY \
                      Filters.SMedian5 \
                      Filters.SMedian9 \
                      Filters.SmoothY5 \
                      Filters.SmoothY9 \
                      Filters.SmoothY91 \
                      Filters.SmoothY92 \
                      Filters.TClamp \
                      Filters.TClamp2 \
                      Filters.TClamp4 \
                      Filters.TClamp8 \
                      Tools.Extract.Extract \
                      Tools.Join4.Join \
                      crunched.CompLib
        MMD $(FILTERDIR)
        CMF Filters.DirectY                        $(FILTERDIR).DirectY
        CMF Filters.SharpenY                       $(FILTERDIR).SharpenY
        CMF Filters.SMedian5                       $(FILTERDIR).SMedian5
        CMF Filters.SMedian9                       $(FILTERDIR).SMedian9
        CMF Filters.SmoothY5                       $(FILTERDIR).SmoothY5
        CMF Filters.SmoothY9                       $(FILTERDIR).SmoothY9
        CMF Filters.SmoothY91                      $(FILTERDIR).SmoothY91
        CMF Filters.SmoothY92                      $(FILTERDIR).SmoothY92
        CMF Filters.TClamp                         $(FILTERDIR).TClamp
        CMF Filters.TClamp2                        $(FILTERDIR).TClamp2
        CMF Filters.TClamp4                        $(FILTERDIR).TClamp4
        CMF Filters.TClamp8                        $(FILTERDIR).TClamp8
        MMD $(TOOLSDIR)
        CMF Tools.Extract.Extract                  $(TOOLSDIR).Extract
        CMF Tools.Join4.Join                       $(TOOLSDIR).Join
        CMF crunched.CompLib                       $(TOOLSDIR).CompLib
Filters.DirectY: Filters.bas.DirectY
        /Filters.bas.DirectY
Filters.SharpenY: Filters.bas.SharpenY
        /Filters.bas.SharpenY
Filters.SMedian5: Filters.bas.SMedian5
        /Filters.bas.SMedian5
Filters.SMedian9: Filters.bas.SMedian9
        /Filters.bas.SMedian9
Filters.SmoothY5: Filters.bas.SmoothY5
        /Filters.bas.SmoothY5
Filters.SmoothY9: Filters.bas.SmoothY9
        /Filters.bas.SmoothY9
Filters.SmoothY91: Filters.bas.SmoothY91
        /Filters.bas.SmoothY91
Filters.SmoothY92: Filters.bas.SmoothY92
        /Filters.bas.SmoothY92
Filters.TClamp: Filters.bas.TClamp
        /Filters.bas.TClamp
Filters.TClamp2: Filters.bas.TClamp2
        /Filters.bas.TClamp2
Filters.TClamp4: Filters.bas.TClamp4
        /Filters.bas.TClamp4
Filters.TClamp8: Filters.bas.TClamp8
        /Filters.bas.TClamp8
Tools.Extract.Extract:
        @Dir Tools.Extract
        @amu_machine
        @Up 2
Tools.Join4.Join:
        @Dir Tools.Join4
        @amu_machine
        @Up 2
crunched.CompLib: bas.CompLib
        $(MKDIR) crunched
        /crunch.CompLib
        BASIC

Application_Decode:   Resources.!Sprites \
                      Resources.!Sprites22 \
                      Resources.Default \
                      Resources.Sprites \
                      Resources.Sprites22
        MMD $(ROOTDIR)
        CMF Resources.!Sprites                     $(ROOTDIR).!Sprites
        CMF Resources.!Sprites22                   $(ROOTDIR).!Sprites22
        CMF Resources.Default                      $(ROOTDIR).Default
        CMF Resources.Sprites                      $(ROOTDIR).Sprites
        CMF Resources.Sprites22                    $(ROOTDIR).Sprites22

Render_Decode:        Painters.1bpp.bas.d11x1y \
                      Painters.1bpp.bas.d12x1y \
                      Painters.1bpp.bas.d12x2y \
                      Painters.2bpp.bas.d21x1y \
                      Painters.2bpp.bas.d22x1y \
                      Painters.2bpp.bas.d22x2y \
                      Painters.4bpp.bas.d41x1y \
                      Painters.4bpp.bas.d42x1y \
                      Painters.4bpp.bas.d42x2y \
                      Painters.8bpp.bas.d81x1y \
                      Painters.8bpp.bas.d81x2y \
                      Painters.8bpp.bas.d82x1y \
                      Painters.8bpp.bas.d82x2y \
                      Painters.8bpp.bas.d82x4y \
                      Painters.16bpp.bas.d161x1y \
                      Painters.16bpp.bas.d161x2y \
                      Painters.16bpp.bas.d162x1y \
                      Painters.16bpp.bas.d162x2y \
                      Painters.16bpp.bas.d162x4y \
                      Painters.16bpp.bas.d162xI1y \
                      Painters.16bpp.bas.d162xI2y \
                      Painters.16bpp.bas.d162xI2yI \
                      Painters.16bpp.bas.d162xI4y \
                      Painters.16bpp.bas.d162xI4yI \
                      Painters.16bpp.bas.d163÷2x1y \
                      Painters.16bpp.bas.d163÷2xI1y \
                      Painters.16bpp.bas.d163÷2xIy \
                      Painters.16bpp.bas.d163÷2xIyI \
                      Painters.16bpp.bas.d163÷2xy \
                      Painters.16bpp.bas.d164÷3x1y \
                      Painters.16bpp.bas.d164÷3xI1y \
                      Painters.16bpp.bas.d165÷3x1y \
                      Painters.16bpp.bas.d165÷3xI1y \
                      Painters.16bpp.bas.d165÷4x1y \
                      Painters.16bpp.bas.d165÷4xI1y \
                      Painters.16bpp.bas.d165÷4xIy \
                      Painters.16bpp.bas.d165÷4xy \
                      Painters.16bpp.bas.d167÷4x1y \
                      Painters.16bpp.bas.d167÷4xI1y \
                      Painters.16bpp.bas.d16Anyxy \
                      Painters.32bpp.bas.CLd32s÷8 \
                      Painters.32bpp.bas.d3211÷8x1y \
                      Painters.32bpp.bas.d3211÷8xI1 \
                      Painters.32bpp.bas.d3213÷8x1y \
                      Painters.32bpp.bas.d3213÷8xI1 \
                      Painters.32bpp.bas.d3215÷8x1y \
                      Painters.32bpp.bas.d3215÷8xI1 \
                      Painters.32bpp.bas.d321x1y \
                      Painters.32bpp.bas.d321x2y \
                      Painters.32bpp.bas.d322x1y \
                      Painters.32bpp.bas.d322x2y \
                      Painters.32bpp.bas.d322x4y \
                      Painters.32bpp.bas.d322xI1y \
                      Painters.32bpp.bas.d322xI2y \
                      Painters.32bpp.bas.d322xI2yI \
                      Painters.32bpp.bas.d322xI4y \
                      Painters.32bpp.bas.d322xI4yI \
                      Painters.32bpp.bas.d323÷2x1y \
                      Painters.32bpp.bas.d323÷2xI1y \
                      Painters.32bpp.bas.d323÷2xIy \
                      Painters.32bpp.bas.d323÷2xIyI \
                      Painters.32bpp.bas.d323÷2xy \
                      Painters.32bpp.bas.d325÷4x1y \
                      Painters.32bpp.bas.d325÷4xI1y \
                      Painters.32bpp.bas.d325÷4xIy \
                      Painters.32bpp.bas.d325÷4xy \
                      Painters.32bpp.bas.d327÷4x1y \
                      Painters.32bpp.bas.d327÷4xI1y \
                      Painters.32bpp.bas.d327÷8x1y \
                      Painters.32bpp.bas.d327÷8xI1y \
                      Painters.32bpp.bas.d329÷8x1y \
                      Painters.32bpp.bas.d329÷8xI1y \
                      Painters.32bpp.bas.d32Anyxy \
                      Colour.bas.6Y5CC16 \
                      Colour.bas.6Y5CC24 \
                      Colour.bas.6Y5UV16 \
                      Colour.bas.6Y5UV24 \
                      Colour.bas.6Y6UV16 \
                      Colour.bas.C6Y5UV16 \
                      Colour.bas.C6Y5UV24 \
                      Colour.bas.C6Y6UV16 \
                      Colour.bas.CYUV16 \
                      Colour.bas.CYUV24 \
                      Colour.bas.Delete \
                      Colour.bas.MarkUnused \
                      Colour.bas.RGB16 \
                      Colour.bas.RGB8 \
                      Colour.bas.Tidy \
                      Colour.bas.ULTI16 \
                      Colour.bas.YCC24 \
                      Colour.bas.YUV16 \
                      Colour.bas.YUV24
        MMD $(PAINTDIR)
        If1bpp CMF Painters.1bpp.bas.d11x1y        $(PAINTDIR).d11x1y
        If1bpp CMF Painters.1bpp.bas.d12x1y        $(PAINTDIR).d12x1y
        If1bpp CMF Painters.1bpp.bas.d12x2y        $(PAINTDIR).d12x2y
        If2bpp CMF Painters.2bpp.bas.d21x1y        $(PAINTDIR).d21x1y
        If2bpp CMF Painters.2bpp.bas.d22x1y        $(PAINTDIR).d22x1y
        If2bpp CMF Painters.2bpp.bas.d22x2y        $(PAINTDIR).d22x2y
        If4bpp CMF Painters.4bpp.bas.d41x1y        $(PAINTDIR).d41x1y
        If4bpp CMF Painters.4bpp.bas.d42x1y        $(PAINTDIR).d42x1y
        If4bpp CMF Painters.4bpp.bas.d42x2y        $(PAINTDIR).d42x2y
        If8bpp CMF Painters.8bpp.bas.d81x1y        $(PAINTDIR).d81x1y
        If8bpp CMF Painters.8bpp.bas.d81x2y        $(PAINTDIR).d81x2y
        If8bpp CMF Painters.8bpp.bas.d82x1y        $(PAINTDIR).d82x1y
        If8bpp CMF Painters.8bpp.bas.d82x2y        $(PAINTDIR).d82x2y
        If8bpp CMF Painters.8bpp.bas.d82x4y        $(PAINTDIR).d82x4y
        If16bpp CMF Painters.16bpp.bas.d161x1y     $(PAINTDIR).d161x1y
        If16bpp CMF Painters.16bpp.bas.d161x2y     $(PAINTDIR).d161x2y
        If16bpp CMF Painters.16bpp.bas.d162x1y     $(PAINTDIR).d162x1y
        If16bpp CMF Painters.16bpp.bas.d162x2y     $(PAINTDIR).d162x2y
        If16bpp CMF Painters.16bpp.bas.d162x4y     $(PAINTDIR).d162x4y
        If16bpp CMF Painters.16bpp.bas.d162xI1y    $(PAINTDIR).d162xI1y
        If16bpp CMF Painters.16bpp.bas.d162xI2y    $(PAINTDIR).d162xI2y
        If16bpp CMF Painters.16bpp.bas.d162xI2yI   $(PAINTDIR).d162xI2yI
        If16bpp CMF Painters.16bpp.bas.d162xI4y    $(PAINTDIR).d162xI4y
        If16bpp CMF Painters.16bpp.bas.d162xI4yI   $(PAINTDIR).d162xI4yI
        If16bpp CMF Painters.16bpp.bas.d163÷2x1y   $(PAINTDIR).d163/2x1y
        If16bpp CMF Painters.16bpp.bas.d163÷2xI1y  $(PAINTDIR).d163/2xI1y
        If16bpp CMF Painters.16bpp.bas.d163÷2xIy   $(PAINTDIR).d163/2xIy
        If16bpp CMF Painters.16bpp.bas.d163÷2xIyI  $(PAINTDIR).d163/2xIyI
        If16bpp CMF Painters.16bpp.bas.d163÷2xy    $(PAINTDIR).d163/2xy
        If16bpp CMF Painters.16bpp.bas.d164÷3x1y   $(PAINTDIR).d164/3x1y
        If16bpp CMF Painters.16bpp.bas.d164÷3xI1y  $(PAINTDIR).d164/3xI1y
        If16bpp CMF Painters.16bpp.bas.d165÷3x1y   $(PAINTDIR).d165/3x1y
        If16bpp CMF Painters.16bpp.bas.d165÷3xI1y  $(PAINTDIR).d165/3xI1y
        If16bpp CMF Painters.16bpp.bas.d165÷4x1y   $(PAINTDIR).d165/4x1y
        If16bpp CMF Painters.16bpp.bas.d165÷4xI1y  $(PAINTDIR).d165/4xI1y
        If16bpp CMF Painters.16bpp.bas.d165÷4xIy   $(PAINTDIR).d165/4xIy
        If16bpp CMF Painters.16bpp.bas.d165÷4xy    $(PAINTDIR).d165/4xy
        If16bpp CMF Painters.16bpp.bas.d167÷4x1y   $(PAINTDIR).d167/4x1y
        If16bpp CMF Painters.16bpp.bas.d167÷4xI1y  $(PAINTDIR).d167/4xI1y
        If16bpp CMF Painters.16bpp.bas.d16Anyxy    $(PAINTDIR).d16Anyxy
        If32bpp MMD $(PAINTDIR).d32
        If32bpp CMF Painters.32bpp.bas.CLd32s÷8    $(PAINTDIR).d32.CLd32s/8
        If32bpp CMF Painters.32bpp.bas.d3211÷8x1y  $(PAINTDIR).d32.d3211/8x1y
        If32bpp CMF Painters.32bpp.bas.d3211÷8xI1  $(PAINTDIR).d32.d3211/8xI1
        If32bpp CMF Painters.32bpp.bas.d3213÷8x1y  $(PAINTDIR).d32.d3213/8x1y
        If32bpp CMF Painters.32bpp.bas.d3213÷8xI1  $(PAINTDIR).d32.d3213/8xI1
        If32bpp CMF Painters.32bpp.bas.d3215÷8x1y  $(PAINTDIR).d32.d3215/8x1y
        If32bpp CMF Painters.32bpp.bas.d3215÷8xI1  $(PAINTDIR).d32.d3215/8xI1
        If32bpp CMF Painters.32bpp.bas.d321x1y     $(PAINTDIR).d32.d321x1y
        If32bpp CMF Painters.32bpp.bas.d321x2y     $(PAINTDIR).d32.d321x2y
        If32bpp CMF Painters.32bpp.bas.d322x1y     $(PAINTDIR).d32.d322x1y
        If32bpp CMF Painters.32bpp.bas.d322x2y     $(PAINTDIR).d32.d322x2y
        If32bpp CMF Painters.32bpp.bas.d322x4y     $(PAINTDIR).d32.d322x4y
        If32bpp CMF Painters.32bpp.bas.d322xI1y    $(PAINTDIR).d32.d322xI1y
        If32bpp CMF Painters.32bpp.bas.d322xI2y    $(PAINTDIR).d32.d322xI2y
        If32bpp CMF Painters.32bpp.bas.d322xI2yI   $(PAINTDIR).d32.d322xI2yI
        If32bpp CMF Painters.32bpp.bas.d322xI4y    $(PAINTDIR).d32.d322xI4y
        If32bpp CMF Painters.32bpp.bas.d322xI4yI   $(PAINTDIR).d32.d322xI4yI
        If32bpp CMF Painters.32bpp.bas.d323÷2x1y   $(PAINTDIR).d32.d323/2x1y
        If32bpp CMF Painters.32bpp.bas.d323÷2xI1y  $(PAINTDIR).d32.d323/2xI1y
        If32bpp CMF Painters.32bpp.bas.d323÷2xIy   $(PAINTDIR).d32.d323/2xIy
        If32bpp CMF Painters.32bpp.bas.d323÷2xIyI  $(PAINTDIR).d32.d323/2xIyI
        If32bpp CMF Painters.32bpp.bas.d323÷2xy    $(PAINTDIR).d32.d323/2xy
        If32bpp CMF Painters.32bpp.bas.d325÷4x1y   $(PAINTDIR).d32.d325/4x1y
        If32bpp CMF Painters.32bpp.bas.d325÷4xI1y  $(PAINTDIR).d32.d325/4xI1y
        If32bpp CMF Painters.32bpp.bas.d325÷4xIy   $(PAINTDIR).d32.d325/4xIy
        If32bpp CMF Painters.32bpp.bas.d325÷4xy    $(PAINTDIR).d32.d325/4xy
        If32bpp CMF Painters.32bpp.bas.d327÷4x1y   $(PAINTDIR).d32.d327/4x1y
        If32bpp CMF Painters.32bpp.bas.d327÷4xI1y  $(PAINTDIR).d32.d327/4xI1y
        If32bpp CMF Painters.32bpp.bas.d327÷8x1y   $(PAINTDIR).d32.d327/8x1y
        If32bpp CMF Painters.32bpp.bas.d327÷8xI1y  $(PAINTDIR).d32.d327/8xI1y
        If32bpp CMF Painters.32bpp.bas.d329÷8x1y   $(PAINTDIR).d32.d329/8x1y
        If32bpp CMF Painters.32bpp.bas.d329÷8xI1y  $(PAINTDIR).d32.d329/8xI1y
        If32bpp CMF Painters.32bpp.bas.d32Anyxy    $(PAINTDIR).d32.d32Anyxy
        MMD $(COLMAPDIR)
        CMF Colour.bas.6Y5CC16                     $(COLMAPDIR).6Y5CC16
        CMF Colour.bas.6Y5CC24                     $(COLMAPDIR).6Y5CC24
        CMF Colour.bas.6Y5UV16                     $(COLMAPDIR).6Y5UV16
        CMF Colour.bas.6Y5UV24                     $(COLMAPDIR).6Y5UV24
        CMF Colour.bas.6Y6UV16                     $(COLMAPDIR).6Y6UV16
        CMF Colour.bas.C6Y5UV16                    $(COLMAPDIR).C6Y5UV16
        CMF Colour.bas.C6Y5UV24                    $(COLMAPDIR).C6Y5UV24
        CMF Colour.bas.C6Y6UV16                    $(COLMAPDIR).C6Y6UV16
        CMF Colour.bas.CYUV16                      $(COLMAPDIR).CYUV16
        CMF Colour.bas.CYUV24                      $(COLMAPDIR).CYUV24
        CMF Colour.bas.Delete                      $(COLMAPDIR).Delete
        CMF Colour.bas.MarkUnused                  $(COLMAPDIR).MarkUnused
        CMF Colour.bas.RGB16                       $(COLMAPDIR).RGB16
        CMF Colour.bas.RGB8                        $(COLMAPDIR).RGB8
        CMF Colour.bas.Tidy                        $(COLMAPDIR).Tidy
        CMF Colour.bas.ULTI16                      $(COLMAPDIR).ULTI16
        CMF Colour.bas.YCC24                       $(COLMAPDIR).YCC24
        CMF Colour.bas.YUV16                       $(COLMAPDIR).YUV16
        CMF Colour.bas.YUV24                       $(COLMAPDIR).YUV24

Render_Encode:        Colour.16yuv \
                      Colour.8rgb11 \
                      Colour.8rgb22 \
                      Colour.8yuv11 \
                      Colour.8yuv22
        MMD $(PALETTEDIR)
        CMF Colour.16yuv                           $(PALETTEDIR).16yuv
        CMF Colour.8rgb11                          $(PALETTEDIR).8rgb11
        CMF Colour.8rgb22                          $(PALETTEDIR).8rgb22
        CMF Colour.8yuv11                          $(PALETTEDIR).8yuv11
        CMF Colour.8yuv22                          $(PALETTEDIR).8yuv22
Colour.8yuv11: Colour.bas.Make8yuv11
        /Colour.bas.Make8yuv11
Colour.8yuv22: Colour.bas.Make8yuv22
        /Colour.bas.Make8yuv22

FrontEnd_Decode:      bas.FrontEnd
        MMD $(FRONTENDDIR)
        CMF bas.FrontEnd                           $(FRONTENDDIR).FrontEnd

Documents_Decode:     Resources.Documents.601vsSquar \
                      Resources.Documents.AE7doc \
                      Resources.Documents.AlienFiles \
                      Resources.Documents.CodecIf \
                      Resources.Documents.ColourIf \
                      Resources.Documents.Fetchers \
                      Resources.Documents.NewFormat2 \
                      Resources.Documents.PaintRouts \
                      Resources.Documents.PrefBig \
                      Resources.Documents.ProgIf \
                      Resources.Documents.ToUseSound
        /bas.MakeHelp LocalRes:!Help
        MMD $(ROOTDIR)
        CMF !Help                                  $(ROOTDIR).!Help
        MMD $(DOCSDIR)
        CMF Resources.Documents.601vsSquar         $(DOCSDIR).601vsSquar
        CMF Resources.Documents.AE7doc             $(DOCSDIR).AE7doc
        CMF Resources.Documents.AlienFiles         $(DOCSDIR).AlienFiles
        CMF Resources.Documents.CodecIf            $(DOCSDIR).CodecIf
        CMF Resources.Documents.ColourIf           $(DOCSDIR).ColourIf
        CMF Resources.Documents.Fetchers           $(DOCSDIR).Fetchers
        CMF Resources.Documents.NewFormat2         $(DOCSDIR).NewFormat2
        CMF Resources.Documents.PaintRouts         $(DOCSDIR).PaintRouts
        CMF Resources.Documents.PrefBig            $(DOCSDIR).PrefBig
        CMF Resources.Documents.ProgIf             $(DOCSDIR).ProgIf
        CMF Resources.Documents.ToUseSound         $(DOCSDIR).ToUseSound

Documents_Encode:     Resources.Documents.CompSound \
                      Resources.Documents.Extract \
                      Resources.Documents.Filters \
                      Resources.Documents.Join \
                      Resources.Documents.ToCapture
        CMF Resources.Documents.CompSound          $(DOCSDIR).CompSound
        CMF Resources.Documents.Extract            $(DOCSDIR).Extract
        CMF Resources.Documents.Filters            $(DOCSDIR).Filters
        CMF Resources.Documents.Join               $(DOCSDIR).Join
        CMF Resources.Documents.ToCapture          $(DOCSDIR).ToCapture

Shapes_Decode:        Resources.Shapes.Acorn.Acorn0 \
                      Resources.Shapes.Acorn.Acorn1 \
                      Resources.Shapes.Acorn.Acorn10 \
                      Resources.Shapes.Acorn.Acorn11 \
                      Resources.Shapes.Acorn.Acorn12 \
                      Resources.Shapes.Acorn.Acorn13 \
                      Resources.Shapes.Acorn.Acorn14 \
                      Resources.Shapes.Acorn.Acorn15 \
                      Resources.Shapes.Acorn.Acorn16 \
                      Resources.Shapes.Acorn.Acorn17 \
                      Resources.Shapes.Acorn.Acorn18 \
                      Resources.Shapes.Acorn.Acorn19 \
                      Resources.Shapes.Acorn.Acorn2 \
                      Resources.Shapes.Acorn.Acorn20 \
                      Resources.Shapes.Acorn.Acorn21 \
                      Resources.Shapes.Acorn.Acorn22 \
                      Resources.Shapes.Acorn.Acorn23 \
                      Resources.Shapes.Acorn.Acorn24 \
                      Resources.Shapes.Acorn.Acorn25 \
                      Resources.Shapes.Acorn.Acorn26 \
                      Resources.Shapes.Acorn.Acorn27 \
                      Resources.Shapes.Acorn.Acorn28 \
                      Resources.Shapes.Acorn.Acorn29 \
                      Resources.Shapes.Acorn.Acorn3 \
                      Resources.Shapes.Acorn.Acorn30 \
                      Resources.Shapes.Acorn.Acorn31 \
                      Resources.Shapes.Acorn.Acorn32 \
                      Resources.Shapes.Acorn.Acorn33 \
                      Resources.Shapes.Acorn.Acorn34 \
                      Resources.Shapes.Acorn.Acorn35 \
                      Resources.Shapes.Acorn.Acorn36 \
                      Resources.Shapes.Acorn.Acorn37 \
                      Resources.Shapes.Acorn.Acorn38 \
                      Resources.Shapes.Acorn.Acorn39 \
                      Resources.Shapes.Acorn.Acorn4 \
                      Resources.Shapes.Acorn.Acorn40 \
                      Resources.Shapes.Acorn.Acorn41 \
                      Resources.Shapes.Acorn.Acorn42 \
                      Resources.Shapes.Acorn.Acorn43 \
                      Resources.Shapes.Acorn.Acorn44 \
                      Resources.Shapes.Acorn.Acorn45 \
                      Resources.Shapes.Acorn.Acorn46 \
                      Resources.Shapes.Acorn.Acorn47 \
                      Resources.Shapes.Acorn.Acorn48 \
                      Resources.Shapes.Acorn.Acorn49 \
                      Resources.Shapes.Acorn.Acorn5 \
                      Resources.Shapes.Acorn.Acorn6 \
                      Resources.Shapes.Acorn.Acorn7 \
                      Resources.Shapes.Acorn.Acorn8 \
                      Resources.Shapes.Acorn.Acorn9 \
                      Resources.Shapes.Acorn2.Acorn0 \
                      Resources.Shapes.Acorn2.Acorn1 \
                      Resources.Shapes.Acorn2.Acorn10 \
                      Resources.Shapes.Acorn2.Acorn11 \
                      Resources.Shapes.Acorn2.Acorn12 \
                      Resources.Shapes.Acorn2.Acorn13 \
                      Resources.Shapes.Acorn2.Acorn14 \
                      Resources.Shapes.Acorn2.Acorn15 \
                      Resources.Shapes.Acorn2.Acorn16 \
                      Resources.Shapes.Acorn2.Acorn17 \
                      Resources.Shapes.Acorn2.Acorn18 \
                      Resources.Shapes.Acorn2.Acorn19 \
                      Resources.Shapes.Acorn2.Acorn2 \
                      Resources.Shapes.Acorn2.Acorn20 \
                      Resources.Shapes.Acorn2.Acorn21 \
                      Resources.Shapes.Acorn2.Acorn22 \
                      Resources.Shapes.Acorn2.Acorn23 \
                      Resources.Shapes.Acorn2.Acorn24 \
                      Resources.Shapes.Acorn2.Acorn25 \
                      Resources.Shapes.Acorn2.Acorn26 \
                      Resources.Shapes.Acorn2.Acorn27 \
                      Resources.Shapes.Acorn2.Acorn28 \
                      Resources.Shapes.Acorn2.Acorn29 \
                      Resources.Shapes.Acorn2.Acorn3 \
                      Resources.Shapes.Acorn2.Acorn30 \
                      Resources.Shapes.Acorn2.Acorn31 \
                      Resources.Shapes.Acorn2.Acorn32 \
                      Resources.Shapes.Acorn2.Acorn33 \
                      Resources.Shapes.Acorn2.Acorn34 \
                      Resources.Shapes.Acorn2.Acorn35 \
                      Resources.Shapes.Acorn2.Acorn36 \
                      Resources.Shapes.Acorn2.Acorn37 \
                      Resources.Shapes.Acorn2.Acorn38 \
                      Resources.Shapes.Acorn2.Acorn39 \
                      Resources.Shapes.Acorn2.Acorn4 \
                      Resources.Shapes.Acorn2.Acorn40 \
                      Resources.Shapes.Acorn2.Acorn41 \
                      Resources.Shapes.Acorn2.Acorn42 \
                      Resources.Shapes.Acorn2.Acorn43 \
                      Resources.Shapes.Acorn2.Acorn44 \
                      Resources.Shapes.Acorn2.Acorn45 \
                      Resources.Shapes.Acorn2.Acorn46 \
                      Resources.Shapes.Acorn2.Acorn47 \
                      Resources.Shapes.Acorn2.Acorn48 \
                      Resources.Shapes.Acorn2.Acorn49 \
                      Resources.Shapes.Acorn2.Acorn5 \
                      Resources.Shapes.Acorn2.Acorn6 \
                      Resources.Shapes.Acorn2.Acorn7 \
                      Resources.Shapes.Acorn2.Acorn8 \
                      Resources.Shapes.Acorn2.Acorn9 \
                      Resources.Shapes.Annulus \
                      Resources.Shapes.Bounce.Bounce00 \
                      Resources.Shapes.Bounce.Bounce01 \
                      Resources.Shapes.Bounce.Bounce02 \
                      Resources.Shapes.Bounce.Bounce03 \
                      Resources.Shapes.Bounce.Bounce04 \
                      Resources.Shapes.Bounce.Bounce05 \
                      Resources.Shapes.Bounce.Bounce06 \
                      Resources.Shapes.Bounce.Bounce07 \
                      Resources.Shapes.Bounce.Bounce08 \
                      Resources.Shapes.Bounce.Bounce09 \
                      Resources.Shapes.Bounce.Bounce10 \
                      Resources.Shapes.Bounce.Bounce11 \
                      Resources.Shapes.Bounce.Bounce12 \
                      Resources.Shapes.Bounce.Bounce13 \
                      Resources.Shapes.Bounce.Bounce14 \
                      Resources.Shapes.Bounce.Bounce15 \
                      Resources.Shapes.Bounce.Bounce16 \
                      Resources.Shapes.Bounce.Bounce17 \
                      Resources.Shapes.Bounce.Bounce18 \
                      Resources.Shapes.Bounce.Bounce19 \
                      Resources.Shapes.Bounce.Bounce20 \
                      Resources.Shapes.Bounce.Bounce21 \
                      Resources.Shapes.Bounce.Bounce22 \
                      Resources.Shapes.Bounce.Bounce23 \
                      Resources.Shapes.Bounce.Bounce24 \
                      Resources.Shapes.Bounce.Bounce25 \
                      Resources.Shapes.Bounce.Bounce26 \
                      Resources.Shapes.Bounce.Bounce27 \
                      Resources.Shapes.Bounce.Bounce28 \
                      Resources.Shapes.Bounce.Bounce29 \
                      Resources.Shapes.Bounce.Bounce30 \
                      Resources.Shapes.Bounce.Bounce31 \
                      Resources.Shapes.Bounce.Bounce32 \
                      Resources.Shapes.Bounce.Bounce33 \
                      Resources.Shapes.Bounce.Bounce34 \
                      Resources.Shapes.Bounce.Bounce35 \
                      Resources.Shapes.Bounce.Bounce36 \
                      Resources.Shapes.Bounce.Bounce37 \
                      Resources.Shapes.Bounce.Bounce38 \
                      Resources.Shapes.Bounce.Bounce39 \
                      Resources.Shapes.Bounce.Bounce40 \
                      Resources.Shapes.Bounce.Bounce41 \
                      Resources.Shapes.Bounce.Bounce42 \
                      Resources.Shapes.Bounce.Bounce43 \
                      Resources.Shapes.Bounce.Bounce44 \
                      Resources.Shapes.Bounce.Bounce45 \
                      Resources.Shapes.Bounce.Bounce46 \
                      Resources.Shapes.Bounce.Bounce47 \
                      Resources.Shapes.Bounce1.Bounce00 \
                      Resources.Shapes.Bounce1.Bounce01 \
                      Resources.Shapes.Bounce1.Bounce02 \
                      Resources.Shapes.Bounce1.Bounce03 \
                      Resources.Shapes.Bounce1.Bounce04 \
                      Resources.Shapes.Bounce1.Bounce05 \
                      Resources.Shapes.Bounce1.Bounce06 \
                      Resources.Shapes.Bounce1.Bounce07 \
                      Resources.Shapes.Bounce1.Bounce08 \
                      Resources.Shapes.Bounce1.Bounce09 \
                      Resources.Shapes.Bounce1.Bounce10 \
                      Resources.Shapes.Bounce1.Bounce11 \
                      Resources.Shapes.Bounce1.Bounce12 \
                      Resources.Shapes.Bounce1.Bounce13 \
                      Resources.Shapes.Bounce1.Bounce14 \
                      Resources.Shapes.Bounce1.Bounce15 \
                      Resources.Shapes.Bounce1.Bounce16 \
                      Resources.Shapes.Bounce1.Bounce17 \
                      Resources.Shapes.Bounce1.Bounce18 \
                      Resources.Shapes.Bounce1.Bounce19 \
                      Resources.Shapes.Bounce1.Bounce20 \
                      Resources.Shapes.Bounce1.Bounce21 \
                      Resources.Shapes.Bounce1.Bounce22 \
                      Resources.Shapes.Bounce1.Bounce23 \
                      Resources.Shapes.Bounce1.Bounce24 \
                      Resources.Shapes.Bounce1.Bounce25 \
                      Resources.Shapes.Bounce1.Bounce26 \
                      Resources.Shapes.Bounce1.Bounce27 \
                      Resources.Shapes.Bounce1.Bounce28 \
                      Resources.Shapes.Bounce1.Bounce29 \
                      Resources.Shapes.Bounce1.Bounce30 \
                      Resources.Shapes.Bounce1.Bounce31 \
                      Resources.Shapes.Bounce1.Bounce32 \
                      Resources.Shapes.Bounce1.Bounce33 \
                      Resources.Shapes.Bounce1.Bounce34 \
                      Resources.Shapes.Bounce1.Bounce35 \
                      Resources.Shapes.Bounce1.Bounce36 \
                      Resources.Shapes.Bounce1.Bounce37 \
                      Resources.Shapes.Bounce1.Bounce38 \
                      Resources.Shapes.Bounce1.Bounce39 \
                      Resources.Shapes.Bounce1.Bounce40 \
                      Resources.Shapes.Bounce1.Bounce41 \
                      Resources.Shapes.Bounce1.Bounce42 \
                      Resources.Shapes.Bounce1.Bounce43 \
                      Resources.Shapes.Bounce1.Bounce44 \
                      Resources.Shapes.Bounce1.Bounce45 \
                      Resources.Shapes.Bounce1.Bounce46 \
                      Resources.Shapes.Bounce1.Bounce47 \
                      Resources.Shapes.CinemaScop \
                      Resources.Shapes.Circle \
                      Resources.Shapes.Circles.Circle0 \
                      Resources.Shapes.Circles.Circle1 \
                      Resources.Shapes.Circles.Circle10 \
                      Resources.Shapes.Circles.Circle11 \
                      Resources.Shapes.Circles.Circle12 \
                      Resources.Shapes.Circles.Circle13 \
                      Resources.Shapes.Circles.Circle14 \
                      Resources.Shapes.Circles.Circle15 \
                      Resources.Shapes.Circles.Circle16 \
                      Resources.Shapes.Circles.Circle17 \
                      Resources.Shapes.Circles.Circle18 \
                      Resources.Shapes.Circles.Circle19 \
                      Resources.Shapes.Circles.Circle2 \
                      Resources.Shapes.Circles.Circle20 \
                      Resources.Shapes.Circles.Circle21 \
                      Resources.Shapes.Circles.Circle22 \
                      Resources.Shapes.Circles.Circle23 \
                      Resources.Shapes.Circles.Circle24 \
                      Resources.Shapes.Circles.Circle25 \
                      Resources.Shapes.Circles.Circle26 \
                      Resources.Shapes.Circles.Circle27 \
                      Resources.Shapes.Circles.Circle28 \
                      Resources.Shapes.Circles.Circle29 \
                      Resources.Shapes.Circles.Circle3 \
                      Resources.Shapes.Circles.Circle30 \
                      Resources.Shapes.Circles.Circle31 \
                      Resources.Shapes.Circles.Circle4 \
                      Resources.Shapes.Circles.Circle5 \
                      Resources.Shapes.Circles.Circle6 \
                      Resources.Shapes.Circles.Circle7 \
                      Resources.Shapes.Circles.Circle8 \
                      Resources.Shapes.Circles.Circle9 \
                      Resources.Shapes.Cube \
                      Resources.Shapes.Cubes.Cube0 \
                      Resources.Shapes.Cubes.Cube1 \
                      Resources.Shapes.Cubes.Cube10 \
                      Resources.Shapes.Cubes.Cube11 \
                      Resources.Shapes.Cubes.Cube12 \
                      Resources.Shapes.Cubes.Cube13 \
                      Resources.Shapes.Cubes.Cube14 \
                      Resources.Shapes.Cubes.Cube15 \
                      Resources.Shapes.Cubes.Cube16 \
                      Resources.Shapes.Cubes.Cube17 \
                      Resources.Shapes.Cubes.Cube18 \
                      Resources.Shapes.Cubes.Cube19 \
                      Resources.Shapes.Cubes.Cube2 \
                      Resources.Shapes.Cubes.Cube20 \
                      Resources.Shapes.Cubes.Cube21 \
                      Resources.Shapes.Cubes.Cube22 \
                      Resources.Shapes.Cubes.Cube23 \
                      Resources.Shapes.Cubes.Cube24 \
                      Resources.Shapes.Cubes.Cube25 \
                      Resources.Shapes.Cubes.Cube26 \
                      Resources.Shapes.Cubes.Cube27 \
                      Resources.Shapes.Cubes.Cube28 \
                      Resources.Shapes.Cubes.Cube29 \
                      Resources.Shapes.Cubes.Cube3 \
                      Resources.Shapes.Cubes.Cube30 \
                      Resources.Shapes.Cubes.Cube31 \
                      Resources.Shapes.Cubes.Cube4 \
                      Resources.Shapes.Cubes.Cube5 \
                      Resources.Shapes.Cubes.Cube6 \
                      Resources.Shapes.Cubes.Cube7 \
                      Resources.Shapes.Cubes.Cube8 \
                      Resources.Shapes.Cubes.Cube9 \
                      Resources.Shapes.Cubes2.Cube0 \
                      Resources.Shapes.Cubes3.Cube0 \
                      Resources.Shapes.Film.Film0 \
                      Resources.Shapes.Film.Film104 \
                      Resources.Shapes.Film.Film112 \
                      Resources.Shapes.Film.Film120 \
                      Resources.Shapes.Film.Film128 \
                      Resources.Shapes.Film.Film136 \
                      Resources.Shapes.Film.Film16 \
                      Resources.Shapes.Film.Film24 \
                      Resources.Shapes.Film.Film32 \
                      Resources.Shapes.Film.Film40 \
                      Resources.Shapes.Film.Film48 \
                      Resources.Shapes.Film.Film56 \
                      Resources.Shapes.Film.Film64 \
                      Resources.Shapes.Film.Film72 \
                      Resources.Shapes.Film.Film8 \
                      Resources.Shapes.Film.Film80 \
                      Resources.Shapes.Film.Film88 \
                      Resources.Shapes.Film.Film96 \
                      Resources.Shapes.FilmB.Film0 \
                      Resources.Shapes.FilmB.Film104 \
                      Resources.Shapes.FilmB.Film112 \
                      Resources.Shapes.FilmB.Film120 \
                      Resources.Shapes.FilmB.Film128 \
                      Resources.Shapes.FilmB.Film136 \
                      Resources.Shapes.FilmB.Film16 \
                      Resources.Shapes.FilmB.Film24 \
                      Resources.Shapes.FilmB.Film32 \
                      Resources.Shapes.FilmB.Film40 \
                      Resources.Shapes.FilmB.Film48 \
                      Resources.Shapes.FilmB.Film56 \
                      Resources.Shapes.FilmB.Film64 \
                      Resources.Shapes.FilmB.Film72 \
                      Resources.Shapes.FilmB.Film8 \
                      Resources.Shapes.FilmB.Film80 \
                      Resources.Shapes.FilmB.Film88 \
                      Resources.Shapes.FilmB.Film96 \
                      Resources.Shapes.Rot6 \
                      Resources.Shapes.Rotate \
                      Resources.Shapes.Split.Split \
                      Resources.Shapes.Swing.Swing00 \
                      Resources.Shapes.Swing.Swing01 \
                      Resources.Shapes.Swing.Swing02 \
                      Resources.Shapes.Swing.Swing03 \
                      Resources.Shapes.Swing.Swing04 \
                      Resources.Shapes.Swing.Swing05 \
                      Resources.Shapes.Swing.Swing06 \
                      Resources.Shapes.Swing.Swing07 \
                      Resources.Shapes.Swing.Swing08 \
                      Resources.Shapes.Swing.Swing09 \
                      Resources.Shapes.Swing.Swing10 \
                      Resources.Shapes.Swing.Swing11 \
                      Resources.Shapes.Swing.Swing12 \
                      Resources.Shapes.Swing.Swing13 \
                      Resources.Shapes.Swing.Swing14 \
                      Resources.Shapes.Swing.Swing15 \
                      Resources.Shapes.Swing.Swing16 \
                      Resources.Shapes.Swing.Swing17 \
                      Resources.Shapes.Swing.Swing18 \
                      Resources.Shapes.Swing.Swing19 \
                      Resources.Shapes.Swing.Swing20 \
                      Resources.Shapes.Swing.Swing21 \
                      Resources.Shapes.Swing.Swing22 \
                      Resources.Shapes.Swing.Swing23 \
                      Resources.Shapes.Swing.Swing24 \
                      Resources.Shapes.Swing.Swing25 \
                      Resources.Shapes.Swing.Swing26 \
                      Resources.Shapes.Swing.Swing27 \
                      Resources.Shapes.Swing.Swing28 \
                      Resources.Shapes.Swing.Swing29 \
                      Resources.Shapes.Swing.Swing30 \
                      Resources.Shapes.Swing.Swing31 \
                      Resources.Shapes.Swing.Swing32 \
                      Resources.Shapes.Swing.Swing33 \
                      Resources.Shapes.Swing.Swing34 \
                      Resources.Shapes.Swing.Swing35 \
                      Resources.Shapes.Swing.Swing36 \
                      Resources.Shapes.Swing.Swing37 \
                      Resources.Shapes.Swing.Swing38 \
                      Resources.Shapes.Swing.Swing39 \
                      Resources.Shapes.Swing.Swing40 \
                      Resources.Shapes.Swing.Swing41 \
                      Resources.Shapes.Swing.Swing42 \
                      Resources.Shapes.Swing.Swing43 \
                      Resources.Shapes.Swing.Swing44 \
                      Resources.Shapes.Swing.Swing45 \
                      Resources.Shapes.Swing.Swing46 \
                      Resources.Shapes.Swing.Swing47 \
                      Resources.Shapes.TallFilm.Film0 \
                      Resources.Shapes.TallFilm.Film104 \
                      Resources.Shapes.TallFilm.Film112 \
                      Resources.Shapes.TallFilm.Film120 \
                      Resources.Shapes.TallFilm.Film128 \
                      Resources.Shapes.TallFilm.Film136 \
                      Resources.Shapes.TallFilm.Film16 \
                      Resources.Shapes.TallFilm.Film24 \
                      Resources.Shapes.TallFilm.Film32 \
                      Resources.Shapes.TallFilm.Film40 \
                      Resources.Shapes.TallFilm.Film48 \
                      Resources.Shapes.TallFilm.Film56 \
                      Resources.Shapes.TallFilm.Film64 \
                      Resources.Shapes.TallFilm.Film72 \
                      Resources.Shapes.TallFilm.Film8 \
                      Resources.Shapes.TallFilm.Film80 \
                      Resources.Shapes.TallFilm.Film88 \
                      Resources.Shapes.TallFilm.Film96 \
                      Resources.Shapes.TestShape.Shape \
                      Resources.Shapes.Tree \
                      Resources.Shapes.ZoomUp.Box04 \
                      Resources.Shapes.ZoomUp.Scale04 \
                      Resources.Trajectory.Bounce \
                      Resources.Trajectory.Bounce1 \
                      Resources.Trajectory.Circle \
                      Resources.Trajectory.RollBFilm \
                      Resources.Trajectory.RollFilm \
                      Resources.Trajectory.RollTFilm \
                      Resources.Trajectory.RotAcorn \
                      Resources.Trajectory.RotAcorn2 \
                      Resources.Trajectory.ScaleUp \
                      Resources.Trajectory.SpinCircle \
                      Resources.Trajectory.SpinCube \
                      Resources.Trajectory.SpinCube2 \
                      Resources.Trajectory.SpinCube3 \
                      Resources.Trajectory.Swing \
                      Resources.Trajectory.ZoomUp
        MMD $(SHAPESDIR)
        MMD $(SHAPESDIR).Acorn
        CMF Resources.Shapes.Acorn.Acorn0          $(SHAPESDIR).Acorn.Acorn0
        CMF Resources.Shapes.Acorn.Acorn1          $(SHAPESDIR).Acorn.Acorn1
        CMF Resources.Shapes.Acorn.Acorn10         $(SHAPESDIR).Acorn.Acorn10
        CMF Resources.Shapes.Acorn.Acorn11         $(SHAPESDIR).Acorn.Acorn11
        CMF Resources.Shapes.Acorn.Acorn12         $(SHAPESDIR).Acorn.Acorn12
        CMF Resources.Shapes.Acorn.Acorn13         $(SHAPESDIR).Acorn.Acorn13
        CMF Resources.Shapes.Acorn.Acorn14         $(SHAPESDIR).Acorn.Acorn14
        CMF Resources.Shapes.Acorn.Acorn15         $(SHAPESDIR).Acorn.Acorn15
        CMF Resources.Shapes.Acorn.Acorn16         $(SHAPESDIR).Acorn.Acorn16
        CMF Resources.Shapes.Acorn.Acorn17         $(SHAPESDIR).Acorn.Acorn17
        CMF Resources.Shapes.Acorn.Acorn18         $(SHAPESDIR).Acorn.Acorn18
        CMF Resources.Shapes.Acorn.Acorn19         $(SHAPESDIR).Acorn.Acorn19
        CMF Resources.Shapes.Acorn.Acorn2          $(SHAPESDIR).Acorn.Acorn2
        CMF Resources.Shapes.Acorn.Acorn20         $(SHAPESDIR).Acorn.Acorn20
        CMF Resources.Shapes.Acorn.Acorn21         $(SHAPESDIR).Acorn.Acorn21
        CMF Resources.Shapes.Acorn.Acorn22         $(SHAPESDIR).Acorn.Acorn22
        CMF Resources.Shapes.Acorn.Acorn23         $(SHAPESDIR).Acorn.Acorn23
        CMF Resources.Shapes.Acorn.Acorn24         $(SHAPESDIR).Acorn.Acorn24
        CMF Resources.Shapes.Acorn.Acorn25         $(SHAPESDIR).Acorn.Acorn25
        CMF Resources.Shapes.Acorn.Acorn26         $(SHAPESDIR).Acorn.Acorn26
        CMF Resources.Shapes.Acorn.Acorn27         $(SHAPESDIR).Acorn.Acorn27
        CMF Resources.Shapes.Acorn.Acorn28         $(SHAPESDIR).Acorn.Acorn28
        CMF Resources.Shapes.Acorn.Acorn29         $(SHAPESDIR).Acorn.Acorn29
        CMF Resources.Shapes.Acorn.Acorn3          $(SHAPESDIR).Acorn.Acorn3
        CMF Resources.Shapes.Acorn.Acorn30         $(SHAPESDIR).Acorn.Acorn30
        CMF Resources.Shapes.Acorn.Acorn31         $(SHAPESDIR).Acorn.Acorn31
        CMF Resources.Shapes.Acorn.Acorn32         $(SHAPESDIR).Acorn.Acorn32
        CMF Resources.Shapes.Acorn.Acorn33         $(SHAPESDIR).Acorn.Acorn33
        CMF Resources.Shapes.Acorn.Acorn34         $(SHAPESDIR).Acorn.Acorn34
        CMF Resources.Shapes.Acorn.Acorn35         $(SHAPESDIR).Acorn.Acorn35
        CMF Resources.Shapes.Acorn.Acorn36         $(SHAPESDIR).Acorn.Acorn36
        CMF Resources.Shapes.Acorn.Acorn37         $(SHAPESDIR).Acorn.Acorn37
        CMF Resources.Shapes.Acorn.Acorn38         $(SHAPESDIR).Acorn.Acorn38
        CMF Resources.Shapes.Acorn.Acorn39         $(SHAPESDIR).Acorn.Acorn39
        CMF Resources.Shapes.Acorn.Acorn4          $(SHAPESDIR).Acorn.Acorn4
        CMF Resources.Shapes.Acorn.Acorn40         $(SHAPESDIR).Acorn.Acorn40
        CMF Resources.Shapes.Acorn.Acorn41         $(SHAPESDIR).Acorn.Acorn41
        CMF Resources.Shapes.Acorn.Acorn42         $(SHAPESDIR).Acorn.Acorn42
        CMF Resources.Shapes.Acorn.Acorn43         $(SHAPESDIR).Acorn.Acorn43
        CMF Resources.Shapes.Acorn.Acorn44         $(SHAPESDIR).Acorn.Acorn44
        CMF Resources.Shapes.Acorn.Acorn45         $(SHAPESDIR).Acorn.Acorn45
        CMF Resources.Shapes.Acorn.Acorn46         $(SHAPESDIR).Acorn.Acorn46
        CMF Resources.Shapes.Acorn.Acorn47         $(SHAPESDIR).Acorn.Acorn47
        CMF Resources.Shapes.Acorn.Acorn48         $(SHAPESDIR).Acorn.Acorn48
        CMF Resources.Shapes.Acorn.Acorn49         $(SHAPESDIR).Acorn.Acorn49
        CMF Resources.Shapes.Acorn.Acorn5          $(SHAPESDIR).Acorn.Acorn5
        CMF Resources.Shapes.Acorn.Acorn6          $(SHAPESDIR).Acorn.Acorn6
        CMF Resources.Shapes.Acorn.Acorn7          $(SHAPESDIR).Acorn.Acorn7
        CMF Resources.Shapes.Acorn.Acorn8          $(SHAPESDIR).Acorn.Acorn8
        CMF Resources.Shapes.Acorn.Acorn9          $(SHAPESDIR).Acorn.Acorn9
        MMD $(SHAPESDIR).Acorn2
        CMF Resources.Shapes.Acorn2.Acorn0         $(SHAPESDIR).Acorn2.Acorn0
        CMF Resources.Shapes.Acorn2.Acorn1         $(SHAPESDIR).Acorn2.Acorn1
        CMF Resources.Shapes.Acorn2.Acorn10        $(SHAPESDIR).Acorn2.Acorn10
        CMF Resources.Shapes.Acorn2.Acorn11        $(SHAPESDIR).Acorn2.Acorn11
        CMF Resources.Shapes.Acorn2.Acorn12        $(SHAPESDIR).Acorn2.Acorn12
        CMF Resources.Shapes.Acorn2.Acorn13        $(SHAPESDIR).Acorn2.Acorn13
        CMF Resources.Shapes.Acorn2.Acorn14        $(SHAPESDIR).Acorn2.Acorn14
        CMF Resources.Shapes.Acorn2.Acorn15        $(SHAPESDIR).Acorn2.Acorn15
        CMF Resources.Shapes.Acorn2.Acorn16        $(SHAPESDIR).Acorn2.Acorn16
        CMF Resources.Shapes.Acorn2.Acorn17        $(SHAPESDIR).Acorn2.Acorn17
        CMF Resources.Shapes.Acorn2.Acorn18        $(SHAPESDIR).Acorn2.Acorn18
        CMF Resources.Shapes.Acorn2.Acorn19        $(SHAPESDIR).Acorn2.Acorn19
        CMF Resources.Shapes.Acorn2.Acorn2         $(SHAPESDIR).Acorn2.Acorn2
        CMF Resources.Shapes.Acorn2.Acorn20        $(SHAPESDIR).Acorn2.Acorn20
        CMF Resources.Shapes.Acorn2.Acorn21        $(SHAPESDIR).Acorn2.Acorn21
        CMF Resources.Shapes.Acorn2.Acorn22        $(SHAPESDIR).Acorn2.Acorn22
        CMF Resources.Shapes.Acorn2.Acorn23        $(SHAPESDIR).Acorn2.Acorn23
        CMF Resources.Shapes.Acorn2.Acorn24        $(SHAPESDIR).Acorn2.Acorn24
        CMF Resources.Shapes.Acorn2.Acorn25        $(SHAPESDIR).Acorn2.Acorn25
        CMF Resources.Shapes.Acorn2.Acorn26        $(SHAPESDIR).Acorn2.Acorn26
        CMF Resources.Shapes.Acorn2.Acorn27        $(SHAPESDIR).Acorn2.Acorn27
        CMF Resources.Shapes.Acorn2.Acorn28        $(SHAPESDIR).Acorn2.Acorn28
        CMF Resources.Shapes.Acorn2.Acorn29        $(SHAPESDIR).Acorn2.Acorn29
        CMF Resources.Shapes.Acorn2.Acorn3         $(SHAPESDIR).Acorn2.Acorn3
        CMF Resources.Shapes.Acorn2.Acorn30        $(SHAPESDIR).Acorn2.Acorn30
        CMF Resources.Shapes.Acorn2.Acorn31        $(SHAPESDIR).Acorn2.Acorn31
        CMF Resources.Shapes.Acorn2.Acorn32        $(SHAPESDIR).Acorn2.Acorn32
        CMF Resources.Shapes.Acorn2.Acorn33        $(SHAPESDIR).Acorn2.Acorn33
        CMF Resources.Shapes.Acorn2.Acorn34        $(SHAPESDIR).Acorn2.Acorn34
        CMF Resources.Shapes.Acorn2.Acorn35        $(SHAPESDIR).Acorn2.Acorn35
        CMF Resources.Shapes.Acorn2.Acorn36        $(SHAPESDIR).Acorn2.Acorn36
        CMF Resources.Shapes.Acorn2.Acorn37        $(SHAPESDIR).Acorn2.Acorn37
        CMF Resources.Shapes.Acorn2.Acorn38        $(SHAPESDIR).Acorn2.Acorn38
        CMF Resources.Shapes.Acorn2.Acorn39        $(SHAPESDIR).Acorn2.Acorn39
        CMF Resources.Shapes.Acorn2.Acorn4         $(SHAPESDIR).Acorn2.Acorn4
        CMF Resources.Shapes.Acorn2.Acorn40        $(SHAPESDIR).Acorn2.Acorn40
        CMF Resources.Shapes.Acorn2.Acorn41        $(SHAPESDIR).Acorn2.Acorn41
        CMF Resources.Shapes.Acorn2.Acorn42        $(SHAPESDIR).Acorn2.Acorn42
        CMF Resources.Shapes.Acorn2.Acorn43        $(SHAPESDIR).Acorn2.Acorn43
        CMF Resources.Shapes.Acorn2.Acorn44        $(SHAPESDIR).Acorn2.Acorn44
        CMF Resources.Shapes.Acorn2.Acorn45        $(SHAPESDIR).Acorn2.Acorn45
        CMF Resources.Shapes.Acorn2.Acorn46        $(SHAPESDIR).Acorn2.Acorn46
        CMF Resources.Shapes.Acorn2.Acorn47        $(SHAPESDIR).Acorn2.Acorn47
        CMF Resources.Shapes.Acorn2.Acorn48        $(SHAPESDIR).Acorn2.Acorn48
        CMF Resources.Shapes.Acorn2.Acorn49        $(SHAPESDIR).Acorn2.Acorn49
        CMF Resources.Shapes.Acorn2.Acorn5         $(SHAPESDIR).Acorn2.Acorn5
        CMF Resources.Shapes.Acorn2.Acorn6         $(SHAPESDIR).Acorn2.Acorn6
        CMF Resources.Shapes.Acorn2.Acorn7         $(SHAPESDIR).Acorn2.Acorn7
        CMF Resources.Shapes.Acorn2.Acorn8         $(SHAPESDIR).Acorn2.Acorn8
        CMF Resources.Shapes.Acorn2.Acorn9         $(SHAPESDIR).Acorn2.Acorn9
        CMF Resources.Shapes.Annulus               $(SHAPESDIR).Annulus
        MMD $(SHAPESDIR).Bounce
        CMF Resources.Shapes.Bounce.Bounce00       $(SHAPESDIR).Bounce.Bounce00
        CMF Resources.Shapes.Bounce.Bounce01       $(SHAPESDIR).Bounce.Bounce01
        CMF Resources.Shapes.Bounce.Bounce02       $(SHAPESDIR).Bounce.Bounce02
        CMF Resources.Shapes.Bounce.Bounce03       $(SHAPESDIR).Bounce.Bounce03
        CMF Resources.Shapes.Bounce.Bounce04       $(SHAPESDIR).Bounce.Bounce04
        CMF Resources.Shapes.Bounce.Bounce05       $(SHAPESDIR).Bounce.Bounce05
        CMF Resources.Shapes.Bounce.Bounce06       $(SHAPESDIR).Bounce.Bounce06
        CMF Resources.Shapes.Bounce.Bounce07       $(SHAPESDIR).Bounce.Bounce07
        CMF Resources.Shapes.Bounce.Bounce08       $(SHAPESDIR).Bounce.Bounce08
        CMF Resources.Shapes.Bounce.Bounce09       $(SHAPESDIR).Bounce.Bounce09
        CMF Resources.Shapes.Bounce.Bounce10       $(SHAPESDIR).Bounce.Bounce10
        CMF Resources.Shapes.Bounce.Bounce11       $(SHAPESDIR).Bounce.Bounce11
        CMF Resources.Shapes.Bounce.Bounce12       $(SHAPESDIR).Bounce.Bounce12
        CMF Resources.Shapes.Bounce.Bounce13       $(SHAPESDIR).Bounce.Bounce13
        CMF Resources.Shapes.Bounce.Bounce14       $(SHAPESDIR).Bounce.Bounce14
        CMF Resources.Shapes.Bounce.Bounce15       $(SHAPESDIR).Bounce.Bounce15
        CMF Resources.Shapes.Bounce.Bounce16       $(SHAPESDIR).Bounce.Bounce16
        CMF Resources.Shapes.Bounce.Bounce17       $(SHAPESDIR).Bounce.Bounce17
        CMF Resources.Shapes.Bounce.Bounce18       $(SHAPESDIR).Bounce.Bounce18
        CMF Resources.Shapes.Bounce.Bounce19       $(SHAPESDIR).Bounce.Bounce19
        CMF Resources.Shapes.Bounce.Bounce20       $(SHAPESDIR).Bounce.Bounce20
        CMF Resources.Shapes.Bounce.Bounce21       $(SHAPESDIR).Bounce.Bounce21
        CMF Resources.Shapes.Bounce.Bounce22       $(SHAPESDIR).Bounce.Bounce22
        CMF Resources.Shapes.Bounce.Bounce23       $(SHAPESDIR).Bounce.Bounce23
        CMF Resources.Shapes.Bounce.Bounce24       $(SHAPESDIR).Bounce.Bounce24
        CMF Resources.Shapes.Bounce.Bounce25       $(SHAPESDIR).Bounce.Bounce25
        CMF Resources.Shapes.Bounce.Bounce26       $(SHAPESDIR).Bounce.Bounce26
        CMF Resources.Shapes.Bounce.Bounce27       $(SHAPESDIR).Bounce.Bounce27
        CMF Resources.Shapes.Bounce.Bounce28       $(SHAPESDIR).Bounce.Bounce28
        CMF Resources.Shapes.Bounce.Bounce29       $(SHAPESDIR).Bounce.Bounce29
        CMF Resources.Shapes.Bounce.Bounce30       $(SHAPESDIR).Bounce.Bounce30
        CMF Resources.Shapes.Bounce.Bounce31       $(SHAPESDIR).Bounce.Bounce31
        CMF Resources.Shapes.Bounce.Bounce32       $(SHAPESDIR).Bounce.Bounce32
        CMF Resources.Shapes.Bounce.Bounce33       $(SHAPESDIR).Bounce.Bounce33
        CMF Resources.Shapes.Bounce.Bounce34       $(SHAPESDIR).Bounce.Bounce34
        CMF Resources.Shapes.Bounce.Bounce35       $(SHAPESDIR).Bounce.Bounce35
        CMF Resources.Shapes.Bounce.Bounce36       $(SHAPESDIR).Bounce.Bounce36
        CMF Resources.Shapes.Bounce.Bounce37       $(SHAPESDIR).Bounce.Bounce37
        CMF Resources.Shapes.Bounce.Bounce38       $(SHAPESDIR).Bounce.Bounce38
        CMF Resources.Shapes.Bounce.Bounce39       $(SHAPESDIR).Bounce.Bounce39
        CMF Resources.Shapes.Bounce.Bounce40       $(SHAPESDIR).Bounce.Bounce40
        CMF Resources.Shapes.Bounce.Bounce41       $(SHAPESDIR).Bounce.Bounce41
        CMF Resources.Shapes.Bounce.Bounce42       $(SHAPESDIR).Bounce.Bounce42
        CMF Resources.Shapes.Bounce.Bounce43       $(SHAPESDIR).Bounce.Bounce43
        CMF Resources.Shapes.Bounce.Bounce44       $(SHAPESDIR).Bounce.Bounce44
        CMF Resources.Shapes.Bounce.Bounce45       $(SHAPESDIR).Bounce.Bounce45
        CMF Resources.Shapes.Bounce.Bounce46       $(SHAPESDIR).Bounce.Bounce46
        CMF Resources.Shapes.Bounce.Bounce47       $(SHAPESDIR).Bounce.Bounce47
        MMD $(SHAPESDIR).Bounce1
        CMF Resources.Shapes.Bounce1.Bounce00      $(SHAPESDIR).Bounce1.Bounce00
        CMF Resources.Shapes.Bounce1.Bounce01      $(SHAPESDIR).Bounce1.Bounce01
        CMF Resources.Shapes.Bounce1.Bounce02      $(SHAPESDIR).Bounce1.Bounce02
        CMF Resources.Shapes.Bounce1.Bounce03      $(SHAPESDIR).Bounce1.Bounce03
        CMF Resources.Shapes.Bounce1.Bounce04      $(SHAPESDIR).Bounce1.Bounce04
        CMF Resources.Shapes.Bounce1.Bounce05      $(SHAPESDIR).Bounce1.Bounce05
        CMF Resources.Shapes.Bounce1.Bounce06      $(SHAPESDIR).Bounce1.Bounce06
        CMF Resources.Shapes.Bounce1.Bounce07      $(SHAPESDIR).Bounce1.Bounce07
        CMF Resources.Shapes.Bounce1.Bounce08      $(SHAPESDIR).Bounce1.Bounce08
        CMF Resources.Shapes.Bounce1.Bounce09      $(SHAPESDIR).Bounce1.Bounce09
        CMF Resources.Shapes.Bounce1.Bounce10      $(SHAPESDIR).Bounce1.Bounce10
        CMF Resources.Shapes.Bounce1.Bounce11      $(SHAPESDIR).Bounce1.Bounce11
        CMF Resources.Shapes.Bounce1.Bounce12      $(SHAPESDIR).Bounce1.Bounce12
        CMF Resources.Shapes.Bounce1.Bounce13      $(SHAPESDIR).Bounce1.Bounce13
        CMF Resources.Shapes.Bounce1.Bounce14      $(SHAPESDIR).Bounce1.Bounce14
        CMF Resources.Shapes.Bounce1.Bounce15      $(SHAPESDIR).Bounce1.Bounce15
        CMF Resources.Shapes.Bounce1.Bounce16      $(SHAPESDIR).Bounce1.Bounce16
        CMF Resources.Shapes.Bounce1.Bounce17      $(SHAPESDIR).Bounce1.Bounce17
        CMF Resources.Shapes.Bounce1.Bounce18      $(SHAPESDIR).Bounce1.Bounce18
        CMF Resources.Shapes.Bounce1.Bounce19      $(SHAPESDIR).Bounce1.Bounce19
        CMF Resources.Shapes.Bounce1.Bounce20      $(SHAPESDIR).Bounce1.Bounce20
        CMF Resources.Shapes.Bounce1.Bounce21      $(SHAPESDIR).Bounce1.Bounce21
        CMF Resources.Shapes.Bounce1.Bounce22      $(SHAPESDIR).Bounce1.Bounce22
        CMF Resources.Shapes.Bounce1.Bounce23      $(SHAPESDIR).Bounce1.Bounce23
        CMF Resources.Shapes.Bounce1.Bounce24      $(SHAPESDIR).Bounce1.Bounce24
        CMF Resources.Shapes.Bounce1.Bounce25      $(SHAPESDIR).Bounce1.Bounce25
        CMF Resources.Shapes.Bounce1.Bounce26      $(SHAPESDIR).Bounce1.Bounce26
        CMF Resources.Shapes.Bounce1.Bounce27      $(SHAPESDIR).Bounce1.Bounce27
        CMF Resources.Shapes.Bounce1.Bounce28      $(SHAPESDIR).Bounce1.Bounce28
        CMF Resources.Shapes.Bounce1.Bounce29      $(SHAPESDIR).Bounce1.Bounce29
        CMF Resources.Shapes.Bounce1.Bounce30      $(SHAPESDIR).Bounce1.Bounce30
        CMF Resources.Shapes.Bounce1.Bounce31      $(SHAPESDIR).Bounce1.Bounce31
        CMF Resources.Shapes.Bounce1.Bounce32      $(SHAPESDIR).Bounce1.Bounce32
        CMF Resources.Shapes.Bounce1.Bounce33      $(SHAPESDIR).Bounce1.Bounce33
        CMF Resources.Shapes.Bounce1.Bounce34      $(SHAPESDIR).Bounce1.Bounce34
        CMF Resources.Shapes.Bounce1.Bounce35      $(SHAPESDIR).Bounce1.Bounce35
        CMF Resources.Shapes.Bounce1.Bounce36      $(SHAPESDIR).Bounce1.Bounce36
        CMF Resources.Shapes.Bounce1.Bounce37      $(SHAPESDIR).Bounce1.Bounce37
        CMF Resources.Shapes.Bounce1.Bounce38      $(SHAPESDIR).Bounce1.Bounce38
        CMF Resources.Shapes.Bounce1.Bounce39      $(SHAPESDIR).Bounce1.Bounce39
        CMF Resources.Shapes.Bounce1.Bounce40      $(SHAPESDIR).Bounce1.Bounce40
        CMF Resources.Shapes.Bounce1.Bounce41      $(SHAPESDIR).Bounce1.Bounce41
        CMF Resources.Shapes.Bounce1.Bounce42      $(SHAPESDIR).Bounce1.Bounce42
        CMF Resources.Shapes.Bounce1.Bounce43      $(SHAPESDIR).Bounce1.Bounce43
        CMF Resources.Shapes.Bounce1.Bounce44      $(SHAPESDIR).Bounce1.Bounce44
        CMF Resources.Shapes.Bounce1.Bounce45      $(SHAPESDIR).Bounce1.Bounce45
        CMF Resources.Shapes.Bounce1.Bounce46      $(SHAPESDIR).Bounce1.Bounce46
        CMF Resources.Shapes.Bounce1.Bounce47      $(SHAPESDIR).Bounce1.Bounce47
        CMF Resources.Shapes.CinemaScop            $(SHAPESDIR).CinemaScop
        CMF Resources.Shapes.Circle                $(SHAPESDIR).Circle
        MMD $(SHAPESDIR).Circles
        CMF Resources.Shapes.Circles.Circle0       $(SHAPESDIR).Circles.Circle0
        CMF Resources.Shapes.Circles.Circle1       $(SHAPESDIR).Circles.Circle1
        CMF Resources.Shapes.Circles.Circle10      $(SHAPESDIR).Circles.Circle10
        CMF Resources.Shapes.Circles.Circle11      $(SHAPESDIR).Circles.Circle11
        CMF Resources.Shapes.Circles.Circle12      $(SHAPESDIR).Circles.Circle12
        CMF Resources.Shapes.Circles.Circle13      $(SHAPESDIR).Circles.Circle13
        CMF Resources.Shapes.Circles.Circle14      $(SHAPESDIR).Circles.Circle14
        CMF Resources.Shapes.Circles.Circle15      $(SHAPESDIR).Circles.Circle15
        CMF Resources.Shapes.Circles.Circle16      $(SHAPESDIR).Circles.Circle16
        CMF Resources.Shapes.Circles.Circle17      $(SHAPESDIR).Circles.Circle17
        CMF Resources.Shapes.Circles.Circle18      $(SHAPESDIR).Circles.Circle18
        CMF Resources.Shapes.Circles.Circle19      $(SHAPESDIR).Circles.Circle19
        CMF Resources.Shapes.Circles.Circle2       $(SHAPESDIR).Circles.Circle2
        CMF Resources.Shapes.Circles.Circle20      $(SHAPESDIR).Circles.Circle20
        CMF Resources.Shapes.Circles.Circle21      $(SHAPESDIR).Circles.Circle21
        CMF Resources.Shapes.Circles.Circle22      $(SHAPESDIR).Circles.Circle22
        CMF Resources.Shapes.Circles.Circle23      $(SHAPESDIR).Circles.Circle23
        CMF Resources.Shapes.Circles.Circle24      $(SHAPESDIR).Circles.Circle24
        CMF Resources.Shapes.Circles.Circle25      $(SHAPESDIR).Circles.Circle25
        CMF Resources.Shapes.Circles.Circle26      $(SHAPESDIR).Circles.Circle26
        CMF Resources.Shapes.Circles.Circle27      $(SHAPESDIR).Circles.Circle27
        CMF Resources.Shapes.Circles.Circle28      $(SHAPESDIR).Circles.Circle28
        CMF Resources.Shapes.Circles.Circle29      $(SHAPESDIR).Circles.Circle29
        CMF Resources.Shapes.Circles.Circle3       $(SHAPESDIR).Circles.Circle3
        CMF Resources.Shapes.Circles.Circle30      $(SHAPESDIR).Circles.Circle30
        CMF Resources.Shapes.Circles.Circle31      $(SHAPESDIR).Circles.Circle31
        CMF Resources.Shapes.Circles.Circle4       $(SHAPESDIR).Circles.Circle4
        CMF Resources.Shapes.Circles.Circle5       $(SHAPESDIR).Circles.Circle5
        CMF Resources.Shapes.Circles.Circle6       $(SHAPESDIR).Circles.Circle6
        CMF Resources.Shapes.Circles.Circle7       $(SHAPESDIR).Circles.Circle7
        CMF Resources.Shapes.Circles.Circle8       $(SHAPESDIR).Circles.Circle8
        CMF Resources.Shapes.Circles.Circle9       $(SHAPESDIR).Circles.Circle9
        CMF Resources.Shapes.Cube                  $(SHAPESDIR).Cube
        MMD $(SHAPESDIR).Cubes
        CMF Resources.Shapes.Cubes.Cube0           $(SHAPESDIR).Cubes.Cube0
        CMF Resources.Shapes.Cubes.Cube1           $(SHAPESDIR).Cubes.Cube1
        CMF Resources.Shapes.Cubes.Cube10          $(SHAPESDIR).Cubes.Cube10
        CMF Resources.Shapes.Cubes.Cube11          $(SHAPESDIR).Cubes.Cube11
        CMF Resources.Shapes.Cubes.Cube12          $(SHAPESDIR).Cubes.Cube12
        CMF Resources.Shapes.Cubes.Cube13          $(SHAPESDIR).Cubes.Cube13
        CMF Resources.Shapes.Cubes.Cube14          $(SHAPESDIR).Cubes.Cube14
        CMF Resources.Shapes.Cubes.Cube15          $(SHAPESDIR).Cubes.Cube15
        CMF Resources.Shapes.Cubes.Cube16          $(SHAPESDIR).Cubes.Cube16
        CMF Resources.Shapes.Cubes.Cube17          $(SHAPESDIR).Cubes.Cube17
        CMF Resources.Shapes.Cubes.Cube18          $(SHAPESDIR).Cubes.Cube18
        CMF Resources.Shapes.Cubes.Cube19          $(SHAPESDIR).Cubes.Cube19
        CMF Resources.Shapes.Cubes.Cube2           $(SHAPESDIR).Cubes.Cube2
        CMF Resources.Shapes.Cubes.Cube20          $(SHAPESDIR).Cubes.Cube20
        CMF Resources.Shapes.Cubes.Cube21          $(SHAPESDIR).Cubes.Cube21
        CMF Resources.Shapes.Cubes.Cube22          $(SHAPESDIR).Cubes.Cube22
        CMF Resources.Shapes.Cubes.Cube23          $(SHAPESDIR).Cubes.Cube23
        CMF Resources.Shapes.Cubes.Cube24          $(SHAPESDIR).Cubes.Cube24
        CMF Resources.Shapes.Cubes.Cube25          $(SHAPESDIR).Cubes.Cube25
        CMF Resources.Shapes.Cubes.Cube26          $(SHAPESDIR).Cubes.Cube26
        CMF Resources.Shapes.Cubes.Cube27          $(SHAPESDIR).Cubes.Cube27
        CMF Resources.Shapes.Cubes.Cube28          $(SHAPESDIR).Cubes.Cube28
        CMF Resources.Shapes.Cubes.Cube29          $(SHAPESDIR).Cubes.Cube29
        CMF Resources.Shapes.Cubes.Cube3           $(SHAPESDIR).Cubes.Cube3
        CMF Resources.Shapes.Cubes.Cube30          $(SHAPESDIR).Cubes.Cube30
        CMF Resources.Shapes.Cubes.Cube31          $(SHAPESDIR).Cubes.Cube31
        CMF Resources.Shapes.Cubes.Cube4           $(SHAPESDIR).Cubes.Cube4
        CMF Resources.Shapes.Cubes.Cube5           $(SHAPESDIR).Cubes.Cube5
        CMF Resources.Shapes.Cubes.Cube6           $(SHAPESDIR).Cubes.Cube6
        CMF Resources.Shapes.Cubes.Cube7           $(SHAPESDIR).Cubes.Cube7
        CMF Resources.Shapes.Cubes.Cube8           $(SHAPESDIR).Cubes.Cube8
        CMF Resources.Shapes.Cubes.Cube9           $(SHAPESDIR).Cubes.Cube9
        MMD $(SHAPESDIR).Cubes2
        CMF Resources.Shapes.Cubes2.Cube0          $(SHAPESDIR).Cubes2.Cube0
        CMF Resources.Shapes.Cubes2.Cube1          $(SHAPESDIR).Cubes2.Cube1
        CMF Resources.Shapes.Cubes2.Cube10         $(SHAPESDIR).Cubes2.Cube10
        CMF Resources.Shapes.Cubes2.Cube11         $(SHAPESDIR).Cubes2.Cube11
        CMF Resources.Shapes.Cubes2.Cube12         $(SHAPESDIR).Cubes2.Cube12
        CMF Resources.Shapes.Cubes2.Cube13         $(SHAPESDIR).Cubes2.Cube13
        CMF Resources.Shapes.Cubes2.Cube14         $(SHAPESDIR).Cubes2.Cube14
        CMF Resources.Shapes.Cubes2.Cube15         $(SHAPESDIR).Cubes2.Cube15
        CMF Resources.Shapes.Cubes2.Cube16         $(SHAPESDIR).Cubes2.Cube16
        CMF Resources.Shapes.Cubes2.Cube17         $(SHAPESDIR).Cubes2.Cube17
        CMF Resources.Shapes.Cubes2.Cube18         $(SHAPESDIR).Cubes2.Cube18
        CMF Resources.Shapes.Cubes2.Cube19         $(SHAPESDIR).Cubes2.Cube19
        CMF Resources.Shapes.Cubes2.Cube2          $(SHAPESDIR).Cubes2.Cube2
        CMF Resources.Shapes.Cubes2.Cube20         $(SHAPESDIR).Cubes2.Cube20
        CMF Resources.Shapes.Cubes2.Cube21         $(SHAPESDIR).Cubes2.Cube21
        CMF Resources.Shapes.Cubes2.Cube22         $(SHAPESDIR).Cubes2.Cube22
        CMF Resources.Shapes.Cubes2.Cube23         $(SHAPESDIR).Cubes2.Cube23
        CMF Resources.Shapes.Cubes2.Cube24         $(SHAPESDIR).Cubes2.Cube24
        CMF Resources.Shapes.Cubes2.Cube25         $(SHAPESDIR).Cubes2.Cube25
        CMF Resources.Shapes.Cubes2.Cube26         $(SHAPESDIR).Cubes2.Cube26
        CMF Resources.Shapes.Cubes2.Cube27         $(SHAPESDIR).Cubes2.Cube27
        CMF Resources.Shapes.Cubes2.Cube28         $(SHAPESDIR).Cubes2.Cube28
        CMF Resources.Shapes.Cubes2.Cube29         $(SHAPESDIR).Cubes2.Cube29
        CMF Resources.Shapes.Cubes2.Cube3          $(SHAPESDIR).Cubes2.Cube3
        CMF Resources.Shapes.Cubes2.Cube30         $(SHAPESDIR).Cubes2.Cube30
        CMF Resources.Shapes.Cubes2.Cube31         $(SHAPESDIR).Cubes2.Cube31
        CMF Resources.Shapes.Cubes2.Cube32         $(SHAPESDIR).Cubes2.Cube32
        CMF Resources.Shapes.Cubes2.Cube33         $(SHAPESDIR).Cubes2.Cube33
        CMF Resources.Shapes.Cubes2.Cube34         $(SHAPESDIR).Cubes2.Cube34
        CMF Resources.Shapes.Cubes2.Cube35         $(SHAPESDIR).Cubes2.Cube35
        CMF Resources.Shapes.Cubes2.Cube36         $(SHAPESDIR).Cubes2.Cube36
        CMF Resources.Shapes.Cubes2.Cube37         $(SHAPESDIR).Cubes2.Cube37
        CMF Resources.Shapes.Cubes2.Cube38         $(SHAPESDIR).Cubes2.Cube38
        CMF Resources.Shapes.Cubes2.Cube39         $(SHAPESDIR).Cubes2.Cube39
        CMF Resources.Shapes.Cubes2.Cube4          $(SHAPESDIR).Cubes2.Cube4
        CMF Resources.Shapes.Cubes2.Cube40         $(SHAPESDIR).Cubes2.Cube40
        CMF Resources.Shapes.Cubes2.Cube41         $(SHAPESDIR).Cubes2.Cube41
        CMF Resources.Shapes.Cubes2.Cube42         $(SHAPESDIR).Cubes2.Cube42
        CMF Resources.Shapes.Cubes2.Cube43         $(SHAPESDIR).Cubes2.Cube43
        CMF Resources.Shapes.Cubes2.Cube44         $(SHAPESDIR).Cubes2.Cube44
        CMF Resources.Shapes.Cubes2.Cube45         $(SHAPESDIR).Cubes2.Cube45
        CMF Resources.Shapes.Cubes2.Cube46         $(SHAPESDIR).Cubes2.Cube46
        CMF Resources.Shapes.Cubes2.Cube47         $(SHAPESDIR).Cubes2.Cube47
        CMF Resources.Shapes.Cubes2.Cube48         $(SHAPESDIR).Cubes2.Cube48
        CMF Resources.Shapes.Cubes2.Cube49         $(SHAPESDIR).Cubes2.Cube49
        CMF Resources.Shapes.Cubes2.Cube5          $(SHAPESDIR).Cubes2.Cube5
        CMF Resources.Shapes.Cubes2.Cube50         $(SHAPESDIR).Cubes2.Cube50
        CMF Resources.Shapes.Cubes2.Cube51         $(SHAPESDIR).Cubes2.Cube51
        CMF Resources.Shapes.Cubes2.Cube52         $(SHAPESDIR).Cubes2.Cube52
        CMF Resources.Shapes.Cubes2.Cube53         $(SHAPESDIR).Cubes2.Cube53
        CMF Resources.Shapes.Cubes2.Cube54         $(SHAPESDIR).Cubes2.Cube54
        CMF Resources.Shapes.Cubes2.Cube55         $(SHAPESDIR).Cubes2.Cube55
        CMF Resources.Shapes.Cubes2.Cube56         $(SHAPESDIR).Cubes2.Cube56
        CMF Resources.Shapes.Cubes2.Cube57         $(SHAPESDIR).Cubes2.Cube57
        CMF Resources.Shapes.Cubes2.Cube58         $(SHAPESDIR).Cubes2.Cube58
        CMF Resources.Shapes.Cubes2.Cube59         $(SHAPESDIR).Cubes2.Cube59
        CMF Resources.Shapes.Cubes2.Cube6          $(SHAPESDIR).Cubes2.Cube6
        CMF Resources.Shapes.Cubes2.Cube60         $(SHAPESDIR).Cubes2.Cube60
        CMF Resources.Shapes.Cubes2.Cube61         $(SHAPESDIR).Cubes2.Cube61
        CMF Resources.Shapes.Cubes2.Cube62         $(SHAPESDIR).Cubes2.Cube62
        CMF Resources.Shapes.Cubes2.Cube63         $(SHAPESDIR).Cubes2.Cube63
        CMF Resources.Shapes.Cubes2.Cube7          $(SHAPESDIR).Cubes2.Cube7
        CMF Resources.Shapes.Cubes2.Cube8          $(SHAPESDIR).Cubes2.Cube8
        CMF Resources.Shapes.Cubes2.Cube9          $(SHAPESDIR).Cubes2.Cube9
        MMD $(SHAPESDIR).Cubes3
        CMF Resources.Shapes.Cubes3.Cube0          $(SHAPESDIR).Cubes3.Cube0
        CMF Resources.Shapes.Cubes3.Cube1          $(SHAPESDIR).Cubes3.Cube1
        CMF Resources.Shapes.Cubes3.Cube10         $(SHAPESDIR).Cubes3.Cube10
        CMF Resources.Shapes.Cubes3.Cube11         $(SHAPESDIR).Cubes3.Cube11
        CMF Resources.Shapes.Cubes3.Cube12         $(SHAPESDIR).Cubes3.Cube12
        CMF Resources.Shapes.Cubes3.Cube13         $(SHAPESDIR).Cubes3.Cube13
        CMF Resources.Shapes.Cubes3.Cube14         $(SHAPESDIR).Cubes3.Cube14
        CMF Resources.Shapes.Cubes3.Cube15         $(SHAPESDIR).Cubes3.Cube15
        CMF Resources.Shapes.Cubes3.Cube16         $(SHAPESDIR).Cubes3.Cube16
        CMF Resources.Shapes.Cubes3.Cube17         $(SHAPESDIR).Cubes3.Cube17
        CMF Resources.Shapes.Cubes3.Cube18         $(SHAPESDIR).Cubes3.Cube18
        CMF Resources.Shapes.Cubes3.Cube19         $(SHAPESDIR).Cubes3.Cube19
        CMF Resources.Shapes.Cubes3.Cube2          $(SHAPESDIR).Cubes3.Cube2
        CMF Resources.Shapes.Cubes3.Cube20         $(SHAPESDIR).Cubes3.Cube20
        CMF Resources.Shapes.Cubes3.Cube21         $(SHAPESDIR).Cubes3.Cube21
        CMF Resources.Shapes.Cubes3.Cube22         $(SHAPESDIR).Cubes3.Cube22
        CMF Resources.Shapes.Cubes3.Cube23         $(SHAPESDIR).Cubes3.Cube23
        CMF Resources.Shapes.Cubes3.Cube24         $(SHAPESDIR).Cubes3.Cube24
        CMF Resources.Shapes.Cubes3.Cube25         $(SHAPESDIR).Cubes3.Cube25
        CMF Resources.Shapes.Cubes3.Cube26         $(SHAPESDIR).Cubes3.Cube26
        CMF Resources.Shapes.Cubes3.Cube27         $(SHAPESDIR).Cubes3.Cube27
        CMF Resources.Shapes.Cubes3.Cube28         $(SHAPESDIR).Cubes3.Cube28
        CMF Resources.Shapes.Cubes3.Cube29         $(SHAPESDIR).Cubes3.Cube29
        CMF Resources.Shapes.Cubes3.Cube3          $(SHAPESDIR).Cubes3.Cube3
        CMF Resources.Shapes.Cubes3.Cube30         $(SHAPESDIR).Cubes3.Cube30
        CMF Resources.Shapes.Cubes3.Cube31         $(SHAPESDIR).Cubes3.Cube31
        CMF Resources.Shapes.Cubes3.Cube32         $(SHAPESDIR).Cubes3.Cube32
        CMF Resources.Shapes.Cubes3.Cube33         $(SHAPESDIR).Cubes3.Cube33
        CMF Resources.Shapes.Cubes3.Cube34         $(SHAPESDIR).Cubes3.Cube34
        CMF Resources.Shapes.Cubes3.Cube35         $(SHAPESDIR).Cubes3.Cube35
        CMF Resources.Shapes.Cubes3.Cube36         $(SHAPESDIR).Cubes3.Cube36
        CMF Resources.Shapes.Cubes3.Cube37         $(SHAPESDIR).Cubes3.Cube37
        CMF Resources.Shapes.Cubes3.Cube38         $(SHAPESDIR).Cubes3.Cube38
        CMF Resources.Shapes.Cubes3.Cube39         $(SHAPESDIR).Cubes3.Cube39
        CMF Resources.Shapes.Cubes3.Cube4          $(SHAPESDIR).Cubes3.Cube4
        CMF Resources.Shapes.Cubes3.Cube40         $(SHAPESDIR).Cubes3.Cube40
        CMF Resources.Shapes.Cubes3.Cube41         $(SHAPESDIR).Cubes3.Cube41
        CMF Resources.Shapes.Cubes3.Cube42         $(SHAPESDIR).Cubes3.Cube42
        CMF Resources.Shapes.Cubes3.Cube43         $(SHAPESDIR).Cubes3.Cube43
        CMF Resources.Shapes.Cubes3.Cube44         $(SHAPESDIR).Cubes3.Cube44
        CMF Resources.Shapes.Cubes3.Cube45         $(SHAPESDIR).Cubes3.Cube45
        CMF Resources.Shapes.Cubes3.Cube46         $(SHAPESDIR).Cubes3.Cube46
        CMF Resources.Shapes.Cubes3.Cube47         $(SHAPESDIR).Cubes3.Cube47
        CMF Resources.Shapes.Cubes3.Cube48         $(SHAPESDIR).Cubes3.Cube48
        CMF Resources.Shapes.Cubes3.Cube49         $(SHAPESDIR).Cubes3.Cube49
        CMF Resources.Shapes.Cubes3.Cube5          $(SHAPESDIR).Cubes3.Cube5
        CMF Resources.Shapes.Cubes3.Cube50         $(SHAPESDIR).Cubes3.Cube50
        CMF Resources.Shapes.Cubes3.Cube51         $(SHAPESDIR).Cubes3.Cube51
        CMF Resources.Shapes.Cubes3.Cube52         $(SHAPESDIR).Cubes3.Cube52
        CMF Resources.Shapes.Cubes3.Cube53         $(SHAPESDIR).Cubes3.Cube53
        CMF Resources.Shapes.Cubes3.Cube54         $(SHAPESDIR).Cubes3.Cube54
        CMF Resources.Shapes.Cubes3.Cube55         $(SHAPESDIR).Cubes3.Cube55
        CMF Resources.Shapes.Cubes3.Cube56         $(SHAPESDIR).Cubes3.Cube56
        CMF Resources.Shapes.Cubes3.Cube57         $(SHAPESDIR).Cubes3.Cube57
        CMF Resources.Shapes.Cubes3.Cube58         $(SHAPESDIR).Cubes3.Cube58
        CMF Resources.Shapes.Cubes3.Cube59         $(SHAPESDIR).Cubes3.Cube59
        CMF Resources.Shapes.Cubes3.Cube6          $(SHAPESDIR).Cubes3.Cube6
        CMF Resources.Shapes.Cubes3.Cube60         $(SHAPESDIR).Cubes3.Cube60
        CMF Resources.Shapes.Cubes3.Cube61         $(SHAPESDIR).Cubes3.Cube61
        CMF Resources.Shapes.Cubes3.Cube62         $(SHAPESDIR).Cubes3.Cube62
        CMF Resources.Shapes.Cubes3.Cube63         $(SHAPESDIR).Cubes3.Cube63
        CMF Resources.Shapes.Cubes3.Cube7          $(SHAPESDIR).Cubes3.Cube7
        CMF Resources.Shapes.Cubes3.Cube8          $(SHAPESDIR).Cubes3.Cube8
        CMF Resources.Shapes.Cubes3.Cube9          $(SHAPESDIR).Cubes3.Cube9
        MMD $(SHAPESDIR).Film
        CMF Resources.Shapes.Film.Film0            $(SHAPESDIR).Film.Film0
        CMF Resources.Shapes.Film.Film104          $(SHAPESDIR).Film.Film104
        CMF Resources.Shapes.Film.Film112          $(SHAPESDIR).Film.Film112
        CMF Resources.Shapes.Film.Film120          $(SHAPESDIR).Film.Film120
        CMF Resources.Shapes.Film.Film128          $(SHAPESDIR).Film.Film128
        CMF Resources.Shapes.Film.Film136          $(SHAPESDIR).Film.Film136
        CMF Resources.Shapes.Film.Film16           $(SHAPESDIR).Film.Film16
        CMF Resources.Shapes.Film.Film24           $(SHAPESDIR).Film.Film24
        CMF Resources.Shapes.Film.Film32           $(SHAPESDIR).Film.Film32
        CMF Resources.Shapes.Film.Film40           $(SHAPESDIR).Film.Film40
        CMF Resources.Shapes.Film.Film48           $(SHAPESDIR).Film.Film48
        CMF Resources.Shapes.Film.Film56           $(SHAPESDIR).Film.Film56
        CMF Resources.Shapes.Film.Film64           $(SHAPESDIR).Film.Film64
        CMF Resources.Shapes.Film.Film72           $(SHAPESDIR).Film.Film72
        CMF Resources.Shapes.Film.Film8            $(SHAPESDIR).Film.Film8
        CMF Resources.Shapes.Film.Film80           $(SHAPESDIR).Film.Film80
        CMF Resources.Shapes.Film.Film88           $(SHAPESDIR).Film.Film88
        CMF Resources.Shapes.Film.Film96           $(SHAPESDIR).Film.Film96
        MMD $(SHAPESDIR).FilmB
        CMF Resources.Shapes.FilmB.Film0           $(SHAPESDIR).FilmB.Film0
        CMF Resources.Shapes.FilmB.Film104         $(SHAPESDIR).FilmB.Film104
        CMF Resources.Shapes.FilmB.Film112         $(SHAPESDIR).FilmB.Film112
        CMF Resources.Shapes.FilmB.Film120         $(SHAPESDIR).FilmB.Film120
        CMF Resources.Shapes.FilmB.Film128         $(SHAPESDIR).FilmB.Film128
        CMF Resources.Shapes.FilmB.Film136         $(SHAPESDIR).FilmB.Film136
        CMF Resources.Shapes.FilmB.Film16          $(SHAPESDIR).FilmB.Film16
        CMF Resources.Shapes.FilmB.Film24          $(SHAPESDIR).FilmB.Film24
        CMF Resources.Shapes.FilmB.Film32          $(SHAPESDIR).FilmB.Film32
        CMF Resources.Shapes.FilmB.Film40          $(SHAPESDIR).FilmB.Film40
        CMF Resources.Shapes.FilmB.Film48          $(SHAPESDIR).FilmB.Film48
        CMF Resources.Shapes.FilmB.Film56          $(SHAPESDIR).FilmB.Film56
        CMF Resources.Shapes.FilmB.Film64          $(SHAPESDIR).FilmB.Film64
        CMF Resources.Shapes.FilmB.Film72          $(SHAPESDIR).FilmB.Film72
        CMF Resources.Shapes.FilmB.Film8           $(SHAPESDIR).FilmB.Film8
        CMF Resources.Shapes.FilmB.Film80          $(SHAPESDIR).FilmB.Film80
        CMF Resources.Shapes.FilmB.Film88          $(SHAPESDIR).FilmB.Film88
        CMF Resources.Shapes.FilmB.Film96          $(SHAPESDIR).FilmB.Film96
        CMF Resources.Shapes.Rot6                  $(SHAPESDIR).Rot6
        CMF Resources.Shapes.Rotate                $(SHAPESDIR).Rotate
        CMF Resources.Shapes.Split.Split           $(SHAPESDIR).Split
        MMD $(SHAPESDIR).Swing
        CMF Resources.Shapes.Swing.Swing00         $(SHAPESDIR).Swing.Swing00
        CMF Resources.Shapes.Swing.Swing01         $(SHAPESDIR).Swing.Swing01
        CMF Resources.Shapes.Swing.Swing02         $(SHAPESDIR).Swing.Swing02
        CMF Resources.Shapes.Swing.Swing03         $(SHAPESDIR).Swing.Swing03
        CMF Resources.Shapes.Swing.Swing04         $(SHAPESDIR).Swing.Swing04
        CMF Resources.Shapes.Swing.Swing05         $(SHAPESDIR).Swing.Swing05
        CMF Resources.Shapes.Swing.Swing06         $(SHAPESDIR).Swing.Swing06
        CMF Resources.Shapes.Swing.Swing07         $(SHAPESDIR).Swing.Swing07
        CMF Resources.Shapes.Swing.Swing08         $(SHAPESDIR).Swing.Swing08
        CMF Resources.Shapes.Swing.Swing09         $(SHAPESDIR).Swing.Swing09
        CMF Resources.Shapes.Swing.Swing10         $(SHAPESDIR).Swing.Swing10
        CMF Resources.Shapes.Swing.Swing11         $(SHAPESDIR).Swing.Swing11
        CMF Resources.Shapes.Swing.Swing12         $(SHAPESDIR).Swing.Swing12
        CMF Resources.Shapes.Swing.Swing13         $(SHAPESDIR).Swing.Swing13
        CMF Resources.Shapes.Swing.Swing14         $(SHAPESDIR).Swing.Swing14
        CMF Resources.Shapes.Swing.Swing15         $(SHAPESDIR).Swing.Swing15
        CMF Resources.Shapes.Swing.Swing16         $(SHAPESDIR).Swing.Swing16
        CMF Resources.Shapes.Swing.Swing17         $(SHAPESDIR).Swing.Swing17
        CMF Resources.Shapes.Swing.Swing18         $(SHAPESDIR).Swing.Swing18
        CMF Resources.Shapes.Swing.Swing19         $(SHAPESDIR).Swing.Swing19
        CMF Resources.Shapes.Swing.Swing20         $(SHAPESDIR).Swing.Swing20
        CMF Resources.Shapes.Swing.Swing21         $(SHAPESDIR).Swing.Swing21
        CMF Resources.Shapes.Swing.Swing22         $(SHAPESDIR).Swing.Swing22
        CMF Resources.Shapes.Swing.Swing23         $(SHAPESDIR).Swing.Swing23
        CMF Resources.Shapes.Swing.Swing24         $(SHAPESDIR).Swing.Swing24
        CMF Resources.Shapes.Swing.Swing25         $(SHAPESDIR).Swing.Swing25
        CMF Resources.Shapes.Swing.Swing26         $(SHAPESDIR).Swing.Swing26
        CMF Resources.Shapes.Swing.Swing27         $(SHAPESDIR).Swing.Swing27
        CMF Resources.Shapes.Swing.Swing28         $(SHAPESDIR).Swing.Swing28
        CMF Resources.Shapes.Swing.Swing29         $(SHAPESDIR).Swing.Swing29
        CMF Resources.Shapes.Swing.Swing30         $(SHAPESDIR).Swing.Swing30
        CMF Resources.Shapes.Swing.Swing31         $(SHAPESDIR).Swing.Swing31
        CMF Resources.Shapes.Swing.Swing32         $(SHAPESDIR).Swing.Swing32
        CMF Resources.Shapes.Swing.Swing33         $(SHAPESDIR).Swing.Swing33
        CMF Resources.Shapes.Swing.Swing34         $(SHAPESDIR).Swing.Swing34
        CMF Resources.Shapes.Swing.Swing35         $(SHAPESDIR).Swing.Swing35
        CMF Resources.Shapes.Swing.Swing36         $(SHAPESDIR).Swing.Swing36
        CMF Resources.Shapes.Swing.Swing37         $(SHAPESDIR).Swing.Swing37
        CMF Resources.Shapes.Swing.Swing38         $(SHAPESDIR).Swing.Swing38
        CMF Resources.Shapes.Swing.Swing39         $(SHAPESDIR).Swing.Swing39
        CMF Resources.Shapes.Swing.Swing40         $(SHAPESDIR).Swing.Swing40
        CMF Resources.Shapes.Swing.Swing41         $(SHAPESDIR).Swing.Swing41
        CMF Resources.Shapes.Swing.Swing42         $(SHAPESDIR).Swing.Swing42
        CMF Resources.Shapes.Swing.Swing43         $(SHAPESDIR).Swing.Swing43
        CMF Resources.Shapes.Swing.Swing44         $(SHAPESDIR).Swing.Swing44
        CMF Resources.Shapes.Swing.Swing45         $(SHAPESDIR).Swing.Swing45
        CMF Resources.Shapes.Swing.Swing46         $(SHAPESDIR).Swing.Swing46
        CMF Resources.Shapes.Swing.Swing47         $(SHAPESDIR).Swing.Swing47
        MMD $(SHAPESDIR).TallFilm
        CMF Resources.Shapes.TallFilm.Film0        $(SHAPESDIR).TallFilm.Film0
        CMF Resources.Shapes.TallFilm.Film104      $(SHAPESDIR).TallFilm.Film104
        CMF Resources.Shapes.TallFilm.Film112      $(SHAPESDIR).TallFilm.Film112
        CMF Resources.Shapes.TallFilm.Film120      $(SHAPESDIR).TallFilm.Film120
        CMF Resources.Shapes.TallFilm.Film128      $(SHAPESDIR).TallFilm.Film128
        CMF Resources.Shapes.TallFilm.Film136      $(SHAPESDIR).TallFilm.Film136
        CMF Resources.Shapes.TallFilm.Film16       $(SHAPESDIR).TallFilm.Film16
        CMF Resources.Shapes.TallFilm.Film24       $(SHAPESDIR).TallFilm.Film24
        CMF Resources.Shapes.TallFilm.Film32       $(SHAPESDIR).TallFilm.Film32
        CMF Resources.Shapes.TallFilm.Film40       $(SHAPESDIR).TallFilm.Film40
        CMF Resources.Shapes.TallFilm.Film48       $(SHAPESDIR).TallFilm.Film48
        CMF Resources.Shapes.TallFilm.Film56       $(SHAPESDIR).TallFilm.Film56
        CMF Resources.Shapes.TallFilm.Film64       $(SHAPESDIR).TallFilm.Film64
        CMF Resources.Shapes.TallFilm.Film72       $(SHAPESDIR).TallFilm.Film72
        CMF Resources.Shapes.TallFilm.Film8        $(SHAPESDIR).TallFilm.Film8
        CMF Resources.Shapes.TallFilm.Film80       $(SHAPESDIR).TallFilm.Film80
        CMF Resources.Shapes.TallFilm.Film88       $(SHAPESDIR).TallFilm.Film88
        CMF Resources.Shapes.TallFilm.Film96       $(SHAPESDIR).TallFilm.Film96
        CMF Resources.Shapes.TestShape.Shape       $(SHAPESDIR).TestShape
        CMF Resources.Shapes.Tree                  $(SHAPESDIR).Tree
        MMD $(SHAPESDIR).ZoomUp
        CMF Resources.Shapes.ZoomUp.Box04          $(SHAPESDIR).ZoomUp.Box04
        CMF Resources.Shapes.ZoomUp.Box08          $(SHAPESDIR).ZoomUp.Box08
        CMF Resources.Shapes.ZoomUp.Box12          $(SHAPESDIR).ZoomUp.Box12
        CMF Resources.Shapes.ZoomUp.Box16          $(SHAPESDIR).ZoomUp.Box16
        CMF Resources.Shapes.ZoomUp.Box20          $(SHAPESDIR).ZoomUp.Box20
        CMF Resources.Shapes.ZoomUp.Box24          $(SHAPESDIR).ZoomUp.Box24
        CMF Resources.Shapes.ZoomUp.Box28          $(SHAPESDIR).ZoomUp.Box28
        CMF Resources.Shapes.ZoomUp.Box32          $(SHAPESDIR).ZoomUp.Box32
        CMF Resources.Shapes.ZoomUp.Box36          $(SHAPESDIR).ZoomUp.Box36
        CMF Resources.Shapes.ZoomUp.Box40          $(SHAPESDIR).ZoomUp.Box40
        CMF Resources.Shapes.ZoomUp.Box44          $(SHAPESDIR).ZoomUp.Box44
        CMF Resources.Shapes.ZoomUp.Box48          $(SHAPESDIR).ZoomUp.Box48
        CMF Resources.Shapes.ZoomUp.Box52          $(SHAPESDIR).ZoomUp.Box52
        CMF Resources.Shapes.ZoomUp.Box56          $(SHAPESDIR).ZoomUp.Box56
        CMF Resources.Shapes.ZoomUp.Box60          $(SHAPESDIR).ZoomUp.Box60
        CMF Resources.Shapes.ZoomUp.Scale04        $(SHAPESDIR).ZoomUp.Scale04
        CMF Resources.Shapes.ZoomUp.Scale08        $(SHAPESDIR).ZoomUp.Scale08
        CMF Resources.Shapes.ZoomUp.Scale12        $(SHAPESDIR).ZoomUp.Scale12
        CMF Resources.Shapes.ZoomUp.Scale16        $(SHAPESDIR).ZoomUp.Scale16
        CMF Resources.Shapes.ZoomUp.Scale20        $(SHAPESDIR).ZoomUp.Scale20
        CMF Resources.Shapes.ZoomUp.Scale24        $(SHAPESDIR).ZoomUp.Scale24
        CMF Resources.Shapes.ZoomUp.Scale28        $(SHAPESDIR).ZoomUp.Scale28
        CMF Resources.Shapes.ZoomUp.Scale32        $(SHAPESDIR).ZoomUp.Scale32
        CMF Resources.Shapes.ZoomUp.Scale36        $(SHAPESDIR).ZoomUp.Scale36
        CMF Resources.Shapes.ZoomUp.Scale40        $(SHAPESDIR).ZoomUp.Scale40
        CMF Resources.Shapes.ZoomUp.Scale44        $(SHAPESDIR).ZoomUp.Scale44
        CMF Resources.Shapes.ZoomUp.Scale48        $(SHAPESDIR).ZoomUp.Scale48
        CMF Resources.Shapes.ZoomUp.Scale52        $(SHAPESDIR).ZoomUp.Scale52
        CMF Resources.Shapes.ZoomUp.Scale56        $(SHAPESDIR).ZoomUp.Scale56
        CMF Resources.Shapes.ZoomUp.Scale60        $(SHAPESDIR).ZoomUp.Scale60
        MMD $(TRAJDIR)
        CMF Resources.Trajectory.Bounce            $(TRAJDIR).Bounce
        CMF Resources.Trajectory.Bounce1           $(TRAJDIR).Bounce1
        CMF Resources.Trajectory.Circle            $(TRAJDIR).Circle
        CMF Resources.Trajectory.RollBFilm         $(TRAJDIR).RollBFilm
        CMF Resources.Trajectory.RollFilm          $(TRAJDIR).RollFilm
        CMF Resources.Trajectory.RollTFilm         $(TRAJDIR).RollTFilm
        CMF Resources.Trajectory.RotAcorn          $(TRAJDIR).RotAcorn
        CMF Resources.Trajectory.RotAcorn2         $(TRAJDIR).RotAcorn2
        CMF Resources.Trajectory.ScaleUp           $(TRAJDIR).ScaleUp
        CMF Resources.Trajectory.SpinCircle        $(TRAJDIR).SpinCircle
        CMF Resources.Trajectory.SpinCube          $(TRAJDIR).SpinCube
        CMF Resources.Trajectory.SpinCube2         $(TRAJDIR).SpinCube2
        CMF Resources.Trajectory.SpinCube3         $(TRAJDIR).SpinCube3
        CMF Resources.Trajectory.Swing             $(TRAJDIR).Swing
        CMF Resources.Trajectory.ZoomUp            $(TRAJDIR).ZoomUp
Resources.Shapes.Cubes2.Cube0: Resources.Shapes.Cubes2.bas.MkRepMapA
        Set RotWorld$Path Resources.Shapes.Cubes2.
        /Resources.Shapes.Cubes2.bas.MkRepMapA
        Unset RotWorld$Path
Resources.Shapes.Cubes3.Cube0: Resources.Shapes.Cubes3.bas.MkRepMapA
        Set RotWorld$Path Resources.Shapes.Cubes3.
        /Resources.Shapes.Cubes3.bas.MkRepMapA
        Unset RotWorld$Path
Resources.Shapes.Split.Split: Resources.Shapes.Split.bas.MakeSplit
        /Resources.Shapes.Split.bas.MakeSplit
Resources.Shapes.TestShape.Shape: Resources.Shapes.TestShape.bas.MakeShape
        /Resources.Shapes.TestShape.bas.MakeShape
Resources.Shapes.ZoomUp.Box04: Resources.Shapes.ZoomUp.bas.MakeZoom
        /Resources.Shapes.ZoomUp.bas.MakeZoom
Resources.Shapes.ZoomUp.Scale04: Resources.Shapes.ZoomUp.bas.MakeScale
        /Resources.Shapes.ZoomUp.bas.MakeScale
Resources.Trajectory.Circle: Resources.Trajectory.bas.MakeCircle
        /Resources.Trajectory.bas.MakeCircle

adpcm_1_Decode_16:    Sound16.adpcm_1.Play \
                      Sound16.adpcm_1.Resources.Info
        MMD $(AUD16DIR).adpcm
        CMF Sound16.adpcm_1.Play                   $(AUD16DIR).adpcm.Play
        CMF Sound16.adpcm_1.Playx2                 $(AUD16DIR).adpcm.Playx2
        CMF Sound16.adpcm_1.Resources.Info         $(AUD16DIR).adpcm.Info
Sound16.adpcm_1.Play:
        @Dir Sound16.adpcm_1
        @amu_machine
        @Up 2

adpcm_1_Decode_8:     Sound.Type1.adpcm.Play \
                      Sound.Type1.adpcm.Playx2 \
                      Sound.Type1.Resources.Info
        MMD $(AUD8DIR).adpcm
        CMF Sound.Type1.adpcm.Play                 $(AUD8DIR).adpcm.Play
        CMF Sound.Type1.adpcm.Playx2               $(AUD8DIR).adpcm.Playx2
        CMF Sound.Type1.Resources.Info             $(AUD8DIR).adpcm.Info

adpcm_Decode_16:      Sound16.adpcm.Play \
                      Sound16.adpcm.Resources.Info
        MMD $(AUD16DIR).adpcm
        CMF Sound16.adpcm.Play                     $(AUD16DIR).adpcm.Play
        CMF Sound16.adpcm.Playx2                   $(AUD16DIR).adpcm.Playx2
        CMF Sound16.adpcm.Resources.Info           $(AUD16DIR).adpcm.Info
Sound16.adpcm.Play:
        @Dir Sound16.adpcm
        @amu_machine
        @Up 2

adpcm_Encode_16:      Sound16.adpcm.Decode \
                      Sound16.adpcm.Decodex2 \
                      Sound16.adpcm.Encode \
                      Sound16.adpcm.Encodex2 \
                      Sound16.adpcm.Resources.CInfo
        CMF Sound16.adpcm.Decode                   $(AUD16DIR).adpcm.Decode
        CMF Sound16.adpcm.Decodex2                 $(AUD16DIR).adpcm.Decodex2
        CMF Sound16.adpcm.Encode                   $(AUD16DIR).adpcm.Encode
        CMF Sound16.adpcm.Encodex2                 $(AUD16DIR).adpcm.Encodex2
        CMF Sound16.adpcm.Resources.CInfo          $(AUD16DIR).adpcm.CInfo

Alaw_Decode_16:       Sound16.Alaw.Play \
                      Sound16.Alaw.Playx2 \
                      Sound16.Alaw.Resources.Info
        MMD $(AUD16DIR).Alaw
        CMF Sound16.Alaw.Play                      $(AUD16DIR).Alaw.Play
        CMF Sound16.Alaw.Playx2                    $(AUD16DIR).Alaw.Playx2
        CMF Sound16.Alaw.Resources.Info            $(AUD16DIR).Alaw.Info

Alaw_Decode_8:        Sound.Alaw.Play \
                      Sound.Alaw.Playx2 \
                      Sound.Alaw.Resources.Info
        MMD $(AUD8DIR).Alaw
        CMF Sound.Alaw.Play                        $(AUD8DIR).Alaw.Play
        CMF Sound.Alaw.Playx2                      $(AUD8DIR).Alaw.Playx2
        CMF Sound.Alaw.Resources.Info              $(AUD8DIR).Alaw.Info

AlawWve_Decode_8:     Sound.AlawWve.Play \
                      Sound.AlawWve.Resources.Info
        MMD $(AUD8DIR).AlawWve
        CMF Sound.AlawWve.Play                     $(AUD8DIR).AlawWve.Play
        CMF Sound.AlawWve.Resources.Info           $(AUD8DIR).AlawWve.Info

g721_Decode_16:       Sound16.g721.Play \
                      Sound16.g721.Resources.Info
        MMD $(AUD16DIR).g721
        CMF Sound16.g721.Play                      $(AUD16DIR).g721.Play
        CMF Sound16.g721.Playx2                    $(AUD16DIR).g721.Playx2
        CMF Sound16.g721.Resources.Info            $(AUD16DIR).g721.Info
Sound16.g721.Play:
        @Dir Sound16.g721
        @amu_machine
        @Up 2

g721_Encode_16:       Sound16.g721.Decode \
                      Sound16.g721.Decodex2 \
                      Sound16.g721.Encode \
                      Sound16.g721.Encodex2 \
                      Sound16.g721.Resources.CInfo
        CMF Sound16.g721.Decode                    $(AUD16DIR).g721.Decode
        CMF Sound16.g721.Decodex2                  $(AUD16DIR).g721.Decodex2
        CMF Sound16.g721.Encode                    $(AUD16DIR).g721.Encode
        CMF Sound16.g721.Encodex2                  $(AUD16DIR).g721.Encodex2
        CMF Sound16.g721.Resources.CInfo           $(AUD16DIR).g721.CInfo

G723-1_Decode_16:     Sound16.g721.Play \
                      Sound16.g721.Resources.Info
        MMD $(AUD16DIR).G723-1
        CMF Sound16.G723-1.Play                    $(AUD16DIR).G723-1.Play
        CMF Sound16.G723-1.Resources.Info          $(AUD16DIR).G723-1.Info

G723-1_Encode_16:     Sound16.G723-1.Decode \
                      Sound16.G723-1.Encode \
                      Sound16.G723-1.Resources.CInfo \
                      Sound16.G723-1.Resources.Flags
        CMF Sound16.G723-1.Decode                  $(AUD16DIR).G723-1.Decode
        CMF Sound16.G723-1.Encode                  $(AUD16DIR).G723-1.Encode
        CMF Sound16.G723-1.Resources.CInfo         $(AUD16DIR).G723-1.CInfo
        CMF Sound16.G723-1.Resources.Flags         $(AUD16DIR).G723-1.Flags

g723_24_Decode_16:    Sound16.g723_24.Play \
                      Sound16.g723_24.Resources.Info
        MMD $(AUD16DIR).g723_24
        CMF Sound16.g723_24.Play                   $(AUD16DIR).g723_24.Play
        CMF Sound16.g723_24.Playx2                 $(AUD16DIR).g723_24.Playx2
        CMF Sound16.g723_24.Resources.Info         $(AUD16DIR).g723_24.Info
Sound16.g723_24.Play:
        @Dir Sound16.g723_24
        @amu_machine
        @Up 2

g723_24_Encode_16:    Sound16.g723_24.Decode \
                      Sound16.g723_24.Decodex2 \
                      Sound16.g723_24.Encode \
                      Sound16.g723_24.Encodex2 \
                      Sound16.g723_24.Resources.CInfo
        CMF Sound16.g723_24.Decode                 $(AUD16DIR).g723_24.Decode
        CMF Sound16.g723_24.Decodex2               $(AUD16DIR).g723_24.Decodex2
        CMF Sound16.g723_24.Encode                 $(AUD16DIR).g723_24.Encode
        CMF Sound16.g723_24.Encodex2               $(AUD16DIR).g723_24.Encodex2
        CMF Sound16.g723_24.Resources.CInfo        $(AUD16DIR).g723_24.CInfo

g723_40_Decode_16:    Sound16.g723_40.Play \
                      Sound16.g723_40.Resources.Info
        MMD $(AUD16DIR).g723_40
        CMF Sound16.g723_40.Play                   $(AUD16DIR).g723_40.Play
        CMF Sound16.g723_40.Playx2                 $(AUD16DIR).g723_40.Playx2
        CMF Sound16.g723_40.Resources.Info         $(AUD16DIR).g723_40.Info
Sound16.g723_40.Play:
        @Dir Sound16.g723_40
        @amu_machine
        @Up 2

g723_40_Encode_16:    Sound16.g723_40.Decode \
                      Sound16.g723_40.Decodex2 \
                      Sound16.g723_40.Encode \
                      Sound16.g723_40.Encodex2 \
                      Sound16.g723_40.Resources.CInfo
        CMF Sound16.g723_40.Decode                 $(AUD16DIR).g723_40.Decode
        CMF Sound16.g723_40.Decodex2               $(AUD16DIR).g723_40.Decodex2
        CMF Sound16.g723_40.Encode                 $(AUD16DIR).g723_40.Encode
        CMF Sound16.g723_40.Encodex2               $(AUD16DIR).g723_40.Encodex2
        CMF Sound16.g723_40.Resources.CInfo        $(AUD16DIR).g723_40.CInfo

g726_16_Decode_16:    Sound16.g726_16.Play \
                      Sound16.g726_16.Resources.Info
        MMD $(AUD16DIR).g726_16
        CMF Sound16.g726_16.Play                   $(AUD16DIR).g726_16.Play
        CMF Sound16.g726_16.Playx2                 $(AUD16DIR).g726_16.Playx2
        CMF Sound16.g726_16.Resources.Info         $(AUD16DIR).g726_16.Info
Sound16.g726_16.Play:
        @Dir Sound16.g726_16
        @amu_machine
        @Up 2

g726_16_Encode_16:    Sound16.g726_16.Decode \
                      Sound16.g726_16.Decodex2 \
                      Sound16.g726_16.Encode \
                      Sound16.g726_16.Encodex2 \
                      Sound16.g726_16.Resources.CInfo
        CMF Sound16.g726_16.Decode                 $(AUD16DIR).g726_16.Decode
        CMF Sound16.g726_16.Decodex2               $(AUD16DIR).g726_16.Decodex2
        CMF Sound16.g726_16.Encode                 $(AUD16DIR).g726_16.Encode
        CMF Sound16.g726_16.Encodex2               $(AUD16DIR).g726_16.Encodex2
        CMF Sound16.g726_16.Resources.CInfo        $(AUD16DIR).g726_16.CInfo

gsm_Decode_16:        Sound16.gsm.Play \
                      Sound16.gsm.Resources.Info
        MMD $(AUD16DIR).gsm
        CMF Sound16.gsm.Play                       $(AUD16DIR).gsm.Play
        CMF Sound16.gsm.Playx2                     $(AUD16DIR).gsm.Playx2
        CMF Sound16.gsm.Resources.Info             $(AUD16DIR).gsm.Info
Sound16.gsm.Play:
        @Dir Sound16.gsm
        @amu_machine
        @Up 2

gsm_Encode_16:        Sound16.gsm.Decode \
                      Sound16.gsm.Decodex2 \
                      Sound16.gsm.Encode \
                      Sound16.gsm.Encodex2 \
                      Sound16.gsm.Resources.CInfo
        CMF Sound16.gsm.Decode                     $(AUD16DIR).gsm.Decode
        CMF Sound16.gsm.Decodex2                   $(AUD16DIR).gsm.Decodex2
        CMF Sound16.gsm.Encode                     $(AUD16DIR).gsm.Encode
        CMF Sound16.gsm.Encodex2                   $(AUD16DIR).gsm.Encodex2
        CMF Sound16.gsm.Resources.CInfo            $(AUD16DIR).gsm.CInfo

Lin16be_Decode_16:    Sound16.Lin16be.Play \
                      Sound16.Lin16be.Playx2 \
                      Sound16.Lin16be.Resources.Info
        MMD $(AUD16DIR).Lin16be
        CMF Sound16.Lin16be.Play                   $(AUD16DIR).Lin16be.Play
        CMF Sound16.Lin16be.Playx2                 $(AUD16DIR).Lin16be.Playx2
        CMF Sound16.Lin16be.Resources.Info         $(AUD16DIR).Lin16be.Info

Lin16be_Decode_8:     Sound.Lin16be.Play \
                      Sound.Lin16be.Playx2 \
                      Sound.Lin16be.Resources.Info
        MMD $(AUD8DIR).Lin16be
        CMF Sound.Lin16be.Play                     $(AUD8DIR).Lin16be.Play
        CMF Sound.Lin16be.Playx2                   $(AUD8DIR).Lin16be.Playx2
        CMF Sound.Lin16be.Resources.Info           $(AUD8DIR).Lin16be.Info

Lin16bes_Decode_8:    Sound.Lin16bes.Play \
                      Sound.Lin16bes.Playx2 \
                      Sound.Lin16bes.Resources.Info
        MMD $(AUD8DIR).Lin16bes
        CMF Sound.Lin16bes.Play                    $(AUD8DIR).Lin16bes.Play
        CMF Sound.Lin16bes.Playx2                  $(AUD8DIR).Lin16bes.Playx2
        CMF Sound.Lin16bes.Resources.Info          $(AUD8DIR).Lin16bes.Info

mpeg-I_Decode_16:     Sound16.mpeg-I.Play \
                      Sound16.mpeg-I.Resources.Info
        MMD $(AUD16DIR).mpeg-I
        CMF Sound16.mpeg-I.Play                    $(AUD16DIR).mpeg-I.Play
        CMF Sound16.mpeg-I.Playx2                  $(AUD16DIR).mpeg-I.Playx2
        CMF Sound16.mpeg-I.Resources.Info          $(AUD16DIR).mpeg-I.Info
Sound16.mpeg-I.Play:
        @Dir Sound16.mpeg-I
        @amu_machine
        @Up 2

mpeg-I_Encode_16:     Sound16.mpeg-I.Decode \
                      Sound16.mpeg-I.Decodex2 \
                      Sound16.mpeg-I.Encode \
                      Sound16.mpeg-I.Encodex2 \
                      Sound16.mpeg-I.Resources.CInfo
        CMF Sound16.mpeg-I.Decode                  $(AUD16DIR).mpeg-I.Decode
        CMF Sound16.mpeg-I.Decodex2                $(AUD16DIR).mpeg-I.Decodex2
        CMF Sound16.mpeg-I.Encode                  $(AUD16DIR).mpeg-I.Encode
        CMF Sound16.mpeg-I.Encodex2                $(AUD16DIR).mpeg-I.Encodex2
        CMF Sound16.mpeg-I.Resources.CInfo         $(AUD16DIR).mpeg-I.CInfo

mpeg-II_Decode_16:    Sound16.mpeg-II.Play \
                      Sound16.mpeg-II.Resources.Info
        MMD $(AUD16DIR).mpeg-II
        CMF Sound16.mpeg-II.Play                   $(AUD16DIR).mpeg-II.Play
        CMF Sound16.mpeg-II.Playx2                 $(AUD16DIR).mpeg-II.Playx2
        CMF Sound16.mpeg-II.Resources.Info         $(AUD16DIR).mpeg-II.Info
Sound16.mpeg-II.Play:
        @Dir Sound16.mpeg-II
        @amu_machine
        @Up 2

mpeg-II_Encode_16:    Sound16.mpeg-II.Decode \
                      Sound16.mpeg-II.Decodex2 \
                      Sound16.mpeg-II.Encode \
                      Sound16.mpeg-II.Encodex2 \
                      Sound16.mpeg-II.Resources.CInfo
        CMF Sound16.mpeg-II.Decode                 $(AUD16DIR).mpeg-II.Decode
        CMF Sound16.mpeg-II.Decodex2               $(AUD16DIR).mpeg-II.Decodex2
        CMF Sound16.mpeg-II.Encode                 $(AUD16DIR).mpeg-II.Encode
        CMF Sound16.mpeg-II.Encodex2               $(AUD16DIR).mpeg-II.Encodex2
        CMF Sound16.mpeg-II.Resources.CInfo        $(AUD16DIR).mpeg-II.CInfo

msadpcm_Decode_16:    Sound16.msadpcm.Play \
                      Sound16.msadpcm.Playx2 \
                      Sound16.msadpcm.Resources.Info
        MMD $(AUD16DIR).msadpcm
        CMF Sound16.msadpcm.Play                   $(AUD16DIR).msadpcm.Play
        CMF Sound16.msadpcm.Playx2                 $(AUD16DIR).msadpcm.Playx2
        CMF Sound16.msadpcm.Resources.Info         $(AUD16DIR).msadpcm.Info

msadpcm_Decode_8:     Sound.msadpcm.Play \
                      Sound.msadpcm.Playx2 \
                      Sound.msadpcm.Resources.Info
        MMD $(AUD8DIR).msadpcm
        CMF Sound.msadpcm.Play                     $(AUD8DIR).msadpcm.Play
        CMF Sound.msadpcm.Playx2                   $(AUD8DIR).msadpcm.Playx2
        CMF Sound.msadpcm.Resources.Info           $(AUD8DIR).msadpcm.Info

MuLaw_Decode_16:      Sound16.MuLaw.Play \
                      Sound16.MuLaw.Playx2 \
                      Sound16.MuLaw.Resources.Info
        MMD $(AUD16DIR).MuLaw
        CMF Sound16.MuLaw.Play                     $(AUD16DIR).MuLaw.Play
        CMF Sound16.MuLaw.Playx2                   $(AUD16DIR).MuLaw.Playx2
        CMF Sound16.MuLaw.Resources.Info           $(AUD16DIR).MuLaw.Info

MuLaw_Decode_8:       Sound.MuLaw.Play \
                      Sound.MuLaw.Playx2 \
                      Sound.MuLaw.Resources.Info
        MMD $(AUD8DIR).MuLaw
        CMF Sound.MuLaw.Play                       $(AUD8DIR).MuLaw.Play
        CMF Sound.MuLaw.Playx2                     $(AUD8DIR).MuLaw.Playx2
        CMF Sound.MuLaw.Resources.Info             $(AUD8DIR).MuLaw.Info

qtima4_Decode_16:     Sound16.qtima4.Play \
                      Sound16.qtima4.Playx2 \
                      Sound16.qtima4.Resources.Info
        MMD $(AUD16DIR).qtima4
        CMF Sound16.qtima4.Play                    $(AUD16DIR).qtima4.Play
        CMF Sound16.qtima4.Playx2                  $(AUD16DIR).qtima4.Playx2
        CMF Sound16.qtima4.Resources.Info          $(AUD16DIR).qtima4.Info

qtima4_Decode_8:      Sound.qtima4.Play \
                      Sound.qtima4.Playx2 \
                      Sound.qtima4.Resources.Info
        MMD $(AUD8DIR).qtima4
        CMF Sound.qtima4.Play                      $(AUD8DIR).qtima4.Play
        CMF Sound.qtima4.Playx2                    $(AUD8DIR).qtima4.Playx2
        CMF Sound.qtima4.Resources.Info            $(AUD8DIR).qtima4.Info

RA14_4_Decode_16:     Sound16.RA14_4.Play \
                      Sound16.RA14_4.Resources.Info
        MMD $(AUD16DIR).RA14_4
        CMF Sound16.RA14_4.Play                    $(AUD16DIR).RA14_4.Play
        CMF Sound16.RA14_4.Resources.Info          $(AUD16DIR).RA14_4.Info

RA28_8_Decode_16:     Sound16.RA28_8.Play \
                      Sound16.RA28_8.Resources.Info
        MMD $(AUD16DIR).RA28_8
        CMF Sound16.RA28_8.Play                    $(AUD16DIR).RA28_8.Play
        CMF Sound16.RA28_8.Resources.Info          $(AUD16DIR).RA28_8.Info

RAdnet_Decode_16:     Sound16.RAdnet.Play \
                      Sound16.RAdnet.Playx2 \
                      Sound16.RAdnet.Resources.Info
        MMD $(AUD16DIR).RAdnet
        CMF Sound16.RAdnet.Play                    $(AUD16DIR).RAdnet.Play
        CMF Sound16.RAdnet.Playx2                  $(AUD16DIR).RAdnet.Playx2
        CMF Sound16.RAdnet.Resources.Info          $(AUD16DIR).RAdnet.Info

RASIPR_Decode_16:     Sound16.RASIPR.Play \
                      Sound16.RASIPR.Resources.Info
        MMD $(AUD16DIR).RASIPR
        CMF Sound16.RASIPR.Play                    $(AUD16DIR).RASIPR.Play
        CMF Sound16.RASIPR.Resources.Info          $(AUD16DIR).RASIPR.Info

SoundA4_Decode_8:     Sound.Type1.SoundA4 \
                      Sound.Type1.SoundA4x2
        MMD $(AUD8DIR)
        CMF Sound.Type1.SoundA4                    $(AUD8DIR).SoundA4
        CMF Sound.Type1.SoundA4x2                  $(AUD8DIR).SoundA4x2

SoundE8_Decode_16:    Sound16.Type1.SoundE8
        MMD $(AUD16DIR)
        CMF Sound16.Type1.SoundE8                  $(AUD16DIR).SoundE8
        CMF Sound16.Type1.SoundE8x2                $(AUD16DIR).SoundE8x2
Sound16.Type1.SoundE8:
        @Dir Sound16.Type1
        @amu_machine
        @Up 2

SoundE8_Decode_8:     Sound.Type1.SoundE8 \
                      Sound.Type1.SoundE8x2
        MMD $(AUD8DIR)
        CMF Sound.Type1.SoundE8                    $(AUD8DIR).SoundE8
        CMF Sound.Type1.SoundE8x2                  $(AUD8DIR).SoundE8x2

SoundS8_Decode_16:    Sound16.Type1.SoundS8
        MMD $(AUD16DIR)
        CMF Sound16.Type1.SoundS8                  $(AUD16DIR).SoundS8
        CMF Sound16.Type1.SoundS8x2                $(AUD16DIR).SoundS8x2
Sound16.Type1.SoundS8:
        @Dir Sound16.Type1
        @amu_machine
        @Up 2

SoundS8_Decode_8:     Sound.Type1.SoundS8 \
                      Sound.Type1.SoundS8x2
        MMD $(AUD8DIR)
        CMF Sound.Type1.SoundS8                    $(AUD8DIR).SoundS8
        CMF Sound.Type1.SoundS8x2                  $(AUD8DIR).SoundS8x2

SoundU8_Decode_16:    Sound16.Type1.SoundU8
        MMD $(AUD16DIR)
        CMF Sound16.Type1.SoundU8                  $(AUD16DIR).SoundU8
        CMF Sound16.Type1.SoundU8x2                $(AUD16DIR).SoundU8x2
Sound16.Type1.SoundU8:
        @Dir Sound16.Type1
        @amu_machine
        @Up 2

SoundU8_Decode_8:     Sound.Type1.SoundU8 \
                      Sound.Type1.SoundU8x2
        MMD $(AUD8DIR)
        CMF Sound.Type1.SoundU8                    $(AUD8DIR).SoundU8
        CMF Sound.Type1.SoundU8x2                  $(AUD8DIR).SoundU8x2

SoundS16_Decode_16:   Sound16.Type1.SoundS16
        MMD $(AUD16DIR)
        CMF Sound16.Type1.SoundS16                 $(AUD16DIR).SoundS16
        CMF Sound16.Type1.SoundS16x2               $(AUD16DIR).SoundS16x2
Sound16.Type1.SoundS16:
        @Dir Sound16.Type1
        @amu_machine
        @Up 2

SoundS16_Decode_8:    Sound.Type1.SoundS16 \
                      Sound.Type1.SoundS16x2
        MMD $(AUD8DIR)
        CMF Sound.Type1.SoundS16                   $(AUD8DIR).SoundS16
        CMF Sound.Type1.SoundS16x2                 $(AUD8DIR).SoundS16x2

SoundU16_Decode_16:   Sound16.Type1.SoundU16
        MMD $(AUD16DIR)
        CMF Sound16.Type1.SoundU16                 $(AUD16DIR).SoundU16
        CMF Sound16.Type1.SoundU16x2               $(AUD16DIR).SoundU16x2
Sound16.Type1.SoundU16:
        @Dir Sound16.Type1
        @amu_machine
        @Up 2

MovingLine_Decode:    Video.MovingLine.Decompress \
                      Video.MovingLine.Resources.Info
        MMD $(VIDDIR).MovingLine
        If1˜8bpp CMF Video.MovingLine.DecompresB   $(VIDDIR).MovingLine.DecompresB
        If4˜16bpp CMF Video.MovingLine.DecompresH  $(VIDDIR).MovingLine.DecompresH
        If8˜32bpp CMF Video.MovingLine.Decompress  $(VIDDIR).MovingLine.Decompress
        CMF Video.MovingLine.Resources.Info        $(VIDDIR).MovingLine.Info
Video.MovingLine.Decompress: Video.MovingLine.bas.MakeDecomp
        /Video.MovingLine.bas.MakeDecomp

MovingLine_Encode:    Video.MovingLine.BatchComp
        CMF Video.MovingLine.BatchComp             $(VIDDIR).MovingLine.BatchComp
Video.MovingLine.BatchComp: Video.MovingLine.bas.BatchComp
        /Video.MovingLine.crunch.BatchComp
        BASIC

Decomp2_Decode:       Video.Decomp2.Decompress \
                      Video.Decomp2.Resources.Info
        MMD $(VIDDIR).Decomp2
        CMF Video.Decomp2.Dec16                    $(VIDDIR).Decomp2.Dec16
        If1˜8bpp CMF Video.Decomp2.DecompresB      $(VIDDIR).Decomp2.DecompresB
        If8˜32bpp CMF Video.Decomp2.Decompress     $(VIDDIR).Decomp2.Decompress
        CMF Video.Decomp2.Resources.Info           $(VIDDIR).Decomp2.Info
Video.Decomp2.Decompress: Video.Decomp2.bas.MakeDecomp
        /Video.Decomp2.bas.MakeDecomp

Decomp2_Encode:       Video.Decomp2.bas.BatchComp
        CMF Video.Decomp2.bas.BatchComp            $(VIDDIR).Decomp2.BatchComp

Decomp3_Decode:       Video.Decomp3.Decompress \
                      Video.Decomp3.Resources.Info
        MMD $(VIDDIR).Decomp3
        CMF Video.Decomp3.Dec16                    $(VIDDIR).Decomp3.Dec16
        If1˜8bpp CMF Video.Decomp3.DecompresB      $(VIDDIR).Decomp3.DecompresB
        If8˜32bpp CMF Video.Decomp3.Decompress     $(VIDDIR).Decomp3.Decompress
        CMF Video.Decomp3.Resources.Info           $(VIDDIR).Decomp3.Info
Video.Decomp3.Decompress: Video.Decomp3.bas.MakeDecomp
        /Video.Decomp3.bas.MakeDecomp

Decomp3_Encode:       Video.Decomp3.bas.BatchComp
        CMF Video.Decomp3.bas.BatchComp            $(VIDDIR).Decomp3.BatchComp

Decomp4_Decode:       Video.Decomp4.Decompress \
                      Video.Decomp4.Resources.Info
        MMD $(VIDDIR).Decomp4
        CMF Video.Decomp4.Dec8                     $(VIDDIR).Decomp4.Dec8
        If8˜32bpp CMF Video.Decomp4.Decompress     $(VIDDIR).Decomp4.Decompress
        CMF Video.Decomp4.Resources.Info           $(VIDDIR).Decomp4.Info
Video.Decomp4.Decompress: Video.Decomp4.bas.MakeDecomp
        /Video.Decomp4.bas.MakeDecomp

Decomp5_Decode:       Video.Decomp5.Decompress \
                      Video.Decomp5.Resources.Info
        MMD $(VIDDIR).Decomp5
        CMF Video.Decomp5.Dec16                    $(VIDDIR).Decomp5.Dec16
        If1˜8bpp CMF Video.Decomp5.DecompresB      $(VIDDIR).Decomp5.DecompresB
        If8˜32bpp CMF Video.Decomp5.Decompress     $(VIDDIR).Decomp5.Decompress
        CMF Video.Decomp5.Resources.Info           $(VIDDIR).Decomp5.Info
Video.Decomp5.Decompress: Video.Decomp5.bas.MakeDecomp
        /Video.Decomp5.bas.MakeDecomp

Decomp5_Encode:       Video.Decomp5.bas.BatchComp
        CMF Video.Decomp5.bas.BatchComp            $(VIDDIR).Decomp5.BatchComp

Decomp6_Decode:       Video.Decomp6.Decompress \
                      Video.Decomp6.Resources.Info
        MMD $(VIDDIR).Decomp6
        CMF Video.Decomp6.Dec16                    $(VIDDIR).Decomp6.Dec16
        If1˜8bpp CMF Video.Decomp6.DecompresB      $(VIDDIR).Decomp6.DecompresB
        If8˜32bpp CMF Video.Decomp6.Decompress     $(VIDDIR).Decomp6.Decompress
        CMF Video.Decomp6.Resources.Info           $(VIDDIR).Decomp6.Info
Video.Decomp6.Decompress: Video.Decomp6.bas.MakeDecomp
        /Video.Decomp6.bas.MakeDecomp

Decomp6_Encode:       Video.Decomp6.bas.BatchComp
        CMF Video.Decomp6.bas.BatchComp            $(VIDDIR).Decomp6.BatchComp

Decomp7_Decode:       Video.Decomp7.Decompress \
                      Video.Decomp7.Resources.Info
        MMD $(VIDDIR).Decomp7
        If1˜8bpp CMF Video.Decomp7.DecompresB      $(VIDDIR).Decomp7.DecompresB
        If4˜16bpp CMF Video.Decomp7.DecompresH     $(VIDDIR).Decomp7.DecompresH
        If8˜32bpp CMF Video.Decomp7.Decompress     $(VIDDIR).Decomp7.Decompress
        CMF Video.Decomp7.Resources.Info           $(VIDDIR).Decomp7.Info
Video.Decomp7.Decompress: Video.Decomp7.bas.MakeDecomp
        /Video.Decomp7.bas.MakeDecomp

Decomp7_Encode:       Video.Decomp7.BatchComp
        CMF Video.Decomp7.BatchComp                $(VIDDIR).Decomp7.BatchComp
Video.Decomp7.BatchComp: Video.Decomp7.bas.BatchComp
        /Video.Decomp7.crunch.BatchComp
        BASIC

Decomp8_Decode:       Video.Decomp8.Decompress \
                      Video.Decomp8.Resources.Info
        MMD $(VIDDIR).Decomp8
        If32bpp CMF Video.Decomp8.Dec21            $(VIDDIR).Decomp8.Dec21
        If32bpp CMF Video.Decomp8.Dec24            $(VIDDIR).Decomp8.Dec24
        If1˜8bpp CMF Video.Decomp8.DecompresB      $(VIDDIR).Decomp8.DecompresB
        If4˜16bpp CMF Video.Decomp8.DecompresH     $(VIDDIR).Decomp8.DecompresH
        If8˜32bpp CMF Video.Decomp8.Decompress     $(VIDDIR).Decomp8.Decompress
        If32bpp CMF Video.Decomp8.FastDec21        $(VIDDIR).Decomp8.FastDec21
        If32bpp CMF Video.Decomp8.FastDec24        $(VIDDIR).Decomp8.FastDec24
        CMF Video.Decomp8.Resources.Info           $(VIDDIR).Decomp8.Info
Video.Decomp8.Decompress: Video.Decomp8.bas.MakeDecomp
        /Video.Decomp8.bas.MakeDecomp

Decomp9_Decode:       Video.Decomp9.Decompress \
                      Video.Decomp9.FastDec21 \
                      Video.Decomp9.Resources.Info
        MMD $(VIDDIR).Decomp9
        If1˜8bpp CMF Video.Decomp9.DecompresB      $(VIDDIR).Decomp9.DecompresB
        If4˜16bpp CMF Video.Decomp9.DecompresH     $(VIDDIR).Decomp9.DecompresH
        If8˜32bpp CMF Video.Decomp9.Decompress     $(VIDDIR).Decomp9.Decompress
        If32bpp CMF Video.Decomp9.FastDec21        $(VIDDIR).Decomp9.FastDec21
        If32bpp CMF Video.Decomp9.FastDec24        $(VIDDIR).Decomp9.FastDec24
        If32bpp CMF Video.Decomp9.FastDecYUV       $(VIDDIR).Decomp9.FastDecYUV
        CMF Video.Decomp9.Resources.Info           $(VIDDIR).Decomp9.Info
Video.Decomp9.Decompress: Video.Decomp9.bas.MakeDecomp
        /Video.Decomp9.bas.MakeDecomp
Video.Decomp9.FastDec21: Video.Decomp9.bas.MakFastDec
        /Video.Decomp9.bas.MakFastDec

Decomp9_Encode:       Video.Decomp9.bas.BatchComp
        CMF Video.Decomp9.bas.BatchComp            $(VIDDIR).Decomp9.BatchComp
        CMF Video.Decomp9.Dec24                    $(VIDDIR).Decomp9.Dec24

Decomp10_Decode:      Video.Decomp10.Decompress \
                      Video.Decomp10.Resources.Info
        MMD $(VIDDIR).Decomp10
        If1˜8bpp CMF Video.Decomp10.DecompresB     $(VIDDIR).Decomp10.DecompresB
        If4˜16bpp CMF Video.Decomp10.DecompresH    $(VIDDIR).Decomp10.DecompresH
        If8˜32bpp CMF Video.Decomp10.Decompress    $(VIDDIR).Decomp10.Decompress
        If32bpp CMF Video.Decomp10.FastDec21       $(VIDDIR).Decomp10.FastDec21
        If32bpp CMF Video.Decomp10.FastDec24       $(VIDDIR).Decomp10.FastDec24
        If32bpp CMF Video.Decomp10.FastDecYUV      $(VIDDIR).Decomp10.FastDecYUV
        CMF Video.Decomp10.Resources.Info          $(VIDDIR).Decomp10.Info
Video.Decomp10.Decompress: Video.Decomp10.bas.MakeDecomp
        /Video.Decomp10.bas.MakeDecomp

Decomp10_Encode:      Video.Decomp10.bas.BatchComp
        CMF Video.Decomp10.bas.BatchComp           $(VIDDIR).Decomp10.BatchComp
        CMF Video.Decomp10.Dec24                   $(VIDDIR).Decomp10.Dec24

Decomp11_Decode:      Video.Decomp11.Decompress \
                      Video.Decomp11.Resources.Info
        MMD $(VIDDIR).Decomp11
        If1˜8bpp CMF Video.Decomp11.DecompresB     $(VIDDIR).Decomp11.DecompresB
        If4˜16bpp CMF Video.Decomp11.DecompresH    $(VIDDIR).Decomp11.DecompresH
        If8˜32bpp CMF Video.Decomp11.Decompress    $(VIDDIR).Decomp11.Decompress
        CMF Video.Decomp11.Resources.Info          $(VIDDIR).Decomp11.Info
Video.Decomp11.Decompress: Video.Decomp11.bas.MakeDecomp
        /Video.Decomp11.bas.MakeDecomp

Decomp11_Encode:      Video.Decomp11.bas.BatchComp
        CMF Video.Decomp11.bas.BatchComp           $(VIDDIR).Decomp11.BatchComp
        CMF Video.Decomp11.Dec24                   $(VIDDIR).Decomp11.Dec24

Decomp12_Decode:      Video.Decomp12.bas.!RunImage \
                      Video.Decomp12.bas.!RunImageM \
                      Video.Decomp12.bas.MMagicUI \
                      Video.Decomp12.Resources.!Run \
                      Video.Decomp12.Resources.Info \
                      Video.Decomp12.Resources.ReadMe \
                      Video.Decomp12.rm.DitherExt \
                      Video.Decomp12.rm.MPEGAudioM \
                      Video.Decomp12.rm.MPEGCtrl \
                      Video.Decomp12.rm.MPEGTrans \
                      Video.Decomp12.rm.MPEGVideoM \
                      Video.Decomp12.rm.SpriteExt
        MMD $(VIDDIR).Decomp12.MCode21
        CMF Video.Decomp12.bas.!RunImage           $(VIDDIR).Decomp12.!RunImage
        CMF Video.Decomp12.bas.!RunImageM          $(VIDDIR).Decomp12.MCode21.!RunImage
        CMF Video.Decomp12.bas.MMagicUI            $(VIDDIR).Decomp12.MMagicUI
        CMF Video.Decomp12.Resources.!Run          $(VIDDIR).Decomp12.!Run
        CMF Video.Decomp12.Resources.Info          $(VIDDIR).Decomp12.Info
        CMF Video.Decomp12.Resources.ReadMe        $(VIDDIR).Decomp12.ReadMe
        CMF Video.Decomp12.rm.DitherExt            $(VIDDIR).Decomp12.DitherExt
        CMF Video.Decomp12.rm.MPEGAudioM           $(VIDDIR).Decomp12.MCode21.MPEGAudioM
        CMF Video.Decomp12.rm.MPEGCtrl             $(VIDDIR).Decomp12.MCode21.MPEGCtrl
        CMF Video.Decomp12.rm.MPEGTrans            $(VIDDIR).Decomp12.MCode21.MPEGTrans
        CMF Video.Decomp12.rm.MPEGVideoM           $(VIDDIR).Decomp12.MCode21.MPEGVideoM
        CMF Video.Decomp12.rm.SpriteExt            $(VIDDIR).Decomp12.SpriteExt

Decomp15_Decode:      Video.Decomp15.Resources.Info \
                      Video.Decomp15.Uncomp15.Decompress \
                      Video.Decomp15.Uncomp15.Resources.Info \
                      Video.Decomp15.Uncomp16.Decompress \
                      Video.Decomp15.Uncomp16.Resources.Info
        MMD $(VIDDIR).Decomp15.Uncomp15
        MMD $(VIDDIR).Decomp15.Uncomp16
        CMF Video.Decomp15.Resources.Info          $(VIDDIR).Decomp15.Info
        If1˜8bpp CMF Video.Decomp15.Uncomp15.DecompresB $(VIDDIR).Decomp15.Uncomp15.DecompresB
        If4˜16bpp CMF Video.Decomp15.Uncomp15.DecompresH $(VIDDIR).Decomp15.Uncomp15.DecompresH
        If8˜32bpp CMF Video.Decomp15.Uncomp15.Decompress $(VIDDIR).Decomp15.Uncomp15.Decompress
        CMF Video.Decomp15.Uncomp15.Resources.Info $(VIDDIR).Decomp15.Uncomp15.Info
        CMF Video.Decomp15.Uncomp16.Dec16          $(VIDDIR).Decomp15.Uncomp16.Dec16
        If1˜8bpp CMF Video.Decomp15.Uncomp16.DecompresB $(VIDDIR).Decomp15.Uncomp16.DecompresB
        If8˜32bpp CMF Video.Decomp15.Uncomp16.Decompress $(VIDDIR).Decomp15.Uncomp16.Decompress
        CMF Video.Decomp15.Uncomp16.Resources.Info $(VIDDIR).Decomp15.Uncomp16.Info
Video.Decomp15.Uncomp15.Decompress: Video.Decomp15.Uncomp15.bas.MakeDecomp
        /Video.Decomp15.Uncomp15.bas.MakeDecomp
Video.Decomp15.Uncomp16.Decompress: Video.Decomp15.Uncomp16.bas.MakeDecomp
        /Video.Decomp15.Uncomp16.bas.MakeDecomp

Decomp16_Decode:      Video.Decomp16.Decompress \
                      Video.Decomp16.Resources.Info
        MMD $(VIDDIR).Decomp16
        If1˜8bpp CMF Video.Decomp16.DecompresB     $(VIDDIR).Decomp16.DecompresB
        If4˜16bpp CMF Video.Decomp16.DecompresH    $(VIDDIR).Decomp16.DecompresH
        If8˜32bpp CMF Video.Decomp16.Decompress    $(VIDDIR).Decomp16.Decompress
        CMF Video.Decomp16.Resources.Info          $(VIDDIR).Decomp16.Info
Video.Decomp16.Decompress: Video.Decomp16.bas.MakeDecomp
        /Video.Decomp16.bas.MakeDecomp

Decomp16_Encode:      Video.Decomp16.bas.BatchComp
        CMF Video.Decomp16.bas.BatchComp           $(VIDDIR).Decomp16.BatchComp
        CMF Video.Decomp16.Dec24                   $(VIDDIR).Decomp16.Dec24

Decomp17_Decode:      Video.Decomp17.Decompress \
                      Video.Decomp17.Resources.Info
        MMD $(VIDDIR).Decomp17
        CMF Video.Decomp17.Dec16                   $(VIDDIR).Decomp17.Dec16
        If1˜8bpp CMF Video.Decomp17.DecompresB     $(VIDDIR).Decomp17.DecompresB
        If4˜16bpp CMF Video.Decomp17.DecompresH    $(VIDDIR).Decomp17.DecompresH
        If8˜32bpp CMF Video.Decomp17.Decompress    $(VIDDIR).Decomp17.Decompress
        CMF Video.Decomp17.Resources.Info          $(VIDDIR).Decomp17.Info
Video.Decomp17.Decompress: Video.Decomp17.bas.MakeDecomp
        /Video.Decomp17.bas.MakeDecomp

Decomp17_Encode:      Video.Decomp17.BatchComp
        CMF Video.Decomp17.BatchComp               $(VIDDIR).Decomp17.BatchComp
Video.Decomp17.BatchComp: Video.Decomp17.bas.BatchComp
        /Video.Decomp17.crunch.BatchComp
        BASIC

Decomp18_Decode:      Video.Decomp18.Dec \
                      Video.Decomp18.Dec16 \
                      Video.Decomp18.Dec216Y5UV \
                      Video.Decomp18.Dec246Y5UV \
                      Video.Decomp18.DecB \
                      Video.Decomp18.DecFast21 \
                      Video.Decomp18.DecFast24 \
                      Video.Decomp18.DecFastYUV \
                      Video.Decomp18.DecH \
                      Video.Decomp18.Resources.Info
        MMD $(VIDDIR).Decomp18
        If8˜32bpp CMF Video.Decomp18.Dec           $(VIDDIR).Decomp18.Decompress
        CMF Video.Decomp18.Dec16                   $(VIDDIR).Decomp18.Dec16
        If32bpp CMF Video.Decomp18.Dec216Y5UV      $(VIDDIR).Decomp18.Dec216Y5UV
        If32bpp CMF Video.Decomp18.Dec246Y5UV      $(VIDDIR).Decomp18.Dec246Y5UV
        If1˜8bpp CMF Video.Decomp18.DecB           $(VIDDIR).Decomp18.DecompresB
        If32bpp CMF Video.Decomp18.DecFast21       $(VIDDIR).Decomp18.FastDec21
        If32bpp CMF Video.Decomp18.DecFast24       $(VIDDIR).Decomp18.FastDec24
        If32bpp CMF Video.Decomp18.DecFastYUV      $(VIDDIR).Decomp18.FastDecYUV
        If4˜16bpp CMF Video.Decomp18.DecH          $(VIDDIR).Decomp18.DecompresH
        CMF Video.Decomp18.Resources.Info          $(VIDDIR).Decomp18.Info
Video.Decomp18.Dec:
        @Dir Video.Decomp18
        @amu_machine
        @Up 2
Video.Decomp18.Dec16:
        @Dir Video.Decomp18
        @amu_machine VARIANT=16
        @Up 2
Video.Decomp18.Dec216Y5UV:
        @Dir Video.Decomp18
        @amu_machine VARIANT=216Y5UV
        @Up 2
Video.Decomp18.Dec246Y5UV:
        @Dir Video.Decomp18
        @amu_machine VARIANT=246Y5UV
        @Up 2
Video.Decomp18.DecB:
        @Dir Video.Decomp18
        @amu_machine VARIANT=B
        @Up 2
Video.Decomp18.DecFast21:
        @Dir Video.Decomp18
        @amu_machine VARIANT=Fast21
        @Up 2
Video.Decomp18.DecFast24:
        @Dir Video.Decomp18
        @amu_machine VARIANT=Fast24
        @Up 2
Video.Decomp18.DecFastYUV:
        @Dir Video.Decomp18
        @amu_machine VARIANT=FastYUV
        @Up 2
Video.Decomp18.DecH:
        @Dir Video.Decomp18
        @amu_machine VARIANT=H
        @Up 2

Decomp18_Encode:      Video.Decomp18.BatchComp.BatchComp \
                      Video.Decomp18.Resources.Args \
                      Video.Decomp18.Dec21 \
                      Video.Decomp18.Dec24 \
                      Video.Decomp18.Resources.BatchComp \
                      Video.Decomp18.Resources.EBatchComp
        CMF Video.Decomp18.BatchComp.BatchComp     $(VIDDIR).Decomp18.RBatchComp
        CMF Video.Decomp18.Resources.Args          $(VIDDIR).Decomp18.Args
        CMF Video.Decomp18.Resources.BatchComp     $(VIDDIR).Decomp18.BatchComp
        CMF Video.Decomp18.Dec21                   $(VIDDIR).Decomp18.Dec21
        CMF Video.Decomp18.Dec24                   $(VIDDIR).Decomp18.Dec24
        CMF Video.Decomp18.Resources.EBatchComp    $(VIDDIR).Decomp18.EBatchComp
Video.Decomp18.BatchComp.BatchComp:
        @Dir Video.Decomp18.BatchComp
        @amu_machine
        @Up 3
Video.Decomp18.Dec21:
        @Dir Video.Decomp18
        @amu_machine VARIANT=21
        @Up 2
Video.Decomp18.Dec24:
        @Dir Video.Decomp18
        @amu_machine VARIANT=24
        @Up 2

Decomp19_Decode:      Video.Decomp19.Decompress \
                      Video.Decomp19.Resources.Info
        MMD $(VIDDIR).Decomp19
        CMF Video.Decomp19.Dec16                   $(VIDDIR).Decomp19.Dec16
        If1˜8bpp CMF Video.Decomp19.DecompresB     $(VIDDIR).Decomp19.DecompresB
        If4˜16bpp CMF Video.Decomp19.DecompresH    $(VIDDIR).Decomp19.DecompresH
        If8˜32bpp CMF Video.Decomp19.Decompress    $(VIDDIR).Decomp19.Decompress
        CMF Video.Decomp19.Resources.Info          $(VIDDIR).Decomp19.Info
Video.Decomp19.Decompress: Video.Decomp19.bas.MakeDecomp
        /Video.Decomp19.bas.MakeDecomp

Decomp19_Encode:      Video.Decomp19.BatchComp
        CMF Video.Decomp19.BatchComp               $(VIDDIR).Decomp19.BatchComp
Video.Decomp19.BatchComp: Video.Decomp19.bas.BatchComp
        /Video.Decomp19.crunch.BatchComp
        BASIC

Decomp20_Decode:      Video.Decomp20.Decompress \
                      Video.Decomp20.Resources.Info
        MMD $(VIDDIR).Decomp20
        If1˜8bpp CMF Video.Decomp20.DecompresB     $(VIDDIR).Decomp20.DecompresB
        If4˜16bpp CMF Video.Decomp20.DecompresH    $(VIDDIR).Decomp20.DecompresH
        If8˜32bpp CMF Video.Decomp20.Decompress    $(VIDDIR).Decomp20.Decompress
        CMF Video.Decomp20.Resources.Info          $(VIDDIR).Decomp20.Info
Video.Decomp20.Decompress: Video.Decomp20.bas.MakeDecomp
        /Video.Decomp20.bas.MakeDecomp

Decomp20_Encode:      Video.Decomp20.BatchComp \
                      Video.Decomp20.D4tab
        CMF Video.Decomp20.BatchComp               $(VIDDIR).Decomp20.BatchComp
        CMF Video.Decomp20.D4tab                   $(VIDDIR).Decomp20.D4tab
Video.Decomp20.BatchComp: Video.Decomp20.bas.BatchComp
        /Video.Decomp20.crunch.BatchComp
        BASIC
Video.Decomp20.D4tab: Video.Decomp20.bas.GenD4Table
        /Video.Decomp20.bas.GenD4Table

Decomp21_Decode:      Video.Decomp21.Decompress \
                      Video.Decomp21.FastDec21 \
                      Video.Decomp21.Resources.Info
        MMD $(VIDDIR).Decomp21
        If1˜8bpp CMF Video.Decomp21.DecompresB     $(VIDDIR).Decomp21.DecompresB
        If4˜16bpp CMF Video.Decomp21.DecompresH    $(VIDDIR).Decomp21.DecompresH
        If8˜32bpp CMF Video.Decomp21.Decompress    $(VIDDIR).Decomp21.Decompress
        If32bpp CMF Video.Decomp21.FastDec21       $(VIDDIR).Decomp21.FastDec21
        If32bpp CMF Video.Decomp21.FastDec24       $(VIDDIR).Decomp21.FastDec24
        If32bpp CMF Video.Decomp21.FastDecYUV      $(VIDDIR).Decomp21.FastDecYUV
        CMF Video.Decomp21.Resources.Info          $(VIDDIR).Decomp21.Info
Video.Decomp21.Decompress: Video.Decomp21.bas.MakeDecomp
        /Video.Decomp21.bas.MakeDecomp
Video.Decomp21.FastDec21: Video.Decomp21.bas.MakFastDec
        /Video.Decomp21.bas.MakFastDec

Decomp21_Encode:      Video.Decomp21.bas.BatchComp
        CMF Video.Decomp21.bas.BatchComp           $(VIDDIR).Decomp21.BatchComp
        CMF Video.Decomp21.Dec24                   $(VIDDIR).Decomp21.Dec24

Decomp22_Decode:      Video.Decomp22.Decompress \
                      Video.Decomp22.Resources.Info
        MMD $(VIDDIR).Decomp22
        If1˜8bpp CMF Video.Decomp22.DecompresB     $(VIDDIR).Decomp22.DecompresB
        If4˜16bpp CMF Video.Decomp22.DecompresH    $(VIDDIR).Decomp22.DecompresH
        If8˜32bpp CMF Video.Decomp22.Decompress    $(VIDDIR).Decomp22.Decompress
        CMF Video.Decomp22.Resources.Info          $(VIDDIR).Decomp22.Info
Video.Decomp22.Decompress: Video.Decomp22.bas.MakeDecomp
        /Video.Decomp22.bas.MakeDecomp

Decomp22_Encode:      Video.Decomp22.bas.BatchComp \
                      Video.Decomp22.D4tab
        CMF Video.Decomp22.bas.BatchComp           $(VIDDIR).Decomp22.BatchComp
        CMF Video.Decomp22.D4tab                   $(VIDDIR).Decomp22.D4tab
        CMF Video.Decomp22.Dec24                   $(VIDDIR).Decomp22.Dec24
Video.Decomp22.D4tab: Video.Decomp22.bas.GenD4Table
        /Video.Decomp22.bas.GenD4Table

Decomp23_Decode:      Video.Decomp23.Decompress \
                      Video.Decomp23.Resources.Info
        MMD $(VIDDIR).Decomp23
        CMF Video.Decomp23.Dec16                   $(VIDDIR).Decomp23.Dec16
        If1˜8bpp CMF Video.Decomp23.DecompresB     $(VIDDIR).Decomp23.DecompresB
        If8˜32bpp CMF Video.Decomp23.Decompress    $(VIDDIR).Decomp23.Decompress
        CMF Video.Decomp23.Resources.Info          $(VIDDIR).Decomp23.Info
Video.Decomp23.Decompress: Video.Decomp23.bas.MakeDecomp
        /Video.Decomp23.bas.MakeDecomp

Decomp23_Encode:      Video.Decomp23.bas.BatchComp
        CMF Video.Decomp23.bas.BatchComp           $(VIDDIR).Decomp23.BatchComp

Decomp24_Decode:      Video.Decomp24.Decompress \
                      Video.Decomp24.Resources.Info
        MMD $(VIDDIR).Decomp24
        CMF Video.Decomp24.Dec16                   $(VIDDIR).Decomp24.Dec16
        If1˜8bpp CMF Video.Decomp24.DecompresB     $(VIDDIR).Decomp24.DecompresB
        If8˜32bpp CMF Video.Decomp24.Decompress    $(VIDDIR).Decomp24.Decompress
        CMF Video.Decomp24.Resources.Info          $(VIDDIR).Decomp24.Info
Video.Decomp24.Decompress: Video.Decomp24.bas.MakeDecomp
        /Video.Decomp24.bas.MakeDecomp

Decomp24_Encode:      Video.Decomp24.bas.BatchComp
        CMF Video.Decomp24.bas.BatchComp           $(VIDDIR).Decomp24.BatchComp

Decomp25_Decode:      Video.Decomp25.Decompress \
                      Video.Decomp25.Resources.Info
        MMD $(VIDDIR).Decomp25
        If4˜16bpp CMF Video.Decomp25.DecompresH    $(VIDDIR).Decomp25.DecompresH
        If8˜32bpp CMF Video.Decomp25.Decompress    $(VIDDIR).Decomp25.Decompress
        If32bpp CMF Video.Decomp25.FastDecYUV      $(VIDDIR).Decomp25.FastDecYUV
        CMF Video.Decomp25.Resources.Info          $(VIDDIR).Decomp25.Info
Video.Decomp25.Decompress: Video.Decomp25.bas.MakeDecomp
        /Video.Decomp25.bas.MakeDecomp

Decomp25_Encode:      Video.Decomp25.bas.BatchComp \
                      Video.Decomp25.D4tab \
                      Video.Decomp25.D4tabU
        CMF Video.Decomp25.bas.BatchComp           $(VIDDIR).Decomp25.BatchComp
        CMF Video.Decomp25.D4tab                   $(VIDDIR).Decomp25.D4tab
        CMF Video.Decomp25.D4tabU                  $(VIDDIR).Decomp25.D4tabU
        CMF Video.Decomp25.Dec24                   $(VIDDIR).Decomp25.Dec24
Video.Decomp25.D4tab: Video.Decomp25.bas.GenD4Table
        /Video.Decomp25.bas.GenD4Table
Video.Decomp25.D4tabU: Video.Decomp25.bas.GenD4Uns
        /Video.Decomp25.bas.GenD4Uns

Decomp26_Decode:      Video.Decomp26.Decompress \
                      Video.Decomp26.Resources.Info
        MMD $(VIDDIR).Decomp26
        If4˜16bpp CMF Video.Decomp26.DecompresH    $(VIDDIR).Decomp26.DecompresH
        If8˜32bpp CMF Video.Decomp26.Decompress    $(VIDDIR).Decomp26.Decompress
        CMF Video.Decomp26.Resources.Info          $(VIDDIR).Decomp26.Info
Video.Decomp26.Decompress: Video.Decomp26.bas.MakeDecomp
        /Video.Decomp26.bas.MakeDecomp

Decomp26_Encode:      Video.Decomp26.bas.BatchComp \
                      Video.Decomp26.D4tab \
                      Video.Decomp26.D4tabU
        CMF Video.Decomp26.bas.BatchComp           $(VIDDIR).Decomp26.BatchComp
        CMF Video.Decomp26.D4tab                   $(VIDDIR).Decomp26.D4tab
        CMF Video.Decomp26.D4tabU                  $(VIDDIR).Decomp26.D4tabU
        CMF Video.Decomp26.Dec24                   $(VIDDIR).Decomp26.Dec24
Video.Decomp26.D4tab: Video.Decomp26.bas.GenD4Table
        /Video.Decomp26.bas.GenD4Table
Video.Decomp26.D4tabU: Video.Decomp26.bas.GenD4Uns
        /Video.Decomp26.bas.GenD4Uns

Decomp27_Decode:      Video.Decomp27.Decompress \
                      Video.Decomp27.Resources.Info
        MMD $(VIDDIR).Decomp27
        If4˜16bpp CMF Video.Decomp27.DecompresH    $(VIDDIR).Decomp27.DecompresH
        If8˜32bpp CMF Video.Decomp27.Decompress    $(VIDDIR).Decomp27.Decompress
        CMF Video.Decomp27.Resources.Info          $(VIDDIR).Decomp27.Info
Video.Decomp27.Decompress: Video.Decomp27.bas.MakeDecomp
        /Video.Decomp27.bas.MakeDecomp

Decomp27_Encode:      Video.Decomp27.bas.BatchComp \
                      Video.Decomp27.D4tab \
                      Video.Decomp27.D4tabU
        CMF Video.Decomp27.bas.BatchComp           $(VIDDIR).Decomp27.BatchComp
        CMF Video.Decomp27.D4tab                   $(VIDDIR).Decomp27.D4tab
        CMF Video.Decomp27.D4tabU                  $(VIDDIR).Decomp27.D4tabU
        CMF Video.Decomp27.Dec24                   $(VIDDIR).Decomp27.Dec24
Video.Decomp27.D4tab: Video.Decomp27.bas.GenD4Table
        /Video.Decomp27.bas.GenD4Table
Video.Decomp27.D4tabU: Video.Decomp27.bas.GenD4Uns
        /Video.Decomp27.bas.GenD4Uns

Decomp100_Decode:     Video.Decomp100.Decompress \
                      Video.Decomp100.Resources.Info
        MMD $(VIDDIR).Decomp100
        If8˜32bpp CMF Video.Decomp100.Decompress   $(VIDDIR).Decomp100.Decompress
        CMF Video.Decomp100.Resources.Info         $(VIDDIR).Decomp100.Info

Decomp500_Decode:     Video.Decomp500.bas.!RunImage \
                      Video.Decomp500.Resources.Info \
                      Video.Decomp500.Resources.ReadMe \
                      Video.Decomp500.rm.EuclidX \
                      Video.Decomp500.rm.IotaSound
        MMD $(VIDDIR).Decomp500
        CMF Video.Decomp500.bas.!RunImage          $(VIDDIR).Decomp500.!RunImage
        CMF Video.Decomp500.Resources.Info         $(VIDDIR).Decomp500.Info
        CMF Video.Decomp500.Resources.ReadMe       $(VIDDIR).Decomp500.ReadMe
        CMF Video.Decomp500.rm.EuclidX             $(VIDDIR).Decomp500.EuclidX
        CMF Video.Decomp500.rm.IotaSound           $(VIDDIR).Decomp500.IotaSound

Decomp600_Decode:     Video.Decomp600.DecompresH \
                      Video.Decomp600.Decompress \
                      Video.Decomp600.Resources.Info
        MMD $(VIDDIR).Decomp600
        If4˜16bpp CMF Video.Decomp600.DecompresH   $(VIDDIR).Decomp600.DecompresH
        If8˜32bpp CMF Video.Decomp600.Decompress   $(VIDDIR).Decomp600.Decompress
        CMF Video.Decomp600.Resources.Info         $(VIDDIR).Decomp600.Info

Decomp601_Decode:     Video.Decomp601.DecompresH \
                      Video.Decomp601.Decompress \
                      Video.Decomp601.Resources.Info
        MMD $(VIDDIR).Decomp601
        If4˜16bpp CMF Video.Decomp601.DecompresH   $(VIDDIR).Decomp601.DecompresH
        If8˜32bpp CMF Video.Decomp601.Decompress   $(VIDDIR).Decomp601.Decompress
        CMF Video.Decomp601.Resources.Info         $(VIDDIR).Decomp601.Info

Decomp602_Decode:     Video.Decomp602.DecompresH \
                      Video.Decomp602.Decompress \
                      Video.Decomp602.Resources.Info
        MMD $(VIDDIR).Decomp602
        If4˜16bpp CMF Video.Decomp602.DecompresH   $(VIDDIR).Decomp602.DecompresH
        If8˜32bpp CMF Video.Decomp602.Decompress   $(VIDDIR).Decomp602.Decompress
        CMF Video.Decomp602.Resources.Info         $(VIDDIR).Decomp602.Info

Decomp602_Encode:     Video.Decomp602.Dec21 \
                      Video.Decomp602.Dec24
        CMF Video.Decomp602.Dec21                  $(VIDDIR).Decomp602.Dec21
        CMF Video.Decomp602.Dec24                  $(VIDDIR).Decomp602.Dec24

Decomp603_Decode:     Video.Decomp603.DecompresH \
                      Video.Decomp603.Decompress \
                      Video.Decomp603.Resources.Info
        MMD $(VIDDIR).Decomp603
        If4˜16bpp CMF Video.Decomp603.DecompresH   $(VIDDIR).Decomp603.DecompresH
        If8˜32bpp CMF Video.Decomp603.Decompress   $(VIDDIR).Decomp603.Decompress
        CMF Video.Decomp603.Resources.Info         $(VIDDIR).Decomp603.Info

Decomp604_Decode:     Video.Decomp604.Dec8 \
                      Video.Decomp604.DecompresH \
                      Video.Decomp604.Decompress \
                      Video.Decomp604.Resources.Info
        MMD $(VIDDIR).Decomp604
        CMF Video.Decomp604.Dec8                   $(VIDDIR).Decomp604.Dec8
        If4˜16bpp CMF Video.Decomp604.DecompresH   $(VIDDIR).Decomp604.DecompresH
        If8˜32bpp CMF Video.Decomp604.Decompress   $(VIDDIR).Decomp604.Decompress
        CMF Video.Decomp604.Resources.Info         $(VIDDIR).Decomp604.Info

Decomp605_Decode:     Video.Decomp605.DecompresB \
                      Video.Decomp605.DecompresH \
                      Video.Decomp605.Decompress \
                      Video.Decomp605.Resources.Info
        MMD $(VIDDIR).Decomp605
        If1˜8bpp CMF Video.Decomp605.DecompresB    $(VIDDIR).Decomp605.DecompresB
        If4˜16bpp CMF Video.Decomp605.DecompresH   $(VIDDIR).Decomp605.DecompresH
        If8˜32bpp CMF Video.Decomp605.Decompress   $(VIDDIR).Decomp605.Decompress
        CMF Video.Decomp605.Resources.Info         $(VIDDIR).Decomp605.Info

Decomp606_Decode:     Video.Decomp606.Dec8 \
                      Video.Decomp606.DecompresH \
                      Video.Decomp606.Decompress \
                      Video.Decomp606.Resources.Info
        MMD $(VIDDIR).Decomp606
        CMF Video.Decomp606.Dec8                   $(VIDDIR).Decomp606.Dec8
        If4˜16bpp CMF Video.Decomp606.DecompresH   $(VIDDIR).Decomp606.DecompresH
        If8˜32bpp CMF Video.Decomp606.Decompress   $(VIDDIR).Decomp606.Decompress
        CMF Video.Decomp606.Resources.Info         $(VIDDIR).Decomp606.Info

Decomp607_Decode:     Video.Decomp607.Dec8 \
                      Video.Decomp607.Decompress \
                      Video.Decomp607.Resources.Info
        MMD $(VIDDIR).Decomp607
        CMF Video.Decomp607.Dec8                   $(VIDDIR).Decomp607.Dec8
        If8˜32bpp CMF Video.Decomp607.Decompress   $(VIDDIR).Decomp607.Decompress
        CMF Video.Decomp607.Resources.Info         $(VIDDIR).Decomp607.Info

Decomp608_Decode:     Video.Decomp608.Dec24 \
                      Video.Decomp608.DecompresH \
                      Video.Decomp608.Decompress \
                      Video.Decomp608.Resources.Info
        MMD $(VIDDIR).Decomp608
        If32bpp CMF Video.Decomp608.Dec24          $(VIDDIR).Decomp608.Dec24
        If4˜16bpp CMF Video.Decomp608.DecompresH   $(VIDDIR).Decomp608.DecompresH
        If8˜32bpp CMF Video.Decomp608.Decompress   $(VIDDIR).Decomp608.Decompress
        CMF Video.Decomp608.Resources.Info         $(VIDDIR).Decomp608.Info

Decomp609_Decode:     Video.Decomp609.DecompresH \
                      Video.Decomp609.Decompress \
                      Video.Decomp609.Resources.Info
        MMD $(VIDDIR).Decomp609
        If4˜16bpp CMF Video.Decomp609.DecompresH   $(VIDDIR).Decomp609.DecompresH
        If8˜32bpp CMF Video.Decomp609.Decompress   $(VIDDIR).Decomp609.Decompress
        CMF Video.Decomp609.Resources.Info         $(VIDDIR).Decomp609.Info

Decomp610_Decode:     Video.Decomp610.Decompress \
                      Video.Decomp610.Resources.Info
        MMD $(VIDDIR).Decomp610
        If8˜32bpp CMF Video.Decomp610.Decompress   $(VIDDIR).Decomp610.Decompress
        CMF Video.Decomp610.Resources.Info         $(VIDDIR).Decomp610.Info

Decomp613_Decode:     Video.Decomp613.Decompress \
                      Video.Decomp613.Resources.Info
        MMD $(VIDDIR).Decomp613
        If8˜32bpp CMF Video.Decomp613.Decompress   $(VIDDIR).Decomp613.Decompress
        CMF Video.Decomp613.Resources.Info         $(VIDDIR).Decomp613.Info

Decomp614_Decode:     Video.Decomp614.DecompresH \
                      Video.Decomp614.Decompress \
                      Video.Decomp614.Resources.Info
        MMD $(VIDDIR).Decomp614
        If4˜16bpp CMF Video.Decomp614.DecompresH   $(VIDDIR).Decomp614.DecompresH
        If8˜32bpp CMF Video.Decomp614.Decompress   $(VIDDIR).Decomp614.Decompress
        CMF Video.Decomp614.Resources.Info         $(VIDDIR).Decomp614.Info

Decomp615_Decode:     Video.Decomp615.DecompresH \
                      Video.Decomp615.Decompress \
                      Video.Decomp615.Resources.Info
        MMD $(VIDDIR).Decomp615
        If4˜16bpp CMF Video.Decomp615.DecompresH   $(VIDDIR).Decomp615.DecompresH
        If8˜32bpp CMF Video.Decomp615.Decompress   $(VIDDIR).Decomp615.Decompress
        CMF Video.Decomp615.Resources.Info         $(VIDDIR).Decomp615.Info

Decomp622_Decode:     Video.Decomp622.Dec8 \
                      Video.Decomp622.Decompress \
                      Video.Decomp622.Resources.Info
        MMD $(VIDDIR).Decomp622
        CMF Video.Decomp622.Dec8                   $(VIDDIR).Decomp622.Dec8
        If8˜32bpp CMF Video.Decomp622.Decompress   $(VIDDIR).Decomp622.Decompress
        CMF Video.Decomp622.Resources.Info         $(VIDDIR).Decomp622.Info

Decomp623_Decode:     Video.Decomp623.Dec8 \
                      Video.Decomp623.Decompress \
                      Video.Decomp623.Resources.Info
        MMD $(VIDDIR).Decomp623
        CMF Video.Decomp623.Dec8                   $(VIDDIR).Decomp623.Dec8
        If8˜32bpp CMF Video.Decomp623.Decompress   $(VIDDIR).Decomp623.Decompress
        CMF Video.Decomp623.Resources.Info         $(VIDDIR).Decomp623.Info

Decomp624_Decode:     Video.Decomp624.DecompresB \
                      Video.Decomp624.DecompresH \
                      Video.Decomp624.Decompress \
                      Video.Decomp624.Resources.Info
        MMD $(VIDDIR).Decomp624
        If1˜8bpp CMF Video.Decomp624.DecompresB    $(VIDDIR).Decomp624.DecompresB
        If4˜16bpp CMF Video.Decomp624.DecompresH   $(VIDDIR).Decomp624.DecompresH
        If8˜32bpp CMF Video.Decomp624.Decompress   $(VIDDIR).Decomp624.Decompress
        CMF Video.Decomp624.Resources.Info         $(VIDDIR).Decomp624.Info

Decomp626_Decode:     Video.Decomp626.Dec21 \
                      Video.Decomp626.Dec24 \
                      Video.Decomp626.Decompress \
                      Video.Decomp626.Resources.Info
        MMD $(VIDDIR).Decomp626
        If32bpp CMF Video.Decomp626.Dec21          $(VIDDIR).Decomp626.Dec21
        If32bpp CMF Video.Decomp626.Dec24          $(VIDDIR).Decomp626.Dec24
        If8˜32bpp CMF Video.Decomp626.Decompress   $(VIDDIR).Decomp626.Decompress
        CMF Video.Decomp626.Resources.Info         $(VIDDIR).Decomp626.Info

Decomp627_Decode:     Video.Decomp627.Decompress \
                      Video.Decomp627.Resources.Info
        MMD $(VIDDIR).Decomp627
        If8˜32bpp CMF Video.Decomp627.Decompress   $(VIDDIR).Decomp627.Decompress
        CMF Video.Decomp627.Resources.Info         $(VIDDIR).Decomp627.Info

Decomp628_Decode:     Video.Decomp628.Decompress \
                      Video.Decomp628.Resources.Info
        MMD $(VIDDIR).Decomp628
        If8˜32bpp CMF Video.Decomp628.Decompress   $(VIDDIR).Decomp628.Decompress
        CMF Video.Decomp628.Resources.Info         $(VIDDIR).Decomp628.Info

Decomp629_Decode:     Video.Decomp629.Decompress \
                      Video.Decomp629.Resources.Info
        MMD $(VIDDIR).Decomp629
        If8˜32bpp CMF Video.Decomp629.Decompress   $(VIDDIR).Decomp629.Decompress
        CMF Video.Decomp629.Resources.Info         $(VIDDIR).Decomp629.Info

Decomp630_Decode:     Video.Decomp630.Resources.!RunImage \
                      Video.Decomp630.Resources.Info
        MMD $(VIDDIR).Decomp630
        CMF Video.Decomp630.Resources.!RunImage    $(VIDDIR).Decomp630.!RunImage
        CMF Video.Decomp630.Resources.Info         $(VIDDIR).Decomp630.Info

Decomp699_Decode:     Video.Decomp699.Decompress \
                      Video.Decomp699.Resources.Info
        MMD $(VIDDIR).Decomp699
        If8˜32bpp CMF Video.Decomp699.Decompress   $(VIDDIR).Decomp699.Decompress
        CMF Video.Decomp699.Resources.Info         $(VIDDIR).Decomp699.Info
Video.Decomp699.Decompress: Video.Decomp699.bas.MakeDecomp
        /Video.Decomp699.bas.MakeDecomp

Decomp800_Decode:     Video.Decomp800.Decompress \
                      Video.Decomp800.Resources.Info
        MMD $(VIDDIR).Decomp800
        If8˜32bpp CMF Video.Decomp800.Decompress   $(VIDDIR).Decomp800.Decompress
        CMF Video.Decomp800.Resources.Info         $(VIDDIR).Decomp800.Info

Decomp800_Encode:     Video.Decomp800.bas.BatchComp \
                      Video.Decomp800.bas.Comp800 \
                      Video.Decomp800.Compress \
                      Video.Decomp800.Resources.!ReadMe
        MMD $(VIDDIR).Decomp800
        CMF Video.Decomp800.bas.BatchComp          $(VIDDIR).Decomp800.BatchComp
        CMF Video.Decomp800.bas.Comp800            $(VIDDIR).Decomp800.Comp800
        CMF Video.Decomp800.Compress               $(VIDDIR).Decomp800.Compress
        CMF Video.Decomp800.Resources.!ReadMe      $(VIDDIR).Decomp800.!ReadMe

Decomp802_Decode:     Video.Decomp802.Decompress \
                      Video.Decomp802.Resources.Info
        MMD $(VIDDIR).Decomp802
        If8˜32bpp CMF Video.Decomp802.Decompress   $(VIDDIR).Decomp802.Decompress
        CMF Video.Decomp802.Resources.Info         $(VIDDIR).Decomp802.Info

Decomp802_Encode:     Video.Decomp802.bas.BatchComp \
                      Video.Decomp802.bas.Comp802 \
                      Video.Decomp802.Compress \
                      Video.Decomp802.Resources.!ReadMe
        MMD $(VIDDIR).Decomp802
        CMF Video.Decomp802.bas.BatchComp          $(VIDDIR).Decomp802.BatchComp
        CMF Video.Decomp802.bas.Comp802            $(VIDDIR).Decomp802.Comp802
        CMF Video.Decomp802.Compress               $(VIDDIR).Decomp802.Compress
        CMF Video.Decomp802.Resources.!ReadMe      $(VIDDIR).Decomp802.!ReadMe

Decomp900_Decode:     Video.Decomp900.Decompress \
                      Video.Decomp900.Resources.Info
        MMD $(VIDDIR).Decomp900
        If8˜32bpp CMF Video.Decomp900.Decompress   $(VIDDIR).Decomp900.Decompress
        CMF Video.Decomp900.Resources.Info         $(VIDDIR).Decomp900.Info

Decomp901_Decode:     Video.Decomp901.Decompress \
                      Video.Decomp901.Resources.Info
        MMD $(VIDDIR).Decomp901
        If8˜32bpp CMF Video.Decomp901.Decompress   $(VIDDIR).Decomp901.Decompress
        CMF Video.Decomp901.Resources.Info         $(VIDDIR).Decomp901.Info
Video.Decomp901.Decompress:
        @Dir Video.Decomp901
        @amu_machine
        @Up 2

Decomp902_Decode:     Video.Decomp902.Dec16 \
                      Video.Decomp902.Decompress \
                      Video.Decomp902.Resources.Info
        MMD $(VIDDIR).Decomp902
        CMF Video.Decomp902.Dec16                  $(VIDDIR).Decomp902.Dec16
        If8˜32bpp CMF Video.Decomp902.Decompress   $(VIDDIR).Decomp902.Decompress
        CMF Video.Decomp902.Resources.Info         $(VIDDIR).Decomp902.Info
Video.Decomp902.Decompress:
        @Dir Video.Decomp902
        @amu_machine
        @Up 2

AVI_QT_WSS_Decode:    Fetchers.AVI_QT_WSS.Universal
        MMD $(FETCHDIR)
        CMF Fetchers.AVI_QT_WSS.Universal          $(FETCHDIR).Universal

PNA_Decode:           Fetchers.PNA.PNA
        MMD $(FETCHDIR)
        CMF Fetchers.PNA.PNA                       $(FETCHDIR).PNA

VPhone_Decode:        Fetchers.VPhone.VPhone
        MMD $(FETCHDIR)
        CMF Fetchers.VPhone.VPhone                 $(FETCHDIR).VPhone

AVI_IMS_Decode:       Fetchers.AVI_IMS.IMS_AVI
        MMD $(FETCHDIR)
        CMF Fetchers.AVI_IMS.IMS_AVI               $(FETCHDIR).IMS_AVI

# ------------------------------------------------------------------------------
# Dynamic dependencies:
