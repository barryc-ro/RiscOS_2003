/* -*-c-*- rid.c */
/* Code for manipulating the RISC OS Internal Data */

/* 23-02-96 DAF         Started altering free code to handle tables     */
/* 26-02-96 DAF         Table manipulation routines                     */
/* 01-03-96 DAF         Table traversal routines (rid_scan() etc)       */
/* 03-03-96 DAF         More utility routines                           */
/* 02-04-96 SJM         Added meta_free, remove windowname              */
/* 19-07-96 SJM         changed form_element handling in connect and free */
/* 02-08-96 SJM         added form_element_enumerate function		*/


#include <stdlib.h>
#include <stdio.h>
#include "rid.h"
#include "tables.h"
#include "memwatch.h"
#include "util.h"
#include "flexwrap.h"

#include "myassert.h"
#include "profile.h"

#ifdef PLOTCHECK
#include "antweb.h"
#include <sys/types.h>
#include "../plotcheck/rectplot.h"
#endif

#ifndef TABLE_DEBUG
#define TABLE_DEBUG 0
#endif

#ifndef RID_FREE_DEBUG
#define RID_FREE_DEBUG 0
#endif

#ifndef ITERATIVE_PANIC
#define ITERATIVE_PANIC 0
#endif

#if 0
#define SCNDBG(a) DBG(a)
#else
#define SCNDBG(a)
#endif

/*****************************************************************************

  We now do a lot of freeing and allocating of rid_pos_item chains. To speed
  this up, and induce less heap fragmentation, we cache pos chains and
  allocate from this list if possible.

 */

/*static rid_pos_item *pos_cache_head, *pos_cache_tail;*/

/*****************************************************************************/

/*static void rid_free_table(rid_table_item *table);*/
static rid_text_item *scanfwd_recurse_if_table(rid_text_item *item);
static rid_text_item *scanfwd_recurse_first_in_caption(rid_table_item *table);
static rid_text_item *scanfwd_recurse_first_in_table(rid_table_item *table);
static rid_text_item *scanfwd_recurse_items_parent(rid_text_item *item);
static rid_text_item *scanfwd_recurse_after_table(rid_table_item *table);
static rid_text_item *scanfwd_recurse_cell_after(rid_table_item *table, int x, int y);
static rid_text_item *scanback_recurse_parent_prev(rid_text_item *item);
static rid_text_item *scanback_recurse_tables_caption(rid_table_item *table);
static rid_text_item *scanback_recurse_cell_before(rid_table_item *table, int x, int y);
static rid_text_item *scanback_recurse_if_table(rid_text_item *item);
static rid_text_item *scanback(rid_text_item *item);
static rid_text_item *scanfwd_recurse(rid_text_item *item);
static rid_text_item *scanback_recurse(rid_text_item *item);

#define MEMZONE_CHUNK_SIZE	1024

#if 0
extern void nullfree(void **vpp)
{
        if (vpp != NULL)
        {
                if (*vpp != NULL)
                {
                        mm_free(*vpp);
                        *vpp = NULL;
                }
        }
}
#endif

static void rid_free_frame(rid_frame *p)
{
    while (p)
    {
        rid_frame *next = p->next;

        switch (p->tag)
        {
            case rid_frame_tag_FRAME:
                if (p->data.frame.name)
                    mm_free(p->data.frame.name);
                if (p->data.frame.src)
                    mm_free(p->data.frame.src);
                break;

            case rid_frame_tag_FRAMESET:
                if (p->data.frameset.widths)
                    mm_free(p->data.frameset.widths);
                if (p->data.frameset.heights)
                    mm_free(p->data.frameset.heights);

                rid_free_frame(p->data.frameset.frame_list);
                break;
        }

        mm_free(p);
        p = next;
    }
}

static void rid_free_area(rid_area_item *p)
{
    while (p)
    {
	rid_area_item *next = p->next;

        if (p->type == rid_area_POLYGON)
            mm_free(p->coords.polygon.point);

	if (p->target)
	    mm_free(p->target);
	if (p->href)
	    mm_free(p->href);
	if (p->alt)
	    mm_free(p->alt);

	mm_free(p);

	p = next;
    }
}

static void rid_free_map(rid_map_item *p)
{
    while (p)
    {
	rid_map_item *next = p->next;

        rid_free_area(p->area_list);

	if (p->name)
	    mm_free(p->name);

	mm_free(p);

	p = next;
    }
}

/* Free list of pos items, stopping just before the specified
   terminating pos item, making term the head of the pos list and
   messing with pointers as needed. Note that we need to NULL
   rid_text_item->line pointers, else they will reference memory that
   has been released. */

