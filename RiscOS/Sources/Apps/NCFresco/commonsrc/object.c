/* -*-c-*- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "antweb.h"
#include "interface.h"
#include "rid.h"

#include "object.h"

object_methods object_table[rid_tag_LAST_TAG] = {
{ &opbreak_size,	&obreak_redraw,		0,
      0,		&obreak_astext,
      0,		0,			0,
      obreak_asdraw,    0},	/* PBREAK */
{ &obreak_size,		&obreak_redraw,		0,
      0,		&obreak_astext,
      0,		0,			0,
      obreak_asdraw,    0},	/* HLINE */
{ &otext_size,		&otext_redraw,		0,
      0,		&otext_astext,
      0,		0,			0,
      &otext_asdraw,    otext_update_highlight },	/* TEXT */
{ &obullet_size,	&obullet_redraw,	0,
      0,		&obullet_astext,
      0,		0,			0,
      &obullet_asdraw,  0},	/* BULLET */
{ &oimage_size,		&oimage_redraw,		&oimage_dispose,
      &oimage_click,	&oimage_astext,
      0,		0,			&oimage_image_handle,
      &oimage_asdraw,   oimage_update_highlight },	/* IMAGE */
{ &oinput_size,		&oinput_redraw,		&oinput_dispose,
      &oinput_click,	&oinput_astext,
      &oinput_caret,	&oinput_key,		&oinput_image_handle,
      &oinput_asdraw,   oinput_update_highlight },	/* INPUT */
{ &otextarea_size,	&otextarea_redraw,	&otextarea_dispose,
      &otextarea_click,	&otextarea_astext,
      &otextarea_caret,	&otextarea_key,		0,
      0,                oinput_update_highlight },	/* TEXTAREA */
{ &oselect_size,	&oselect_redraw,	&oselect_dispose,
      &oselect_click,	&oselect_astext,
      0,		0,			0,
      0,                oinput_update_highlight },	/* SELECT */
{ &otable_size,		&otable_redraw,		&otable_dispose,
      0,		&otable_astext,
      0,		0,			0,
      &otable_asdraw,   0			},	/* TABLE */
{ &oobject_size,	&oobject_redraw,	&oobject_dispose,
      oobject_click,	&oobject_astext,
      0,		0,			0,
      &oobject_asdraw,  0			}	/* OBJECT */

};
