/* frameutils.c */

/* Bits of frames code common to NCFresco and desktop Fresco
 *
 * Authors:
 *      Simon Middleton <smiddleton@acorn.co.uk>
 *      Peter Hartley   <peter@ant.co.uk>
 *
 */

/* RiscOSLib includes */
#include "coords.h"
#include "wimp.h"
#include "msgs.h"

/* Application includes */
#include "debug.h"
#include "interface.h"
#ifdef STBWEB
#include "stbview.h"
#include "stbtb.h"
#include "stbfe.h"
#else
#include "view.h"
#endif
#include "frameutils.h"


/* forward reference */

static void frameutils_resizing_start( fe_view v, const wimp_mousestr *m, const wimp_box *bounds, int type);


fe_view frameutils_find_top(fe_view v)
{
    if (v) while (v->parent)
        v = v->parent;
    return v;
}


/* frameutils_get_cvt
 * Returns cvtstr for the view even if it's not yet been opened
 */

coords_cvtstr frameutils_get_cvt(fe_view v)
{
    coords_cvtstr cvt;
    if (v->w)
    {
        wimp_wstate state;
        if (frontend_complain(wimp_get_wind_state(v->w, &state)) != NULL)
	{
            STBDBG(( "win %x v %p\n", v->w, v));
	}
        return *(coords_cvtstr *)&state.o.box;
    }

    cvt.box = v->box;
    cvt.scx = cvt.scy = 0;
    return cvt;
}


/* frameutils_check_resize
 * Are we on a frame resizing zone?
 */

int frameutils_check_resize(fe_view start, int x, int y, wimp_box *box, int *handle, fe_view *resizing_v)
{
    fe_view v;
    for (v = start; v; v = v->next)
    {
#if 0
        STBDBG(( "resize: view %p\n", v));
#endif
        if (v->displaying)
        {
            coords_cvtstr cvt = frameutils_get_cvt(v);

            int type = backend_frame_resize_bounds(v->displaying,
                coords_x_toworkarea(x, &cvt),
                coords_y_toworkarea(y, &cvt),
                box, handle);

            if (type != be_resize_NONE)
            {
                if (resizing_v)
                    *resizing_v = v;
                return type;
            }
        }

        if (v->children)
        {
            int type = frameutils_check_resize(v->children, x, y, box, handle, resizing_v);
            if (type != be_resize_NONE)
                return type;
        }
    }
    return be_resize_NONE;
}

BOOL frameutils_maybe_resize( fe_view v, wimp_mousestr *m )
{
    wimp_box box;
    int type;
    fe_view vv;
    int resize_handle;

    type = frameutils_check_resize(v, m->x, m->y, &box, &resize_handle, &vv);

    if (type != be_resize_NONE)
    {
        coords_cvtstr cvt = frameutils_get_cvt(vv);
        vv->resize_handle = resize_handle;
#if 0
        LAYDBG(( "vw%p: resize %d\n", v, type));
#endif
        coords_box_toscreen(&box, &cvt);
        frameutils_resizing_start(vv, m, &box, type);
        return TRUE;
    }
    return FALSE;
}

static fe_view resizing_view = NULL;

BOOL frameutils_are_we_resizing( void )
{
    return resizing_view ? TRUE : FALSE;
}

static void frameutils_resizing_start( fe_view v, const wimp_mousestr *m, const wimp_box *bounds, int type)
{
    wimp_dragstr drag;

    /* we always actually drag the top window */
    drag.window = frameutils_find_top(v)->w;
    drag.type = wimp_USER_FIXED;
    drag.parent = *bounds;
    drag.box = *bounds;
    if (type == be_resize_WIDTH)
        drag.box.x0 = drag.box.x1 = m->x;
    else
        drag.box.y0 = drag.box.y1 = m->y;

    LAYDBG(( "resizing: start box %d,%d %d,%d\n", drag.parent.x0, drag.parent.y0, drag.parent.x1, drag.parent.y1));

    if (frontend_complain(wimp_drag_box(&drag)) == NULL)
    {
#if 0
        dragging_last_x = m->x;
        dragging_last_y = m->y;
#endif

        /* record the view containing the framese we are resizing */
        resizing_view = v;

#ifdef STBWEB
	if (use_toolbox)
	    tb_status_set_message(status_type_HELP, msgs_lookup("hrsz2"));
#endif

        LAYDBG(( "resizing: started at %d,%d\n", m->x, m->y));
    }
}

BOOL frameutils_resize( const wimp_mousestr *m )
{
    if (m->bbits == 0)
    {
        fe_view v = resizing_view;
        coords_cvtstr cvt = frameutils_get_cvt(v);

        resizing_view = NULL;

#ifdef STBWEB
	if (use_toolbox)
	    tb_status_set_message(status_type_HELP, NULL);
#endif

        LAYDBG(( "resizing: buttons released\n"));

        backend_frame_resize(v->displaying,
            coords_x_toworkarea(m->x, &cvt),
            coords_y_toworkarea(m->y, &cvt),
            v->resize_handle);

#ifdef STBWEB
	fe_refresh_window(frameutils_find_top(v)->w, NULL);
#else
        frontend_view_redraw( v->parent ? v->parent : v, NULL );
#endif

        return TRUE;
    }
#if 0
    else if (m->x != dragging_last_x || m->y != dragging_last_y)
    {
#if 0
        STBDBG(( "resizing: view %p to %d,%d\n", resizing_view, m->x, m->y));
#endif
        dragging_last_x = m->x;
        dragging_last_y = m->y;
    }
#endif
    return FALSE;
}

void frameutils_resize_abort( void )
{
    if ( resizing_view )
    {
        wimp_drag_box(NULL);
        resizing_view = NULL;
    }
}
