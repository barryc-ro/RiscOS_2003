 /* > stbopen.c

 * Handle the frontend_open_url function and some derivatives

 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "wimp.h"
#include "werr.h"

#include "makeerror.h"
#include "interface.h"
#include "config.h"
#include "memwatch.h"
#include "util.h"
#include "version.h"

#include "stbview.h"
#include "stbhist.h"
#include "stbopen.h"
#include "stbutils.h"
#include "stbfe.h"
#include "stbtb.h"
#include "frameutils.h"

/* ------------------------------------------------------------------------------------------- */

char *fe_frame_specifier_create(fe_view v, char *buf, int len)
{
    char dig[8];
    sprintf(dig, "_%d", v->frame_index);

    if (v->parent)
	fe_frame_specifier_create(v->parent, buf, len - strlen(buf));

    strlencat(buf, dig, len);

    return buf;
}

fe_view fe_frame_specifier_decode(fe_view top, const char *spec)
{
    char *ss = strdup(spec), *s;
    fe_view v;

    s = strtok(ss, "_");
    v = NULL;

    STBDBGN(("framespec_decode: from %p for '%s'\n", top, spec));
    if (s)
    {
	do
	{
	    int index = atoi(s);

	    STBDBGN(("framespec_decode: index %d", index));

	    if (v == NULL)
		v = top;
	    else
		v = v->children;

	    while (v && index--)
		v = v->next;

	    STBDBGN((" yields %p\n", v));
	}
	while ((s = strtok(NULL, "_")) != NULL);
    }

    mm_free(ss);

    return v;
}

/* ------------------------------------------------------------------------------------------- */

fe_view fe_find_target(fe_view start, const char *target)
{
    fe_view v = NULL;
    if (target) for (v = start; v; v = v->next)
    {
        if (v->name && strcasecomp(v->name, target) == 0)
            break;

        if (v->children)
        {
            fe_view vv = fe_find_target(v->children, target);
            if (vv)
            {
                v = vv;
                break;
            }
        }
    }
    return v;
}

/* ------------------------------------------------------------------------------------------- */

fe_view fe_find_window(fe_view start, wimp_w w)
{
    fe_view v;
    for (v = start; v; v = v->next)
    {
        if (v->w == w)
            break;

        if (v->children)
        {
            fe_view vv = fe_find_window(v->children, w);
            if (vv)
            {
                v = vv;
                break;
            }
        }
    }
    return v;
}

fe_view fe_find_top_popup(fe_view v)
{
    v = frameutils_find_top(v);

    if (v) while (v->next)
	v = v->next;

    return v;
}

fe_view fe_find_top_nopopup(fe_view v)
{
    v = frameutils_find_top(v);

    if (v) while (v->prev)
	v = v->prev;

    return v;
}

int fe_popup_open(void)
{
    return main_view->next != NULL;
}

int fe_external_popup_open(void)
{
    return main_view->next != NULL && strncmp(main_view->next->name, TARGET_DBOX, sizeof(TARGET_DBOX)-1) == 0;
}

/* ------------------------------------------------------------------------------------------- */

/*
 * Check to see if new_url is the same as the urls of any view from v upwards
 */

static BOOL check_recursion(fe_view v, const char *new_url)
{
    STBDBG(("stbfe: check_recursion: new '%s'\n", new_url));

    while (v)
    {
        char *url;
        backend_doc_info(/* v->fetching ? v->fetching : */ v->displaying, NULL, NULL, &url, NULL);

	STBDBG(("stbfe: check_recursion: '%s' fetching %d from_frame %d\n", strsafe(url), v->fetching ? 1 : 0, v->from_frame));

	if (strcmp(new_url, url) == 0) /* was strcasecomp */
            return TRUE;

	/* stop after we've checked the page that initiated this fetch originally */
	if (!v->from_frame)
	    return FALSE;

        v = v->parent;
    }
    return FALSE;
}


/* ------------------------------------------------------------------------------------------- */

/*
 * frontend_open_url is called when the front or backends want to open a new page.
 * We discard any page currently being fetched.
 * At this point we should initialise the fetch status.

 * Targetting:

 * There is a hierarchy of views.
 * The top is the outermost frameset document. This encloses a number of nested documents,
 * any of which could be a new frameset document, etc. Recursion is not allowed and this level
 * should check this and ignore the repeated url.

 * special names are
   _blank: use a new window
   _self: use this frame (equivalent to windowname==NULL)
   _parent: use the next frame/document up
   _top: use the uppermost one of the hierarchy

 * parent in this call could point to any one of the views in the hierarchy. We need to ascend to
 * the top and then locate the correct view.

 */

os_error *frontend_open_url(char *url, fe_view parent, char *target, fe_post_info *bfile, int flags)
{
    os_error *ep;
    char *referer = NULL, *title = NULL;
    int oflags;
    BOOL is_internal;

    DBG(("frontend_open_url '%s' in window '%s' parent v%p '%s' flags %x\n", url ? url : "<none>", target ? target : "<none>", parent, parent ? parent->name : "", flags));

    if (target && parent)
    {
        if (strcasecomp(target, TARGET_SELF) == 0)
        {
            /* use current window*/
        }
        else if (strcasecomp(target, TARGET_PARENT) == 0)
        {
            /* up the stack one*/
            parent = parent->parent;
        }
        else if (strcasecomp(target, TARGET_BLANK) == 0)
        {
            /* straight to the top*/
            parent = main_view;
        }
        else if (strcasecomp(target, TARGET_TOP) == 0)
        {
            parent = frameutils_find_top(parent);
        }
        else if (target[0] == '_' && isdigit(target[1]))
	{
	    parent = fe_frame_specifier_decode(frameutils_find_top(parent), target);
	}
	else
        {
            parent = fe_find_target(frameutils_find_top(parent), target);
        }
    }

    /* Special targets open up a transient window */
    if (target && parent == NULL && strncmp(target, "__", 2) == 0 && strcasecomp(target, "__top") != 0)
    {
	parent = fe_find_target(main_view, target);
	if (!parent)
	    parent = fe_dbox_view(target);
    }

    /* don't check recursion unless this was initiated from a frameset */
    if (parent && (flags & fe_open_url_FROM_FRAME) && check_recursion(parent->parent, url))
    {
	STBDBG(("Frame recursion detected - aborting fetch"));
	return NULL;
    }

    STBDBG(("found view '%p' fetching %p\n", parent, parent ? parent->fetching : NULL));

    if (parent == NULL)
        parent = main_view;

    if (parent && parent->fetching)
    {
        char *fetching;

        /* if re-requesting the same URL then ignore click
         * note that this doesn't cope with fragment ids as 'fetching' will
         * never have the fragment in it and 'url' will.
         */

	STBDBG(("requesting info\n"));

	ep = backend_doc_info(parent->fetching, NULL, NULL, &fetching, NULL);

	STBDBG(("requested info e %p title '%s'\n", ep, strsafe(fetching)));

        if (!ep && fetching && strcmp(fetching, url) == 0)
	{
	    STBDBG(("already fetching returning\n"));
	    return NULL;
	}

	STBDBG(("disposing of fetching\n"));

	backend_dispose_doc(parent->fetching);
	parent->fetching = NULL;
    }

    parent->dont_add_to_history = (flags & fe_open_url_NO_HISTORY) != 0;
    parent->pending_download_finished = FALSE;

    STBDBG(("checking currently displaying %p\n", parent->displaying));

    if (parent->displaying)
    {
	if (backend_doc_info(parent->displaying, NULL, NULL, &referer, &title) != NULL)
	    referer = NULL;

/* 	if (parent->hist_at) */
/* 	{ */
/* 	    wimp_wstate state; */
/* 	    wimp_get_wind_state(parent->w, &state); */
/* 	    STBDBG(("history: %p '%s' write scroll pos %d\n", parent->hist_at, parent->hist_at->url, state.o.box.y1)); */
/* 	    parent->hist_at->scroll_pos = state.o.y + parent->margin.y1; */
/* 	} */
    }

    STBDBG(("check referer '%s'\n", strsafe(referer)));

    /* If it's the same page then just move the right point */
    if (referer) /*  && (flags & fe_open_url_NO_CACHE) == 0) */
    {
        char *frag = strrchr(url, '#');
	if (frag && strncmp(referer, url, frag ? frag - url : strlen(url)) == 0)
	{
            fe_view top = frameutils_find_top(parent);

	    /* 30Jun97 added frame and fetching_data check*/
	    if (top->fetching_data.hist)
	    {
		top->hist_at = top->fetching_data.hist;
		top->fetching_data.hist = NULL;
		DBG(("history: v%p top%p set hist_at to %p\n", parent, top, top->hist_at));
	    }
	    else if ((flags & fe_open_url_FROM_FRAME) == 0)
	    {
		fe_history_visit(parent, url, title);
	    }

	    frontend_view_status(parent, sb_status_URL, url);

	    if ((ep = backend_goto_fragment(parent->displaying, frag+1)) != NULL)
		return ep;

	    /* need to ensure highlight is visible after jumping to fragment */
	    fe_ensure_highlight_after_fetch(parent);
	    
	    return NULL;
        }
    }

    /* close the keyboard when opening any URL */
    fe_keyboard_close();
    
    session_log(url, session_REQUESTED);

    /* open the fetch status */
    if (keyboard_state == fe_keyboard_ONLINE)
	fe_status_open_fetch_only(parent);

    is_internal = strncmp(url, PROGRAM_NAME"internal:", sizeof(PROGRAM_NAME"internal:")-1) == 0 ||
	strncmp(url, "ncint:", sizeof("ncint:")-1) == 0;
    
    /* move the highlight */
    if ((flags & fe_open_url_FROM_FRAME) == 0 &&
	!parent->open_transient &&
	!is_internal &&
	pointer_mode == pointermode_OFF &&
	use_toolbox && tb_is_status_showing() &&
	config_mode_cursor_toolbar)
    {
	tb_status_highlight_stop();
    }

    /* setup the opening flags */
    if (flags & fe_open_url_NO_CACHE)
        oflags = parent->flags | be_openurl_flag_NOCACHE;
    else
        oflags = parent->flags &~ be_openurl_flag_NOCACHE;

    if ((flags & fe_open_url_FROM_HISTORY) || (parent->browser_mode == fe_browser_mode_HISTORY))
	oflags |= be_openurl_flag_HISTORY;

     if (is_internal)
        oflags |= be_openurl_flag_BODY_COLOURS;

     if (keyboard_state == fe_keyboard_OFFLINE)
	 oflags |= be_openurl_flag_FAST_LOAD;

#if 0 /* DEBUG */
    if (bfile && bfile->body_file)
    {
        char buf[256];
        strcpy(buf, "filer_run ");
        strcat(buf, bfile->body_file);
        wimp_starttask(buf);
    }
#endif

    parent->fetch_images = 0;
    parent->had_completed = FALSE;
    parent->images_had = parent->images_waiting = 0;

    parent->from_frame = flags & fe_open_url_FROM_FRAME ? 1 : 0;

    parent->threaded++;

    /* Abort the current fetch before getting the new page */
    if ((flags & fe_open_url_FROM_FRAME) == 0)
	fe_abort_fetch(parent, TRUE);
    
    STBDBG(("frontend_open_url: backend IN transient %d\n", parent->open_transient));
    ep = backend_open_url(parent, &parent->fetching, url, bfile, flags & fe_open_url_NO_REFERER ? NULL : referer, oflags);
    STBDBG(("frontend_open_url: backend OUT fetching %p error %x\n", parent->fetching, ep ? ep->errnum : 0));

    if (ep)
    {
	if (ep->errnum == ANTWEB_ERROR_BASE + ERR_NO_ACTION)
	{
	    fe_no_new_page(parent, NULL);
	    fe_ensure_highlight_after_fetch(parent);
	    ep = NULL;
	}
	else if (parent->open_transient)
	    fe_dispose_view(parent);
    }
    else
    {
	if (keyboard_state == fe_keyboard_OFFLINE && parent->fetching)
	    fe_status_open_fetch_only(parent);

	/* if there was no error then check to see if download has finished */
/*  	fe_check_download_finished(parent); */
    }

    if (--parent->threaded == 0 && parent->delete_pending > 0)
    {
	fe_no_new_page(parent, NULL);
/*   	fe_check_download_finished(parent); */
	fe_dispose_view(parent);

/* 	fe_ensure_highlight(main_view, 0); */
    }

    return ep;
}

