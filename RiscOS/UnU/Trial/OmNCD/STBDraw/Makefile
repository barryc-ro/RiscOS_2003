# Project:   STBDraw


# Toolflags:
CCflags  = -c -depend !Depend -IC:, ${DEC} -throwback -zM
C++flags = -c -depend !Depend -throwback -IC:
Pascalflags = -c -depend !Depend -throwback -IP:
Linkflags = -m -o $@
ObjAsmflags = -depend !Depend -ThrowBack -Stamp -quit -CloseExec
CMHGflags =
LibFileflags = -c -o $@
Squeezeflags = -o $@
AAsmflags = -depend !Depend -quit -CloseExec -To $@ -From

Files = @.o.DrawSupp @.o.module @.o.swicall @.o.muldiv @.o.pbox

# Final targets:
@.STBDraw:  $(Files) C:o.Stubs
        link $(linkflags) $(Files) C:o.Stubs


# User-editable dependencies:


# Static dependencies:
@.o.DrawSupp:   @.cmhg.DrawSupp
        cmhg @.cmhg.DrawSupp -o @.o.DrawSupp
@.o.module:   @.c.module
        cc $(ccflags) -o @.o.module @.c.module
@.o.swicall:   @.s.swicall
        objasm $(objasmflags) -from @.s.swicall -to @.o.swicall
@.o.muldiv:   @.s.muldiv
        objasm $(objasmflags) -from @.s.muldiv -to @.o.muldiv
@.o.pbox:   @.s.pbox
        objasm $(objasmflags) -from @.s.pbox -to @.o.pbox

# Dynamic dependencies:
o.swicall: s.swicall
o.muldiv: s.muldiv
o.pbox: s.pbox
o.getvno: s.getvno
o.getvno: h.regdefs
o.geo.module:	c.module
o.swicall: s.swicall
o.muldiv: s.muldiv
o.pbox: s.pbox
o.getvno: s.getvno
o.getvno: h.regdefs
o.getvno: h.swinos
o.getvno: h.macros
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.swicall
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.commands
o.module:	h.drawfile
o.module:	OS:h.types
o.module:	OS:h.os
o.module:	OS:h.osspriteop
o.module:	OS:h.draw
o.module:	OS:h.font
o.module:	OS:h.wimp
o.module:	h.muldiv
