# Makefile for Global Assembler Header files
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 26-May-94  AMcC   Created.
# 28-Jul-94  AMcC   removed build rules for h.swis (done by RISC_OSLib)
# 11-Aug-94  AMcC   Don't create an Arthur directory in Export.Hdr.Global
# 19-Oct-94  AMcC   Export ImageSize hdrs
# 31-Oct-94  AMcC   Export IO.IOMDL (IOMD for Morris)
# 03-Dec-96  RWB    Don't derive headers, import them instead.
# 21-Aug-99  SNB    Restored derived headers now that our master copies 
#                   are in CVS. Added use of OPTIONS=ALL to force all 
#                   Machine and ImageSize headers to be exported.
#

#
# Paths
#
HDRDIR = <hdr$dir>.Global
HDIR   = <CExport$dir>

# Generic options:
#
MKDIR    = mkdir -p
AC       = access
AS       = aasm
CP       = copy
RM       = remove
TOKENISE = tokenise
HDR2H    = build:Hdr2H

CFLAGS   = -c -depend !Depend ${THROWBACK} -IC:
AFLAGS   = -depend !Depend ${THROWBACK} -stamp -quit
ACFLAGS  = rw/r
CPFLAGS  = ~cfr~v

TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = HdrSrc

EXPORTS   = \
 ${HDRDIR}.AUN \
 ${HDRDIR}.CMOS \
 ${HDRDIR}.Countries \
 ${HDRDIR}.CPU.ARM600 \
 ${HDRDIR}.CPU.Generic26 \
 ${HDRDIR}.DDVMacros \
 ${HDRDIR}.Debug \
 ${HDRDIR}.Devices \
 ${HDRDIR}.DevNos \
 ${HDRDIR}.DrawObj \
 ${HDRDIR}.DrawTags \
 ${HDRDIR}.ExtraLong \
 ${HDRDIR}.FDC.FDC711 \
 ${HDRDIR}.FileTypes \
 ${HDRDIR}.FSNumbers \
 ${HDRDIR}.Heap \
 ${HDRDIR}.ImageSize.<ImageSize> \
 ${HDRDIR}.IO.GenericIO \
 ${HDRDIR}.IO.IOC \
 ${HDRDIR}.IO.IOC-A1 \
 ${HDRDIR}.IO.IOEB \
 ${HDRDIR}.IO.IOMD \
 ${HDRDIR}.IO.IOMDL \
 ${HDRDIR}.KEYV \
 ${HDRDIR}.ListOpts \
 ${HDRDIR}.MEMM.ARM600 \
 ${HDRDIR}.MEMM.MEMC1 \
 ${HDRDIR}.Machine.<Machine> \
 ${HDRDIR}.Machine.Machine \
 ${HDRDIR}.Macros \
 ${HDRDIR}.Messages \
 ${HDRDIR}.NdrDebug \
 ${HDRDIR}.NewErrors \
 ${HDRDIR}.OsWords \
 ${HDRDIR}.PaletteV \
 ${HDRDIR}.PDriverReg \
 ${HDRDIR}.PDumperReg \
 ${HDRDIR}.PoduleReg \
 ${HDRDIR}.Pointer \
 ${HDRDIR}.PrintTypes \
 ${HDRDIR}.Proc \
 ${HDRDIR}.Services \
 ${HDRDIR}.Sprite \
 ${HDRDIR}.Symbols \
 ${HDRDIR}.System \
 ${HDRDIR}.SWIs \
 ${HDRDIR}.Tokens \
 ${HDRDIR}.UpCall \
 ${HDRDIR}.VIDC.VIDC1a \
 ${HDRDIR}.VIDC.VIDC20 \
 ${HDIR}.Global.h.AUN \
 ${HDIR}.Global.h.CMOS \
 ${HDIR}.Global.h.Countries \
 ${HDIR}.Global.h.Devices \
 ${HDIR}.Global.h.DevNos \
 ${HDIR}.Global.h.DrawObj \
 ${HDIR}.Global.h.DrawTags \
 ${HDIR}.Global.h.EnvNumbers \
 ${HDIR}.Global.h.FileTypes \
 ${HDIR}.Global.h.FreeWay \
 ${HDIR}.Global.h.FSNumbers \
 ${HDIR}.Global.h.Heap \
 ${HDIR}.Global.h.KeyV \
 ${HDIR}.Global.h.OsWords \
 ${HDIR}.Global.h.PaletteV \
 ${HDIR}.Global.h.PDriverReg \
 ${HDIR}.Global.h.PDumperReg \
 ${HDIR}.Global.h.PoduleReg \
 ${HDIR}.Global.h.Pointer \
 ${HDIR}.Global.h.Services \
 ${HDIR}.Global.h.SWIs \
 ${HDIR}.Global.h.Upcall \
 ${HDIR}.Global.h.Tokens \

#
# Derived assembler header files
#
FILETYPES = Derived.FileTypes
SWIS      = Derived.SWIs

#
# Generic rules:
#

export: ${OPTIONS} ${EXPORTS} Dirs
        @echo ${COMPONENT}: export complete

Dirs:
        ${MKDIR} <build$dir>.Export.C.h
        ${MKDIR} <build$dir>.Export.C.o
        ${MKDIR} <build$dir>.Export.Hdr.Global
        ${MKDIR} <build$dir>.Export.Hdr.Interface
        ${MKDIR} <build$dir>.Export.Hdr.Interface2
        ${MKDIR} ${HDRDIR}.CPU
        ${MKDIR} ${HDRDIR}.FDC
        ${MKDIR} ${HDRDIR}.IO
        ${MKDIR} ${HDRDIR}.ImageSize
        ${MKDIR} ${HDRDIR}.Machine
        ${MKDIR} ${HDRDIR}.MEMM
        ${MKDIR} ${HDRDIR}.VIDC
        ${MKDIR} ${HDIR}.Global.h
        ${MKDIR} ${HDIR}.Interface.h
        ${MKDIR} ${HDIR}.Interface2.h

clean:
	${RM} ${FILETYPES}
	${RM} ${SWIS}
        @echo ${COMPONENT}: derived headers cleaned

#
# Special rule for exporting all ImageSize and Machine files.
# Trigger by invoking the export with OPTIONS=ALL.
#
ALL: ${HDRDIR}.Machine.Machine ${HDRDIR}.ImageSize.<ImageSize>
	${CP} hdr.Machine ${HDRDIR}.Machine ${CPFLAGS}
	${CP} hdr.ImageSize ${HDRDIR}.ImageSize ${CPFLAGS}

#
# Rules for deriving headers:
#
${FILETYPES}: Register.ISV-Types s.DoTypes
        ${AS} ${AFLAGS} -from s.DoTypes -to $@ 
        ${AC} $@ ${ACFLAGS}
        settype $@ text

${SWIS}: Register.ISV-SWIs s.DoSWIs
        ${AS} ${AFLAGS} -from s.DoSWIs -to $@ 
        ${AC} $@ ${ACFLAGS}
        settype $@ text

#
# Rules for exporting:
#

# Registration headers:
${HDRDIR}.AUN:        Register.AUN;            ${CP} Register.AUN        $@ ${CPFLAGS}
${HDRDIR}.Countries:  Register.Countries;      ${CP} Register.Countries  $@ ${CPFLAGS}
${HDRDIR}.Devices:    Register.Devices;        ${CP} Register.Devices    $@ ${CPFLAGS}
${HDRDIR}.DevNos:     Register.DevNos;         ${CP} Register.DevNos     $@ ${CPFLAGS}
${HDRDIR}.DrawObj:    Register.DrawObj;        ${CP} Register.DrawObj    $@ ${CPFLAGS}
${HDRDIR}.DrawTags:   Register.DrawTags;       ${CP} Register.DrawTags   $@ ${CPFLAGS}
${HDRDIR}.FSNumbers:  Register.FSNumbers;      ${CP} Register.FSNumbers  $@ ${CPFLAGS}
${HDRDIR}.KEYV:       Register.KEYV;           ${CP} Register.KEYV       $@ ${CPFLAGS}
${HDRDIR}.Messages:   Register.Messages;       ${CP} Register.Messages   $@ ${CPFLAGS}
${HDRDIR}.NewErrors:  Register.NewErrors;      ${CP} Register.NewErrors  $@ ${CPFLAGS}
${HDRDIR}.OsWords:    Register.OsWords;        ${CP} Register.OsWords    $@ ${CPFLAGS}
${HDRDIR}.PaletteV:   Register.PaletteV;       ${CP} Register.PaletteV   $@ ${CPFLAGS}
${HDRDIR}.Pointer:    Register.Pointer;        ${CP} Register.Pointer    $@ ${CPFLAGS}
${HDRDIR}.PDriverReg: Register.PDriverReg;     ${CP} Register.PDriverReg $@ ${CPFLAGS}
${HDRDIR}.PDumperReg: Register.PDumperReg;     ${CP} Register.PDumperReg $@ ${CPFLAGS}
${HDRDIR}.PoduleReg:  Register.PoduleReg;      ${CP} Register.PoduleReg  $@ ${CPFLAGS}
${HDRDIR}.PrintTypes: Register.PrintTypes;     ${CP} Register.PrintTypes $@ ${CPFLAGS}
${HDRDIR}.Services:   Register.Services;       ${CP} Register.Services   $@ ${CPFLAGS}
${HDRDIR}.UpCall:     Register.UpCall;         ${CP} Register.UpCall     $@ ${CPFLAGS}

