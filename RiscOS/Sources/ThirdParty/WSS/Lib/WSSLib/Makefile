# Makefile for WSSLib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name         Description
# ----         ----         -----------
# 2001-04-03   NBingham     Created
#

#
# Paths
#
EXPORT_DIR  = <Lib$Dir>.ThirdParty.WSS.WSSLib
EXP_HDR     = <export$dir>
EXP_C_H     = <Cexport$dir>.h
EXP_C_O     = <Cexport$dir>.o

#
# Generic options:
#
include Makefiles:StdTools

COMPONENT = WSSLib
TARGET   ?= ${COMPONENT}
TARGETZ   = ${TARGET}ZM

#
# Flags
#
CINCLUDES = -Itbox:,C:TCPIPLibs,<Lib$Dir>.ThirdParty.WSS
CDEFINES  = ${DFLAGS} ${OPTS}
AFLAGS   += -NoCache
CFLAGS   += -Wap -ffah

#
# Include files
#
DIRS     = o._dirs


# Program specific options:
#
OBJS      = o.WSSdev o.WSSerr o.WSSflex o.WSSfs o.WSSgr o.WSSicn 	        \
	    o.WSSmem o.WSSmnu o.WSSmode o.WSSplugin o.WSSprt o.WSSptr o.WSSscan	\
	    o.WSSscn o.WSSslider o.WSSspr o.WSSsprf o.WSStask o.WSStm o.WSSvar  \
	    o.WSSwin
OBJSZ     = oz.WSSdev oz.WSSerr oz.WSSflex oz.WSSfs oz.WSSgr oz.WSSicn 	                \
	    oz.WSSmem oz.WSSmnu oz.WSSmode oz.WSSplugin oz.WSSprt oz.WSSptr oz.WSSscan	\
	    oz.WSSscn oz.WSSslider oz.WSSspr oz.WSSsprf oz.WSStask oz.WSStm oz.WSSvar   \
	    oz.WSSwin

#
# Rule patterns
#
.SUFFIXES: .z .o .c .s .oz
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.oz:;     ${CC} ${CFLAGS} -zm -zps1 -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {FALSE}"
.s.oz:;     ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {TRUE}"

#
# Build
#

${DIRS}:    
         ${MKDIR} o
         ${MKDIR} z
         ${MKDIR} oz
         ${TOUCH} $@

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} z ${WFLAGS}
        ${WIPE} oz ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${TARGETZ}
        @echo ${COMPONENT}: cleaned

export: export_${PHASE}
        @echo ${COMPONENT}: export complete

export_hdrs:
        ${MKDIR} ${EXPORT_DIR}.h
        ${CP} h.keys ${EXPORT_DIR}.h.keys   ${CPFLAGS}
        ${CP} h.WSSdev ${EXPORT_DIR}.h.WSSdev   ${CPFLAGS}
        ${CP} h.WSSerr ${EXPORT_DIR}.h.WSSerr   ${CPFLAGS}
        ${CP} h.WSSerrxxx ${EXPORT_DIR}.h.WSSerrxxx   ${CPFLAGS}
        ${CP} h.WSSflex ${EXPORT_DIR}.h.WSSflex   ${CPFLAGS}
        ${CP} h.WSSfs ${EXPORT_DIR}.h.WSSfs   ${CPFLAGS}
        ${CP} h.WSSgr ${EXPORT_DIR}.h.WSSgr   ${CPFLAGS}
        ${CP} h.WSSicn ${EXPORT_DIR}.h.WSSicn   ${CPFLAGS}
        ${CP} h.WSSmem ${EXPORT_DIR}.h.WSSmem   ${CPFLAGS}
        ${CP} h.WSSmnu ${EXPORT_DIR}.h.WSSmnu   ${CPFLAGS}
        ${CP} h.WSSmode ${EXPORT_DIR}.h.WSSmode   ${CPFLAGS}
        ${CP} h.WSSplugin ${EXPORT_DIR}.h.WSSplugin   ${CPFLAGS}
        ${CP} h.WSSprt ${EXPORT_DIR}.h.WSSprt   ${CPFLAGS}
        ${CP} h.WSSptr ${EXPORT_DIR}.h.WSSptr   ${CPFLAGS}
        ${CP} h.WSSscan ${EXPORT_DIR}.h.WSSscan   ${CPFLAGS}
        ${CP} h.WSSscn ${EXPORT_DIR}.h.WSSscn   ${CPFLAGS}
        ${CP} h.WSSslider ${EXPORT_DIR}.h.WSSslider   ${CPFLAGS}
        ${CP} h.WSSspr ${EXPORT_DIR}.h.WSSspr   ${CPFLAGS}
        ${CP} h.WSSsprf ${EXPORT_DIR}.h.WSSsprf   ${CPFLAGS}
        ${CP} h.WSStask ${EXPORT_DIR}.h.WSStask   ${CPFLAGS}
        ${CP} h.WSStm ${EXPORT_DIR}.h.WSStm   ${CPFLAGS}
        ${CP} h.WSSvar ${EXPORT_DIR}.h.WSSvar   ${CPFLAGS}
        ${CP} h.WSSwin ${EXPORT_DIR}.h.WSSwin   ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ}
        ${MKDIR} ${EXPORT_DIR}.o
        ${CP} ${TARGET} ${EXPORT_DIR}.o.${TARGET} ${CPFLAGS}
        ${CP} ${TARGETZ} ${EXPORT_DIR}.o.${TARGETZ} ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)



# ------------------------------------------------------------------------------
# Final link
#

$(TARGET): $(OBJS) $(DIRS)
        ${AR} ${ARFLAGS} -o $@ ${OBJS}

$(TARGETZ): $(OBJSZ) $(DIRS)
        ${AR} ${ARFLAGS} -o $@ ${OBJSZ}

#
# Dynamic dependencies:
