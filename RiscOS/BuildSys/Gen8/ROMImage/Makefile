# Makefile for ROMImage

CD      = dir
LD      = link
MKDIR   = cdir
MKIMAGE = BigSplit1

SRCDIR  = <src$dir>
INSTDIR = <install$dir>
CLIBDIR = <install$dir>.<Build>.Lib
ABSSYM  = RISC_OSLib:o.AbsSym

MODULES = \
 EnsureLine \
 ProgressUI \
 StoredInfo \
 NCDialUI \
 NCDialler \
 Watchdog \
 PPP \
 UART \
 ScrSaver \
 CSFS \
 ISPCapture \
 NCMAlite \
 Standby \
 FrontPanel \
 LED \
 MultiMedia \
 CodecCtl \
 MPlay \
 ESPDrvrsAS \
 Synth \
 SampleExp \
 SoundFX \
 SSound \
 ScreenFade \
 IconHigh \
 RedrawMgr \
 NCKeyboard \
 KeyWatch \
 NCFresco \
 Resolver \
 WebGopher \
 webftp \
 http \
 SoakTest \
 NCOptions \
 FilePlay \
 SoundFile \
 ReplaySpt \
 msadpcm \
 TaskModule \
 CacheFS \
 FileWatch \
 NetTime \
 ParaJoy \
 RCABlast \
 IRRelPtr \
 IRKB4 \
 IRKeyboard \
 IRHandset \
 CRK93H1 \
 IRDefault \
 IRDirector \
 NCBootblk \
 Cron \
 SCInter \
 SCTrans \
 Serial \
 ModeFiles \
 ShrinkWrap \
 DynAreas \
 NHTwitter \
 Interlace \
 ToolAction \
 Menu \
 Window \
 Toolbox \
 PPrimer \
 PDriver \
 PDumperLE \
 PDumperE2 \
 PDumperLJ \
 PDumperIW \
 PDumperDM \
 PDumperSpt \
 PDriverPS \
 PDriverDP \
 MakePSFont \
 Internet \
 MManager \
 AUNMsgs \
 BootCmds \
 ScrModes \
 WaveSynth \
 FilterMgr \
 SystemDevs \
 Super \
 Squash \
 SprExtend \
 Sound2 \
 Sound1 \
 Sound0 \
 ScrBlank \
 ROMFonts \
 IntKey \
 Parallel \
 Obey \
 Inter \
 ITable \
 Hourglass \
 FPEmulator \
 Fonts \
 FileCore \
 DrawMod \
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
 UniqueID \
 NVRAM \
 TinyStubs \
 RISC_OSLib \
 MsgTrans \
 Messages \
 ResourceFS \
 FileSwitch \
 OSVersion \
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
        @setolength File$Length ${INSTDIR}.<Build>.RCA.Kernel
        @echo Kernel                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) 

Podule:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.Podule
        @echo Podule                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

OSVersion:
        @setolength File$Length ${INSTDIR}.<Build>.OS_Core.OSVersion
        @echo OSVersion                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FileSwitch:
        @setolength File$Length ${INSTDIR}.<Build>.FileSys.FileSwitch
        @echo FileSwitch                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ResourceFS:
        @setolength File$Length ${INSTDIR}.<Build>.FileSys.ResourceFS
        @echo ResourceFS                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Messages:
        @setolength File$Length ${INSTDIR}.<Build>.Internat.Messages
        @echo Messages                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

MsgTrans:
        @setolength File$Length ${INSTDIR}.<Build>.Internat.MsgTrans
        @echo MsgTrans                   <PositionInRom>  <File$Length>
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

TinyStubs:
        @${CD} ${SRCDIR}.Toolbox.TinyStubs
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.TinyStubs
        @echo TinyStubs                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NVRAM:
        @${CD} ${SRCDIR}.HWSupport.NVRAM
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.NVRAM
        @echo NVRAM                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

UniqueID:
        @${CD} ${SRCDIR}.Programmer.UniqueID
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.UniqueID
        @echo UniqueID                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

TerrMgr:
        @setolength File$Length ${INSTDIR}.<Build>.Internat.TerrMgr
        @echo TerrMgr                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

UK:
        @setolength File$Length ${INSTDIR}.<Build>.Internat.UK
        @echo UK                         <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

USA:
        @setolength File$Length ${INSTDIR}.<Build>.Internat.USA
        @echo USA                        <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Wimp:
        @setolength File$Length ${INSTDIR}.<Build>.Desktop.Wimp
        @echo Wimp                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Desktop:
        @setolength File$Length ${INSTDIR}.<Build>.Desktop.Desktop
        @echo Desktop                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PS2Driver:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.PS2Driver
        @echo PS2Driver                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ARM:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.ARM
        @echo ARM                        <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

