#!/bin/sh
#
# generate the Microsoft makefiles and .def files
#

PATH=util:../util:$PATH

perl util/mk1mf.pl VC-MSDOS no-sock >ms/msdos.mak
perl util/mk1mf.pl VC-W31-32 >ms/w31.mak
perl util/mk1mf.pl VC-W31-32 dll >ms/w31dll.mak
perl util/mk1mf.pl VC-NT >ms/nt.mak
perl util/mk1mf.pl VC-NT dll >ms/ntdll.mak

perl util/mkdef.pl 16 crypto > ms/crypto16.def
perl util/mkdef.pl 32 crypto > ms/crypto32.def
perl util/mkdef.pl 16 ssl > ms/ssl16.def
perl util/mkdef.pl 32 ssl > ms/ssl32.def
