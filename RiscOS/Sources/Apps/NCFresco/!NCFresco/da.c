/* > da.c
 */

#if MEMLIB
static int da_not_used;
#else

#include "swis.h"

#include "version.h"

#include "da.h"

#define BOOL int
#define TRUE 1
#define FALSE 0
#ifndef NULL
#define NULL 0
#endif

enum			/* Dynamic area reason codes */
{
    DynamicArea_Create,
    DynamicArea_Remove,
    DynamicArea_ReturnInfo,
    DynamicArea_Enumerate,
    DynamicArea_Renumber,
    DynamicArea_ReturnApplSize
};

enum			/* user mode access rights */
{
    Access_UserReadWrite,
    Access_UserReadOnly,
    Access_UserNoAccess
};

enum
{
    Area_NotBufferable	= 0x10,
    Area_NotCacheable	= 0x20,
    Area_DoublyMapped	= 0x40,
    Area_NotDraggable	= 0x80
};

#define MoveMemoryErrors_Val	0x1C0
#define MoveMemoryErrors_Mask	0x003

int da_adjust(int da_number, int *da_size, int nbytes)
{
    int bytes_moved;
    _kernel_oserror *e;

    e = _swix(OS_ChangeDynamicArea, _INR(0,1) | _OUT(1),
        da_number, nbytes - *da_size, &bytes_moved);

    if (e)
    {
	usrtrc("da: error in adjust 0x%x '%s', request %d size %d\n", e->errnum, e->errmess, nbytes, *da_size);
	return FALSE;
    }

    if (nbytes < *da_size)
        *da_size -= bytes_moved;
    else
	*da_size += bytes_moved;

    return TRUE;
}

void da_free(int *da_number, int *da_size, void **da_base)
{
    _kernel_oserror *e;

    e = _swix(OS_DynamicArea, _INR(0,1), DynamicArea_Remove, *da_number);

    if (e) usrtrc("da: error in free 0x%x '%s'\n", e->errnum, e->errmess);

    *da_number = 0;
    *da_size = 0;
    *da_base = NULL;
}

int da_create(const char *programname, int *da_number_out, int *da_size_out, void **base_out)
{
    _kernel_oserror *e;

    e = _swix(OS_DynamicArea, _INR(0,8)|_OUT(1)|_OUT(3),
	        DynamicArea_Create,
		-1,				/* number */
		0,
		-1,				/* logical address */
		Area_NotDraggable,		/* flags */
		-1,				/* max size */
		0,				/* -> area handler */
		0,				/* -> workspace */
		programname,
		da_number_out,
		base_out);

    if (e) usrtrc("da: error in create 0x%x '%s'\n", e->errnum, e->errmess);

    *da_size_out = 0;
    
    return e == NULL;
}

#endif

/* da.c */
