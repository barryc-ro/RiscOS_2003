/* commonsrc/objects.h */

#ifndef __objects_h
#define __objects_h

#ifndef __interface_h
#include "interface.h"
#endif

#ifndef __rid_h
#include "rid.h"
#endif

extern rid_object_type objects_type_test(int ftype);
extern void objects_check_movement(be_doc doc);
extern BOOL objects_bbox(be_doc doc, be_item ti, wimp_box *box);

#endif

/* eof objects.h */
