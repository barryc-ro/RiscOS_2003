/* > layout.c
 */

#include <stdio.h>

#include "bbc.h"
#include "coords.h"

#include "antweb.h"
#include "interface.h"
#include "memwatch.h"
#include "rid.h"
#include "url.h"
#include "util.h"

#include "render.h"
#include "rcolours.h"

#include "layout.h"

#define DEFAULT_MARGIN_X	8 /* frame inner margins in OS units */
#define DEFAULT_MARGIN_Y	8

/*
 * Note PX is now OS units due to the parser change so all values are held accordingly
 */

/* ---------------------------------------------------------------------------------------------------------- */

typedef struct layout_spacing_info layout_spacing_info;

#define layout_spacing_HORIZONTAL	0x01 /* horizontal or vertical space */
#define layout_spacing_RESIZE		0x02 /* can this space be dragged */
#define layout_spacing_BEVEL		0x04 /* should we draw the 3D bevel thing */

struct layout_spacing_info
{
    layout_spacing_info *next;      /* next in list */
    wimp_box box;                   /* bounding box of this spacer */
    wimp_box bbox;                  /* bounding box of surrounding frames */
    wimp_box container_box;         /* bounding box of parent frameset */
    rid_frameset_item *container;   /* the containing frameset */
    int index;                      /* row or column number */
    int flags;
    int divider_index;			/* reference for the frame divider array[] */
    int colour;				/* colour to draw it in (BGR0) */
};

/* ---------------------------------------------------------------------------------------------------------- */

/*
 * This function must take the layout information in the frame list and
 * decide how big the frames must be
 * The eventual layout is a flat array of box positions but we must recurse
 * down the frame sets to build it up.
 */

static int be_get_frame_size(const rid_stdunits *val, double pcent_unit, double mult_unit, double px_unit)
{
    PRSDBG(( "layout: units %d/%g\n", val->type, val->u.f));
    switch (val->type)
    {
        case rid_stdunit_MULT:
            return (int)(val->u.f*mult_unit) &~ 1;

        case rid_stdunit_PX:    /* pixels */
            return (int)(val->u.f*px_unit) &~ 1;

        case rid_stdunit_PCENT:
            return (int)(val->u.f*pcent_unit) &~ 1;
    }
    return 0;
}

/*
 * This method for allocating space deducts the space that will be used for the margin spacing (but not the window borders)
 * before doling out the pixels and then sets the vals[] to the middle of each boundary.
 * ie one frame eneds at val[]-SPACING and the next starts at val[]+SPACING
 */

static int *be_build_frame_sizes(const rid_stdunits *vals, int n, const rid_frame_unit_totals *totals, int start, int size, int spacing)
{
    int *pos;

    pos = mm_calloc(n + 1, sizeof(int));
    pos[0] = start;

    size -= (n-1)*spacing*2;

    if (n == 1)
    {
        pos[1] = start + size;
    }
    else
    {
        double mult_unit = 0;
        double pcent_unit = 0;
        double px_unit = 1;
        int i;

        if (totals->pcent == 0 && totals->mult == 0)
        {
            /* if only pixels then we might have to scale them up */
            /* remember size is in OS coords */
            px_unit = ((double)size)/totals->px;
        }

        if (totals->pcent)
        {
            /* if we have variables then percents are what they say they are */
            if (totals->mult)
                pcent_unit = size / 100.0;
            /* otherwise we have to scale the percent unit to fill the space */
            /* taking into account what has been used by pixels (as a percentage) */
            else
                pcent_unit = size / (totals->pcent + totals->px*100.0/size);
        }

        /* if we have variables then divide up space not used by pixels and percents. */
        if (totals->mult)
            mult_unit = ((double)size - (double)totals->px - totals->pcent*pcent_unit) / totals->mult;

        for (i = 1; i <= n; i++)
        {
            pos[i] = pos[i-1] + be_get_frame_size(&vals[i-1], pcent_unit, mult_unit, px_unit);
            if (i == 1 || i == n)
                pos[i] += spacing;
            else
                pos[i] += spacing*2;
        }
    }

    return pos;
}

/* convert all the variable size fields into fixed pixel amounts */
/* correct backwards for all the spacing added */

