# Project:   URITxTest


# Toolflags:
CCflags = -c -wp -depend !Depend -IC: -throwback
C++flags = -c -depend !Depend -IC: -throwback
Linkflags = -aif -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CMHGflags = -depend !Depend -throwback -IC:
LibFileflags = -c -o $@
Squeezeflags = -o $@

# Final targets:
!RunImage:   o.Error o.Main C:o.URI o.dirs\
        C:o.Stubs C:tboxlibs.o.eventlib \
        C:tboxlibs.o.eventlibm C:tboxlibs.o.toolboxlib \
        C:tboxlibs.o.wimplib
        Link $(Linkflags) o.Error o.Main \
        C:o.URI C:o.Stubs \
        C:tboxlibs.o.eventlib C:tboxlibs.o.eventlibm \
        C:tboxlibs.o.toolboxlib C:tboxlibs.o.wimplib

clean:
        -wipe o ~c~v
        -delete !RunImage

o.dirs:
        cdir o
        create o.dirs

# Static dependencies:
o.Error:   c.Error
           cc $(ccflags) -o o.Error c.Error
o.Main:    c.Main
           cc $(ccflags) -o o.Main c.Main

# Dynamic dependencies:
