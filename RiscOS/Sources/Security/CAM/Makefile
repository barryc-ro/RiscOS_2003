#
#         Makefile for Ant's funky new CAM module
#

# Version: 0.07
# Date   : 18-Dec-96
# Author : ant@nc.acorn.co.uk


# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = c:,TCPIPLibs:
LIBFILES  = TCPIPLibs:o.inetlibzm TCPIPLibs:o.socklibzm TCPIPLibs:o.unixlibzm





# ----------------------------------------------------------------- #
# Various Macros
# ----------------------------------------------------------------- #

# C FLAGS:
#
# FATAL_CAM - will cause the NC to lock up on authentication failure
# CHANGE_MODE - whether pretty printing issues mode 28 or not
# PRETTY_FONTS - whether pretty printing happens, or just regular output
# DEBUG_TIMEOUTS - short timeouts for soak test purposes (need soak test NCAS)
# ENABLE_DEBUGGING - enable the usual debugging system
#
# NCAS_VAR_EXCLUSIVE - if set, NCAS details come from EITHER the environment OR the smartcard,
#                    NCAS_OSVAR_LOOKUP - if set, details from environment, 
#					 else from smartcard.
#
# NCAS_VAR_EXCLUSIVE - if not set, NCAS details come from the environment, and
#		     ENV_ONLY- if set, NCAS details come only from the environment
#			       else missing details will be fetched from the s/card if possible
#

CC = cc

PRODUCT_OPTS = -DFATAL_CAM -UCHANGE_MODE -DPRETTY_FONTS -UDEBUG_TIMEOUTS -UENABLE_DEBUGGING\
		  -UNCAS_VAR_EXCLUSIVE -UENV_ONLY
	
DEBUG_OPTS = -UFATAL_CAM -UCHANGE_MODE -UPRETTY_FONTS -DDEBUG_TIMEOUTS -DENABLE_DEBUGGING\
	     -UNCAS_VAR_EXCLUSIVE -UENV_ONLY
	
BUILD_OPTS = $(PRODUCT_OPTS)

CFLAGS = -I$(INCLUDES) -DCOMPAT_INET4 -c -Throwback -depend !Depend $(BUILD_OPTS) -zM 

LINK = link
LINKFLAGS = -module

CMHG = cmhg
CMHGFLAGS =

WIPE = wipe
WIPEFLAGS = F~C~R~V

OBJASM = objasm

# ----------------------------------------------------------------- #
# Rules for making things - do not change.
# ----------------------------------------------------------------- #
	   
.SUFFIXES:	.o .c .s .cmhg
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.c.o:;		$(CC) $(CFLAGS) $<
.s.o:;		$(OBJASM) -from $< -to $@


# ----------------------------------------------------------------- #
# Targets to be compiled
# ----------------------------------------------------------------- #

TARGETS = module.o header.o vectors.o DBUG.o cam_cmds.o cam_swis.o \
	  dynalloc.o sockIO.o cam_cam.o machine.o cam_rnd.o cam_pdu.o \
	  cam_des.o cam_font.o cam_utils.o wedge.o cam_line.o



CAM:	  	$(TARGETS)       
		$(LINK) $(LINKFLAGS) -o $@ $(TARGETS) $(LIBFILES) c:o.stubs
		

clean:;		@$(WIPE) o.* $(WIPEFLAGS)
	
# ----------------------------------------------------------------- #
					   
