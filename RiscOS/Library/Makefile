#
# Binary Build Environment Makefile
#
# The Library needs to participate in the generation of the BBE by exporting
# those tools required by the BBE.
#

BBETYPE = library
bbe-library: bbe-generic-resources-get-alias
  BBE_Export_Dir Acorn
  BBE_Export_Dir ARM
  BBE_Export_Dir Build
  BBE_Export_Dir DrSmiths
  BBE_Export_Dir GNU
  BBE_Export_Dir Misc
  BBE_Export_Dir Unix
  ${MKDIR} ${BBEDIR}.ToolOptions
  BBE_Export_File ToolOptions.${APCS}
  BBE_Export_File VersionNum
  @| end of Library export