#if FRAMES
static void fix_frame_sizes(rid_stdunits *vals, int n, rid_frame_unit_totals *totals, int size, int spacing)
{
    rid_stdunits *val;
    int i;
    int *pos = be_build_frame_sizes(vals, n, totals, 0, size, spacing);

    totals->px = 0;
    totals->mult = 0;
    totals->pcent = 0;

    for (i = 1, val = vals; i <= n; i++, val++)
    {
        val->type = rid_stdunit_PX;
        val->u.f = ((double)pos[i] - pos[i-1]);     /* /2 for OS coords to pixels */
        if (i == 1 || i == n)
           val->u.f -= (double)spacing;
        else
           val->u.f -= (double)spacing*2;

        totals->px += (int)val->u.f;
    }
    mm_free(pos);
}
#endif

static int divider_current_index = 0;

static int be_frame_layout_1(const rid_frame *frameset, const wimp_box *bbox, fe_frame_info *info, char **urls, antweb_doc *doc)
{
    const rid_frameset_item *fs = &frameset->data.frameset;
    rid_frame *frame, *frame_previous_row;
    int nframes = fs->ncols*fs->nrows;
    int frames_added = 0;
    int *xpos, *ypos;
    int i;
    int last_row_divider = 0, last_col_divider = 0;
    int bdflag = fs->bwidth ? 0 : rid_frame_divider_BORDERLESS;

    PRSDBG(( "layout: %p/%p %dx%d sizes @%p,%p\n", frameset, fs, fs->ncols, fs->nrows, fs->widths, fs->heights));

    /* build the arrays of widths and heights */
    xpos = be_build_frame_sizes(fs->widths, fs->ncols, &fs->width_totals, bbox->x0, bbox->x1 - bbox->x0, fs->bwidth);
    ypos = be_build_frame_sizes(fs->heights, fs->nrows, &fs->height_totals, bbox->y0, bbox->y1 - bbox->y0, fs->bwidth);

    frame_previous_row = NULL;

    for (i = 0, frame = fs->frame_list; i < nframes && frame; i++, frame = frame->next)
    {
        wimp_box bb;
        int row, col;

        col = i%fs->ncols;
        row = i/fs->ncols;

        /* build the bounding box for this frame/frameset */
        /* offset from the boundary points if not at the edge of a frameset */
        /* the frameset will already have had spacing added so this is correct */
        bb.x0 = xpos[col] + (col == 0 ? 0 : fs->bwidth);
        bb.x1 = xpos[col+1] - (col == fs->ncols-1 ? 0 : fs->bwidth);
        bb.y0 = ypos[row] + (row == 0 ? 0 : fs->bwidth);
        bb.y1 = ypos[row+1] - (row == fs->nrows-1 ? 0 : fs->bwidth);

	PRSDBG(( "layout: frame %d\n", i));

	/* set up dividers */
	frame->dividers[rid_frame_divider_TOP] = bdflag | (row == 0 ?
	    frameset->dividers[rid_frame_divider_TOP] :			/* copy top from parent */
	    frame_previous_row->dividers[rid_frame_divider_BOTTOM]);	/* use the divider from the row above */

	frame->dividers[rid_frame_divider_BOTTOM] = bdflag | (row == fs->nrows-1 ?
	    frameset->dividers[rid_frame_divider_BOTTOM] :		/* copy bottom from parent */
	    (last_row_divider = divider_current_index++));		/* create new divider and store */


	frame->dividers[rid_frame_divider_LEFT] = bdflag | (col == 0 ?
	    frameset->dividers[rid_frame_divider_LEFT] :		/* copy left from parent */
	    last_col_divider);						/* use the divider created last time around */

	frame->dividers[rid_frame_divider_RIGHT] = bdflag | (col == fs->ncols-1 ?
	    frameset->dividers[rid_frame_divider_RIGHT] :		/* copy right from parent */
	    (last_col_divider = divider_current_index++));		/* create new divider and store */

	/* set up spacing array */
	if (row > 0 && fs->bwidth)
        {
            layout_spacing_info *spacing = mm_calloc(sizeof(struct layout_spacing_info), 1);

            /* bounding box of draggable area */
            spacing->box.y0 = - ypos[row] - fs->bwidth;
            spacing->box.y1 = - ypos[row] + fs->bwidth;
            spacing->box.x0 = bb.x0;
            spacing->box.x1 = bb.x1;

            /* parent box in work area coords */
            spacing->bbox.x0 = bb.x0;
            spacing->bbox.x1 = bb.x1;
            spacing->bbox.y0 = - bb.y1;
            spacing->bbox.y1 = - (ypos[row-1] + ((row-1) == 0 ? 0 : fs->bwidth));

            spacing->container_box = *bbox;

            spacing->container = (rid_frameset_item *)&frameset->data.frameset;
            spacing->index = row;

	    spacing->divider_index = frame->dividers[rid_frame_divider_TOP];

            spacing->flags = layout_spacing_HORIZONTAL | layout_spacing_RESIZE;

	    spacing->colour = frame->bordercolour;

            spacing->next = doc->spacing_list;
            doc->spacing_list = spacing;

	    PRSDBG(( "layout: spacing %d,%d %d,%d i %d\n", spacing->box.x0, spacing->box.y0, spacing->box.x1, spacing->box.y1, spacing->index));
        }

        if (col > 0 && fs->bwidth)
        {
            layout_spacing_info *spacing = mm_calloc(sizeof(struct layout_spacing_info), 1);

            /* bounding box of draggable area */
            spacing->box.y0 = - bb.y1;
            spacing->box.y1 = - bb.y0;
            spacing->box.x0 = xpos[col] - fs->bwidth;
            spacing->box.x1 = xpos[col] + fs->bwidth;

            /* parent box in work area coords */
            spacing->bbox.x0 = xpos[col-1] + ((col-1) == 0 ? 0 : fs->bwidth);
            spacing->bbox.x1 = bb.x1;
            spacing->bbox.y0 = - bb.y1;
            spacing->bbox.y1 = - bb.y0;

            spacing->container_box = *bbox;

            spacing->container = (rid_frameset_item *)&frameset->data.frameset;
            spacing->index = col;

	    spacing->divider_index = frame->dividers[rid_frame_divider_LEFT];

	    spacing->flags = layout_spacing_RESIZE;

	    spacing->colour = frame->bordercolour;

            spacing->next = doc->spacing_list;
            doc->spacing_list = spacing;

	    PRSDBG(( "layout: spacing %d,%d %d,%d i %d\n", spacing->box.x0, spacing->box.y0, spacing->box.x1, spacing->box.y1, spacing->index));
        }


	PRSDBG(( "layout: i %d frame %p tag %d box %d,%d %d,%d\n", i, frame, frame->tag, bb.x0, bb.y0, bb.x1, bb.y1));

        switch (frame->tag)
        {
            case rid_frame_tag_FRAME:
            {
                const rid_frame_item *f = &frame->data.frame;

                /* copy information from the rid to fe structure */
                info->name = f->name;
                info->scrolling = f->scrolling;
                info->noresize = f->noresize;

                info->box.x0 = bb.x0;
                info->box.x1 = bb.x1;
                info->box.y0 = - bb.y1;   /* reverse the signs at this point */
                info->box.y1 = - bb.y0;

                info->margin.x0 = f->marginwidth != -1 ? f->marginwidth : DEFAULT_MARGIN_X;       /* marginwidth and height are converted to OS in parsing */
                info->margin.x1 = -info->margin.x0;
                info->margin.y0 = f->marginheight != -1 ? f->marginheight : DEFAULT_MARGIN_Y;
                info->margin.y1 = -info->margin.y0;

		/* copy in divider values */
		memcpy(info->dividers, frame->dividers, sizeof(info->dividers));

                /* also fill in the url of this frame */
                if (urls)
                    *urls++ = f->src;
                info++;
                frames_added++;
                break;
            }

            case rid_frame_tag_FRAMESET:
            {
                int n = be_frame_layout_1(frame, &bb, info, urls, doc);

                if (urls)
                    urls += n;
                info += n;
                frames_added += n;
                break;
            }
        }


	/* incement on the previous row ptr or kick it off if at the end of the first row */
	if (frame_previous_row)
	    frame_previous_row = frame_previous_row->next;
	else if (i == fs->ncols-1)
	    frame_previous_row = fs->frame_list;
    }

    mm_free(xpos);
    mm_free(ypos);

    return frames_added;
}

