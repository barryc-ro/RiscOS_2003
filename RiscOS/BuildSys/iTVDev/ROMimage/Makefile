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
 ShareFS \
 FwSTB \
 NetConfig \
 ATMstbL \
 Diagnostic

#
# Build rules:
#
install: ${MODULES}
        ${MKIMAGE} { < ForBigSplt }
        settype <install$dir>.Images.iTVDev/007 FE5

Diagnostic:
        @seteval PositionInRom &03C00000
        @setolength File$Length ${INSTDIR}.iTVDev.Diagnostic
        @echo Diagnostic                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ATMstbL:
        @setolength File$Length ${INSTDIR}.iTVDev.ATMstbL
        @echo ATMstbL                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NetConfig:
        @setolength File$Length ${INSTDIR}.iTVDev.NetConfig
        @echo NetConfig                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FwSTB:
        @setolength File$Length ${INSTDIR}.iTVDev.FwSTB
        @echo FwSTB                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ShareFS:
        @setolength File$Length ${INSTDIR}.iTVDev.ShareFS
        @echo ShareFS                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.iTVDev.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.iTVDev.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.iTVDev.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSReset:
        @setolength File$Length ${INSTDIR}.iTVDev.CMOSReset
        @echo CMOSReset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

