# Makefile for Boot
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 15th Nov 1994  JRC    Black Boot edifice created
# 21st Nov 1994  AMcC   Added ResetBoot
# 29th Nov 1994  AMcC   Made separate rule for installing Configure dir (Monitors etc)
#                       Now installs required Monitors by name
#  5th Jan 1995  AMcC   VProtect modsqz'd
#  9th Jan 1995  JRC    Squeeze LoadCMOS, SafeLogon, ClrMonitor
# 30th Jan 1995  AMcC   Corrected access permissions (files in Choices mustn't be locked)
#                       Extracted common bits into separate rules
# 30th Jan 1995  AMcC   ResetBoot - install phase depends on install_Configure
#                                 - install its own !Run file (not the common one)
# 13th Feb 1995  AMcC   ResetBoot - don't install_Configure
# 27th Mar 1995  AMcC   Removed AKF70 from install_configure rule
# 16th May 1995  JRC    No longer deletes ShareBoot SetupNet files: instead, there are separate
#                       ones for ShareBoot, ArmBoot and Boot. DOesn't remove ww access
#                       from everything.
# 14th Jun 1995  AMcC   Added lines to remove ~CDReinit from !ArmBoot and ShareBoot
#  4th Jul 1995  JRC    socket.h now in OSLib
#  7th Jul 1995  JRC    Changed INSTDIR to INSTAPP, except in 1 vital place
#  1st Aug 1995  JRC    Added BootVars target, for installation of BootVars in Scrap

#
# Program specific options:
#
#COMPONENT = ...
#   This must be set on the command line to one of Boot, BootLib, ShareBoot,
#   ArmBoot, ResetBoot. Case is important. INSTDIR must also set, as usual.
APP        = !${COMPONENT}
RDIR       = ${COMPONENT}
LDIR       = ${RDIR}.${LOCALE}
INSTAPP    = ${INSTDIR}.${APP}
RESOURCES  = ${INSTAPP}.Resources

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
ATTR    = -attr
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
SQUEEZE = squeeze
WIPE    = -wipe

AFLAGS  = ${THROWBACK} -depend !Depend -nocache -stamp -quit
CFLAGS  = ${THROWBACK} ${COMPAT} -c -depend !Depend -fafh ${INCLUDES} ${DFLAGS} -wp ${OPTIONS}
CPFLAGS = ~cfr~v
SQFLAGS = -f
WFLAGS  = ~c~vr

#
# Libraries
#
CLIB      = CLib:o.Stubs
OSLIB     = OSLib:o.OSLib
SOCKLIB	  = TCPIPLibs:o.socklib
ROMSTUBS  = RISC_OSLib:o.ROMStubs
ABSSYM    = RISC_OSLib:o.AbsSym
WRAPPER   = RISC_OSLib:s.ModuleWrap

LIBSD     = ${RLIB} ${CLIB}

COMPAT = -DCOMPAT_INET4

#
# Include files
#
INCLUDES  = -ITCPIPLibs:,C:

FILES = \
   Library.AddApp \
   Library.AppSize \
   Utils.BootVars \
   Library.Do \
   Library.IfThere \
   Library.LoadCMOS \
   Library.SafeLogon \
   RO350Hook.Boot.PreDesk.MemFix \
   Utils.FreePool \
   Utils.VProtect \
   ClrMonitor

#
# Rule patterns
#
.SUFFIXES: .oz .od .asm .o .Source .c
.c.o:;    ${CC} ${CFLAGS} -ff -o $@ $<
.c.oz:;   ${CC} ${CFLAGS} -ff -zM -zps1 -o $@ $<
.c.od:;   ${CC} ${CFLAGS} -DTRACE=1 -g -o $@ $<
.asm.o:;  ${AS} ${AFLAGS} $< $@
.asm.od:; ${AS} ${AFLAGS} -g $< $@

#
# Main rules:
#
all: ${FILES}
        @echo ${COMPONENT}: Application built (Disc)

install: set_access install_${COMPONENT}
   @echo ${COMPONENT}: installed (Disc)

install_Boot: install_Configure install_appfiles ${FILES}
   ${MKDIR}                                     ${INSTAPP}.Choices
   ${CP} RO360Hook.Boot                         ${INSTAPP}.Choices.Boot ${CPFLAGS}
   ${CP} RO360Hook.Desktop                      ${INSTAPP}.Choices.Boot.Desktop ${CPFLAGS}
   ${CP} RO360Hook.PreDesktop                   ${INSTAPP}.Choices.Boot.PreDesktop ${CPFLAGS}
   ${CP} ^.InetSetup.Resources.<Locale>.Blanks.SetUpNet  ${INSTAPP}.Choices.Boot.PreDesk.SetupNet ${CPFLAGS}
   ${CP} Utils                                  ${INSTAPP}.Utils ${CPFLAGS}
   ${MKDIR}                                     ${INSTAPP}.Library
   @Echo Deleting superfluous files ...
   ${WIPE} ${INSTAPP}.Utils.NetBye ${WFLAGS}

install_ArmBoot: install_Configure install_appfiles ${FILES}
   ${MKDIR}                                     ${INSTAPP}.Choices
   ${CP} RO360Hook.Boot                         ${INSTAPP}.Choices.Boot ${CPFLAGS}
   ${CP} RO360Hook.DesktopBye                   ${INSTAPP}.Choices.Boot.Desktop ${CPFLAGS}
   ${CP} RO360Hook.PreDesktop                   ${INSTAPP}.Choices.Boot.PreDesktop ${CPFLAGS}
   ${MKDIR}                                     ${INSTAPP}.Choices.Boot.PreDesk.SetupNet
   ${CP} RO360Hook.ArmNet                       ${INSTAPP}.Choices.Boot.PreDesk.SetupNet.!Run ${CPFLAGS}
   ${CP} Library                                ${INSTAPP}.Library ${CPFLAGS}
   ${CP} Utils                                  ${INSTAPP}.Utils ${CPFLAGS}
   | RO2 machines need unmodsqz'd VProtect
   ${CP} rm.VProtect                            ${INSTAPP}.Utils.VProtect ${CPFLAGS}
   ${MKDIR}                                     ${INSTAPP}.Utils.RO200Hook
   ${CP} RO200Hook.Boot                         ${INSTAPP}.Utils.RO200Hook.Boot ${CPFLAGS}
   ${CP} RO200Hook.DesktopBye                   ${INSTAPP}.Utils.RO200Hook.Boot.Desktop ${CPFLAGS}
   ${CP} RO200Hook.PreDesktop                   ${INSTAPP}.Utils.RO200Hook.Boot.PreDesktop ${CPFLAGS}
   ${MKDIR}                                     ${INSTAPP}.Utils.RO310Hook
   ${CP} RO310Hook.Boot                         ${INSTAPP}.Utils.RO310Hook.Boot ${CPFLAGS}
   ${CP} RO310Hook.DesktopBye                   ${INSTAPP}.Utils.RO310Hook.Boot.Desktop ${CPFLAGS}
   ${CP} RO310Hook.PreDesktop                   ${INSTAPP}.Utils.RO310Hook.Boot.PreDesktop ${CPFLAGS}
   ${MKDIR}                                     ${INSTAPP}.Utils.RO350Hook
   ${CP} RO350Hook.Boot                         ${INSTAPP}.Utils.RO350Hook.Boot ${CPFLAGS}
   ${CP} RO350Hook.DesktopBye                   ${INSTAPP}.Utils.RO350Hook.Boot.Desktop ${CPFLAGS}
   ${CP} RO350Hook.PreDesktop                   ${INSTAPP}.Utils.RO350Hook.Boot.PreDesktop ${CPFLAGS}
   @Echo Deleting superfluous files ...
   ${WIPE}                                      ${INSTAPP}.Choices.Boot.Tasks.~CDReinit  ${WFLAGS}