BASIC105:
        @setolength File$Length ${INSTDIR}.<Build>.Programmer.BASIC105
        @echo BASIC105                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

BASICTrans:
        @setolength File$Length ${INSTDIR}.<Build>.Programmer.BASICTrans
        @echo BASICTrans                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Buffers:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.Buffers
        @echo Buffers                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Colours:
        @setolength File$Length ${INSTDIR}.<Build>.Video.Colours
        @echo Colours                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Debugger:
        @setolength File$Length ${INSTDIR}.<Build>.Programmer.Debugger
        @echo Debugger                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

DeviceFS:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.DeviceFS
        @echo DeviceFS                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

DrawMod:
        @setolength File$Length ${INSTDIR}.<Build>.Video.DrawMod
        @echo DrawMod                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FileCore:
        @setolength File$Length ${INSTDIR}.<Build>.FileSys.FileCore
        @echo FileCore                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Fonts:
        @setolength File$Length ${INSTDIR}.<Build>.Video.Fonts
        @echo Fonts                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FPEmulator:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.FPEmulator
        @echo FPEmulator                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Hourglass:
        @setolength File$Length ${INSTDIR}.<Build>.Video.Hourglass
        @echo Hourglass                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ITable:
        @setolength File$Length ${INSTDIR}.<Build>.Video.ITable
        @echo ITable                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Inter:
        @setolength File$Length ${INSTDIR}.<Build>.Internat.Inter
        @echo Inter                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Obey:
        @setolength File$Length ${INSTDIR}.<Build>.Programmer.Obey
        @echo Obey                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Parallel:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.Parallel
        @echo Parallel                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

IntKey:
        @setolength File$Length ${INSTDIR}.<Build>.Internat.IntKey
        @echo IntKey                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ROMFonts:
        @setolength File$Length ${INSTDIR}.<Build>.Video.ROMFonts
        @echo ROMFonts                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ScrBlank:
        @setolength File$Length ${INSTDIR}.<Build>.Video.ScrBlank
        @echo ScrBlank                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Sound0:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.Sound0
        @echo Sound0                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Sound1:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.Sound1
        @echo Sound1                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Sound2:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.Sound2
        @echo Sound2                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SprExtend:
        @setolength File$Length ${INSTDIR}.<Build>.Video.SprExtend
        @echo SprExtend                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Squash:
        @setolength File$Length ${INSTDIR}.<Build>.Programmer.Squash
        @echo Squash                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Super:
        @setolength File$Length ${INSTDIR}.<Build>.Video.Super
        @echo Super                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SystemDevs:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.SystemDevs
        @echo SystemDevs                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FilterMgr:
        @setolength File$Length ${INSTDIR}.<Build>.Desktop.FilterMgr
        @echo FilterMgr                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

WaveSynth:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.WaveSynth
        @echo WaveSynth                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ScrModes:
        @${CD} ${SRCDIR}.Video.UserI.ScrModes
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.ScrModes
        @echo ScrModes                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

BootCmds:
        @${CD} ${SRCDIR}.Programmer.BootCmds
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.BootCmds
        @echo BootCmds                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

AUNMsgs:
        @setolength File$Length ${INSTDIR}.<Build>.Networking.AUNMsgs
        @echo AUNMsgs                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

MManager:
        @setolength File$Length ${INSTDIR}.<Build>.Networking.MManager
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

MakePSFont:
        @${CD} ${SRCDIR}.Printing.Modules.MakePSFont
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.MakePSFont
        @echo MakePSFont                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDriverDP:
        @setolength File$Length ${INSTDIR}.<Build>.Printing.PDriverDP
        @echo PDriverDP                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDriverPS:
        @setolength File$Length ${INSTDIR}.<Build>.Printing.PDriverPS
        @echo PDriverPS                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperSpt:
        @setolength File$Length ${INSTDIR}.<Build>.Printing.PDumperSpt
        @echo PDumperSpt                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperDM:
        @setolength File$Length ${INSTDIR}.<Build>.Printing.PDumperDM
        @echo PDumperDM                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperIW:
        @setolength File$Length ${INSTDIR}.<Build>.Printing.PDumperIW
        @echo PDumperIW                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperLJ:
        @setolength File$Length ${INSTDIR}.<Build>.Printing.PDumperLJ
        @echo PDumperLJ                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperE2:
        @setolength File$Length ${INSTDIR}.<Build>.Printing.PDumperE2
        @echo PDumperE2                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDumperLE:
        @${CD} ${SRCDIR}.Printing.NewDumpers.PDumperLE
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.PDumperLE
        @echo PDumperLE                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PDriver:
        @setolength File$Length ${INSTDIR}.<Build>.Printing.PDriver
        @echo PDriver                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PPrimer:
        @${CD} ${SRCDIR}.Printing.PPrimer
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.PPrimer
        @echo PPrimer                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Toolbox:
        @${CD} ${SRCDIR}.Toolbox.Toolbox
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.Toolbox
        @echo Toolbox                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Window:
        @${CD} ${SRCDIR}.Toolbox.Window
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.Window
        @echo Window                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Menu:
        @${CD} ${SRCDIR}.Toolbox.Menu
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.Menu
        @echo Menu                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ToolAction:
        @${CD} ${SRCDIR}.Toolbox.ToolAction
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.ToolAction
        @echo ToolAction                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Interlace:
        @setolength File$Length ${INSTDIR}.<Build>.Video.Interlace
        @echo Interlace                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NHTwitter:
        @setolength File$Length ${INSTDIR}.<Build>.Video.NHTwitter
        @echo NHTwitter                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

