/* > imagemap.c

 * derived from W3C server code (HTImage.c)

 * Note x and y are offsets within the image taken relative to the top left corner
 * with +ve being right and down.

 * Most image map definitions are contained within the same file as their use so this is easy.
 * Some however are stored in separate files. These need to be downloaded and parsed as soon
 * as we know about them, without waiting for an access.

 * These pages will probably not contain any viewable text (I assume) so the plan is to
 * download the page, parse it, extract the map information and attach it to the map list
 * in the rid_header of the antweb_doc that requested it, then discard the page.

 * We can transfer the map_list wholesale except that we then eed to go through and patch up
 * the names so that they include the URL in them so that find_map will work correctly.

 * Note: this file is immune from display scaling as it is done to the values passed in.

 * 06/05/96: SJM: Added DEFAULT attribute. matches any point
 * 20/06/96: SJM: Added POINT attribute.

 */

#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "bbc.h"

#include "memwatch.h"
#include "status.h"
#include "util.h"
#include "rid.h"
#include "antweb.h"
#include "parsers.h"
#include "url.h"
#include "images.h"

#include "config.h"

#include "interface.h"
#include "imagemap.h"
#include "backend.h"

#define HTMIN(a,b)  ((a) < (b) ? (a) : (b))
#define HTMAX(a,b)  ((a) > (b) ? (a) : (b))

#define IMAGEMAP_POLL_INTERVAL   100

/*
** ******************************************************************
**	ROUTINES FOR POINT INSIDE/OUTSIDE RESOLUTION
** ******************************************************************
*/

#define SQR(x)   ((x) * (x))

static int inside_rect(rid_area_item *area, int x, int y)
{
    IMGDBG(( "check area %p rect %d,%d %d,%d\n", area, area->coords.rect.x0, area->coords.rect.y0, area->coords.rect.x1, area->coords.rect.y1));

    return HTMIN(area->coords.rect.x0, area->coords.rect.x1) <= x &&
	HTMAX(area->coords.rect.x0, area->coords.rect.x1) >= x &&
	HTMIN(area->coords.rect.y0, area->coords.rect.y1) <= y &&
	HTMAX(area->coords.rect.y0, area->coords.rect.y1) >= y;
}


static int inside_circle(rid_area_item *area, int x, int y)
{
    IMGDBG(( "check area %p circle %d,%d,%d\n", area, area->coords.circle.x, area->coords.circle.y, area->coords.circle.r));
    return SQR(area->coords.circle.x - x) + SQR(area->coords.circle.y - y) <= SQR(area->coords.circle.r);
}


static int inside_polygon(rid_area_item *area, int x, int y)
{
    intxy *a = area->coords.polygon.point;
    intxy *b = a+1;
    intxy *first = a;
    intxy *check_previous = 0;
    double cross_x;
    int crossings = 0;
    int i;

    IMGDBG(( "check area %p polygon %d points\n", area, area->coords.polygon.npoints));

    for (i = 0; i < area->coords.polygon.npoints-1; i++)
        {
    IMGDBG(( "check linee %d,%d to %d,%d\n", a->x, a->y, b->x, b->y));
	    if (x == a->x  &&  y == a->y) {
		/* Point is a vertex, so it's inside */
		return 1;
	    }
	    else if (a->y == b->y) {	/* Horizontal line */
		if (y == a->y &&
		    ((a->x <= x  &&  x <= b->x) ||
		     (a->x >= x  &&  x >= b->x))) {
		    /* Line horizontal and point is on that line */
		    return 1;
		}
	    }
	    else if (check_previous) {
		if ((check_previous->y < a->y  &&  a->y < b->y)  ||
		    (check_previous->y > a->y  &&  a->y > b->y)) {
		    /* Lines from vertex go to different sides -- crossing */
		    crossings++;
		}
		/* Else lines from vertex go to same side -- no crossing */
		check_previous = 0;
	    }
	    else if (y >= HTMIN(a->y, b->y) &&
		     y <= HTMAX(a->y, b->y) &&
		     x <= HTMAX(a->x, b->x)) {	/* If crossing is possible */

		if (a->x == b->x) {		/* Vertical line */
		    if (a->x == x) {
			/* Point is on an edge -- inside */
			return 1;
		    }
		    else if (a->x > x) {
			crossings++;
		    }
		}
		else {
		    /* At this point we know that lines cross -- the	*/
		    /* question is which side of the point they cross.	*/
		    cross_x = (double)a->x +
			((y - (double)a->y) * (b->x - (double)a->x)) /
			    (b->y - (double)a->y);

		    if (cross_x == b->x) {
			/* Crosses exactly at the ending vertex	*/
			/* -- handled on the next round.	*/
			check_previous = a;
		    }
		    else if (cross_x == x) {
			/* Point is on an edge -- inside */
			return 1;
		    }
		    else if (cross_x > x) {
			crossings++;
		    }
		}
	    }
	    /* Else crossing is not possible */

	a++;
	    b++;
	}

	if (check_previous) {
	    if ((check_previous->y < a->y  &&  a->y < first->y)  ||
		(check_previous->y > a->y  &&  a->y > first->y)) {
		/* Lines from vertex go to different sides -- crossing */
		crossings++;
	    }
	}

	return (crossings % 2) == 1;
}


rid_area_item *imagemap_find_area(rid_map_item * map, int x, int y)
{
    rid_area_item *area, *found;
/*     rid_area_item *default_area = NULL; */
    rid_area_item *closest_point = NULL;
    int closest_dist = INT_MAX;

    if (!map)
        return 0;

    IMGDBG(( "find area at %d,%d in %p\n", x, y, map));

    found = 0;
    for (area = map->area_list; !found && area; area = area->next)
    {
        switch (area->type)
        {
            case rid_area_RECT:
                if (inside_rect(area, x, y))
                    found = area;
                break;
            case rid_area_CIRCLE:
                if (inside_circle(area, x, y))
                    found = area;
                break;
            case rid_area_POLYGON:
                if (inside_polygon(area, x, y))
                    found = area;
                break;

            case rid_area_POINT:
            {
                int dx = (x - area->coords.point.x);
                int dy = (y - area->coords.point.y);
                int dist = dx*dx + dy*dy;

                if (dist < closest_dist)
                {
                    closest_point = area;
                    closest_dist = dist;
                }
                break;
            }

            case rid_area_DEFAULT:
#if 1		/* used to save checking default till last however looks like established practise
		 * is to check it at its position (ignoring anything afterwards)
		 */
		found = area;
#else
                default_area = area;
#endif
                break;
        }
    }

    /* only if coords are not withint a rect, circle or polygon do we check point and then default */
    if (!found && closest_point)
        found = closest_point;

#if 0
    if (!found && default_area)
        found = default_area;
#endif

    IMGDBG(( "found at %p\n", found));

    return found;
}

#ifdef STBWEB
rid_area_item *imagemap_find_area_from_name(rid_header * rh, const char *name, int x, int y)
{
    rid_map_item *map;
    map = imagemap_find_map(rh, name);
    if (map)
        return imagemap_find_area(map, x, y);
    return NULL;
}
#endif

rid_map_item *imagemap_find_map(rid_header * rh, const char *name)
{
    rid_map_item *map;

    IMGDBG(( "find map '%s' ", name ? name : "<none>"));

    if (!name)
        return 0;

    if (name[0] == '#')
        name++;

    for (map = rh->map_list; map; map = map->next)
    {
        if (strcasecomp(map->name, name) == 0)
            break;
    }

    IMGDBG(( "- found at %p\n", map));

    return map;
}

#if 0
/* not in use until we have some way of selecting the individual areas of a map */
void imagemap_draw_area(antweb_doc *doc, void *im, rid_area_item *area, int xb, int yb)
{
    rid_area_item *area = tii->data.usemap.selection;
    int dx, dy;

    if (!area)
        return;

    image_get_scales((image)im, &dx, &dy);

    switch (area->type)
    {
        case rid_area_RECT:
        {
            wimp_box box;
            box.x0 = area->coords.rect.x0*doc->scale_value/100*dx;
            box.y0 = area->coords.rect.y0*doc->scale_value/100*dy;
            box.x1 = area->coords.rect.x1*doc->scale_value/100*dx;
            box.y1 = area->coords.rect.y1*doc->scale_value/100*dy;
            bbc_rectangle(xb + box.x0,   yb - box.y0, box.x1-box.x0-1, -(box.y1-box.y0-1));
            bbc_rectangle(xb + box.x0-2, yb - (box.y0-2), box.x1-box.x0+4-1, -(box.y1-box.y0+4-1));
            break;
        }

        case rid_area_CIRCLE:
        {
            int x = xb + area->coords.circle.x*doc->scale_value/100*dx;
            int y = yb - area->coords.circle.y*doc->scale_value/100*dy;
            int r = area->coords.circle.r*doc->scale_value/100*2;
            bbc_circle(x, y, r);
            bbc_circle(x, y, r+2);
            break;
        }

        case rid_area_POLYGON:
        {
            int i;
            intxy *p = area->coords.polygon.point;
            bbc_move(xb + p->x*doc->scale_value/100*dx, yb - p->y*doc->scale_value/100*dy);
            for (i = 1, p++; i < area->coords.polygon.npoints; i++, p++)
                bbc_draw(xb + p->x*doc->scale_value/100*dx, yb - p->y*doc->scale_value/100*dy);
            break;
        }

        case rid_area_POINT:
        {
            int x = xb + area->coords.point.x*doc->scale_value/100*dx;
            int y = yb - area->coords.point.y*doc->scale_value/100*dy;
            bbc_circle(x, y, 8);
            break;
        }

        case rid_area_DEFAULT:
            break;
    }
}
#endif

#if 1

static void add_blank_map(antweb_doc *doc, const char *url, const char *frag)
{
    rid_map_item *map = mm_calloc(sizeof(rid_map_item), 1);

    /* fill in name */
    if (frag)
    {
        map->name = mm_malloc(strlen(url) + 1 + strlen(frag) + 1);
        sprintf(map->name, "%s#%s", url, frag);
    }
    else
    {
        map->name = strdup(url);
    }

    /* set error bit */
    map->flags = rid_map_ERROR;

    /* link into list */
    map->next = doc->rh->map_list;
    doc->rh->map_list = map;

    IMGDBG(( "imagemap error: adding blank '%s'\n", map->name));
}

extern pparse_details *be_lookup_parser(int ft);

static access_complete_flags imagemap_doc_complete(void *h, int status, char *cfile, char *url)
{
    antweb_doc *doc = h;
    antweb_doc *parent_doc;
    int ft;
    pparse_details *pd;

    parent_doc = (antweb_doc*)doc->parent;

    IMGDBG(( "imagemap document complete: parent_doc %p status %d url '%s'\n", parent_doc, status, url));
    /* clear access handle since its finished */
    doc->ah = NULL;

    if (status != status_COMPLETED_FILE)
    {
        add_blank_map(parent_doc, doc->url, doc->frag);

        backend_dispose_doc(doc);
        parent_doc->fetching = NULL;

        imagemap_check_all_images(parent_doc);
        return 0;
    }

    ft = file_type(cfile);
    pd = be_lookup_parser(ft);
    if (pd->whole)
    {
        rid_header *rh = pd->whole(cfile, url);
        if (rh && rh->map_list)
        {
            rid_map_item *map, *map_last;
            int url_len;

            /* we use the url we asked for as that is what the map will reference */
            /* and it may have changed by the time it gets back to us */
            url_len = strlen(doc->url);
            map_last = NULL;
            for (map = rh->map_list; map; map = map->next)
            {
                char *s;

                /* add url on to start of name */
                s = mm_malloc(url_len + 1 + strlen(map->name) + 1);
                sprintf(s, "%s#%s", doc->url, map->name);
                mm_free(map->name);
                map->name = s;

                map_last = map;
            }

            /* transfer new maps onto existing maps */
            map_last->next = parent_doc->rh->map_list;
            parent_doc->rh->map_list = rh->map_list;

            /* clear the pointer so they don't get freed */
            doc->rh->map_list = NULL;
        }
        else
            add_blank_map(parent_doc, doc->url, doc->frag);
    }
    else
        add_blank_map(parent_doc, doc->url, doc->frag);

    /* dispose of the doc */
    backend_dispose_doc(doc);
    parent_doc->fetching = NULL;

    /* see if there are any more to do */
    imagemap_check_all_images(parent_doc);

    return access_CACHE;
}