extern void rid_free_pos_term(rid_pos_item *p, rid_pos_item *term, rid_text_stream *stream)
{
    rid_float_item *fl = NULL;

    PINC_FREE_POS_TERM;

    while (p != term && p != NULL)
    {
	rid_pos_item *next = p->next ;
	rid_text_item *ti = p->first;

	PINC_FREE_POS_TERM_ITERS;

	/* NULL pointers for non-floating items */
	for (; ti != NULL; ti = rid_scanf(ti))
	{
	    if ( !FLOATING_ITEM(ti) )
	    {
		if (ti->line != p)
		    break;
		ti->line = NULL;
	    }
	}

	if (p->floats)
	{
            for (fl = p->floats->left; fl != NULL; )
            {
                rid_float_item *nextfl = fl->next;
		/* NULL pointers for floating items */
		fl->ti->line = NULL;
                mm_free(fl);
                fl = nextfl;
            }

            for (fl = p->floats->right; fl != NULL; )
            {
                rid_float_item *nextfl = fl->next;
		fl->ti->line = NULL;
                mm_free(fl);
                fl = nextfl;
            }

	    mm_free(p->floats);
	}

	mm_free(p);
	p = next;
    }

    if (term == NULL)
    {
	/* NB: This case should probably not be required, but just in case */
	stream->pos_list = stream->pos_last = NULL;
    }
    else
    {
	term->prev = NULL;
	stream->pos_list = term;
    }
}

/* Free list of pos items. */

extern void rid_free_pos(rid_pos_item *p)
{
    rid_float_item *fl = NULL;

    PINC_FREE_POS;

    while (p != NULL)
    {
	rid_pos_item *next = p->next ;

	PINC_FREE_POS_ITERS;

	if (p->floats)
	{
	    /* pdh: in the New Formatter, floats are replicated (i think) */

            rid_float_item *nextfl;

            fl = p->floats->left;
            while ( fl )
            {
                nextfl = fl->next;
                mm_free(fl);
                fl = nextfl;
            }

            fl = p->floats->right;
            while ( fl )
            {
                nextfl = fl->next;
                mm_free(fl);
                fl = nextfl;
            }

            /*FMTDBGN(( "pi%p: freeing floats %p\n", p, p->floats ));*/
	    mm_free(p->floats);
	}

	mm_free(p);
	p = next;
    }
}

/* Free list of pos items with a bit more. */
/* If an item is a table, free all it's pos lists and set the pos list */
/* pointers in each stream to NULL (this last is very important!) */

extern void rid_free_pos_tree(rid_pos_item *p)
{
    rid_text_item *ti;

    /*FMTDBGN(("rid_free_pos_tree(%p)\n", p));*/

    if (p == NULL)
	return;

#if RID_FREE_DEBUG
    fprintf(stderr, "Freeing pos tree\n");
#endif

    for (ti = p->first; ti != NULL; ti = rid_scanf(ti))
    {
	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table *)ti)->table;
	    rid_table_cell *cell;
	    int x = -1, y = 0;

#if RID_FREE_DEBUG
	    fprintf(stderr, "Freeing table pos\n");
#endif
	    if (table->caption)
	    {
#if RID_FREE_DEBUG
		fprintf(stderr, "Freeing caption pos\n");
#endif
		rid_free_pos_tree(table->caption->stream.pos_list);
		table->caption->stream.pos_list = NULL;
		table->caption->stream.pos_last = NULL;
	    }
	    while ( (cell = rid_next_root_cell(table, &x, &y)) != NULL )
	    {
#if RID_FREE_DEBUG
		fprintf(stderr, "Freeing cell(%d,%d) pos\n", x, y);
#endif
		rid_free_pos_tree(cell->stream.pos_list);
		cell->stream.pos_list = NULL;
		cell->stream.pos_last = NULL;
	    }
	}
    }

#if 1
    /* pdh: why not just... */
    rid_free_pos(p);
#else

    while (p)
    {
	rid_pos_item *next = p->next;

	if (p->floats)
	{
	    if (p->floats->left)
	    {
		if (fl != p->floats->left)
		{
		    if (fl)
		    {
#if RID_FREE_DEBUG
			fprintf(stderr, "Freeing float l %p\n", fl);
#endif
			mm_free(fl);
		    }
		    fl = p->floats->left;
		}
	    }
	    if (p->floats->right)
	    {
		if (fr != p->floats->right)
		{
		    if (fr)
		    {
#if RID_FREE_DEBUG
			fprintf(stderr, "Freeing float r %p\n", fr);
#endif
			mm_free(fr);
		    }
		    fr = p->floats->right;
		}
	    }
#if RID_FREE_DEBUG
	    fprintf(stderr, "Freeing float %p\n", p->floats);
#endif
	    mm_free(p->floats);
	}

#if RID_FREE_DEBUG
	fprintf(stderr, "Freeing pos %p\n", p);
#endif
	mm_free(p);
	p = next;
    }

    if (fl)
    {
#if RID_FREE_DEBUG
	fprintf(stderr, "Freeing float l %p\n", fl);
#endif
	mm_free(fl);
    }

    if (fr)
    {
#if RID_FREE_DEBUG
	fprintf(stderr, "Freeing float r %p\n", fr);
#endif
	mm_free(fr);
    }
#endif
}

void rid_free_object(rid_object_item *obj)
{
    rid_object_param *param = obj->params;

    while (param)
    {
	rid_object_param *next = param->next;
	mm_free(param->name);
	mm_free(param->value);
	mm_free(param->type);
	mm_free(param);
	param = next;
    }

    mm_free(obj->classid);
    mm_free(obj->classid_mime_type);
    mm_free(obj->data);
    mm_free(obj->data_mime_type);
    mm_free(obj->codebase);
    mm_free(obj->usemap);
    mm_free(obj->standby);
    mm_free(obj->name);

    rid_free_map(obj->map);

    mm_free(obj);
}

