/* frameutils.h */

/* Bits of frames code common to NCFresco and desktop Fresco
 * (C) ANT Limited 1997  All Rights Reserved
 */

#ifndef commonsrc_frameutils_h
#define commonsrc_frameutils_h

#ifndef __coords_h
#include "coords.h"     /* from RiscOSLib */
#endif

#ifndef __interface_h
#include "interface.h"
#endif

/* Note: some of these routines rely on the contents of a fe_view.
 * This is a different structure in Fresco and NCFresco, but the names
 * of the frame-related members are the same. Yes I know this is dodgy.
 */

int frameutils_check_resize( fe_view start, int x, int y, wimp_box *box,
                             int *handle, fe_view *resizing_v );

coords_cvtstr frameutils_get_cvt( fe_view v );

fe_view frameutils_find_top( fe_view v );

BOOL frameutils_maybe_resize( fe_view v, wimp_mousestr *m );
BOOL frameutils_are_we_resizing( void );
void frameutils_resize_abort( void );

BOOL frameutils_resize( const wimp_mousestr *m );

#endif
