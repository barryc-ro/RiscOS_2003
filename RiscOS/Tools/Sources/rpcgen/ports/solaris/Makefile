# Makefile for rpcgen
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 31-May-01  SNB          Created.

# Generic options:
#
MKDIR   = mkdir -p
CC      = gcc
LD      = gcc

CCFLAGS = -c ${INCLUDES} ${DFLAGS} ${THROWBACK}
LDFLAGS =
CLX     = ${CLXPATH}/clx.a

#
# Include files
#
INCLUDES  = -I${CLXPATH}
DFLAGS    = -DTARGET_IS_RISCOS

#
# Program specific options:
#
COMPONENT  = armrpcgen
TARGET     = ${COMPONENT}

OBJS=	  rpc_main.o rpc_hout.o rpc_cout.o rpc_parse.o  \
      	  rpc_scan.o rpc_util.o rpc_svcout.o rpc_clntout.o  \
	  rpc_tblout.o rpc_sample.o 

LIBS=	${CLX}

#
# Rule patterns
#
.c.o:;	${CC} ${CCFLAGS} -o $@ $<

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} 
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
	rm -rf *.o ${TARGET}

realclean:
	rm -rf *.o ${TARGET}
	find . -type l -exec rm -f {} \;

# Dynamic dependencies:
