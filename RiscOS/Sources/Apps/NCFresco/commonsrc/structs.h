/*
 * structs.h
 * 
 * Forward declarations of the major typedefs used. This greatly
 * simplifies a lot of external function declarations.
 */

#ifndef __structs_h
#define __structs_h

/*typedef struct rid_stdunits             *rid_stdunits;*/
#define rid_stdunits                    VALUE
typedef struct rid_table_props          rid_table_props;
typedef struct rid_width_info           rid_width_info;
typedef struct rid_table_caption        rid_table_caption;
typedef struct rid_table_cell           rid_table_cell;
typedef struct rid_table_colgroup       rid_table_colgroup;
typedef struct rid_table_rowgroup       rid_table_rowgroup;
typedef struct rid_table_item           rid_table_item;
typedef struct rid_text_item_table      rid_text_item_table;
typedef struct rid_text_stream          rid_text_stream;
typedef struct rid_table_rowhdr         rid_table_rowhdr;
typedef struct rid_table_colhdr         rid_table_colhdr;

typedef struct rid_frame                rid_frame;
typedef struct rid_frame_item           rid_frame_item;
typedef struct rid_frameset_item        rid_frameset_item;
typedef struct rid_frame_unit_totals    rid_frame_unit_totals;
typedef struct rid_area_item            rid_area_item;
typedef struct rid_map_item             rid_map_item;
typedef struct rid_fmt_info             rid_fmt_info;
typedef struct rid_fmt_state		rid_fmt_state;
typedef struct rid_meta_item            rid_meta_item;

typedef struct rid_object_param		rid_object_param;
typedef struct rid_object_item		rid_object_item;
typedef struct rid_text_item_object	rid_text_item_object;

#endif /* __structs_h */

/* eof structs.h */





