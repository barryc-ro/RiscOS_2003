#!/bin/sh

perl util/perlpath.pl /usr/local
perl util/ssldir.pl /usr/local  
perl util/mk1mf.pl FreeBSD >Makefile.FreeBSD
perl Configure FreeBSD
