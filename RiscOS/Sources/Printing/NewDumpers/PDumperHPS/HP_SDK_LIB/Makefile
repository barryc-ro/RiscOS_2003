COMPONENT = HP_SDK_LIB

OBJS = \
o.aladdin o.ap2100 o.breaks_open o.broadway o.capture \
o.cgtimes o.Compression o.Context o.courier o.create_so o.creator o.DJ600 \
o.DJ600_maps o.DJ630 o.DJ660 o.DJ660_maps o.DJ690 o.DJ690_maps o.DJ6XX \
o.DJ895 o.DJ895_maps o.DJ895_maps2 o.DJ970_maps o.DJ970_maps2 o.filterhpa \
o.Font o.globals o.Header o.hpprint_c_api o.htfed_open o.imager \
o.imager_open o.interp_open o.Job o.ltrgothic o.platform o.Printer \
o.Registry o.ResSynth o.rgbtocmyk o.scaler_open o.script o.SystemServices \
o.TextManager o.TextSupport o.Translator o.univers o.version \
o.dj350 o.dj400 o.dj400_maps o.dj540 o.dj890_maps o.models o.versioncode

include Makefiles:StdTools
include Makefiles:ModStdRule
include Makefiles:ModuleLibs
include Makefiles:RAMCModule

C++FLAGS = ${CFLAGS} -zM -Wp -ws -zps1 -D_LITTLE_ENDIAN
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
o.models: c++.models h.models
o.dj400_maps: c++.dj400_maps
o.dj540: c++.dj540
o.dj890_maps: c++.dj890_maps
o.Translator: c++.Translator h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.imager h.hptypes
o.versioncode: c++.versioncode h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa
o.TextManager: c++.TextManager
o.SystemServices: c++.SystemServices h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.io_defs
o.platform: c++.platform CPP:h.malloc CPP:h.memory
o.Printer: c++.Printer h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.io_defs h.resources
o.Registry: c++.Registry h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.DJ895 h.broadway
o.ResSynth: c++.ResSynth h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa
o.rgbtocmyk: c++.rgbtocmyk h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.imager h.hptypes
o.scaler_open: c++.scaler_open h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.scaler_open
o.breaks_open: c++.breaks_open h.resources
o.broadway: c++.broadway h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.io_defs h.dj895 h.broadway
o.Compression: c++.Compression h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa
o.Context: c++.Context h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa
o.courier: c++.courier
o.create_so: c++.create_so h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.scaler_open
o.creator: c++.creator h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.imaging h.imager h.hptypes h.resources
o.DJ660_maps: c++.DJ660_maps
o.DJ690: c++.DJ690
o.DJ690_maps: c++.DJ690_maps
o.DJ6XX: c++.DJ6XX
o.DJ895: c++.DJ895 h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.io_defs h.dj895
o.DJ895_maps: c++.DJ895_maps
o.DJ895_maps2: c++.DJ895_maps2
o.DJ970_maps2: c++.DJ970_maps2
o.filterhpa: c++.filterhpa
o.globals: c++.globals h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa
o.Header: c++.Header h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa
o.htfed_open: c++.htfed_open h.hptypes h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.imaging h.imager
o.imager: c++.imager h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.imager h.hptypes
o.imager_open: c++.imager_open h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.imaging h.imager h.hptypes
o.interp_open: c++.interp_open h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.imaging h.imager h.hptypes
o.IO_Utils: c++.IO_Utils h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.io_defs
o.Job: c++.Job h.header h.platform CPP:h.malloc CPP:h.memory h.debug h.hpprintapi h.global_types h.models h.modes h.internal h.printer h.modes h.font h.filterhpa h.imaging_open h.imager h.hptypes h.imaging_prop h.imager h.scaler_open h.scaler_prop h.debug