extern void rid_free_text(rid_text_item *p)
{
    while (p)
    {
	rid_text_item *next = p->next;

	switch (p->tag)
	{
	case rid_tag_TEXT:
	    {
#if 0				/* This text is all in one big memzone now */
		rid_text_item_text *tit = (rid_text_item_text *) p;
		if (tit->data)
		    mm_free(tit->data);
#endif
		break;
	    }
	case rid_tag_IMAGE:
	    {
		rid_text_item_image *tii = (rid_text_item_image *) p;

		if (tii->src)
		    mm_free(tii->src);
		if (tii->alt)
		    mm_free(tii->alt);
		if (tii->usemap)
		    mm_free(tii->usemap);
		break;
	    }
        case rid_tag_TABLE:
            {
#if TABLE_DEBUG
                fprintf(stderr, "Free table %p\n", p);
#endif
                rid_free_table( ((rid_text_item_table *)p)->table );
                break;
            }

	case rid_tag_OBJECT:
	    rid_free_object(((rid_text_item_object*)p)->object);
	    break;
	}

	mm_free(p);
	p = next;
    }
}

extern void rid_free_aref(rid_aref_item *p)
{
    while (p)
    {
	rid_aref_item *next = p->next;

	if (p->name)
	    mm_free(p->name);

	if (p->href)
	    mm_free(p->href);

	if (p->rel)
	    mm_free(p->rel);

	if (p->target)
	    mm_free(p->target);

	if (p->title)
	    mm_free(p->title);

	mm_free(p);
	p = next;
    }
}

#if !NEW_TEXTAREA
extern void rid_free_textarea_lines(rid_textarea_line *p)
{
    while (p)
    {
	rid_textarea_line *next = p->next;

	if (p->text)
	    mm_free(p->text);
	mm_free(p);
	p = next;
    }
}
#endif

extern void rid_free_select_options(rid_option_item *p)
{
    while (p)
    {
	rid_option_item *next = p->next;

	if (p->text)
	    mm_free(p->text);
	if (p->value)
	    mm_free(p->value);

	mm_free(p);
	p = next;
    }
}

extern void rid_free_form_elements(rid_form_element *f)
{
    while (f)
    {
	rid_form_element *next = f->next;

	switch (f->tag)
	{
	case rid_form_element_INPUT:
	{
	    rid_input_item *p = (rid_input_item *)f;
	    mm_free(p->name);
	    mm_free(p->value);
	    mm_free(p->src);
	    mm_free(p->src_sel);

	    switch(p->tag)
	    {
	    case rid_it_TEXT:
	    case rid_it_PASSWD:
		mm_free(p->data.str);
		break;
	    }
	    break;
	}

	case rid_form_element_TEXTAREA:
	{
	    rid_textarea_item *p = (rid_textarea_item *)f;
	    if (p->name)
		mm_free(p->name);

#if NEW_TEXTAREA
	    memzone_destroy(&p->default_text);
	    /* other zone is freed in otextarea_dispose() */
#else
	    rid_free_textarea_lines(p->default_lines);
	    rid_free_textarea_lines(p->lines);
#endif
	    break;
	}

	case rid_form_element_SELECT:
	{
	    rid_select_item *p = (rid_select_item *)f;
	    if (p->name)
		mm_free(p->name);
	    rid_free_select_options(p->options);
	    break;
	}
	}

	mm_free(f->id);
	mm_free(f);
	f = next;
    }
}

extern void rid_free_form(rid_form_item *p)
{
    while (p)
    {
	rid_form_item *next = p->next;

	rid_free_form_elements(p->kids);

	if (p->action)
	    mm_free(p->action);

	if (p->target)
	    mm_free(p->target);

	mm_free(p->id);

	mm_free(p->enc_type);

	mm_free(p);

	p = next;
    }
}

static void rid_free_meta(rid_meta_item *p)
{
    while (p)
    {
	rid_meta_item *next = p->next;

	mm_free(p->content);
	mm_free(p->httpequiv);
	mm_free(p->name);
	mm_free(p);
	p = next;
    }
}

/*****************************************************************************

    Free all descendents of the table and then free the table and associated
    storage.  Can't rely upon pos list!

*/

extern void rid_free_props(rid_table_props *props)
{
    if (props != NULL)
    {
  	nullfree(&props->lang);
  	nullfree(&props->style);
  	mm_free(props);		/* SJM: changed from nullfree */
    }
}

extern void rid_free_colgroup(rid_table_colgroup *colgroup)
{
    if (colgroup != NULL)
    {
  	nullfree((void**)&colgroup->id);
  	nullfree((void**)&colgroup->class);
  	rid_free_props(colgroup->props);
  	mm_free(colgroup); /* SJM: added */
    }
}

