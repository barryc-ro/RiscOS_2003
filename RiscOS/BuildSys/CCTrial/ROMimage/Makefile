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
 CMOSReset \
 CMOSReset \
 CMOSReset \
 CMOSReset \
 CMOSReset \
 CMOSReset \
 NetConfig \
 ATMstbL \
 Diagnostic

#
# Build rules:
#
install: ${MODULES}
        ${MKIMAGE} { < ForBigSplt }
        settype <install$dir>.Images.Trial/008 FE5

Diagnostic:
        @seteval PositionInRom &03C00000
        @setolength File$Length ${INSTDIR}.CCTrial.Diagnostic
        @echo Diagnostic                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ATMstbL:
        @setolength File$Length ${INSTDIR}.CCTrial.ATMstbL
        @echo ATMstbL                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NetConfig:
        @setolength File$Length ${INSTDIR}.CCTrial.NetConfig
        @echo NetConfig                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.CCTrial.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.CCTrial.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.CCTrial.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.CCTrial.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.CCTrial.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.CCTrial.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

