# Makefile for Messages module utilities
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-Oct-94  SMC          Created.
#

#
# Generic options:
#
CC      = cc
LD      = link
RM      = remove
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -IC:
WFLAGS  = ~cr~v

#
# Libraries
#
CLIB    = CLIB:o.stubs

#
# Program specific options:
#
TARGETS = TagExtract MsgExt TokGen MsgTok MsgExt2 TokGen2 MsgTok2 ScanRes ResCommon

#
# Rule patterns
#
.c.o:;  ${CC} ${CCFLAGS} -o $@ $<

#
# Generic rules:
#
all: ${TARGETS}

clean:
        ${RM} TagExtract
        ${RM} MsgExt
        ${RM} TokGen
        ${RM} MsgTok
        ${RM} MsgExt2
        ${RM} TokGen2
        ${RM} MsgTok2
        ${RM} ScanRes
        ${RM} ResCommon
        ${WIPE} o.* ${WFLAGS}

TagExtract: o.TagExtract
        ${LD} -o $@ o.TagExtract ${CLIB}

ResCommon: o.ResCommon
        ${LD} -o $@ o.ResCommon ${CLIB}

MsgExt: o.MsgExt o.MsgCommon
        ${LD} -o $@ o.MsgExt o.MsgCommon ${CLIB}

TokGen: o.TokGen
        ${LD} -o $@ o.TokGen ${CLIB}

MsgTok: o.MsgTok
        ${LD} -o $@ o.MsgTok ${CLIB}

MsgExt2: o.MsgExt2
        ${LD} -o $@ o.MsgExt2 ${CLIB}

TokGen2: o.TokGen2
        ${LD} -o $@ o.TokGen2 ${CLIB}

MsgTok2: o.MsgTok2
        ${LD} -o $@ o.MsgTok2 ${CLIB}

ScanRes: o.ScanRes
        ${LD} -o $@ o.ScanRes ${CLIB}

# Dynamic dependencies:
