# Project:   PDumperHPS

COMPONENT = PDumperHPS
TARGET = ${COMPONENT}

OBJS = o.PDumpServices o.Init o.Services o.StarCmds o.Routines o.PaceServices o.Variables o.Swi o.modheader

LIBS = HP_SDK_LIB.HP_SDK_LIB ${C++LIB} 
       
include Makefiles:ModuleLibs
include Makefiles:StdTools
include Makefiles:ModStdRule
include Makefiles:RAMCModule

C++FLAGS = ${CFLAGS} -zM -Wp -ws -zps1 -D_LITTLE_ENDIAN
LDFLAGS += -c++

# Final targets:

all: ${RAM_MODULE}  h.modheader dirs HP_SDK_LIB.HP_SDK_LIB

clean:
        ${WIPE} ${WPREFLAGS} o             ${WFLAGS}
        ${WIPE} ${WPREFLAGS} rm            ${WFLAGS} 
        ${WIPE} ${WPREFLAGS} h.modheader   ${WFLAGS}
        dir HP_SDK_LIB
        amu_machine clean
        back
        @echo ${COMPONENT}: cleaned

dirs:
        ${MKDIR} o

HP_SDK_LIB.HP_SDK_LIB:
        dir HP_SDK_LIB
        amu_machine library HPDEFINES="${HPDEFINES}"
        back

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

        
# User-editable dependencies:

# Static dependencies:

o.modheader: dirs h.modheader

# Dynamic dependencies:
