/* commonsrc/tables.h */
/* Table construction and manipulation routines */
/* 23-1-96 DRAFT */

#ifndef TABLES_H
#define TABLES_H

#ifndef __htmlparser_h
#include "htmlparser.h"
#endif

extern void pre_thtd_warning(HTMLCTX *me);
extern void rid_size_stream(rid_header *rh, rid_text_stream *stream, rid_fmt_info *fmt, int flags, rid_text_item *ti);
extern rid_table_cell *rid_next_root_cell(rid_table_item *table, int *xp, int *yp);
extern rid_table_cell *rid_prev_root_cell(rid_table_item *table, int *xp, int *yp);
extern rid_stdunits parse_stdunits(const char *c);
extern rid_stdunits rid_canonicalise_stdunits(rid_stdunits u);
extern void rid_table_share_width(rid_header *rh, rid_text_stream *stream, rid_text_item *orig_item, rid_fmt_info *parfmt);
extern void rid_recurse_eventual_table_positions(rid_text_item *ti, int real_top);
extern void rid_recurse_eventual_stream_positions(rid_text_stream *stream, int real_top);
extern int  dummy_tidy_proc(rid_header *rh, rid_pos_item *new_item, int width, int display_width);
extern void dummy_table_proc(rid_header *rh, rid_text_stream *stream, rid_text_item *item, rid_fmt_info *parfmt);
extern void rid_getprop(rid_table_item *table, int x, int y, int prop, void *result);
extern void table_caption_stream_origin(rid_table_item *table, int *dx, int *dy);
extern void table_cell_stream_origin(rid_table_item *table, rid_table_cell *cell, int *dx,int *dy);

typedef struct {
  	int t, b, l, r;
} border_str;

extern void table_cell_borders(rid_table_item *table, rid_table_cell *cell,
				int x, int y, border_str *bb);

#include "dump.h"


/* backend.c */
extern rid_pos_item *be_formater_loop_core(rid_header *rh, rid_text_stream *st, rid_text_item *ti, rid_fmt_info *fmt, int flags);


/* States the table state machine can be in */

enum
{
        tabstate_IDLE        ,  /* Idle */
        tabstate_PRE         ,  /* Seen <TABLE> */
        tabstate_CAPT        ,  /* Seen <CAPTION> */
        tabstate_CGRP        ,  /* Seen <COLGROUP> */
        tabstate_COL         ,  /* Seen <COL> */
        tabstate_HEAD        ,  /* Seen <HEAD> */
        tabstate_HDTR        ,  /* Seen <HEAD> <TR> */
        tabstate_FOOT        ,  /* Seen <FOOT> */
        tabstate_FTTR        ,  /* Seen <FOOT> <TR> */
        tabstate_BODY        ,  /* Seen <BODY> */
        tabstate_BDTR        ,  /* Seen <BODY> <TR> */
        tabstate_POST        ,  /* Seen </TABLE> */
        tabstate_BAD            /* Seen something we dislike */
};

extern void rid_size_table( rid_header *rh, rid_table_item *table, rid_fmt_info *parfmt );


#endif /* TABLES_H */
