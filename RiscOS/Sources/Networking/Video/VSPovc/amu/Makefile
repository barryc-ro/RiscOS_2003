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
#
# USE_MN_CALLBACKS - forces module to define a MediaNet callback routine
#
# SILENT_NETWORK_ERRORS - stops serious network errors from being reported on
#                         screen

VPATH=@ rts.src.vid

INCLUDES = c:
LIBFILES =
LINKFLAGS=-rmf

CMHGFLAGS=-p


CC = cc
CMHG=cmhg
OBJASM=objasm

CFLAGS = -c -depend !Depend -zM -ffa -wp -throwback -DRISCOS -DSYSTEM -I$(INCLUDES) -DCOMPAT_INET4 -DNDEBUG -DSILENT_NETWORK_ERRORS



INCLUDES=C:,mx_pri:,mx_pub:,mx_inc:,rts:,TCPIPlibs:

IDLLIB= @.fromidl.lib.idllib
YOLIB= @.mx.src.yo.lib.yolib
MNLIB=@.mx.src.mn.lib.mnlib
MTLIB=@.mx.src.mt.lib.mtlib
YSLIB=@.mx.src.ys.lib.yslib
YRLIB=@.mx.src.yr.lib.yrlib

.SUFFIXES:	.o .c .cmhg
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.c.o:;		$(CC) $(CFLAGS) $<
.s.o:;		$(OBJASM) -from $< -to $@

OBJS=o.module o.vspc o.smnudp o.mzswrap o.mzscl o.mzsclii o.umdn o.vsphdr o.debug

VSPovc:  $(OBJS) $(MNLIB) $(IDLLIB) $(MTLIB) ${YOLIB} ${YSLIB} ${YRLIB}
         link $(LINKFLAGS) -o $@ $(OBJS) $(MNLIB) $(MTLIB) ${YRLIB} ${YOLIB} ${YSLIB} ${IDLLIB} clib:o.stubs tcpiplibs:o.inetlibzm tcpiplibs:o.socklibzm
         @echo ***********************************
         @echo * VSPovc constructed successfully *
         @echo ***********************************


h.cmhg:  amu.makefile cmhg.vsphdr
         cmhg -p -d h.cmhg cmhg.VSPhdr

