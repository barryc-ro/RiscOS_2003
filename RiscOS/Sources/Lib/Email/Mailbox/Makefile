# File:    Makefile
# Purpose: Makefile for Mailbox class library
# Author:  Ben Laughton
#
# History:
#
# 1998-10-11: BAL
# Created

# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
EXP_DIR		= <Lib$Dir>.Email.Mailbox


# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CP		= copy
CC		= $(MemCheckCC) cc
C++		= $(MemCheckCC) c++
#C++		= g++
CM		= cmhg
RM		= remove
LD		= link
LB		= libfile
WIPE		= -wipe

CCFLAGS		= -c -depend !Depend -ffah -wP\
		  -I$(INCLUDES) $(DFLAGS) $(THROWBACK) $(CCEXTRA)
C++FLAGS	= -c -depend !Depend -ffa -wP\
		  -I$(INCLUDES) $(DFLAGS) $(THROWBACK) $(C++EXTRA)
#C++FLAGS	= -c -mamu -mstubs\
#		  $(DFLAGS) $(C++EXTRA) -mthrowback\
#		  -I lib:tboxlibs -I C: -I lib:C++Lib -I lib: -I@
ASFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~clfnr~v
LBFLAGS		= -c
WFLAGS		= ~CFR~V


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= <Lib$Dir>.tboxlibs,C:,<Lib$Dir>.C++Lib,<Lib$Dir>


# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Mailbox
TARGET		= Mailbox
TARGETD		= Mailboxd
TARGETZ		= Mailboxzm
EXPORTS		= $(EXP_HDR).$(COMPONENT)

OBJS =\
 o.Account\
 o.MBoxCB\
 o.Message\
 o.Operation\
 o.ProtocolImp\
 o.ProtocolOp\
 o.utils\
 o.WimpRegistry\


LOCALOBJS =\
 Local.o.LocalAccount\
 Local.o.LocalMailbox\
 Local.o.LocalMessage\


MBOXCBOBJS =\
 MBoxOpCB.o.connect\
 MBoxOpCB.o.disconnect\
 MBoxOpCB.o.expunge\
 MBoxOpCB.o.gethdrs\
 MBoxOpCB.o.getmsg\
 MBoxOpCB.o.getlength\
 MBoxOpCB.o.getnummsgs\
 MBoxOpCB.o.getsize\
 MBoxOpCB.o.MBoxOpCB\
 MBoxOpCB.o.process\
 MBoxOpCB.o.setflags\
 MBoxOpCB.o.sendmsg\


# MBoxOpCB.o.updateids\


POP3OBJS =\
 POP3.o.POP3Account\
 POP3.o.POP3Disconnect\
 POP3.o.POP3GetMsg\
 POP3.o.POP3GetHdrs\
 POP3.o.POP3GetNumMsgs\
 POP3.o.POP3Imp\
 POP3.o.POP3ListMsgs\
 POP3.o.POP3ListUIDs\
 POP3.o.POP3Logon\
 POP3.o.POP3Mailbox\
 POP3.o.POP3Message\
 POP3.o.POP3Op\
 POP3.o.POP3Process\


SENDQOBJS =\
 SendQ.o.SendQAccount\
 SendQ.o.SendQMailbox\
 SendQ.o.SendQMessage\
 SendQ.o.SMTPImp\
 SendQ.o.SMTPOp\
 SendQ.o.SMTPSendMsg\


OBJSI =\
 i.Account\
 i.MBoxCB\
 i.Message\
 i.Operation\
 i.ProtocolImp\
 i.ProtocolOp\
 i.utils\
 i.WimpRegistry\


MBOXCBOBJSI =\
 MBoxOpCB.i.connect\
 MBoxOpCB.i.disconnect\
 MBoxOpCB.i.expunge\
 MBoxOpCB.i.gethdrs\
 MBoxOpCB.i.getmsg\
 MBoxOpCB.i.getlength\
 MBoxOpCB.i.getnummsgs\
 MBoxOpCB.i.getsize\
 MBoxOpCB.i.MailboxCB\
 MBoxOpCB.i.process\
 MBoxOpCB.i.setflags\


# MBoxOpCB.o.updateids\


POP3OBJSI =\
 POP3.i.POP3Account\
 POP3.i.POP3Disconnect\
 POP3.i.POP3GetMsg\
 POP3.i.POP3GetHdrs\
 POP3.i.POP3GetNumMsgs\
 POP3.i.POP3Imp\
 POP3.i.POP3ListMsgs\
 POP3.i.POP3ListUIDs\
 POP3.i.POP3Logon\
 POP3.i.POP3Mailbox\
 POP3.i.POP3Message\
 POP3.i.POP3Op\
 POP3.i.POP3Process\


OBJSD =\
 od.Account\
 od.MBoxCB\
 od.Message\
 od.Operation\
 od.ProtocolImp\
 od.ProtocolOp\
 od.utils\
 od.WimpRegistry\


MBOXCBOBJSD =\
 MBoxOpCB.od.connect\
 MBoxOpCB.od.disconnect\
 MBoxOpCB.od.expunge\
 MBoxOpCB.od.gethdrs\
 MBoxOpCB.od.getlength\
 MBoxOpCB.od.getmsg\
 MBoxOpCB.od.getnummsgs\
 MBoxOpCB.od.getsize\
 MBoxOpCB.od.MBoxOpCB\
 MBoxOpCB.od.process\
 MBoxOpCB.od.sendmsg\
 MBoxOpCB.od.setflags\


# MBoxOpCB.od.updateids\


LOCALOBJSD =\
 Local.od.LocalAccount\
 Local.od.LocalMailbox\
 Local.od.LocalMessage\


POP3OBJSD =\
 POP3.od.POP3Account\
 POP3.od.POP3Disconnect\
 POP3.od.POP3GetHdrs\
 POP3.od.POP3GetMsg\
 POP3.od.POP3GetNumMsgs\
 POP3.od.POP3Imp\
 POP3.od.POP3ListMsgs\
 POP3.od.POP3ListUIDs\
 POP3.od.POP3Logon\
 POP3.od.POP3Mailbox\
 POP3.od.POP3Message\
 POP3.od.POP3Op\
 POP3.od.POP3Process\


SENDQOBJSD =\
 SendQ.od.SendQAccount\
 SendQ.od.SendQMailbox\
 SendQ.od.SendQMessage\
 SendQ.od.SMTPImp\
 SendQ.od.SMTPOp\
 SendQ.od.SMTPSendMsg\




# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .od .z .i .s .h .c .c++
.c.o:;		$(CC)  $(CCFLAGS)  -o $@ $<
.c.od:;		$(CC)  $(CCFLAGS)  -g -o $@ $<
.c.z:;		$(CC)  $(CCFLAGS)  -zm -zps1 -o $@ $<
.c++.o:;	$(C++) $(C++FLAGS) -o $@ $<
.c++.i:;	$(C++) $(C++FLAGS) -c -C -E $< >> $@
.c++.od:;	$(C++) $(C++FLAGS) +g -o $@ $<
#.c++.od:;	$(C++) $(C++FLAGS) -g -o $@ $<
.s.o:;		$(AS)  $(ASFLAGS)  -o $@ $<


# ------------------------------------------------------------------------------
# Build all the library
#

all:	$(TARGET)

debug:	$(TARGETD) export_hdrs
	$(CP)	$(TARGETD)	$(EXP_DIR).o.$(TARGETD)		$(CPFLAGS)

preprocess:	$(OBJSI) $(MBOXCBOBJSI) $(POP3OBJSI)
	@echo $(COMPONENT): preprocessing complete

# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

#test:
#	echo $(OBJS) { > tf }
#	sed s/o./od./g tf { > tf2 }

export: export_$(PHASE)

install_rom: $(TARGET)
	$(CP) $(TARGET) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
	@echo $(COMPONENT): rom module installed

