/* -*-c-*- */
/* 23-02-96 DAF         Changes for tables (23-1-96 DRAFT). */
/*                      &rh->stream changed to rh->curstream. */
/* 04-03-96 DAF         Checked using right function names for */
/*                      tables. Call routines on end of some */
/*                      groups. */
/* 26-03-96 DAF         Added new_aref_item() calls for table */
/*                      elements for ID=BLAH attribute. */
/*                      Word splitting in text_item_push_word() */
/*                      when ALIGN=CHAR active and present. */

/* hparse.c - derived from HTMLRISCOS.c */

/* void* passed out is an HTMLCTX */
/* This references an SGMLCTX and a rid_header */

/* Generate my RISC OS internal format from an HTML stream */

#include <string.h>
#include <ctype.h>
#include <assert.h>

#ifndef __wimp_h
#include "wimp.h"
#endif

#include "memwatch.h"

#include "rid.h"
#include "charsets.h"
#include "util.h"
#include "webfonts.h"
#include "url.h"

#include "filetypes.h"
#include "parsers.h"
#include "images.h"

#include "tables.h"
#include "myassert.h"

#include "indent.h"

#include "htmlparser.h"

#if UNICODE
#include "Unicode/charsets.h"
#include "autojp.h"
#include "config.h"
#endif

#ifndef PARSE_DEBUG
#define PARSE_DEBUG 0
#endif

#define DEFAULT_CHARSET csWindows1250

static BOOL include_frames = FALSE;

static void HTRISCOS_put_string (HTMLCTX * me, const char* s);
static rid_header *close_actions(HTMLCTX *htmlctx);
static rid_header *pparse_gopher_close(void *h, char *cfile);

#if SGML_REPORTING

static SGMLBACK def_callback =
{
    my_sgml_note_good_open,
    my_sgml_note_good_close,
    my_sgml_note_implied_open,
    my_sgml_note_implied_close,
    my_sgml_note_missing_open,
    my_sgml_note_missing_close,
    my_sgml_note_close_without_open,
    my_sgml_note_had_unknown_attribute,
    my_sgml_note_had_bad_attribute,
    my_sgml_note_not_repeatable,
    my_sgml_note_antiquated_feature,
    my_sgml_note_badly_formed_comment,
    my_sgml_note_stack_overflow,
    my_sgml_note_stack_underflow,
    my_sgml_note_unexpected_character,
    my_sgml_note_has_tables,
    my_sgml_note_has_forms,
    my_sgml_note_has_scripts,
    my_sgml_note_has_frames,
    my_sgml_note_has_address,
    my_sgml_note_unknown_element,
    my_sgml_note_unknown_attribute,
    my_sgml_note_bad_attribute,
    my_sgml_note_message,
    my_sgml_reset_report,
    my_sgml_pre_open
};

#else

static SGMLBACK def_callback =
{
    NULL, /*my_sgml_note_good_open,*/
    NULL, /*my_sgml_note_good_close,*/
    NULL, /*my_sgml_note_implied_open,*/
    NULL, /*my_sgml_note_implied_close,*/
    NULL, /*my_sgml_note_missing_open,*/
    NULL, /*my_sgml_note_missing_close,*/
    NULL, /*my_sgml_note_close_without_open,*/
    NULL, /*my_sgml_note_had_unknown_attribute,*/
    NULL, /*my_sgml_note_had_bad_attribute,*/
    NULL, /*my_sgml_note_not_repeatable,*/
    NULL, /*my_sgml_note_antiquated_feature,*/
    NULL, /*my_sgml_note_badly_formed_comment,*/
    NULL, /*my_sgml_note_stack_overflow,*/
    NULL, /*my_sgml_note_stack_underflow,*/
    NULL, /*my_sgml_note_unexpected_character,*/
    NULL, /*my_sgml_note_has_tables,*/
    NULL, /*my_sgml_note_has_forms,*/
    NULL, /*my_sgml_note_has_scripts,*/
    NULL, /*my_sgml_note_has_frames,*/
    NULL, /*my_sgml_note_has_address,*/
    NULL, /*my_sgml_note_unknown_element,*/
    NULL, /*my_sgml_note_unknown_attribute,*/
    NULL, /*my_sgml_note_bad_attribute,*/
    NULL, /*my_sgml_note_message,*/
    NULL, /*my_sgml_reset_report,*/
    *my_sgml_pre_open
};

#endif

#if UNICODE
static void html_feed_characters(HTMLCTX *htmlctx, const char *buffer, int bytes)
{
    SGMLCTX *context = htmlctx->sgmlctx;
    rid_header *rh;
    
    if (context->enc_num == csAutodetectJP)
    {
	int left = bytes;
	int state = autojp_consume_string(&context->autodetect.enc_num, &context->autodetect.state, buffer, &left);

	switch (state)
	{
	case autojp_ASCII:
	    /* pass through as is */
	    PRSDBG(("sgml_feed_characters: ASCII pass through '%.*s'\n", bytes, buffer));
	    break;

	case autojp_DECIDED:
	    PRSDBG(("sgml_feed_characters: DECIDED %d pass through '%.*s' '%.*s'\n",
		    context->autodetect.enc_num, 
		    context->autodetect.inhand.ix,
		    context->autodetect.inhand.data,
		    bytes, buffer));

	    DBG(("set_encoding AUTODETECT %d (inhand %d) language %d\n",
		 context->autodetect.enc_num, context->autodetect.inhand.ix,
		 lang_name_to_num(encoding_default_language(context->autodetect.enc_num))));

	    rh = htmlctx->rh;
	    
	    /* set the encoding */
	    sgml_set_encoding(context, context->autodetect.enc_num);

 	    /* tell frontend encoding and this base language - used to be after process pending (was there a good reason?) */
	    rh->encoding = context->autodetect.enc_num;
	    mm_free(rh->language);
	    rh->language = strdup(encoding_default_language(context->autodetect.enc_num));
	    rh->language_num = lang_name_to_num(rh->language);

	    /* process the stuff pending */
	    if (context->autodetect.inhand.ix)
	    {
		/* recurse in case we hit a META tag and need to change encoding again */
		sgml_feed_characters(context, 
				     context->autodetect.inhand.data,
				     context->autodetect.inhand.ix);

		/* mark buffer as used */
		context->autodetect.inhand.ix = 0;
	    }
	    break;
	
	case autojp_UNDECIDED:
	    PRSDBG(("sgml_feed_characters: UNDECIDED pass through '%.*s' adding last %d to inhand\n", bytes - left, buffer, left));

	    /* add the undecided stuff to inhand */
	    add_to_buffer(&context->autodetect.inhand, buffer + bytes - left, left);

	    /* process the safe part of the buffer */
	    bytes = bytes - left;
	    break;
	}
    }

    sgml_feed_characters( htmlctx->sgmlctx, buffer, bytes );
}
#else
#define html_feed_characters(a,b,c) sgml_feed_characters((a)->sgmlctx, b, c)
#endif

/*****************************************************************************/

/*	Character handling
**	------------------
**
*/
static void HTRISCOS_put_character (HTMLCTX * me, char c)
{
    PRSDBG(("HTRISCOS_put_character()\n"));

    html_feed_characters( me, &c, 1 );
}

/*	String handling
**	---------------
*/
static void HTRISCOS_put_string (HTMLCTX * me, const char* s)
{
    PRSDBG(("HTRISCOS_put_string()\n"));
    
    html_feed_characters( me, s, strlen(s) );
}


static void HTRISCOS_write (HTMLCTX * me, const char* s, int l)
{
    PRSDBG(("HTRISCOS_write()\n"));

    html_feed_characters( me, s, l );
}

/*****************************************************************************/

/*	Free an HTML object
**	-------------------
**
*/
static void HTRISCOS_free (HTMLCTX * me)
{
    sgml_free_context(me->sgmlctx);

#if 0
    (*me->targetClass._free)(me->target);	/* ripple through */
#endif
    mm_free(me);
}


static void HTRISCOS_abort (HTMLCTX * me /*, HTError e*/ )
{
    HTRISCOS_free(me);
}


/*	Structured Object Class
**	-----------------------
*/
static const HTMLCTXClass HTRISCOSeration = /* As opposed to print etc */
{
	HTRISCOS_free,
	HTRISCOS_abort,
	HTRISCOS_put_character,        	HTRISCOS_put_string,	HTRISCOS_write
#if 0
	,
	HTRISCOS_start_element, 	HTRISCOS_end_element,
	HTRISCOS_put_entity
#endif
};


/*****************************************************************************

  Generate a new HTMLCTX structure. It will require an SGMLCTX
  attaching to it before it is really useful.

  */


static HTMLCTX * core_HTMLToRiscos (int encoding )
{
    HTMLCTX* me = (HTMLCTX*) mm_calloc(1, sizeof(*me));
    rid_header *rh;

    if (me == NULL)
    {
	usrtrc( "Couldn't get memory for HTMLCTX\n");
	return NULL;
    }

    me->magic = HTML_MAGIC;

#if 0
    fprintf(stderr, "HTMLCTX created at %p, magic %x\n", me, me->magic);
#endif

    me->last_mode = HTMLMODE_BOGUS;
    me->isa = &HTRISCOSeration;
    me->rh = rh = (rid_header *) mm_calloc(1, sizeof(rid_header));

    if (rh == NULL)
    {
	usrtrc( "Couldn't get memory for rid_header\n");
	mm_free(me);
	return NULL;
    }

    if (memzone_init(&(rh->texts), MEMZONE_CHUNKS) == FALSE)
    {
	usrtrc( "Couldn't init memzone\n");

	mm_free(rh);
	mm_free(me);
	return NULL;
    }

    rh->curstream = & rh->stream;
    rh->refreshtime = -1;
    rh->margin.top = rh->margin.left = -1;
    rh->stream.parent = rh;

#if UNICODE
    /* decide what encoding to start off in */
    if (encoding == 0 || encoding == csAutodetectJP)	/* no HTTP + user set to auto - use ASCII */
	rh->encoding = csASCII;
    else						/* HTTP or override set - use it */
	rh->encoding = encoding;

    rh->language = strdup(encoding_default_language(rh->encoding));
    rh->language_num = lang_name_to_num(rh->language);
#endif
    
#if 0
    /*me->write_ptr = &(me->buf[0]);*/
    /* me->white_count = 0 */
    /* me->aref = NULL */
    /* me->table = NULL */

    /*me->sp = me->stack + MAX_NESTING - 1;*/
    me->sp->tag_number = -1;	/* INVALID */
    me->sp->style.wf_index = WEBFONT_BASE; /* Mark as base style */
    /* me->sp->style.flags = 0; */
    /* me->sp->style.indent = 0; */

    me->strip_space = STRIP_SPACE;
#endif

    return me;
}

static SGMLCTX * SGML_new(HTMLCTX *me, int encoding)
{
    SGMLCTX *context = sgml_new_context();

    PRSDBG(("SGML_new: htmlctx %p sgmlctx %p encoding %d\n", me, context, encoding));
    
    if (context == NULL)
    {
	/* FIXME: better handling */
	usrtrc( "No memory for new SGML context in SGML_new()\n");
	return NULL;
    }

    /* Now initialise HTML information and callback functions */

    context->elements = elements;
    context->clictx = me;
    context->chopper = sgml_fmt_word_chopper;
    context->deliver = sgml_deliver;
    context->callback = def_callback;

#if UNICODE
    /* initialiase encoding to what's decided in HTML init, this value will come from user or HTTP header */
    PRSDBG(("set_encoding: encoding set to %d language %s (%d)\n", me->rh->encoding, me->rh->language, me->rh->language_num));
    context->encoding = encoding_new(me->rh->encoding, FALSE);

    /* be safe - if can't load encoding then use ASCII - guaranteed to work */
    if (context->encoding == NULL)
    {
	PRSDBG(("set_encoding: default to ASCII\n"));
	me->rh->encoding = csASCII;
	context->encoding = encoding_new(me->rh->encoding, FALSE);
    }
	
    context->enc_num = encoding;

    /* initialiase autodetect routines */
    if (encoding == csAutodetectJP)
    {
	PRSDBG(("set_encoding: jp autodetect enabled\n"));
	context->autodetect.state = autojp_state_INIT;
	context->autodetect.enc_num = csAutodetectJP;
    }

    /* select format to store in internally */
    {
	int enc = 0;
	switch (config_encoding_internal)
	{
	case 0:
	    enc = csAcornLatin1;
	    break;
	case 1:
	case 2:
	    enc = csUTF8;
	    break;
	case 3:
	    enc = csShiftJIS;
	    break;
	case 4:
	    enc = csEUCPkdFmtJapanese;
	    break;
	}

	PRSDBG(("set_encoding: write encoding set to %d\n", enc));
	
	if (enc)
	{
	    context->encoding_write = encoding_new(enc, TRUE);
	    context->enc_num_write = enc;
	}
    }

    if (context->encoding_write == NULL)
    {
	PRSDBG(("set_encoding: write encoding default to ASCII\n"));

	context->encoding_write = encoding_new(csASCII, TRUE);
	context->enc_num_write = csASCII;
    }
#endif
    
#if SGML_REPORTING
    context->report.output = DBGOUT;
    (*context->callback.reset_report) (context);
#endif

    ASSERT(context->tos != NULL);

    SET_EFFECTS(context->tos, STYLE_WF_INDEX, WEBFONT_BASE);
 
    return context;
}


/* Create an SGMLCTX and  an HTMLCTX, bind and initialise them, */
/* and return the HTMLCTX pointer. */

static HTMLCTX *create_new_html(int encoding)
{
    SGMLCTX *sgmlctx;
    HTMLCTX *htmlctx;

    htmlctx = core_HTMLToRiscos(encoding);

    if (htmlctx == NULL)
	return NULL;

    sgmlctx = SGML_new( htmlctx, encoding );

    /* FIXME:  should free HTMLCTX and descendent data */
    if (sgmlctx == NULL)
	return NULL;

    ASSERT(htmlctx->magic == HTML_MAGIC);
    ASSERT(sgmlctx->magic == SGML_MAGIC);

    htmlctx->sgmlctx = sgmlctx;
#if UNICODE
    htmlctx->rh->encoding = sgmlctx->enc_num;
    htmlctx->rh->encoding_write = sgmlctx->enc_num_write;
#endif
    return htmlctx;
}

/*****************************************************************************/

#define FREAD_BUFSIZE	4096

/*
 * SJM: Hopefully this isn't used anymore. Doesn't support encodings.
 */

static rid_header *parse_some_file(char *fname, char *url, int ft)
{
    void *h;
    FILE *fp;
    char *buffer;
    pparse_details *ppd;
    rid_header *rh;

    for(ppd = file_parsers; ppd->ftype != -1 && ppd->ftype != ft; ppd++)
	;

    if (ppd->ftype == -1)
    {
	usrtrc( "\n\n**** Broken lookup in file parser.  Contact ANT Ltd. ****\n\n");
	return 0;
    }

    h = ppd->new(url, ft, 0);
    if (h == NULL)
    {
	usrtrc( "Can't create pparse_details\n" );
	return 0;
    }
    
    fp = mmfopen(fname, "r");
    if (fp == 0)
    {
	usrtrc( "Can't open file\n");
	return 0;
    }

    buffer = mm_malloc(FREAD_BUFSIZE);
    
    while (!feof(fp))
    {
	int n;

	if ((n = fread(buffer, 1, FREAD_BUFSIZE, fp)) != 0)
	{
	    ppd->data(h, buffer, n, 1);
	}
    }

    mm_free(buffer);
    
    mmfclose(fp);

    rh = ppd->close(h, fname);

    return rh;
}

static rid_header *parse_html_file(char *fname, char *url)
{
    return parse_some_file(fname, url, FILETYPE_HTML);
}



/*
  DTD ideas currently hardwired into htmldefs.c, giving global
  entities table from which we always initialise.

  */

static void *pparse_html_new(char *url, int ft, int encoding)
{
    SGMLCTX *sgmlctx;
    HTMLCTX *htmlctx;

    htmlctx = create_new_html(encoding);

    if (htmlctx == NULL)
	return NULL;

    sgmlctx = htmlctx->sgmlctx;

    ASSERT(htmlctx->magic == HTML_MAGIC);
    ASSERT(sgmlctx->magic == SGML_MAGIC);

    return htmlctx;
}

static int pparse_html_data(void *h, char *buffer, int len, int more)
{
    HTMLCTX *htmlctx = (HTMLCTX *)h;

    ASSERT(htmlctx->magic == HTML_MAGIC);

    /* SJM: mark as threaded */
    htmlctx->sgmlctx->threaded = 1;
    
    htmlctx->isa->write(htmlctx, buffer, len);

    /* SJM: mark as unthreaded and check for close */
    htmlctx->sgmlctx->threaded = 0;
    if (htmlctx->sgmlctx->pending_close)
    {
	PRSDBG(("pparse_html_data (%p): pending close\n", htmlctx));
	close_actions(htmlctx);
    }
#if 0
    HTStream *new = h;

    /* FIXME: */
    /*new->isa->put_block(new, buffer, len);*/
#endif

    return 1;
}

static rid_header *pparse_html_rh(void *h)
{
    HTMLCTX *htmlctx = (HTMLCTX *)h;

    ASSERT(htmlctx->magic == HTML_MAGIC);

    return htmlctx->rh;

#if 0
    HTStream *new = h;
    rid_header *result;
    HTMLCTX* me;

    me = SGML_Get_Target(new);

    result = me->rh;

    return result;
#endif
}

/*****************************************************************************


 */

static void html_free_context(HTMLCTX *htmlctx)
{
    mm_free(htmlctx->basetarget);

    mm_free(htmlctx);
}

/*****************************************************************************

  Close down the specified parser stream and return the resulting
  rid_header structure we've been building up. All HTMLCTX/SGMLCTX memory
  will be freed.

  */

static rid_header *close_actions(HTMLCTX *htmlctx)
{
    rid_header *result = htmlctx->rh;
    SGMLCTX *sgmlctx = htmlctx->sgmlctx;

    ASSERT(htmlctx->magic == HTML_MAGIC);
    ASSERT(sgmlctx->magic == SGML_MAGIC);

    PRSDBG(("close_actions(%p)\n", htmlctx));

    /* PROBABLY DON'T WANT THIS IN THE LONG RUN */
#if SGML_REPORTING
    sgmlctx->report.output = DBGOUT;
#endif

    sgml_stream_finished(htmlctx->sgmlctx);

    memzone_tidy(&result->texts); /* order changed by DL as the above may generate more */

#if SGML_REPORTING
    print_report(sgmlctx);
#endif

    sgml_free_context(htmlctx->sgmlctx);
    html_free_context(htmlctx);

    return result;
}

static rid_header *pparse_html_close(void *h, char *cfile)
{
    HTMLCTX* htmlctx = (HTMLCTX *) h;

    PRSDBG(("Exiting with title '%s'\n", htmlctx->rh->title ? htmlctx->rh->title : "<none>"));

    /* SJM: if threaded then mark as pending close */
    if (htmlctx->sgmlctx->threaded)
    {
	PRSDBG(("pparse_html_close(%p) close called whilst threaded\n", htmlctx));
	htmlctx->sgmlctx->pending_close = 1;
	return htmlctx->rh;
    }
    
    return close_actions(htmlctx);
}

/*****************************************************************************/

/*

  This now creates an SGML context, as this is where attribute stacking
  is performed. Delivery direct to the chopper should be sufficient, and
  keeps things moderately efficient.

  */

static void *pparse_text_new(char *url, int ft, int encoding)
{
    SGMLCTX *sgmlctx;
    HTMLCTX *htmlctx;

    htmlctx = create_new_html(encoding);

    if (htmlctx == NULL)
	return NULL;

    sgmlctx = htmlctx->sgmlctx;

    ASSERT(htmlctx->magic == HTML_MAGIC);
    ASSERT(sgmlctx->magic == SGML_MAGIC);

    /* Use <PRE> presentation style */
    pseudo_html(htmlctx, "<body><pre>");
    /* Except lock out any SGML recognition */

    PRSDBG(("Setting up auto text stuff %p\n", state_stuck_text));
    sgmlctx->tos->matching_mode = match_perm_text;
    sgmlctx->chopper = sgml_pre_word_chopper;
    sgml_reset_chopper_state(sgmlctx);
    string_free(&htmlctx->inhand_string);
    sgmlctx->state = state_stuck_text;

    return htmlctx;
}

static int pparse_text_data(void *h, char *buffer, int len, int more)
{
    HTMLCTX *htmlctx = (HTMLCTX *)h;
    SGMLCTX *sgmlctx;
    
    ASSERT(htmlctx->magic == HTML_MAGIC);

    sgmlctx = htmlctx->sgmlctx;

    ASSERT(sgmlctx->magic == SGML_MAGIC);

    /* SJM: mark as threaded */
    sgmlctx->threaded = 1;
    
#if UNICODE
    /* Correct, but horendously slow */
    sgml_feed_characters(sgmlctx, buffer, len);
#else
    {
	USTRING s;

	s.ptr = buffer;
	s.bytes = len;

	(*sgmlctx->chopper) (sgmlctx, s);
    }
#endif
    /* SJM: mark as unthreaded and check for close */
    sgmlctx->threaded = 0;
    if (sgmlctx->pending_close)
    {
	PRSDBG(("pparse_text_data (%p): pending close\n", htmlctx));
	close_actions(htmlctx);
    }

    return 1;
}

static rid_header *pparse_text_rh(void *h)
{
    HTMLCTX *htmlctx = (HTMLCTX *)h;

    ASSERT(htmlctx->magic == HTML_MAGIC);

    return htmlctx->rh;
}

static rid_header *pparse_text_close(void *h, char *cfile)
{
    HTMLCTX* htmlctx = (HTMLCTX *) h;

    /* SJM: if threaded then mark as pending close */
    if (htmlctx->sgmlctx->threaded)
    {
	PRSDBG(("pparse_text_close (%p): marking pending close\n", htmlctx));
	htmlctx->sgmlctx->pending_close = 1;
	return htmlctx->rh;
    }

    return close_actions(htmlctx);
}

static rid_header *parse_text_file(char *fname, char *url)
{
    return parse_some_file(fname, url, FILETYPE_TEXT);
}

/*****************************************************************************/

typedef struct {
    image i;
    HTMLCTX* me;
    int already;
    int have_image;
    char *url;
} imp_str;

static rid_header *pparse_image_close(void *h, char *cfile)
{
    imp_str *impp;
    rid_header *res;

    impp = (imp_str *) h;

    /* SJM: if threaded then mark as pending close */
    if (impp->me->sgmlctx->threaded)
    {
	/* end stream here as cfile is not available later */
	image_stream_end(impp->i, cfile);

	impp->me->sgmlctx->pending_close = 1;
	return impp->me->rh;
    }

    ASSERT(impp->me->magic == HTML_MAGIC);

    res = close_actions(impp->me);

    if (cfile)			/* SJM: cfile is null if called from pparse_image_data */
	image_stream_end(impp->i, cfile);

    mm_free(impp->url);

    mm_free(impp);

    return res;
}

static rid_header *pparse_image_rh(void *h)
{
    imp_str *impp;
    rid_header *res;

    impp = (imp_str *) h;

    ASSERT(impp->me->magic == HTML_MAGIC);

    res = impp->me->rh;

    return res;
}

static int pparse_image_data(void *h, char *buffer, int len, int more)
{
    imp_str *impp;

    impp = (imp_str *) h;

    ASSERT(impp->me->magic == HTML_MAGIC);

    /* SJM: mark as threaded */
    impp->me->sgmlctx->threaded = 1;
    
    if (!impp->have_image)
    {
	impp->have_image = TRUE;

	pseudo_html(impp->me, "<IMG SRC=\"%s\"><BR>", impp->url);
    }

    if (impp->i && impp->already == 0)
    {
	image_stream_data(impp->i, buffer, len, !more);
    }

    /* SJM: mark as unthreaded and check for close */
    impp->me->sgmlctx->threaded = 0;
    if (impp->me->sgmlctx->pending_close)
	pparse_image_close(impp, NULL);

    return TRUE;
}

static void *pparse_image_new(char *url, int ft, int encoding)
{
    imp_str *impp;

    impp = mm_calloc(1, sizeof(*impp));

    if (impp)
    {
	impp->me = create_new_html(0);

	if (impp->me == NULL)
	{
	    mm_free(impp);
	    return NULL;
	}

	impp->url = strdup(url);

	if (image_stream(url, ft, &impp->already, &impp->i) != 0)
	    impp->i = NULL;
    }

    return impp;
    encoding = encoding;
}

/*****************************************************************************/

static rid_header *parse_gif_file(char *fname, char *url)
{
    HTMLCTX* me;
    rid_header *result;

    me = create_new_html(0);

    if (me == NULL)
	return NULL;

    pseudo_html(me, "<IMG SRC=\"%s\"><BR>", url);

    result = me->rh;

    memzone_tidy(&(result->texts));

    mm_free(me);

    return result;
}

/*****************************************************************************/

static rid_header *parse_gopher_file(char *fname, char *url)
{
    return parse_some_file(fname, url, FILETYPE_GOPHER);
}

typedef struct {
    HTMLCTX* me;
    int used;
    char buffer[1024];
} gparse_str;

static void *pparse_gopher_new(char *url, int ft, int encoding)
{
    gparse_str *gp;

    gp = mm_malloc(sizeof(*gp));
    if (gp)
    {
	gp->me = create_new_html(0);

	if (gp->me == NULL)
	{
	    /* FIXME: memory freeing */
	    mm_free(gp);
	    return NULL;
	}

	gp->used = 0;
	gp->buffer[0] = 0;

	/* And you get all the benefits of the syntax expansion as well! */
	HTRISCOS_put_string(gp->me, "<H1>Gopher Menu</H1><BR>");
    }

    return gp;
}

static void pparse_gopher_put_line(HTMLCTX* me, char *buffer)
{
    char type;
    char *entry, *object, *site = NULL, *port = NULL, *goplus = NULL;
    char *newurl;
    int n;
    int ft;
    char ibuf[32];

    type = buffer[0];

    entry = buffer + 1;
    object = strchr(buffer+1, '\t');
    if (object)
    {
	*object = 0;
	object++;

	site = strchr(object, '\t');
	if (site)
	{
	    *site = 0;
	    site++;

	    port = strchr(site, '\t');
	    if (port)
	    {
		*port = 0;
		port++;

		goplus = strchr(port, '\t');
		if (goplus)
		{
		    *goplus = 0;
		    goplus++;
		}
	    }

	}
    }

    if (port && (port[0] == 0  || (port[0] == '0' && port[1] == 0)) )
    {
	port = 0;
    }

    if (object && object[0] == 0)
    {
	object = 0;
    }
    else
    {
	object = url_escape_chars(object, "#;?:");
    }
#if DEBUG
    fprintf(stderr, "Type='%c', entry='%s', object='%s', site='%s', port='%s'\n",
	    type,
	    entry ? entry : "<None>",
	    object ? object : "<None>",
	    site ? site : "<None>",
	    port ? port : "<None>" );
#endif
    newurl = mm_malloc(1024);

    newurl[0] = 0;

    switch (type)
    {
    case '0':		/* Document */
    case '1':		/* Directory */
    case '4':		/* BinHex */
    case '5':		/* DOS Binary */
    case '6':		/* UNIX uuencoded */
    case '9':		/* Random binary file */
    case 'g':		/* GIF file */
    case 'I':		/* Random image file */
    case '7':		/* Query item */
	/* Gopher item */
	strcpy(ibuf, "icontype:");
	ft = -1;
	switch (type)
	{
	default:
	case '0':		/* Document */
	case '4':		/* BinHex */
	case '6':		/* UNIX uuencoded */
	    ft = FILETYPE_TEXT;
	    break;
	case '1':		/* Directory */
	    strcat(ibuf, "directory");
	    break;
	case '7':		/* Query item */
	    strcat(ibuf, "query");
	    break;
	case '5':		/* DOS Binary */
	    ft = FILETYPE_DOS;
	    break;
	case '9':		/* Random binary file */
	case 'I':		/* Random image file */
	    ft = FILETYPE_DATA;
	    break;
	case 'g':		/* GIF file */
	    ft = FILETYPE_GIF;
	    break;
	}

	if (ft != -1)
	    sprintf(ibuf + strlen(ibuf), ",%03x", ft);

	if (site)
	{
	    strcat(newurl, "gopher://");
	    strcat(newurl, site);
	    if (port)
	    {
		strcat(newurl, ":");
		strcat(newurl, port);
	    }
	}
	strcat(newurl, "/");
	n = strlen(newurl);
	newurl[n] = type;
	newurl[n+1] = 0;
	if (object)
	{
	    strcat(newurl, object);
	}

	pseudo_html(me, "<IMG SRC=\"%s\"><A HREF=\"%s\">%s</A><BR>",
		    ibuf,
		    newurl,
		    entry);
	break;
    case '8':
    case 'T':
	/* Telnet/tn3270 item */

	pseudo_html(me, "<IMG SRC=\"icontype:telnet\">");

	if (site)
	{
	    strcat(newurl, type == '8' ? "telnet://" : "tn3270://" );
	    if (object)
	    {
		strcat(newurl, object);
		strcat(newurl, "@");
	    }
	    strcat(newurl, site);
	    if (port)
	    {
		strcat(newurl, ":");
		strcat(newurl, port);
	    }
	    pseudo_html(me, "<A HREF=\"%s\">%s</A><BR>", newurl, entry);
	}
	else
	    pseudo_html(me, "%s<BR>", entry);
	break;
    case 'h':		/* Hypertext */
	/* Gopher item */

	pseudo_html(me, "<IMG SRC=\"icontype:,faf\">");

	if (object && strncmp(object, "GET ", 4) == 0)
	{
	    if (site)
	    {
		strcat(newurl, "http://");
		strcat(newurl, site);
		if (port)
		{
		    strcat(newurl, ":");
		    strcat(newurl, port);
		}
	    }

	    if (object)
	    {
		strcat(newurl, object + 4);
	    }
	    pseudo_html(me, "<A HREF=\"%s\">%s</A><BR>", newurl, entry);
	}
	else
	    pseudo_html(me, "%s<BR>", entry);
	break;
    case '2':		/* CSO */
    case '3':		/* Should not happen */
    default:
	/* Unknown item */

	pseudo_html(me, "<IMG SRC=\"icontype:blank\">%s<BR>", entry);
	break;
    }

    if (object)
	mm_free(object);

    mm_free(newurl);
}

static int pparse_gopher_data(void *h, char *buffer, int len, int more)
{
    gparse_str *gp = h;

    gp->me->sgmlctx->threaded = 1;
   
    while (len)
    {
	char c;
	int overflow;

	c = *buffer;
	buffer++;
	len--;

	overflow = (gp->used > (sizeof(gp->buffer) - 2));

	if (overflow || c == '\r' || c == '\n')
	{
	    if (!overflow && (c != *buffer) && (*buffer == '\r' || *buffer == '\n'))
	    {
		buffer++;
		len--;
	    }

	    if (gp->used > 1 || (gp->used == 1 && gp->buffer[0] != '.'))
	    {
		gp->buffer[gp->used] = 0;
		pparse_gopher_put_line(gp->me, gp->buffer);
		gp->used = 0;
		gp->buffer[0] = 0;
	    }
	}
	else
	{
	    gp->buffer[gp->used++] = c;
	}
    }

    gp->me->sgmlctx->threaded = 0;
    if (gp->me->sgmlctx->pending_close)
	pparse_gopher_close(gp, NULL);

    return 1;
}

static rid_header *pparse_gopher_rh(void *h)
{
    gparse_str *gp = h;

    return gp->me->rh;
}

static rid_header *pparse_gopher_close(void *h, char *cfile)
{
    rid_header *result;
    gparse_str *gp = h;

    /* SJM: mark pending close if threaded */
    if (gp->me->sgmlctx->threaded )
    {
	gp->me->sgmlctx->pending_close = 1;
	return gp->me->rh;
    }
    
    if (gp->used > 1 || (gp->used == 1 && gp->buffer[0] != '.'))
    {
	gp->buffer[gp->used] = 0;
	pparse_gopher_put_line(gp->me, gp->buffer);
	gp->used = 0;
	gp->buffer[0] = 0;
    }

    result = close_actions(gp->me);

    mm_free(gp);

    return result;
}

/*****************************************************************************/

extern void parse_frames(int yes)
{
    include_frames = yes == -1 ? !include_frames : yes;
}

/*****************************************************************************/

pparse_details file_parsers[] = 
{
    { 
	FILETYPE_HTML,	
	parse_html_file,	
	pparse_html_new,	
	pparse_html_data,	
	pparse_html_rh,	
	pparse_html_close
    },
    {
	FILETYPE_TEXT,
	parse_text_file,	
	pparse_text_new,	
	pparse_text_data,	
	pparse_text_rh,
	pparse_text_close
    },
    {
	FILETYPE_GOPHER,	
	parse_gopher_file,	
	pparse_gopher_new,	
	pparse_gopher_data,	
	pparse_gopher_rh,
	pparse_gopher_close
    },
    {
	FILETYPE_ANY_IMAGE,
	parse_gif_file,
	pparse_image_new,	
	pparse_image_data,	
	pparse_image_rh,	
	pparse_image_close
    },
    /* List terminator */
    { 
	-1,			
	NULL,			
	NULL,			
	NULL,			
	NULL,		
	NULL
    }
};

/* eof */
