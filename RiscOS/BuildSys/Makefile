# Makefile for the Build System Binary Build Environment
#
# $Id$
#
include Makefiles:StdTools

BBETYPE = buildsys
bbe-buildsys: bbe-generic-resources-get-alias
  BBE_Export_Dir ImageName
  BBE_Export_Dir perl
  BBE_Export_Dir Makefiles
  ${RM} ${BBEDIR}.Makefiles.BBEExport
  ${MKDIR} ${BBEDIR}.Components
  BBE_Export_File Components.${COMPONENTFILE}
  BBE_Export_File VersionNum
  ${FILTERCOMP} Components.${COMPONENTFILE} < ModuleDB > ${BBEDIR}.ModuleDB