# Derived registration headers:
${HDRDIR}.FileTypes:  ${FILETYPES};            ${CP} ${FILETYPES}        $@ ${CPFLAGS}
${HDRDIR}.SWIs:       ${SWIS};                 ${CP} ${SWIS}             $@ ${CPFLAGS}

# The C versions of registration headers:
${HDIR}.Global.h.AUN:        Register.AUN ${HDR2H};            perl ${HDR2H} Register.AUN        $@
${HDIR}.Global.h.Countries:  Register.Countries ${HDR2H};      perl ${HDR2H} Register.Countries  $@
${HDIR}.Global.h.Devices:    Register.Devices ${HDR2H};        perl ${HDR2H} Register.Devices    $@
${HDIR}.Global.h.DevNos:     Register.DevNos ${HDR2H};         perl ${HDR2H} Register.DevNos     $@
${HDIR}.Global.h.DrawObj:    Register.DrawObj ${HDR2H};        perl ${HDR2H} Register.DrawObj    $@
${HDIR}.Global.h.DrawTags:   Register.DrawTags ${HDR2H};       perl ${HDR2H} Register.DrawTags   $@
${HDIR}.Global.h.EnvNumbers: Register.EnvNumbers ${HDR2H};     perl ${HDR2H} Register.EnvNumbers $@
${HDIR}.Global.h.FreeWay:    Register.FreeWay ${HDR2H};        perl ${HDR2H} Register.FreeWay    $@
${HDIR}.Global.h.FSNumbers:  Register.FSNumbers ${HDR2H};      perl ${HDR2H} Register.FSNumbers  $@
${HDIR}.Global.h.KeyV:       Register.KeyV ${HDR2H};           perl ${HDR2H} Register.KeyV       $@
${HDIR}.Global.h.Messages:   Register.Messages ${HDR2H};       perl ${HDR2H} Register.Messages   $@
# NewErrors still has a few problems.  Partial support is there.
${HDIR}.Global.h.OsWords:    Register.OsWords ${HDR2H};        perl ${HDR2H} Register.OsWords    $@
${HDIR}.Global.h.PaletteV:   Register.PaletteV ${HDR2H};       perl ${HDR2H} Register.PaletteV   $@
${HDIR}.Global.h.PDriverReg: Register.PDriverReg ${HDR2H};     perl ${HDR2H} Register.PDriverReg $@
${HDIR}.Global.h.PDumperReg: Register.PDumperReg ${HDR2H};     perl ${HDR2H} Register.PDumperReg $@
${HDIR}.Global.h.PoduleReg:  Register.PoduleReg ${HDR2H};      perl ${HDR2H} Register.PoduleReg  $@
${HDIR}.Global.h.Pointer:    Register.Pointer ${HDR2H};        perl ${HDR2H} Register.Pointer    $@
# PrintTypes is just full of comments
${HDIR}.Global.h.Services:   Register.Services ${HDR2H};       perl ${HDR2H} Register.Services   $@
${HDIR}.Global.h.UpCall:     Register.UpCall ${HDR2H};         perl ${HDR2H} Register.UpCall     $@

# The C versions of derived registration headers:
${HDIR}.Global.h.FileTypes:  ${FILETYPES} ${HDR2H};            perl ${HDR2H} ${FILETYPES}        $@
${HDIR}.Global.h.SWIs:       ${SWIS} ${HDR2H};                 perl ${HDR2H} ${SWIS}             $@


# The C versions of local header files.
# Files which don't work yet:
#   No files remaining which do not work at all.
# Files which work with some warnings (which indicate incorrect constants have been declared)
# and have rules declared below but which are NOT GENERATED because they are not in the export list:
#   Sprite      Messages
# Files which have no equivalent.
#   DDVMacros   Debug       ExtraLong   ListOpts    Macros      NdrDebug    Proc        Symbols
${HDIR}.Global.h.Heap:       Hdr.Heap ${HDR2H};           perl ${HDR2H} Hdr.Heap       $@
${HDIR}.Global.h.Sprite:     Hdr.Sprite ${HDR2H};         perl ${HDR2H} Hdr.Sprite     $@
${HDIR}.Global.h.Tokens:     Hdr.Tokens ${HDR2H};         perl ${HDR2H} Hdr.Tokens     $@
${HDIR}.Global.h.CMOS:       Hdr.CMOS ${HDR2H};           perl ${HDR2H} Hdr.CMOS       $@


# Machine/hardware specific headers:
${HDRDIR}.Machine.<Machine>: hdr.Machine.<Machine>; ${CP} hdr.Machine.<Machine>    $@ ${CPFLAGS}
${HDRDIR}.Machine.Machine:   hdr.Machine.Machine;   ${CP} hdr.Machine.Machine      $@ ${CPFLAGS}
${HDRDIR}.ImageSize.<ImageSize>: hdr.ImageSize.<ImageSize>; ${CP} hdr.ImageSize.<ImageSize> $@ ${CPFLAGS}
${HDRDIR}.CPU.ARM600:        hdr.CPU.ARM600;        ${CP} hdr.CPU.ARM600           $@ ${CPFLAGS}
${HDRDIR}.CPU.Generic26:     hdr.CPU.Generic26;     ${CP} hdr.CPU.Generic26        $@ ${CPFLAGS}
${HDRDIR}.FDC.FDC711:        hdr.FDC.FDC711;        ${CP} hdr.FDC.FDC711           $@ ${CPFLAGS}
${HDRDIR}.IO.GenericIO:      hdr.IO.GenericIO;      ${CP} hdr.IO.GenericIO 	   $@ ${CPFLAGS}
${HDRDIR}.IO.IOC:            hdr.IO.IOC;            ${CP} hdr.IO.IOC               $@ ${CPFLAGS}
${HDRDIR}.IO.IOC-A1:         hdr.IO.IOC-A1;         ${CP} hdr.IO.IOC-A1            $@ ${CPFLAGS}
${HDRDIR}.IO.IOEB:           hdr.IO.IOEB;           ${CP} hdr.IO.IOEB              $@ ${CPFLAGS}
${HDRDIR}.IO.IOMD:           hdr.IO.IOMD;           ${CP} hdr.IO.IOMD              $@ ${CPFLAGS}
${HDRDIR}.IO.IOMDL:          hdr.IO.IOMDL;          ${CP} hdr.IO.IOMDL             $@ ${CPFLAGS}
${HDRDIR}.MEMM.ARM600:       hdr.MEMM.ARM600;       ${CP} hdr.MEMM.ARM600          $@ ${CPFLAGS}
${HDRDIR}.MEMM.MEMC1:        hdr.MEMM.MEMC1;        ${CP} hdr.MEMM.MEMC1           $@ ${CPFLAGS}
${HDRDIR}.VIDC.VIDC1a:       hdr.VIDC.VIDC1a;       ${CP} hdr.VIDC.VIDC1a          $@ ${CPFLAGS}
${HDRDIR}.VIDC.VIDC20:       hdr.VIDC.VIDC20;       ${CP} hdr.VIDC.VIDC20          $@ ${CPFLAGS}

# Macros:
${HDRDIR}.DDVMacros:         hdr.DDVMacros;         ${CP} hdr.DDVMacros            $@ ${CPFLAGS}
${HDRDIR}.Debug:             hdr.Debug;             ${CP} hdr.Debug                $@ ${CPFLAGS}
${HDRDIR}.ExtraLong:         hdr.ExtraLong;         ${CP} hdr.ExtraLong            $@ ${CPFLAGS}
${HDRDIR}.Macros:            hdr.Macros;            ${CP} hdr.Macros               $@ ${CPFLAGS}
${HDRDIR}.NdrDebug:          hdr.NdrDebug;          ${CP} hdr.NdrDebug             $@ ${CPFLAGS}
${HDRDIR}.Proc:              hdr.Proc;              ${CP} hdr.Proc                 $@ ${CPFLAGS}

# The rest:
${HDRDIR}.CMOS:              hdr.CMOS;              ${CP} hdr.CMOS                 $@ ${CPFLAGS}
${HDRDIR}.Heap:              hdr.Heap;              ${CP} hdr.Heap                 $@ ${CPFLAGS}
${HDRDIR}.ListOpts:          hdr.ListOpts;          ${CP} hdr.ListOpts             $@ ${CPFLAGS}
${HDRDIR}.Sprite:            hdr.Sprite;            ${CP} hdr.Sprite               $@ ${CPFLAGS}
${HDRDIR}.Symbols:           hdr.Symbols;           ${CP} hdr.Symbols              $@ ${CPFLAGS}
${HDRDIR}.System:            hdr.System;            ${CP} hdr.System               $@ ${CPFLAGS}
${HDRDIR}.Tokens:            hdr.Tokens;            ${CP} hdr.Tokens               $@ ${CPFLAGS}

# Dynamic dependencies:
