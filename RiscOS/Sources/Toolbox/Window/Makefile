# Makefile for a Toolbox Object module
#
# $Id$
#
# Component specific options:
#
VPATH = @ gadgets

RAMMESSAGES = o.msgs

COMMON_OBJ = o.Modhdr_NoD o.actbut o.adjuster o.button o.create o.delete o.display o.draggable o.events\
 o.gadgets o.getstate o.globals o.hide o.label o.labelbox o.miscop o.numrange o.optbut\
 o.popupmenu o.radiobut o.show o.simple o.slider o.slider_ven o.stringset o.task\
 o.toolbar o.toolbox o.utils o.writable

ROM_OBJS = o.mainROM ${COMMON_OBJ}

RAM_OBJS = o.main ${RAMMESSAGES} ${COMMON_OBJ} ${WIMPLIB}

DBG_OBJS = od.main ${RAMMESSAGES} o.Modhdr\
 od.actbut od.adjuster od.button od.create od.delete od.display od.draggable od.events\
 od.gadgets od.getstate od.globals od.hide od.label od.labelbox od.miscop od.numrange od.optbut\
 od.popupmenu od.radiobut od.show od.simple od.slider od.slider_ven od.stringset od.task\
 od.toolbar o.toolbox od.utils od.writable ${WIMPLIB}

CDEFINES=-D${SYSTEM} ${STYLE}

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