extern void rid_free_colhdr(rid_table_colhdr *colhdr)
{
    if (colhdr != NULL)
    {
      	rid_free_props(colhdr->props);
      	nullfree((void**)&colhdr->id);
      	nullfree((void**)&colhdr->class);
  	mm_free(colhdr); /* SJM: added */
    }
}

extern void rid_free_rowgroup(rid_table_rowgroup *rowgroup)
{
    if (rowgroup != NULL)
    {
     	rid_free_props(rowgroup->props);
     	nullfree((void**)&rowgroup->id);
     	nullfree((void**)&rowgroup->class);
  	mm_free(rowgroup); /* SJM: added */
    }
}

extern void rid_free_rowhdr(rid_table_rowhdr *rowhdr)
{
    if (rowhdr != NULL)
    {
     	rid_free_props(rowhdr->props);
     	nullfree((void**)&rowhdr->id);
     	nullfree((void**)&rowhdr->class);
  	mm_free(rowhdr); /* SJM: added */
    }
}


extern void rid_free_caption(rid_table_caption *caption)
{
    if (caption != NULL)
    {
        rid_free_text(caption->stream.text_list);
        nullfree((void**)&caption->id);
        nullfree((void**)&caption->class);
        rid_free_props(caption->props);
        mm_free(caption);		/* SJM: changed from nullfree */
    }
}


extern void rid_free_table(rid_table_item *table)
{
        rid_table_cell *cell;
        int x = -1, y = 0;

#if TABLE_DEBUG
        fprintf(stderr, "rid_table_free(%p)\n", table);
#endif

        if (table == NULL)
                return;

        /* Free descendent items first */
        /* This will free all items in all text streams in the */
        /* table, but nothing else from the cell/caption */

        rid_free_caption(table->caption);
        while ( (cell = rid_next_root_cell(table, &x, &y)) != NULL )
        {
                rid_free_text(cell->stream.text_list);
        }

        /* Then free associated storage */


        x = -1, y = 0;
        while ( (cell = rid_next_root_cell(table, &x, &y)) != NULL )
        {
		int a,b;

		for (a = 0; a < cell->span.x; a++)
		  	for (b = 0; b < cell->span.y; b++)
				*CELLFOR(table, x + a, y + b) = NULL;

                nullfree((void**)&cell->id);
                nullfree((void**)&cell->axes);
                nullfree((void**)&cell->axis);
                nullfree((void**)&cell->class);
                rid_free_props(cell->props);
		nullfree((void**)&cell);
       }

	for (x = 0; x < table->cells.x; x++)
		rid_free_colhdr(table->colhdrs[x]);
	for (y = 0; y < table->cells.y; y++)
		rid_free_rowhdr(table->rowhdrs[y]);
	for (x = 0; x < table->num_groups.x; x++)
		rid_free_colgroup(table->colgroups[x]);
	for (y = 0; y < table->num_groups.y; y++)
		rid_free_rowgroup(table->rowgroups[y]);

        nullfree((void**)&table->array);
        nullfree((void**)&table->colhdrs);
        nullfree((void**)&table->rowhdrs);
    	nullfree((void**)&table->colgroups);
        nullfree((void**)&table->rowgroups);
        nullfree((void**)&table->id);
        nullfree((void**)&table->class);
	nullfree((void**)&table->minpct);
	nullfree((void**)&table->maxpct);
	nullfree((void**)&table->minabs);
	nullfree((void**)&table->maxabs);
	nullfree((void**)&table->cumminabs);
	nullfree((void**)&table->cummaxabs);
        rid_free_props(table->props);
        nullfree((void**)&table);
}

extern void rid_free(rid_header *rh)
{
    /* Need to free all the chains of information, all the strings etc. */

    rid_free_pos_tree(rh->stream.pos_list);
    /* This will recurse to free any tables */
    rid_free_text(rh->stream.text_list);
    rid_free_aref(rh->aref_list);
    rid_free_form(rh->form_list);
    rid_free_frame(rh->frames);
    rid_free_map(rh->map_list);
    rid_free_meta(rh->meta_list);

    memzone_destroy(&(rh->texts));

    if (rh->title)
	mm_free(rh->title);

    if (rh->base)
	mm_free(rh->base);

    if (rh->tile.src)
	mm_free(rh->tile.src);

    if (rh->refreshurl)
	mm_free(rh->refreshurl);

    mm_free(rh);
}


/*****************************************************************************/

extern void rid_frame_connect(rid_frame *f_top, rid_frame *f)
{
    rid_frameset_item *fs = &f_top->data.frameset;
    if (fs->frame_list)
    {
	fs->frame_last->next = f;
	fs->frame_last = f;
    }
    else
    {
	fs->frame_list = fs->frame_last = f;
    }
}

extern void rid_area_item_connect(rid_map_item *m, rid_area_item *a)
{
    if (m->area_list)
    {
	m->area_last->next = a;
	m->area_last = a;
    }
    else
    {
	m->area_list = m->area_last = a;
    }
}

extern void rid_scaff_item_push(rid_text_stream *st, int flags)
{
    rid_text_item *scaff = mm_calloc(1, sizeof(*scaff));

    scaff->tag = rid_tag_SCAFF;
    scaff->flag = flags;

    if (st->text_list)
    {
	st->text_last->next = scaff;
	st->text_last = scaff;
    }
    else
    {
	st->text_last = st->text_list = scaff;
    }
}

