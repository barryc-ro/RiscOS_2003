# Makefile for a Toolbox Object module
#
# $Id$
#
# Component specific options:
#
RAMBOTH    = o.msgs

COMMON_OBJ = o.Modhdr_NoD o.auxiliary o.create o.delete o.events o.FindAll\
 o.getstate o.hide o.MakeList o.miscop o.show o.Sort o.Sprite o.task\
 o.getitemtex o.setstate o.getsel o.delitem o.additem

ROM_OBJS = o.mainROM ${COMMON_OBJ}

RAM_OBJS = o.main ${RAMBOTH} ${COMMON_OBJ}

DBG_OBJS = o.Modhdr od.auxiliary od.create od.delete od.events od.FindAll\
 od.getstate od.hide od.MakeList od.miscop od.show od.Sort od.Sprite od.task\
 od.getitemtex od.setstate od.getsel od.delitem od.additem od.main ${RAMBOTH}


CINCLUDES = -ISupport
DFLAGS = -DDesk__MODULE_CLIENT

VPATH = @ Support Support.scrolllist

EXTRARAMLIBS = C:Desk.o.Desk_M
EXTRAROMLIBS = ${EXTRARAMLIBS}

#
# Get main rules
#
include C:tboxlibint.TboxMake


#
# Module specific rules:
#
resources: resources-both
        @echo ${COMPONENT}: resource files copied to Messages module

clean: toolbox-clean
        @echo ${COMPONENT}: cleaned

#---------------------------------------------------------------------------
# Dynamic dependencies:
