#!/bin/csh
#
# replace.sh <file> <search> <replace>
#
# e.g. replace.sh \!STBBoot/Choices/DEBUG,feb "\|Set Debug\$State \"High\"" "Set Debug\$State \"High\""
#
««Cmnt21»» ««Version»», ««Date»»

perl -pe "s/$argv[2]/$argv[3]/g" < $argv[1] > $argv[1].bak
if -e $argv[1].bak mv $argv[1].bak $argv[1]
