
#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"

#include "robits.h"

int riscos_FileExists( char *fname )
{
    _kernel_osfile_block bk;
    int i = _kernel_osfile(17, fname, &bk);

        if ( i <= 0 )
        {
                return 0;
        }

        return (i & 1) == 1;
}

int riscos_DirExists( char *fname )
{
    _kernel_osfile_block bk;
    int i = _kernel_osfile(17, fname, &bk);

        if ( i <= 0 )
        {
                return 0;
        }

        return (i & 2) == 2;
}

int riscos_ReadTTY( char *buffer, int bufsize )
{
    int count = 0;

    fflush( stdout );

    for (;;)
    {
        int result, r2, byte;

        if ( bufsize <= 1 )
            return count;

        result = _kernel_osbyte( 129, 0, 0 );
        r2 = (result >> 8) & 0xFF;

        if ( r2 == 0x1B )
            return -1;          /* escape */
        if ( r2 == 0xFF )
            return count;       /* timeout */

	byte = result & 0xFF;

        buffer[count] = byte;
	_kernel_oswrch( byte );
        count++;
        bufsize--;

	if ( byte == 13 )
	{
	    buffer[count] = 10;
	    _kernel_oswrch( 10 );
	    count++;
	    bufsize--;
	}
    }
    return bufsize; /* never reached */
}

extern void riscos_OBJ_FoulAndGrodyHack( void );
extern void riscos_BN_init( void );

void riscos_ZM_Initialise( void )
{
#ifdef RISCOS_ZM
    riscos_OBJ_FoulAndGrodyHack();
    riscos_BN_init();
#endif
}

#undef realloc
void *riscos_ZM_realloc( void *ptr, size_t newsize )
{
    void *answer;

    if ( ptr )
    {
        int *iptr = (int*)ptr;
        int size = iptr[-1];

        answer = malloc( newsize );
        if ( !answer )
        {
            return 0;
        }

        if ( size > newsize )
            memcpy( answer, ptr, newsize );
        else
            memcpy( answer, ptr, size );
        free( ptr );
    }
    else
        answer = malloc( newsize );

    return answer;
}