# Dynamic dependencies:
o.mzscl:	rts.src.vid.c.mzscl
o.mzscl:	mx_pri:h.sysi
o.mzscl:	mx_pri:h.sysx
o.mzscl:	mx_pri:h.sx
o.mzscl:	mx_pri:h.sysb8
o.mzscl:	mx_pub:h.ysid
o.mzscl:	mx_pub:h.ysb8
o.mzscl:	mx_pub:h.ys
o.mzscl:	mx_pub:h.ysbv
o.mzscl:	mx_pub:h.ysfo
o.mzscl:	mx_pub:h.ysstr
o.mzscl:	mx_pub:h.ysm
o.mzscl:	mx_pub:h.yslst
o.mzscl:	mx_pub:h.yshsh
o.mzscl:	mx_pub:h.ysc
o.mzscl:	mx_pub:h.yse
o.mzscl:	mx_pub:h.ystm
o.mzscl:	mx_pub:h.ysv
o.mzscl:	mx_pub:h.ysmsc
o.mzscl:	mx_pub:h.ysl
o.mzscl:	mx_pub:h.ysr
o.mzscl:	mx_pub:h.yslog
o.mzscl:	mx_pub:h.ysx
o.mzscl:	mx_pub:h.ysthr
o.mzscl:	mx_pub:h.yo
o.mzscl:	mx_pub:h.ytex
o.mzscl:	mx_pub:h.yodefs
o.mzscl:	mx_pub:h.yotk
o.mzscl:	mx_pub:h.yrtypes
o.mzscl:	mx_pub:h.mn
o.mzscl:	mx_pub:h.mnerr
o.mzscl:	mx_pub:h.mnsem
o.mzscl:	mx_pub:h.mna
o.mzscl:	mx_pub:h.mnt
o.mzscl:	mx_pub:h.yocoa
o.mzscl:	mx_pub:h.mzscli
o.mzscl:	mx_pub:h.mzs
o.mzscl:	mx_inc:h.mzsclii
o.mzscl:	rts:h.mzscl
o.mzscl:	mx_pub:h.mzscli
o.mzscl:	mx_pub:h.mzs
o.mzsclii:	rts.src.vid.c.mzsclii
o.mzsclii:	mx_pri:h.sysi
o.mzsclii:	mx_pri:h.sysx
o.mzsclii:	mx_pri:h.sx
o.mzsclii:	mx_pri:h.sysb8
o.mzsclii:	mx_pub:h.ysid
o.mzsclii:	mx_pub:h.ysb8
o.mzsclii:	mx_pub:h.ys
o.mzsclii:	mx_pub:h.ysbv
o.mzsclii:	mx_pub:h.ysfo
o.mzsclii:	mx_pub:h.ysstr
o.mzsclii:	mx_pub:h.ysm
o.mzsclii:	mx_pub:h.yslst
o.mzsclii:	mx_pub:h.yshsh
o.mzsclii:	mx_pub:h.ysc
o.mzsclii:	mx_pub:h.yse
o.mzsclii:	mx_pub:h.ystm
o.mzsclii:	mx_pub:h.ysv
o.mzsclii:	mx_pub:h.ysmsc
o.mzsclii:	mx_pub:h.ysl
o.mzsclii:	mx_pub:h.ysr
o.mzsclii:	mx_pub:h.yslog
o.mzsclii:	mx_pub:h.ysx
o.mzsclii:	mx_pub:h.ysthr
o.mzsclii:	mx_pub:h.yo
o.mzsclii:	mx_pub:h.ytex
o.mzsclii:	mx_pub:h.yodefs
o.mzsclii:	mx_pub:h.yotk
o.mzsclii:	mx_pub:h.yrtypes
o.mzsclii:	mx_pub:h.mn
o.mzsclii:	mx_pub:h.mnerr
o.mzsclii:	mx_pub:h.mnsem
o.mzsclii:	mx_pub:h.mna
o.mzsclii:	mx_pub:h.mnt
o.mzsclii:	mx_pub:h.yocoa
o.mzsclii:	rts:h.mzscl
o.mzsclii:	mx_pub:h.mzscli
o.mzsclii:	mx_pub:h.mzs
o.mzsclii:	mx_inc:h.mzsclii
o.debug:	c.debug
o.debug:	C:h.kernel
o.debug:	C:h.swis
o.vspc:	c.vspc
o.vspc:	h.debug
o.vspc:	C:h.kernel
o.vspc:	TCPIPlibs:sys.h.types
o.vspc:	TCPIPlibs:sys.h.cdefs
o.vspc:	TCPIPlibs:machine.h.endian
o.vspc:	TCPIPlibs:machine.h.ansi
o.vspc:	TCPIPlibs:machine.h.types
o.vspc:	C:h.kernel
o.vspc:	C:h.swis
o.vspc:	h.vsp
o.vspc:	h.misc
o.vspc:	h.vspc
o.vspc:	h.mzswrap
o.vspc:	h.module
o.mzswrap:	c.mzswrap
o.mzswrap:	mx_pri:h.sysi
o.mzswrap:	mx_pri:h.sysx
o.mzswrap:	mx_pri:h.sx
o.mzswrap:	mx_pri:h.sysb8
o.mzswrap:	mx_pub:h.ysid
o.mzswrap:	mx_pub:h.ysb8
o.mzswrap:	mx_pub:h.ys
o.mzswrap:	mx_pub:h.ysbv
o.mzswrap:	mx_pub:h.ysfo
o.mzswrap:	mx_pub:h.ysstr
o.mzswrap:	mx_pub:h.ysm
o.mzswrap:	mx_pub:h.yslst
o.mzswrap:	mx_pub:h.yshsh
o.mzswrap:	mx_pub:h.ysc
o.mzswrap:	mx_pub:h.yse
o.mzswrap:	mx_pub:h.ystm
o.mzswrap:	mx_pub:h.ysv
o.mzswrap:	mx_pub:h.ysmsc
o.mzswrap:	mx_pub:h.ysl
o.mzswrap:	mx_pub:h.ysr
o.mzswrap:	mx_pub:h.yslog
o.mzswrap:	mx_pub:h.ysx
o.mzswrap:	mx_pub:h.ysthr
o.mzswrap:	mx_pub:h.mzcch
o.mzswrap:	mx_pub:h.yo
o.mzswrap:	mx_pub:h.ytex
o.mzswrap:	mx_pub:h.yodefs
o.mzswrap:	mx_pub:h.yotk
o.mzswrap:	mx_pub:h.yrtypes
o.mzswrap:	mx_pub:h.mn
o.mzswrap:	mx_pub:h.mnerr
o.mzswrap:	mx_pub:h.mnsem
o.mzswrap:	mx_pub:h.mna
o.mzswrap:	mx_pub:h.mnt
o.mzswrap:	mx_pub:h.mzc
o.mzswrap:	mx_pub:h.mzz
o.mzswrap:	mx_pub:h.mkd
o.mzswrap:	rts:h.mkdc
o.mzswrap:	mx_pub:h.mkd
o.mzswrap:	mx_pub:h.mzalgctn
o.mzswrap:	mx_pub:h.mzs
o.mzswrap:	rts:h.mzscl
o.mzswrap:	mx_pub:h.mzscli
o.mzswrap:	mx_pub:h.mzs
o.mzswrap:	mx_pub:h.mkd
o.mzswrap:	h.vsp
o.mzswrap:	h.misc
o.mzswrap:	C:h.kernel
o.mzswrap:	C:h.swis
o.mzswrap:	h.mpegctrl
o.mzswrap:	h.mzswrap
o.mzswrap:	h.vspc
o.mzswrap:	h.module
o.mzswrap:	h.debug
o.module:	c.module
o.module:	h.debug
o.module:	C:h.kernel
o.module:	TCPIPlibs:sys.h.types
o.module:	TCPIPlibs:sys.h.cdefs
o.module:	TCPIPlibs:machine.h.endian
o.module:	TCPIPlibs:machine.h.ansi
o.module:	TCPIPlibs:machine.h.types
o.module:	h.vsp
o.module:	TCPIPlibs:sys.h.socket
o.module:	TCPIPlibs:sys.h.cdefs
o.module:	TCPIPlibs:h.socklib
o.module:	TCPIPlibs:sys.h.types
o.module:	TCPIPlibs:sys.h.socket
o.module:	TCPIPlibs:sys.h.stat
o.module:	TCPIPlibs:sys.h.time
o.module:	TCPIPlibs:sys.h.cdefs
o.module:	TCPIPlibs:sys.h.cdefs
o.module:	TCPIPlibs:sys.h.time
o.module:	TCPIPlibs:sys.h.uio
o.module:	TCPIPlibs:sys.h.cdefs
o.module:	C:h.kernel
o.module:	TCPIPlibs:sys.h.ioctl
o.module:	TCPIPlibs:sys.h.ttycom
o.module:	TCPIPlibs:sys.h.ioccom
o.module:	TCPIPlibs:sys.h.ioccom
o.module:	TCPIPlibs:sys.h.filio
o.module:	TCPIPlibs:sys.h.ioccom
o.module:	TCPIPlibs:sys.h.sockio
o.module:	TCPIPlibs:sys.h.ioccom
o.module:	TCPIPlibs:sys.h.cdefs
o.module:	TCPIPlibs:netinet.h.in
o.module:	TCPIPlibs:h.inetlib
o.module:	TCPIPlibs:sys.h.types
o.module:	TCPIPlibs:netinet.h.in
o.module:	TCPIPlibs:net.h.if
o.module:	TCPIPlibs:sys.h.socket
o.module:	TCPIPlibs:sys.h.time
o.module:	TCPIPlibs:net.h.if_arp
o.module:	TCPIPlibs:sys.h.socket
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.misc
o.module:	h.mpegctrl
o.module:	h.cmhg
o.module:	h.module
o.module:	h.vspc
o.module:	h.mzswrap
o.smnudp:	c.smnudp
o.smnudp:	C:h.kernel
o.smnudp:	C:h.swis
o.smnudp:	C:h.errno
o.smnudp:	TCPIPlibs:sys.h.fcntl
o.smnudp:	TCPIPlibs:sys.h.types
o.smnudp:	TCPIPlibs:sys.h.cdefs
o.smnudp:	TCPIPlibs:machine.h.endian
o.smnudp:	TCPIPlibs:machine.h.ansi
o.smnudp:	TCPIPlibs:machine.h.types
o.smnudp:	TCPIPlibs:sys.h.cdefs
o.smnudp:	TCPIPlibs:sys.h.types
o.smnudp:	TCPIPlibs:sys.h.time
o.smnudp:	TCPIPlibs:sys.h.cdefs
o.smnudp:	TCPIPlibs:sys.h.socket
o.smnudp:	TCPIPlibs:sys.h.cdefs
o.smnudp:	TCPIPlibs:h.socklib
o.smnudp:	TCPIPlibs:sys.h.types
o.smnudp:	TCPIPlibs:sys.h.socket
o.smnudp:	TCPIPlibs:sys.h.stat
o.smnudp:	TCPIPlibs:sys.h.time
o.smnudp:	TCPIPlibs:sys.h.cdefs
o.smnudp:	TCPIPlibs:sys.h.time
o.smnudp:	TCPIPlibs:sys.h.uio
o.smnudp:	TCPIPlibs:sys.h.cdefs
o.smnudp:	C:h.kernel
o.smnudp:	TCPIPlibs:h.netdb
o.smnudp:	TCPIPlibs:sys.h.cdefs
o.smnudp:	TCPIPlibs:netinet.h.in
o.smnudp:	TCPIPlibs:h.inetlib
o.smnudp:	TCPIPlibs:sys.h.types
o.smnudp:	TCPIPlibs:netinet.h.in
o.smnudp:	TCPIPlibs:sys.h.ioctl
o.smnudp:	TCPIPlibs:sys.h.ttycom
o.smnudp:	TCPIPlibs:sys.h.ioccom
o.smnudp:	TCPIPlibs:sys.h.ioccom
o.smnudp:	TCPIPlibs:sys.h.filio
o.smnudp:	TCPIPlibs:sys.h.ioccom
o.smnudp:	TCPIPlibs:sys.h.sockio
o.smnudp:	TCPIPlibs:sys.h.ioccom
o.smnudp:	TCPIPlibs:sys.h.cdefs
o.smnudp:	mx_pri:h.sx
o.smnudp:	mx_pub:h.mn
o.smnudp:	mx_pri:h.sysx
o.smnudp:	mx_pri:h.sysb8
o.smnudp:	mx_pub:h.ysid
o.smnudp:	mx_pub:h.ysb8
o.smnudp:	mx_pub:h.mnerr
o.smnudp:	mx_pub:h.mnsem
o.smnudp:	mx_pub:h.mna
o.smnudp:	mx_pub:h.mnt
o.smnudp:	mx_pub:h.mnnio
o.smnudp:	mx_pri:h.smn
o.smnudp:	h.smnudp
o.smnudp:	h.udpNIO
o.smnudp:	h.rosevents
o.smnudp:	h.options
o.smnudp:	h.module
