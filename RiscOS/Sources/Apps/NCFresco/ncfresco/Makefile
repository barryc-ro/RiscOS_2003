# fresco/ncfresco/Makefile - Cross build RISC OS ncfresco from a RiscIx box.
#                          This is (now) the standard build method.
#
# borris@ant.co.uk
#
#############################################################################

CC=		armcc
LD=             armlink
AS=             armasm

.SUFFIXES:            # Delete the default suffixes
.SUFFIXES: .c .o .h .s .oz # Define our suffix list

.c.o:
		@echo "Compiling $<"
		@$(CC) -c -o $@ $< $(CFLAGS)

.s.o:
		@echo "Assembling $<"
		$(AS) -o $@ $< $(ASFLAGS)

#############################################################################

# ,ff8 normal, fd3 debug
RUN=		../\!NCFresco/\!RunImage,ff8
MODULE=		../rm/ncfresco
AOF=		../aof/ncfresco

NCFRESCO_RISCOS:	$(RUN)

NCFRESCO_MODULE:	$(MODULE)

NCFRESCO_AOF:		$(AOF)

# empty normal, -debug debug
D=		# -debug

$(RUN): $(OBJS) $(LIBS)
		@echo "Linking Application $(@)"
		@$(LD) $(LDFLAGS) $(D) -o $@ $(OBJS) $(LIBS)
		ls -l $@

$(MODULE): $(OBJS) $(LIBS)
		@echo "Linking Module $(@)"
		@$(LD) $(LDFLAGS) $(D) -rmf -o $@ $(OBJS) $(LIBS)
		ls -l $@

$(AOF): $(OBJS) $(LIBS)
		@echo "Partial linking aof $(@)"
		@$(LD) $(LDFLAGS) $(D) -aof -o $@ $(OBJS) $(LIBS)
		ls -l $@


#############################################################################

# eof

# DO NOT DELETE THIS LINE - armdepend dependencies follow
