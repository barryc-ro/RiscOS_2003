/* event2.h */

#ifndef included_event2_h
#define included_event2_h

/* Any number of people can claim nulls, only one at a time can claim drags */

typedef void (*event2_null_proc)( void *handle );

void event2_claim_nulls( event2_null_proc, void *handle );
void event2_release_nulls( event2_null_proc, void *handle );

typedef void (*event2_drag_proc)( void *handle, wimp_box *final );

void event2_claim_drag( event2_drag_proc, void *handle );
void event2_release_drag( void );
/* released automatically on userdrag event */

/* Faff for dealing with icons */
void icon_setshade( wimp_w w, wimp_i i, BOOL shade );
void icon_setselect( wimp_w w, wimp_i i, BOOL select );
BOOL icon_selected( wimp_w w, wimp_i i );

#endif
