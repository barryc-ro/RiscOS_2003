/* > version.h
 *
 */

#ifndef __version_h
# define __version_h

#include "../../VersionNum"

/* --------------------------------------------------------------------------------------------- */

#define APP_NAME	"ICAClient"

#define APP_DIR_VAR	APP_NAME"$Dir"
#define APP_DIR		"<"APP_DIR_VAR">"
#define APP_PATH_VAR	APP_NAME"$Path"
#define APP_PATH	APP_NAME":"

#define OPTIONS_VAR		APP_NAME"$Options"
#define FILE_OPTIONS_VAR	APP_NAME"$FileOptions"
#define NOFILE_OPTIONS_VAR	APP_NAME"$NoFileOptions"

#define INI_PATH		APP_NAME"Ini:"

#define VERSION_STRING		Module_MajorVersion " (" Module_Date ") " Module_MinorVersion

#define SPLASH_FILE		APP_PATH "Splash"

#define ENSURE_TOOLBOX_CMD	"/" APP_PATH "EnsureTB"

#define APPSRV_FILE	INI_PATH "AppSrv"

/* --------------------------------------------------------------------------------------------- */

#endif

/* eof version.h */
