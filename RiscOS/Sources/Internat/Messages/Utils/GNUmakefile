
.SUFFIXES:
.SUFFIXES: .o .c

.c.o:
	gcc -g -c -o $@ $< -I${CLXPATH}

PROGRAMS = LocaleChk MsgExt MsgExt2 MsgTok MsgTok2 TokGen TokGen2 TagExtract ScanRes ResCommon

.PHONY: all
all: ${PROGRAMS}

clean:;	rm -f *.o ${PROGRAMS}

LocaleChk: LocaleChk.o shared.o
	gcc -g -o LocaleChk LocaleChk.o shared.o ${CLXPATH}/clx.a
MsgExt: MsgExt.o shared.o
	gcc -g -o MsgExt MsgExt.o shared.o ${CLXPATH}/clx.a
MsgExt2: MsgExt2.o shared.o
	gcc -g -o MsgExt2 MsgExt2.o shared.o ${CLXPATH}/clx.a
MsgTok: MsgTok.o shared.o
	gcc -g -o MsgTok MsgTok.o shared.o ${CLXPATH}/clx.a
MsgTok2: MsgTok2.o shared.o
	gcc -g -o MsgTok2 MsgTok2.o shared.o ${CLXPATH}/clx.a
ResCommon: ResCommon.o shared.o
	gcc -g -o ResCommon ResCommon.o shared.o ${CLXPATH}/clx.a
ScanRes: ScanRes.o shared.o
	gcc -g -o ScanRes ScanRes.o shared.o ${CLXPATH}/clx.a
TokGen: TokGen.o shared.o
	gcc -g -o TokGen TokGen.o shared.o ${CLXPATH}/clx.a
TokGen2: TokGen2.o shared.o
	gcc -g -o TokGen2 TokGen2.o shared.o ${CLXPATH}/clx.a
TagExtract: TagExtract.o shared.o
	gcc -g -o TagExtract TagExtract.o shared.o ${CLXPATH}/clx.a