DynAreas:
        @setolength File$Length ${INSTDIR}.<Build>.Programmer.DynAreas
        @echo DynAreas                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ShrinkWrap:
        @${CD} ${SRCDIR}.Programmer.ShrinkWrap
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.ShrinkWrap
        @echo ShrinkWrap                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ModeFiles:
        @setolength File$Length ${INSTDIR}.<Build>.Video.ModeFiles
        @echo ModeFiles                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Serial:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.Serial
        @echo Serial                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SCTrans:
        @${CD} ${SRCDIR}.HWSupport.SmartCard.SCTrans
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.SCTrans
        @echo SCTrans                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SCInter:
        @${CD} ${SRCDIR}.HWSupport.SmartCard.SCInter
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.SCInter
        @echo SCInter                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Cron:
        @setolength File$Length ${INSTDIR}.<Build>.Programmer.Cron
        @echo Cron                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NCBootblk:
        @${CD} ${SRCDIR}.Security.NCBootblk
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.NCBootblk
        @echo NCBootblk                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

IRDirector:
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDirector
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.IRDirector
        @echo IRDirector                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

IRDefault:
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.IRDefault
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.IRDefault
        @echo IRDefault                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CRK93H1:
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.CRK93H1
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.CRK93H1
        @echo CRK93H1                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

IRHandset:
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.IRHandset
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.IRHandset
        @echo IRHandset                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

IRKeyboard:
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.IRKeyboard
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.IRKeyboard
        @echo IRKeyboard                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

IRKB4:
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.IRKB4
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.IRKB4
        @echo IRKB4                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

IRRelPtr:
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.IRRelPtr
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.IRRelPtr
        @echo IRRelPtr                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

RCABlast:
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.RCABlast
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.RCABlast
        @echo RCABlast                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ParaJoy:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.ParaJoy
        @echo ParaJoy                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NetTime:
        @${CD} ${SRCDIR}.Networking.NetTime
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.NetTime
        @echo NetTime                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FileWatch:
        @${CD} ${SRCDIR}.Programmer.FileWatch
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.FileWatch
        @echo FileWatch                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CacheFS:
        @setolength File$Length ${INSTDIR}.<Build>.FileSys.CacheFS
        @echo CacheFS                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

TaskModule:
        @${CD} ${SRCDIR}.Desktop.TaskModule
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.TaskModule
        @echo TaskModule                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

msadpcm:
        @${CD} ${SRCDIR}.Audio.msadpcm
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.msadpcm
        @echo msadpcm                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ReplaySpt:
        @${CD} ${SRCDIR}.Audio.ReplaySpt
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.ReplaySpt
        @echo ReplaySpt                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SoundFile:
        @${CD} ${SRCDIR}.Audio.SoundFile
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.SoundFile
        @echo SoundFile                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FilePlay:
        @${CD} ${SRCDIR}.Audio.FilePlay
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.FilePlay
        @echo FilePlay                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NCOptions:
        @${CD} ${SRCDIR}.Apps.NCOptions
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.NCOptions
        @echo NCOptions                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SoakTest:
        @setolength File$Length ${INSTDIR}.<Build>.Test.SoakTest
        @echo SoakTest                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

http:
        @${CD} ${SRCDIR}.Networking.HTTP
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.http
        @echo http                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

webftp:
        @${CD} ${SRCDIR}.Networking.WebFTP
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.webftp
        @echo webftp                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

WebGopher:
        @${CD} ${SRCDIR}.Networking.WebGopher
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.WebGopher
        @echo WebGopher                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Resolver:
        @${CD} ${SRCDIR}.Networking.Resolver
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.Resolver
        @echo Resolver                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NCFresco:
        @${CD} ${SRCDIR}.Apps.NCFrescoRO
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.NCFresco
        @echo NCFresco                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

KeyWatch:
        @${CD} ${SRCDIR}.Programmer.KeyWatch
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.KeyWatch
        @echo KeyWatch                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NCKeyboard:
        @${CD} ${SRCDIR}.Apps.NCKeyboard
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.NCKeyboard
        @echo NCKeyboard                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

RedrawMgr:
        @${CD} ${SRCDIR}.Desktop.RedrawMgr
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.RedrawMgr
        @echo RedrawMgr                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

IconHigh:
        @${CD} ${SRCDIR}.Video.UserI.IconHigh
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.IconHigh
        @echo IconHigh                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ScreenFade:
        @${CD} ${SRCDIR}.Video.Render.ScreenFade
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.ScreenFade
        @echo ScreenFade                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SSound:
        @setolength File$Length ${INSTDIR}.<Build>.Audio.SSound
        @echo SSound                     <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SoundFX:
        @${CD} ${SRCDIR}.Audio.SoundFX
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.SoundFX
        @echo SoundFX                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

SampleExp:
        @${CD} ${SRCDIR}.Audio.SampleExp
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.SampleExp
        @echo SampleExp                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Synth:
        @setolength File$Length ${INSTDIR}.<Build>.Audio.Synth
        @echo Synth                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ESPDrvrsAS:
        @setolength File$Length ${INSTDIR}.<Build>.Audio.ESPDrvrsAS
        @echo ESPDrvrsAS                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

MPlay:
        @setolength File$Length ${INSTDIR}.<Build>.Audio.MPlay
        @echo MPlay                      <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CodecCtl:
        @${CD} ${SRCDIR}.Apps.CodecCtl
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.CodecCtl
        @echo CodecCtl                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

MultiMedia:
        @setolength File$Length ${INSTDIR}.<Build>.Apps.MultiMedia
        @echo MultiMedia                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

LED:
        @${CD} ${SRCDIR}.HWSupport.LED
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.LED
        @echo LED                        <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

FrontPanel:
        @${CD} ${SRCDIR}.HWSupport.FrontPanel
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.FrontPanel
        @echo FrontPanel                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Standby:
        @${CD} ${SRCDIR}.HWSupport.Standby
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.Standby
        @echo Standby                    <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NCMAlite:
        @${CD} ${SRCDIR}.Security.NCMAlite
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.NCMAlite
        @echo NCMAlite                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ISPCapture:
        @${CD} ${SRCDIR}.Security.ISPCapture
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.ISPCapture
        @echo ISPCapture                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

CSFS:
        @${CD} ${SRCDIR}.FileSys.CSFS
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.CSFS
        @echo CSFS                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ScrSaver:
        @${CD} ${SRCDIR}.Video.UserI.ScrSaver
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.ScrSaver
        @echo ScrSaver                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

UART:
        @setolength File$Length ${INSTDIR}.<Build>.HWSupport.UART
        @echo UART                       <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

PPP:
        @${CD} ${SRCDIR}.Networking.Modem.PPP
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.PPP
        @echo PPP                        <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

Watchdog:
        @${CD} ${SRCDIR}.Apps.Watchdog
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.Watchdog
        @echo Watchdog                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NCDialler:
        @${CD} ${SRCDIR}.Networking.Modem.NCDialler
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.NCDialler
        @echo NCDialler                  <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

NCDialUI:
        @${CD} ${SRCDIR}.Networking.Modem.NCDialUI
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.NCDialUI
        @echo NCDialUI                   <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

StoredInfo:
        @${CD} ${SRCDIR}.Test.StoredInfo
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.StoredInfo
        @echo StoredInfo                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

ProgressUI:
        @${CD} ${SRCDIR}.Video.UserI.ProgressUI
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.ProgressUI
        @echo ProgressUI                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

EnsureLine:
        @${CD} ${SRCDIR}.Networking.Modem.EnsureLine
        @set alias$amu_rom_link amu MACHINE=<Machine> LOCALE=<Locale> ADDRESS=<PositionInRom> rom_link %0
        amu_rom_link LINKDIR=${CLIBDIR}.linked
        @unset alias$amu_rom_link
        @back
        setolength File$Length ${CLIBDIR}.linked.EnsureLine
        @echo EnsureLine                 <PositionInRom>  <File$Length>
        @seteval PositionInRom PositionInRom + ((File$Length + 3) AND NOT 3) + 4

