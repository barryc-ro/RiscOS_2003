
.SUFFIXES: .c .oo

all:
	@echo "Making cryptlib"
	(cd crypto; make )
	@echo "Making lib"
	(cd ssl; make )

clean:
	( cd crypto; make clean )
	( cd ssl; make clean )
