# Project:   Fader


# Toolflags:
CCflags  = -c -depend !Depend -IC:, -throwback -zM
C++flags = -c -depend !Depend -throwback -IC:
Pascalflags = -c -depend !Depend -throwback -IP:
Linkflags = -m -o $@
ObjAsmflags = -depend !Depend -ThrowBack -Stamp -quit -CloseExec
CMHGflags =
LibFileflags = -c -o $@
Squeezeflags = -o $@
AAsmflags = -depend !Depend -quit -CloseExec -To $@ -From

Files = @.o.module @.o.swicall @.o.Fader

# Final targets:
@.Fader:  $(Files) C:o.Stubs
        link $(linkflags) $(Files) C:o.Stubs


# User-editable dependencies:


# Static dependencies:
.c.o:;        cc $(CCflags) $< -o $@
.s.o:;        objasm $(ObjAsmflags) -from $< -to $@
.cmhg.o:;       cmhg $(CMHGflags) $< -o $@
# Dynamic dependencies:
o.swicall: s.swicall
o.module:	c.module
o.module:	C:h.kernel
o.module:	h.swicall
o.module:	C:h.kernel
o.module:	C:h.swis