# Dynamic dependencies:
o.DBUG:	c.DBUG
o.DBUG:	H.DBUG
o.cam_cmds:	c.cam_cmds
o.cam_cmds:	c:h.kernel
o.cam_cmds:	c:h.swis
o.cam_cmds:	h.cam_cmds
o.cam_cmds:	h.cam_cam
o.cam_cmds:	h.cam_font
o.cam_cmds:	h.cam_utils
o.cam_cmds:	h.cam_des
o.cam_cmds:	h.machine
o.cam_cmds:	h.cam_line
o.dynalloc:	c.dynalloc
o.dynalloc:	c:h.kernel
o.dynalloc:	c:h.swis
o.dynalloc:	h.dynalloc
o.dynalloc:	h.DBUG
o.cam_rnd:	c.cam_rnd
o.cam_rnd:	c:h.kernel
o.cam_rnd:	c:h.swis
o.cam_rnd:	h.cam_rnd
o.cam_des:	c.cam_des
o.cam_des:	c:h.kernel
o.cam_des:	c:h.swis
o.cam_des:	h.cam_des
o.cam_des:	h.cam_cam
o.cam_des:	h.scinter
o.cam_des:	h.module
o.cam_des:	h.DBUG
o.cam_des:	TCPIPLibs:h.inetlib
o.cam_des:	TCPIPLibs:sys.h.types
o.cam_des:	TCPIPLibs:sys.h.cdefs
o.cam_des:	TCPIPLibs:machine.h.endian
o.cam_des:	TCPIPLibs:machine.h.ansi
o.cam_des:	TCPIPLibs:machine.h.types
o.cam_des:	TCPIPLibs:netinet.h.in
o.cam_font:	c.cam_font
o.cam_font:	c:h.kernel
o.cam_font:	c:h.swis
o.cam_font:	h.cam_font
o.cam_font:	h.cam_pdu
o.cam_font:	h.dynalloc
o.cam_font:	h.DBUG
o.cam_utils:	c.cam_utils
o.cam_utils:	h.cam_utils
o.module:	c.module
o.module:	c:h.kernel
o.module:	c:h.swis
o.module:	h.module
o.module:	h.DBUG
o.module:	h.machine
o.module:	h.vectors
o.module:	h.cam_cmds
o.module:	h.cam_swis
o.module:	h.cam_cam
o.module:	h.dynalloc
o.module:	h.sockIO
o.module:	TCPIPLibs:h.netdb
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	TCPIPLibs:h.inetlib
o.module:	TCPIPLibs:sys.h.types
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	TCPIPLibs:machine.h.endian
o.module:	TCPIPLibs:machine.h.ansi
o.module:	TCPIPLibs:machine.h.types
o.module:	TCPIPLibs:netinet.h.in
o.module:	TCPIPLibs:h.socklib
o.module:	TCPIPLibs:sys.h.types
o.module:	TCPIPLibs:sys.h.socket
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	TCPIPLibs:sys.h.stat
o.module:	TCPIPLibs:sys.h.time
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	TCPIPLibs:sys.h.time
o.module:	TCPIPLibs:sys.h.uio
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	c:h.kernel
o.module:	TCPIPLibs:h.unixlib
o.module:	TCPIPLibs:sys.h.ioctl
o.module:	TCPIPLibs:sys.h.ttycom
o.module:	TCPIPLibs:sys.h.ioccom
o.module:	TCPIPLibs:sys.h.ioccom
o.module:	TCPIPLibs:sys.h.filio
o.module:	TCPIPLibs:sys.h.ioccom
o.module:	TCPIPLibs:sys.h.sockio
o.module:	TCPIPLibs:sys.h.ioccom
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	h.cam_font
o.module:	h.cam_pdu
o.module:	h.cam_des
o.module:	h.cam_line
o.vectors:	c.vectors
o.vectors:	c:h.kernel
o.vectors:	c:h.swis
o.vectors:	h.cam_cam
o.vectors:	h.machine
o.vectors:	h.vectors
o.vectors:	h.module
o.vectors:	h.DBUG
o.vectors:	h.sockIO
o.vectors:	TCPIPLibs:h.netdb
o.vectors:	TCPIPLibs:sys.h.cdefs
o.vectors:	TCPIPLibs:h.inetlib
o.vectors:	TCPIPLibs:sys.h.types
o.vectors:	TCPIPLibs:sys.h.cdefs
o.vectors:	TCPIPLibs:machine.h.endian
o.vectors:	TCPIPLibs:machine.h.ansi
o.vectors:	TCPIPLibs:machine.h.types
o.vectors:	TCPIPLibs:netinet.h.in
o.vectors:	TCPIPLibs:h.socklib
o.vectors:	TCPIPLibs:sys.h.types
o.vectors:	TCPIPLibs:sys.h.socket
o.vectors:	TCPIPLibs:sys.h.cdefs
o.vectors:	TCPIPLibs:sys.h.stat
o.vectors:	TCPIPLibs:sys.h.time
o.vectors:	TCPIPLibs:sys.h.cdefs
o.vectors:	TCPIPLibs:sys.h.cdefs
o.vectors:	TCPIPLibs:sys.h.time
o.vectors:	TCPIPLibs:sys.h.uio
o.vectors:	TCPIPLibs:sys.h.cdefs
o.vectors:	c:h.kernel
o.vectors:	TCPIPLibs:h.unixlib
o.vectors:	TCPIPLibs:sys.h.ioctl
o.vectors:	TCPIPLibs:sys.h.ttycom
o.vectors:	TCPIPLibs:sys.h.ioccom
o.vectors:	TCPIPLibs:sys.h.ioccom
o.vectors:	TCPIPLibs:sys.h.filio
o.vectors:	TCPIPLibs:sys.h.ioccom
o.vectors:	TCPIPLibs:sys.h.sockio
o.vectors:	TCPIPLibs:sys.h.ioccom
o.vectors:	TCPIPLibs:sys.h.cdefs
o.vectors:	h.scinter
o.cam_swis:	c.cam_swis
o.cam_swis:	c:h.kernel
o.cam_swis:	c:h.swis
o.cam_swis:	h.cam_swis
o.cam_swis:	h.cam_cam
o.cam_swis:	h.dynalloc
o.cam_swis:	h.sockIO
o.cam_swis:	TCPIPLibs:h.netdb
o.cam_swis:	TCPIPLibs:sys.h.cdefs
o.cam_swis:	TCPIPLibs:h.inetlib
o.cam_swis:	TCPIPLibs:sys.h.types
o.cam_swis:	TCPIPLibs:sys.h.cdefs
o.cam_swis:	TCPIPLibs:machine.h.endian
o.cam_swis:	TCPIPLibs:machine.h.ansi
o.cam_swis:	TCPIPLibs:machine.h.types
o.cam_swis:	TCPIPLibs:netinet.h.in
o.cam_swis:	TCPIPLibs:h.socklib
o.cam_swis:	TCPIPLibs:sys.h.types
o.cam_swis:	TCPIPLibs:sys.h.socket
o.cam_swis:	TCPIPLibs:sys.h.cdefs
o.cam_swis:	TCPIPLibs:sys.h.stat
o.cam_swis:	TCPIPLibs:sys.h.time
o.cam_swis:	TCPIPLibs:sys.h.cdefs
o.cam_swis:	TCPIPLibs:sys.h.cdefs
o.cam_swis:	TCPIPLibs:sys.h.time
o.cam_swis:	TCPIPLibs:sys.h.uio
o.cam_swis:	TCPIPLibs:sys.h.cdefs
o.cam_swis:	c:h.kernel
o.cam_swis:	TCPIPLibs:h.unixlib
o.cam_swis:	TCPIPLibs:sys.h.ioctl
o.cam_swis:	TCPIPLibs:sys.h.ttycom
o.cam_swis:	TCPIPLibs:sys.h.ioccom
o.cam_swis:	TCPIPLibs:sys.h.ioccom
o.cam_swis:	TCPIPLibs:sys.h.filio
o.cam_swis:	TCPIPLibs:sys.h.ioccom
o.cam_swis:	TCPIPLibs:sys.h.sockio
o.cam_swis:	TCPIPLibs:sys.h.ioccom
o.cam_swis:	TCPIPLibs:sys.h.cdefs
o.cam_swis:	h.module
o.cam_swis:	h.DBUG
o.sockIO:	c.sockIO
o.sockIO:	c:h.kernel
o.sockIO:	c:h.swis
o.sockIO:	h.module
o.sockIO:	h.sockIO
o.sockIO:	TCPIPLibs:h.netdb
o.sockIO:	TCPIPLibs:sys.h.cdefs
o.sockIO:	TCPIPLibs:h.inetlib
o.sockIO:	TCPIPLibs:sys.h.types
o.sockIO:	TCPIPLibs:sys.h.cdefs
o.sockIO:	TCPIPLibs:machine.h.endian
o.sockIO:	TCPIPLibs:machine.h.ansi
o.sockIO:	TCPIPLibs:machine.h.types
o.sockIO:	TCPIPLibs:netinet.h.in
o.sockIO:	TCPIPLibs:h.socklib
o.sockIO:	TCPIPLibs:sys.h.types
o.sockIO:	TCPIPLibs:sys.h.socket
o.sockIO:	TCPIPLibs:sys.h.cdefs
o.sockIO:	TCPIPLibs:sys.h.stat
o.sockIO:	TCPIPLibs:sys.h.time
o.sockIO:	TCPIPLibs:sys.h.cdefs
o.sockIO:	TCPIPLibs:sys.h.cdefs
o.sockIO:	TCPIPLibs:sys.h.time
o.sockIO:	TCPIPLibs:sys.h.uio
o.sockIO:	TCPIPLibs:sys.h.cdefs
o.sockIO:	c:h.kernel
o.sockIO:	TCPIPLibs:h.unixlib
o.sockIO:	TCPIPLibs:sys.h.ioctl
o.sockIO:	TCPIPLibs:sys.h.ttycom
o.sockIO:	TCPIPLibs:sys.h.ioccom
o.sockIO:	TCPIPLibs:sys.h.ioccom
o.sockIO:	TCPIPLibs:sys.h.filio
o.sockIO:	TCPIPLibs:sys.h.ioccom
o.sockIO:	TCPIPLibs:sys.h.sockio
o.sockIO:	TCPIPLibs:sys.h.ioccom
o.sockIO:	TCPIPLibs:sys.h.cdefs
o.sockIO:	h.DBUG
o.cam_cam:	c.cam_cam
o.cam_cam:	TCPIPLibs:h.inetlib
o.cam_cam:	TCPIPLibs:sys.h.types
o.cam_cam:	TCPIPLibs:sys.h.cdefs
o.cam_cam:	TCPIPLibs:machine.h.endian
o.cam_cam:	TCPIPLibs:machine.h.ansi
o.cam_cam:	TCPIPLibs:machine.h.types
o.cam_cam:	TCPIPLibs:netinet.h.in
o.cam_cam:	h.cam_cam
o.cam_cam:	h.dynalloc
o.cam_cam:	h.sockIO
o.cam_cam:	TCPIPLibs:h.netdb
o.cam_cam:	TCPIPLibs:sys.h.cdefs
o.cam_cam:	TCPIPLibs:h.inetlib
o.cam_cam:	TCPIPLibs:sys.h.types
o.cam_cam:	TCPIPLibs:netinet.h.in
o.cam_cam:	TCPIPLibs:h.socklib
o.cam_cam:	TCPIPLibs:sys.h.types
o.cam_cam:	TCPIPLibs:sys.h.socket
o.cam_cam:	TCPIPLibs:sys.h.cdefs
o.cam_cam:	TCPIPLibs:sys.h.stat
o.cam_cam:	TCPIPLibs:sys.h.time
o.cam_cam:	TCPIPLibs:sys.h.cdefs
o.cam_cam:	TCPIPLibs:sys.h.cdefs
o.cam_cam:	TCPIPLibs:sys.h.time
o.cam_cam:	TCPIPLibs:sys.h.uio
o.cam_cam:	TCPIPLibs:sys.h.cdefs
o.cam_cam:	c:h.kernel
o.cam_cam:	TCPIPLibs:h.unixlib
o.cam_cam:	TCPIPLibs:sys.h.ioctl
o.cam_cam:	TCPIPLibs:sys.h.ttycom
o.cam_cam:	TCPIPLibs:sys.h.ioccom
o.cam_cam:	TCPIPLibs:sys.h.ioccom
o.cam_cam:	TCPIPLibs:sys.h.filio
o.cam_cam:	TCPIPLibs:sys.h.ioccom
o.cam_cam:	TCPIPLibs:sys.h.sockio
o.cam_cam:	TCPIPLibs:sys.h.ioccom
o.cam_cam:	TCPIPLibs:sys.h.cdefs
o.cam_cam:	h.module
o.cam_cam:	h.DBUG
o.cam_cam:	c:h.kernel
o.cam_cam:	c:h.swis
o.cam_cam:	h.machine
o.cam_cam:	h.vectors
o.machine:	c.machine
o.machine:	c:h.kernel
o.machine:	c:h.swis
o.machine:	h.machine
o.machine:	h.module
o.machine:	h.cam_cam
o.machine:	h.cam_rnd
o.machine:	h.cam_pdu
o.machine:	h.cam_des
o.machine:	h.sockIO
o.machine:	TCPIPLibs:h.netdb
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	TCPIPLibs:h.inetlib
o.machine:	TCPIPLibs:sys.h.types
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	TCPIPLibs:machine.h.endian
o.machine:	TCPIPLibs:machine.h.ansi
o.machine:	TCPIPLibs:machine.h.types
o.machine:	TCPIPLibs:netinet.h.in
o.machine:	TCPIPLibs:h.socklib
o.machine:	TCPIPLibs:sys.h.types
o.machine:	TCPIPLibs:sys.h.socket
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	TCPIPLibs:sys.h.stat
o.machine:	TCPIPLibs:sys.h.time
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	TCPIPLibs:sys.h.time
o.machine:	TCPIPLibs:sys.h.uio
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	c:h.kernel
o.machine:	TCPIPLibs:h.unixlib
o.machine:	TCPIPLibs:sys.h.ioctl
o.machine:	TCPIPLibs:sys.h.ttycom
o.machine:	TCPIPLibs:sys.h.ioccom
o.machine:	TCPIPLibs:sys.h.ioccom
o.machine:	TCPIPLibs:sys.h.filio
o.machine:	TCPIPLibs:sys.h.ioccom
o.machine:	TCPIPLibs:sys.h.sockio
o.machine:	TCPIPLibs:sys.h.ioccom
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	h.vectors
o.machine:	h.cam_font
o.machine:	h.cam_line
o.machine:	h.DBUG
o.cam_pdu:	c.cam_pdu
o.cam_pdu:	h.cam_pdu
o.cam_pdu:	h.sockIO
o.cam_pdu:	TCPIPLibs:h.netdb
o.cam_pdu:	TCPIPLibs:sys.h.cdefs
o.cam_pdu:	TCPIPLibs:h.inetlib
o.cam_pdu:	TCPIPLibs:sys.h.types
o.cam_pdu:	TCPIPLibs:sys.h.cdefs
o.cam_pdu:	TCPIPLibs:machine.h.endian
o.cam_pdu:	TCPIPLibs:machine.h.ansi
o.cam_pdu:	TCPIPLibs:machine.h.types
o.cam_pdu:	TCPIPLibs:netinet.h.in
o.cam_pdu:	TCPIPLibs:h.socklib
o.cam_pdu:	TCPIPLibs:sys.h.types
o.cam_pdu:	TCPIPLibs:sys.h.socket
o.cam_pdu:	TCPIPLibs:sys.h.cdefs
o.cam_pdu:	TCPIPLibs:sys.h.stat
o.cam_pdu:	TCPIPLibs:sys.h.time
o.cam_pdu:	TCPIPLibs:sys.h.cdefs
o.cam_pdu:	TCPIPLibs:sys.h.cdefs
o.cam_pdu:	TCPIPLibs:sys.h.time
o.cam_pdu:	TCPIPLibs:sys.h.uio
o.cam_pdu:	TCPIPLibs:sys.h.cdefs
o.cam_pdu:	c:h.kernel
o.cam_pdu:	TCPIPLibs:h.unixlib
o.cam_pdu:	TCPIPLibs:sys.h.ioctl
o.cam_pdu:	TCPIPLibs:sys.h.ttycom
o.cam_pdu:	TCPIPLibs:sys.h.ioccom
o.cam_pdu:	TCPIPLibs:sys.h.ioccom
o.cam_pdu:	TCPIPLibs:sys.h.filio
o.cam_pdu:	TCPIPLibs:sys.h.ioccom
o.cam_pdu:	TCPIPLibs:sys.h.sockio
o.cam_pdu:	TCPIPLibs:sys.h.ioccom
o.cam_pdu:	TCPIPLibs:sys.h.cdefs
o.cam_pdu:	h.cam_cam
o.cam_pdu:	h.DBUG
o.cam_line:	c.cam_line
o.cam_line:	c:h.swis
o.cam_line:	c:h.kernel
o.cam_line:	c:h.kernel
o.cam_line:	h.cam_line
o.cam_line:	h.machine
o.cam_line:	h.vectors
o.cam_line:	h.sockIO
o.cam_line:	TCPIPLibs:h.netdb
o.cam_line:	TCPIPLibs:sys.h.cdefs
o.cam_line:	TCPIPLibs:h.inetlib
o.cam_line:	TCPIPLibs:sys.h.types
o.cam_line:	TCPIPLibs:sys.h.cdefs
o.cam_line:	TCPIPLibs:machine.h.endian
o.cam_line:	TCPIPLibs:machine.h.ansi
o.cam_line:	TCPIPLibs:machine.h.types
o.cam_line:	TCPIPLibs:netinet.h.in
o.cam_line:	TCPIPLibs:h.socklib
o.cam_line:	TCPIPLibs:sys.h.types
o.cam_line:	TCPIPLibs:sys.h.socket
o.cam_line:	TCPIPLibs:sys.h.cdefs
o.cam_line:	TCPIPLibs:sys.h.stat
o.cam_line:	TCPIPLibs:sys.h.time
o.cam_line:	TCPIPLibs:sys.h.cdefs
o.cam_line:	TCPIPLibs:sys.h.cdefs
o.cam_line:	TCPIPLibs:sys.h.time
o.cam_line:	TCPIPLibs:sys.h.uio
o.cam_line:	TCPIPLibs:sys.h.cdefs
o.cam_line:	c:h.kernel
o.cam_line:	TCPIPLibs:h.unixlib
o.cam_line:	TCPIPLibs:sys.h.ioctl
o.cam_line:	TCPIPLibs:sys.h.ttycom
o.cam_line:	TCPIPLibs:sys.h.ioccom
o.cam_line:	TCPIPLibs:sys.h.ioccom
o.cam_line:	TCPIPLibs:sys.h.filio
o.cam_line:	TCPIPLibs:sys.h.ioccom
o.cam_line:	TCPIPLibs:sys.h.sockio
o.cam_line:	TCPIPLibs:sys.h.ioccom
o.cam_line:	TCPIPLibs:sys.h.cdefs
o.cam_line:	h.cam_cam
o.cam_line:	h.dynalloc
o.cam_line:	h.DBUG
