# Makefile for C and RISC_OS Headers, Libraries and Stubs
#
#---------------------------------------------------------------------------------------------
# Library             Exported as             Macro        Runs in   Type          Use
#---------------------------------------------------------------------------------------------
# lib.stubs           CLib:o.stubs            ${CLIB}       RAM      C_Stubs
# lib.risc_oslib      RISC_OSLib:o.risc_oslib ${RLIB}
# lib.rstubs          RISC_OSLib:o.rstubs     ${RSTUBS}     RAM      RISC_OSStubs
# lib.romcstubs       RISC_OSLib:o.romcstubs  ${ROMCSTUBS}  ROM      C_Stubs       Module_Only
# lib.romstubs        RISC_OSLib:o.romstubs   ${ROMSTUBS}   ROM      RISC_OSStubs  Module_Only
# lib.romastubs                                             ROM      RISC_OSStubs  Apps_Only
# lib.ansilib
# lib.clib
# lib.OverMgr         CLib:o.overmgr
# lib.riscoslibm
#---------------------------------------------------------------------------------------------
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 20-Jul-94  AMcC         Created.
# 28-Sep-94  AMcC         Now exports some internal headers for Edit
#

# Set default component and target.
# Alternative possible target is RISC_OSLib
COMPONENT ?= SharedCLibrary
TARGET    ?= CLib

#
# Paths
#
EXP_HDR = <export$dir>
LIBDIR  = <Lib$Dir>

#
# Generic options:
#
AS      = aasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
LIBFILE = libfile
MKDIR   = do mkdir -p
MODSQZ  = modsqz
OBJASM  = objasm
RM      = remove
WIPE    = -wipe

# Note the output redirection on the end of this macro to enable parameters
# to match those of the old application xtentries.
XTENT   = perl build:xtentries >

DIFF    = gnu.diff -df >null:

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} -DDDE -fK
CPFLAGS = ~cfr~v
WFLAGS  = ~c~r~v

# create a new library, add symbol table
LIBFLAGS = -c -o

#
# Flags common to *all* phases of linking
#
LDFLAGS = -linkversion 519

#
# Include files
#
INCLUDES =

STK_EXTN =

# RISCOS_Lib txt flags
DFLAGS =\
 -DFIELDNUM\
 -DBIG_WINDOWS\
 -DSETOPTIONS\
 -DALLOW_OLD_PATTERNS\
 -DSET_MISC_OPTIONS

# ANSILib and stub flags
CLFLAGS =\
 -DDEFAULT_TEXT

ALFLAGS =\
 -PD "DEFAULT_TEXT SETL {TRUE}"

VLFLAGS =\
 -DLIB_SHARED="\"\""

# Shared C Library flags
CSFLAGS =\
 -I@,^.clib\
 -DSHARED_C_LIBRARY\
 -DUROM\
 ${SCL_APCS}

VSFLAGS =\
 -DLIB_SHARED="\"Shared \""

ASFLAGS =\
 ${SCL_APCS}

# Setting UROM on the command-line for rlib
UROM_DEFS =\
 -PD "UROM SETL {FALSE}"
UROM_ROM_DEFS =\
 -PD "UROM SETL {TRUE}"

VPATH = @ kernel clib rlib

.SUFFIXES:  .o .o_rl .m_o_rl .rm_o .rm_o_rl
#
# Rule patterns
#
# Note
.c.o:;        ${CC} ${CFLAGS} -fah ${CLFLAGS} -o $@ $<
.c.o_rl:;     ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.m_o_rl:;   ${CC} ${CFLAGS} -ffah ${DFLAGS} -zM -o $@ $<
.c.rm_o:;     ${CC} ${CFLAGS} -ffah ${DFLAGS} -zm1 ${CSFLAGS} ${OPTIONS} -o $@ $<
.c.rm_o_rl:;  ${CC} ${CFLAGS} -ffah ${DFLAGS} -zm1 ${CSFLAGS} ${OPTIONS} -o $@ $<
.s.o:;        ${OBJASM} ${AFLAGS} ${ALFLAGS} $< $@
.s.o_rl:;     ${OBJASM} ${AFLAGS} $< $@
.s.m_o_rl:;   ${OBJASM} ${AFLAGS} $< $@
.s.rm_o:;     ${OBJASM} ${AFLAGS} ${ASFLAGS} ${OPTIONS} $< $@
.s.rm_o_rl:;  ${OBJASM} ${AFLAGS} ${ASFLAGS} ${OPTIONS} $< $@

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym
C_ABSSYM  = RISC_OSLib:o.c_abssym
A_ABSSYM  = RISC_OSLib:o.a_abssym

#
# Program specific options:
#
HEADERS =\
 CLIB:h.assert \
 CLIB:h.ctype \
 CLIB:h.errno \
 CLIB:h.float \
 CLIB:h.inttypes \
 CLIB:h.iso646 \
 CLIB:h.kernel \
 CLIB:h.limits \
 CLIB:h.locale \
 CLIB:h.math \
 CLIB:h.setjmp \
 CLIB:h.signal \
 CLIB:h.stdarg \
 CLIB:h.stdbool \
 CLIB:h.stddef \
 CLIB:h.stdint \
 CLIB:h.stdio \
 CLIB:h.stdlib \
 CLIB:h.string \
 CLIB:h.swis \
 CLIB:h.time \
 CLIB:h.varargs \
 RISC_OSLib:h.akbd \
 RISC_OSLib:h.alarm \
 RISC_OSLib:h.baricon \
 RISC_OSLib:h.bbc \
 RISC_OSLib:h.colourmenu \
 RISC_OSLib:h.colourpick \
 RISC_OSLib:h.colourtran \
 RISC_OSLib:h.coords \
 RISC_OSLib:h.ctl \
 RISC_OSLib:h.dbox \
 RISC_OSLib:h.dboxfile \
 RISC_OSLib:h.dboxquery \
 RISC_OSLib:h.dboxtcol \
 RISC_OSLib:h.dragasprit \
 RISC_OSLib:h.drawfdiag \
 RISC_OSLib:h.drawferror \
 RISC_OSLib:h.drawfobj \
 RISC_OSLib:h.drawftypes \
 RISC_OSLib:h.drawmod \
 RISC_OSLib:h.event \
 RISC_OSLib:h.fileicon \
 RISC_OSLib:h.flex \
 RISC_OSLib:h.font \
 RISC_OSLib:h.fontlist \
 RISC_OSLib:h.fontselect \
 RISC_OSLib:h.heap \
 RISC_OSLib:h.help \
 RISC_OSLib:h.magnify \
 RISC_OSLib:h.menu \
 RISC_OSLib:h.msgs \
 RISC_OSLib:h.msgtrans \
 RISC_OSLib:h.os \
 RISC_OSLib:h.pointer \
 RISC_OSLib:h.print \
 RISC_OSLib:h.res \
 RISC_OSLib:h.resspr \
 RISC_OSLib:h.saveas \
 RISC_OSLib:h.sprite \
 RISC_OSLib:h.template \
 RISC_OSLib:h.trace \
 RISC_OSLib:h.txt \
 RISC_OSLib:h.txtedit \
 RISC_OSLib:h.txtopt \
 RISC_OSLib:h.txtscrap \
 RISC_OSLib:h.txtwin \
 RISC_OSLib:h.typdat \
 RISC_OSLib:h.visdelay \
 RISC_OSLib:h.werr \
 RISC_OSLib:h.wimp \
 RISC_OSLib:h.wimpt \
 RISC_OSLib:h.win \
 RISC_OSLib:h.xferrecv \
 RISC_OSLib:h.xfersend

# Unfortunately these need to be exported to build Edit
INTERNAL_HEADERS =\
 RISC_OSLib:h.EditIntern.message \
 RISC_OSLib:h.EditIntern.txtar \
 RISC_OSLib:h.EditIntern.txtfile \
 RISC_OSLib:h.EditIntern.txtoptmenu

LIBRARIES =\
 Clib:o.ansilib \
 CLib:o.stubs \
 RISC_OSLib:o.risc_oslib \
 RISC_OSLib:o.riscoslibm \
 RISC_OSLib:o.rstubs \
 RISC_OSLib:o.romstubs \
 RISC_OSLib:o.romastubs \
 RISC_OSLib:o.romcstubs

EXPORTED_LIBS =\
 Clib:o.ansilib \
 CLib:o.stubs \
 RISC_OSLib:o.risc_oslib \
 RISC_OSLib:o.riscoslibm \
 RISC_OSLib:o.rstubs \
 RISC_OSLib:o.romstubs \
 RISC_OSLib:o.romcstubs

MODWRAP   = RISC_OSLib:s.modulewrap

OBJECT    = aof.${TARGET}
EXPORTS   = ${MODWRAP} \
            ${EXPORTED_LIBS} \
            ${HEADERS} \
            ${INTERNAL_HEADERS}

#
# Objects for Clib:o.ansilib
#
ANSI_OBJS =\
 o.alloc o.armprof o.armsys o.clib o.ctype o.error o.fpprintf \
 o.kernel o.locale o.math o.memcpset o.overmgr o.printf o.scanf \
 o.signal o.sort o.stdio o.stdlib o.string o.swiv o.time o.fenv \
 o.longlong

RM_OBJS =\
 rm_o.k_modbody \
 rm_o.cl_modbody \
 rm_o.alloc \
 rm_o.armsys \
 rm_o.bsearch_a \
 rm_o.ctype \
 rm_o.error \
 rm_o.fpprintf \
 rm_o.locale \
 rm_o.longlong \
 rm_o.math \
 rm_o.memcpset \
 rm_o.printf \
 rm_o.scanf \
 rm_o.signal \
 rm_o.sort \
 rm_o.stdio \
 rm_o.stdlib \
 rm_o.string \
 rm_o.time \
 rm_o.fenv \
 rm_o.swiv \
 rm_o.cl_spare

#
# Objects for lib.clib (relocatable module)
#
CLIB_MOD_OBJS =\
 rm_o.cl_rmhdr \
 o.initmodule \
 ${RM_OBJS}

#
# Objects for a C library only ROM shared library
#
CLIB_ROM_OBJS =\
 rm_o.cl_rmhdr \
 rm_o.initmodule \
 ${RM_OBJS}

