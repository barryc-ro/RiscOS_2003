/* > vdu.c
 *
 */

#include "windows.h"
#include "vdu.h"

#include "../../inc/client.h"

#include "swis.h"

typedef struct
{
    int flags;
    int xres;
    int yres;
    int log2_bpp;
    int frame_rate;
} os_mode_selector;

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

/* eof vdu.c */
