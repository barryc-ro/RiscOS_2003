# System Makefile

CD      = dir
MKDIR   = cdir
SYSDIR  = <build$dir>.BuildSys
SRCDIR  = <src$dir>
INSTDIR = <install$dir>
MSGDIR  = ${SRCDIR}.OS_Core.Internat.Messages
RESDIR  = ${MSGDIR}.Resources.<Locale>.<System>.Resources
MAKE    = @amu ${MFLAGS} RESDIR=${RESDIR}

BUILD = \
 Standby \
 ModeFiles \
 DynamicAreas \
 Interlace \
 TinyStubs \
 tboxlib \
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
 BootCommands \
 DrawFile \
 ScreenModes \
 SharedCLibrary \
 Percussion \
 StringLib \
 WaveSynth \
 FilterManager \
 TaskWindow \
 SystemDevices \
 SuperSample \
 Squash \
 SpriteExtend \
 SoundScheduler \
 SoundChannels \
 SoundDMA \
 SerialDeviceSupport \
 SerialDeviceDriver \
 ScreenBlanker \
 RTCAdjust \
 ROMFonts \
 PipeFS \
 InternationalKeyboard \
 ParallelDeviceDriver \
 Obey \
 NHTwitter \
 Joystick \
 International \
 ITable \
 IIC \
 Hourglass \
 GameModes \
 FPEmulator \
 FontManager \
 RamFS \
 FileCore \
 DrawMod \
 DragAnObj \
 DragASprite \
 DeviceFS \
 Debugger \
 ColourTrans \
 BufferManager \
 BASIC \
 ARM \
 PS2Driver \
 Desktop \
 WindowManager \
 USA \
 UK \
 TerritoryManager \
 MessageTrans \
 Messages \
 ResourceFS \
 FileSwitch \
 CMOSReset \
 Podule \
 Kernel

EXPORT = \
 Portable \
 ShellCLI \
 SCSIFS \
 SCSI \
 HostFS \
 Free \
 Econet \
 DOSFS \
 DMAManager \
 DDT \
 CDFSDriver \
 CDFS \
 ATAPI \
 ARM3 \
 ADFS \
 ToolboxMF \
 OSLib \
 ShareD \
 HdrSrc

RESOURCE = \
 Help

#
# Build rules:
#
clean: ${BUILD}
        @echo clean: Complete

export: ${BUILD} ${EXPORT} dirs
        @echo export: Complete

resources: ${BUILD} ${RESOURCE} dirs
        @echo resources: Complete

# rom build
rom: ${BUILD} dirs
        @echo rom: Complete

install_rom: ROMImage
        @echo install_rom: Complete

# disc build
all: ${BUILD}
        @echo all: Complete

install: ${BUILD}
        @echo install: Complete

dirs:
        set Resource$Dir ${MSGDIR}.Resources.<Locale>.<System>
        ${MKDIR} <install$dir>
        ${MKDIR} <build$dir>.Export
        ${MKDIR} <build$dir>.Export.Hdr
        ${MKDIR} <build$dir>.Export.Hdr.Global
        ${MKDIR} <build$dir>.Export.Hdr.Interface
        ${MKDIR} <build$dir>.Export.Hdr.Interface2
        ${MKDIR} <build$dir>.Export.Lib
        ${MKDIR} ${MSGDIR}.Resources
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.${SYSTEM}
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.${SYSTEM}.Resources
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.${SYSTEM}.Resources2
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.${SYSTEM}.Apps
#
# Component rules:
#

Kernel:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.OS_Core.Kernel
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

ROMImage: ${BUILD}
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.Images
        @${CD} ${SYSDIR}.Omega2.ROMImage
        ${MAKE} INSTDIR=${INSTDIR}.Images

HdrSrc:
        @echo $@ ...
        @${CD} ${SRCDIR}.HdrSrc
        ${MAKE}

ShareD:
        @echo $@ ...
        @${CD} ${SRCDIR}.NetWorking.AUN.Access.ShareFS.ShareFS
        ${MAKE}

OSLib:
        @echo $@ ...
        @${CD} ${SRCDIR}.Lib.OSLib
        ${MAKE}

ToolboxMF:
        @echo $@ ...
        @${CD} ${SRCDIR}.Import.Aquarius
        ${MAKE}

ADFS:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.FileSys.ADFS.ADFS
        ${MAKE}

ARM3:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.HWSupport.ARM3
        ${MAKE}

ATAPI:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.HWSupport.CD.ATAPI
        ${MAKE}

CDFS:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.FileSys.CDFS.CDFS
        ${MAKE}

CDFSDriver:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.HWSupport.CD.CDFSDriver
        ${MAKE}

DDT:
        @echo $@ ...
        @${CD} ${SRCDIR}.Import.DDT
        ${MAKE}

DMAManager:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.HWSupport.DMA
        ${MAKE}

DOSFS:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.FileSys.ImageFS.DOSFS
        ${MAKE}

Econet:
        @echo $@ ...
        @${CD} ${SRCDIR}.Networking.Econet
        ${MAKE}

Free:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.Desktop.Free
        ${MAKE}

HostFS:
        @echo $@ ...
        @${CD} ${SRCDIR}.Programmer.HostFS
        ${MAKE}

SCSI:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.HwSupport.SCSI.SCSIDriver
        ${MAKE}

SCSIFS:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.FileSys.SCSIFS.SCSIFS
        ${MAKE}

ShellCLI:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.Desktop.ShellCLI
        ${MAKE}

Portable:
        @echo $@ ...
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Portable
        ${MAKE}

