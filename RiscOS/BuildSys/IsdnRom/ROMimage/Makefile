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
 CLIProt \
 Resolver \
 NCRegSS \
 NCRegistry \
 Caradon \
 EEROM \
 NCConfig \
 ISDN \
 PPP \
 DeviceFS \
 NCIsdn

#
# Build rules:
#
install: ${MODULES}
        ${MKIMAGE} { < ForBigSplt }
        settype <install$dir>.Images.Isdn/029 FE5

NCIsdn:
        @seteval PositionInRom &03C00000
        @setolength File$Length ${INSTDIR}.IsdnRom.NCIsdn
        @echo NCIsdn                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

DeviceFS:
        @setolength File$Length ${INSTDIR}.IsdnRom.DeviceFS
        @echo DeviceFS                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PPP:
        @setolength File$Length ${INSTDIR}.IsdnRom.PPP
        @echo PPP                        <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ISDN:
        @setolength File$Length ${INSTDIR}.IsdnRom.ISDN
        @echo ISDN                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NCConfig:
        @setolength File$Length ${INSTDIR}.IsdnRom.NCConfig
        @echo NCConfig                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

EEROM:
        @setolength File$Length ${INSTDIR}.IsdnRom.EEROM
        @echo EEROM                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Caradon:
        @setolength File$Length ${INSTDIR}.IsdnRom.Caradon
        @echo Caradon                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NCRegistry:
        @setolength File$Length ${INSTDIR}.IsdnRom.NCRegistry
        @echo NCRegistry                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NCRegSS:
        @setolength File$Length ${INSTDIR}.IsdnRom.NCRegSS
        @echo NCRegSS                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Resolver:
        @setolength File$Length ${INSTDIR}.IsdnRom.Resolver
        @echo Resolver                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CLIProt:
        @setolength File$Length ${INSTDIR}.IsdnRom.CLIProt
        @echo CLIProt                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

