# This file was created by hand.
# Copyright (C) Advanced RISC Machines 1992.

COMPONENT=armasm
CLX=${CLXPATH}/clx.a

CC=gcc
CFLAGS=-c -O2 -I$(CLXPATH)  $(CFL) -DHOST_WANTS_NO_BANNER=1 -D__acorn -DBINARY_INCLUDES

OBJALL=aasm.o apply.o applysub.o asdsupp.o asm.o cli.o code.o condasm.o \
errors.o expr.o exprstor.o formatio.o fpio.o getdir.o getline.o globvars.o \
initdir.o listing.o literals.o locallab.o macrotyp.o mapping.o nametyp.o \
occur.o opcode.o p1dir.o p1line.o p2dir.o p2line.o stats.o store.o symdump.o \
tables.o tokens.o vars.o 
DERIVED = datenow.h

noarg: ${COMPONENT}
	@echo "Made ${COMPONENT} (the default target)"

.c.o:
	${CC} ${CFLAGS} ${CFL$*} -o $@ $<

# Dependencies

${COMPONENT}: $(OBJALL) $(CLX)
	$(CC) -s -o $@ $(OBJALL) $(CLX)
	@echo "Made $@"

install: ${COMPONENT}
	install ${COMPONENT} ${RISCOS_BUILD_TOOLS}/${COMPONENT}
	strip ${RISCOS_BUILD_TOOLS}/${COMPONENT}

clean:
	rm -rf ${OBJALL} ${COMPONENT} ${DERIVED}
	@echo cleaned ${COMPONENT}

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned ${COMPONENT} links

links:
	./mklinks
	@echo made ${COMPONENT} links

.PHONY: datenow.h
datenow.h:
	touch -f $@

aasm.o: aasm.c hostdep.h options.h asm.h \
 ${CLXPATH}/aof.h constant.h getline.h errors.h \
 formatio.h nametyp.h globvars.h initdir.h tables.h cli.h listing.h \
 symdump.h vars.h version.h VersionNum message.h
apply.o: apply.c apply.h exprtyp.h nametyp.h constant.h applysub.h \
 errors.h expr.h tables.h exprstor.h message.h
applysub.o: applysub.c applysub.h exprtyp.h nametyp.h constant.h \
 errors.h exprstor.h message.h /usr/include/sys/stat.h getline.h
asdsupp.o: asdsupp.c ${CLXPATH}/aof.h \
 ${CLXPATH}/asdfmt.h asdsupp.h constant.h \
 ${CLXPATH}/bytesex.h code.h aofvars.h \
 ${CLXPATH}/aout.h nametyp.h tables.h store.h \
 globvars.h
asm.o: asm.c hostdep.h options.h aofvars.h constant.h \
 ${CLXPATH}/aout.h ${CLXPATH}/aof.h \
 asdsupp.h mapping.h asm.h getline.h \
 ${CLXPATH}/bytesex.h code.h nametyp.h tables.h \
 condasm.h globvars.h errors.h ${CLXPATH}/fname.h \
 formatio.h initdir.h cli.h listing.h literals.h locallab.h macrotyp.h \
 opcode.h p1dir.h p2dir.h p1line.h p2line.h stats.h store.h message.h
cli.o: cli.c hostdep.h options.h aofvars.h constant.h \
 ${CLXPATH}/aout.h ${CLXPATH}/aof.h \
 asm.h getline.h ${CLXPATH}/bytesex.h \
 ${CLXPATH}/chunkfmt.h errors.h formatio.h nametyp.h \
 globvars.h cli.h version.h VersionNum \
 ${CLXPATH}/prgname.h message.h \
 ${CLXPATH}/fname.h store.h
code.o: code.c options.h ${CLXPATH}/aof.h \
 ${CLXPATH}/aout.h \
 ${CLXPATH}/chunkfmt.h mapping.h nametyp.h constant.h \
 aofvars.h occur.h tables.h store.h version.h VersionNum asm.h \
 getline.h ${CLXPATH}/bytesex.h code.h errors.h \
 globvars.h listing.h message.h
condasm.o: condasm.c condasm.h getline.h constant.h globvars.h \
 nametyp.h errors.h macrotyp.h tables.h message.h
errors.o: errors.c hostdep.h options.h errors.h constant.h asm.h \
 ${CLXPATH}/aof.h getline.h condasm.h globvars.h \
 nametyp.h formatio.h cli.h listing.h message.h
expr.o: expr.c p1line.h constant.h apply.h exprtyp.h nametyp.h \
 aofvars.h ${CLXPATH}/aout.h \
 ${CLXPATH}/aof.h code.h tables.h getline.h locallab.h \
 errors.h exprstor.h expr.h globvars.h occur.h tokens.h vars.h \
 message.h
exprstor.o: exprstor.c exprstor.h exprtyp.h nametyp.h constant.h \
 message.h
formatio.o: formatio.c formatio.h nametyp.h constant.h hostdep.h \
 options.h getline.h globvars.h message.h errors.h \
 ${CLXPATH}/fname.h
fpio.o: fpio.c ${CLXPATH}/ieeeflt.h constant.h fpio.h \
 p1line.h message.h
getdir.o: getdir.c options.h code.h constant.h \
 ${CLXPATH}/aof.h aofvars.h \
 ${CLXPATH}/aout.h nametyp.h tables.h asm.h getline.h \
 condasm.h globvars.h errors.h expr.h exprtyp.h getdir.h cli.h \
 listing.h macrotyp.h store.h message.h hostdep.h
