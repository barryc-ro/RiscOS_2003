#!/bin/sh
# Man-cgi : a Common Gateway Interface which converts the output of
#           the "man" command to HTML
#
VERSION="1.16"
#
# Author  : Panagiotis J. Christias <christia@theseas.ntua.gr>
#
# Usage   : it goes in the /cgi-bin/ directory. When called without 
#           arguments (http://www-server/cgi-bin/man-cgi) it outputs a
#           "Front Page" with a section list and query form. When called
#           with arguments, the first argument is the topic (e.g. ls)
#           and the second argument (optional) is the section to look in.
#
#           Examples:
#               http://www.ntua.gr/cgi-bin/man-cgi?ls
#               http://www.ntua.gr/cgi-bin/man-cgi?sleep+3
#
# Notice  : Man-cgi uses the following two images, you may want to get them
#           and put them in your w3 server:
#               http://www.ntua.gr/images/doty.gif
#               http://www.ntua.gr/images/pages.gif
#
# History :
#   1.16    Added -s section flag
#   1.15    Multiple blank lines are supressed.
#   1.14    Man-cgi now recognizes cross-references that
#           contain periods (e.g. foo.foo(3)).
#   1.13    Really fixed problems occuring in pages which used
#           multi-overstrike for emboldening (hopefully).
#   1.12    Fixed problems occuring in pages which used multi-overstrike
#           for emboldening. Now Man-cgi works any number of overstrike.
#   1.11    Fixed problem with hyphenated cross-references.
#   1.10 and earlier, no history at that time...
#
# Last update: 05-Dec-96
#

### CONFIGURATION SECTION : change the following variables to fit your needs ###

# The HTTPD server may not pass the enviroment variables to the script.
# You should add your system path here :
PATH=/usr/gnu/bin:/bin:/usr/ucb:/usr/bin:/usr/local/bin:/usr/misc/bin
export PATH

# The same for the man path here :
MANPATH=/usr/gnu/man:/usr/local/man:/usr/X11R5/man:/usr/new/man:/usr/misc/man:/usr/man:/usr/openwin/man:/usr/lang/man
export MANPATH

# The full URL of the Man-cgi :
MANCGI='http://www.internal.acorn.co.uk/~renaissa/man.cgi'

# The URL of the two images :
DOT='http://www.internal.acorn.co.uk/~renaissa/pics/bullet.gif'
PAGES='http://www.internal.acorn.co.uk/~renaissa/pics/pages.gif'

### END OF CONFIGURATION SECTION ###############################################

SECTION=$2
COMMAND=$1

echo "Content-type: text/html"
echo ""

if [ $# -eq 2 ] ; then
  if [ $2 = "00" ] ; then
    read a
    COMMAND=`echo $a | sed 's/command=\(.*\)&.*/\1/'`
    SECTION=`echo $a | sed 's/command=.*&section=\(.*\)$/\1/'`

    if [ $SECTION = "any" ] ; then
	SECTION=""
    fi;
  fi;
fi
if [ -n "$SECTION" ]
then 
     SECTION_SPEC="$SECTION"
     SECTION_NUMBER="($SECTION)"
else 
     SECTION_SPEC="1"
     SECTION_NUMBER=""
fi
if [ $# -ne 0 ] ; then
  cat <<END
  <html>
  <head><title>UNIX man page $COMMAND $SECTION_NUMBER</title></head>
  <body BGCOLOR="#ffffff">
  <center> <h1>
      <IMG SRC=$PAGES> UNIX man page $COMMAND $SECTION_NUMBER
  </h1> </center>
  <h4>NOTE: click <a href="$MANCGI?$COMMAND">here</a> if you get an empty page.</h4>
  <hr>
END
  man $SECTION_SPEC $COMMAND | \
  sed \
          -e '/-$/N
{
s/\([0-9A-z][-,0-9A-z]*\)-\n\(  *\)\([0-9A-z][-,0-9A-z]*([1-9][A-z]*)\)/\1\3\
\2/
}' \
          -e '/-$/N
{
s/\([0-9A-z][-,0-9A-z]*\)-\n\(  *\)\([0-9A-z][-,0-9A-z]*([1-9][A-z]*)\)/\1\3\
\2/
}' \
          -e '/-$/N
{
s/\([0-9A-z][-,0-9A-z]*\)-\n\(  *\)\([0-9A-z][-,0-9A-z]*([1-9][A-z]*)\)/\1\3\
\2/
}' \
          -e '/-$/N
{
s/\([0-9A-z][-,0-9A-z]*\)-\n\(  *\)\([0-9A-z][-,0-9A-z]*([1-9][A-z]*)\)/\1\3\
\2/
}' \
	  -e 's/</«/g' \
	  -e 's/>/»/g' \
   \
	  -e '/^[A-Z]/s/.//g' \
	  -e 's/^[A-Z][ ,A-Z]*$/<H3>&<\/H3>/' \
   \
	  -e 's/_\(.\)/<i>\1<\/i>/g' \
	  -e 's/.\(.\)/<b>\1<\/b>/g' \
   \
          -e 's#</b><b>.##g' \
          -e 's#</b>.<b>##g' \
          -e 's#.##g' \
          -e 's#_</i<b><</b>i>##g'  \
   \
	  -e 's/<\/i><i>//g' \
	  -e 's/<\/b><b>//g' \
   \
	  -e 's/^  \([A-Z][ ,0-9A-z]*\)$/  <b>\1<\/b>/' \
	  -e 's/^   \([A-Z][ ,0-9A-z]*\)$/  <b>\1<\/b>/' \
   \
	  -e "/^   /s#\(\([0-9A-z][-.,0-9A-z]*\)(\([1-9]\)[A-z]*)\)#<a href=\"$MANCGI\?\2+\3\">\1</a>#g" \
	  -e "/^   /s#\(<i>\([0-9A-z][-.,0-9A-z]*\)</i>(\([1-9]\)[A-z]*)\)#<a href=\"$MANCGI\?\2+\3\">\1</a>#g" \
   \
	  -e "s#<b>+<\/b>#<img src=$DOT>#" \
   \
          -e 's/«/\&lt;/g' \
          -e 's/»/\&gt;/g' \
   \
          -e '1s/^/<PRE>/' \
          -e '$s/$/<\/PRE>/' | \
awk '/^$/     { if(!blank) { print; blank=1 } }
     /^..*$/  { print; blank=0 }'

else
  cat <<END
  <html>
  <head><title>UNIX man page $COMMAND ($SECTION_NUMBER)</title></head>
  <body bgcolor="#ffffff">
  <center> <h1>
      <img src=$PAGES> UNIX man pages
  </h1> </center>
  
  <P><hr>
  You can request a man page by name and (optionally) by section:
  <P>
  <form method=post action=$MANCGI?00+00>
    Set command name :  <input name=command size=24>
    <input type=submit value="Fetch man page">
    <p>
    Set Section number : <select name=section>
      <option value="any"> any
      <option value="1" > 1
      <option value="2" > 2
      <option value="3" > 3
      <option value="4" > 4
      <option value="5" > 5
      <option value="6" > 6
      <option value="7" > 7
      <option value="8" > 8
      <option value="n" > n
      <option value="x" > x
    </select>
  </form>
END
fi
cat <<END
  <hr>
  Generated by <a href="http://www.internal.acorn.co.uk/~renaissa/man.cgi">man-cgi</a> $VERSION
  </body>
  </html>
END
