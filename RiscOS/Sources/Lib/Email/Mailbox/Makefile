# File:    Makefile
# Purpose: Makefile for Mailbox class library
# Author:  Ben Laughton
#
# History:
#
# 1998-08-28: BAL
#   Created.
# 1998-09-28: RDW
#   install_rom target removed, not required.
#   rom target removed, not required.
#   all target changed to make RAM version for development builds
#   Mailboxz target added to export 




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

all:
	@amu -f MainMake $(MFLAGS) TARGET=Mailbox EXT=o

debug:
	@amu -f MainMake $(MFLAGS) TARGET=Mailboxd EXT=od

preprocess:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i

export:
	@amu -f MainMake $(MFLAGS) TARGET=Mailbox EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=Mailboxzm EXT=z

clean:
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) TARGET=AvoidAMUBug EXT=z
	@$(RM) Mailbox
	@$(RM) Mailboxd
	@$(RM) Mailboxzm
	@echo $(COMPONENT): cleaned
