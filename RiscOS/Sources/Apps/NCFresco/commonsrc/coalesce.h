/* coalesce.h */

/* Run Fresco rid text items together to save memory
 * (C) ANT Limited 1997  All Rights Reserved
 */

#ifndef commonsrc_coalesce_h
#define commonsrc_coalesce_h

#ifndef __rid_h
#include "rid.h"
#endif

void coalesce( rid_header *rh, rid_text_item *ti, rid_text_item *dont );
void un_coalesce( rid_header *rh, rid_text_item *ti );

#endif
/* coalesce.h */

/* Run Fresco rid text items together to save memory
 * (C) ANT Limited 1997  All Rights Reserved
 */

#ifndef commonsrc_coalesce_h
#define commonsrc_coalesce_h

#ifndef __rid_h
#include "rid.h"
#endif

void coalesce( rid_header *rh, rid_pos_item *line );
void un_coalesce( rid_header *rh, rid_text_item *ti );

#endif
