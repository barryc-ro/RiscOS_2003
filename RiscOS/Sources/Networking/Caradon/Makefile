# Project:   Caradon


# Toolflags:
# CCflags  = -c -depend !Depend -IC:, -throwback
CCflags  = -c -depend !Depend -IC:, -throwback -zM 
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

Files = o.module o.driver o.filegen o.swicall o.header o.resourcefs \
        o.screen o.muldiv o.swiven \
        o.backdrop o.connect o.onhook o.offhook o.ppp \
        o.enternnpc o.proceed o.proceedc
Libs  = C:o.Stubs C:o.inetlibzm

# Final targets:
@.Caradon:  $(Files)  $(Libs)
        link $(linkflags) $(Files) $(Libs)

o.resourcefs: @.Resources.Apps.!Sennen.!RunImage
        genres Resources -r -o $@

# User-editable dependencies:

.SUFFIXES: .d

# Static dependencies:
.c.o:;        cc $(CCflags) $< -o $@
.s.o:;        objasm $(ObjAsmflags) -from $< -to $@
.cmhg.o:;     cmhg $(CMHGflags) $< -o $@ h.cmhg_defs
.d.o:;        datatoaof -f $< -o $@ $(Datatoaofflags)

# Dynamic dependencies:
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
o.module:	C:Sockets.h.in
