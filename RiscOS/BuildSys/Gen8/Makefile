# System Makefile

CD      = dir
MKDIR   = cdir
SYSDIR  = <build$dir>.BuildSys
SRCDIR  = <src$dir>
INSTDIR = <install$dir>
MSGDIR  = ${SRCDIR}.Internat.Messages
RESDIR  = ${MSGDIR}.Resources.<Locale>.<Build>.Resources
MAKE    = @amu ${MFLAGS} RESDIR=${RESDIR}

BUILD = \
 unixlib \
 socklib \
 rpclib \
 inetlib \
 TCPIPheaders \
 AcornNCLib \
 ModuleTaskLib \
 PlainArgvLib \
 ModMallocLib \
 MemCheckLib \
 HTMLLib \
 DeskLib \
 EnsureLine \
 ProgressUI \
 StoredInfo \
 NCDialUI \
 NCDialler \
 Watchdog \
 PPP \
 UARTDeviceDriver \
 ScrSaver \
 CSFS \
 ISPCapture \
 ManagedAccessLite \
 Standby \
 FrontPanel \
 LED \
 MultiMedia \
 CodecCtl \
 MPlay \
 ESPSynthDrivers \
 MIDISynthesiser \
 SampleExp \
 SoundFX \
 SharedSnd \
 ScreenFade \
 IconHigh \
 RedrawManager \
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
 IRRCABlast \
 IRRelPtr \
 IRKB4 \
 IRKeyboard \
 IRHandset \
 CRK93H1 \
 IRDefault \
 IRDirector \
 NCBootblk \
 Cron \
 SCInterface \
 SCTransport \
 DualSerial \
 ModeFiles \
 ShrinkWrap \
 DynamicAreas \
 NHTwitter \
 Interlace \
 ToolAction \
 Menu \
 Window \
 Toolbox \
 tboxlib \
 PPrimer \
 PDriver \
 PDumperLE \
 PDumpers \
 PDumperSpt \
 PDriverPS \
 PDriverDP \
 MakePSFont \
 Internet \
 MManager \
 AUNMsgs \
 BootCommands \
 ScreenModes \
 WaveSynth \
 FilterManager \
 SystemDevices \
 SuperSample \
 Squash \
 SpriteExtend \
 SoundScheduler \
 SoundChannels \
 SoundDMA \
 ScreenBlanker \
 ROMFonts \
 InternationalKeyboard \
 ParallelDeviceDriver \
 Obey \
 International \
 ITable \
 Hourglass2 \
 FPEmulator \
 FontManager \
 FileCore \
 DrawMod \
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
 UniqueID \
 NVRAM \
 TinyStubs \
 SharedCLibrary \
 MessageTrans \
 Messages \
 ResourceFS \
 FileSwitch \
 OSVersion \
 Podule \
 Kernel

EXPORT = \
 RemoteDebug \
 SSLRO \
 DrawFile \
 PipeFS \
 Hourglass \
 GameModes \
 DragAnObj \
 DragASprite \
 CMOSReset \
 MPEGHdrs \
 TaskManager \
 FSLock \
 Filer_Action \
 ColourPicker \
 Portable \
 TaskWindow \
 ShellCLI \
 SerialDeviceDriver \
 SCSIFS \
 SCSI \
 RamFS \
 HostFS \
 IIC \
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
 OSLib \
 ShareD \
 HdrSrc

RESOURCE = \
 AutoDemo \
 Boot \
 PrintDefs \
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
        set Resource$Dir ${MSGDIR}.Resources.<Locale>.<Build>
        ${MKDIR} <install$dir>
        ${MKDIR} <build$dir>.Export
        ${MKDIR} <build$dir>.Export.Hdr
        ${MKDIR} <build$dir>.Export.Hdr.Global
        ${MKDIR} <build$dir>.Export.Hdr.Interface
        ${MKDIR} <build$dir>.Export.Hdr.Interface2
        ${MKDIR} <build$dir>.Export.Lib
        ${MKDIR} ${MSGDIR}.Resources
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.<BUILD>
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.<BUILD>.Resources
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.<BUILD>.Resources2
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.<BUILD>.Apps
#
# Component rules:
#

Kernel:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.RCA
        @${CD} ${SRCDIR}.Kernel
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.RCA

ROMImage: ${BUILD}
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.Images
        @${CD} ${SYSDIR}.<Build>.ROMImage
        ${MAKE} INSTDIR=${INSTDIR}.Images

