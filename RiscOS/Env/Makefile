# Makefile for the Build System Binary Build Environment
#
# $Id$
#

BBETYPE = env
bbe-env: bbe-generic-resources-get-alias
  BBE_Export_File ${ENVFILE}
  BBE_Export_File !Common
  BBE_Export_File Makefile
  BBE_Export_File VersionNum


# Determine whether the macro processor in amu works or not
FIXED = no
find-amu-version:
  @set BuildSys$AMU$Fixed ${FIXED}
