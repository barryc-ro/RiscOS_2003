/* -*-c-*- */
/* CHANGELOG
 * 19/07/06: SJM: changed some variable names to stop compiler wingeing
 * 24/07/96: SJM: Restored my earlier changes as they handle splitting of items bigger than a page
 * 19/09/96: SJM: Added check for error in print_getrectangle. Made most of the functions statics.
 *                Added call to abortjob when error occurs, stops Escape from bombing your machine!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "memwatch.h"

#include "wimp.h"
#include "msgs.h"
#include "font.h"
#include "swis.h"
#include "colourtran.h"
#include "bbc.h"
#include "print.h"
#include "visdelay.h"

#include "rid.h"
#include "antweb.h"
#include "webfonts.h"
#include "url.h"
#include "util.h"
#include "makeerror.h"
#include "images.h"

#include "status.h"

#include "filetypes.h"
#include "consts.h"
#include "object.h"
#include "auth.h"
#include "config.h"
#include "rcolours.h"

#include "interface.h"

#include "printing.h"
#include "debug.h"

#define PRNDBG(a)	DBG(a); waitabit()

#define MP2OS	(72000/180)

#ifndef PDriver_Command
#define PDriver_Command	0x8015E
#endif

typedef struct _awp_job_str *awp_job;

/* Check pagination */
/* Given the scales page size (size/scale factor) return the number of pages in the document */
/* os_error *awp_check_pages(be_doc doc, int scale, int *pages); */

/* os_error *awp_start_job(be_doc doc, int scale, int flags, awp_job *job); */
/* os_error *awp_end_job(be_doc doc, awp_job job); */

/* os_error *awp_render_page(be_doc doc, awp_job job, int page, int copies);	 */

/* os_error *awp_paginate(antweb_doc *doc, int plen, int flags); */


struct _awp_job_str {
    print_pagesizestr psize;	/* Printer page size details */
    int job, oldjob;		/* File handles */
    awp_page_str *page;		/* The last page used */
    int page_no;		/* The number of the last page */
    int scale, flags;
    int pwidth, pheight;	/* The width and height of the paper in OS units */
    int old_doc_flags, old_bgt_flags;
    wimp_paletteword old_col_link, old_col_plain, old_col_back, old_col_cref;
#if USE_MARGINS
    wimp_box old_margin;
#endif
    int old_blending, old_display_scale, old_scale_value;
};

#if DEBUG
static void waitabit(void)
{
    clock_t t = clock();
    t += 5;
    while (clock() < t)
	;
}
#else
#define waitabit()
#endif

static os_error *awp_open_printer(int *jobp)
{
    os_regset reg;
    os_error *ep;

    /* Open the printer: file */
    reg.r[0] = 0x80;  /* Output */
    reg.r[1] = (int) "Printer:";

    ep = os_find(&reg);

    if (ep == NULL)
	*jobp = reg.r[0];

    return ep;
}

static void awp_close_printer(int job)
{
    os_regset reg;

    reg.r[0] = 0;
    reg.r[1] = job;

    os_find(&reg);
}

static os_error *awp_old_render_page(antweb_doc *doc, awp_job job, int copies)
{
    wimp_box         box;
    int              more;
    wimp_redrawstr   r;
    awp_page_str *page;
    os_error *ep;

    /* Start to set up redraw struct for diagram */

    PRNDBG(("Calling drawpage, doc = 0x%p\n", doc));
    ep = print_drawpage(copies, 0, 0, (print_box *) &box, &more, (int*) &page);
    if (ep)
	return ep;

    PRNDBG(("Page is 0x%p, bounding box is: %d,%d,%d,%d\n", page, box.x0, box.y0, box.x1, box.y1));

    visdelay_percent((page->line->top * 100) / doc->rh->stream.height);

    /* Loop */
    while (more && ep == NULL)
    {
	PRNDBG(("More data, page is 0x%p, bounding box is: %d,%d,%d,%d\n", page, box.x0, box.y0, box.x1, box.y1));
	r.g = box;
	r.box.x0 = r.box.y0 = 0;
	r.scx = 0;
	r.box.x1 = job->pwidth;
#if SI_PRINT
	r.box.y1 = job->pheight;
	r.scy = page->line->top + page->offset;
#else
	r.box.y1 = page->pheight;
	r.scy = page->line->top;
#endif

	backend_render_rectangle(&r, doc, 0);

	PRNDBG(("Getting next rectangle\n"));

	/* Get next rectangle */
	ep = print_getrectangle((print_box *) &box, &more, (int*) &page);
    }

    PRNDBG(("Done render\n"));

    return ep;
}

void awp_free_pages(be_doc doc)
{
    awp_page_str *page, *p2;

    page = doc->paginate;
    while (page)
    {
	p2 = page->next;
	mm_free(page);
	page = p2;
    }

    doc->paginate = doc->last_page = NULL;
}

static os_error *awp_paginate(antweb_doc *doc, int plen, int flags)
{
    awp_page_str *new_page;
    rid_pos_item *pi, *pi2;
    int page_top;
    int offset;     /* offset within page */

    PRNDBG(("Page length %d\n", plen));

    if (doc->paginate)
	awp_free_pages(doc);

    pi = doc->rh->stream.pos_list;

    if (pi == NULL)
	return NULL;

    new_page = mm_calloc(1, sizeof(*new_page));
    new_page->doc = doc;
    new_page->line = pi;
    doc->paginate = doc->last_page = new_page;

    offset = 0;
    while(pi)
    {
#if SI_PRINT
	page_top = pi->top + offset;
#else
	page_top = pi->top;
#endif
	pi2 = pi->next;

	while(pi2 && pi2->top > (page_top - plen))
	{
	    pi = pi2;
	    pi2 = pi2->next;
	}

	/* Three cases.  Either there are no more line (pi2 == NULL),
	   or the first line is longer than the page (new_page->line == pi) or
	   the line pi would not fit.
	   Offset is set to the start point for the next page.
	 */

#if SI_PRINT
	if (pi2 == NULL)            /* no more, so move on */
	{
            offset = 0;

	    new_page->last = pi;
	    pi = NULL;
	}
	else if (new_page->line == pi)   /* this line is too big for the page */
	{
            offset -= plen;

	    new_page->last = pi;
	}
        else                        /* line wouldn't fit */
        {
            offset = (page_top - plen) - pi->top;

            if (pi->top - pi2->top > plen)  /* won't fit on page */
            {
                new_page->last = pi;
            }
            else
            {
                offset = 0;
                new_page->last = pi->prev;
            }
	}
#else
	if (pi2 == NULL || new_page->line == pi)
	{
	    new_page->last = pi;
	    /* If we stopped because there was no more, or we stopped before we moved, move on */
	    pi = pi2;
	}
	else
	{
	    new_page->last = pi->prev;
	}
	new_page->pheight = (new_page->line->top - (new_page->last->top -
					  (new_page->last->max_up + new_page->last->max_down) ));
#endif

	new_page = mm_calloc(1, sizeof(*new_page));
	new_page->doc = doc;
	doc->last_page->next = new_page;
	new_page->prev = doc->last_page;
	doc->last_page = new_page;

	new_page->line = pi;
#if SI_PRINT
	new_page->offset = offset;
#endif
    }

    return NULL;
}

static os_error *awp_check_pages(be_doc doc, int scale, int *pages)
{
    awp_page_str *page;
    int i;

    /* Every valid page has a first line pointer, the last page structure just marks the end */
    for (i=0,page = doc->paginate; page && page->line; i++, page = page->next)
	;

    *pages = i;

    return NULL;
}

