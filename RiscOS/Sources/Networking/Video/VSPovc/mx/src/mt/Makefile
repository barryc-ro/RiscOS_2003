# makefile for Medianet modules
#
# debug directives:
#
# NDEBUG - should disable ALL debugging when defined
#

TARGET=lib.mtlib

VPATH=@ sosd.riscos

LIBFILES =
LINKFLAGS=-rmf

CMHGFLAGS=-p


CC = cc
INCLUDES=C:,mx_pri:,mx_pub:,mx_inc:,rts:,TCPIPlibs:
CMHG=cmhg
OBJASM=objasm

LIB=libfile
LIBFLAGS=-c -o

CFLAGS = -c -depend !Depend -zM -ffa -wp -throwback -DRISCOS -DSYSTEM -I$(INCLUDES) -DCOMPAT_INET4 -DNDEBUG



.SUFFIXES:	.o .c .cmhg
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.c.o:;		$(CC) $(CFLAGS) $<
.s.o:;		$(OBJASM) -from $< -to $@

OBJS=o.mtcctx o.mtrgx o.mtsp o.mtl

lib.mtlib:  $(OBJS)
         $(LIB) $(LIBFLAGS) lib.mtlib $(OBJS)
         @echo ***************************************
         @echo * MT library constructed successfully *
         @echo ***************************************


# Dynamic dependencies:
o.mtcctx:	c.mtcctx
o.mtcctx:	mx_pri:h.sysi
o.mtcctx:	mx_pri:h.sysx
o.mtcctx:	mx_pri:h.sx
o.mtcctx:	mx_pri:h.sysb8
o.mtcctx:	mx_pub:h.ysid
o.mtcctx:	mx_pub:h.ysb8
o.mtcctx:	mx_pub:h.ys
o.mtcctx:	mx_pub:h.ysbv
o.mtcctx:	mx_pub:h.ysfo
o.mtcctx:	mx_pub:h.ysstr
o.mtcctx:	mx_pub:h.ysm
o.mtcctx:	mx_pub:h.yslst
o.mtcctx:	mx_pub:h.yshsh
o.mtcctx:	mx_pub:h.ysc
o.mtcctx:	mx_pub:h.yse
o.mtcctx:	mx_pub:h.ystm
o.mtcctx:	mx_pub:h.ysv
o.mtcctx:	mx_pub:h.ysmsc
o.mtcctx:	mx_pub:h.ysl
o.mtcctx:	mx_pub:h.ysr
o.mtcctx:	mx_pub:h.yslog
o.mtcctx:	mx_pub:h.ysx
o.mtcctx:	mx_pub:h.ysthr
o.mtcctx:	mx_inc:h.mtcctx
o.mtrgx:	c.mtrgx
o.mtrgx:	mx_pri:h.sysi
o.mtrgx:	mx_pri:h.sysx
o.mtrgx:	mx_pri:h.sx
o.mtrgx:	mx_pri:h.sysb8
o.mtrgx:	mx_pub:h.ysid
o.mtrgx:	mx_pub:h.ysb8
o.mtrgx:	mx_pub:h.ys
o.mtrgx:	mx_pub:h.ysbv
o.mtrgx:	mx_pub:h.ysfo
o.mtrgx:	mx_pub:h.ysstr
o.mtrgx:	mx_pub:h.ysm
o.mtrgx:	mx_pub:h.yslst
o.mtrgx:	mx_pub:h.yshsh
o.mtrgx:	mx_pub:h.ysc
o.mtrgx:	mx_pub:h.yse
o.mtrgx:	mx_pub:h.ystm
o.mtrgx:	mx_pub:h.ysv
o.mtrgx:	mx_pub:h.ysmsc
o.mtrgx:	mx_pub:h.ysl
o.mtrgx:	mx_pub:h.ysr
o.mtrgx:	mx_pub:h.yslog
o.mtrgx:	mx_pub:h.ysx
o.mtrgx:	mx_pub:h.ysthr
o.mtrgx:	mx_inc:h.mtrgx
o.mtsp:	c.mtsp
o.mtsp:	mx_pri:h.sysi
o.mtsp:	mx_pri:h.sysx
o.mtsp:	mx_pri:h.sx
o.mtsp:	mx_pri:h.sysb8
o.mtsp:	mx_pub:h.ysid
o.mtsp:	mx_pub:h.ysb8
o.mtsp:	mx_pub:h.ys
o.mtsp:	mx_pub:h.ysbv
o.mtsp:	mx_pub:h.ysfo
o.mtsp:	mx_pub:h.ysstr
o.mtsp:	mx_pub:h.ysm
o.mtsp:	mx_pub:h.yslst
o.mtsp:	mx_pub:h.yshsh
o.mtsp:	mx_pub:h.ysc
o.mtsp:	mx_pub:h.yse
o.mtsp:	mx_pub:h.ystm
o.mtsp:	mx_pub:h.ysv
o.mtsp:	mx_pub:h.ysmsc
o.mtsp:	mx_pub:h.ysl
o.mtsp:	mx_pub:h.ysr
o.mtsp:	mx_pub:h.yslog
o.mtsp:	mx_pub:h.ysx
o.mtsp:	mx_pub:h.ysthr
o.mtsp:	mx_inc:h.mtsp
o.mtl:	c.mtl
o.mtl:	mx_pri:h.sysi
o.mtl:	mx_pri:h.sysx
o.mtl:	mx_pri:h.sx
o.mtl:	mx_pri:h.sysb8
o.mtl:	mx_pub:h.ysid
o.mtl:	mx_pub:h.ysb8
o.mtl:	mx_pub:h.ys
o.mtl:	mx_pub:h.ysbv
o.mtl:	mx_pub:h.ysfo
o.mtl:	mx_pub:h.ysstr
o.mtl:	mx_pub:h.ysm
o.mtl:	mx_pub:h.yslst
o.mtl:	mx_pub:h.yshsh
o.mtl:	mx_pub:h.ysc
o.mtl:	mx_pub:h.yse
o.mtl:	mx_pub:h.ystm
o.mtl:	mx_pub:h.ysv
o.mtl:	mx_pub:h.ysmsc
o.mtl:	mx_pub:h.ysl
o.mtl:	mx_pub:h.ysr
o.mtl:	mx_pub:h.yslog
o.mtl:	mx_pub:h.ysx
o.mtl:	mx_pub:h.ysthr
o.mtl:	mx_pub:h.ysfmt
o.mtl:	mx_inc:h.mtl