#
# Objects for ROM shared library
#
ROM_OBJS =\
 rm_o.cl_rmhdr \
 rm_o_rl.initmodule \
 ${RM_OBJS} \
 rm_o_rl.rl_modbody \
 rm_o_rl.akbd \
 rm_o_rl.alarm \
 rm_o_rl.baricon \
 rm_o_rl.bastxt \
 rm_o_rl.bbc \
 rm_o_rl.colourmenu \
 rm_o_rl.colourtran \
 rm_o_rl.dbox \
 rm_o_rl.dboxfile \
 rm_o_rl.dboxquery \
 rm_o_rl.dboxtcol \
 rm_o_rl.dragasprit \
 rm_o_rl.drawmod \
 rm_o_rl.event \
 rm_o_rl.fileicon \
 rm_o_rl.flex \
 rm_o_rl.font \
 rm_o_rl.fontlist \
 rm_o_rl.fontselect \
 rm_o_rl.heap \
 rm_o_rl.magnify \
 rm_o_rl.menu \
 rm_o_rl.msgs \
 rm_o_rl.os \
 rm_o_rl.pointer \
 rm_o_rl.poll \
 rm_o_rl.res \
 rm_o_rl.resspr \
 rm_o_rl.saveas \
 rm_o_rl.sprite \
 rm_o_rl.swi \
 rm_o_rl.template \
 rm_o_rl.txt \
 rm_o_rl.txt1 \
 rm_o_rl.txt3 \
 rm_o_rl.txtar \
 rm_o_rl.txtedit \
 rm_o_rl.txtfile \
 rm_o_rl.txtfind \
 rm_o_rl.txtmisc \
 rm_o_rl.txtoptmenu \
 rm_o_rl.txtscrap \
 rm_o_rl.txtundo \
 rm_o_rl.txtwin \
 rm_o_rl.visdelay \
 rm_o_rl.werr \
 rm_o_rl.wimp \
 rm_o_rl.wimpt \
 rm_o_rl.win \
 rm_o_rl.xferrecv \
 rm_o_rl.xfersend \
 rm_o_rl.help \
 rm_o_rl.print \
 rm_o_rl.txtopt \
 rm_o_rl.txtregexp \
 rm_o_rl.rl_spare

#
# Objects for RISCOSLIB:o.risc_oslib
#
RLIB_OBJS =\
 o_rl.akbd o_rl.alarm o_rl.baricon o_rl.bastxt o_rl.bbc o_rl.colourmenu \
 o_rl.colourtran o_rl.coords o_rl.ctl o_rl.dbox o_rl.dboxfile o_rl.dboxquery \
 o_rl.dboxtcol o_rl.dragasprit o_rl.drawcheck o_rl.drawfdiag o_rl.drawfiles \
 o_rl.drawfobj o_rl.drawmod o_rl.drawtextc o_rl.event o_rl.fileicon o_rl.flex \
 o_rl.font o_rl.fontlist o_rl.fontselect o_rl.heap o_rl.help o_rl.magnify o_rl.menu \
 o_rl.msgs o_rl.os o_rl.pointer o_rl.poll o_rl.print o_rl.res o_rl.resspr \
 o_rl.saveas o_rl.sprite o_rl.swi o_rl.template o_rl.trace o_rl.txt o_rl.txt1 \
 o_rl.txt3 o_rl.txtar o_rl.txtedit o_rl.txtfile o_rl.txtfind o_rl.txtmisc \
 o_rl.txtopt o_rl.txtoptmenu o_rl.txtregexp o_rl.txtscrap o_rl.txtundo o_rl.txtwin \
 o_rl.visdelay o_rl.werr o_rl.wimp o_rl.wimpt o_rl.win o_rl.xferrecv o_rl.xfersend

#
# Objects for RISCOSLIB:o.riscoslibm
#
RLIB_MOD_OBJS =\
 m_o_rl.akbd m_o_rl.alarm m_o_rl.baricon m_o_rl.bastxt m_o_rl.bbc \
 m_o_rl.colourmenu m_o_rl.colourtran m_o_rl.coords m_o_rl.ctl m_o_rl.dbox \
 m_o_rl.dboxfile m_o_rl.dboxquery  m_o_rl.dboxtcol m_o_rl.dragasprit \
 m_o_rl.drawcheck m_o_rl.drawfdiag m_o_rl.drawfiles m_o_rl.drawfobj m_o_rl.drawmod \
 m_o_rl.drawtextc m_o_rl.event m_o_rl.fileicon m_o_rl.flex m_o_rl.font \
 m_o_rl.fontlist m_o_rl.fontselect m_o_rl.heap m_o_rl.help m_o_rl.magnify \
 m_o_rl.menu m_o_rl.msgs m_o_rl.os m_o_rl.pointer m_o_rl.poll m_o_rl.print \
 m_o_rl.res m_o_rl.resspr m_o_rl.saveas m_o_rl.sprite m_o_rl.swi m_o_rl.template \
 m_o_rl.trace m_o_rl.txt m_o_rl.txt1 m_o_rl.txt3 m_o_rl.txtar m_o_rl.txtedit \
 m_o_rl.txtfile m_o_rl.txtfind m_o_rl.txtmisc m_o_rl.txtopt m_o_rl.txtoptmenu \
 m_o_rl.txtregexp m_o_rl.txtscrap m_o_rl.txtundo m_o_rl.txtwin m_o_rl.visdelay \
 m_o_rl.werr m_o_rl.wimp m_o_rl.wimpt m_o_rl.win m_o_rl.xferrecv m_o_rl.xfersend

#
# Build rules:
#
rom: ${OBJECT}
        @echo ${COMPONENT}: rom library built

export: export_${PHASE}
        @echo ${COMPONENT}: export complete (${PHASE})

export_hdrs: ${EXP_HDR}.SharedCLib; @

export_libs: ${EXPORTS} dirs; @

install: lib.clib
        ${MKDIR}  ${INSTDIR}
        ${CP} lib.clib ${INSTDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: ram module installed

install_rom: ${OBJECT}
        ${CP} ${OBJECT} ${INSTDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom library installed

clean:
        @echo ${COMPONENT}: *** NOT cleaned *** run locally if required

clean_all:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} o_rl.* ${WFLAGS}
        ${WIPE} m_o_rl.* ${WFLAGS}
        ${WIPE} rm_o.* ${WFLAGS}
        ${WIPE} rm_o_rl.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${WIPE} aof.* ${WFLAGS}
        ${WIPE} syms.* ${WFLAGS}
        ${RM} derived.swis
        ${RM} s.swioptions
        ${RM} s.abssym
        ${RM} s.c_abssym
        ${RM} s.a_abssym
        ${WIPE} lib.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.RISC_OSLib
        ${MKDIR} ${RESDIR}.CLib
        IfThere clib.Resources.${LOCALE}.<System>.Messages Then Set MessagesFile clib.Resources.${LOCALE}.<System>.Messages else Set MessagesFile clib.Resources.${LOCALE}.Messages
        ${CP} <MessagesFile> ${RESDIR}.CLib.Messages ${CPFLAGS}
        IfThere rlib.Resources.${LOCALE}.<System>.Messages Then Set MessagesFile rlib.Resources.${LOCALE}.<System>.Messages else Set MessagesFile rlib.Resources.${LOCALE}.Messages
        ${CP} <MessagesFile> ${RESDIR}.RISC_OSLib.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

BBETYPE = clibrary
bbe-clibrary: bbe-generic
  ${MKDIR} ${BBEDIR}.clib.Resources.${LOCALE}
  ${MKDIR} ${BBEDIR}.rlib.Resources.${LOCALE}
  ${MKDIR} ${BBEDIR}.aof
  ${MKDIR} ${BBEDIR}.map
  ${MKDIR} ${BBEDIR}.syms
  ${MKDIR} ${BBEDIR}.linked
  ${MKDIR} ${BBEDIR}.kernel.s
  ${MKDIR} ${BBEDIR}.clib.s
  ${MKDIR} ${BBEDIR}.rlib.s
  BBE_Export_Dir clib.Resources.${LOCALE}.${SYSTEM}
  BBE_Export_File clib.Resources.${LOCALE}.Messages
  BBE_Export_Dir rlib.Resources.${LOCALE}.${SYSTEM}
  BBE_Export_File rlib.Resources.${LOCALE}.Messages
  BBE_Export_File rlib.swi
  BBE_Export_File kernel.s.k_entries
  BBE_Export_File clib.s.cl_entries
  BBE_Export_File rlib.s.rl_entries
  BBE_Export_File aof.${TARGET}
  ${CP} clib.rom_link_v ${VERBATIMROMLINK} ${CPFLAGS}

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${LIBDIR}.CLib
        ${MKDIR} ${LIBDIR}.CLib.h
        ${MKDIR} ${LIBDIR}.CLib.o
        ${MKDIR} ${LIBDIR}.RISC_OSLib
        ${MKDIR} ${LIBDIR}.RISC_OSLib.h
        ${MKDIR} ${LIBDIR}.RISC_OSLib.h.EditIntern
        ${MKDIR} ${LIBDIR}.RISC_OSLib.s
        ${MKDIR} ${LIBDIR}.RISC_OSLib.o

#
# ROM target {re-linked at ROM Image build time}
#
aof.RISC_OSLib: ${ROM_OBJS}
        ${LD} ${LDFLAGS} -o $@ -aof ${ROM_OBJS}

aof.CLib: ${CLIB_ROM_OBJS}
        ${LD} ${LDFLAGS} -o $@ -aof ${CLIB_ROM_OBJS}

#
# Final link for the ROM Image {using given base address}
#
rom_link_Entries:
        ${XTENT} syms.C_Entries kernel.s.k_entries clib.s.cl_entries
        print rlib.swi { >> syms.C_Entries }
        | C_Entries *currently* only adds _swi/_swix to RO2 symbols.
        egrep -v "^(0x00000000 . )?_swix?$" < syms.C_Entries > syms.A_Entries

rom_link_RISC_OSLib: ${ROM_OBJS} rom_link_Entries
        ${XTENT} syms.Entries kernel.s.k_entries clib.s.cl_entries rlib.s.rl_entries
        print rlib.swi { >> syms.Entries }
        ${LD} ${LDFLAGS} -o linked.RISC_OSLib ${ROM_OBJS} \
              -symdefs syms.C_Entries \
              -symdefs syms.A_Entries \
              -symdefs syms.Entries \
              -symbols syms.${TARGET} \
              -bin -base ${ADDRESS} \
              -map > map.${TARGET}

rom_link_CLib: ${CLIB_ROM_OBJS} rom_link_Entries
        ${XTENT} syms.Entries kernel.s.k_entries clib.s.cl_entries
        print rlib.swi { >> syms.Entries }
        ${LD} ${LDFLAGS} -o linked.CLib ${CLIB_ROM_OBJS} \
              -symdefs syms.C_Entries \
              -symdefs syms.A_Entries \
              -symdefs syms.Entries \
              -symbols syms.${TARGET} \
              -bin -base ${ADDRESS} \
              -map > map.${TARGET}

rom_link: rom_link_${TARGET}
        ${CP} linked.${TARGET} ${LINKDIR}.${TARGET} ${CPFLAGS}
        |
        ${CP} syms.Entries ${ABSSYM} ${CPFLAGS}
        ${CP} syms.C_Entries ${C_ABSSYM} ${CPFLAGS}
        ${CP} syms.A_Entries ${A_ABSSYM} ${CPFLAGS}
        |
        @echo ${COMPONENT}: rom_link complete

#
# Build rules for Libraries and Stubs:
#
lib.ansilib: ${ANSI_OBJS}
        ${LIBFILE} ${LIBFLAGS} $@ ${ANSI_OBJS}

lib.risc_oslib: ${RLIB_OBJS}
        ${LIBFILE} ${LIBFLAGS} $@ ${RLIB_OBJS}

lib.riscoslibm: ${RLIB_MOD_OBJS}
        ${LIBFILE} ${LIBFLAGS} $@ ${RLIB_MOD_OBJS}

o.cl_stub_r: clib.s.cl_stub_r
        ${OBJASM} ${ALFLAGS} -from clib.s.cl_stub_r -to $@

o.cl_stub2_r: clib.s.cl_stub2_r
        ${OBJASM} ${ALFLAGS} -from clib.s.cl_stub2_r -to $@

o.k_stub2_r: kernel.s.k_stub2_r
        ${OBJASM} ${ALFLAGS} -from kernel.s.k_stub2_r -to $@

o_rl.rl_stub_r: rlib.s.rl_stub_r
        ${OBJASM} ${ALFLAGS} -from rlib.s.rl_stub_r -to $@

rm_o.cl_stub_rm: clib.s.cl_stub_rm
        ${OBJASM} ${ALFLAGS} -from clib.s.cl_stub_rm -to $@

rm_o.cl_stub2_rm: clib.s.cl_stub2_rm
        ${OBJASM} ${ALFLAGS} -from clib.s.cl_stub2_rm -to $@

rm_o.k_stub2_rm: kernel.s.k_stub2_rm
        ${OBJASM} ${ALFLAGS} -from kernel.s.k_stub2_rm -to $@

rm_o_rl.rl_stub_rm: rlib.s.rl_stub_rm
        ${OBJASM} ${ALFLAGS} -from rlib.s.rl_stub_rm -to $@

lib.stubs: o.cl_stub_r o.k_stub2_r o.cl_stub2_r
        ${LIBFILE} ${LIBFLAGS} $@ o.cl_stub_r o.k_stub2_r o.cl_stub2_r

lib.rstubs: o_rl.rl_stub_r o.k_stub2_r o.cl_stub2_r
        ${LIBFILE} ${LIBFLAGS} $@ o_rl.rl_stub_r o.k_stub2_r o.cl_stub2_r

lib.romcstubs: rm_o.cl_stub_rm rm_o.k_stub2_rm rm_o.cl_stub2_rm
        ${LIBFILE} ${LIBFLAGS} $@ rm_o.cl_stub_rm rm_o.k_stub2_rm rm_o.cl_stub2_rm

lib.romstubs: rm_o_rl.rl_stub_rm rm_o.k_stub2_rm rm_o.cl_stub2_rm
        ${LIBFILE} ${LIBFLAGS} $@ rm_o_rl.rl_stub_rm rm_o.k_stub2_rm rm_o.cl_stub2_rm

lib.romastubs: rlib.s.rl_stub_a
        ${OBJASM} ${AFLAGS} -from rlib.s.rl_stub_a -to $@

# Non-exported libraries:
lib.clib: ${CLIB_MOD_OBJS}
        ${LD} ${LDFLAGS} -o $@ -module ${CLIB_MOD_OBJS}
        ${MODSQZ} $@

# Derived headers:
derived.swis: s.makehswis h.swisheader s.swioptions
        ${AS} ${AFLAGS} -from s.makehswis -to $@
        settype $@ text

s.swioptions:
        @SWIOptions

#
# Rules for exporting:
#
${EXP_HDR}.SharedCLib: hdr.SharedCLib
        ${CP} hdr.SharedCLib $@ ${CPFLAGS}

# The swis.h header is only copied if the file differs from an existing
# exported header.  This prevents needless recompilations
CLIB:h.swis: derived.swis
        -${DIFF} $@ derived.swis
        If "<AMU$ReturnCode>" <> "0" Then ${CP} derived.swis $@ ${CPFLAGS}

# Headers:
CLIB:h.assert: clib.h.assert  ;   ${CP} clib.h.assert $@ ${CPFLAGS}
CLIB:h.ctype: clib.h.ctype;       ${CP} clib.h.ctype $@ ${CPFLAGS}
CLIB:h.errno: clib.h.errno;       ${CP} clib.h.errno $@ ${CPFLAGS}
CLIB:h.fenv: clib.h.fenv;         ${CP} clib.h.fenv $@ ${CPFLAGS}
CLIB:h.float: clib.h.float;       ${CP} clib.h.float $@ ${CPFLAGS}
CLIB:h.inttypes: clib.h.inttypes; ${CP} clib.h.inttypes $@ ${CPFLAGS}
CLIB:h.iso646: clib.h.iso646;     ${CP} clib.h.iso646 $@ ${CPFLAGS}
CLIB:h.kernel: clib.h.kernel;     ${CP} clib.h.kernel $@ ${CPFLAGS}
CLIB:h.limits: clib.h.limits;     ${CP} clib.h.limits $@ ${CPFLAGS}
CLIB:h.locale: clib.h.locale;     ${CP} clib.h.locale $@ ${CPFLAGS}
CLIB:h.math: clib.h.math;         ${CP} clib.h.math $@ ${CPFLAGS}
CLIB:h.setjmp: clib.h.setjmp;     ${CP} clib.h.setjmp $@ ${CPFLAGS}
CLIB:h.signal: clib.h.signal;     ${CP} clib.h.signal $@ ${CPFLAGS}
CLIB:h.stdarg: clib.h.stdarg;     ${CP} clib.h.stdarg $@ ${CPFLAGS}
CLIB:h.stdbool: clib.h.stdbool;   ${CP} clib.h.stdbool $@ ${CPFLAGS}
CLIB:h.stddef: clib.h.stddef;     ${CP} clib.h.stddef $@ ${CPFLAGS}
CLIB:h.stdint: clib.h.stdint;     ${CP} clib.h.stdint $@ ${CPFLAGS}
CLIB:h.stdio: clib.h.stdio;       ${CP} clib.h.stdio $@ ${CPFLAGS}
CLIB:h.stdlib: clib.h.stdlib;     ${CP} clib.h.stdlib $@ ${CPFLAGS}
CLIB:h.string: clib.h.string;     ${CP} clib.h.string $@ ${CPFLAGS}
CLIB:h.time: clib.h.time;         ${CP} clib.h.time $@ ${CPFLAGS}
CLIB:h.varargs: clib.h.varargs;   ${CP} clib.h.varargs $@ ${CPFLAGS}


RISC_OSLib:h.akbd: rlib.h.akbd;              ${CP} rlib.h.akbd $@ ${CPFLAGS}
RISC_OSLib:h.alarm: rlib.h.alarm;            ${CP} rlib.h.alarm $@ ${CPFLAGS}
RISC_OSLib:h.baricon: rlib.h.baricon;        ${CP} rlib.h.baricon $@ ${CPFLAGS}
RISC_OSLib:h.bbc: rlib.h.bbc;                ${CP} rlib.h.bbc $@ ${CPFLAGS}
RISC_OSLib:h.colourmenu: rlib.h.colourmenu;  ${CP} rlib.h.colourmenu $@ ${CPFLAGS}
RISC_OSLib:h.colourpick: rlib.h.colourpick;  ${CP} rlib.h.colourpick $@ ${CPFLAGS}
RISC_OSLib:h.colourtran: rlib.h.colourtran;  ${CP} rlib.h.colourtran $@ ${CPFLAGS}
RISC_OSLib:h.coords: rlib.h.coords;          ${CP} rlib.h.coords $@ ${CPFLAGS}
RISC_OSLib:h.ctl: rlib.h.ctl;                ${CP} rlib.h.ctl $@ ${CPFLAGS}
RISC_OSLib:h.dbox: rlib.h.dbox;              ${CP} rlib.h.dbox $@ ${CPFLAGS}
RISC_OSLib:h.dboxfile: rlib.h.dboxfile;      ${CP} rlib.h.dboxfile $@ ${CPFLAGS}
RISC_OSLib:h.dboxquery: rlib.h.dboxquery;    ${CP} rlib.h.dboxquery $@ ${CPFLAGS}
RISC_OSLib:h.dboxtcol: rlib.h.dboxtcol;      ${CP} rlib.h.dboxtcol $@ ${CPFLAGS}
RISC_OSLib:h.dragasprit: rlib.h.dragasprit;  ${CP} rlib.h.dragasprit $@ ${CPFLAGS}
RISC_OSLib:h.drawfdiag: rlib.h.drawfdiag;    ${CP} rlib.h.drawfdiag $@ ${CPFLAGS}
RISC_OSLib:h.drawferror: rlib.h.drawferror;  ${CP} rlib.h.drawferror $@ ${CPFLAGS}
RISC_OSLib:h.drawfobj: rlib.h.drawfobj;      ${CP} rlib.h.drawfobj $@ ${CPFLAGS}
RISC_OSLib:h.drawftypes: rlib.h.drawftypes;  ${CP} rlib.h.drawftypes $@ ${CPFLAGS}
RISC_OSLib:h.drawmod: rlib.h.drawmod;        ${CP} rlib.h.drawmod $@ ${CPFLAGS}
RISC_OSLib:h.event: rlib.h.event;            ${CP} rlib.h.event $@ ${CPFLAGS}
RISC_OSLib:h.fileicon: rlib.h.fileicon;      ${CP} rlib.h.fileicon $@ ${CPFLAGS}
RISC_OSLib:h.flex: rlib.h.flex;              ${CP} rlib.h.flex $@ ${CPFLAGS}
RISC_OSLib:h.font: rlib.h.font;              ${CP} rlib.h.font $@ ${CPFLAGS}
RISC_OSLib:h.fontlist: rlib.h.fontlist;      ${CP} rlib.h.fontlist $@ ${CPFLAGS}
RISC_OSLib:h.fontselect: rlib.h.fontselect;  ${CP} rlib.h.fontselect $@ ${CPFLAGS}
RISC_OSLib:h.heap: rlib.h.heap;              ${CP} rlib.h.heap $@ ${CPFLAGS}
RISC_OSLib:h.help: rlib.h.help;              ${CP} rlib.h.help $@ ${CPFLAGS}
RISC_OSLib:h.magnify: rlib.h.magnify;        ${CP} rlib.h.magnify $@ ${CPFLAGS}
RISC_OSLib:h.menu: rlib.h.menu;              ${CP} rlib.h.menu $@ ${CPFLAGS}
RISC_OSLib:h.msgs: rlib.h.msgs;              ${CP} rlib.h.msgs $@ ${CPFLAGS}
RISC_OSLib:h.msgtrans: rlib.h.msgtrans;      ${CP} rlib.h.msgtrans $@ ${CPFLAGS}
RISC_OSLib:h.os: rlib.h.os;                  ${CP} rlib.h.os $@ ${CPFLAGS}
RISC_OSLib:h.pointer: rlib.h.pointer;        ${CP} rlib.h.pointer $@ ${CPFLAGS}
RISC_OSLib:h.print: rlib.h.print;            ${CP} rlib.h.print $@ ${CPFLAGS}
RISC_OSLib:h.res: rlib.h.res;                ${CP} rlib.h.res $@ ${CPFLAGS}
RISC_OSLib:h.resspr: rlib.h.resspr;          ${CP} rlib.h.resspr $@ ${CPFLAGS}
RISC_OSLib:h.saveas: rlib.h.saveas;          ${CP} rlib.h.saveas $@ ${CPFLAGS}
RISC_OSLib:h.sprite: rlib.h.sprite;          ${CP} rlib.h.sprite $@ ${CPFLAGS}
RISC_OSLib:h.template: rlib.h.template;      ${CP} rlib.h.template $@ ${CPFLAGS}
RISC_OSLib:h.trace: rlib.h.trace;            ${CP} rlib.h.trace $@ ${CPFLAGS}
RISC_OSLib:h.txt: rlib.h.txt;                ${CP} rlib.h.txt $@ ${CPFLAGS}
RISC_OSLib:h.txtedit: rlib.h.txtedit;        ${CP} rlib.h.txtedit $@ ${CPFLAGS}
RISC_OSLib:h.txtopt: rlib.h.txtopt;          ${CP} rlib.h.txtopt $@ ${CPFLAGS}
RISC_OSLib:h.txtscrap: rlib.h.txtscrap;      ${CP} rlib.h.txtscrap $@ ${CPFLAGS}
RISC_OSLib:h.txtwin: rlib.h.txtwin;          ${CP} rlib.h.txtwin $@ ${CPFLAGS}
RISC_OSLib:h.typdat: rlib.h.typdat;          ${CP} rlib.h.typdat $@ ${CPFLAGS}
RISC_OSLib:h.visdelay: rlib.h.visdelay;      ${CP} rlib.h.visdelay $@ ${CPFLAGS}
RISC_OSLib:h.werr: rlib.h.werr;              ${CP} rlib.h.werr $@ ${CPFLAGS}
RISC_OSLib:h.wimp: rlib.h.wimp;              ${CP} rlib.h.wimp $@ ${CPFLAGS}
RISC_OSLib:h.wimpt: rlib.h.wimpt;            ${CP} rlib.h.wimpt $@ ${CPFLAGS}
RISC_OSLib:h.win: rlib.h.win;                ${CP} rlib.h.win $@ ${CPFLAGS}
RISC_OSLib:h.xferrecv: rlib.h.xferrecv;      ${CP} rlib.h.xferrecv $@ ${CPFLAGS}
RISC_OSLib:h.xfersend: rlib.h.xfersend;      ${CP} rlib.h.xfersend $@ ${CPFLAGS}

RISC_OSLib:h.EditIntern.message: rlib.h.EditIntern.message;       ${CP} rlib.h.EditIntern.message $@ ${CPFLAGS}
RISC_OSLib:h.EditIntern.txtar: rlib.h.EditIntern.txtar;           ${CP} rlib.h.EditIntern.txtar $@ ${CPFLAGS}
RISC_OSLib:h.EditIntern.txtfile: rlib.h.EditIntern.txtfile;       ${CP} rlib.h.EditIntern.txtfile $@ ${CPFLAGS}
RISC_OSLib:h.EditIntern.txtoptmenu: rlib.h.EditIntern.txtoptmenu; ${CP} rlib.h.EditIntern.txtoptmenu $@ ${CPFLAGS}

# Libraries
Clib:o.ansilib:          lib.ansilib;        ${CP} lib.ansilib    $@ ${CPFLAGS}
CLib:o.stubs:            lib.stubs;          ${CP} lib.stubs $@ ${CPFLAGS}
RISC_OSLib:o.risc_oslib: lib.risc_oslib;     ${CP} lib.risc_oslib $@ ${CPFLAGS}
RISC_OSLib:o.riscoslibm: lib.riscoslibm;     ${CP} lib.riscoslibm $@ ${CPFLAGS}
RISC_OSLib:o.rstubs:     lib.rstubs;         ${CP} lib.rstubs $@ ${CPFLAGS}
RISC_OSLib:o.romstubs:   lib.romstubs;       ${CP} lib.romstubs $@ ${CPFLAGS}
RISC_OSLib:o.romastubs:  lib.romastubs;      ${CP} lib.romastubs $@ ${CPFLAGS}
RISC_OSLib:o.romcstubs:  lib.romcstubs;      ${CP} lib.romcstubs $@ ${CPFLAGS}

# ModuleWrap
RISC_OSLib:s.modulewrap: s.modulewrap;       ${CP} s.modulewrap $@ ${CPFLAGS}

#
# Objects whose name isn't the same as their source
#
o.clib: clib.s.cl_obj_r
        ${OBJASM} ${AFLAGS} ${ALFLAGS} -from clib.s.cl_obj_r -to $@

o.kernel: kernel.s.k_obj_r
        ${OBJASM} ${AFLAGS} ${ALFLAGS} -from kernel.s.k_obj_r -to $@

rm_o.bsearch_a: c.bsearch
        ${CC} ${CFLAGS} -ffah -zm1 ${CSFLAGS} -zkA ${OPTIONS} -o $@ c.bsearch

rm_o.clib:  clib.s.cl_obj_r
        ${OBJASM} ${AFLAGS} ${ASFLAGS} ${OPTIONS} -from clib.s.cl_obj_r -to $@

rm_o.cl_modbody: clib.s.cl_mod_r
        ${OBJASM} ${AFLAGS} ${ASFLAGS} ${OPTIONS} ${UROM_ROM_DEFS} -from clib.s.cl_mod_r -to $@

rm_o_rl.rl_modbody: rlib.s.rl_mod_r
        ${OBJASM} ${AFLAGS} ${ASFLAGS} ${OPTIONS} -from rlib.s.rl_mod_r -to $@

rm_o.k_modbody: kernel.s.k_mod_r
        ${OBJASM} ${AFLAGS} ${ASFLAGS} ${OPTIONS} -from kernel.s.k_mod_r -to $@

rm_o.memcpy: s.memcpy s.h_regs
        ${OBJASM} ${AFLAGS} ${ASFLAGS} ${OPTIONS} -from s.memcpy -to $@

o.initmodule: clib.s.initmod_r
        ${OBJASM} ${AFLAGS} ${ASFLAGS} ${OPTIONS} -from clib.s.initmod_r -to $@

rm_o.initmodule: clib.s.initmod_rm
        ${OBJASM} ${AFLAGS} ${ASFLAGS} ${OPTIONS} -from clib.s.initmod_rm -to $@

rm_o_rl.initmodule: rlib.s.initmod_r
        ${OBJASM} ${AFLAGS} ${ASFLAGS} ${OPTIONS} -from rlib.s.initmod_r -to $@

#
# Objects that require extra flags
#
o.armsys: c.armsys
        ${CC} ${CFLAGS} -fah ${CLFLAGS} ${VLFLAGS} -o $@ c.armsys

o.armprof: c.armprof
        ${CC} ${CFLAGS} -fah ${CLFLAGS} ${VLFLAGS} -pcc -o $@ c.armprof

o.string: c.string
        ${CC} ${CFLAGS} -fah ${CLFLAGS} -DSEPARATE_MEMCPY -DSEPARATE_MEMSET -o $@ c.string

rm_o.armsys: c.armsys
        ${CC} ${CFLAGS} -ffah -zm1 ${CSFLAGS} ${OPTIONS} ${VSFLAGS} -o $@ c.armsys

rm_o.string: c.string
        ${CC} ${CFLAGS} -ffah -zm1 ${CSFLAGS} ${OPTIONS} -DSEPARATE_MEMCPY -DSEPARATE_MEMSET -o $@ c.string

#
# Special case: use "rom_defs" for ROM library (o_rm), "defs" for the others
# Note: New Assembler - could use -PreDefine "UROM SETL {FALSE}"

o_rl.swi: rlib.s.swi
        ${OBJASM} ${UROM_DEFS} rlib.s.swi o_rl.swi ${AFLAGS}

o_rl.poll: rlib.s.poll
        ${OBJASM} ${UROM_DEFS} rlib.s.poll o_rl.poll ${AFLAGS}

m_o_rl.swi: rlib.s.swi
        ${OBJASM} ${UROM_DEFS} rlib.s.swi m_o_rl.swi ${AFLAGS}

m_o_rl.poll: rlib.s.poll
        ${OBJASM} ${UROM_DEFS} rlib.s.poll m_o_rl.poll ${AFLAGS}

rm_o_rl.swi: rlib.s.swi
        ${OBJASM} ${UROM_ROM_DEFS} rlib.s.swi rm_o_rl.swi ${AFLAGS} ${OPTIONS}

rm_o_rl.poll: rlib.s.poll
        ${OBJASM} ${UROM_ROM_DEFS} rlib.s.poll rm_o_rl.poll ${AFLAGS} ${OPTIONS}

#
# trace separated out to allow defining of TRACE
#
o_rl.trace: rlib.c.trace
   ${CC} ${CFLAGS} ${DFLAGS} -DTRACE -o o_rl.trace rlib.c.trace

m_o_rl.trace: rlib.c.trace
   ${CC} ${CFLAGS} -ffah ${DFLAGS} -zM -DTRACE -o m_o_rl.trace rlib.c.trace

rm_o_rl.trace: rlib.c.trace
   ${CC} ${CFLAGS} -ffah ${DFLAGS} -zm1 ${CSFLAGS} ${STK_EXTN} ${OPTIONS} -DTRACE -o rm_o_rl.trace rlib.c.trace

# Dynamic dependencies:
