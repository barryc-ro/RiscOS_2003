# File:    Makefile
# Purpose: Makefile for Mailbox class library
# Author:  Ben Laughton
#
# History:
#
# 1998-08-28: BAL
# Created.




# ------------------------------------------------------------------------------
# Generic options
#

RM		= remove




# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Mailbox




# ------------------------------------------------------------------------------
# Build rules
#

all: rom

rom:
	@amu -f MainMake $(MFLAGS) TARGET=Mailbox EXT=o

debug:
	@amu -f MainMake $(MFLAGS) TARGET=Mailboxd EXT=od

preprocess:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i

export:
	@amu -f MainMake $(MFLAGS) TARGET=Mailbox

install_rom:
	@amu -f MainMake $(MFLAGS) TARGET=Mailbox

clean:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=z
	@$(RM) Mailbox
	@$(RM) Mailboxd
	@$(RM) Mailboxzm
	@echo $(COMPONENT): cleaned
