/* > mem.c

 * Various memory handling routines/wrappers

 */

#include "windows.h"

#include "client.h"
#include "logapi.h"

#include "swis.h"
typedef _kernel_oserror os_error;

#include "MemLib/memflex.h"
#include "MemLib/memheap.h"
#include "mem.h"

#include "../../app/version.h"

HGLOBAL GlobalAlloc(int flags, int size)
{
    void *ptr;

    ptr = MemHeap_malloc(size);

#ifdef MemCheck_MEMCHECK
    if (ptr)
	MemCheck_RegisterMiscBlock( ptr, size );
#endif

    return (HGLOBAL)ptr;
}

HGLOBAL GlobalFree( HGLOBAL ptr )
{
#ifdef MemCheck_MEMCHECK
    if (ptr)
	MemCheck_UnRegisterMiscBlock( ptr );
#endif

    MemHeap_free( ptr );

    return (HGLOBAL)0;
}

int FlexAlloc( flex_ptr anchor, int size)
{
    int success;

    MEMCHECK_PUSH();

    success = LOGERR(MemFlex_Alloc( anchor, size )) == NULL;

#ifdef MemCheck_MEMCHECK
    if (success)
	MemCheck_RegisterFlexBlock( anchor, size );
#endif
    
    MEMCHECK_POP();

    return success;
}

int FlexMidExtend( flex_ptr anchor, int offset, int size )
{
    int success;

    MEMCHECK_PUSH();

    success = LOGERR(MemFlex_MidExtend( anchor, offset, size )) == NULL;

#ifdef MemCheck_MEMCHECK
    if (success)
	MemCheck_ResizeFlexBlock( anchor, MemFlex_Size(anchor) );
#endif
    
    MEMCHECK_POP();

    return success;
}

void FlexFree( flex_ptr anchor )
{
    MEMCHECK_PUSH();

#ifdef MemCheck_MEMCHECK
    if (anchor && *(void **)anchor)
	MemCheck_UnRegisterFlexBlock( anchor );
#endif

    MemFlex_Free( anchor );
    
    MEMCHECK_POP();
}

void MemInit( void )
{
    LOGERR(MemFlex_Initialise2(APP_NAME " flex"));
    LOGERR(MemHeap_Initialise(APP_NAME " heap"));
}

/* eof mem.c */
