# Makefile for ROMImage

CD      = dir
LD      = link
MKDIR   = cdir
MKIMAGE = BigSplit1

SRCDIR  = <src$dir>
INSTDIR = <install$dir>
CLIBDIR = <install$dir>.ROM.Lib
ABSSYM  = RISC_OSLib:o.AbsSym

MODULES = \
 Standby \
 ModeFiles \
 DynAreas \
 Interlace \
 TinyStubs \
 PPrimer \
 STBPrint \
 PDriver \
 PDumperE2 \
 PDumperLJ \
 PDumperIW \
 PDumperDM \
 PDumperSpt \
 PDriverPS \
 PDriverDP \
 MakePSFont \
 NFSSTB \
 Internet \
 MManager \
 AUNMsgs \
 BootCmds \
 DrawFile \
 ScrModes \
 RISC_OSLib \
 Percussion \
 StringLib \
 WaveSynth \
 FilterMgr \
 TaskWindow \
 SystemDevs \
 Super \
 Squash \
 SprExtend \
 Sound2 \
 Sound1 \
 Sound0 \
 SerialSpt \
 Serial \
 ScrBlank \
 RTCAdjust \
 ROMFonts \
 PipeFS \
 IntKey \
 Parallel \
 Obey \
 NHTwitter \
 Joystick \
 Inter \
 ITable \
 IIC \
 Hourglass \
 GameModes2 \
 FPEmulator \
 Fonts \
 RAMFS \
 FileCore \
 DrawMod \
 DragAnObj \
 DragASprit \
 DeviceFS \
 Debugger \
 Colours \
 Buffers \
 BASICTrans \
 BASIC105 \
 ARM \
 PS2Driver \
 Desktop \
 Wimp \
 USA \
 UK \
 TerrMgr \
 MsgTrans \
 Messages \
 ResourceFS \
 FileSwitch \
 CMOSreset \
 Podule \
 Kernel

#
# Build rules:
#
install_rom: ${MODULES} dirs
        ${MKIMAGE} { < ForBigSplt }

dirs:
        ${MKDIR} ${CLIBDIR}.linked
        ${MKDIR} ${CLIBDIR}.map

Kernel:
        @seteval PositionInRom &03800000
        @setolength File$Length ${INSTDIR}.ROM.Omega.Kernel
        @echo Kernel                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) 

Podule:
        @setolength File$Length ${INSTDIR}.ROM.Omega.Podule
        @echo Podule                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CMOSreset:
        @setolength File$Length ${INSTDIR}.ROM.STB.CMOSreset
        @echo CMOSreset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FileSwitch:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.FileSwitch
        @echo FileSwitch                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ResourceFS:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.ResourceFS
        @echo ResourceFS                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Messages:
        @setolength File$Length ${INSTDIR}.ROM.Omega2.Messages
        @echo Messages                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

MsgTrans:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.MsgTrans
        @echo MsgTrans                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

TerrMgr:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.TerrMgr
        @echo TerrMgr                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

UK:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.UK
        @echo UK                         <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

USA:
        @setolength File$Length ${INSTDIR}.ROM.Omega.USA
        @echo USA                        <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Wimp:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Wimp
        @echo Wimp                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Desktop:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Desktop
        @echo Desktop                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PS2Driver:
        @setolength File$Length ${INSTDIR}.ROM.Omega.PS2Driver
        @echo PS2Driver                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ARM:
        @setolength File$Length ${INSTDIR}.ROM.Omega.ARM
        @echo ARM                        <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

BASIC105:
        @setolength File$Length ${INSTDIR}.ROM.Programmer.BASIC105
        @echo BASIC105                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

BASICTrans:
        @setolength File$Length ${INSTDIR}.ROM.Programmer.BASICTrans
        @echo BASICTrans                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Buffers:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Buffers
        @echo Buffers                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Colours:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Colours
        @echo Colours                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Debugger:
        @setolength File$Length ${INSTDIR}.ROM.Omega.Debugger
        @echo Debugger                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

DeviceFS:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.DeviceFS
        @echo DeviceFS                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

DragASprit:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.DragASprit
        @echo DragASprit                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

DragAnObj:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.DragAnObj
        @echo DragAnObj                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

DrawMod:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.DrawMod
        @echo DrawMod                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FileCore:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.FileCore
        @echo FileCore                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

RAMFS:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.RAMFS
        @echo RAMFS                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Fonts:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Fonts
        @echo Fonts                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FPEmulator:
        @setolength File$Length ${INSTDIR}.ROM.Omega.FPEmulator
        @echo FPEmulator                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

GameModes2:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.GameModes2
        @echo GameModes2                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Hourglass:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Hourglass
        @echo Hourglass                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

