# Project:   Caradon

#
# Paths
#
EXP_HDR = <export$dir>
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg502
CP      = copy
LD      = link
RM      = remove
WIPE    = @-wipe
CD	= dir
DEFMOD  = DefMod
AR	= LibFile
MODSQZ  = modsqz

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
INCLUDES= -IC: 

ASFLAGS = 	-depend !Depend -Stamp -quit -module
CFLAGS = 	-c -depend !Depend -IC:, -throwback -zM -DISDN
DATATOAOFFLAGS=	-d "" -l _len -dp -s -p


# Libraries
#
INETLIB  = TCPIPLibs:o.inetlibzm
CLIB	 = C:o.Stubs

#
# Program specific options:
#
COMPONENT   = Caradon
TARGET      = rm.Caradon
EXPORTS =
OBJS =  o.module o.driver o.serial o.filegen o.headermod o.resourcefs o.screen \
        o.muldiv o.swiven o.backdrop o.connect o.onhook o.offhook o.ppp \
        o.enternnpc o.proceed o.proceedc o.callback o.clearmsg o.configs o.log

#
# Rule patterns
#
.SUFFIXES: 	.d
.c.o:;		$(CC) $(CFLAGS) -o $@ $<
.s.o:;		$(AS) ${ASFLAGS} -o $@ $<
.cmhg.o:;     	$(CMHG) $< -o $@ h.cmhg_modem
.d.o:;        	datatoaof -f $< -o $@ $(DATATOAOFFLAGS)


#
# Generic rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
  	${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Custom rules
#
${TARGET}: ${OBJS} ${CLIB} ${INETLIB}
	${LD} -o $@ -m ${OBJS} ${CLIB} ${INETLIB}
	${MODSQZ} $@
	
o.resourcefs: @.ResModem.Apps.!Sennen.!RunImage
        genres ResModem -r -o $@



# Dynamic dependencies:
o.serial:	c.serial
o.serial:	C:h.kernel
o.serial:	C:h.swis
o.serial:	h.serial
o.serial:	C:h.string
o.serial:	h.irqs
o.filegen:	c.filegen
o.filegen:	h.defs
o.filegen:	C:h.kernel
o.filegen:	h.screen
o.filegen:	h.defs
o.filegen:	h.smartcard
o.filegen:	C:h.swis
o.filegen:	Sockets.h.in
o.screen:	c.screen
o.screen:	h.defs
o.screen:	C:h.kernel
o.screen:	C:h.swis
o.muldiv: s.muldiv
o.swiven: s.swiven
o.swiven: h.regdefs
o.swiven: h.swinos
o.swiven: h.macros
o.callback: s.callback
o.configs:	c.configs
o.configs:	C:h.swis
o.configs:	C:h.kernel
o.configs:	C:h.kernel
o.configs:	h.configs
o.log:	c.log
o.log:	h.log
o.module:	c.module
o.module:	C:h.kernel
o.module:	h.cmhg_modem
o.module:	h.serial
o.module:	h.callback
o.module:	h.driver
o.module:	h.screen
o.module:	h.defs
o.module:	C:h.kernel
o.module:	h.smartcard
o.module:	C:h.swis
o.module:	h.wimp
o.module:	h.defs
o.module:	h.configs
o.module:	Sockets.h.in
o.module:	h.log