HdrSrc:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Programmer.HdrSrc
        ${MAKE}

ShareD:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Networking.AUN.Access.ShareFS.ShareFS
        ${MAKE}

OSLib:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Lib.OSLib
        ${MAKE}

ADFS:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.FileSys.ADFS.ADFS
        ${MAKE}

ARM3:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.HWSupport.ARM3
        ${MAKE}

ATAPI:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.HWSupport.CD.ATAPI
        ${MAKE}

CDFS:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.FileSys.CDFS.CDFS
        ${MAKE}

CDFSDriver:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.HWSupport.CD.CDFSDriver
        ${MAKE}

DDT:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Programmer.DDT
        ${MAKE}

DMAManager:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.HWSupport.DMA
        ${MAKE}

DOSFS:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.FileSys.ImageFS.DOSFS
        ${MAKE}

Econet:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Networking.Econet
        ${MAKE}

Free:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Desktop.Free
        ${MAKE}

IIC:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.HWSupport.IIC
        ${MAKE}

HostFS:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Programmer.HostFS
        ${MAKE}

RamFS:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.FileSys.RAMFS.RAMFS
        ${MAKE}

SCSI:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.HWSupport.SCSI.SCSIDriver
        ${MAKE}

SCSIFS:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.FileSys.SCSIFS.SCSIFS
        ${MAKE}

SerialDeviceDriver:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.HWSupport.Serial
        ${MAKE}

ShellCLI:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Desktop.ShellCLI
        ${MAKE}

TaskWindow:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Desktop.TaskWindow
        ${MAKE}

Portable:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.HWSupport.Portable
        ${MAKE}

ColourPicker:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Video.UserI.Picker
        ${MAKE}

Filer_Action:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Desktop.FilerAct
        ${MAKE}

FSLock:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.FileSys.FSLock
        ${MAKE}

TaskManager:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Desktop.Switcher
        ${MAKE}

MPEGHdrs:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Video.MPEG.MPEGHdrs
        ${MAKE}

CMOSReset:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.HWSupport.CMOSreset
        ${MAKE}

DragASprite:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Desktop.DragASprit
        ${MAKE}

DragAnObj:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Desktop.DragAnObj
        ${MAKE}

GameModes:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Video.UserI.GameModes2
        ${MAKE}

Hourglass:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Video.Render.Hourglass
        ${MAKE}

PipeFS:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.FileSys.PipeFS
        ${MAKE}

DrawFile:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Video.Render.DrawFile
        ${MAKE}

SSLRO:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Networking.libs.SSLRO
        ${MAKE}

RemoteDebug:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Lib.remotedb
        ${MAKE}

Help:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Apps.Help
        ${MAKE}

Podule:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.Podule
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

OSVersion:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.OS_Core
        @${CD} ${SRCDIR}.OSVersion
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.OS_Core

FileSwitch:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.FileSys
        @${CD} ${SRCDIR}.FileSys.FileSwitch
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.FileSys

ResourceFS:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.FileSys
        @${CD} ${SRCDIR}.FileSys.ResourceFS.ResourceFS
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.FileSys

Messages:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Internat
        @${CD} ${SRCDIR}.Internat.Messages
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Internat

MessageTrans:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Internat
        @${CD} ${SRCDIR}.Internat.MsgTrans
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Internat

SharedCLibrary:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.RISC_OSLib
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

TinyStubs:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Toolbox
        @${CD} ${SRCDIR}.Toolbox.TinyStubs
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Toolbox

NVRAM:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.NVRAM
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

UniqueID:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.UniqueID
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

TerritoryManager:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Internat
        @${CD} ${SRCDIR}.Internat.Territory.Manager
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Internat

UK:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Internat
        @${CD} ${SRCDIR}.Internat.Territory.UK
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Internat

USA:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Internat
        @${CD} ${SRCDIR}.Internat.Territory.USA
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Internat

WindowManager:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Desktop
        @${CD} ${SRCDIR}.Desktop.Wimp
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Desktop

Desktop:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Desktop
        @${CD} ${SRCDIR}.Desktop.Desktop
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Desktop

PS2Driver:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.PS2Driver
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

ARM:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.ARM
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

BASIC:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.BASIC
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

BufferManager:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.Buffers
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

ColourTrans:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.Colours
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

Debugger:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.Debugger
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

DeviceFS:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.DeviceFS
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

DrawMod:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.Draw
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

FileCore:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.FileSys
        @${CD} ${SRCDIR}.FileSys.FileCore
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.FileSys

FontManager:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.Fonts.Manager
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

FPEmulator:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.FPASC.riscos
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

Hourglass2:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.Hourglass2
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

ITable:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.Fonts.ITable
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

International:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Internat
        @${CD} ${SRCDIR}.Internat.Inter
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Internat

Obey:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.Obey
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

ParallelDeviceDriver:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.Parallel
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

InternationalKeyboard:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Internat
        @${CD} ${SRCDIR}.Internat.IntKey
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Internat

ROMFonts:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.Fonts.ROMFonts
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

ScreenBlanker:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.ScrBlank
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

SoundDMA:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.Sound.Sound0
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

SoundChannels:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.Sound.Sound1
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

SoundScheduler:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.Sound.Sound2
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

SpriteExtend:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.SprExtend
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

Squash:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.Squash
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

SuperSample:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.Super
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

SystemDevices:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.SystemDevs
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

FilterManager:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Desktop
        @${CD} ${SRCDIR}.Desktop.Filter
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Desktop

WaveSynth:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.Sound.Voices.WaveSynth
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

ScreenModes:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.UserI.ScrModes
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

BootCommands:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.BootCmds
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

AUNMsgs:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.AUN.AUNMsgs
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

MManager:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.AUN.MManager
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

Internet:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.AUN.Internet.build
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

MakePSFont:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Printing
        @${CD} ${SRCDIR}.Printing.Modules.MakePSFont
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Printing

PDriverDP:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Printing
        @${CD} ${SRCDIR}.Printing.Modules.PDModules
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Printing

PDriverPS:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Printing
        @${CD} ${SRCDIR}.Printing.Modules.PDModules
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Printing

PDumperSpt:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Printing
        @${CD} ${SRCDIR}.Printing.Modules.PDumperSpt
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Printing

PDumpers:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Printing
        @${CD} ${SRCDIR}.Printing.PDumpers
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Printing

PDumperLE:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Printing
        @${CD} ${SRCDIR}.Printing.NewDumpers.PDumperLE
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Printing

PDriver:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Printing
        @${CD} ${SRCDIR}.Printing.Modules.PDriver
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Printing

PrintDefs:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Printing.PrintDefs
        ${MAKE}

PPrimer:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Printing
        @${CD} ${SRCDIR}.Printing.PPrimer
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Printing

tboxlib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Toolbox
        @${CD} ${SRCDIR}.Toolbox.Common
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Toolbox

Toolbox:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Toolbox
        @${CD} ${SRCDIR}.Toolbox.Toolbox
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Toolbox

Window:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Toolbox
        @${CD} ${SRCDIR}.Toolbox.Window
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Toolbox

Menu:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Toolbox
        @${CD} ${SRCDIR}.Toolbox.Menu
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Toolbox

ToolAction:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Toolbox
        @${CD} ${SRCDIR}.Toolbox.ToolAction
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Toolbox

Boot:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Apps.Boot
        ${MAKE}

AutoDemo:
        @echo $@ ...
	@echo System=<System>
        @${CD} ${SRCDIR}.Apps.AutoDemo
        ${MAKE}

Interlace:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.Interlace
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

NHTwitter:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.NHTwitter
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

DynamicAreas:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.DynAreas
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

ShrinkWrap:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.ShrinkWrap
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

ModeFiles:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.UserI.ModeFiles
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

DualSerial:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.DualSerial
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

SCTransport:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.SmartCard.SCTrans
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

SCInterface:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.SmartCard.SCInter
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

Cron:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.Cron
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

NCBootblk:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Security
        @${CD} ${SRCDIR}.Security.NCBootblk
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Security

IRDirector:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDirector
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

IRDefault:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.IRDefault
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

CRK93H1:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.CRK93H1
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

IRHandset:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.IRHandset
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

IRKeyboard:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.IRKeyboard
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

IRKB4:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.IRKB4
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

IRRelPtr:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.IRRelPtr
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

IRRCABlast:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.IR2.IRDrivers.RCABlast
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

ParaJoy:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.ParaJoy
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

NetTime:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.NetTime
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

FileWatch:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.FileWatch
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

CacheFS:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.FileSys
        @${CD} ${SRCDIR}.FileSys.CacheFS
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.FileSys

TaskModule:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Desktop
        @${CD} ${SRCDIR}.Desktop.TaskModule
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Desktop

