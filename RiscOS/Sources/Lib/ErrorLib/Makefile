# Project:   ErrorLib

CC =		cc
LD = 		Link
ASM =	 	ObjAsm
CMHG =	 	CMHG
LB =		LibFile
RM =		Remove
WIPE =		-Wipe

CFLAGS =	-c -depend !Depend -Itbox:,C: -UDEBUGLIB -fah -strict -throwback
CPPFLAGS = 	-c -depend !Depend -IC: -throwback
LDFLAGS	= 	-aif -c++
LBFLAGS =	-c
ASMFLAGS =	-throwback -NoCache -depend !Depend
CMHGFLAGS = 	-depend !Depend -throwback -IC:
WIPEFLAGS =	~C~V

TARGET = 	Targets.ErrorLib

OBJECTS	= 	o.error

.SUFFIXES:	.o .c
.c.o:;		$(CC) $(CFLAGS) -o $@ $<

all: $(TARGET)

clean:
	$(RM)	$(TARGET)
	$(WIPE)	o.*		$(WIPEFLAGS)

$(TARGET): $(OBJECTS)
	$(LB) $(LBFLAGS) -o $@ $(OBJECTS)
