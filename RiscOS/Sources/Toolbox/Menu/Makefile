# Makefile for a Toolbox Object module
#
# $Id$
#
# Component specific options:
#
RAMMESSAGES = o.msgs

COMMON_OBJ = o.Modhdr_NoD o.auxiliary o.create o.delete o.events o.getstate\
 o.hide o.miscop o.show o.task

ROM_OBJS = o.mainROM ${COMMON_OBJ}

RAM_OBJS = o.main ${RAMMESSAGES} ${COMMON_OBJ}

DBG_OBJS = od.main ${RAMMESSAGES} o.Modhdr od.auxiliary od.create od.delete od.events od.getstate\
 od.hide od.miscop od.show od.task

#
# Get main rules
#
include C:tboxlibint.TboxMake

#
# Module specific rules:
#
resources: resources-messages
        @echo ${COMPONENT}: resource files copied to Messages module

clean: toolbox-clean
        @echo ${COMPONENT}: cleaned

#---------------------------------------------------------------------------
# Dynamic dependencies:
