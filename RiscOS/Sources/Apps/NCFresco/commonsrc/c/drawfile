/* -*-c-*- */

/* drawfile.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "memwatch.h"

#include "drawftypes.h"
#include "visdelay.h"
#include "wimp.h"
#include "msgs.h"
#include "font.h"
#include "swis.h"

#include "rid.h"
#include "antweb.h"
#include "webfonts.h"
#include "util.h"
#include "makeerror.h"
#include "images.h"

#include "filetypes.h"
#include "consts.h"
#include "object.h"
#include "config.h"

#include "interface.h"

#include "stream.h"

#include "render.h"
#include "rcolours.h"

#include "dfsupport.h"
#include "drawfile.h"

#include "verstring.h"

BOOL drawfile_doc_saver_draw(char *fname, be_doc doc)
{
    os_error *ep;
    int fh;
    os_regset r;
    rid_header *rh = doc->rh;
    int writepoint;
    int dheight, dwidth;
    int gbb_base;
    wimp_paletteword trans;
    wimp_box tb;

    trans.word = ~0;

    /* Open the file */

    r.r[0] = 0x8f;
    r.r[1] = (int) (long) fname;

    ep = os_find(&r);

    if (ep == NULL)
	fh = r.r[0];
    else
	fh = 0;

    if (ep)
	goto err;

    writepoint = 0;

    visdelay_begin();

    /* Write Draw header */
    dheight = - rh->stream.height;
    dwidth = (rh->stream.fwidth > rh->stream.widest) ? rh->stream.fwidth : rh->stream.widest;

    df_write_fileheader(fh, dwidth, dheight, &writepoint, program_name);

    /* Declare the fonts */

    webfont_drawfile_fontlist(fh, &writepoint);

    /* Write a global bounding box */
    gbb_base = df_write_box(fh, "Document", &writepoint);

    /* Draw a 'background' */
    tb.x0 = 0;
    tb.y0 = 0;
    tb.x1 = dwidth;
    tb.y1 = dheight;

    df_write_filled_rect(fh, &tb, trans, render_get_colour(render_colour_BACK, doc), &writepoint);

    stream_write_as_drawfile(doc, &(doc->rh->stream), fh, &writepoint, 0, dheight);

    /* Fix the size of the document in the global bounding box */
    tb.x0 = 0;
    tb.y0 = 0;
    tb.x1 = dwidth;
    tb.y1 = dheight;

    df_write_box_fix(fh, gbb_base, &tb, &writepoint);

    visdelay_end();

    /* Close the file */
    r.r[0] = 0;
    r.r[1] = fh;
	
    os_find(&r);

    set_file_type(fname, FILETYPE_DRAWFILE);

    /* Retrun OK */
    return TRUE;

 err:
    usrtrc( "Error saving as draw: %s\n", ep->errmess);
    if (fh)
    {
	r.r[0] = 0;
	r.r[1] = fh;
	
	os_find(&r);
    }
    return FALSE;
}

