/* -*-C-*-
 *
 * format.h
 */

#ifndef __format_h
#define __format_h

/*****************************************************************************/

#ifdef FORMAT_PRIVATE_BITS

#define MAYBE		0
#define MUST		1
#define DONT		2

extern void stomp_captions_until_working(rid_header *header);

#endif /* FORMAT_PRIVATE_BITS */

/*****************************************************************************/

/*****************************************************************************/

/*
  This structure contains the intermediate state used during
  formatting.

  start of pos list
  ZOM completed lines
  ZOm text lines - 'text_line'
  ZOM partially done float lines - no direct handle
  ZOm float lines - 'float_line'
  end of pos list

  */

typedef struct RID_FMT_STATE
{
    rid_header		*rh;
    antweb_doc 		*doc;
    rid_text_stream	*stream;
    rid_text_item *	next_item;
    int			fmt_method;
    int			previous_pad;
    int			format_width;
    int			y_text_pos; /* Distance so far (-ve) */
    int			linenum;
    int			depth;
    /* Per text line */
    rid_pos_item *	text_line;
    int			x_text_pos; /* From current left margin */
    /* Per unbreakable sequence */
    int			unbreakable_width;
    rid_text_item *	unbreakable_start;
    rid_text_item *	unbreakable_stop;
    rid_text_item *	last_last_unbreakable; /* Cleared each line */
    /* Per floating line(s) region */
    rid_pos_item *	float_line;
    
} RID_FMT_STATE;

/*****************************************************************************/

extern void rid_toplevel_format(antweb_doc *doc,
				rid_header *rh, 
				rid_text_item *start_from, 
				int fwidth,
				int fheight);


extern void format_attach_header(RID_FMT_STATE *fmt, rid_header *rh);

extern void format_attach_stream(RID_FMT_STATE *fmt, rid_text_stream *stream);

extern void format_stream(antweb_doc *doc,
			  rid_header *rh,
			  rid_text_stream *stream,
			  int fmt_method,
			  int depth);

extern void format_precondition(rid_header *header);
extern void format_postcondition(rid_header *header);


#endif /* __format_h */

/* eof */
