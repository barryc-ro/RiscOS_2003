# makefile to build LIBIDL - library of functions from the Oracle IDLs

INCLUDES = c:
LIBFILES =

CC = cc
CFLAGS = -c -zM -ffa -wp -throwback -DRISCOS -DSYSTEM -I$(INCLUDES)
INCLUDES=C:,mx_pri:,mx_pub:,mx_inc:,rts:


.SUFFIXES:	.o .c
.c.o:;		$(CC) $(CFLAGS) -I $(INCLUDES) $<

OBJS=o.yoidlC o.ydyoidlC o.yoevtC o.yoidlS o.yoevtS o.yostdC o.yotransC    \
     o.mzzC o.mzcchC o.mzsC o.mzscliC o.mzscliS o.mzcC o.mkdC o.mzalgctnC  \
     o.mzacomC 
     
LIBRARY: $(OBJS) amu.makefile
         libfile -c -o lib.libidl o.*   
         @echo *******************************************
         @echo * libidl library constructed successfully *  
         @echo *******************************************
     

o.yoidlC    : amu.makefile c.yoidlC

o.yoidlS    : amu.makefile c.yoidlS

o.yoevtC    : amu.makefile c.yoevtC

o.yoevtS    : amu.makefile c.yoevtS

o.mzscliC   : amu.makefile c.mzscliC

o.mzscliS   : amu.makefile c.mzscliS

o.yostdC    : amu.makefile c.yostdC

o.yotransC  : amu.makefile c.yotransC
  
o.mzzC      : amu.makefile c.mzzC

o.mzcchC    : amu.makefile c.mzcchC

o.mzsC      : amu.makefile c.mzsC

o.mzcC      : amu.makefile c.mzcC

o.mkdC      : amu.makefile c.mkdC

o.mzalgctnC : amu.makefile c.mzalgctnC
     
o.mzacomC   : amu.makefile c.mzacomC

o.ydyoidlC  : amu.makefile c.ydyoidlC

