# Project:   URITest


# Toolflags:
CCflags = -c -wp -depend !Depend -IC: -throwback
C++flags = -c -depend !Depend -IC: -throwback
Linkflags = -aif -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CMHGflags = -depend !Depend -throwback -IC:
LibFileflags = -c -o $@
Squeezeflags = -o $@


# Final targets:
URIRxTest:   @.o.URIRxTest C:o.URI o.dirs\
        C:o.Stubs C:tboxlibs.o.wimplib
        Link $(Linkflags) @.o.URIRxTest C:o.URI \
        C:o.stubs C:tboxlibs.o.wimplib

clean:
        -wipe o ~c~v
        -delete URIRxTest

o.dirs:
        cdir o
        create o.dirs

# Static dependencies:
@.o.URIRxTest:   @.c.URIRxTest
        cc $(ccflags) -o @.o.URIRxTest @.c.URIRxTest

# Dynamic dependencies:
