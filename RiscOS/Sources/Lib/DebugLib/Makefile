CC =		cc
LD = 		Link
ASM =	 	ObjAsm
CMHG =	 	CMHG
LB =		LibFile
RM =		Remove
WIPE =		-Wipe

#CFLAGS =	-c -depend !Depend -Itbox:,C: -zo -zps1 -ffah -throwback
CFLAGS =	-c -depend !Depend -Itbox:,C: -UNOSOCKETS -zo -zps1 -ffah -throwback
CPPFLAGS = 	-c -depend !Depend -IC: -throwback
LDFLAGS	= 	-aif -c++
LBFLAGS =	-c
ASMFLAGS =	-throwback -NoCache -depend !Depend
CMHGFLAGS = 	-depend !Depend -throwback -IC:
WIPEFLAGS =	~C~V

TARGET = 	Targets.DebugLib

OBJECTS	= 	o.debug o.inits o.quits o.output

#LIBS =		libs.pd_pl libs.svcprint libs.t_m_pl libs.t_pl
LIBS =		libs.pd_pl libs.svcprint libs.t_m_pl libs.t_pl libs.remote

.SUFFIXES:	.o .c
.c.o:;		$(CC) $(CFLAGS) -o $@ $<

all: $(TARGET)

clean:
	$(RM)	$(TARGET)
	$(WIPE)	o.*		$(WIPEFLAGS)

$(TARGET): $(OBJECTS)
	$(LB) $(LBFLAGS) -o $@ $(OBJECTS) $(LIBS)

# Dynamic dependencies:
o.debug:	c.debug
o.debug:	h.include
o.debug:	tbox:h.toolbox
o.debug:	C:h.kernel
o.debug:	tbox:h.wimp
o.debug:	tbox:h.wimp
o.debug:	tbox:h.wimplib
o.debug:	C:h.kernel
o.debug:	C:h.swis
o.debug:	h.svcprint
o.debug:	h.PDebug
o.debug:	h.Trace
o.debug:	h.remote
o.debug:	h.debug
o.debug:	h.inits
o.debug:	h.output
o.debug:	h.quits
o.debug:	h.globals
o.inits:	c.inits
o.inits:	h.include
o.inits:	tbox:h.toolbox
o.inits:	C:h.kernel
o.inits:	tbox:h.wimp
o.inits:	tbox:h.wimp
o.inits:	tbox:h.wimplib
o.inits:	C:h.kernel
o.inits:	C:h.swis
o.inits:	h.svcprint
o.inits:	h.PDebug
o.inits:	h.Trace
o.inits:	h.remote
o.inits:	h.output
o.inits:	h.inits
o.inits:	h.globals
o.quits:	c.quits
o.quits:	h.include
o.quits:	tbox:h.toolbox
o.quits:	C:h.kernel
o.quits:	tbox:h.wimp
o.quits:	tbox:h.wimp
o.quits:	tbox:h.wimplib
o.quits:	C:h.kernel
o.quits:	C:h.swis
o.quits:	h.svcprint
o.quits:	h.PDebug
o.quits:	h.Trace
o.quits:	h.remote
o.quits:	h.quits
o.quits:	h.globals
o.output:	c.output
o.output:	h.include
o.output:	tbox:h.toolbox
o.output:	C:h.kernel
o.output:	tbox:h.wimp
o.output:	tbox:h.wimp
o.output:	tbox:h.wimplib
o.output:	C:h.kernel
o.output:	C:h.swis
o.output:	h.svcprint
o.output:	h.PDebug
o.output:	h.Trace
o.output:	h.remote
o.output:	h.output
o.output:	h.debug
o.output:	h.globals