msadpcm:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Audio
        @${CD} ${SRCDIR}.Audio.msadpcm
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Audio

ReplaySpt:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Audio
        @${CD} ${SRCDIR}.Audio.ReplaySpt
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Audio

SoundFile:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Audio
        @${CD} ${SRCDIR}.Audio.SoundFile
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Audio

FilePlay:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Audio
        @${CD} ${SRCDIR}.Audio.FilePlay
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Audio

NCOptions:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Apps
        @${CD} ${SRCDIR}.Apps.NCOptions
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Apps

SoakTest:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Test
        @${CD} ${SRCDIR}.Test.SoakTest
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Test

http:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.HTTP
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

webftp:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.WebFTP
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

WebGopher:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.WebGopher
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

Resolver:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.Resolver
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

NCFresco:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Apps
        @${CD} ${SRCDIR}.Apps.NCFrescoRO
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Apps

KeyWatch:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Programmer
        @${CD} ${SRCDIR}.Programmer.KeyWatch
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Programmer

NCKeyboard:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Apps
        @${CD} ${SRCDIR}.Apps.NCKeyboard
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Apps

RedrawManager:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Desktop
        @${CD} ${SRCDIR}.Desktop.RedrawMgr
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Desktop

IconHigh:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.UserI.IconHigh
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

ScreenFade:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.Render.ScreenFade
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

SharedSnd:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Audio
        @${CD} ${SRCDIR}.Audio.SharedSnd
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Audio

SoundFX:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Audio
        @${CD} ${SRCDIR}.Audio.SoundFX
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Audio

SampleExp:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Audio
        @${CD} ${SRCDIR}.Audio.SampleExp
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Audio

MIDISynthesiser:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Audio
        @${CD} ${SRCDIR}.Audio.MIDI.Synth
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Audio

ESPSynthDrivers:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Audio
        @${CD} ${SRCDIR}.Audio.MIDI.ESPDrvrsAS
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Audio

MPlay:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Audio
        @${CD} ${SRCDIR}.Audio.MIDI.Mplay
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Audio

CodecCtl:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Apps
        @${CD} ${SRCDIR}.Apps.CodecCtl
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Apps

MultiMedia:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Apps
        @${CD} ${SRCDIR}.Apps.MultiMedia
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Apps

LED:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.LED
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

FrontPanel:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.FrontPanel
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

Standby:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.Standby
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

ManagedAccessLite:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Security
        @${CD} ${SRCDIR}.Security.NCMAlite
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Security

ISPCapture:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Security
        @${CD} ${SRCDIR}.Security.ISPCapture
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Security

CSFS:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.FileSys
        @${CD} ${SRCDIR}.FileSys.CSFS
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.FileSys

ScrSaver:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.UserI.ScrSaver
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

UARTDeviceDriver:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.HWSupport
        @${CD} ${SRCDIR}.HWSupport.UARTDriver
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.HWSupport

PPP:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.Modem.PPP
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

Watchdog:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Apps
        @${CD} ${SRCDIR}.Apps.Watchdog
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Apps

NCDialler:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.Modem.NCDialler
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

NCDialUI:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.Modem.NCDialUI
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

StoredInfo:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Test
        @${CD} ${SRCDIR}.Test.StoredInfo
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Test

ProgressUI:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Video
        @${CD} ${SRCDIR}.Video.UserI.ProgressUI
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Video

EnsureLine:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Networking
        @${CD} ${SRCDIR}.Networking.Modem.EnsureLine
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Networking

DeskLib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.Desk
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

HTMLLib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.HTML
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

MemCheckLib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.MemCheck
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

ModMallocLib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.ModMalloc
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

PlainArgvLib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.PlainArgv
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

ModuleTaskLib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.ModuleTask
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

AcornNCLib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.AcornNC
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

TCPIPheaders:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.TCPIPLibs.headers
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

inetlib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.TCPIPLibs.inetlib
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

rpclib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.TCPIPLibs.rpclib.rpc
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

socklib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.TCPIPLibs.socklib
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

unixlib:
        @echo $@ ...
	@echo System=<System>
        @${MKDIR} ${INSTDIR}.<Build>
        @${MKDIR} ${INSTDIR}.<Build>.Lib
        @${CD} ${SRCDIR}.Lib.TCPIPLibs.unixlib
        ${MAKE} INSTDIR=${INSTDIR}.<Build>.Lib

# Finished
