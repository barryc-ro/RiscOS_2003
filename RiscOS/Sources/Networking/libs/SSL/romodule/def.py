#!/usr/local/bin/python

import sys
import string

for d in sys.argv[1:]:
    src = open( d, "r" )
    lines = src.readlines()
    src.close()
    ok = 0
    for l in lines:
        s = string.split(l)
        if s:
            if ok == 1 and s[0] != "":
                print s[0]
            if ok == 0 and s[0] == "EXPORTS":
                ok = 1