getline.o: getline.c options.h aofvars.h constant.h \
 ${CLXPATH}/aout.h ${CLXPATH}/aof.h \
 asm.h getline.h errors.h formatio.h nametyp.h globvars.h macrotyp.h \
 tables.h message.h
globvars.o: globvars.c hostdep.h options.h globvars.h constant.h \
 aofvars.h ${CLXPATH}/aout.h \
 ${CLXPATH}/aof.h
initdir.o: initdir.c ${CLXPATH}/aof.h constant.h \
 errors.h globvars.h initdir.h nametyp.h tables.h store.h vars.h \
 message.h
listing.o: listing.c constant.h errors.h formatio.h nametyp.h \
 getline.h globvars.h listing.h aofvars.h \
 ${CLXPATH}/aout.h ${CLXPATH}/aof.h \
 p1line.h message.h vars.h
literals.o: literals.c ${CLXPATH}/aof.h aofvars.h \
 constant.h ${CLXPATH}/aout.h code.h nametyp.h \
 tables.h errors.h globvars.h literals.h store.h message.h
locallab.o: locallab.c constant.h errors.h globvars.h locallab.h \
 tables.h nametyp.h aofvars.h ${CLXPATH}/aout.h \
 ${CLXPATH}/aof.h p1line.h store.h tokens.h exprtyp.h \
 vars.h message.h
macrotyp.o: macrotyp.c asm.h ${CLXPATH}/aof.h \
 constant.h getline.h condasm.h globvars.h nametyp.h errors.h \
 formatio.h initdir.h tables.h listing.h locallab.h macrotyp.h occur.h \
 p1line.h store.h vars.h message.h
mapping.o: mapping.c globvars.h constant.h aofvars.h \
 ${CLXPATH}/aout.h ${CLXPATH}/aof.h \
 mapping.h ${CLXPATH}/bytesex.h asm.h getline.h code.h \
 nametyp.h tables.h store.h
nametyp.o: nametyp.c nametyp.h constant.h store.h
occur.o: occur.c asm.h ${CLXPATH}/aof.h constant.h \
 getline.h condasm.h globvars.h nametyp.h formatio.h occur.h tables.h \
 store.h message.h
opcode.o: opcode.c hostdep.h options.h errors.h constant.h globvars.h \
 nametyp.h opcode.h store.h message.h
p1dir.o: p1dir.c hostdep.h options.h macrotyp.h getline.h constant.h \
 nametyp.h tables.h aofvars.h ${CLXPATH}/aout.h \
 ${CLXPATH}/aof.h asdsupp.h asm.h code.h tokens.h \
 exprtyp.h condasm.h globvars.h errors.h expr.h formatio.h fpio.h \
 getdir.h initdir.h listing.h literals.h locallab.h occur.h p1line.h \
 p1dir.h store.h vars.h message.h
p1line.o: p1line.c code.h constant.h ${CLXPATH}/aof.h \
 aofvars.h ${CLXPATH}/aout.h nametyp.h tables.h \
 condasm.h getline.h globvars.h errors.h expr.h exprtyp.h formatio.h \
 fpio.h listing.h literals.h locallab.h macrotyp.h tokens.h occur.h \
 opcode.h p1dir.h p1line.h vars.h message.h
p2dir.o: p2dir.c hostdep.h options.h ${CLXPATH}/aof.h \
 asm.h constant.h getline.h aofvars.h \
 ${CLXPATH}/aout.h tokens.h exprtyp.h nametyp.h code.h \
 tables.h condasm.h globvars.h errors.h expr.h formatio.h fpio.h \
 getdir.h initdir.h listing.h literals.h locallab.h macrotyp.h \
 mapping.h p1line.h p2dir.h store.h vars.h message.h
p2line.o: p2line.c ${CLXPATH}/aof.h aofvars.h \
 constant.h ${CLXPATH}/aout.h mapping.h tokens.h \
 exprtyp.h nametyp.h code.h tables.h condasm.h getline.h globvars.h \
 errors.h expr.h formatio.h fpio.h listing.h literals.h locallab.h \
 macrotyp.h opcode.h p1line.h p2dir.h p2line.h vars.h message.h
stats.o: stats.c constant.h stats.h
store.o: store.c store.h message.h
symdump.o: symdump.c code.h constant.h \
 ${CLXPATH}/aof.h aofvars.h \
 ${CLXPATH}/aout.h nametyp.h tables.h formatio.h \
 getline.h globvars.h occur.h stats.h store.h symdump.h vars.h \
 message.h
tables.o: tables.c code.h constant.h ${CLXPATH}/aof.h \
 aofvars.h ${CLXPATH}/aout.h nametyp.h tables.h \
 errors.h globvars.h stats.h store.h message.h
tokens.o: tokens.c options.h aofvars.h constant.h \
 ${CLXPATH}/aout.h ${CLXPATH}/aof.h \
 errors.h exprtyp.h nametyp.h exprstor.h getline.h globvars.h \
 locallab.h tables.h macrotyp.h occur.h p1line.h tokens.h vars.h \
 message.h
vars.o: vars.c constant.h errors.h getline.h globvars.h macrotyp.h \
 nametyp.h tables.h aofvars.h ${CLXPATH}/aout.h \
 ${CLXPATH}/aof.h vars.h message.h
