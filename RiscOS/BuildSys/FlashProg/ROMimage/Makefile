# Makefile for ROMImage

CD      = dir
LD      = link
MKDIR   = cdir
MKIMAGE = MakeExpROM

SRCDIR  = <src$dir>
INSTDIR = <install$dir>
CLIBDIR = <install$dir>.
ABSSYM  = RISC_OSLib:o.AbsSym

MODULES = \
 FlashApp \
 FlashProg

#
# Build rules:
#
install: ${MODULES}
        ${MKIMAGE} { < ForBigSplt }
        settype <install$dir>.Images.FlashP/011 FE5

FlashProg:
        @seteval PositionInRom &03C00000
        @setolength File$Length ${INSTDIR}.FlashProg.FlashProg
        @echo FlashProg                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FlashApp:
        @setolength File$Length ${INSTDIR}.FlashProg.FlashApp
        @echo FlashApp                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