clean:
	$(WIPE) o.* $(WFLAGS)
	$(WIPE) od.* $(WFLAGS)
	$(WIPE) z.* $(WFLAGS)
	$(WIPE) MBoxOpCB.o.* $(WFLAGS)
	$(WIPE) MBoxOpCB.od.* $(WFLAGS)
	$(WIPE) Local.o.* $(WFLAGS)
	$(WIPE) Local.od.* $(WFLAGS)
	$(WIPE) POP3.o.* $(WFLAGS)
	$(WIPE) POP3.od.* $(WFLAGS)
	$(RM) $(TARGET)
	$(RM) $(TARGETD)
	$(RM) $(TARGETZ)
	@echo $(COMPONENT): cleaned

export_hdrs: dirs
	$(CP)	h.*		$(EXP_DIR).h.*			$(CPFLAGS)
	$(CP)	MBoxOpCB.h.*	$(EXP_DIR).MBoxOpCB.h.*		$(CPFLAGS)
	$(CP)	Local.h.*	$(EXP_DIR).Local.h.*		$(CPFLAGS)
	$(CP)	POP3.h.*	$(EXP_DIR).POP3.h.*		$(CPFLAGS)
	$(CP)	SendQ.h.*	$(EXP_DIR).SendQ.h.*		$(CPFLAGS)
	@echo $(COMPONENT): export complete (hdrs)		$(CPFLAGS)

export_libs: $(TARGET) dirs
	$(CP)	$(TARGET)	$(EXP_DIR).o.$(TARGET)		$(CPFLAGS)
	@echo $(COMPONENT): export complete (libs)

local_dirs:
	$(MKDIR) o
	$(MKDIR) MBoxOpCB.o
	$(MKDIR) Local.o
	$(MKDIR) POP3.o
	$(MKDIR) SendQ.o

local_dirs_d:
	$(MKDIR) od
	$(MKDIR) MBoxOpCB.od
	$(MKDIR) Local.od
	$(MKDIR) POP3.od
	$(MKDIR) SendQ.od

dirs:
	$(MKDIR) $(LIBDIR)
	$(MKDIR) $(EXP_DIR)
	$(MKDIR) $(EXP_DIR).h
	$(MKDIR) $(EXP_DIR).o
	$(MKDIR) $(EXP_DIR).z
	$(MKDIR) $(EXP_DIR).MBoxOpCB
	$(MKDIR) $(EXP_DIR).MBoxOpCB.h
	$(MKDIR) $(EXP_DIR).Local
	$(MKDIR) $(EXP_DIR).Local.h
	$(MKDIR) $(EXP_DIR).POP3
	$(MKDIR) $(EXP_DIR).POP3.h
	$(MKDIR) $(EXP_DIR).SendQ
	$(MKDIR) $(EXP_DIR).SendQ.h


# ------------------------------------------------------------------------------
# Final link
#

$(TARGET): $(OBJS) $(LOCALOBJS) $(MBOXCBOBJS) $(POP3OBJS) $(SENDQOBJS) local_dirs
	$(LB) $(LBFLAGS) -o $@ $(OBJS) $(LOCALOBJS) $(MBOXCBOBJS) $(POP3OBJS)\
	                       $(SENDQOBJS)

$(TARGETD): $(OBJSD) $(LOCALOBJSD) $(MBOXCBOBJSD) $(POP3OBJSD) $(SENDQOBJSD) local_dirs_d
	$(LB) $(LBFLAGS) -o $@ $(OBJSD) $(LOCALOBJSD) $(MBOXCBOBJSD)\
	                       $(POP3OBJSD) $(SENDQOBJSD)

$(TARGETZ): $(OBJSZ) local_dirs
	$(LB) $(LBFLAGS) -o $(TARGETZ) $(OBJSZ)

$(EXP_HDR).$(COMPONENT): hdr.$(COMPONENT)
	$(CP) hdr.$(COMPONENT) $@ $(CPFLAGS)


# ------------------------------------------------------------------------------
# Dynamic dependencies:
