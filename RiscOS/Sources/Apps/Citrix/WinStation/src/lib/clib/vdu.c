/* > vdu.c
 *
 */

#include "windows.h"
#include "vdu.h"

#include <stdlib.h>

#include "../../inc/client.h"
#include "../../inc/clib.h"

#include "swis.h"

typedef struct
{
    int flags;
    int xres;
    int yres;
    int log2_bpp;
    int frame_rate;
} os_mode_selector;

#define ModeFiles_MonitorType	0x4D480
#define ModeFiles_SafeArea	0x4D481

/* ---------------------------------------------------------------------------------------------------- */

int vduval(int var)
{
    int val[3];

    val[0] = var;
    val[1] = -1;

    LOGERR(_swix(OS_ReadVduVariables, _INR(0,1), val, val + 2));

    return val[2];
}

int modeval(int mode, int var)
{
    int val;
    if (_swix(OS_ReadModeVariable, _INR(0,1) | _OUT(2), mode, var, &val) == NULL)
	return val;
    return -1;
}
    
void GetModeSpec(int *width, int *height)
{
    if (width)
	*width = vduval(vduvar_XLimit) + 1;
    if (height)
	*height = vduval(vduvar_YLimit) + 1;
}

int GetSafeModeSpec(int *width, int *height)
{
    int x0, y0, x1, y1;
    if (LOGERR(_swix(ModeFiles_SafeArea, _IN(0) | _OUTR(0,3),
		     0, /* flags */
		     &x0, &y0, &x1, &y1)) == NULL)
    {
	if (width)
	    *width = x1 - x0;
	if (height)
	    *height = y1 - y0;

	return TRUE;
    }
    return FALSE;
}


/*
 * This will look at the return from the monitorType SWI first. If that doesn't exist
 * it will look for the TV$Type system variable.
 */

int IsATV(void)
{
    static int is_a_tv_var = -1;

    if (is_a_tv_var == -1)
    {
	int type;
	if (LOGERR(_swix(ModeFiles_MonitorType, _IN(0) | _OUT(0), 0, &type)))
	{
	    char *s = getenv("TV$Type");
	    is_a_tv_var = s && (stricmp(s, "PAL") == 0 || stricmp(s, "NTSC") == 0);
	}
	else
	{
	    is_a_tv_var = type == 0 || type == 8;
	}
    }

    return is_a_tv_var;
}

/* ---------------------------------------------------------------------------------------------------- */

/*
 * Get a sprite type corresponding to the current mode
 */

static int build_mode_number(int log2bpp)
{
    return 1 + (log2bpp+1)*0x08000000 +
	(180 >> vduval(vduvar_YEig))*0x00004000 +
	(180 >> vduval(vduvar_XEig))*0x00000002;
}

int GetModeNumber(void)
{
    int mode;

    /* Else try and read screen mode - new way (3.5) */
    if (_swix(OS_ScreenMode, _IN(0) | _OUT(1), 1, &mode) != NULL)
    {
	/* Else try and read screen mode - old way (3.1) */
        LOGERR(_swix(OS_Byte, _IN(0) | _OUT(2), 135, &mode));
    }

    /* If new style mode must use new style sprites (3.5) */
    if ((unsigned)mode > 255)
    {
        os_mode_selector *m = (os_mode_selector *)(long)mode;
	mode = build_mode_number(m->log2_bpp);
    }

    return mode;
}

/* ---------------------------------------------------------------------------------------------------- */

#define ScreenFade_FadeRectangle	0x4e5c0
#define ScreenFade_ProcessorUsage	0x4e5c1
#define ScreenFade_GetFadeInfo		0x4e5c2
#define ScreenFade_ReleaseFade		0x4e5c3

#define FADE_TIME		100
#define FADE_LINE_SPACING	8

void FadeScreen(unsigned colour)
{
    int ref;
    if (LOGERR(_swix(ScreenFade_FadeRectangle, _INR(0,7) | _OUT(1),
		     1<<8,
		     0, 0, -1, -1, 
		     FADE_TIME,
		     colour,
		     FADE_LINE_SPACING,
		     &ref)) == NULL)
    {
	
	int status;

	do
	{
	    LOGERR(_swix(ScreenFade_GetFadeInfo, _INR(0,1) | _OUT(0),
			 0, ref, &status));
	}
	while ((status & 0xff) != 0xff);

	LOGERR(_swix(ScreenFade_ReleaseFade, _INR(0,1), 0, ref));
    }
}

/* ---------------------------------------------------------------------------------------------------- */

/* eof vdu.c */
