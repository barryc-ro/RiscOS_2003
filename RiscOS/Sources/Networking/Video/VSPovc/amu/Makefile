# makefile for VSPovc module

VPATH=@ rts.src.vid

INCLUDES = c:
LIBFILES =
LINKFLAGS=-rmf

CC = cc
CMHG=cmhg
OBJASM=objasm

CFLAGS = -c -zM -ffa -wp -throwback -DRISCOS -DSYSTEM -I$(INCLUDES)
INCLUDES=C:,mx_pri:,mx_pub:,mx_inc:,rts:

IDLLIB= @.fromidl.lib.libidl
YOLIB= @.mx.src.yo.lib.libyo


.SUFFIXES:	.o .c .cmhg
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.c.o:;		$(CC) $(CFLAGS) $<
.s.o:;		$(OBJASM) -from $< -to $@

OBJS=o.module o.vspc o.smnudp o.mzswrap o.mzscl o.mzsclii o.umdn o.vsphdr

VSPovc:  $(OBJS) $(IDLLIB)
         link $(LINKFLAGS) -o $@ $(OBJS) @.mx.src.mn.o.* @.mx.src.mt.o.* \
            @.mx.src.yo.o.* @.mx.src.ys.o.* @.mx.src.yr.o.* ${IDLLIB} \
            clib:o.stubs InetInc:o.inetlibzm InetInc:o.socklibzm ${YOLIB}
         @echo ***********************************
         @echo * VSPovc constructed successfully *  
         @echo ***********************************
         
         
o.module:   amu.makefile c.module

o.vspc   :  amu.makefile c.vspc

o.smnudp :  amu.makefile c.smnudp

o.mzswrap:  amu.makefile c.mzswrap 

o.mzscl  :  amu.makefile c.mzscl

o.mzsclii:  amu.makefile c.mzsclii 

o.umdn   :  amu.makefile s.umdn

o.vsphdr :  amu.makefile cmhg.vsphdr  


