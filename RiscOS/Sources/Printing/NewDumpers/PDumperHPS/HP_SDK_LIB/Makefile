COMPONENT = HP_SDK_LIB

OBJS = \
o.aladdin o.ap2100 o.breaks_open o.broadway o.capture \
o.cgtimes o.Compression o.Context o.courier o.create_so o.creator o.DJ600 \
o.DJ600_maps o.DJ630 o.DJ660 o.DJ660_maps o.DJ690 o.DJ690_maps o.DJ6XX \
o.DJ895 o.DJ895_maps o.DJ895_maps2 o.DJ970_maps o.DJ970_maps2 o.filterhpa \
o.Font o.globals o.Header o.hpprint_c_api o.htfed_open o.imager \
o.imager_open o.interp_open o.IO_Utils o.Job o.ltrgothic o.platform o.Printer \
o.Registry o.ResSynth o.rgbtocmyk o.scaler_open o.script o.SystemServices \
o.TextManager o.TextSupport o.Translator o.univers o.version \
o.dj350 o.dj400 o.dj400_maps o.dj540 o.dj890_maps o.models o.versioncode

include Makefiles:StdTools
include Makefiles:ModStdRule
include Makefiles:ModuleLibs
include Makefiles:RAMCModule

C++FLAGS = ${CFLAGS} -zM -Wp -ws -zps1
CINCLUDES += -ICPP:
ARFLAGS += -o

# Final targets:

library: ${COMPONENT}
        @echo ${COMPONENT}: library built

clean:
        ${WIPE} ${WPREFLAGS} o.*        ${WFLAGS}
        ${WIPE} ${WPREFLAGS} HP_SDK_LIB ${WFLAGS}
        ${STRIPDEPEND}

# Make the library
${COMPONENT}: ${OBJS}
        ${AR} ${COMPONENT} ${ARFLAGS} ${OBJS}

# Dynamic dependencies:
