/* > stbmap.c

 * Handle the cursor key map mode

 */

#include <math.h>
#include <stddef.h>

#include "akbd.h"
#include "alarm.h"
#include "coords.h"
#include "wimp.h"

#include "interface.h"
#include "debug.h"
#include "util.h"

#include "stbfe.h"
#include "stbutils.h"
#include "fevents.h"

/* ----------------------------------------------------------------------------- */

static fe_view map_view = NULL;
static be_item map_item = NULL;

/* ----------------------------------------------------------------------------- */

static int fe_map_update_position_to(int x, int y)
{
    wimp_box box;
    BOOL limit = FALSE;
    int flags = 0;

    if (fe_item_screen_box(map_view, map_item, &box))
    {
        if (x < box.x0)
	{
	    limit = TRUE;
	    flags = be_link_BACK;
	}
        if (x > box.x1)
	{
	    limit = TRUE;
	    flags = 0;
	}
        if (y < box.y0)
	{
	    limit = TRUE;
	    flags = be_link_VERT;
	}
        if (y > box.y1)
	{
	    limit = TRUE;
	    flags = be_link_VERT | be_link_BACK;
	}
    }

    if (limit)
	return flags | be_link_MOVE_POINTER;

    frontend_pointer_set_position(NULL, x, y);

    return 0;
}

/* ----------------------------------------------------------------------------- */

#define MAX_SPEED       64
#define REPEAT_DELAY    40
#define SINGLE_STEP     8
#define FACTOR          1.3
#define MULT            5

static int cont = 0;
static int t_last[8] = { 0 };

static int change_step_speed(void)
{
    int step;
    cont++;
    step = (int)(MULT*pow(FACTOR, cont));
    if (step > MAX_SPEED)
        step = MAX_SPEED;
    return step;
}

static int check_move(int *last_t)
{
    int t = alarm_timenow();
    int diff = t - *last_t;
    int dd;

    if (diff <= REPEAT_DELAY)
    {
        dd = change_step_speed();
    }
    else
    {
        cont = 0;
        dd = SINGLE_STEP;
    }

    *last_t = t;

    return dd;
}

void fe_map_event_handler(int event, fe_view v)
{
    int x = 0, y = 0;
    wimp_mousestr m;

    wimp_get_point_info(&m);

    STBDBG(("fe_map_event_handler(): event %x v %p @ %d,%d\n", event, v, m.x, m.y));
    
    switch (event)
    {
        case fevent_MAP_SELECT:
        {
            fe_view v = map_view;
            fe_map_mode(NULL, NULL);            /* clears map_view variable */
            fe_fake_click(v, m.x, m.y);
            break;
        }

        case fevent_MAP_STOP:
        case fevent_MAP_CANCEL:
            fe_map_mode(NULL, NULL);
            break;

        case fevent_MAP_MOVE | fevent_MAP_MOVE_LEFT:
            x = - check_move(&t_last[0]);
            break;

        case fevent_MAP_MOVE | fevent_MAP_MOVE_RIGHT:
            x =   check_move(&t_last[1]);
            break;

        case fevent_MAP_MOVE | fevent_MAP_MOVE_UP:
            y =   check_move(&t_last[2]);
            break;

        case fevent_MAP_MOVE | fevent_MAP_MOVE_DOWN:
            y = - check_move(&t_last[3]);
            break;

        case fevent_MAP_MOVE | fevent_MAP_MOVE_LEFT | fevent_MAP_MOVE_UP:
            x = - check_move(&t_last[4]);
            y = - x;
            break;

        case fevent_MAP_MOVE | fevent_MAP_MOVE_LEFT | fevent_MAP_MOVE_DOWN:
            x = - check_move(&t_last[5]);
            y =   x;
            break;

        case fevent_MAP_MOVE | fevent_MAP_MOVE_RIGHT | fevent_MAP_MOVE_UP:
            x =   check_move(&t_last[6]);
            y =   x;
            break;

        case fevent_MAP_MOVE | fevent_MAP_MOVE_RIGHT | fevent_MAP_MOVE_DOWN:
            x =   check_move(&t_last[7]);
            y = - x;
            break;
    }

    if (x || y)
    {
        int flags = fe_map_update_position_to(m.x + x, m.y + y);
	if (flags)
	{
	    fe_map_mode(NULL, NULL);
	    fe_move_highlight(v, flags);
	}
    }
}

void fe_map_check_pointer_move(const wimp_mousestr *m)
{
    m = m;
}

void fe_map_mode(fe_view v, be_item ti)
{
    map_view = v;
    map_item = ti;

    if (v)
    {
        fe_set_pointer(1U<<31);
	sound_event(snd_MODE_MAP_START);
    }
    else
    {
        fe_set_pointer(0);
	sound_event(snd_MODE_MAP_END);
    }
    
    STBDBG(("stbmap: mode v %p item %p\n", v, ti));
}

fe_view fe_map_view(void)
{
    return map_view;
}

/* ----------------------------------------------------------------------------- */

/* eof stbmap.c */
