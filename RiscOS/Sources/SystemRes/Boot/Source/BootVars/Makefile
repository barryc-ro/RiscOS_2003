
CC     = cc -ITCPIPLibs:,C:,C:RMVersion -fafh -wp -DCOMPAT_INET4
LINK   = Link

LIBS  = C:RMVersion.o.RMVersion TCPIPLibs:o.socklib CLib:o.Stubs

OBJS = o.main

BootVars: ${OBJS}
   ${LINK} -output BootVars ${OBJS} ${LIBS}

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
.def.h:;        DefMod -h < $< > $@
.def.s:;        DefMod -s < $< > $@
.def.o:;        DefMod -l -o l.$* < $<
                LibFile -c -o $@ -via ViaFile
                Wipe l.$* ~C~FR~V

#------------------------------------------------------------------------
# Dynamic dependencies:
