# Makefile for Daytona
# ====================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date          Name    Description
# ====          ====    ===========
# 1997-10-06    BAL     Created

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT       = Daytona
APP             = !Daytona
MODULE          = rm.!Daytona
ROM_MODULE      = aof.Daytona
LDIR            = Resources.${LOCALE}

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR          = ${RESDIR}.${COMPONENT}
RESAPP          = <Resource$Dir>.Apps.${APP}


# -----------------------------------------------------------------------------
# Generic options
#

MKDIR           = cdir
AS              = objasm
CC              = cc
C++             = c++
CMHG            = cmhg
CP              = copy
LD              = link
RM              = remove
SQUEEZE         = squeeze
WIPE            = -wipe

AFLAGS          = ${THROWBACK} -depend !Depend -nocache -stamp -quit
CFLAGS          = ${THROWBACK} -depend !Depend -ffa ${INCLUDES} -wp
C++FLAGS        = ${THROWBACK} -depend !Depend -ffa ${INCLUDES} -wp
CPFLAGS         = ~cfr~v~n
SQFLAGS         = -f
WFLAGS          = ~cf~vr


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DFLAGS          = 
DDFLAGS         = ${DFLAGS} -fn
DZFLAGS         = 
DDZFLAGS        = ${DZFLAGS} -fn


# -----------------------------------------------------------------------------
# Libraries
#

LIBDIR          = <Lib$Dir>

CLIB            = CLib:o.stubs
C++LIB          = CPP:o.c++lib
ROMCSTUBS       = RISC_OSLib:o.romcstubs
ABSSYM          = RISC_OSLib:o.c_abssym
WRAPPER         = RISC_OSLib:s.ModuleWrap

SOCKLIB         = TCPIPLibs:o.socklib5

EVENTLIB        = tbox:o.eventlib
TOOLBOXLIB      = tbox:o.toolboxlib
WIMPLIB         = tbox:o.wimplib
TEXTGADGETLIB   = tbox:o.textgadget

ACORNSMTPLIB    = <CExport$Dir>.o.AcornSMTP
ACORNPOP3LIB    = <CExport$Dir>.o.AcornPOP3

NFELIB          = <Lib$Dir>.NetFax.NFELib.o.NFELib
EMAILLIB	= <Lib$Dir>.Email.EmailLib.o.EmailLib
UILIB		= <Lib$Dir>.NetFax.UILib.o.UILib
ERRORLIB	= ${LIBDIR}.ErrorLib.o.ErrorLib
MSGLIB		= ${LIBDIR}.MsgLib.o.MsgLib
NUMCONVERT	= ${LIBDIR}.NumConvert.o.numconv


LIBSM = \
 ${UILIB}zm \
 ${NFELIB}zm \
 ${EMAILLIB}zm\
 ${MSGLIB}zm \
 ${ERRORLIB} \
 ${ACORNPOP3LIB} \
 ${ACORNSMTPLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB}m \
 ${SOCKLIB}zm \
 ${NUMCONVERT}zm \
 ${C++LIB}


LIBS = \
 ${UILIB} \
 ${NFELIB} \
 ${EMAILLIB}\
 ${MSGLIB} \
 ${ERRORLIB} \
 ${ACORNPOP3LIB} \
 ${ACORNSMTPLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${SOCKLIB} \
 ${NUMCONVERT} \
 ${C++LIB} \
 ${CLIB}


LIBSD = \
 ${UILIB} \
 ${NFELIB} \
 ${EMAILLIB}\
 ${MSGLIB} \
 ${ERRORLIB} \
 ${ACORNPOP3LIB} \
 ${ACORNSMTPLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${SOCKLIB} \
 ${C++LIB} \
 ${NUMCONVERT} \
 ${CLIB}


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES        =\
-Itbox:,\
C:,\
TCPIPLibs:,\
<Lib$Dir>.NetFax.NFELib,\
<Lib$Dir>.Email.EmailLib,\
<Lib$Dir>.NetFax.UILib,\
<Lib$Dir>.MsgLib,\
<Lib$Dir>.ErrorLib,\
<Lib$Dir>.DebugLib

FILES = \
 ${LDIR}.!Boot \
 ${LDIR}.!Run \
 ${LDIR}.!Sprites \
 ${LDIR}.Animations \
 ${LDIR}.Messages \
 ${LDIR}.Res \
 ${TARGET}

# Include these in the ROM module
RESFILES = \
 ${LDIR}.!Sprites \
 ${LDIR}.Animations


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = \
 o.AddrBookIF \
 o.AddrFile \
 o.CasIF\
 o.Dialler \
 o.emailrx \
 o.EnoteIF\
 o.FaxIF \
 o.FontSetup \
 o.list\
 o.LocModem \
 o.LocXfer \
 o.LogFileIF \
 o.Main \
 o.memhandle\
 o.mimedecode\
 o.MimeIF \
 o.MimeQ \
 o.OpenWin \
 o.Pop3IF \
 o.PrintQ \
 o.QuCommon\
 o.RemModem \
 o.RemXfer \
 o.runes \
 o.ScheduleIF\
 o.Screen \
 o.SendQ \
 o.Serial \
 o.serial_lib \
 o.SetupIF \
 o.slideshow\
 o.SmtpIF \
 o.StateKicke \
 o.TextTiffIF\
 o.tokenmsg\
 o.ui \
 o.zFakeMenu \
 o.zFakePrint \
 o.zFakeScan \
 o.ZmodemIF


OBJSZ = \
 oz.AddrBookIF \
 oz.AddrFile \
 oz.CasIF\
 oz.Dialler \
 oz.emailrx\
 oz.EnoteIF\
 oz.FaxIF \
 oz.FontSetup \
 oz.List\
 oz.LocModem \
 oz.LocXfer \
 oz.LogFileIF \
 oz.Main \
 oz.memhandle\
 oz.mimedecode\
 oz.MimeIF \
 oz.MimeQ \
 oz.OpenWin \
 oz.Pop3IF \
 oz.PrintQ \
 oz.QuCommon\
 oz.RemModem \
 oz.RemXfer \
 oz.runes \
 oz.ScheduleIF\
 oz.Screen \
 oz.SendQ \
 oz.Serial \
 oz.serial_lib \
 oz.SetupIF \
 oz.slideshow\
 oz.SmtpIF \
 oz.StateKicke \
 oz.TextTiffIF\
 oz.tokenmsg\
 oz.ui \
 oz.zFakeMenu \
 oz.zFakePrint \
 oz.zFakeScan \
 oz.ZmodemIF


OBJSD =\
 od.AddrBookIF \
 od.AddrFile \
 od.CasIF\
 od.Dialler \
 od.emailrx\
 od.EnoteIF\
 od.FaxIF \
 od.FontSetup \
 od.List\
 od.LocModem \
 od.LocXfer \
 od.LogFileIF \
 od.Main \
 od.memhandle\
 od.mimedecode\
 od.MimeIF \
 od.MimeQ \
 od.OpenWin \
 od.Pop3IF \
 od.PrintQ \
 od.QuCommon\
 od.RemModem \
 od.RemXfer \
 od.runes \
 od.ScheduleIF\
 od.Screen \
 od.SendQ \
 od.Serial \
 od.serial_lib \
 od.SetupIF \
 od.slideshow\
 od.SmtpIF \
 od.StateKicke \
 od.TextTiffIF\
 od.tokenmsg\
 od.ui \
 od.zFakeMenu \
 od.zFakePrint \
 od.zFakeScan \
 od.ZmodemIF

#OBJSDZ =


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .s .c .c++

.c.o:;          @echo
                @echo Compiling $<
                @echo ${DFLAGS}
                @${CC} ${CFLAGS} ${DFLAGS}        -c -o $@ $<

.c.oz:;         @echo
                @echo Compiling $<
                @echo ${DZFLAGS}
                @${CC} ${CFLAGS} ${DZFLAGS}       -c -zM -zps1 -DROM -o $@ $<

.c.od:;         @echo
                @echo Compiling $<
                @echo ${DDFLAGS}
                @${CC} ${CFLAGS} ${DDFLAGS}       -c -g -o $@ $<

.c.odz:;        @echo
                @echo Compiling $<
                @echo ${DDZFLAGS}
                @${CC} ${CFLAGS} ${DDZFLAGS}      -c -zM -zps1 -DROM -o $@ $<

.c++.o:;        @echo
                @echo Compiling $<
                @echo ${DFLAGS}
                @${C++} ${C++FLAGS} ${DFLAGS}     -c -o $@ $<

.c++.oz:;       @echo
                @echo Compiling $<
                @echo ${DZFLAGS}
                @${C++} ${C++FLAGS} ${DZFLAGS}    -c -zM -zps1 -DROM -o $@ $<

.c++.od:;       @echo
                @echo Compiling $<
                @echo ${DDFLAGS}
                @${C++} ${C++FLAGS} ${DDFLAGS}    -c +g -o $@ $<

.c++.odz:;      @echo
                @echo Compiling $<
                @echo ${DDZFLAGS}
                @${C++} ${C++FLAGS} ${DDZFLAGS}   -c -zM -zps1 -DROM -o $@ $<

.s.o:;          @echo
                @echo Assembling $<
                @echo ${AFLAGS}
                @${AS} ${AFLAGS} $< $@



# -----------------------------------------------------------------------------
# Main rules
#

all: ${FILES}
        @echo ${COMPONENT}: Application built (Disc)

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install_common: ${FILES}
        @echo
        ${CP} ${LDIR}.!Boot             ${INSTDIR}.!Boot        ${CPFLAGS}
        ${CP} ${TARGET}                 ${INSTDIR}.!RunImage    ${CPFLAGS}
        ${CP} ${LDIR}.!Sprites          ${INSTDIR}.!Sprites     ${CPFLAGS}
        ${CP} ${LDIR}.Animations        ${INSTDIR}.Animations   ${CPFLAGS}
        ${CP} ${LDIR}.Messages          ${INSTDIR}.Messages     ${CPFLAGS}
        ${CP} ${LDIR}.Reports           ${INSTDIR}.Reports       ${CPFLAGS}
        ${CP} ${LDIR}.Res               ${INSTDIR}.Res          ${CPFLAGS}

install: install_common
        ${CP} ${LDIR}.!Run              ${INSTDIR}.!Run         ${CPFLAGS}
        @echo
        @echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
        ${CP} ${LDIR}.!RunD             ${INSTDIR}.!Run         ${CPFLAGS}
        @echo
        @echo ${COMPONENT}: Debug application installed ${INSTDIR}

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo
        @echo ${COMPONENT}: Module installed (ROM)

resources:
	@echo ${RESDIR}
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESAPP}
        ${MKDIR} ${RESDIR}.User
        ${CP} ${LDIR}.ROM.!Run          ${RESAPP}.!Run          ${CPFLAGS}
        ${CP} ${LDIR}.!Sprites          ${RESDIR}.!Sprites      ${CPFLAGS}
        ${CP} ${LDIR}.Animations        ${RESDIR}.Animations    ${CPFLAGS}
        ${CP} ${LDIR}.Messages          ${RESDIR}.Messages      ${CPFLAGS}
        ${CP} ${LDIR}.Reports           ${RESDIR}.Reports       ${CPFLAGS}
        ${CP} ${LDIR}.Res               ${RESDIR}.Res           ${CPFLAGS}
        @echo
        @echo ${COMPONENT}: Resource files copied to Messages module

clean:
        @echo starting
        ${WIPE} abs             ${WFLAGS}
        ${WIPE} aof             ${WFLAGS}
        ${WIPE} linked          ${WFLAGS}
        ${WIPE} map             ${WFLAGS}
        ${WIPE} o               ${WFLAGS}
        ${WIPE} od              ${WFLAGS}
        ${WIPE} oz              ${WFLAGS}
        ${WIPE} odz             ${WFLAGS}
        ${WIPE} rm              ${WFLAGS}
        ${WIPE} syms            ${WFLAGS}
        ${RM}   s.AppName
        ${RM}   s.ModuleWrap
        ${RM}   o.dirs
        @echo
        @echo ${COMPONENT}: Cleaned

o.dirs:
        @${MKDIR} abs
        @${MKDIR} aof
        @${MKDIR} linked
        @${MKDIR} map
        @${MKDIR} o
        @${MKDIR} od
        @${MKDIR} oz
        @${MKDIR} odz
        @${MKDIR} rm
        @${MKDIR} syms
        @${MKDIR} Targets
        @${MKDIR} Targets.!Daytona
        create o.dirs
        stamp  o.dirs


# -----------------------------------------------------------------------------
# Development rules
#

linkmap: ${OBJS} ${LIBS}
        ${LD} -map -o null:x ${OBJS} ${LIBS} > map.linked

map:
        ${LD} -map -bin -base 0 -o null: ${OBJS} ${LIBS} > map.base0

clean_all: clean
        ${WIPE} Targets         ${WFLAGS}
        @echo ${COMPONENT}: Cleaned all


# -----------------------------------------------------------------------------
# Final linkfor ROM Image (using given base address)
#

rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} \
                ${ROM_MODULE} ${ABSSYM} -map > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