/* ------------------------------------------------------------------------------------------- */

/* open a file given a RISC OS filename*/

os_error *fe_show_file(fe_view v, char *file, int no_history)
{
    char *url;
    os_error *ep;

    ep = fe_file_to_url(file, &url);
    if (ep)
        return ep;

    ep = frontend_open_url(url, v, TARGET_TOP, NULL, fe_open_url_NO_CACHE | (no_history ? fe_open_url_NO_HISTORY : 0) | fe_open_url_NO_REFERER);

    mm_free(url);

    return ep;
}

os_error *fe_show_file_in_frame(fe_view v, char *file, char *frame)
{
    char *url;
    os_error *ep;

    ep = fe_file_to_url(file, &url);
    if (ep)
        return ep;

    ep = frontend_open_url(url, v, frame, NULL, fe_open_url_NO_CACHE | fe_open_url_NO_HISTORY | fe_open_url_NO_REFERER);

    mm_free(url);

    return ep;
}

os_error *fe_internal_url_with_source(fe_view v, const char *internal, const char *target)
{
    char buffer[256];

    strcpy(buffer, "ncint:");
    strlencat(buffer, internal, sizeof(buffer));
    strlencat(buffer, "&source=", sizeof(buffer));
    fe_frame_specifier_create(v, buffer, sizeof(buffer));

    return frontend_open_url(buffer, NULL, (char *)target, NULL, fe_open_url_NO_CACHE);
}

/* ------------------------------------------------------------------------------------------- */

static os_error *fe__reload_possible(fe_view v, void *handle)
{
    int *possible = handle;

    if (v && v->displaying && v->browser_mode == fe_browser_mode_WEB)
	(*possible)++;

    return NULL;
}

static os_error *fe__reload(fe_view v, void *handle)
{
    os_error *ep = NULL;

    if (v && v->displaying && v->browser_mode == fe_browser_mode_WEB)
    {
        char *url;

	/* First, flush all the images. */
	ep = backend_doc_flush_image(v->displaying, 0, be_openurl_flag_DEFER_IMAGES);
	if (ep)
	    return ep;

	if (!ep) ep = backend_doc_info(v->displaying, NULL, NULL, &url, NULL);

	/* We have to take a copy because if the file is from local disc
	 * the 'displaying' doc will be removed before
	 * the operation has completed
	 * We use the NO_HISTORY flag so that it doesn't get entered into the history.
	 */
        if (!ep)
        {
	    wimp_wstate state;

            url = strdup(url);

	    /* set scroll offsets so we reload to the same position
	    wimp_get_wind_state(v->w, &state);
	    v->fetching_data.xscroll = state.o.x + v->margin.x0;
	    v->fetching_data.yscroll = state.o.y + v->margin.y1;
	    */

	    ep = frontend_complain(frontend_open_url(url, v, TARGET_SELF, NULL,
						     fe_open_url_NO_CACHE | fe_open_url_NO_REFERER));
            mm_free(url);
        }
    }

    return ep;
    NOT_USED(handle);
}

int fe_reload_possible(fe_view v)
{
    int possible = 0;
    iterate_frames(v, fe__reload_possible, &possible);
    return possible != 0;
}

os_error *fe_reload(fe_view v)
{
    sound_event(snd_RELOAD);

    /* set the fetching_data hist ptr so that we don't get added to the history */
    v->fetching_data.hist = v->hist_at;

    return iterate_frames(v, fe__reload, NULL);
}

/* ------------------------------------------------------------------------------------------- */

os_error *fe_new_view(fe_view parent, const wimp_box *extent, const fe_frame_info *ip, BOOL open, fe_view *vp)
{
    os_error *e;
    fe_view view = mm_calloc(1, sizeof(*view));
    wimp_box visible;

    view->magic = ANTWEB_VIEW_MAGIC;

    view->pending_user = -1;
    
    if (config_defer_images)
	view->flags |= be_openurl_flag_DEFER_IMAGES;
    if (config_display_antialias)
	view->flags |= be_openurl_flag_ANTIALIAS;
    if (config_display_body_colours)
	view->flags |= be_openurl_flag_BODY_COLOURS;

    /* if a top level window */
    if (parent == NULL)
    {
	view->margin = ip->margin;

	view->backend_margin.x0 =  20;
	view->backend_margin.x1 = -20;
	view->backend_margin.y0 =  10;
	view->backend_margin.y1 = -10;

#if 0
	/* took this out as although it gives some more area usually
	 * it also makes some pages unviewable especially frames pages
	 */
 	if (view->margin.x0)	/* was is_a_tv() - really 'do we have a safe area?' */
	{
 	    view->margin.x0 -= view->backend_margin.x0;
	    view->margin.x1 -= view->backend_margin.x1;
	    view->margin.y0 -= view->backend_margin.y0;
	    view->margin.y1 -= view->backend_margin.y1;
	}
#endif
    }
    else
    {
	/* if a frame */
	view->backend_margin = ip->margin;
    }

    view->box = *extent;

    view->browser_mode = parent ? fe_browser_mode_WEB : fe_browser_mode_UNSET;

    visible = *extent;

    e = feutils_window_create(&visible, &view->margin, ip, fe_bg_colour(parent), open, &view->w); /* modifies extent*/
    if (e)
    {
        mm_free(view);
        return e;
    }

    STBDBG(("create win %x from view %p\n", view->w, view));

    view->scrolling = ip->scrolling;

    if (ip->scrolling == fe_scrolling_YES)
        view->x_scroll_bar = view->y_scroll_bar = TRUE;

    view->name = strdup(ip->name);
    view->parent = parent;

    if (ip->dividers)
	memcpy(view->dividers, ip->dividers, sizeof(view->dividers));
    view->dividers_max = 4;	/* ??? */

    /* add to end of chain in parent */
    /* if this changes then frontend_frame_layout(refresh) may have to change */
    if (parent)
    {
        view->prev = parent->children_last;
        view->next = NULL;

        if (parent->children_last)
            parent->children_last->next = view;
        else
            parent->children = view;

        parent->children_last = view;
    }

    if (vp)
    {
/* 	fe_internal_opening_view(view); */
        *vp = view;
    }

    return NULL;
}

/* ------------------------------------------------------------------------------------------- */

void fe_dispose_view_children(fe_view v)
{
    fe_view vv = v->children;
    while (vv)
    {
        fe_view next = vv->next;
        fe_dispose_view(vv);
        vv = next;
    }
    v->children = v->children_last = NULL;
}

void fe_dispose_view(fe_view v)
{
    wimp_caretstr cs;
    BOOL had_caret;

    if (!v)
        return;

    if (v->threaded)
    {
	v->delete_pending++;
	STBDBG(("fe_dispose_view: threaded pending %d\n", v->delete_pending));
	return;
    }
    else if (--v->delete_pending > 0)
    {
	STBDBG(("fe_dispose_view: unthreading pending %d\n", v->delete_pending));
	return;
    }

    STBDBG(("fe_dispose_view: disposing\n"));

    v->delete_pending = -1;

    /* restore pointer bounding box */
    if (strncmp(v->name, TARGET_DBOX, sizeof(TARGET_DBOX)-1) == 0)
	pointer_limit(screen_box.x0, screen_box.y0, screen_box.x1, screen_box.y1);

    fe_internal_deleting_view(v);

#if 1
    /* unlink from chain before deleting */
    if (v->prev)
	v->prev->next = v->next;
    if (v->next)
	v->next->prev = v->prev;
#endif

    wimp_get_caret_pos(&cs);
    had_caret = cs.w == v->w;

    if (v->displaying)
	backend_dispose_doc(v->displaying);

    if (v->fetching)
	backend_dispose_doc(v->fetching);

    fe_history_dispose(v);

    alarm_removeall(v);

    fe_dispose_view_children(v);

    if (v->w)
    {
        wimp_delete_wind(v->w);
	STBDBG(("delete win %x from view %p\n", v->w, v));
    }

    if (last_click_view == v)
        last_click_view = NULL;

    if (dragging_view == v)
        dragging_view = NULL;

    if (fe_map_view() == v)
        fe_map_mode(NULL, NULL);

    if (had_caret && v->parent)
        fe_get_wimp_caret(frameutils_find_top(v)->w);

/*     if (selected_view == v) */
/* 	selected_view = NULL; */
    if (v->is_selected)
    {
	/* FIXME: do something with selection */
    }

    fe_frame_link_array_free(v);
    
    mm_free(v->specialselect);
    mm_free(v->selected_id);
    mm_free(v->name);
    mm_free(v->return_page);
    mm_free(v->app_return_page);

    mm_free(v->pending_user_name);
    mm_free(v->onload);
    mm_free(v->onunload);
    mm_free(v->onblur);
    mm_free(v->submitonunload);
    mm_free(v->real_url);
    
    v->magic = 0;
    mm_free(v);
}

/* ------------------------------------------------------------------------------------------- */

static BOOL iconised = FALSE;

void fe_iconise(BOOL iconise)
{
/*     fe_view v = main_view; */

    if (iconise != iconised)
    {
	if (iconised)
	{
	}
	else
	{
	}

	iconised = iconise;
    }
}

/* ------------------------------------------------------------------------------------------- */

void view_dump(const char *name)
{
    fe_view v = fe_find_target(main_view, name);
    if (!v)
	return;

    DBG(("\n"));
    DBG(("view:        %p '%s'\n", v, v->name));
    DBG(("magic:       %x\n", v->magic));
    DBG(("links:       up %p prev %p next %p down %p down last %p\n", v->parent, v->prev, v->next, v->children, v->children_last));
    DBG(("document:    disp %p fetch %p\n", v->displaying, v->fetching));
    DBG(("open flags:  %x\n", v->flags));
    DBG(("doc size     %d x %d\n", v->doc_width, v->doc_height));
    DBG(("wimp:        %x\n", v->w));
    DBG(("safe margin: %d,%d %d,%d\n", v->margin.x0, v->margin.y0, v->margin.x1, v->margin.y1));
    DBG(("back margin: %d,%d %d,%d\n", v->backend_margin.x0, v->backend_margin.y0, v->backend_margin.x1, v->backend_margin.y1));
    DBG(("box:         %d,%d %d,%d\n", v->box.x0, v->box.y0, v->box.x1, v->box.y1));
    DBG(("scolling:    type %d bars %d,%d\n", v->scrolling, v->x_scroll_bar, v->y_scroll_bar));
    DBG(("stretch:     %d\n", v->stretch_document));
    DBG(("\n"));
}

/* ------------------------------------------------------------------------------------------- */

/* eof stbopen.c*/
