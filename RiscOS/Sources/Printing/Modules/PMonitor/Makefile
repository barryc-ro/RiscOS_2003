# Project:   PMonitor

COMPONENT = PMonitor
TARGET    = PMonitor

OBJS = o.main o.header o.counting o.error o.IdString o.ink

LIBS = ${DEBUGLIB} ${INETLIB} ${SOCK5LIB}
       
include Makefiles:ModuleLibs
include Makefiles:StdTools
include Makefiles:ModStdRule
include Makefiles:RAMCModule

# Final targets:

clean:
        ${WIPE} ${WPREFLAGS} o.*           ${WFLAGS}
        ${WIPE} ${WPREFLAGS} h.header      ${WFLAGS}
        ${WIPE} ${WPREFLAGS} rm.PMonitor   ${WFLAGS}

# User-editable dependencies:

# Static dependencies:
o.main: h.header

# Dynamic dependencies:
