OMXROOT = m:
BUILDROOT = c:\mx

include m:\src\buildtools\winnt.mak

SRC	= $(SRCHOME)\mx\src

COMPONENT = mx-inc

INCSRC	= $(OMXROOT)\inc

MSGDIR = $(SRC)\mesg

MNIDLC_FLAGS =  -a coa -I ../pub -o $(INCSRC) -s oracle \
		-R "ys.log.msg-path=$(MSGDIR)" -R "mnidlc.c-cplus-kwd=true"

all idldirs:	$(INCSRC)\yeevent.h

lib compile link clean cleanbin cleanobj cleanlib idlhdrs olint resource:
	@-echo	$@ in inc done.

$(INCSRC)\yeevent.h : $(INCSRC)\yeevent.idl
	$(YCIDL) $(MNIDLC_FLAGS) -l $(INCSRC)\yeevent.idl


