#!/bin/sh
#
# clean up the mess that NT makes of my source tree
#

chmod +x Configure util/*
echo cleaning
/bin/rm -f `find . -name '*.$$$' -print` 2>/dev/null >/dev/null
echo 'removing those damn ^M'
perl -pi -e 's/\015//' * */* */*/* 2>/dev/null >/dev/null
make -f Makefile.ssl links
