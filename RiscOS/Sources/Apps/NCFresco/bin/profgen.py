#!/usr/local/bin/python
"""Generate C/H files for Fresco profiling support

(C) Copyright ANT Limited 1997. All rights reserved.

Usage: %d in-file c-file h-file

in-file is a serioues of records, one line per record. Blank lines and
lines beginning with # are ignored. Each record has three fields,
seperated by whitespace. The second file is the enumeration to
reference from C, the third the textual name to print for this. The
first is a single character from the following list:

i	32 bit value
l	64 bit value

"""

#-----------------------------------------------------------------------------

c_header = """/* Profile C data source file */

#include "profile.h"

#if PROFILE
"""

c_footer = """

#endif /* PROFILE */

/* eof */
"""

h_header = """

#ifndef __profdat_h
#define __profdat_h

#ifndef __profile_h
#include "profile.h"
#endif

"""


h_footer = """

#endif /* __profdat_h */

/* eof */
"""

#-----------------------------------------------------------------------------

import os, sys, string

def emit(type, keys, names, cfile, hfile):
    ix = 0
    if type == 'i':
	s1, s2 = '32', ''
    else:
	s1, s2 = '64', '64'

    hfile.write( '#define PROF_NUM_%sBIT	%d\n' % (s1, len(keys)) )
    cfile.write( 'prof_rec%st prof_global%s[PROF_NUM_%sBIT] =\n{\n' % (s2, s2, s1) )
    hfile.write( '#if PROFILE\n' );
    hfile.write( 'extern prof_rec%st prof_global%s[PROF_NUM_%sBIT];\n' % (s2, s2, s1) )
    hfile.write( '#endif\n\n' );

    for x in xrange(len(keys)):
	key, name = string.upper(keys[x]), names[x]
	    
	hfile.write( '#define PROF_%s		%d\n' % (key, ix) )
	hfile.write( '#define PINC_%s		profile_INC%s(PROF_%s)\n' % (key, s2, key) )
	hfile.write( '#define PADD_%s(b)	profile_ADD%s(PROF_%s,b)\n\n' % (key, s2, key) )

	if ix == len(keys) - 1:
	    term = '\n};\n\n'
	else:
	    term = ',\n'

	cfile.write( '{ 0, "%s" }	/* %s */ %s' % (name, key, term) )

	ix = ix + 1

    return

def main():
    if len(sys.argv) != 4:
	print __doc__ % sys.argv[0]
	sys.exit(1)

    infile_name, cfile_name, hfile_name = sys.argv[1], sys.argv[2], sys.argv[3]
    infile = open(infile_name, 'r')
    lines = infile.readlines()
    infile.close()
    cfile, hfile = open(cfile_name, 'w'), open(hfile_name, 'w')

    dict32, dict64 = {}, {}

    cfile.write(c_header)
    hfile.write(h_header)

    for line in lines:
	all = string.split(line)
	if len(all) == 0 or len(all[0]) == 0 or all[0][0] == '#':
	    continue

	a,b = all[0], all[1]
	if len(all) == 2:
	    c = all[1]
	else:
	    c = string.join(all[2:])

	if a == 'i':
	    dict32[ b ] = c
	elif a == 'l':
	    dict64[ b ] = c
	else:
	    print 'What?', line

    emit( 'i', dict32.keys(), dict32.values(), cfile, hfile )
    emit( 'l', dict64.keys(), dict64.values(), cfile, hfile )

    cfile.write(c_footer)
    hfile.write(h_footer)

    cfile.close()
    hfile.close()

    print 'Finished converting %s into %s and %s' % (infile_name, cfile_name, hfile_name)

    return

if __name__ == '__main__': main()
