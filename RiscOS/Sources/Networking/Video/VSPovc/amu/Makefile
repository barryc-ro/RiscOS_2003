# makefile for VSPovc module
#
# debug directives:
#
# NDEBUG - should disable ALL debugging when defined
#
# EOS - sets series of system variables detailing what caused video to stop
#       (OVCdebug$*)
#
# SOS - same as EOS but for start of stream
#
# USEOLDREAD_POS - forces old read_pos() routine to be used.

VPATH=@ rts.src.vid

INCLUDES = c:
LIBFILES =
LINKFLAGS=-rmf

CC = cc
CMHG=cmhg
OBJASM=objasm

#CFLAGS = -c -zM -ffa -wp -throwback -DRISCOS -DSYSTEM -I$(INCLUDES) -DNDEBUG -D#USEOLDREAD_POS
CFLAGS = -c -zM -ffa -wp -throwback -DRISCOS -DSYSTEM -I$(INCLUDES) -DSOS -DUSEOLDREAD_POS
INCLUDES=C:,mx_pri:,mx_pub:,mx_inc:,rts:,TCPIPlibs:

IDLLIB= @.fromidl.lib.libidl
YOLIB= @.mx.src.yo.lib.libyo


.SUFFIXES:	.o .c .cmhg
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.c.o:;		$(CC) $(CFLAGS) $<
.s.o:;		$(OBJASM) -from $< -to $@

OBJS=o.module o.vspc o.smnudp o.mzswrap o.mzscl o.mzsclii o.umdn o.vsphdr o.debug

VSPovc:  $(OBJS) $(IDLLIB)
         link $(LINKFLAGS) -o $@ $(OBJS) ${IDLLIB} ${YOLIB} -via amu.link
         @echo ***********************************
         @echo * VSPovc constructed successfully *
         @echo ***********************************


o.debug:   amu.makefile c.debug h.debug

o.module:   amu.makefile c.module cmhg.h h.mzswrap h.module h.vspc smnudp.h debug.h

o.vspc   :  amu.makefile c.vspc h.mzswrap h.module h.vspc debug.h

o.smnudp :  amu.makefile c.smnudp module.h

o.mzswrap:  amu.makefile c.mzswrap h.mzswrap h.module h.vspc h.debug

o.mzscl  :  amu.makefile c.mzscl

o.mzsclii:  amu.makefile c.mzsclii

o.umdn   :  amu.makefile s.umdn

o.vsphdr :  amu.makefile cmhg.vsphdr

cmhg.h:  amu.makefile cmhg.vsphdr
         cmhg -d h.cmhg cmhg.VSPhdr