extern void rid_text_item_connect(rid_text_stream *st, rid_text_item *t)
{
    PINC_TEXT_ITEM_CONNECT;

    if (st == NULL || st == TABLE_NULL)
    {
	/* @@@@ Work out why this happens! */
#ifdef PLOTCHECK
	SAY("rid_text_item_connect(%p, %p): NULL !!!!!\n", st, t);
	plotcheck_boom();
#endif
	usrtrc("rid_text_item_connect: NULL stream\n");
	return;
    }

    if (st->text_list == NULL)
	rid_scaff_item_push(st, rid_flag_NO_BREAK);

#if 1
    st->text_last->next = t;
    st->text_last = t;
#else
    /* moved to separate function called before the main push */
    if (st->text_list)
    {
	st->text_last->next = t;
	st->text_last = t;
    }
    else
    {
	/* This is the first item in the stream. Insert a magic
           scaffold object first. */
	rid_text_item *scaff = mm_calloc(1, sizeof(*scaff));

	FMTDBGN(("rid_text_item_connect: inserted rid_tag_SCAFF object\n"));

	scaff->tag = rid_tag_SCAFF;
	scaff->flag = rid_flag_NO_BREAK;
	scaff->next = t;
	st->text_list = scaff;
	st->text_last = t;
    }
#endif
}

extern void rid_pos_item_connect(rid_text_stream *st, rid_pos_item *p)
{
    /*FMTDBGN(("rid_pos_item_connect(%p, %p)\n", st, p));*/

    PINC_POS_ITEM_CONNECT;

    if (st->pos_list)
    {
	st->pos_last->next = p;
	p->prev = st->pos_last;
	st->pos_last = p;
    }
    else
    {
	st->pos_list = st->pos_last = p;
    }
}

extern void rid_aref_item_connect(rid_header *rh, rid_aref_item *a)
{
    if (rh->aref_list)
    {
	rh->aref_last->next = a;
	a->prev = rh->aref_last;
	rh->aref_last = a;
    }
    else
    {
	rh->aref_list = rh->aref_last = a;
    }
}

extern void rid_form_item_connect(rid_header *rh, rid_form_item *f)
{
    if (rh->form_list)
    {
	rh->form_last->next = f;
	f->prev = rh->form_last;
	rh->form_last = f;
    }
    else
    {
	rh->form_list = rh->form_last = f;
    }
}

extern void rid_form_element_connect(rid_form_item *f, rid_form_element *i)
{
    if (f->kids)
    {
	f->last_kid->next = i;
	i->prev = f->last_kid;
	f->last_kid = i;
    }
    else
    {
	f->kids = f->last_kid = i;
    }

    i->parent = f;
}

extern void rid_option_item_connect(rid_select_item *s, rid_option_item *o)
{
    if (s->options)
    {
	s->last_option->next = o;
	o->prev = s->last_option;
	s->last_option = o;
    }
    else
    {
	s->options = s->last_option = o;
    }
}

extern void rid_meta_connect(rid_header *rh, rid_meta_item *m)
{
    m->next = rh->meta_list;
    rh->meta_list = m;
}

/*****************************************************************************/

extern int memzone_init(memzone *mz, int flags)
{
    int ok;

    mz->flags = flags;

    mz->used = 0;

    mz->alloced = (flags & MEMZONE_CHUNKS) ? MEMZONE_CHUNK_SIZE : 16;

#if ITERATIVE_PANIC
    do
    {
	ok = flex_alloc((flex_ptr) &(mz->data), mz->alloced);

	if (ok)
	    break;
    }
    while (mm_can_we_recover(FALSE));

    if (!ok)
	mz->alloced = 0;
#else
    ok = flex_alloc((flex_ptr) &(mz->data), mz->alloced);

    if (!ok)
    {
	if (mm_can_we_recover(FALSE) == 1)
	    ok = flex_alloc((flex_ptr) &(mz->data), mz->alloced);
	if (!ok)
	    mz->alloced = 0;
    }
#endif
    return ok;
}

extern int memzone_alloc(memzone *mz, int size)
{
    int need;
    int rval;

    if (mz->flags & MEMZONE_WORDS)
	size = (size + 3) & (~3);

    need = size - (mz->alloced - mz->used);

    if (need > 0)
    {
	int oldsize = mz->alloced;
	BOOL ok;

	if (mz->flags & MEMZONE_CHUNKS)
	    need = ((need / MEMZONE_CHUNK_SIZE) + 1) * MEMZONE_CHUNK_SIZE;

	mz->alloced += need;

#if ITERATIVE_PANIC
	do
	{
	    ok = flex_extend((flex_ptr) &(mz->data), mz->alloced);

	    if (ok)
		break;
	}
	while (mm_can_we_recover(FALSE));

	if (!ok)
	{
	    mz->alloced = oldsize;

	    return -1;
	}
#else
	ok = flex_extend((flex_ptr) &(mz->data), mz->alloced);

	/* SJM: try and recover if extend fails */
	if (!ok && mm_can_we_recover(FALSE) == 1)
	    ok = flex_extend((flex_ptr) &(mz->data), mz->alloced);

	if (!ok)
	{
	    mz->alloced = oldsize;

	    return -1;
	}
#endif
    }

    rval = mz->used;
    mz->used += size;

    return rval;
}

extern int memzone_tidy(memzone *mz)
{
    int ok;

    ok = flex_extend((flex_ptr) &(mz->data), mz->used);

    if (ok)
	mz->alloced = mz->used;

    return ok;
}

extern void memzone_destroy(memzone *mz)
{
    flex_free((flex_ptr) &(mz->data));
}

/*****************************************************************************

    Return either the supplied item or the top most parent rid_text_item.
    Requires pos list to work.  The result is always an item in the top level
    chain - ie findable through rh->stream.text_list and rid_scanf().

*/

extern rid_text_item *rid_outermost_item(rid_text_item *item)
{
        rid_text_stream *stream;

        if (item != NULL && item->line != NULL && (stream = item->line->st) != NULL )
        {
                if (stream->partype == rid_pt_CAPTION)
                {
                        rid_table_caption *caption = (rid_table_caption *) stream->parent;
                        return rid_outermost_item( (rid_text_item *) caption->table->parent );
                }
                else if (stream->partype == rid_pt_CELL)
                {
                        rid_table_cell *cell = (rid_table_cell *) stream->parent;
                        return rid_outermost_item( (rid_text_item *) cell->parent->parent );
                }
        }

        return item;
}

/*****************************************************************************

    These are support routines for the rid_scan() functionality.  They handle
    the recursion and leaping around necessary.  Since introducing the
    constructional requirement that all tables have a non-table item
    immediately adjacent to them, this is slightly overkill.

*/

static rid_text_item *scanfwd_recurse_if_table(rid_text_item *item)
{
    SCNDBG(("scanfwd_recurse_if_table: %p\n", item));

        if (item == NULL)
                return NULL;
        if (item->tag != rid_tag_TABLE)
                return item;

        return scanfwd_recurse_first_in_table( ((rid_text_item_table *)item)->table );
}

static rid_text_item *scanfwd_recurse_first_in_caption(rid_table_item *table)
{
    SCNDBG(("scanfwd_recurse_first_in_caption: %p\n", table));

  	if (table == NULL)
		return NULL;

        if (table->caption == NULL || table->caption->stream.text_list == NULL )
                return scanfwd_recurse_cell_after(table, -1, 0);

        return table->caption->stream.text_list;
}

static rid_text_item *scanfwd_recurse_first_in_table(rid_table_item *table)
{
    SCNDBG(("scanfwd_recurse_first_if_table: %p\n", table));

        return scanfwd_recurse_first_in_caption(table);
}

static rid_text_item *scanfwd_recurse_items_parent(rid_text_item *item)
{
        rid_pos_item *pos;
        rid_text_stream *stream;

    SCNDBG(("scanfwd_recurse_items_parent: %p line %p\n", item, item->line));
    SCNDBG(("scanfwd_recurse_items_parent: stream %p\n", item->line ? item->line->st : 0));
    SCNDBG(("scanfwd_recurse_items_parent: stream parent %p\n", item->line && item->line->st ? item->line->st->parent : 0));

	if (item == NULL)
		return NULL;

	if ((pos = item->line) == NULL)
		return NULL;

	ASSERT(pos->st != NULL);

	if ((stream = pos->st) == NULL)
		return NULL;

        if (stream->partype == rid_pt_CAPTION)
        {
                rid_table_caption *capt = (rid_table_caption *) stream->parent;
                return scanfwd_recurse_cell_after(capt->table, -1, 0);
        }

        if (stream->partype == rid_pt_CELL)
        {
                rid_table_cell *cell = (rid_table_cell *) stream->parent;
                return scanfwd_recurse_cell_after(cell->parent, cell->cell.x, cell->cell.y);
        }

        return NULL;
}

static rid_text_item *scanfwd_recurse_after_table(rid_table_item *table)
{
        rid_text_item_table *rtit;

    SCNDBG(("scanfwd_recurse_after_table: %p\n", table));

        if (table == NULL)
        	return NULL;

	rtit = table->parent;

        if (rtit->base.next != NULL)
                return rtit->base.next;

        return scanfwd_recurse_items_parent(&rtit->base);
}

static rid_text_item *scanfwd_recurse_cell_after(rid_table_item *table, int x, int y)
{
        rid_table_cell *cell;

    SCNDBG(("scanfwd_recurse_cell_after: %p %d,%d\n", table, x, y));

	if (table == NULL)
		return NULL;

        while ( (cell = rid_next_root_cell(table, &x, &y)) != NULL )
        {
                if ( cell->stream.text_list != NULL )
                        return cell->stream.text_list;
        }

        return scanfwd_recurse_after_table(table);
}