static os_error *awp_start_job(be_doc doc, int scale, int flags, awp_job *job)
{
    awp_job new_job;
    os_error *ep;
    int had_reformat = 0, had_rescale = 0;

    new_job = mm_calloc(1, sizeof(*new_job));
    if (new_job == NULL)
	return makeerror(ERR_NO_MEMORY);

    visdelay_begin();

    new_job->scale = (scale << 16) / 100;
    new_job->flags = flags;

    ep = print_pagesize(&(new_job->psize));
    if (ep)
    {
	PRNDBG(("Getting page size gave error '%s'\n", ep->errmess));
	goto err;
    }

    /* must change fonts/margins here before we reformat */

#if USE_MARGINS
    /* disable screen margins as printing uses its own */
    new_job->old_margin = doc->margin;
    memset(&doc->margin, 0, sizeof(doc->margin));
#endif

    /* reset scale to 100% */
    new_job->old_display_scale = config_display_scale;

    new_job->old_scale_value = doc->scale_value;
    doc->scale_value = 100;

    if (config_display_scale != 100)
    {
	config_display_scale = 100;
	webfonts_reinitialise();

	antweb_trigger_fetching(doc);
    }

    had_rescale = 1;

    new_job->pwidth = (((new_job->psize.bbox.x1 - new_job->psize.bbox.x0 - (2000 * config_print_border)) *
		    100) /
		   (scale * MP2OS));
    new_job->pheight = (((new_job->psize.bbox.y1 - new_job->psize.bbox.y0 - (2000 * config_print_border)) *
		     100) /
		    (scale * MP2OS));

    if (flags & awp_print_SIDEWAYS)
    {
	int t;

	t = new_job->pheight;
	new_job->pheight = new_job->pwidth;
	new_job->pwidth = t;
    }

    doc->rh->stream.fwidth = new_job->pwidth;

    /* Reformat the the new_job paper width here */
    ep = antweb_document_format(doc, new_job->pwidth );
    if (ep)
    {
	PRNDBG(("Reformat to page width gave error '%s'\n", ep->errmess));
	goto err;
    }
    had_reformat = 1;

    ep = awp_paginate(doc, new_job->pheight, 0);
    if (ep)
    {
	PRNDBG(("paginate gave error '%s'\n", ep->errmess));
	goto err;
    }

    image_palette_change();

    /* Open printer file */
    ep = awp_open_printer(&new_job->job);
    if (ep)
    {
	PRNDBG(("open printer gave error '%s'\n", ep->errmess));
	goto err;
    }

    /* Select job */
    ep = print_selectjob(new_job->job, doc->url, &new_job->oldjob);
    if (ep)
    {
	PRNDBG(("Select job gave error '%s'\n", ep->errmess));
	goto err;
    }

    PRNDBG(("Declaring fonts\n"));

    webfont_declare_printer_fonts();

    new_job->old_doc_flags = doc->flags;
    new_job->old_bgt_flags = doc->rh->bgt;

    new_job->old_col_link = config_colours[render_colour_AREF];
    new_job->old_col_cref = config_colours[render_colour_CREF];
    new_job->old_col_back = config_colours[render_colour_BACK];
    new_job->old_col_plain = config_colours[render_colour_PLAIN];

    if (flags & awp_print_NO_PICS)
	doc->flags |= doc_flag_NO_PICS;
    if (flags & awp_print_UNDERLINE)
	doc->flags |= doc_flag_UL_LINKS;
    else
	doc->flags &= ~doc_flag_UL_LINKS;
    if (flags & awp_print_NO_BACK)
	doc->rh->bgt &= ~(rid_bgt_COLOURS | rid_bgt_IMAGE | rid_bgt_FCOL | rid_bgt_LCOL | rid_bgt_VCOL);
    if (flags & awp_print_NO_COLOUR)
    {
	config_colours[render_colour_AREF].word = 0;
	config_colours[render_colour_PLAIN].word = 0;
	config_colours[render_colour_CREF].word = 0;
	config_colours[render_colour_BACK].word = 0xffffff00;

	doc->flags &= ~doc_flag_DOC_COLOURS;
    }
    doc->flags |= doc_flag_NO_FILL | doc_flag_USE_DRAW_MOD;

    /* disable font blending as it stops text from being printed */
    new_job->old_blending = config_display_blending;
    config_display_blending = FALSE;

    *job = new_job;

    return NULL;

 err:
    if (had_rescale)
    {
	doc->scale_value = new_job->old_scale_value;

	if (new_job->old_display_scale != 100)
	{
	    config_display_scale = new_job->old_display_scale;
	    webfonts_reinitialise();

	    antweb_trigger_fetching(doc);
	}

#if USE_MARGINS
	doc->margin = new_job->old_margin;
#endif
    }

    /* What if we reformatted the text? */
    if (had_reformat)
    {
	fe_view_dimensions fvd;
	frontend_view_get_dimensions(doc->parent, &fvd);

	doc->rh->stream.fwidth = fvd.user_width;
#if USE_MARGINS
	doc->rh->stream.fwidth -= doc->margin.x0 - doc->margin.x1;
#endif

	antweb_document_format(doc, fvd.user_width);
    }

    visdelay_end();

    mm_free(new_job);
    return ep;
}

static os_error *awp_end_job(be_doc doc, awp_job job, BOOL abort)
{
    os_error *ep;
    fe_view_dimensions fvd;

    /* End job */
    PRNDBG(("Ending job: abort %d job %d\n", abort, job->job));

    ep = abort ? print_abortjob(job->job) : print_endjob(job->job);
    if (ep)
    {
	PRNDBG(("end/abort job gave error '%s'\n", ep->errmess));
    }

    /* Close printer file */
    PRNDBG(("Closing file job %d\n", job->job));
    awp_close_printer(job->job);

    PRNDBG(("Freeing pages\n"));

    awp_free_pages(doc);

    PRNDBG(("Done printing!\n"));

    doc->flags = job->old_doc_flags;
    doc->rh->bgt = job->old_bgt_flags;
#if USE_MARGINS
    doc->margin = job->old_margin;
#endif

    config_colours[render_colour_AREF] = job->old_col_link;
    config_colours[render_colour_CREF] = job->old_col_cref;
    config_colours[render_colour_BACK] = job->old_col_back;
    config_colours[render_colour_PLAIN] = job->old_col_plain;

    config_display_blending = job->old_blending;

    doc->scale_value = job->old_scale_value;

    if (job->old_display_scale != 100)
    {
	config_display_scale = job->old_display_scale;
	webfonts_reinitialise();

	antweb_trigger_fetching(doc);
    }

    image_palette_change();

    mm_free(job);

    /* (Re)format to the display width */
    frontend_view_get_dimensions(doc->parent, &fvd);

    doc->rh->stream.fwidth = fvd.user_width;
#if USE_MARGINS
    doc->rh->stream.fwidth -= doc->margin.x0 - doc->margin.x1;
#endif

    antweb_document_format(doc, fvd.user_width);

    visdelay_end();

    return ep;
}

static os_error *awp_render_page(be_doc doc, awp_job job, int page, int copies)
{
    print_transmatstr  trans;
    print_positionstr  ppos;
    print_box pbox;
    os_error *ep;
    int bgcol;
#if !SI_PRINT
    int yfix;
    int scale_pc = (job->scale * 100) >> 16;
#endif

    if (page != job->page_no)
    {
	PRNDBG(("Not a known page\n"));

	if (job->page && page == (job->page_no + 1))
	{
	    PRNDBG(("Moving to the next page\n"));

	    job->page_no++;
	    job->page = job->page->next;

	    if (job->page == NULL || job->page->next == NULL)
		return makeerror(ERR_BAD_PAGE);
	}
	else
	{
	    awp_page_str *pageptr;		/* The last page used */
	    int page_no;		/* The number of the last page */

	    PRNDBG(("Looking for the correct page\n"));

	    for(page_no = 1, pageptr = doc->paginate;
		page_no < page && pageptr && pageptr->line;
		page_no++, pageptr = pageptr->next)
	    {
		PRNDBG(("Looking for page %d, current page is %d at 0x%p\n",
			page, page_no, pageptr));
	    }

	    if (page_no != page)
	    {
		return makeerror(ERR_BAD_PAGE);
	    }

	    PRNDBG(("Page number %d is at 0x%p\n", page_no, pageptr));

	    job->page = pageptr;
	    job->page_no = page_no;
	}
    }

    pbox.x0 = 0;
    pbox.x1 = job->pwidth;
#if SI_PRINT
    if (job->page->last->next)
    {
        pbox.y0 = job->pheight - (job->page->line->top - job->page->last->next->top);
        if (pbox.y0 < 0)
            pbox.y0 = 0;
    }
    else
        pbox.y0 = 0;
    pbox.y1 = job->pheight;
#else
    pbox.y0 = 0;
    pbox.y1 = job->page->pheight;

    /* In a bad worst case we have yfix can be the hight of the paper
     * in milipoints and the intermediate is 100 times this.  For A0
     * protrait this is 337,000,000 1/100ths of milipoints.  This does
     * not overflow a signed int. */

    yfix = (job->pheight - pbox.y1) * (scale_pc * MP2OS) / 100;
#endif

    if (job->flags & awp_print_SIDEWAYS)
    {
	trans.xx = 0;
	trans.xy = job->scale;
	trans.yx = -job->scale;
	trans.yy = 0;
#if SI_PRINT
	ppos.dx = job->psize.bbox.x1- (1000 * config_print_border);
	ppos.dy = job->psize.bbox.y0 + pbox.y0*((job->scale * MP2OS) >> 16) + (1000 * config_print_border);
#else
	ppos.dx = job->psize.bbox.x1 - yfix - (1000 * config_print_border);
	ppos.dy = job->psize.bbox.y0 + (1000 * config_print_border);
#endif
    }
    else
    {
	trans.xx = job->scale;
	trans.xy = 0;
	trans.yx = 0;
	trans.yy = job->scale;
#if SI_PRINT
	ppos.dx = job->psize.bbox.x0 + (1000 * config_print_border);
	ppos.dy = job->psize.bbox.y0 + pbox.y0*((job->scale * MP2OS) >> 16) + (1000 * config_print_border);
#else
	ppos.dx = job->psize.bbox.x0 + (1000 * config_print_border);
	ppos.dy = job->psize.bbox.y0 + yfix + (1000 * config_print_border);
#endif
    }

    PRNDBG(("Giving rectangle:\n"));
    PRNDBG(("Box = %d,%d,%d,%d\n", pbox.x0, pbox.y0, pbox.x1, pbox.y1 ));
    PRNDBG(("Page position is %d,%d (%d,%d)\n", ppos.dx, ppos.dy, ppos.dx / MP2OS, ppos.dy / MP2OS));
    PRNDBG(("Page given is 0x%p\n", job->page));

    bgcol = (job->flags & awp_print_NO_BACK) ?
	0xffffff00 : ((doc->rh->bgt & rid_bgt_COLOURS) ?
		      doc->rh->colours.back : (int) config_colours[render_colour_BACK].word);

    ep = print_giverectangle((int) (long) job->page, &pbox, &trans, &ppos, bgcol);

    if (ep == NULL)
    {
	ep = awp_old_render_page(doc, job, copies);
#if DEBUG
	if (ep)
	    PRNDBG(("Render page gave error '%s'\n", ep->errmess));
#endif
    }

    return ep;
}

os_error *awp_print_document(be_doc doc, int scale, int flags, int copies)
{
    os_error *ep;
    int total_pages;
    awp_job job;
    int pfrom, pto;
    int inc, copy;
    int pageno;

    /* ensure it doesn't crash with divide by zero error! */
    if (scale < 1)
	scale = 1;

    ep = awp_start_job(doc, scale, flags, &job);
    if (ep == NULL)
    {
	ep = awp_check_pages(doc, scale, &total_pages);

	pfrom = 1;
	pto = total_pages;

	/* @@@@ In here we need a whole stack of checks */

	PRNDBG(("About to print doc 0x%p, %d copies, from page %d to page %d, flags = 0x%02x\n",
		doc, copies, pfrom, pto, flags));

	if (flags & awp_print_REVERSED)
	{
	    int t = pfrom;

	    pfrom = pto;
	    pto = t - 1;
	    inc = -1;
	}
	else
	{
	    pto = pto + 1;
	    inc = 1;
	}

	PRNDBG(("From %d to %d step %d\n", pfrom, pto, inc));

	if (ep == NULL)
	{
	    for (copy = 0; ep == NULL && copy < ((flags & awp_print_COLLATED) ? copies : 1); copy++)
	    {
		for (pageno = pfrom; ep == NULL && pageno != pto; pageno += inc)
		{
		    PRNDBG(("About to print page %d\n", pageno));

		    ep = awp_render_page(doc, job, pageno, (flags & awp_print_COLLATED) ? 1 : copies);
		}
	    }
	}
	if (ep)
	    awp_end_job(doc, job, TRUE);
	else
	    ep = awp_end_job(doc, job, FALSE);
    }

    return ep;
}

#ifdef STBWEB
/* New SWI that sends a command direct to the printer */
os_error *awp_command(int cmd_no)
{
    int job;
    os_error *e;

    e = awp_open_printer(&job);
    if (!e)
    {
	e = (os_error *)_swix(PDriver_Command, _INR(0,1), cmd_no, job);

	awp_close_printer(job);
    }
    return e;
}
#endif

/* eof commonsrc/printing.c */