install_ShareBoot: install_Configure install_appfiles ${FILES}
   ${MKDIR}                                     ${INSTAPP}.Choices
   ${CP} RO360Hook.Boot                         ${INSTAPP}.Choices.Boot ${CPFLAGS}
   ${CP} RO360Hook.Desktop                      ${INSTAPP}.Choices.Boot.Desktop ${CPFLAGS}
   ${CP} RO360Hook.PreDesktop                   ${INSTAPP}.Choices.Boot.PreDesktop ${CPFLAGS}
   ${MKDIR}                                     ${INSTAPP}.Choices.Boot.PreDesk.SetupNet
   ${CP} RO360Hook.ShareNet                     ${INSTAPP}.Choices.Boot.PreDesk.SetupNet.!Run ${CPFLAGS}
   ${CP} Library                                ${INSTAPP}.Library ${CPFLAGS}
   ${CP} Utils                                  ${INSTAPP}.Utils ${CPFLAGS}
   ${MKDIR}                                     ${INSTAPP}.Utils.RO310Hook
   ${CP} RO310Hook.Boot                         ${INSTAPP}.Utils.RO310Hook.Boot ${CPFLAGS}
   ${CP} RO310Hook.Desktop                      ${INSTAPP}.Utils.RO310Hook.Boot.Desktop ${CPFLAGS}
   ${CP} RO310Hook.PreDesktop                   ${INSTAPP}.Utils.RO310Hook.Boot.PreDesktop ${CPFLAGS}
   ${MKDIR}                                     ${INSTAPP}.Utils.RO350Hook
   ${CP} RO350Hook.Boot                         ${INSTAPP}.Utils.RO350Hook.Boot ${CPFLAGS}
   ${CP} RO350Hook.Desktop                      ${INSTAPP}.Utils.RO350Hook.Boot.Desktop ${CPFLAGS}
   ${CP} RO350Hook.PreDesktop                   ${INSTAPP}.Utils.RO350Hook.Boot.PreDesktop ${CPFLAGS}
   @Echo Deleting superfluous files ...
   ${WIPE}                                      ${INSTAPP}.Choices.Boot.Tasks.~CDReinit  ${WFLAGS}
   ${WIPE}                                      ${INSTAPP}.Utils.NetBye ${WFLAGS}

install_ResetBoot:
   ${MKDIR}                                     ${INSTAPP}
   ${CP} ${RDIR}.!Help                          ${INSTAPP}.!Help ${CPFLAGS}
   ${ATTR} -ol +ow                              ${INSTAPP}.!HelpText #Just in case it's already there
   Kitten Version ${LDIR}.!HelpText           > ${INSTAPP}.!HelpText
   ${CP} ${LDIR}.!Run                           ${INSTAPP}.!Run ${CPFLAGS}
   ${CP} ${RDIR}.!RunImage                      ${INSTAPP}.!RunImage ${CPFLAGS}
   ${CP} ${LDIR}.!Sprites                       ${INSTAPP}.!Sprites ${CPFLAGS}
   ${CP} ${LDIR}.!Sprites22                     ${INSTAPP}.!Sprites22 ${CPFLAGS}
   ${CP} ${LDIR}.Messages                       ${INSTAPP}.Messages ${CPFLAGS}
   ${MKDIR}                                     ${INSTAPP}.Choices
   ${CP} RO360Hook.Boot                         ${INSTAPP}.Choices.Boot ${CPFLAGS}
   ${CP} RO360Hook.Desktop                      ${INSTAPP}.Choices.Boot.Desktop ${CPFLAGS}
   ${CP} RO360Hook.PreDesktop                   ${INSTAPP}.Choices.Boot.PreDesktop ${CPFLAGS}
   ${CP} ^.InetSetup.Resources.<Locale>.Blanks.SetUpNet  ${INSTAPP}.Choices.Boot.PreDesk.SetupNet ${CPFLAGS}

install_BootLib: ${FILES}
   ${CP} Library                                ${INSTDIR}.!Boot.Library ${CPFLAGS}

install_appfiles:
   ${MKDIR}                                     ${INSTAPP}
   ${CP} ${LDIR}.!Boot                          ${INSTAPP}.!Boot ${CPFLAGS}
   ${ATTR} -ol +ow                              ${INSTAPP}.!Help #Just in case it's already there
   Kitten Version ${LDIR}.!Help               > ${INSTAPP}.!Help
   ${CP} Source.!Run                            ${INSTAPP}.!Run ${CPFLAGS}
   ${CP} ${RDIR}.!Sprites                       ${INSTAPP}.!Sprites ${CPFLAGS}
   ${CP} ${RDIR}.!Sprites22                     ${INSTAPP}.!Sprites22 ${CPFLAGS}

install_Configure:
   ${MKDIR}                               ${INSTAPP}
   ${MKDIR}                               ${INSTAPP}.Choices
   ${MKDIR}                               ${INSTAPP}.Choices.Boot
   ${MKDIR}                               ${RESOURCES}
   ${MKDIR}                               ${RESOURCES}.Configure
   ${MKDIR}                               ${RESOURCES}.Configure.Monitors
   ${MKDIR}                               ${RESOURCES}.Configure.Monitors.Acorn
   ${CP} ClrMonitor                       ${RESOURCES}.Configure.ClrMonitor ${CPFLAGS}
   ${CP} Configure.2DTools                ${RESOURCES}.Configure.2DTools    ${CPFLAGS}
   ${CP} Configure.FontChange             ${RESOURCES}.Configure.FontChange ${CPFLAGS}
   ${CP} Configure.Monitors.Acorn.AKF50   ${RESOURCES}.Configure.Monitors.Acorn.AKF50 ${CPFLAGS}
   ${CP} Configure.Monitors.Acorn.AKF52   ${RESOURCES}.Configure.Monitors.Acorn.AKF52 ${CPFLAGS}
   ${CP} Configure.Monitors.Acorn.AKF53   ${RESOURCES}.Configure.Monitors.Acorn.AKF53 ${CPFLAGS}
   ${CP} Configure.Monitors.Acorn.AKF60   ${RESOURCES}.Configure.Monitors.Acorn.AKF60 ${CPFLAGS}
   ${CP} Configure.Monitors.Acorn.AKF65   ${RESOURCES}.Configure.Monitors.Acorn.AKF65 ${CPFLAGS}
   ${CP} Configure.Monitors.Acorn.AKF85   ${RESOURCES}.Configure.Monitors.Acorn.AKF85 ${CPFLAGS}
   ${CP} Configure.Monitors.Acorn.AKF91   ${RESOURCES}.Configure.Monitors.Acorn.AKF91 ${CPFLAGS}
   ${CP} Configure.Monitors.Acorn.AKF92   ${RESOURCES}.Configure.Monitors.Acorn.AKF92 ${CPFLAGS}
   ${CP} Configure.Textures               ${RESOURCES}.Configure.Textures  ${CPFLAGS}

install_BootVars: Utils.BootVars
   ${CP} Utils.BootVars ${INSTDIR}.BootVars ${CPFLAGS}

set_access:
   ${ATTR} -directories +wr ${INSTAPP}
   ${ATTR} -files +ol +or -ow +wr ${INSTAPP}
   ${ATTR} -files -ol +ow ${INSTAPP}.Choices
# NB leave ww access as it was!!

clean:
   ${WIPE} Library.AddApp ${WFLAGS}
   ${WIPE} Library.AppSize ${WFLAGS}
   ${WIPE} Utils.BootVars ${WFLAGS}
   ${WIPE} Source.BootVars.o.main ${WFLAGS}
   ${WIPE} Library.Do ${WFLAGS}
   ${WIPE} Library.IfThere ${WFLAGS}
   ${WIPE} Library.LoadCMOS ${WFLAGS}
   ${WIPE} Source.LoadCMOS.o.main ${WFLAGS}
   ${WIPE} Library.SafeLogon ${WFLAGS}
   ${WIPE} Source.SafeLogon.o.main ${WFLAGS}
   ${WIPE} ClrMonitor ${WFLAGS}
   ${WIPE} Source.ClrMonitor.o.main ${WFLAGS}
   ${WIPE} Utils.FreePool ${WFLAGS}
   ${WIPE} Utils.VProtect ${WFLAGS}
   ${WIPE} RO350Hook.Boot.PreDesk.MemFix ${WFLAGS}
   @echo ${COMPONENT}: cleaned

clean_all:
   -Destroy Library.AddApp Library.AppSize Utils.BootVars \
         Source.BootVars.o.main Library.Do \
         Library.IfThere Library.LoadCMOS Source.LoadCMOS.o.main \
         Library.SafeLogon Source.SafeLogon.o.main \
         ClrMonitor Source.ClrMonitor.o.main Utils.FreePool Utils.VProtect \
         RO350Hook.Boot.PreDesk.MemFix
   ${ATTR} -ol ${INSTAPP}
   ${WIPE} ${INSTAPP} ${WFLAGS}
#
# Static dependencies:
#
Source.BootVars.o.main: Source.BootVars.c.main
   cc ${CFLAGS} -ffah -o Source.BootVars.o.main Source.BootVars.c.main

Library.AddApp: Source.AddApp.Source.AddApp
   AAsm ${AFLAGS} Source.AddApp.Source.AddApp Library.AddApp -stamp -quit
   SetType Library.AddApp Utility

Library.AppSize: Source.AppSize.Source.AppSize
   AAsm ${AFLAGS} Source.AppSize.Source.AppSize Library.AppSize -stamp -quit
   SetType Library.AppSize Utility

Utils.BootVars: Source.BootVars.o.main
   ${LD} ${LDFLAGS} -o Utils.BootVars Source.BootVars.o.main C:RMVersion.o.RMVersion ${SOCKLIB} ${CLIB}
   ${SQUEEZE} $@

Library.Do: Source.Do.Source.Do
   AAsm ${AFLAGS} Source.Do.Source.Do Library.Do -stamp -quit
   SetType Library.Do Utility

Library.IfThere: Source.IfThere.Source.IfThere
   AAsm ${AFLAGS} Source.IfThere.Source.IfThere Library.IfThere -stamp -quit
   SetType Library.IfThere Utility

Library.LoadCMOS: Source.LoadCMOS.o.main
   ${LD} ${LDFLAGS} -o $@ Source.LoadCMOS.o.main ${CLIB}
   ${SQUEEZE} $@

Library.SafeLogon: Source.SafeLogon.o.main
   ${LD} ${LDFLAGS} -o $@ Source.SafeLogon.o.main ${OSLIB} ${CLIB}
   ${SQUEEZE} $@

ClrMonitor: Source.ClrMonitor.o.main
   ${LD} ${LDFLAGS} -o $@ Source.ClrMonitor.o.main ${OSLIB} ${CLIB}
   ${SQUEEZE} $@

Utils.FreePool: Source.FreePool.Source.FreePool
   AAsm ${AFLAGS} Source.FreePool.Source.FreePool Utils.FreePool -stamp -quit
   SetType Utils.FreePool Utility

Utils.VProtect: rm.VProtect
   ModSqz rm.VProtect $@

RO350Hook.Boot.PreDesk.MemFix: Source.MemFix.Source.MemFix
   AAsm ${AFLAGS} Source.MemFix.Source.MemFix RO350Hook.Boot.PreDesk.MemFix -stamp -quit
   SetType RO350Hook.Boot.PreDesk.MemFix Utility

#---------------------------------------------------------------------------
# Library.AddApp: Hdr:ListOpts
# Dynamic dependencies:
