/* -*-c-*- */

/* oscaff.c */

/* Methods for scaff objects */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wimp.h"
#include "bbc.h"

#include "antweb.h"
#include "interface.h"
#include "rid.h"
#include "consts.h"
#include "config.h"
#include "render.h"
#include "rcolours.h"
#include "dfsupport.h"

#include "object.h"
#include "version.h"
#include "gbf.h"



void oscaff_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    ti->width = 0;
    ti->pad = 0;
    ti->max_up = 0;
    ti->max_down = 0;
}


void oscaff_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
}

void oscaff_astext(rid_text_item *ti, rid_header *rh, FILE *f)
{
}



void oscaff_asdraw(rid_text_item *ti, antweb_doc *doc, int fh,
		  int x, int y, int *fileoff, wimp_box *bb)
{
}


/* eof oscaff.c */
