/* ExtrasLib:SubFlex.h */

/* A flex block within a flex block
 *
 * Authors:
 *      Peter Hartley       <peter@ant.co.uk>
 */


#ifndef ExtrasLib_SubFlex_h
#define ExtrasLib_SubFlex_h

#ifndef ExtrasLib_MemFlex_h
#include "memflex.h"
#endif

typedef int* subflex_ptr;

os_error *SubFlex_Initialise( flex_ptr master );

os_error *SubFlex_Alloc( subflex_ptr anchor, int size, flex_ptr master );

os_error *SubFlex_MidExtend( subflex_ptr anchor, int at, int by,
                             flex_ptr master );

os_error *SubFlex_Free( subflex_ptr anchor, flex_ptr master );

int       SubFlex_Size( subflex_ptr anchor, flex_ptr master );


#endif