static rid_text_item *scanback_recurse_parent_prev(rid_text_item *item)
{
        rid_pos_item *pos;
        rid_text_stream *stream;

    SCNDBG(("scanfwd_recurse_parent_prev: %p\n", item));

	if (item == NULL)
		return NULL;

	if ( (pos = item->line) == NULL)
		return NULL;

	if ( (stream = pos->st) == NULL)
		return NULL;

        if (stream->partype == rid_pt_CAPTION)
        {
                rid_table_caption *capt = (rid_table_caption *) stream->parent;
                rid_table_item *table = capt->table;

                return scanback( (rid_text_item *) table->parent );
        }

        if (stream->partype == rid_pt_CELL)
        {
                rid_table_cell *cell = (rid_table_cell *) stream->parent;
                return scanback_recurse_cell_before(cell->parent, cell->cell.x, cell->cell.y);
        }

        return scanback(item);
}

static rid_text_item *scanback_recurse_tables_caption(rid_table_item *table)
{
    SCNDBG(("scanfwd_recurse_tables_caption: %p\n", table));

  	if (table == NULL)
  		return NULL;

        if (table->caption && table->caption->stream.text_list)
        {
                rid_text_item *item = table->caption->stream.text_list;
                while (item->next)
                        item = item->next;
                return item;
        }

        return scanback((rid_text_item *) table->parent);
}

static rid_text_item *scanback_recurse_cell_before(rid_table_item *table, int x, int y)
{
        rid_table_cell *cell;

    SCNDBG(("scanfwd_recurse_cell_before: %p %d,%d\n", table, x, y));

	if (table == NULL)
		return NULL;

        while ( (cell = rid_prev_root_cell(table, &x, &y)) != NULL )
        {
                if ( cell->stream.text_list != NULL )
                {
                        rid_text_item *item = cell->stream.text_list;
                        while (item->next)
                                item = item->next;
                        return item;
                }
        }

        return scanback_recurse_tables_caption(table);
}

static rid_text_item *scanback_recurse_if_table(rid_text_item *item)
{
    SCNDBG(("scanfwd_recurse_if_table: %p\n", item));

	if (item == NULL)
  		return NULL;

        if (item->tag == rid_tag_TABLE)
        {
                rid_table_item *table = ((rid_text_item_table *)item)->table;
                return scanback_recurse_cell_before(table, table->cells.x, table->cells.y - 1);
        }

        return item;
}


/*****************************************************************************

Three routines to handle the basic variants of rid_scan(). Might well be
incorporated into rid_scan() these days.

*/

static rid_text_item *scanback(rid_text_item *item)
{
        rid_pos_item *pos;
        rid_text_item *new;

        if (item == NULL)
                return NULL;

        if ( (pos = item->line) == NULL)
                return NULL;

	switch (item->tag)
	{
	case rid_tag_TABLE:
	{
	    rid_text_item_table *tit = (rid_text_item_table *)item;
#if DEBUG
	    fprintf(stderr, "scanback on table %p - prev ptr is %p\n", item, tit->table->prev);
#endif
	    /* SJM: if null then fall through to old case */
	    if (tit->table->prev != NULL)
		return tit->table->prev;
	}
	break;

	case rid_tag_IMAGE:
	{
	    rid_text_item_image *tii = (rid_text_item_image *)item;
#if DEBUG
	    fprintf(stderr, "scanback on image %p - prev ptr is %p\n", item, tii->prev);
#endif
	    /* SJM: if null then fall through to old case */
	    if (tii->prev != NULL)
		return tii->prev;
	}
	break;
	}

        if (pos->first != item)
        {       /* Item is in this list */
                for (new = pos->first; new && new->next != item; new = new->next)
                        ;
                return new;
        }

        do
        {       /* Find earlier line with something in it */
                if ( (pos = pos->prev) == NULL )
                        return NULL;
        } while (pos->first == NULL);

	ASSERT(pos->next != NULL);

        /* Find last item of this line */

        for (item = pos->first;
       	     item && item->next && item->next != pos->next->first;
       	     item = item->next)
                ;

        return item;
}

static rid_text_item *scanfwd_recurse(rid_text_item *item)
{
    SCNDBG(("scanfwd_recurse: %p\n", item));

        if (item == NULL)
                return NULL;

        if (item->tag == rid_tag_TABLE)
                return scanfwd_recurse_first_in_table( ((rid_text_item_table *)item)->table );

        if (item->next != NULL)
                return scanfwd_recurse_if_table(item->next);

        return scanfwd_recurse_items_parent(item);
}

static rid_text_item *scanback_recurse(rid_text_item *item)
{
        rid_text_item *new;

        if (item == NULL)
                return NULL;

        if ( (new = scanback(item)) != NULL )
                return scanback_recurse_if_table(new);

        return scanback_recurse_parent_prev(item);
}

/*****************************************************************************/

/* Presented interface */

extern rid_text_item * rid_scan(rid_text_item * item, int action)
{
    SCNDBG(("rid_scan: %p %x\n", item, action));

	if (item == NULL)
                return NULL;

        /* Call ourselves for the filter stuff for now */
        if ( (action & SCAN_FILTER) != 0 )
        {
                rid_tag tag = (rid_tag) (action & rid_tag_MASK);
                action &= ~SCAN_FILTER;

		/* SJM: extra flag to consider the item passed in */
		if ((action & SCAN_THIS) &&
		    ((item->tag == tag) ^ ((action & SCAN_EXCLUDE) != 0)))
		    return item;

		while (item != NULL)
                {
                        item = rid_scan(item, action);
                        if ( item && ((item->tag == tag) ^ ((action & SCAN_EXCLUDE) != 0)) ) /* 211096 wasn't checking item==NULL here */
                                return item;
                }
                return NULL;
        }

        if ( (action & SCAN_BACK) == 0 )
        {       /* Scanning forward is easier */
                item = (action & SCAN_RECURSE) ? scanfwd_recurse(item) : item->next;
        }
        else
        {       /* Takes a bit more work */
                item = (action & SCAN_RECURSE) ? scanback_recurse(item) : scanback(item);
        }

        return item;
}

/*****************************************************************************

  Given a rid_text_item that is floating and a rid_pos_item that it is
  believed to be floating on, return either NULL or the rid_float_item
  on the line referencing the floater.  */

extern rid_float_item * rid_get_float_item(rid_text_item *ti, rid_pos_item *pi)
{
    rid_float_item *fi;

    ASSERT(ti != NULL);
    ASSERT(pi != NULL);
    TASSERT( FLOATING_ITEM(ti) );

    /* No floaters, so dumb question */
    if (pi->floats == NULL)
	return NULL;

    if ( (ti->flag & rid_flag_LEFTWARDS) != 0 )
	fi = pi->floats->left;
    else
	fi = pi->floats->right;

    while (fi != NULL && fi->ti != ti)
	fi = fi->next;

    return fi;
}

/*****************************************************************************

    Support routine for when about to reformat.  Previously, we would set
    stream->widest = stream->height = 0. This does much the same over any
    contained text streams as well.

*/

extern void rid_zero_widest_height_from_item(rid_text_item *item)
{
    FMTDBGN(("rid_zero_widest_height_from_item(%p)\n", item));

    for (; item != NULL; item = rid_scanf(item))
    {
	if (item->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table *)item)->table;
	    rid_table_cell *cell;
	    int x = -1, y = 0;
	    if (table->caption)
		rid_zero_widest_height(&table->caption->stream);
	    while ( (cell = rid_next_root_cell(table, &x, &y)) != NULL )
		rid_zero_widest_height(&cell->stream);
	}
    }
}

extern void rid_zero_widest_height(rid_text_stream *stream)
{
    FMTDBGN(("rid_zero_widest_height(%p)\n", stream));

    if (stream != NULL)
    {
	stream->widest = stream->height = 0;
	rid_zero_widest_height_from_item(stream->text_list);
    }
}

/*****************************************************************************

    Clone a pos list.  This has only been properly thought through for the
    top level pos list when used within antweb_doc_image_change().  If you're
    going to use this anywhere else, then check everything is still
    consistent.  Relies on mm_calloc() not returning a NULL pointer!

*/

extern rid_pos_item *rid_clone_pos_list(rid_pos_item *pos)
{
        rid_pos_item *first, *new, *prev;

	PINC_CLONE_POS_LIST;

        if (pos == NULL)
                return NULL;

        first = mm_calloc(1, sizeof(*new));
        *first = *pos;
        first->prev = NULL;
        first->next = NULL;
	first->floats = NULL;
        prev = first;
        pos = pos->next;

        for (; pos != NULL; pos = pos->next, prev = new)
        {
                new = mm_calloc(1, sizeof(*new));
                *new = *pos;
                new->prev = prev;
                prev->next = new;
                new->next = NULL;
		new->floats = NULL;
        }

        return first;
}

/*****************************************************************************/

/* Some useful form scanning functions */

extern char *form_get_element_name(rid_form_element *fe)
{
    switch (fe->tag)
    {
    case rid_form_element_INPUT:
	return ((rid_input_item *)fe)->name;
    case rid_form_element_SELECT:
	return ((rid_select_item *)fe)->name;
    case rid_form_element_TEXTAREA:
	return ((rid_textarea_item *)fe)->name;
    }
    return NULL;
}

extern void form_element_enumerate(rid_form_item *form, int element_type, int tag, const char *name, form_element_enumerate_fn fn, void *handle)
{
    rid_form_element *fe;

    for (fe = form->kids; fe; fe = fe->next)
    {
	if (element_type == fe->tag)
	{
	    char *el_name = form_get_element_name(fe);

	    if ((element_type != rid_form_element_INPUT || ((rid_input_item *)fe)->tag == tag) &&
		(name == form_element_enumerate_ALL || (name == NULL && el_name == NULL) || strcasecomp(el_name, name) == 0))
	    {
		fn(fe, handle);
	    }
	}
    }
}

/*****************************************************************************/

#if 0
extern rid_pos_item *rid_pos_alloc(void)
{
    return mm_malloc(sizeof(rid_pos_item));
}

extern void rid_pos_free(rid_pos_item *pi)
{
    rid_free_pos(pi);
}

extern void rid_pos_free_chain(rid_pos_item *pi)
{
}

extern void rid_pos_cache_flush(void)
{
}
#endif

/*****************************************************************************/

/* eof rid.c */
