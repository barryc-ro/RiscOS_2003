dnl
dnl buildoptions.m4
dnl
dnl Copyright (C) Pace Micro Technology plc. 2001
dnl
dnl $Id$
dnl

dnl
dnl These macros are related to setting up the Pace debugging library
dnl DebugLib for its own internal builds.
dnl
dnl Adding a new device
dnl ===================
dnl
dnl When adding a new debug "device", you need to add your device to any default sets for
dnl which it shuld be included; in _PACE_DEBUGLIB_BUILD_OPTION_DEFAULTS add in a new call
dnl under the others to _PACE_DEBUGLIB_BUILD_OPTION.  Add the appropriate substitution at
dnl the bottom of DebugLib/DebugLib.h.in and add another enum member to the debug_devices
dnl type. Add the new .c and .h files for your new device (copy an existing device's code
dnl to ensure you get it right).  Add your new device to the methods table in src/debug.c
dnl remembering that the array index must match your debug_device identity.  Include your
dnl header file at the top of src/debug.c.  Add both your new files to src/Makefile.am to
dnl the libdebuglib_la_SOURCES macro.


dnl This macro can be used to set up default lists of what is and what is not included
dnl in a DebugLib build.  In particular it adds the "--enable-defaults=SPEC" option.  SPEC
dnl may be anything that matches the case clauses in this macro.  Note however that we have
dnl used some names to mean things that you might not thing are intuitive:
dnl
dnl all           All portable devices (ie. no OS-specific devices!)
dnl everything    Really everything (but probably won't build)
dnl powertv       Devices available on PowerTV OSes
dnl riscos        Devices available on RISC OS
dnl
dnl You can do things like: "--enable-defaults=everything --disable-pdebug" to put in everything
dnl except specific devices.  You can specify multiple --enable-defaults which will simply add
dnl more and more devices into the configured list.  Only --disable options can remove devices.

AC_DEFUN([_PACE_DEBUGLIB_BUILD_OPTION_DEFAULTS],
[
  AC_ARG_ENABLE([defaults],
    AC_HELP_STRING([--enable-defaults=SPEC],
                   [Sets up the build options based on the list SPEC in config/buildoptions.m4]),
                   ac_cv_debuglib_defaults=$enableval, ac_cv_debuglib_defaults=no)
  AC_CACHE_CHECK([whether to infer a default set of build devices],
    [ac_cv_debuglib_defaults],
    [ac_cv_debuglib_defaults=no])
  case "${ac_cv_debuglib_defaults}" in
    no|yes)
      AC_MSG_NOTICE([all build options except printf are disabled by default])
      ac_cv_enable_printf=yes;;
    everything)
      ac_cv_enable_tracker=yes
      ac_cv_enable_printf=yes
      ac_cv_enable_pdebug=yes
      ac_cv_enable_remotedb=yes
      ac_cv_enable_debugit=yes
      ac_cv_enable_tml=yes
      ac_cv_enable_file=yes
      ac_cv_enable_serial=yes
      ac_cv_enable_dadebug=yes
      ac_cv_enable_brainlink=yes
      ac_cv_enable_myprintf=yes
      ac_cv_enable_ptvlog=yes
      ac_cv_enable_syslog=yes
      ac_cv_enable_socket=yes;;
    all)
      ac_cv_enable_printf=yes
      ac_cv_enable_file=yes
      ac_cv_enable_myprintf=yes
      ac_cv_enable_syslog=yes
      ac_cv_enable_socket=yes;;
    riscos|RISCOS)
      ac_cv_enable_tracker=yes
      ac_cv_enable_printf=yes
      ac_cv_enable_pdebug=yes
      ac_cv_enable_remotedb=yes
      ac_cv_enable_debugit=yes
      ac_cv_enable_tml=yes
      ac_cv_enable_file=yes
      ac_cv_enable_serial=yes
      ac_cv_enable_dadebug=yes
      ac_cv_enable_brainlink=yes
      ac_cv_enable_myprintf=yes
      ac_cv_enable_socket=yes;;
    powertv|POWERTV|ptv)
      ac_cv_enable_printf=yes
      ac_cv_enable_myprintf=yes
      ac_cv_enable_ptvlog=yes;;
    *) AC_MSG_ERROR([unknown value (${ac_cv_debuglib_defaults}) - see config/buildoptions.m4])
  esac
])



