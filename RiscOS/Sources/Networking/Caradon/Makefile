# Project:   Caradon


# Toolflags:
# CCflags  = -c -depend !Depend -IC:, -throwback
CCflags  = -c -depend !Depend -IC:, -throwback -zM -DFAKEWEB
C++flags = -c -depend !Depend -throwback -IC:
Pascalflags = -c -depend !Depend -throwback -IP:
# Linkflags = -aif -o $@
Linkflags = -m -o $@
ObjAsmflags = -depend !Depend -ThrowBack -Stamp -quit -CloseExec
CMHGflags =
LibFileflags = -c -o $@
Squeezeflags = -o $@
AAsmflags = -depend !Depend -quit -CloseExec -To $@ -From
Datatoaofflags = -d "" -l _len -dp -s -p

CP      = copy
CPFLAGS = ~cfr~v
COMPONENT   = Caradon
TARGET      = rm.Caradon

Files = o.module o.driver o.filegen o.swicall o.header o.resourcefs \
        o.screen o.muldiv o.swiven \
        o.backdrop o.connect o.onhook o.offhook o.ppp\
        o.enternnpc o.proceed o.proceedc
Libs  = C:o.Stubs C:o.inetlibzm

# Final targets:
rm.Caradon:  $(Files)  $(Libs)
        link $(linkflags) $(Files) $(Libs)
        modsqz $(TARGET)

o.resourcefs: @.Resources.Apps.!Sennen.!RunImage
        genres Resources -r -o $@

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

# User-editable dependencies:

.SUFFIXES: .d

# Static dependencies:
.c.o:;        cc $(CCflags) $< -o $@
.s.o:;        objasm $(ObjAsmflags) -from $< -to $@
.cmhg.o:;     cmhg $(CMHGflags) $< -o $@ h.cmhg_defs
.d.o:;        datatoaof -f $< -o $@ $(Datatoaofflags)

# Dynamic dependencies:
o.swicall: s.swicall
o.muldiv: s.muldiv
o.swiven: s.swiven
o.swiven: h.regdefs
o.swiven: h.swinos
o.swiven: h.macros
o.screen:	c.screen
o.screen:	h.defs
o.screen:	C:h.kernel
o.screen:	h.inlines
o.screen:	C:h.swis
o.screen:	h.swicall
o.screen:	h.defs
o.screen:	C:h.swis
o.module:	c.module
o.module:	h.cmhg_defs
o.module:	h.driver
o.module:	h.inlines
o.module:	C:h.swis
o.module:	C:h.kernel
o.module:	C:h.kernel
o.module:	h.screen
o.module:	h.defs
o.module:	C:h.kernel
o.module:	h.smartcard
o.module:	h.swicall
o.module:	h.defs
o.module:	C:h.swis
o.module:	Sockets.h.in
o.filegen:	c.filegen
o.filegen:	h.defs
o.filegen:	C:h.kernel
o.filegen:	h.inlines
o.filegen:	C:h.swis
o.filegen:	h.smartcard
o.filegen:	h.swicall
o.filegen:	h.defs
o.filegen:	C:h.swis
o.filegen:	Sockets.h.in
