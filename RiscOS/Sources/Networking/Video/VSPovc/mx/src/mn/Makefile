# makefile for Medianet modules
#
# debug directives:
#
# NDEBUG - should disable ALL debugging when defined
#
# NOTICK - turns off periodic call to mnIdle (and hence the heartbeat)

TARGET=lib.mnlib

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

CFLAGS = -c -depend !Depend -zM -ffa -wp -throwback -DRISCOS -DSYSTEM -I$(INCLUDES) -DCOMPAT_INET4 -DNDEBUG -DNOTICK



.SUFFIXES:	.o .c .cmhg
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.c.o:;		$(CC) $(CFLAGS) $<
.s.o:;		$(OBJASM) -from $< -to $@

OBJS=o.mn o.mna o.mne o.mnb o.mnm o.mnn o.mno o.mnr o.mnrc o.mnrs o.mnt o.mntr \
            o.mnts o.mnx o.mzn o.mznc o.mznt o.mzmgs o.mnscc o.mnsc o.mnscmd \
            o.mnscs o.mnscsop o.smni

lib.mnlib:  $(OBJS) $(IDLLIB)
         $(LIB) $(LIBFLAGS) lib.mnlib $(OBJS)
         @echo ***************************************
         @echo * MN library constructed successfully *
         @echo ***************************************


# Dynamic dependencies:
o.smni:	sosd.riscos.c.smni
o.smni:	mx_pub:h.mn
o.smni:	mx_pri:h.sysx
o.smni:	mx_pri:h.sx
o.smni:	mx_pri:h.sysb8
o.smni:	mx_pub:h.ysid
o.smni:	mx_pub:h.ysb8
o.smni:	mx_pub:h.mnerr
o.smni:	mx_pub:h.mnsem
o.smni:	mx_pub:h.mna
o.smni:	mx_pub:h.mnt
o.smni:	mx_pub:h.mnnio
o.smni:	mx_pub:h.mzn
o.smni:	mx_pub:h.mnr
o.smni:	mx_pub:h.mnx
o.smni:	mx_pub:h.mnrs
o.smni:	sosd.riscos.h.smn
o.smni:	mx_pri:h.sysfp
o.smni:	mx_pub:h.ys
o.smni:	mx_pub:h.ysbv
o.smni:	mx_pub:h.ysfo
o.smni:	mx_pub:h.ysstr
o.smni:	mx_pub:h.ysm
o.smni:	mx_pub:h.yslst
o.smni:	mx_pub:h.yshsh
o.smni:	mx_pub:h.ysc
o.smni:	mx_pub:h.yse
o.smni:	mx_pub:h.ystm
o.smni:	mx_pub:h.ysv
o.smni:	mx_pub:h.ysmsc
o.smni:	mx_pub:h.ysl
o.smni:	mx_pub:h.ysr
o.smni:	mx_pub:h.yslog
o.smni:	mx_pub:h.ysx
o.smni:	mx_pub:h.ysthr
o.smni:	mx_inc:h.mtl
o.smni:	mx_pri:h.sysi
o.smni:	C:h.kernel
o.smni:	C:h.swis
o.smni:	mx_pub:h.smnudp
o.smni:	mx_pub:h.misc