oz.ModuleWrap: s.ModuleWrap s.AppName ${RESFILES}
        ${AS} ${AFLAGS} s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
        ${CP} ${WRAPPER} $@ ${CPFLAGS}

s.AppName: ${LDIR}.Messages
        awk -f awk.AppName ${LDIR}.Messages > $@


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: ${OBJS} ${LIBS} o.dirs
        @echo
        ${LD} -s syms.!RI -o $@ ${OBJS} ${LIBS}
        ${SQUEEZE} ${SQFLAGS} $@

abs.!RI_D: ${OBJSD} ${LIBSD} o.dirs
        @echo
        ${LD} -debug -s syms.!RI_D -o $@ ${OBJSD} ${LIBSD}

${MODULE}: oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB} o.dirs
        @echo
        ${LD} -s syms.RAMMod -o $@ -module oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSZ} ${ROMCSTUBS} ${LIBSM} o.dirs
        @echo
        ${LD} -s syms.ROMMod -o $@ -aof oz.ModuleWrap ${OBJSZ} ${LIBSM} ${ROMCSTUBS}

rm.!DaytonaD: odz.ModuleWrap ${OBJSDZ} ${LIBSM} o.dirs
        @echo
        ${LD} -s syms.ROMDMod -o $@ -module oz.ModuleWrap ${OBJSDZ} ${LIBSM}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
