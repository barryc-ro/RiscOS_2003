# Makefile for Paint
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 25-Oct-94  AMcC   Updated for Black build
# 28-Oct-94  AMcC   added -zps1 flag to .c.oz rule (module version)
#

#
# Program specific options:
#
COMPONENT  = Paint
APP        = !Paint
MODULE     = rm.!Paint
ROM_MODULE = aof.Paint
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}
RESAPP = <resource$dir>.Apps.${APP}

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
SQUEEZE = squeeze
WIPE    = -wipe

AFLAGS  = ${THROWBACK} -depend !Depend -nocache -stamp -quit
CFLAGS  = ${THROWBACK} -c -depend !Depend -fah ${INCLUDES} -wp
CPFLAGS = ~cfr~v
SQFLAGS = 
WFLAGS  = ~c~v

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISC_OSLib:o.risc_oslib
ROMSTUBS  = RISC_OSLib:o.romstubs
ABSSYM    = RISC_OSLib:o.abssym
WRAPPER   = RISC_OSLib:s.ModuleWrap

#
# Include files
#
INCLUDES = -IC:

FILES = \
 ${RDIR}.!Boot\
 ${LDIR}.!Help\
 ${LDIR}.!Run\
 ${LDIR}.Messages\
 ${LDIR}.Sprites\
 ${LDIR}.Templates\
 abs.!RunImage

FILESD = \
 ${RDIR}.!Boot\
 ${LDIR}.!Help\
 ${LDIR}.!Run\
 ${LDIR}.Messages\
 ${LDIR}.Sprites\
 ${LDIR}.Templates\
 abs.!RunImageD

# Include these in the ROM module
RESFILES =

DFILES = \
 debug.${APP}.!Boot\
 debug.${APP}.!Help\
 debug.${APP}.!Run\
 debug.${APP}.!RunImage\
 debug.${APP}.Messages\
 debug.${APP}.Sprites\
 debug.${APP}.Templates

OBJS      = o.colours o.main o.menus o.psprite o.sprwindow o.tools o.toolwindow \
            o.jpeg o.writepixel
OBJSZ     = oz.colours oz.main oz.menus oz.psprite oz.sprwindow\
            oz.tools oz.toolwindow o.jpeg o.writepixel
OBJSD     = od.colours od.ftrace od.guard od.main od.menus\
            od.psprite od.sprwindow od.tools od.toolwindow o.jpeg o.m  o.writepixel
ASMS      = s.colours s.main s.menus s.psprite s.sprwindow s.tools s.toolwindow
INCS      = i.colours i.main i.menus i.psprite i.sprwindow i.tools i.toolwindow

#
# Rule patterns
#
.SUFFIXES: .oz .od .i .asm
.c.o:;      ${CC} ${CFLAGS} -ff -o $@ $<
.c.oz:;     ${CC} ${CFLAGS} -ff -zM -zps1 -o $@ $<
.c.od:;     ${CC} ${CFLAGS} -DTRACE=1 -o $@ $<
.asm.o:;    ${AS} ${AFLAGS} $< $@
.c.i:;      ${CC} -E ${CFLAGS} $< > $@
.c.s:;      ${CC} ${CFLAGS} -s $<

#
# Main rules:
#
# Application
#
all: ${FILES}
        @echo ${COMPONENT}: Application built (Disc)

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install: ${FILES}
        ${CP} ${RDIR}.!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} ${LDIR}.!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} ${LDIR}.!Run       ${INSTDIR}.!Run       ${CPFLAGS}
        ${CP} abs.!RunImage      ${INSTDIR}.!RunImage  ${CPFLAGS}
        ${CP} ${LDIR}.Messages   ${INSTDIR}.Messages   ${CPFLAGS}
        ${CP} ${LDIR}.Sprites    ${INSTDIR}.Sprites    ${CPFLAGS}
        ${CP} ${LDIR}.Templates  ${INSTDIR}.Templates  ${CPFLAGS}
        Access ${INSTDIR}.!Boot      lr/r
        Access ${INSTDIR}.!Help      lr/r
        Access ${INSTDIR}.!Run       lr/r
        Access ${INSTDIR}.!RunImage  lr/r
        Access ${INSTDIR}.Messages   lr/r
        Access ${INSTDIR}.Sprites    lr/r
        Access ${INSTDIR}.Templates  lr/r
        @echo ${COMPONENT}: Application installed (Disc)

installd: ${FILESD}
        ${CP} ${RDIR}.!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} ${LDIR}.!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} ${LDIR}.!Run       ${INSTDIR}.!Run       ${CPFLAGS}
        ${CP} abs.!RunImageD     ${INSTDIR}.!RunImage  ${CPFLAGS}
        ${CP} ${LDIR}.Messages   ${INSTDIR}.Messages   ${CPFLAGS}
        ${CP} ${LDIR}.Sprites    ${INSTDIR}.Sprites    ${CPFLAGS}
        ${CP} ${LDIR}.Templates  ${INSTDIR}.Templates  ${CPFLAGS}
        Access ${INSTDIR}.!Boot      lr/r
        Access ${INSTDIR}.!Help      lr/r
        Access ${INSTDIR}.!Run       lr/r
        Access ${INSTDIR}.!RunImage  lr/r
        Access ${INSTDIR}.Messages   lr/r
        Access ${INSTDIR}.Sprites    lr/r
        Access ${INSTDIR}.Templates  lr/r
        @echo ${COMPONENT}: Debug application installed (Disc)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources:
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESAPP}
        ${CP} ${RDIR}.!Boot     ${RESAPP}.!Boot  ${CPFLAGS}
        ${CP} ${LDIR}.!Help     ${RESAPP}.!Help  ${CPFLAGS}
        ${CP} ${RDIR}.ROM.!Run  ${RESAPP}.!Run   ${CPFLAGS}
        |
        ${CP} ${LDIR}.Messages  ${RESDIR}.Messages  ${CPFLAGS}
        ${CP} ${LDIR}.Sprites   ${RESDIR}.Sprites   ${CPFLAGS}
        ${CP} ${LDIR}.Templates ${RESDIR}.Templates ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} oz.* ${WFLAGS}
        ${WIPE} od.* ${WFLAGS}
        ${WIPE} s.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} abs.!RunImage
        ${RM} abs.!RunImageD
        ${RM} ${ROM_MODULE}
        ${RM} ${MODULE}
        @echo ${COMPONENT}: cleaned

#
# Development rules:
# 
app: ${FILES}
        @echo Made application ${APP}

appd: ${DFILES}
        @echo Made debug.${APP}

mod: ${MODULE}
        @echo ${COMPONENT}: Module built (RAM)

asms: ${ASMS}

incs: ${INCS}

clean_all:
        Access ${INSTDIR}.!Boot      rw/rw
        Access ${INSTDIR}.!Help      rw/rw
        Access ${INSTDIR}.!Run       rw/rw
        Access ${INSTDIR}.!RunImage  rw/rw
        Access ${INSTDIR}.Messages   rw/rw
        Access ${INSTDIR}.Sprites    rw/rw
        Access ${INSTDIR}.Templates  rw/rw
        Destroy ${OBJS} ${OBJSZ} ${OBJSD} o.ModuleWrap \
        ${APP}.!RunImage \
        debug.${APP}.!RunImage \
        ${ROM_MODULE} ${MODULE} \
        ${ASMS} s.AppName s.ModuleWrap s.ResFiles \
        ${DFILES} ${INSTDIR}.!Boot ${INSTDIR}.!Help ${INSTDIR}.!Run \
        ${INSTDIR}.!RunImage ${INSTDIR}.Messages ${INSTDIR}.Sprites \
        ${INSTDIR}.Templates

#
# Static dependencies:
#
abs.!RunImage: ${OBJS} ${RLIB} ${CLIB}
        ${LD} -o $@ ${OBJS} ${RLIB} ${CLIB}
        ${SQUEEZE} ${SQFLAGS} $@

${MODULE}: oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB}
        ${LD} -o $@ -module oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSZ} ${ROMSTUBS}
        ${LD} -o $@ -aof oz.ModuleWrap ${OBJSZ} ${ROMSTUBS}

# final link for ROM Image (using given base address)
rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM} \
              -map > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

oz.ModuleWrap: s.ModuleWrap ${RESFILES} s.AppName s.Resfiles
        ${AS} ${AFLAGS} s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
        ${CP} ${WRAPPER} $@ ${CPFLAGS}

s.AppName: ${LDIR}.Messages
        awk -f awk.AppName ${LDIR}.Messages > $@

s.ResFiles: asm.ResFiles
        ${CP} asm.ResFiles $@ ${CPFLAGS}

#---------------------------------------------------------------------------
# debugging version:
#
abs.!RunImageD: ${OBJSD} ${RLIB} ${CLIB}
        ${LD} -o $@ ${OBJSD} ${RLIB} ${CLIB}
        SetType $@ Absolute

debug.${APP}.!Boot: ${RDIR}.!Boot
        ${CP} ${RDIR}.!Boot $@ ${CPFLAGS}

debug.${APP}.!Help: ${LDIR}.!Help
        ${CP} ${LDIR}.!Help $@ ${CPFLAGS}

debug.${APP}.!Run: ${LDIR}.!Run
        ${CP} ${LDIR}.!Run $@ ${CPFLAGS}

debug.${APP}.Messages: ${LDIR}.Messages
        ${CP} ${LDIR}.Messages $@ ${CPFLAGS}

debug.${APP}.Sprites: ${LDIR}.Sprites
        ${CP} ${LDIR}.Sprites $@ ${CPFLAGS}

debug.${APP}.Templates: ${LDIR}.Templates
        ${CP} ${LDIR}.Templates $@ ${CPFLAGS}

#---------------------------------------------------------------------------
# Dynamic dependencies:
