# Project:   PDumperHPS

COMPONENT = PDumperHPS
TARGET    = PDumperHPS

OBJS = o.PDumpServices o.Init o.Services o.StarCmds o.Routines o.PaceServices o.Variables o.Swi o.header

LIBS = HP_SDK_LIB.HP_SDK_LIB ${C++LIB} 
       
include Makefiles:ModuleLibs
include Makefiles:StdTools
include Makefiles:ModStdRule
include Makefiles:RAMCModule

C++FLAGS = ${CFLAGS} -zM -Wp -ws -zps1 -D_LITTLE_ENDIAN
LDFLAGS += -c++

# Final targets:

clean:
        ${WIPE} ${WPREFLAGS} o.*           ${WFLAGS}
        ${WIPE} ${WPREFLAGS} rm.PDumperHPS ${WFLAGS} 
        ${WIPE} ${WPREFLAGS} h.header      ${WFLAGS}
        ${STRIPDEPEND}

# User-editable dependencies:

# Static dependencies:

# Dynamic dependencies:
