SRC	  = $(SRCHOME)/mx/src

include $(SRC)/buildtools/defaults.mk

COMPONENT = mx-inc

INCSRC	= $(SRCHOME)/mx/inc

MSGDIR = $(SRC)/mesg

MNIDLC_FLAGS =  -a coa -I ../pub -o $(INCSRC) -s oracle \
		-R "ys.log.msg-path=$(MSGDIR)" -R "mnidlc.c-cplus-kwd=true"

all idldirs:	$(INCSRC)/yeevent.h 

lib compile link cleanbin cleanobj cleanlib idlhdrs olint resource:

$(INCSRC)/yeevent.h: $(INCSRC)/yeevent.idl
	$(YCIDL) $(MNIDLC_FLAGS) -l $<

clean::
	rm -f yeevent.h yeevent?.[ch]