static os_error *imagemap_check_file(antweb_doc *parent_doc, const char *url)
{
    os_error *e;
    antweb_doc *doc;
    char *frag, *use_url;

    IMGDBG(( "imagemap: check url %s\n", url));

    doc = mm_calloc(1, sizeof(antweb_doc));

    /* strip off the map name when accessing it */
    doc->url = strdup(url);
    if ( (frag = strrchr(doc->url, '#')) != 0)
    {
        *frag = 0;
        doc->frag = strdup(frag+1);
    }

    /* join on the base ref */
    use_url = url_join(BASE(parent_doc), doc->url);

    /* misuse the parent field - its meant to be a parent view */
    doc->parent = (fe_view) parent_doc;

    /* set fetching here as if the file is cached it will be loaded and cleared within call to access_url */
    parent_doc->fetching = doc;

    e = access_url(use_url, 0 /*flags*/, NULL /*ofile*/, NULL /*bfile*/, NULL, NULL /*referrer*/, NULL /*progress*/, imagemap_doc_complete, doc, &doc->ah);

    mm_free(use_url);

    /* if an error and not already disposed of then clear up and continue */
    /* if fetching is null it implies doc_complete has been called */
    if (e && parent_doc->fetching)
    {
        parent_doc->fetching = NULL;
        backend_dispose_doc(doc);

        imagemap_check_all_images(parent_doc);
    }

    return e;
}

/*
 * Called only from the alarm function. This function will go and fetch one file.
 */

static void imagemap_check_all_image_fn(int called_at, void *handle)
{
    antweb_doc *doc = handle;
    rid_text_item *ti;

    if (doc->fetching)
        return;

    IMGDBG(( "imagemap: check all %p\n", doc));

    /* first item */
    ti = doc->rh->stream.text_list;

    IMGDBG(( "imagemap: scan from %p\n", ti));

    for (ti = rid_scan(ti, SCAN_THIS | SCAN_FWD | SCAN_RECURSE | SCAN_FILTER | rid_tag_IMAGE);
	 ti;
	 ti = rid_scan(ti, SCAN_FWD | SCAN_RECURSE | SCAN_FILTER | rid_tag_IMAGE))
    {
        rid_text_item_image *tii = (rid_text_item_image *)ti;
        char *url = tii->usemap;

        /* url, non-local and not already fetched the file */
        if (url && url[0] != '#' && strchr(url, '#') != NULL && imagemap_find_map(doc->rh, url) == NULL)
        {
            os_error *e = imagemap_check_file(doc, url);
            if (e)
                usrtrc( "imagemap: error &%x '%s'\n", e->errnum, e->errmess);
            return;
        }
    }

    IMGDBG(( "imagemap: checked all in %p fetching %p\n", doc, doc->fetching));
}

/*
 * Schedule the next check.
 */

os_error *imagemap_check_all_images(antweb_doc *doc)
{
    alarm_set(alarm_timenow() + IMAGEMAP_POLL_INTERVAL, imagemap_check_all_image_fn, doc);
    return NULL;
}

#endif

/* eof imagemap.c */
