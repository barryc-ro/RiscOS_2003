/* > vdu.h

 *

 */

#define vduvar_ModeFlags 0
#define vduvar_NColours	3
#define vduvar_XEig	4
#define vduvar_YEig	5
#define vduvar_LineLength	6
#define vduvar_Log2BPP	9
#define vduvar_XLimit	11
#define vduvar_YLimit	12

#define modevar_DisplayStart 149

extern int vduval(int var);
extern int modeval(int mode, int var);
extern void GetModeSpec(int *width, int *height);
extern int GetModeNumber(void);

/* eof vdu.h */