dnl To avoid repetition, this private macro expands to the full required stuff
dnl to add a --enable option.  Arg 1 is the text after --enable-; Arg 2 is help
dnl text for ./configure --help; Arg 3 is the middle part of the macro name to
dnl go in config.h.  Arg 4 is optional and represents any extra text to place
dnl after the help string.  Also, @pace_debuglib_ifdef_device_$1@
dnl is set to an *exportable* pre-processor conditional for DebugLib.h (because
dnl you cannot use ENABLE_$1_SUPPORT macros in publicised heder files), and
dnl @pace_debuglib_has_device_$1@ is defined (or not) as DEBUGLIB_HAS_$3_SUPPORT
dnl for the exportable header file to enable clients to determine at compile-
dnl time whether they want to make any choices based on the DebugLib installed.

AC_DEFUN([_PACE_DEBUGLIB_BUILD_OPTION],
[
  AC_ARG_ENABLE([$1],
    AC_HELP_STRING([--enable-$1],
                   [$2 support $4]),
                   ac_cv_enable_$1=$enableval)
  AC_CACHE_CHECK(whether to include $2, ac_cv_enable_$1, ac_cv_enable_$1=no)
  if test "$ac_cv_enable_$1" = "no"; then
    pace_debuglib_ifdef_device_$1="#if 0 /* No support for $2 */"
    pace_debuglib_has_device_$1="/* #undef DEBUGLIB_HAS_$3_SUPPORT */"
  else
    AC_DEFINE([ENABLE_$3_SUPPORT],1,[Include built-in support for $2])
    pace_debuglib_ifdef_device_$1="#if 1 /* Support for $2 */"
    pace_debuglib_has_device_$1="#define DEBUGLIB_HAS_$3_SUPPORT"
  fi
  AC_SUBST(pace_debuglib_ifdef_device_$1)
  AC_SUBST(pace_debuglib_has_device_$1)
])

dnl
dnl PACE_DEBUGLIB_BUILD_OPTIONS
dnl
dnl This macro is called from configure.in to add in all the build options.  This avoids
dnl cluttering up configure.in.
dnl

AC_DEFUN([PACE_DEBUGLIB_BUILD_OPTIONS],
[
  _PACE_DEBUGLIB_BUILD_OPTION_DEFAULTS
  _PACE_DEBUGLIB_BUILD_OPTION([tracker], [!Tracker], [TRACKER], [(RISC OS only)])
  _PACE_DEBUGLIB_BUILD_OPTION([printf], [console printf], [PRINTF])
  _PACE_DEBUGLIB_BUILD_OPTION([pdebug], [Parallel debug], [PDEBUG], [(RISC OS only)])
  _PACE_DEBUGLIB_BUILD_OPTION([remotedb], [Remote DB], [REMOTEDB], [(RISC OS only)])
  _PACE_DEBUGLIB_BUILD_OPTION([debugit], [DebugIt], [DEBUGIT], [(RISC OS only)])
  _PACE_DEBUGLIB_BUILD_OPTION([tml], [The Missing Link (TML)], [TML], [(RISC OS only)])
  _PACE_DEBUGLIB_BUILD_OPTION([file], [file], [FILE])
  _PACE_DEBUGLIB_BUILD_OPTION([serial], [Serial], [SERIAL], [(RISC OS only)])
  _PACE_DEBUGLIB_BUILD_OPTION([dadebug], [Dynamic area buffering], [DADEBUG], [(RISC OS only)])
  _PACE_DEBUGLIB_BUILD_OPTION([brainlink], [BrainLink], [BRAINLINK], [(RISC OS only)])
  _PACE_DEBUGLIB_BUILD_OPTION([myprintf], [User printf callback], [MYPRINTF])
  _PACE_DEBUGLIB_BUILD_OPTION([ptvlog], [PowerTV logger], [PTVLOG], [(PowerTV only)])
  _PACE_DEBUGLIB_BUILD_OPTION([syslog], [UNIX syslog], [SYSLOG])
  _PACE_DEBUGLIB_BUILD_OPTION([socket], [TCP stream socket], [SOCKET])
])
