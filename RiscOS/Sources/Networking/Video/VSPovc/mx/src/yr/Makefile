# makefile for Medianet modules
#
# debug directives:
#
# NDEBUG - should disable ALL debugging when defined
#

TARGET=lib.yrlib

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

OBJS=o.yr o.yrc o.yrwrite o.yrtk o.yrdump o.yrdst


lib.yrlib:  $(OBJS)
         $(LIB) $(LIBFLAGS) lib.yrlib $(OBJS)
         @echo ***************************************
         @echo * YR library constructed successfully *
         @echo ***************************************


# Dynamic dependencies:
o.yr:	c.yr
o.yr:	mx_pri:h.sysi
o.yr:	mx_pri:h.sysx
o.yr:	mx_pri:h.sx
o.yr:	mx_pri:h.sysb8
o.yr:	mx_pub:h.ysid
o.yr:	mx_pub:h.ysb8
o.yr:	mx_pub:h.ys
o.yr:	mx_pub:h.ysbv
o.yr:	mx_pub:h.ysfo
o.yr:	mx_pub:h.ysstr
o.yr:	mx_pub:h.ysm
o.yr:	mx_pub:h.yslst
o.yr:	mx_pub:h.yshsh
o.yr:	mx_pub:h.ysc
o.yr:	mx_pub:h.yse
o.yr:	mx_pub:h.ystm
o.yr:	mx_pub:h.ysv
o.yr:	mx_pub:h.ysmsc
o.yr:	mx_pub:h.ysl
o.yr:	mx_pub:h.ysr
o.yr:	mx_pub:h.yslog
o.yr:	mx_pub:h.ysx
o.yr:	mx_pub:h.ysthr
o.yr:	mx_pub:h.ysfmt
o.yr:	mx_pub:h.yr
o.yr:	mx_pub:h.yo
o.yr:	mx_pub:h.ytex
o.yr:	mx_pub:h.yodefs
o.yr:	mx_pub:h.yotk
o.yr:	mx_pub:h.yrtypes
o.yr:	mx_pub:h.mn
o.yr:	mx_pub:h.mnerr
o.yr:	mx_pub:h.mnsem
o.yr:	mx_pub:h.mna
o.yr:	mx_pub:h.mnt
o.yr:	h.yr0
o.yr:	mx_pri:h.sysfp
o.yrc:	c.yrc
o.yrc:	mx_pri:h.sysi
o.yrc:	mx_pri:h.sysx
o.yrc:	mx_pri:h.sx
o.yrc:	mx_pri:h.sysb8
o.yrc:	mx_pub:h.ysid
o.yrc:	mx_pub:h.ysb8
o.yrc:	mx_pub:h.ys
o.yrc:	mx_pub:h.ysbv
o.yrc:	mx_pub:h.ysfo
o.yrc:	mx_pub:h.ysstr
o.yrc:	mx_pub:h.ysm
o.yrc:	mx_pub:h.yslst
o.yrc:	mx_pub:h.yshsh
o.yrc:	mx_pub:h.ysc
o.yrc:	mx_pub:h.yse
o.yrc:	mx_pub:h.ystm
o.yrc:	mx_pub:h.ysv
o.yrc:	mx_pub:h.ysmsc
o.yrc:	mx_pub:h.ysl
o.yrc:	mx_pub:h.ysr
o.yrc:	mx_pub:h.yslog
o.yrc:	mx_pub:h.ysx
o.yrc:	mx_pub:h.ysthr
o.yrc:	mx_pub:h.yr
o.yrc:	mx_pub:h.yo
o.yrc:	mx_pub:h.ytex
o.yrc:	mx_pub:h.yodefs
o.yrc:	mx_pub:h.yotk
o.yrc:	mx_pub:h.yrtypes
o.yrc:	mx_pub:h.mn
o.yrc:	mx_pub:h.mnerr
o.yrc:	mx_pub:h.mnsem
o.yrc:	mx_pub:h.mna
o.yrc:	mx_pub:h.mnt
o.yrc:	h.yr0
o.yrc:	mx_pri:h.sysfp
o.yrwrite:	c.yrwrite
o.yrwrite:	mx_pri:h.sysi
o.yrwrite:	mx_pri:h.sysx
o.yrwrite:	mx_pri:h.sx
o.yrwrite:	mx_pri:h.sysb8
o.yrwrite:	mx_pub:h.ysid
o.yrwrite:	mx_pub:h.ysb8
o.yrwrite:	mx_pub:h.ys
o.yrwrite:	mx_pub:h.ysbv
o.yrwrite:	mx_pub:h.ysfo
o.yrwrite:	mx_pub:h.ysstr
o.yrwrite:	mx_pub:h.ysm
o.yrwrite:	mx_pub:h.yslst
o.yrwrite:	mx_pub:h.yshsh
o.yrwrite:	mx_pub:h.ysc
o.yrwrite:	mx_pub:h.yse
o.yrwrite:	mx_pub:h.ystm
o.yrwrite:	mx_pub:h.ysv
o.yrwrite:	mx_pub:h.ysmsc
o.yrwrite:	mx_pub:h.ysl
o.yrwrite:	mx_pub:h.ysr
o.yrwrite:	mx_pub:h.yslog
o.yrwrite:	mx_pub:h.ysx
o.yrwrite:	mx_pub:h.ysthr
o.yrwrite:	mx_pri:h.sysxcd
o.yrwrite:	mx_pub:h.yr
o.yrwrite:	mx_pub:h.yo
o.yrwrite:	mx_pub:h.ytex
o.yrwrite:	mx_pub:h.yodefs
o.yrwrite:	mx_pub:h.yotk
o.yrwrite:	mx_pub:h.yrtypes
o.yrwrite:	mx_pub:h.mn
o.yrwrite:	mx_pub:h.mnerr
o.yrwrite:	mx_pub:h.mnsem
o.yrwrite:	mx_pub:h.mna
o.yrwrite:	mx_pub:h.mnt
o.yrwrite:	h.yr0
o.yrwrite:	mx_pri:h.sysfp
o.yrtk:	c.yrtk
o.yrtk:	mx_pri:h.sysi
o.yrtk:	mx_pri:h.sysx
o.yrtk:	mx_pri:h.sx
o.yrtk:	mx_pri:h.sysb8
o.yrtk:	mx_pub:h.ysid
o.yrtk:	mx_pub:h.ysb8
o.yrtk:	mx_pub:h.ys
o.yrtk:	mx_pub:h.ysbv
o.yrtk:	mx_pub:h.ysfo
o.yrtk:	mx_pub:h.ysstr
o.yrtk:	mx_pub:h.ysm
o.yrtk:	mx_pub:h.yslst
o.yrtk:	mx_pub:h.yshsh
o.yrtk:	mx_pub:h.ysc
o.yrtk:	mx_pub:h.yse
o.yrtk:	mx_pub:h.ystm
o.yrtk:	mx_pub:h.ysv
o.yrtk:	mx_pub:h.ysmsc
o.yrtk:	mx_pub:h.ysl
o.yrtk:	mx_pub:h.ysr
o.yrtk:	mx_pub:h.yslog
o.yrtk:	mx_pub:h.ysx
o.yrtk:	mx_pub:h.ysthr
o.yrtk:	mx_pub:h.yr
o.yrtk:	mx_pub:h.yo
o.yrtk:	mx_pub:h.ytex
o.yrtk:	mx_pub:h.yodefs
o.yrtk:	mx_pub:h.yotk
o.yrtk:	mx_pub:h.yrtypes
o.yrtk:	mx_pub:h.mn
o.yrtk:	mx_pub:h.mnerr
o.yrtk:	mx_pub:h.mnsem
o.yrtk:	mx_pub:h.mna
o.yrtk:	mx_pub:h.mnt
o.yrdump:	c.yrdump
o.yrdump:	mx_pri:h.sysi
o.yrdump:	mx_pri:h.sysx
o.yrdump:	mx_pri:h.sx
o.yrdump:	mx_pri:h.sysb8
o.yrdump:	mx_pub:h.ysid
o.yrdump:	mx_pub:h.ysb8
o.yrdump:	mx_pub:h.ys
o.yrdump:	mx_pub:h.ysbv
o.yrdump:	mx_pub:h.ysfo
o.yrdump:	mx_pub:h.ysstr
o.yrdump:	mx_pub:h.ysm
o.yrdump:	mx_pub:h.yslst
o.yrdump:	mx_pub:h.yshsh
o.yrdump:	mx_pub:h.ysc
o.yrdump:	mx_pub:h.yse
o.yrdump:	mx_pub:h.ystm
o.yrdump:	mx_pub:h.ysv
o.yrdump:	mx_pub:h.ysmsc
o.yrdump:	mx_pub:h.ysl
o.yrdump:	mx_pub:h.ysr
o.yrdump:	mx_pub:h.yslog
o.yrdump:	mx_pub:h.ysx
o.yrdump:	mx_pub:h.ysthr
o.yrdump:	mx_pub:h.yr
o.yrdump:	mx_pub:h.yo
o.yrdump:	mx_pub:h.ytex
o.yrdump:	mx_pub:h.yodefs
o.yrdump:	mx_pub:h.yotk
o.yrdump:	mx_pub:h.yrtypes
o.yrdump:	mx_pub:h.mn
o.yrdump:	mx_pub:h.mnerr
o.yrdump:	mx_pub:h.mnsem
o.yrdump:	mx_pub:h.mna
o.yrdump:	mx_pub:h.mnt
o.yrdump:	h.yr0
o.yrdump:	mx_pri:h.sysfp
o.yrdst:	c.yrdst
o.yrdst:	mx_pri:h.sysi
o.yrdst:	mx_pri:h.sysx
o.yrdst:	mx_pri:h.sx
o.yrdst:	mx_pri:h.sysb8
o.yrdst:	mx_pub:h.ysid
o.yrdst:	mx_pub:h.ysb8
o.yrdst:	mx_pub:h.ys
o.yrdst:	mx_pub:h.ysbv
o.yrdst:	mx_pub:h.ysfo
o.yrdst:	mx_pub:h.ysstr
o.yrdst:	mx_pub:h.ysm
o.yrdst:	mx_pub:h.yslst
o.yrdst:	mx_pub:h.yshsh
o.yrdst:	mx_pub:h.ysc
o.yrdst:	mx_pub:h.yse
o.yrdst:	mx_pub:h.ystm
o.yrdst:	mx_pub:h.ysv
o.yrdst:	mx_pub:h.ysmsc
o.yrdst:	mx_pub:h.ysl
o.yrdst:	mx_pub:h.ysr
o.yrdst:	mx_pub:h.yslog
o.yrdst:	mx_pub:h.ysx
o.yrdst:	mx_pub:h.ysthr
o.yrdst:	mx_pub:h.yr
o.yrdst:	mx_pub:h.yo
o.yrdst:	mx_pub:h.ytex
o.yrdst:	mx_pub:h.yodefs
o.yrdst:	mx_pub:h.yotk
o.yrdst:	mx_pub:h.yrtypes
o.yrdst:	mx_pub:h.mn
o.yrdst:	mx_pub:h.mnerr
o.yrdst:	mx_pub:h.mnsem
o.yrdst:	mx_pub:h.mna
o.yrdst:	mx_pub:h.mnt
o.yrdst:	h.yr0
o.yrdst:	mx_pri:h.sysfp
