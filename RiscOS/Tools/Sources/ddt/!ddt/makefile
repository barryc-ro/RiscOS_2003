!run:           ddt.o templates.o os.o lib.o
        link -o !runimage ddt.o templates.o os.o lib.o

os.o:           os.s
        objasm s.os -to o.os -stamp -quit

ddt.o:          ddt.c templates.h os.h
        cc -c -ff ddt.c

templates.o:    templates.c os.h lib.h templates.h
        cc -c -ff templates.c

lib.o:          lib.c lib.h
        cc -c -ff lib.c