/* ---------------------------------------------------------------------------------------------------------- */

/* Set the BEVEL and RESIZE flags correctly now that all the frames have
 * been allocated and positioned.
 */

static void set_spacing_flags_frame(layout_spacing_info *spacing, rid_frame *frameset)
{
    rid_frame *frame;

    /* for all frames in the frameset */
    for (frame = frameset->data.frameset.frame_list; frame; frame = frame->next)
    {
	BOOL match;

	PRSDBG(("set_spacing_flags_frame: frame%p tag %d dividers %d,%d,%d,%d border %d\n",
		frame, frame->tag,
		frame->dividers[rid_frame_divider_LEFT],
		frame->dividers[rid_frame_divider_TOP],
		frame->dividers[rid_frame_divider_RIGHT],
		frame->dividers[rid_frame_divider_BOTTOM],
		frame->border));

	/* see if this frame borders this spacing unit */
	if (spacing->flags & layout_spacing_HORIZONTAL)
	{
	    match = spacing->divider_index == frame->dividers[rid_frame_divider_TOP] ||
		spacing->divider_index == frame->dividers[rid_frame_divider_BOTTOM];
	}
	else
	{
	    match = spacing->divider_index == frame->dividers[rid_frame_divider_LEFT] ||
		spacing->divider_index == frame->dividers[rid_frame_divider_RIGHT];
	}

	/* if matches and this frame wants a 3D border then give it to it */
	if (match && frame->border)
	    spacing->flags |= layout_spacing_BEVEL;

	switch (frame->tag)
        {
	case rid_frame_tag_FRAME:
	    /* if it matches and this frame ois not resizable then the spacer is not moveable */
	    if (match && frame->data.frame.noresize)
		spacing->flags &= ~layout_spacing_RESIZE;
	    break;

	case rid_frame_tag_FRAMESET:
	    /* if a frameset then recurse */
	    set_spacing_flags_frame(spacing, frame);
	    break;
	}
    }
}

static void set_spacing_flags(antweb_doc *doc)
{
    layout_spacing_info *spacing;

    /* for each spacing unit in turn, check all the frames */
    for (spacing = doc->spacing_list; spacing; spacing = spacing->next)
    {
	PRSDBG(("set_spacing_flags: doc%p spacer%p divindex %d flags %x\n", doc, spacing, spacing->divider_index, spacing->flags));

	set_spacing_flags_frame(spacing, doc->rh->frames);
    }
}

/* ---------------------------------------------------------------------------------------------------------- */

void layout_layout(antweb_doc *doc, int totalw, int totalh, int refresh_only, int *dividers, int max)
{
    fe_frame_info *info;
    wimp_box box;
    char **urls;
    int i;

    /* ensure last list is freed */
    layout_free_spacing_list(doc);

    /* allocate temp space for stuff */
    info = mm_calloc(sizeof(*info), doc->rh->nframes);
    urls = refresh_only ? NULL : mm_calloc(sizeof(*urls), doc->rh->nframes);

    box.x0 = 0;
    box.y0 = 0;
    box.x1 = totalw;
    box.y1 = -totalh;   /* we want a positive box */

    PRSDBG(( "layout: max %d framesin %dx%d\n", doc->rh->nframes, totalw, -totalh));

    /* the initial frame count is the maximum there could be not counting nesting */

    /* initialise the outermost divider array */
    if (!refresh_only)
    {
	divider_current_index = max;
	memcpy(doc->rh->frames->dividers, dividers, sizeof(doc->rh->frames->dividers));
    }

    /* kick off the layout with the outer frameset */
    doc->rh->nframes = be_frame_layout_1(doc->rh->frames, &box, info, urls, doc);

    /* allocate the spacing flags */
    set_spacing_flags(doc);

#if DEBUG
{
    fprintf(stderr, "layout: actually %d frames\n", doc->rh->nframes);
    for (i = 0; i < doc->rh->nframes; i++)
    {
        fe_frame_info *ip = &info[i];
        fprintf(stderr, "'%16s' %4d,%4d %4d,%4d div %d,%d,%d,%d '%s'\n",
		ip->name ? ip->name : "<none>",
		ip->box.x0, ip->box.y0, ip->box.x1, ip->box.y1,
		ip->dividers[0], ip->dividers[1], ip->dividers[2], ip->dividers[3],
		urls && urls[i] ? urls[i] : "<none>");
    }
}
#endif

    if (!refresh_only) for (i = 0; i < doc->rh->nframes; i++)
        info[i].src = urls[i] ? url_join(BASE(doc), urls[i]) : NULL;

    frontend_frame_layout(doc->parent, doc->rh->nframes, info, refresh_only, divider_current_index);

    if (!refresh_only) for (i = 0; i < doc->rh->nframes; i++)
        mm_free(info[i].src);

    mm_free(info);
    mm_free(urls);
}

#if FRAMES
int layout_frame_resize_bounds(antweb_doc *doc, int x, int y, wimp_box *box, int *handle)
{
    layout_spacing_info *spc;
    for (spc = doc->spacing_list; spc; spc = spc->next)
    {
        PRSDBGN(("layout: check %d,%d against %d,%d %d,%d\n", x, y, spc->box.x0, spc->box.y0, spc->box.x1, spc->box.y1));
        if ((spc->flags & layout_spacing_RESIZE) &&
	    x >= spc->box.x0 && x <= spc->box.x1 && y >= spc->box.y0 && y <= spc->box.y1)
        {
            /* use this pointer as a handle */
            if (handle)
                *handle = (int)(long) spc;

            /* write out the frameset bounds as the parent */
            if (box)
                *box = spc->bbox;

            return spc->flags & layout_spacing_HORIZONTAL ? be_resize_HEIGHT : be_resize_WIDTH;
        }
    }
    return be_resize_NONE;
}
#endif

void layout_free_spacing_list(antweb_doc *doc)
{
    layout_spacing_info *spc = doc->spacing_list;
    while (spc)
    {
        layout_spacing_info *next = spc->next;
        mm_free(spc);
        spc = next;
    }
    doc->spacing_list = NULL;
}

#define MIN_X_SIZE  16
#define MIN_Y_SIZE  16

#if DEBUG >= 2
static void dump_list(int n, VALUE *list)
{
    int i;
    fprintf(stderr, "layout: update heights to");
    for (i = 0; i < n; i++)
        fprintf(stderr, " %g", list[i].u.f);
    fprintf(stderr, "\n");
}
#else
#define dump_list(n, list)
#endif

#if FRAMES
void layout_frame_resize(antweb_doc *doc, int x, int y, int handle)
{
    layout_spacing_info *spc = (layout_spacing_info *)(long) handle;
    rid_frameset_item *fs = spc->container;

    PRSDBG(( "layout: resize doc %p handle %x to %d,%d\n", doc, handle, x, y));

    if (spc->flags & layout_spacing_HORIZONTAL)
    {
        if (y < spc->bbox.y0 + MIN_Y_SIZE)
            y = spc->bbox.y0 + MIN_Y_SIZE;
        if (y > spc->bbox.y1 - MIN_Y_SIZE)
            y = spc->bbox.y1 - MIN_Y_SIZE;

        fix_frame_sizes(fs->heights, fs->nrows, &fs->height_totals,
            spc->container_box.y1 - spc->container_box.y0, fs->bwidth);

        fs->heights[spc->index - 1].u.f = (spc->bbox.y1 - (double)y - fs->bwidth);
        fs->heights[spc->index].u.f     = ((double)y - spc->bbox.y0 - fs->bwidth);

	dump_list(fs->nrows, fs->heights);
    }
    else
    {
        if (x < spc->bbox.x0 + MIN_X_SIZE)
            x = spc->bbox.x0 + MIN_X_SIZE;
        if (x > spc->bbox.x1 - MIN_X_SIZE)
            x = spc->bbox.x1 - MIN_X_SIZE;

        fix_frame_sizes(fs->widths, fs->ncols, &fs->width_totals,
            spc->container_box.x1 - spc->container_box.x0, fs->bwidth);

        fs->widths[spc->index - 1].u.f  = ((double)x - spc->bbox.x0 - fs->bwidth);
        fs->widths[spc->index].u.f      = (spc->bbox.x1 - (double)x - fs->bwidth);

	dump_list(fs->ncols, fs->widths);
    }

    backend_reset_width(doc, 0);
}
#endif

/* ---------------------------------------------------------------------------------------------------------- */

/* Render the bevels for one document */

void layout_render_bevels(wimp_redrawstr *r, antweb_doc *doc)
{
    layout_spacing_info *spc;
    for (spc = doc->spacing_list; spc; spc = spc->next)
    {
	wimp_box box;

	box = spc->box;
	coords_box_toscreen(&box, (coords_cvtstr *)&r->box);

	if (coords_boxesoverlap(&box, &r->g))
	{
	    if (spc->flags & layout_spacing_BEVEL)
	    {
		render_plinth_full(0, 0, plinth_col_HL_L, plinth_col_D,
				   render_plinth_NOFILL,
				   box.x0, box.y0, box.x1-box.x0, box.y1-box.y0,
				   doc);

/* 		render_plinth_full(plinth_col_HL_M, 0, plinth_col_L, plinth_col_HL_D, */
 		render_plinth_full(spc->colour | render_colour_RGB, /* plinth_col_HL_D, */
				   0, plinth_col_HL_M, plinth_col_M,
				   0,
				   box.x0 + 4, box.y0 + 4, box.x1-box.x0 - 8, box.y1-box.y0 - 8,
				   doc);
	    }
	    else
	    {
		render_set_colour(spc->colour | render_colour_RGB, doc);
		bbc_rectanglefill(box.x0, box.y0, box.x1-box.x0, box.y1-box.y0);
	    }
	}
    }
}

/* ---------------------------------------------------------------------------------------------------------- */

/* Write the layout of the frames as a table */

static int layout__write_table(FILE *f, const rid_frame *frameset, be_layout_write_table_fn fn, const char *prefix, int base_count, int fmt_w, int fmt_h, int width, int height)
{
    const rid_frame *frame;
    const rid_frameset_item *fs = &frameset->data.frameset;
    int i, count = base_count;

    int *xpos = be_build_frame_sizes(fs->widths, fs->ncols, &fs->width_totals, 0, fmt_w, fs->bwidth);
    int *ypos = be_build_frame_sizes(fs->heights, fs->nrows, &fs->height_totals, 0, fmt_h, fs->bwidth);

    PRSDBG(("layout__writetable: frameset %p prefix '%s' fn %p base count %d\n", frameset, strsafe(prefix), fn, base_count));

    fprintf(f, "<TABLE BORDER CELLSPACING=0 CELLPADDING=0>\n");

    /* for all frames in the frameset */
    for (frame = frameset->data.frameset.frame_list, i = 0; frame; frame = frame->next, i++)
    {
        int row, col, w, h, scaled_w, scaled_h;
	char spec[64];

	PRSDBG(("layout__writetable: frame %p\n", frame));

        col = i%fs->ncols;
        row = i/fs->ncols;

	if (col == 0)
	    fprintf(f, "<TR>");

	w = xpos[col+1] - xpos[col];
	h = ypos[row+1] - ypos[row];

	scaled_w = w * width / fmt_w;
	scaled_h = h * height / fmt_h;

	fprintf(f, "<TD WIDTH=%d HEIGHT=%d ALIGN=CENTER VALIGN=MIDDLE>", scaled_w/2, scaled_h/2);

	switch (frame->tag)
        {
	case rid_frame_tag_FRAME:		/* if a frame then call back to frontend */
	    sprintf(spec, "%s_%d", strsafe(prefix), count);
	    if (fn)
		fn(f, spec, scaled_w, scaled_h);
	    count++;
	    break;

	case rid_frame_tag_FRAMESET:		/* if a frameset then recurse */
	    count += layout__write_table(f, frame, fn, prefix, count, w, h, scaled_w, scaled_h);
	    break;
	}
    }

    fprintf(f, "</TABLE>\n");

    mm_free(xpos);
    mm_free(ypos);

    return count;
}

void backend_layout_write_table(FILE *f, be_doc doc, be_layout_write_table_fn fn, const char *prefix, int width, int height)
{
    int w, h;

    w = doc->rh->stream.widest;
    h = doc->rh->stream.height;

#if USE_MARGINS
    w += doc->margin.x0 - doc->margin.x1;
    h -= doc->margin.y0 - doc->margin.y1;
#endif

    layout__write_table(f, doc->rh->frames, fn, prefix, 0, w, -h, width, height);
}

/* ---------------------------------------------------------------------------------------------------------- */

/* eof layout.c */
