# Makefile for DrawFile
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name  Description
# ----           ----  -----------
# 28th Nov 1994  JRC   Created.
# 11th Jan 1995  AMcC  Updated - diff rules for 3.10 and post 3.10

#
# Paths
#

RESDIR = <resource$dir>.Resources2

#
# Generic options:
#
CC     = cc -IOS: -fach -wp -DDRAWFILE
CP     = copy
OBJASM = ObjAsm ${THROWBACK} -depend !Depend -stamp -quit
CMHG   = CMHG
LINK   = Link
MKDIR  = cdir
RM     = remove
WIPE   = -wipe
ZM     = -zM

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries:
#
OSLIB  = OSLib:OSLib.o
RSTUBS = RISC_OSLib:o.romcstubs
ABSSYM = RISC_OSLib:o.abssym

#
# Program specific options:
#
COMPONENT = DrawFile
TARGET    = aof.DrawFile


OBJS = o.bbox o.callback o.declare o.header o.main \
      o.muldiv o.render o.riscos o.riscosa o.textarea o.trfm o.verify
OBJS_310 = \
      o.bbox o.callback o.declare o.files o.header310 oSA.main \
      o.muldiv o.render o.riscos o.textarea o.trfm o.verify

OBJSD = debug.bbox debug.callback debug.declare o.files o.header \
      o.hostfs debug.main o.muldiv debug.render \
      debug.riscos debug.textarea debug.trfm debug.verify debug.trace debug.m

ASMS = s.bbox s.declare s.main s.render s.textarea s.verify

INCS = i.bbox i.declare i.main i.render i.files i.textarea i.verify

RESOURCES = o.Messages
RESOURCES310 = o.Message310

H = h.drawfile #h.hostfs

#
# Generic rules {used by top-level build}:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} oSA.* ${WFLAGS}
        ${RM} linked.${COMPONENT}
        ${RM} map.${COMPONENT}
        ${RM} ${TARGET}
        ${WIPE} rm.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.Messages       ${RESDIR}.${COMPONENT}.Messages       ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${OSLIB} ${RSTUBS}
        ${LINK} -o $@ -aof ${OBJS} ${OSLIB} ${RSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LINK} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# Extra {development} rules:
#

all: RM.DrawFile 
   @Echo Made all

debug: RM.DrawFileD
   @Echo Made debug

clean_all:
   -Destroy ${OBJS} ${OBJSD} ${ASMS} ${INCS} RM.DrawFile RM.DrawFileD
   @Echo Made clean

asms: ${ASMS}
   @Echo Made asms

incs: ${INCS}
   @Echo Made incs

objs: ${OBJS} ${OBJSD}
   @Echo Made objs

map: ${OBJS} ${OSLIB} CLib:o.Stubs
   ${LINK} -map -output null: -s Symbols ${OBJS} ${OSLIB} CLib:o.Stubs > Map

release: RM.DrawFile
   %Copy RM.DrawFile Choices:Boot.PreDesk.DrawFile ~CFLNRV
   /<Prefix$Dir>.RM.DrawFile
   @Echo Made release

# Internal targets
RM.DrawFile: ${OBJS} ${OSLIB} CLib:o.Stubs
   ${LINK} -module -o $@ ${OBJS} ${OSLIB} CLib:o.Stubs

RM.DrawFileD: ${OBJSD} ${RESOURCES} ${OSLIB} CLib:o.Stubs
   ${LINK} -module -o $@ ${OBJSD} ${RESOURCES} ${OSLIB} CLib:o.Stubs

# For Aquarius
RM.310.DrawFile: ${OBJS_310} ${RESOURCES310} ${OSLIB} CLib:o.Stubs
   ${LINK} -module -o $@ ${OBJS_310} ${RESOURCES310} ${OSLIB} CLib:o.Stubs

# Static dependencies:

o.Message310: Resources.Message310
   ResGen Messages o.Message310 Resources.Message310 Resources.DrawFile.Messages

#General rules
.SUFFIXES:      .cmhg .h .c .debug .o .oSA .Resources .s .asm .i .def
.c.o:;          ${CC} ${THROWBACK} -depend !Depend -c ${ZM} -ff -o $@ $<
.c.oSA:;        ${CC} ${THROWBACK} -depend !Depend -c ${ZM} -ff -DSTANDALONE -o $@ $<
.c.debug:;      ${CC} ${THROWBACK} -depend !Depend -c ${ZM} -DTRACE=1 -o $@ $<
.c.i:;          ${CC} -E -C $< > $@
.c.s:;          ${CC} ${ZM} -S -ff $<
.cmhg.o:;       ${CMHG} $< $@
.Resources.o .Resources.debug:
                ResGen $* $@ $< Resources.DrawFile.$*
.asm.o:;        ${OBJASM} $< $@
.asm.debug:;    ${OBJASM} $< $@
#.def.h:;        DefMod -h < $< > $@
#.def.s:;        DefMod -s < $< > $@
#.def.o:;        DefMod -l -o l.$* < $<
#                LibFile -c -o $@ -via ViaFile
#                Wipe l.$* ~C~FR~V

# Dynamic dependencies:
o.bbox:	c.bbox
o.bbox:	OS:h.drawfile
o.bbox:	OS:h.types
o.bbox:	OS:h.os
o.bbox:	OS:h.osspriteop
o.bbox:	OS:h.draw
o.bbox:	OS:h.font
o.bbox:	OS:h.wimp
o.bbox:	OS:h.jpeg
o.bbox:	OS:h.os
o.bbox:	h.jc_trace
o.bbox:	h.trfm
o.bbox:	h.bbox
o.bbox:	h.main
o.bbox:	C:h.kernel
o.bbox:	OS:h.messagetrans
o.bbox:	h.callback
o.bbox:	h.verify
o.callback:	c.callback
o.callback:	OS:h.types
o.callback:	h.callback
o.callback:	OS:h.os
o.callback:	C:h.trace
o.callback:	h.m
o.callback:	h.realloc
o.callback:	C:h.trace
o.callback:	h.riscos
o.callback:	OS:h.territory
o.callback:	h.jc_trace
o.declare:	c.declare
o.declare:	OS:h.drawfile
o.declare:	OS:h.types
o.declare:	OS:h.os
o.declare:	OS:h.osspriteop
o.declare:	OS:h.draw
o.declare:	OS:h.font
o.declare:	OS:h.wimp
o.declare:	OS:h.jpeg
o.declare:	OS:h.os
o.declare:	OS:h.pdriver
o.declare:	OS:h.messagetrans
o.declare:	h.riscos
o.declare:	OS:h.territory
o.declare:	h.jc_trace
o.declare:	h.trfm
o.declare:	h.declare
o.declare:	h.callback
o.declare:	h.main
o.declare:	C:h.kernel
o.declare:	h.textarea
o.declare:	h.verify
o.main:	c.main
o.main:	C:h.kernel
o.main:	OS:h.messagetrans
o.main:	OS:h.types
o.main:	OS:h.os
o.main:	OS:h.macros
o.main:	OS:h.os
o.main:	OS:h.osfile
o.main:	OS:h.fileswitch
o.main:	OS:h.osmodule
o.main:	OS:h.resourcefs
o.main:	h.m
o.main:	h.realloc
o.main:	C:h.trace
o.main:	h.riscos
o.main:	OS:h.territory
o.main:	h.jc_trace
o.main:	h.bbox
o.main:	OS:h.drawfile
o.main:	OS:h.osspriteop
o.main:	OS:h.draw
o.main:	OS:h.font
o.main:	OS:h.wimp
o.main:	OS:h.jpeg
o.main:	h.declare
o.main:	h.callback
o.main:	h.main
o.main:	C:h.kernel
o.main:	h.files
o.main:	h.render
o.main:	h.verify
o.muldiv: asm.muldiv
o.render:	c.render
o.render:	OS:h.colourtrans
o.render:	OS:h.types
o.render:	OS:h.os
o.render:	OS:h.osspriteop
o.render:	OS:h.font
o.render:	OS:h.draw
o.render:	OS:h.jpeg
o.render:	OS:h.macros
o.render:	OS:h.os
o.render:	h.callback
o.render:	C:h.trace
o.render:	h.muldiv
o.render:	h.riscos
o.render:	OS:h.territory
o.render:	h.jc_trace
o.render:	h.trfm
o.render:	OS:h.drawfile
o.render:	OS:h.wimp
o.render:	h.main
o.render:	C:h.kernel
o.render:	OS:h.messagetrans
o.render:	h.render
o.render:	h.textarea
o.render:	h.verify
o.riscos:	c.riscos
o.riscos:	OS:h.messagetrans
o.riscos:	OS:h.types
o.riscos:	OS:h.os
o.riscos:	OS:h.macros
o.riscos:	OS:h.os
o.riscos:	OS:h.territory
o.riscos:	OS:h.wimp
o.riscos:	OS:h.osspriteop
o.riscos:	OS:h.font
o.riscos:	h.m
o.riscos:	h.realloc
o.riscos:	C:h.trace
o.riscos:	h.riscos
o.riscos:	h.jc_trace
o.riscosa: asm.riscosa
o.riscosa: OS:Hdr.OS
o.riscosa: OS:Hdr.Types
o.textarea:	c.textarea
o.textarea:	OS:h.colourtrans
o.textarea:	OS:h.types
o.textarea:	OS:h.os
o.textarea:	OS:h.osspriteop
o.textarea:	OS:h.font
o.textarea:	OS:h.draw
o.textarea:	OS:h.macros
o.textarea:	OS:h.os
o.textarea:	OS:h.pdriver
o.textarea:	OS:h.messagetrans
o.textarea:	h.callback
o.textarea:	C:h.trace
o.textarea:	h.riscos
o.textarea:	OS:h.territory
o.textarea:	h.jc_trace
o.textarea:	h.trfm
o.textarea:	OS:h.drawfile
o.textarea:	OS:h.wimp
o.textarea:	OS:h.jpeg
o.textarea:	h.main
o.textarea:	C:h.kernel
o.textarea:	h.textarea
o.textarea:	h.verify
o.trfm:	c.trfm
o.trfm:	OS:h.macros
o.trfm:	OS:h.os
o.trfm:	OS:h.types
o.trfm:	h.muldiv
o.trfm:	h.jc_trace
o.trfm:	h.trfm
o.verify:	c.verify
o.verify:	OS:h.macros
o.verify:	OS:h.os
o.verify:	OS:h.types
o.verify:	h.riscos
o.verify:	OS:h.territory
o.verify:	h.jc_trace
o.verify:	OS:h.drawfile
o.verify:	OS:h.osspriteop
o.verify:	OS:h.draw
o.verify:	OS:h.font
o.verify:	OS:h.wimp
o.verify:	OS:h.jpeg
o.verify:	h.main
o.verify:	C:h.kernel
o.verify:	OS:h.messagetrans
o.verify:	h.callback
o.verify:	h.verify
