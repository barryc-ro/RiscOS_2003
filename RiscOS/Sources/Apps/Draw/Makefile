# Makefile for Draw
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
COMPONENT  = Draw
APP        = !Draw
MODULE     = rm.!Draw
ROM_MODULE = aof.Draw
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
CFLAGS  = ${THROWBACK} -depend !Depend -fah ${INCLUDES} -wp
CPFLAGS = ~cfr~v
SQFLAGS = -f
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLib:o.stubs
RLIB      = RISC_OSLib:o.risc_oslib
ROMSTUBS  = RISC_OSLib:o.romstubs
ABSSYM    = RISC_OSLib:o.abssym
WRAPPER   = RISC_OSLib:s.ModuleWrap

LIBSD     = ${RLIB} ${CLIB}

#
# Include files
#
INCLUDES  = -IC:

FILES =\
 ${RDIR}.!Boot\
 ${LDIR}.!Help\
 ${LDIR}.!Run\
 $(RDIR).!Sprites\
 ${LDIR}.Messages\
 $(RDIR).Sprites\
 ${LDIR}.Templates\
 abs.!RunImage

FILESD =\
 ${RDIR}.!Boot\
 ${LDIR}.!Help\
 ${LDIR}.!Run\
 $(RDIR).!Sprites\
 ${LDIR}.Messages\
 $(RDIR).Sprites\
 ${LDIR}.Templates\
 abs.!RunImageD

# Include these in the ROM module
RESFILES =\
 ${RDIR}.Sprites

DFILES =\
 Debug.${APP}.!Boot\
 Debug.${APP}.!Help\
 Debug.${APP}.!Run \
 Debug.${APP}.!RunImage\
 Debug.${APP}.!Sprites\
 Debug.${APP}.Messages\
 Debug.${APP}.Sprites\
 Debug.${APP}.Templates

OBJS =\
 o.Draw o.DrawAction o.DrawCheck o.DrawDispl o.DrawDXF\
 o.DrawEdit o.DrawEnter o.DrawFileIO o.DrawGrid o.DrawHelp\
 o.DrawMenu o.DrawObject o.DrawPrint o.DrawScan o.DrawSelect\
 o.DrawTextC o.DrawTrans o.DrawUndo o.bezierarc o.jpeg

OBJSZ =\
 oz.Draw oz.DrawAction oz.DrawCheck oz.DrawDispl oz.DrawDXF\
 oz.DrawEdit oz.DrawEnter oz.DrawFileIO oz.DrawGrid oz.DrawHelp\
 oz.DrawMenu oz.DrawObject oz.DrawPrint oz.DrawScan oz.DrawSelect\
 oz.DrawTextC oz.DrawTrans oz.DrawUndo oz.bezierarc o.jpeg

OBJSD =\
 od.Draw od.DrawAction od.DrawCheck od.DrawDispl\
 od.DrawDXF od.DrawEdit od.DrawEnter od.DrawFileIO od.DrawGrid\
 od.DrawHelp od.DrawMenu od.DrawObject od.DrawPrint od.DrawScan\
 od.DrawSelect od.DrawTextC od.DrawTrans od.DrawUndo\
 od.bezierarc od.ftrace od.guard od.flex o.jpeg

ASMS =\
 s.Draw s.DrawAction s.DrawCheck s.DrawDispl s.DrawDXF\
 s.DrawEdit s.DrawEnter s.DrawFileIO s.DrawGrid s.DrawHelp\
 s.DrawMenu s.DrawObject s.DrawPrint s.DrawScan s.DrawSelect\
 s.DrawTextC s.DrawTrans s.DrawUndo s.bezierarc

#
# Rule patterns
#
.SUFFIXES: .oz .od .asm
.c.o:;    ${CC} ${CFLAGS} -c -ff $<
.c.oz:;   ${CC} ${CFLAGS} -c -ff -zM -zps1 -o $@ $<
.c.od:;   ${CC} ${CFLAGS} -c -DTRACE=1 -g -o $@ $<
.asm.o:;  ${AS} ${AFLAGS} $< $@
.c.s:;    ${CC} ${CFLAGS} -s -ff $<

#
# Main rules:
#
all: ${FILES}
        @echo ${COMPONENT}: Application built (Disc)

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install: ${FILES}
        $(CP) $(RDIR).!Boot     ${INSTDIR}.!Boot     ${CPFLAGS}
        $(CP) $(LDIR).!Help     ${INSTDIR}.!Help     ${CPFLAGS}
        $(CP) $(LDIR).!Run      ${INSTDIR}.!Run      ${CPFLAGS}
        $(CP) abs.!RunImage     ${INSTDIR}.!RunImage ${CPFLAGS}
        $(CP) $(RDIR).!Sprites  ${INSTDIR}.!Sprites  ${CPFLAGS}
        $(CP) $(LDIR).Messages  ${INSTDIR}.Messages  ${CPFLAGS}
        $(CP) $(RDIR).Sprites   ${INSTDIR}.Sprites   ${CPFLAGS}
        $(CP) $(LDIR).Templates ${INSTDIR}.Templates ${CPFLAGS}
        -Access ${INSTDIR}.!Boot     lr/r
        -Access ${INSTDIR}.!Help     lr/r
        -Access ${INSTDIR}.!Run      lr/r
        -Access ${INSTDIR}.!RunImage lr/r
        -Access ${INSTDIR}.!Sprites  lr/r
        -Access ${INSTDIR}.Messages  lr/r
        -Access ${INSTDIR}.Sprites   lr/r
        -Access ${INSTDIR}.Templates lr/r
        @echo ${COMPONENT}: Application installed (Disc)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

