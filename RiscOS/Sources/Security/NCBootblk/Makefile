# Makefile for NCBootblock
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When          Who     What
# ----          ---     ----
# 7-Apr-97      AJS     Created
#

#
# Paths
#
EXP_HDR = <export$dir>
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

#
# Module specific macros
#
# Different compile options - choose one
# AUTHENTICATE_SIGNATURES will cause NCBootblock to reject inauthentic smartcard data
#
CDEFINES   = -UUSE_TINY -UREMOTE_DEBUG -UAUTHENTICATE_SIGNATURES ${OPTS}
CINCLUDES  = -IC:

#
# Generic options:
#
include Makefiles:StdTools
include Makefiles:ModuleLibs

#
# Program specific options:
#

OBJS      = module.o header.o reg_swis.o reg_events.o enum_ddi.o \
            reg_scbb.o reg_cmds.o reg_status.o reg_utils.o dbugzm.o \
            machine.o dynalloc.o reg_auth.o reg_index.o \
            reg_files.o BORDERDBUG.o kprintf.o reg_sign.o reg_tags.o

LIBS      = ${WIMPLIB} ${LIBSD} ${NET5LIBS} ${RLIB}
ROM_LIBS  = ${LIBS} ${C++LIB}

EXPORTS   = ${EXP_C_H}.DBUG

#
# Generic rules:
#
include Makefiles:ModStdRule
include Makefiles:ROMCModule
include Makefiles:RAMCModule

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Exported interface headers
#
${EXP_C_H}.DBUG:        h.DBUG
        ${CP} h.DBUG $@ ${CPFLAGS}

# Dynamic dependencies:
