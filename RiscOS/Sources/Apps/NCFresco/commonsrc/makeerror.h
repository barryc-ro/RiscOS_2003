/* -*-c-*- */

os_error *makeerror(int n);
os_error *makeerrorf(int n, ...);

#define ANTWEB_ERROR_BASE	0x80AB0

#define ERR_CANT_MAKE_WIN	1
#define ERR_UNSUPORTED_SCHEME	2
#define ERR_PARSE_FAILED	3
#define ERR_BAD_DOCUMENT	4
#define ERR_NO_SUCH_FRAG	5
#define ERR_NO_MEMORY		6
#define ERR_BAD_IMAGE_HANDLE	7
#define ERR_CANT_READ_FILE	8
#define ERR_NO_SUCH_HOST	9
#define ERR_CANT_FIND_SERVICE	10
#define ERR_CANT_GET_URL	11
#define ERR_NO_HISTORY		12
#define ERR_BAD_PASSWD		13
#define ERR_BAD_AUTH		14
#define ERR_NOT_IMPLEMENTED	15
#define ERR_CANT_OPEN_HOTLIST	16
#define ERR_USED_HELPER		17
#define ERR_CANT_OPEN_AUTH_FILE	18
#define ERR_NO_SCRAP_DIR	19
#define ERR_NOT_AN_IMAGE	20
#define ERR_BAD_FORM		21
#define ERR_BAD_CONTEXT		22
#define ERR_NEED_DOCUMENT	23
#define ERR_NO_PRINTER		24
#define ERR_BAD_PAGE		25
#define ERR_PAGE_FETCHING	26
#define ERR_PAGE_NOT_WHOLE	27
#define ERR_ACCESS_DENIED	28
#define ERR_FIND_FAILED         29
#define ERR_NO_ACTION	        30
#define ERR_PRINT_FAILED        31
#define ERR_CANT_OPEN_FILE      32
#define ERR_OLE_LOCAL           33
#define ERR_CANT_MAKE_DIR       34
#define ERR_NO_EDITOR           35
#define ERR_PRINT_FRAMESET      36
#define ERR_DEMO_VERSION        37
#define ERR_NO_DISC_SPACE       38
#define ERR_BAD_FILE_TYPE	39
#define ERR_CANT_READ_URL_FILE	40
