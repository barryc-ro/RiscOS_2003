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
 UART \
 CMOSReset \
 EMCMsgs

#
# Build rules:
#
install: ${MODULES}
        ${MKIMAGE} { < ForBigSplt }
        settype <install$dir>.Images.EMCRom/014 FE5

EMCMsgs:
        @seteval PositionInRom &03C00000
        @setolength File$Length ${INSTDIR}.EMCRom.EMCMsgs
        @echo EMCMsgs                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.EMCRom.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

UART:
        @setolength File$Length ${INSTDIR}.EMCRom.UART
        @echo UART                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

