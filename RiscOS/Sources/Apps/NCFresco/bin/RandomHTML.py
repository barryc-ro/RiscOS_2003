#!/usr/local/bin/python
""" Generate Random HTML from an Idiom Template

    This takes a template of HTML fragments and stitches them together
    to form a random document.

    A fragment is a string or a tuple of strings or fragment lists.
    When a fragment is selected the strings are emitted and a random nested fragment
    is chosen.
"""

from types import *
import rand

head = []
body = []

head [1:1] = [ "<title> Dummy Title </title>",
               "" ]

body1 = [ (body, body), 
          (body, body, body), 
          (body, body, body, body),
          (body, body, body, body, body) ]

text        = [ "A long boring sentence that drones on and on and on with far too many references to Watney's Red Barrel.",
                "A short sentence.",
                "Word",
                "&lt;", "&gt;", "&entity;",
                "" ]

headings    = [ ("<H1>", body, "</H1>"),
                ("<H2>", body, "</H2>"),
                ("<H3>", body, "</H3>"),
                ("<H4>", body, "</H4>"),
                ("<H5>", body, "</H5>"),
                ("<H6>", body, "</H6>") ]

paras       = [ ("<p>", body),
                ("<p align=center>", body),
                ("<p align=right>", body),
                ("<p>", body, "</p>"),
                ("<p align=center>", body, "</p>"),
                ("<p align=right>", body, "</p>") ]

listitems   = [ ("<li>", body),
                ("<li>", body, "</li>"),
                ("<li type=disc>", body),
                ("<li type=disc>", body, "</li>"),
                ("<li type=square>", body),
                ("<li type=square>", body, "</li>"),
                ("<li type=circle>", body),
                ("<li type=circle>", body, "</li>") ]

listbody = []
listbody[1:] = [ paras, listitems, text,
                (listbody, listbody),
                (listbody, listbody, listbody) ]

body2       = [ headings, paras,
                "<hr>",
                ("<ul>", listbody, "</ul>"),
                ("<ol>", listbody, "</ol>"),
                ("<b>", body, "</b>"),
                ("<i>", body, "</i>"),
                ("<em>", body, "</em>"),
                ("<tt>", body, "</tt>"),
                "<b>", "<i>", "<em>", "<tt>", 
                "</b>", "</i>", "</em>", "</tt>" ]

body [1:1] = [ body1, body2, text ]

root = [ ("<head>", head, "</head> <body>", body1, "</body>"),
         (head, "<body>", body1, "</body>"),
         ("<head>", head, "</head>", body1),
         (head, body1) ]

def expand (depth, fragment):
    if type (fragment) is StringType:
        print depth * ' ' + fragment
    elif type (fragment) is ListType:
        expand (depth, rand.choice (fragment))
    elif type (fragment) is TupleType:
        for elt in fragment:
            if rand.rand() % 64 > depth:
                expand (depth+1, elt)
            else:
                print depth * ' ' + "Leaf"

expand (0, root)
