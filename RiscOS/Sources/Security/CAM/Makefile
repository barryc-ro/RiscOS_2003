#
#         Makefile for Ant's funky new CAM module
#

# Version: 0.07
# Date   : 18-Dec-96
# Author : ant@nc.acorn.co.uk


# ----------------------------------------------------------------- #
# Include files and Lib files
# ----------------------------------------------------------------- #

INCLUDES = c:
LIBFILES = c:^.libs.o.inetlibzm \
           c:^.libs.o.socklibzm \
           c:^.libs.o.unixlibzm




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

CFLAGS = -I$(INCLUDES) -c -Throwback -depend !Depend $(BUILD_OPTS) -zM 

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
o.module:	c:h.netdb
o.module:	c:h.inetlib
o.module:	c:sys.h.types
o.module:	c:netinet.h.in
o.module:	c:h.socklib
o.module:	c:sys.h.types
o.module:	c:sys.h.socket
o.module:	c:sys.h.stat
o.module:	c:sys.h.time
o.module:	c:sys.h.time
o.module:	c:sys.h.uio
o.module:	c:h.kernel
o.module:	c:h.unixlib
o.module:	c:sys.h.ioctl
o.module:	c:sys.h.ttychars
o.module:	c:sys.h.ttydev
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
o.vectors:	c:h.netdb
o.vectors:	c:h.inetlib
o.vectors:	c:sys.h.types
o.vectors:	c:netinet.h.in
o.vectors:	c:h.socklib
o.vectors:	c:sys.h.types
o.vectors:	c:sys.h.socket
o.vectors:	c:sys.h.stat
o.vectors:	c:sys.h.time
o.vectors:	c:sys.h.time
o.vectors:	c:sys.h.uio
o.vectors:	c:h.kernel
o.vectors:	c:h.unixlib
o.vectors:	c:sys.h.ioctl
o.vectors:	c:sys.h.ttychars
o.vectors:	c:sys.h.ttydev
o.vectors:	h.scinter
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
o.cam_swis:	c.cam_swis
o.cam_swis:	c:h.kernel
o.cam_swis:	c:h.swis
o.cam_swis:	h.cam_swis
o.cam_swis:	h.cam_cam
o.cam_swis:	h.dynalloc
o.cam_swis:	h.sockIO
o.cam_swis:	c:h.netdb
o.cam_swis:	c:h.inetlib
o.cam_swis:	c:sys.h.types
o.cam_swis:	c:netinet.h.in
o.cam_swis:	c:h.socklib
o.cam_swis:	c:sys.h.types
o.cam_swis:	c:sys.h.socket
o.cam_swis:	c:sys.h.stat
o.cam_swis:	c:sys.h.time
o.cam_swis:	c:sys.h.time
o.cam_swis:	c:sys.h.uio
o.cam_swis:	c:h.kernel
o.cam_swis:	c:h.unixlib
o.cam_swis:	c:sys.h.ioctl
o.cam_swis:	c:sys.h.ttychars
o.cam_swis:	c:sys.h.ttydev
o.cam_swis:	h.module
o.cam_swis:	h.DBUG
o.dynalloc:	c.dynalloc
o.dynalloc:	c:h.kernel
o.dynalloc:	c:h.swis
o.dynalloc:	h.dynalloc
o.dynalloc:	h.DBUG
o.sockIO:	c.sockIO
o.sockIO:	c:h.kernel
o.sockIO:	c:h.swis
o.sockIO:	h.module
o.sockIO:	h.sockIO
o.sockIO:	c:h.netdb
o.sockIO:	c:h.inetlib
o.sockIO:	c:sys.h.types
o.sockIO:	c:netinet.h.in
o.sockIO:	c:h.socklib
o.sockIO:	c:sys.h.types
o.sockIO:	c:sys.h.socket
o.sockIO:	c:sys.h.stat
o.sockIO:	c:sys.h.time
o.sockIO:	c:sys.h.time
o.sockIO:	c:sys.h.uio
o.sockIO:	c:h.kernel
o.sockIO:	c:h.unixlib
o.sockIO:	c:sys.h.ioctl
o.sockIO:	c:sys.h.ttychars
o.sockIO:	c:sys.h.ttydev
o.sockIO:	h.DBUG
o.cam_cam:	c.cam_cam
o.cam_cam:	c:h.inetlib
o.cam_cam:	c:sys.h.types
o.cam_cam:	c:netinet.h.in
o.cam_cam:	h.cam_cam
o.cam_cam:	h.dynalloc
o.cam_cam:	h.sockIO
o.cam_cam:	c:h.netdb
o.cam_cam:	c:h.inetlib
o.cam_cam:	c:sys.h.types
o.cam_cam:	c:netinet.h.in
o.cam_cam:	c:h.socklib
o.cam_cam:	c:sys.h.types
o.cam_cam:	c:sys.h.socket
o.cam_cam:	c:sys.h.stat
o.cam_cam:	c:sys.h.time
o.cam_cam:	c:sys.h.time
o.cam_cam:	c:sys.h.uio
o.cam_cam:	c:h.kernel
o.cam_cam:	c:h.unixlib
o.cam_cam:	c:sys.h.ioctl
o.cam_cam:	c:sys.h.ttychars
o.cam_cam:	c:sys.h.ttydev
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
o.machine:	c:h.netdb
o.machine:	c:h.inetlib
o.machine:	c:sys.h.types
o.machine:	c:netinet.h.in
o.machine:	c:h.socklib
o.machine:	c:sys.h.types
o.machine:	c:sys.h.socket
o.machine:	c:sys.h.stat
o.machine:	c:sys.h.time
o.machine:	c:sys.h.time
o.machine:	c:sys.h.uio
o.machine:	c:h.kernel
o.machine:	c:h.unixlib
o.machine:	c:sys.h.ioctl
o.machine:	c:sys.h.ttychars
o.machine:	c:sys.h.ttydev
o.machine:	h.vectors
o.machine:	h.cam_font
o.machine:	h.cam_line
o.machine:	h.DBUG
o.cam_rnd:	c.cam_rnd
o.cam_rnd:	c:h.kernel
o.cam_rnd:	c:h.swis
o.cam_rnd:	h.cam_rnd
o.cam_pdu:	c.cam_pdu
o.cam_pdu:	h.cam_pdu
o.cam_pdu:	h.sockIO
o.cam_pdu:	c:h.netdb
o.cam_pdu:	c:h.inetlib
o.cam_pdu:	c:sys.h.types
o.cam_pdu:	c:netinet.h.in
o.cam_pdu:	c:h.socklib
o.cam_pdu:	c:sys.h.types
o.cam_pdu:	c:sys.h.socket
o.cam_pdu:	c:sys.h.stat
o.cam_pdu:	c:sys.h.time
o.cam_pdu:	c:sys.h.time
o.cam_pdu:	c:sys.h.uio
o.cam_pdu:	c:h.kernel
o.cam_pdu:	c:h.unixlib
o.cam_pdu:	c:sys.h.ioctl
o.cam_pdu:	c:sys.h.ttychars
o.cam_pdu:	c:sys.h.ttydev
o.cam_pdu:	h.cam_cam
o.cam_pdu:	h.DBUG
o.cam_des:	c.cam_des
o.cam_des:	c:h.kernel
o.cam_des:	c:h.swis
o.cam_des:	h.cam_des
o.cam_des:	h.cam_cam
o.cam_des:	h.scinter
o.cam_des:	h.module
o.cam_des:	h.DBUG
o.cam_des:	c:h.inetlib
o.cam_des:	c:sys.h.types
o.cam_des:	c:netinet.h.in
o.cam_font:	c.cam_font
o.cam_font:	c:h.kernel
o.cam_font:	c:h.swis
o.cam_font:	h.cam_font
o.cam_font:	h.cam_pdu
o.cam_font:	h.dynalloc
o.cam_font:	h.DBUG
o.cam_utils:	c.cam_utils
o.cam_utils:	h.cam_utils
o.cam_line:	c.cam_line
o.cam_line:	c:h.swis
o.cam_line:	c:h.kernel
o.cam_line:	c:h.kernel
o.cam_line:	h.cam_line
o.cam_line:	h.machine
o.cam_line:	h.vectors
o.cam_line:	h.sockIO
o.cam_line:	c:h.netdb
o.cam_line:	c:h.inetlib
o.cam_line:	c:sys.h.types
o.cam_line:	c:netinet.h.in
o.cam_line:	c:h.socklib
o.cam_line:	c:sys.h.types
o.cam_line:	c:sys.h.socket
o.cam_line:	c:sys.h.stat
o.cam_line:	c:sys.h.time
o.cam_line:	c:sys.h.time
o.cam_line:	c:sys.h.uio
o.cam_line:	c:h.kernel
o.cam_line:	c:h.unixlib
o.cam_line:	c:sys.h.ioctl
o.cam_line:	c:sys.h.ttychars
o.cam_line:	c:sys.h.ttydev
o.cam_line:	h.cam_cam
o.cam_line:	h.dynalloc
o.cam_line:	h.DBUG