installd: ${FILESD}
        $(CP) $(RDIR).!Boot     ${INSTDIR}.!Boot     ${CPFLAGS}ln
        $(CP) $(LDIR).!Help     ${INSTDIR}.!Help     ${CPFLAGS}ln
        $(CP) $(LDIR).!Run      ${INSTDIR}.!Run      ${CPFLAGS}ln
        $(CP) abs.!RunImageD    ${INSTDIR}.!RunImage ${CPFLAGS}ln
        $(CP) $(RDIR).!Sprites  ${INSTDIR}.!Sprites  ${CPFLAGS}ln
        $(CP) $(LDIR).Messages  ${INSTDIR}.Messages  ${CPFLAGS}ln
        $(CP) $(RDIR).Sprites   ${INSTDIR}.Sprites   ${CPFLAGS}ln
        $(CP) $(LDIR).Templates ${INSTDIR}.Templates ${CPFLAGS}ln
        -Access ${INSTDIR}.!Boot     lr/r
        -Access ${INSTDIR}.!Help     lr/r
        -Access ${INSTDIR}.!Run      lr/r
        -Access ${INSTDIR}.!RunImage lr/r
        -Access ${INSTDIR}.!Sprites  lr/r
        -Access ${INSTDIR}.Messages  lr/r
        -Access ${INSTDIR}.Sprites   lr/r
        -Access ${INSTDIR}.Templates lr/r
        @echo ${COMPONENT}: Application installed (Disc)

resources:
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESAPP}
        ${CP} ${RDIR}.ROM.!Boot ${RESAPP}.!Boot ${CPFLAGS}
        ${CP} ${LDIR}.!Help     ${RESAPP}.!Help ${CPFLAGS}
        ${CP} ${RDIR}.ROM.!Run  ${RESAPP}.!Run  ${CPFLAGS}
        |
        ${CP} ${LDIR}.Messages  ${RESDIR}.Messages ${CPFLAGS}
        ${CP} ${LDIR}.Templates ${RESDIR}.Templates ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} s.* ${WFLAGS}
        ${WIPE} oz.* ${WFLAGS}
        ${WIPE} od.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} abs.!RunImage
        ${RM} ${ROM_MODULE}
        ${RM} ${MODULE}
        @echo ${COMPONENT}: cleaned

#
# Development rules:
# 
app: ${FILES}
        @echo Made application ${APP}

appd: ${DFILES}
        @echo Made Debug.${APP}

mod: ${MODULE}
        @echo ${COMPONENT}: Module built (RAM)

asms: ${ASMS}

linkmap: ${OBJS} ${LIBS}
   ${LD} -map -o null:x ${OBJS} ${LIBS} > map.linked

map:
   ${LD} -map -bin -base 0 -o null: ${OBJS} ${LIBS} > map.base0

clean_all:
   -Access ${INSTDIR}.!Boot     rw/rw
   -Access ${INSTDIR}.!Help     rw/rw
   -Access ${INSTDIR}.!Run      rw/rw
   -Access ${INSTDIR}.!RunImage rw/rw
   -Access ${INSTDIR}.!Sprites  rw/rw
   -Access ${INSTDIR}.Messages  rw/rw
   -Access ${INSTDIR}.Sprites   rw/rw
   -Access ${INSTDIR}.Templates rw/rw
   Destroy ${OBJS} ${OBJSD} ${ASMS} ${ROM_MODULE} ${MODULE} abs.!RunImage \
         s.AppName s.ModuleWrap s.ResFiles \
         Debug.RM.!Draw ${INSTDIR}.!Boot ${INSTDIR}.!Help \
         ${INSTDIR}.!Run ${INSTDIR}.!RunImage ${INSTDIR}.!Sprites \
         ${INSTDIR}.Messages ${INSTDIR}.Sprites ${INSTDIR}.Templates
#
# Static dependencies:
#
abs.!RunImage: ${OBJS} ${RLIB} ${CLIB}
        ${LD} -o $@ ${OBJS} ${RLIB} ${CLIB}
        ${SQUEEZE} ${SQFLAGS} $@

abs.!RunImageD: ${OBJSD} ${RLIB} ${CLIB}
        ${LD} -debug -o $@ ${OBJSD} ${RLIB} ${CLIB}

${MODULE}: oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB}
        ${LD} -o $@ -module oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSZ} ${ROMSTUBS}
        ${LD} -o $@ -aof oz.ModuleWrap ${OBJSZ} ${ROMSTUBS}

# final link for ROM Image (using given base address)
rom_link:
        ${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM} \
              -map > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

oz.ModuleWrap: s.ModuleWrap s.AppName ${RESFILES}
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
Debug.${APP}.!RunImage: ${OBJSD} ${LIBSD}
        ${LD} -o $@ ${OBJSD} ${LIBSD}
        SetType $@ Absolute
        Squeeze $@

Debug.${APP}.!Boot: $(RDIR).!Boot
        ${CP} $(RDIR).!Boot $@ ${CPFLAGS}

Debug.${APP}.!Help: $(LDIR).!Help
        ${CP} $(LDIR).!Help $@ ${CPFLAGS}

Debug.${APP}.!Run: $(LDIR).!Run
        ${CP} $(LDIR).!Run $@ ${CPFLAGS}

Debug.${APP}.!Sprites: $(RDIR).!Sprites
        ${CP} $(RDIR).!Sprites $@ ${CPFLAGS}

Debug.${APP}.Messages: $(LDIR).Messages
        ${CP} $(LDIR).Messages $@ ${CPFLAGS}

Debug.${APP}.Sprites: $(RDIR).Sprites
        ${CP} $(RDIR).Sprites $@ ${CPFLAGS}

Debug.${APP}.Templates: $(LDIR).Templates
        ${CP} $(LDIR).Templates $@ ${CPFLAGS}
#---------------------------------------------------------------------------
# Dynamic dependencies:
