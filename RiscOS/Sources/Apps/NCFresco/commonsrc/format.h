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
  formatting. By preserving this state, the formatter may be stopped
  and started correctly.

  W	normal word, typically text
  F	floating
  I	input cursor
  O	output cursor
  P	rid_pos_item
  T	rid_text_item

  */

typedef struct RID_FMT_STATE
{
    rid_header		*rh;
    antweb_doc 		*doc;
    rid_text_stream	*stream;

    unsigned int	brkstate:2,
			fmt_method:2;


    /*rid_pos_item	*WIP;*/
    rid_pos_item	*WOP;
    rid_pos_item	*FIP;
    rid_pos_item	*FOP;

    rid_text_item	*WIT;
    /*rid_text_item	*WOT;*/
    rid_text_item	*FIT;
    rid_text_item	*FOT;

    rid_text_item	*SOL;

    int			previous_pad;

    int			left_indent;
    int			right_indent;
    int			center_width;
    int			format_width;

    rid_text_item *	next_item;

    int			unbreakable_width;
    rid_text_item *	unbreakable_start;
    rid_text_item *	unbreakable_stop;
    rid_text_item *	last_last_unbreakable; /* Cleared each line */

    rid_pos_item *	text_line;
    rid_pos_item *	float_line;
    rid_pos_item *	text_pickup;
    /*rid_pos_item *	last_float_line;*/

    int			x_text_pos; /* From current left margin */
    int			y_text_pos; /* Distance so far (-ve) */

    intxy		text_pos;
    intxy		max_pos;


    
} RID_FMT_STATE;

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
			  int fmt_method);


#endif /* __format_h */

/* eof */