Help:
        @echo $@ ...
        @${CD} ${SRCDIR}.Apps.Help
        ${MAKE}

Podule:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Podule
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

CMOSReset:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.STB
        @${CD} ${SRCDIR}.STB.Misc.CMOSreset
        ${MAKE} INSTDIR=${INSTDIR}.ROM.STB

FileSwitch:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.FileSys.FileSwitch
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

ResourceFS:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.FileSys.ResourceFS.ResourceFS
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

Messages:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega2
        @${CD} ${SRCDIR}.OS_Core.Internat.Messages
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega2

MessageTrans:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Internat.MsgTrans
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

TerritoryManager:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Internat.Territory.Manager
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

UK:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Internat.Territory.UK
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

USA:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.OS_Core.Internat.Territory.USA
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

WindowManager:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Desktop.Wimp
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

Desktop:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Desktop.Desktop
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

PS2Driver:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.OS_Core.HWSupport.PS2Driver
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

ARM:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.OS_Core.HWSupport.ARM
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

BASIC:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Programmer
        @${CD} ${SRCDIR}.Programmer.BASIC
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Programmer

BufferManager:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Buffers
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

ColourTrans:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.Render.Colours
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

Debugger:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.Programmer.Debugger
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

DeviceFS:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.DeviceFS
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

DragASprite:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Desktop.DragASprit
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

DragAnObj:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Desktop.DragAnObj
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

DrawMod:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.Render.Draw
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

FileCore:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.FileSys.FileCore
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

RamFS:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.FileSys.RAMFS.RAMFS
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

FontManager:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.Render.Fonts.Manager
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

FPEmulator:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.OS_Core.HWSupport.FPASC.riscos
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

GameModes:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.GameModes2
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

Hourglass:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.Render.Hourglass
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

IIC:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.IIC
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

ITable:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.Render.Fonts.ITable
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

International:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Internat.Inter
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

Joystick:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Joystick
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

NHTwitter:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.STB
        @${CD} ${SRCDIR}.Trial.WebBrowser.NHTwitter
        ${MAKE} INSTDIR=${INSTDIR}.ROM.STB

Obey:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Programmer
        @${CD} ${SRCDIR}.Programmer.Obey
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Programmer

ParallelDeviceDriver:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Parallel
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

InternationalKeyboard:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Internat.IntKey
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

PipeFS:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.FileSys.PipeFS
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

ROMFonts:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.Render.Fonts.ROMFonts
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

RTCAdjust:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.OS_Core.HWSupport.RTCAdjust
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

ScreenBlanker:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.Render.ScrBlank
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

SerialDeviceDriver:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Serial
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

SerialDeviceSupport:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.SerialSpt
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

SoundDMA:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Sound.Sound0
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

SoundChannels:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Sound.Sound1
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

SoundScheduler:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Sound.Sound2
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

SpriteExtend:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.Render.SprExtend
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

Squash:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Squash
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

SuperSample:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.Render.Super
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

SystemDevices:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.SystemDevs
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

TaskWindow:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Desktop.TaskWindow
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

FilterManager:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Desktop.Filter
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

WaveSynth:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Sound.Voices.WaveSynth
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

StringLib:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Sound.Voices.StringLib
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

Percussion:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.HWSupport.Sound.Voices.Percussion
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

SharedCLibrary:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Lib
        @${CD} ${SRCDIR}.Lib.RISC_OSLib
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Lib

ScreenModes:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.UserI.ScrModes
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

DrawFile:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.Video.Render.DrawFile
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

BootCommands:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.OS_Core
        @${CD} ${SRCDIR}.OS_Core.BootCmds
        ${MAKE} INSTDIR=${INSTDIR}.ROM.OS_Core

AUNMsgs:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Networking
        @${CD} ${SRCDIR}.Networking.AUN.AUNMsgs
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Networking

MManager:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Networking
        @${CD} ${SRCDIR}.Networking.AUN.MManager
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Networking

Internet:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Networking
        @${CD} ${SRCDIR}.Networking.AUN.Internet.build
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Networking

NFSSTB:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Networking
        @${CD} ${SRCDIR}.Networking.NFS
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Networking

MakePSFont:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.Printing.Printers.Modules.MakePSFont
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

PDriverDP:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.Printing.Printers.Modules.PDModules
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

PDriverPS:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.Printing.Printers.Modules.PDModules
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

PDumperSpt:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.Printing.Printers.Modules.PDumperSpt
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

PDumperDM:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.Printing.Printers.PDumpers
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

PDumperIW:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.Printing.Printers.PDumpers
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

PDumperLJ:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.Printing.Printers.PDumpers
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

PDumperE2:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.Printing.Printers.PDumpers
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

PDriver:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.Printing.Printers.Modules.PDriver
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

STBPrint:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.STB.Printing.STBPrint
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

PPrimer:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Printing
        @${CD} ${SRCDIR}.STB.Printing.PPrimer
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Printing

tboxlib:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Toolbox
        @${CD} ${SRCDIR}.Import.Aquarius.tbox.common
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Toolbox

TinyStubs:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Toolbox
        @${CD} ${SRCDIR}.Import.Aquarius.TinyStubs
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Toolbox

Interlace:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.STB.Display.Interlace
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

DynamicAreas:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.STB.Misc.DynAreas
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

ModeFiles:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.STB.Display.ModeFiles
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

Standby:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.ROM
        @${MKDIR} ${INSTDIR}.ROM.Omega
        @${CD} ${SRCDIR}.STB.Misc.Standby
        ${MAKE} INSTDIR=${INSTDIR}.ROM.Omega