IIC:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.IIC
        @echo IIC                        <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ITable:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.ITable
        @echo ITable                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Inter:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Inter
        @echo Inter                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Joystick:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Joystick
        @echo Joystick                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NHTwitter:
        @setolength File$Length ${INSTDIR}.ROM.STB.NHTwitter
        @echo NHTwitter                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Obey:
        @setolength File$Length ${INSTDIR}.ROM.Programmer.Obey
        @echo Obey                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Parallel:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Parallel
        @echo Parallel                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

IntKey:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.IntKey
        @echo IntKey                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PipeFS:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.PipeFS
        @echo PipeFS                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ROMFonts:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.ROMFonts
        @echo ROMFonts                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

RTCAdjust:
        @setolength File$Length ${INSTDIR}.ROM.Omega.RTCAdjust
        @echo RTCAdjust                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ScrBlank:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.ScrBlank
        @echo ScrBlank                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Serial:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Serial
        @echo Serial                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SerialSpt:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.SerialSpt
        @echo SerialSpt                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Sound0:
        @setolength File$Length ${INSTDIR}.ROM.Omega.Sound0
        @echo Sound0                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Sound1:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Sound1
        @echo Sound1                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Sound2:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Sound2
        @echo Sound2                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SprExtend:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.SprExtend
        @echo SprExtend                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Squash:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Squash
        @echo Squash                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Super:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Super
        @echo Super                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SystemDevs:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.SystemDevs
        @echo SystemDevs                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

TaskWindow:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.TaskWindow
        @echo TaskWindow                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FilterMgr:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.FilterMgr
        @echo FilterMgr                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

WaveSynth:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.WaveSynth
        @echo WaveSynth                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

StringLib:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.StringLib
        @echo StringLib                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Percussion:
        @setolength File$Length ${INSTDIR}.ROM.OS_Core.Percussion
        @echo Percussion                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

RISC_OSLib:
        @${CD} ${SRCDIR}.Lib.RISC_OSLib
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.RISC_OSLib
        @echo RISC_OSLib                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ScrModes:
        @${CD} ${SRCDIR}.OS_Core.Video.UserI.ScrModes
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.ScrModes
        @echo ScrModes                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

DrawFile:
        @${CD} ${SRCDIR}.OS_Core.Video.Render.DrawFile
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.DrawFile
        @echo DrawFile                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

BootCmds:
        @${CD} ${SRCDIR}.OS_Core.BootCmds
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.BootCmds
        @echo BootCmds                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

AUNMsgs:
        @setolength File$Length ${INSTDIR}.ROM.Networking.AUNMsgs
        @echo AUNMsgs                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

MManager:
        @setolength File$Length ${INSTDIR}.ROM.Networking.MManager
        @echo MManager                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Internet:
        @${CD} ${SRCDIR}.Networking.AUN.Internet.build
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.Internet
        @echo Internet                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NFSSTB:
        @${CD} ${SRCDIR}.Networking.NFS
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.NFSSTB
        @echo NFSSTB                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

MakePSFont:
        @${CD} ${SRCDIR}.Printing.Printers.Modules.MakePSFont
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.MakePSFont
        @echo MakePSFont                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDriverDP:
        @setolength File$Length ${INSTDIR}.ROM.Printing.PDriverDP
        @echo PDriverDP                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDriverPS:
        @setolength File$Length ${INSTDIR}.ROM.Printing.PDriverPS
        @echo PDriverPS                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperSpt:
        @setolength File$Length ${INSTDIR}.ROM.Printing.PDumperSpt
        @echo PDumperSpt                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperDM:
        @setolength File$Length ${INSTDIR}.ROM.Printing.PDumperDM
        @echo PDumperDM                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperIW:
        @setolength File$Length ${INSTDIR}.ROM.Printing.PDumperIW
        @echo PDumperIW                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperLJ:
        @setolength File$Length ${INSTDIR}.ROM.Printing.PDumperLJ
        @echo PDumperLJ                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperE2:
        @setolength File$Length ${INSTDIR}.ROM.Printing.PDumperE2
        @echo PDumperE2                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDriver:
        @setolength File$Length ${INSTDIR}.ROM.Printing.PDriver
        @echo PDriver                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

STBPrint:
        @${CD} ${SRCDIR}.STB.Printing.STBPrint
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.STBPrint
        @echo STBPrint                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PPrimer:
        @${CD} ${SRCDIR}.STB.Printing.PPrimer
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.PPrimer
        @echo PPrimer                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

TinyStubs:
        @${CD} ${SRCDIR}.Import.Aquarius.TinyStubs
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.TinyStubs
        @echo TinyStubs                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Interlace:
        @setolength File$Length ${INSTDIR}.ROM.Omega.Interlace
        @echo Interlace                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

DynAreas:
        @setolength File$Length ${INSTDIR}.ROM.Omega.DynAreas
        @echo DynAreas                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ModeFiles:
        @setolength File$Length ${INSTDIR}.ROM.Omega.ModeFiles
        @echo ModeFiles                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Standby:
        @${CD} ${SRCDIR}.STB.Misc.Standby
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.Standby
        @echo Standby                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

