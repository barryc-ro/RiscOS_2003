/* > version.h
 *
 */

#ifndef __version_h
# define __version_h

#include "../../VersionNum"

/* --------------------------------------------------------------------------------------------- */

#define APP_NAME	"WSClient"

#define APP_DIR_VAR	APP_NAME"$Dir"
#define APP_DIR		"<"APP_DIR_VAR">"
#define APP_PATH_VAR	APP_NAME"$Path"
#define APP_PATH	APP_NAME":"

#define INI_PATH	APP_NAME"Ini:"

#define VERSION_STRING	Module_MajorVersion " (" Module_Date ") " Module_MinorVersion
/* #define VERSION_STRING	"0.01 (17-Dec-97)" */

/* --------------------------------------------------------------------------------------------- */

#define filetype_ICA	0x1CA

/* --------------------------------------------------------------------------------------------- */

#endif

/* eof version.h */
