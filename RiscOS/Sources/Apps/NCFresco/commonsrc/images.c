/* -*-c-*- */

/* Image cache for images in documents */

/*
 * 18/3/96: SJM: Added w and h parameters to image_render for scaling images
 * 21/3/96: SJM: changed webimage include, fixed wide_table bug with wimp_readpixtrans
 *  3/5/96: SJM: added display_scale use and changed unset tii->ww and hh to be -1 rather than 0
 * 18/6/96: NvS: Fixed scale-to-fit for non-square pixels.
 * 24/6/96: SJM: changed image callbacks to use #defines and extended progress to turn world.
 * 19/7/96: SJM: tidied up some comments and conditional includes
 * 30/8/96: SJM: moved some routines around and started redoing animation code
 * 02/9/96: SJM: moved best colour check to portutil.c Remove callbacks from find_icontype
 * 03/9/96: SJM: Change callbacks from find_icontype to WORLD and added similar for cache completed case.
 * 04/9/96: SJM: added CHECK_EXPIRE flag to image_find.
 * 10/9/96: SJM: finished the new animation stuff. Moved around and commented lots of code.
 *		 Changes image_tile to not destroy the tile if the background changes but juet re render it.
 *		 Use logical sizes from image_rec. Change image_info() to width and height from image str.
 * 13/9/96: SJM: It did assume if no pixtrans then it didn't need scaling, however this isn't case for plotting
 *		 16bpp to 32bpp and vice versa.
 * 19/9/96: SJM: changed hard coded 68 for default images to size read from image
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel.h"
#include "memwatch.h"

#include "os.h"
#include "sprite.h"
#include "wimp.h"
#include "bbc.h"
#include "swis.h"
#include "resspr.h"
#include "flexwrap.h"
#include "visdelay.h"
#include "drawftypes.h"
#include "colourtran.h"
#include "alarm.h"

#include "images.h"
#include "access.h"

#include "status.h"

#include "util.h"
#include "makeerror.h"
#include "antweb.h"
#include "filetypes.h"
#include "config.h"
#include "rcolours.h"
#include "dfsupport.h"

#include "threads.h"
#include "version.h"
#include "unwind.h"

/* ------------------------------------------------------------------------- */

#ifndef NEW_WEBIMAGE
#define NEW_WEBIMAGE	1
#endif

#ifndef NEW_ANIMATION
#define NEW_ANIMATION	1
#endif

#ifndef MEMCHECK_THREADS
#define MEMCHECK_THREADS 1
#endif

#ifdef STBWEB_BUILD
# if NEW_WEBIMAGE
#  include "libs/webimage/webimage.h"
# else
#  include "libs/webim_old/webimage.h"
# endif
#else
# include "../webimage/webimage.h"
#endif

#if !NEW_WEBIMAGE
# include "sprt2sprt.h"
#endif

/* Set debug to 2 or more for loads of detail on the image translation */

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef FLEX
#define FLEX 1
#endif

#ifndef IMAGE_SCALE_TO_TAG
#define IMAGE_SCALE_TO_TAG 1
#endif

#define REAPER_POLL_TIME 500	/* Images arriving more than 5 seconds apart need to reload */

#define MAX_CACHE_BYTES		(32*1024)	/* target size for cached bg image */
#define ABS_MAX_CACHE_BYTES	(128*1024)	/* size beyond which we won't create cached bg image */
#define CACHE_SPRITE_PLOT_NAME	"cacheplot"
#define CACHE_SPRITE_BUILD_NAME	"cachebuild"

#define FLEX_FUDGE		4		/* get round sprite extend bug with reading over end of sprite areas */

/* ------------------------------------------------------------------------- */

/*

 * In an HTML document an 'image' can be any objects fetched by a URL.
 * The object is usually some sort of displayable item like a .gif or
 * a .xbm file but it could just as well be a sounds file or an MPEG
 * movie.  Since images may be used several times within a document
 * (for example, a bitmap used for several bullet points), and since
 * when they are in sprite format an image may be rather large, it is
 * important to reuse image data wherever possible.  To this end
 * images have a reference count and the same image can be returned to
 * several users.

 * In this system an image is requested through the images subsystem
 * and is refered to by its URL.  At this time the image system is
 * given a function pointer and a data pointer for update callbacks.
 * The data pointer is usually the antweb_data for the document that
 * displays the image. for the document that wants to display the
 * image.  An image handle is returned and the image is reqested using
 * the access calls.  At any time the image handle can be used to
 * render the image onto the screen and to find out how large the
 * image is.  If the image has not been fetched the size and
 * appearance are that of a default icon.  If the image has been
 * fetched but it can not be draw (either because we have no converter
 * or because it is not a picture) then the size and appearance of the
 * file type icon is given.  If the appearance changes (for example
 * after a transfer is completed) then the function/data pointer pairs
 * are called to cause documents to be updated.  If a the same
 * function/data pointer pair is registered with the image more than
 * once it is only called once.

 */

typedef struct image_callback_str {
    int use_count;
    image_callback cb;
    void *h;
    struct image_callback_str *next;
} image_callback_str;

#define IMAGE_MAGIC 0x78e6b2c9

#if NEW_WEBIMAGE
typedef struct {
    int ftype;
} webimage_str;
#else
typedef char * (*webimage_start)(getsrc_func getsrc,
				 putdst_func putdst, seekdst_func seekdst, imagerec_func imrec,
				 void *user_get, void *user_put, BOOL blockdetail);

typedef struct {
    int ftype;
    webimage_start start;
} webimage_str;
#endif

typedef enum
{
    pixtrans_UNKNOWN,		/* No table allocated, may need one, pixtrans is NULL */
    pixtrans_NONE_NEEDED,	/* No table is needed, pixtrans is NULL */
    pixtrans_NONE_SCALE,	/* No table allocated, pixtrans is NULL, OS will do bit packing/stretching */
    pixtrans_NARROW,		/* Table is a narrow <= 1 byte */
    pixtrans_WIDE		/* Table is wide > 1 byte */
} image_pixtrans_type;

typedef struct _image_info {
    int magic;
    struct _image_info *next, *prev;
    int use_count;
    char *url;
    char *ref;
    int data_so_far;
    int data_size;
    int file_load_addr;
    int file_exec_addr;
    unsigned int hash;
    char *cfile;
    int file_type;
    webimage_str *wi;
    image_flags flags;
    sprite_area **areap;
    sprite_area *our_area, *their_area, *cache_area;
    wimp_paletteword cache_bgcol;
#if NEW_ANIMATION
    int cache_mask;		/* if 0 then the cache sprite has no mask and there is no build sprite */
    int cache_frame;		/* last frame number built from build sprite into plot sprite */
    int cache_bgid;		/* a value uniquely identifying the last background rendered */
    int cache_x, cache_y;	/* phase of last bg plotting in cache */
#endif
    sprite_id id;
    char sname[13];		/* Used when we point to a file icon or the default icon */

    sprite_pixtrans *pt;		/* If non-NULL points to a pixel translation table */
    image_pixtrans_type table_type;	/* See defines above */

    int width, height;
    FILE *fh;			/* Used during reading the data */
    thread tt;
    int put_offset;
    image_callback_str *cblist;
    access_handle ah;
#if NEW_WEBIMAGE
    frame_rec *frame;
    int frames;
    int repeats;
    int cur_frame, cur_repeat;
    char errbuf[256];
#endif
} image_info_str;

#define wi_flag_MASK	0x01

/* ------------------------------------------------------------------------- */

static void image_fetch_next(void);

static image image_list, image_last;
static int being_fetched;

static int image_thread_data_size;
static char *image_thread_data_ptr;
static int image_thread_data_more;
static int image_thread_data_status;

static int image_white_byte(image im, wimp_paletteword colour);

static void image_issue_callbacks(image i, int changed, wimp_box *box);
static void image_animation_alarm(int at, void *h);
static void image_startup_animation(image i);

#if NEW_ANIMATION

#define image_frame_PLOT_IMAGE	(1<<0)
#define image_frame_PLOT_MASK	(1<<1)
#define image_frame_CLEAR_MASK	(1<<2)
#define image_frame_TO_PLOT_SPRITE	(1<<3)

static void image_animation_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy);
static void image_animation_render_frame(image i, int flags);

#endif

/* ------------------------------------------------------------------------- */

#if NEW_WEBIMAGE
static webimage_str translators[] = {
{ FILETYPE_SPRITE },
{ FILETYPE_GIF	},
{ FILETYPE_XBM	},
{ FILETYPE_JPEG	},
{ FILETYPE_PNG	},
{ FILETYPE_TIFF },
{ -1		}
};
#else
static webimage_str translators[] = {
{ FILETYPE_SPRITE,	&sprt2sprite	},
{ FILETYPE_GIF,		&gif2sprite	},
{ FILETYPE_XBM,		&xbm2sprite	},
{ FILETYPE_JPEG,	&jpg2sprite	},
#if 0
{ FILETYPE_PNG,		&png2sprite	},
#endif
{ -1, 			0		}
};
#endif

/* ------------------------------------------------------------------------- */

static void free_pt(image i)
{
    if (i->pt)
    {
	mm_free(i->pt);
	i->pt = NULL;
	i->table_type = pixtrans_UNKNOWN;
    }
}

/* ------------------------------------------------------------------------- */

/*
 * Image thread/webimage type routines
 */


/*
 * If we have no data at all get some.
 * If we have more than enough return it with flag false.
 * If we have some or exactly enough return it with flag true.
 */

#if NEW_WEBIMAGE
static int new_image_get_bytes(char *buf, int buf_len, void *h, BOOL *flush)
{
    int rc;

    IMGDBG(("get_bytes: in: Asked for %d bytes, have %d bytes\n", buf_len, image_thread_data_size));

    if (image_thread_data_size == 0)
    {
	thread_wait("Need more data");

	IMGDBG(("Now have %d bytes\n", image_thread_data_size));
    }
    
    if (image_thread_data_size == 0)
    {
	IMGDBG(("get_bytes: out: rc=-1\n"));
	return -1;
    }

    rc = (image_thread_data_size > buf_len) ? buf_len : image_thread_data_size;

    memcpy(buf, image_thread_data_ptr, rc);

    image_thread_data_ptr += rc;
    image_thread_data_size -= rc;

    *flush = (image_thread_data_size == 0);

    IMGDBG(("get_bytes: out: rc=%d\n", rc));

    return rc;
}
#else
static int new_image_get_bytes(char *buf, int buf_len, void *h)
{
    int rc, data_back;

    if (image_thread_data_size >= buf_len)
    {
	memcpy(buf, image_thread_data_ptr, buf_len);
	image_thread_data_ptr += buf_len;
	image_thread_data_size -= buf_len;

	return buf_len;
    }

    rc = image_thread_data_size;
    if (image_thread_data_size)
    {
	memcpy(buf, image_thread_data_ptr, image_thread_data_size);
	buf += image_thread_data_size;
	buf_len -= image_thread_data_size;
    }

    do
    {
	thread_wait("Need more data");

	data_back = image_thread_data_size;

	if (data_back)
	{
	    int to_copy;

	    to_copy = (buf_len > image_thread_data_size) ? image_thread_data_size : buf_len;

	    memcpy(buf, image_thread_data_ptr, to_copy);
	    buf += to_copy;
	    buf_len -= to_copy;
	    image_thread_data_ptr += to_copy;
	    image_thread_data_size -= to_copy;

	    rc += to_copy;
	}

    } while (data_back && buf_len);

    return rc ? rc : -1;
}
#endif

static void image_put_bytes(char *buf, int buf_len, void *h)
{
    image i = (image) h;

    IMGDBGN(("put_bytes: in: Putting 0x%x bytes at 0x%p to offset 0x%x\n", buf_len, buf, i->put_offset));

    if ( (i->put_offset + buf_len)  > i->our_area->size)
    {
	usrtrc( "Too much image data: %x + %x > %x\n",
		i->put_offset, buf_len, i->our_area->size);
    }
    else
    {
	flexmem_noshift();
	memcpy(((char*) i->our_area) + i->put_offset, buf, buf_len);

	i->put_offset += buf_len;

	flexmem_shift();
    }

    if ((i->flags & image_flag_REALTHING) == 0)
    {
	int fill;

	i->areap = &(i->our_area);
	i->id.tag = sprite_id_name;
	i->id.s.name = i->sname;
	flexmem_noshift();
	strncpy(i->sname, ((sprite_header *) (i->our_area + 1))->name, 12);
	
	flexmem_shift();

	/* Fill with zero if we have no palette yet, use white if we can */
	fill = ((i->flags & image_flag_MASK) ?
		0 :		/* With a mask fill with zero */
		((i->put_offset < (sizeof(sprite_area) + sizeof(sprite_header))) ?
		 0xff :		/* Without a mask fill with 0xff */
		 image_white_byte(i, i->cache_bgcol)));	/* unless we can do better */

	IMGDBG(("image fill: bg %08x fill %08x mask %d\n", i->cache_bgcol.word, fill, i->flags & image_flag_MASK ? 1 : 0));

        if (fill & 0xffffff00)
	{
            int *ip = (int *)(((char*) i->our_area) + ((i->put_offset + 3) &~ 3));
            int *end = (int *)(((char*) i->our_area) + i->our_area->size);
            while (ip < end)
                *ip++ = fill;
	}
	else
	{
	    IMGDBG(("image fill: memset %p %x %d\n", ((char*) i->our_area) + i->put_offset, fill, i->our_area->size - i->put_offset));
    	    flexmem_noshift();
	    /* I think this may be necessary */
	    if (i->our_area->size - i->put_offset > 0)
	    {
		IMGDBGN(("memset: from %p =%02x %d bytes\n", ((char*) i->our_area) + i->put_offset, fill, i->our_area->size - i->put_offset));
		memset(((char*) i->our_area) + i->put_offset, fill, i->our_area->size - i->put_offset);
	    }
	    flexmem_shift();
	}

	free_pt(i);

	i->flags |= image_flag_REALTHING/* | image_flag_RENDERABLE */;

	IMGDBG(("New sprite area at %p, sprite name %s, width = %d, height = %d\n",
		i->our_area, i->id.s.name, i->width, i->height));
    }

    /* only set renderable bit when palette has arrived */
    if ((i->flags & image_flag_RENDERABLE) == 0)
    {
        sprite_header *sph = (sprite_header *) ((char *)i->our_area + i->our_area->sproff);
        int pal_end = sph->mask > sph->image ? sph->mask : sph->image;

        if ((i->put_offset >= sizeof(sprite_area) + pal_end) &&
	    (i->flags & image_flag_USE_LOGICAL) == 0)
        {
            i->flags |= image_flag_RENDERABLE;
	    free_pt(i);
        }
    }

    i->flags |= image_flag_CHANGED;

    IMGDBGN(("put_bytes: out: flags %x\n", i->flags));
}

static void image_seek_fn(int pos, void *h)
{
    image i = (image) h;

    i->put_offset = 16 + pos;

    IMGDBGN(("Seeking to offset %d, put_offset=0x%x\n", pos, i->put_offset));
}

static BOOL image_rec_fn(image_rec *ir, void *h)
{
    image i = (image) h;
    int size;

    IMGDBGN(("Given image_rec %p\n", ir));

    size = ir->size;

    if (i->our_area)
    {
	int OK;

	OK = flex_extend((flex_ptr) &(i->our_area), size + 16 + FLEX_FUDGE);

	if (OK)			/* SJM: only fill in any of this if we got the memory */
	{
	    i->our_area->size = size+16;
	    i->our_area->freeoff = size + 16;

	    i->frames = i->our_area->number = ir->frames;

	    if (ir->frame)
	    {
		int size = i->frames * sizeof(frame_rec);

		if (i->frame)
		    i->frame = mm_realloc(i->frame, size);
		else
		    i->frame = mm_malloc(size);

		memcpy(i->frame, ir->frame, size);
	    }

	    i->repeats = ir->repeat;

	    if (i->frames > 1)
		i->flags |= image_flag_ANIMATION;
	}
	
	return OK;
    }

    i->width = ir->x_logical;	/* changed to logical sizes */
    i->height = ir->y_logical;

    if (ir->x != ir->x_logical || ir->y != ir->y_logical)
	i->flags |= image_flag_USE_LOGICAL;

    IMGDBG(("img: size %dx%d logical size %dx%d\n", ir->x, ir->y, ir->x_logical, ir->y_logical));

    if (ir->interlaced)
	i->flags |= image_flag_INTERLACED;

    if (ir->mask)
	i->flags |= image_flag_MASK;

#if FLEX
    if (flex_alloc((flex_ptr) &(i->our_area), size + 16 + FLEX_FUDGE) == FALSE)
    {
	usrtrc( "Failed to get memory for image\n");
	i->flags |= (image_flag_ERROR | image_flag_CHANGED);
	return FALSE;
    }
    else
    {
	IMGDBG(("Flex alloc for %d bytes gave ptr %p\n", size+16, i->our_area));
    }
#else
    i->our_area = mm_malloc(size + 16);

    if (i->our_area == NULL)
    {
	usrtrc( "Failed to get memory for image\n");
	return FALSE;
    }
    else
    {
	IMGDBG(("Malloc for %d bytes gave ptr %p\n", size+16, i->our_area));
    }
#endif

    memset(i->our_area, 0, sizeof(sprite_area) + sizeof(sprite_header));

    i->our_area->size = size+16;
    i->our_area->number = 1;
    i->our_area->sproff = 16;
    i->our_area->freeoff = size + 16;

    IMGDBGN(("Sprite area is %d bytes at %p\n", size+16, i->our_area));

    i->put_offset = 16;

    IMGDBGN(("rec_fn: out: have info; x=%d, y=%d\n", ir->x, ir->y));

    return TRUE;
}

static void image_set_error(image i)
{
    i->flags |= (image_flag_ERROR | image_flag_CHANGED);
    i->flags &= ~image_flag_RENDERABLE;

    i->their_area = resspr_area(); /* Wimp sprite area */
    i->areap = &(i->their_area);
    strcpy(i->sname, "sprerror");
    i->id.tag = sprite_id_name;
    i->id.s.name = i->sname;

    free_pt(i);
}

static int bastard_main(int argc, char **argv)
{
#if NEW_WEBIMAGE == 0
    webimage_start start;
#endif
    char *result;
    void *i;
    int flags;

    IMGDBG(("Bastard_main called %d, %p\n", argc, argv));

    i = (void *) argv;

    flags = (config_deep_images ? webimage_DEEPSPRITE : 0) +
	((((image)i)->flags & image_flag_NO_BLOCKS) ? 0 : webimage_BLOCKDETAIL);

#ifdef STBWEB
    /* Disable deep-spriting if not in highcolor mode */
    if (bbc_vduvar(bbc_Log2BPP) <= 3)
	flags &= ~webimage_DEEPSPRITE;
#endif

#if NEW_WEBIMAGE
    result = img2sprite(&new_image_get_bytes,	/* Get bytes from source */
			&image_put_bytes,	/* Put bytes to dest */
			&image_seek_fn,	/* Seek to where to put bytes */
			&image_rec_fn,	/* Tell us about the image */
			i, i,		/* Handles for get and put */
			flags,
			((image)i)->errbuf);	/* Flags */

#else
    start = (webimage_start) ((int*)argc);
    result = start(&new_image_get_bytes,	/* Get bytes from source */
		   &image_put_bytes,	/* Put bytes to dest */
		   &image_seek_fn,	/* Seek to where to put bytes */
		   &image_rec_fn,	/* Tell us about the image */
		   i, i,		/* Handles for get and put */
		   flags);		/* Flags */
#endif

    IMGDBG(("Bastard_main done r=%d\n", (int)result));

    return (int) (long) result;
}

static int image_thread_start(image i)
{
    IMGDBG(("About to start thread\n"));

    image_thread_data_size = 0;
    image_thread_data_ptr = 0;
    image_thread_data_more = 0;
    image_thread_data_status = 0;

    MemCheck_SetChecking(1,1);
    
#if NEW_WEBIMAGE
    i->tt = thread_start(&bastard_main, 0, (char**) i, 4096);
#else
    i->tt = thread_start(&bastard_main, (int) i->wi->start, (char**) i, 4096);
#endif

    IMGDBG(("New thread 0x%p\n", i->tt));

    return (i->tt != 0);
}

static int image_thread_process(image i, int fh, int from, int to)
{
    os_gbpbstr gpb;
    char buffer[4096];
    int len;

    IMGDBGN(("image_thread_process: in: i %p fh %d from %d to %d\n", i, fh, from, to));

    while (from < to && i->tt->status == thread_ALIVE)
    {
	IMGDBG(("Loading some data\n"));

	len = (to-from) > sizeof(buffer) ? sizeof(buffer) : (to-from);

	gpb.action = 3;
	gpb.file_handle = fh;
	gpb.data_addr = buffer;
	gpb.number = len;
	gpb.seq_point = from;

	IMGDBGN(("Reading %d bytes from file %d.\n", len, fh));

	if (os_gbpb(&gpb) == NULL)
	{
	    IMGDBGN(("Sending %d bytes to the image thread.\n", len));

	    image_thread_data_size = len;
	    image_thread_data_ptr = buffer;
	    image_thread_data_more = ((from + len) < to);

	    IMGDBG(("Running thread again\n"));

	    MemCheck_RegisterMiscBlock(buffer, sizeof(buffer));

	    thread_run(i->tt);

	    MemCheck_UnRegisterMiscBlock(buffer);
	}

	from += len;
    }

    IMGDBGN(("image_thread_process: out: status %d\n", i->tt->status));

    return (i->tt->status == thread_ALIVE);
}

static char *image_thread_end(image i)
{
    char *res;
#if !MEMCHECK_THREADS
    MemCheck_checking checking = MemCheck_SetChecking(0, 0);
#endif

    IMGDBGN(("image_thread_end: in: i=%p\n", i));

#if 1
    /* thread doesn't seem to die in two calls when called from image_flush or the like */
    _kernel_osbyte(0xE5, 0, 0);
    _kernel_escape_seen();

    while (i->tt->status == thread_ALIVE && !_kernel_escape_seen())
    {
	image_thread_data_size = 0;
	image_thread_data_ptr = 0;
	image_thread_data_more = 0;
	thread_run(i->tt);
    }    

    _kernel_osbyte(0xE5, 0xFF, 0);
    _kernel_osbyte(0x7C, 0xFF, 0);

#else
    /* Call once to flush out any data in buffers */
    if (i->tt->status == thread_ALIVE)
    {
	image_thread_data_size = 0;
	image_thread_data_ptr = 0;
	image_thread_data_more = 0;
	thread_run(i->tt);
    }

    /* Call again to make sure we have stopped */
    if (i->tt->status == thread_ALIVE)
    {
	image_thread_data_size = 0;
	image_thread_data_ptr = 0;
	image_thread_data_more = 0;
	thread_run(i->tt);
    }
#endif
    if (i->tt->status == thread_ALIVE)
	res = "Thread did not finnish";
    else
	res = (char *) (long) i->tt->rc;

    /* Clear up the thread */

#if 0
    fprintf(stderr, "About to destroy thread 0x%p\n", i->tt);
#endif
    thread_destroy(i->tt);
 
#if !MEMCHECK_THREADS
    MemCheck_RestoreChecking(checking);
#endif
    i->tt = NULL;

    IMGDBGN(("image_thread_end: out: res='%s'\n", strsafe(res)));

    return res;
}


static char *image_process(image i, char *cfile)
{
    int fh;
    os_regset r;
    os_error *ep;
    char *res;

    r.r[0] = 0x4f;
    r.r[1] = (int) (long) cfile;

    ep = os_find(&r);
    if (ep)
	return ep->errmess;


    fh = r.r[0];

    i->flags |= image_flag_NO_BLOCKS;

    if (image_thread_start(i))
    {
	int size;

	r.r[0] = 2;
	r.r[1] = fh;
	r.r[2] = 0;

	os_args(&r);

	size = r.r[2];

	image_thread_process(i, fh, 0, (size>>1) );
	image_thread_process(i, fh, (size>>1), size );
#if 0
	fprintf(stderr, "Calling image_thread_end() from image_process()\n");
#endif
	res = image_thread_end(i);
    }
    else
    {
	res = "Could not make thread";
    }

    r.r[0] = 0;
    r.r[1] = fh;

    os_find(&r);

    return res;
}

static char *image_process_to_end(image i, char *cfile)
{
    int fh;
    os_regset r;
    os_error *ep;
    char *res;
    int size;

    r.r[0] = 0x4f;
    r.r[1] = (int) (long) cfile;

    ep = os_find(&r);
    if (ep)
	return ep->errmess;

    fh = r.r[0];

    r.r[0] = 2;
    r.r[1] = fh;
    r.r[2] = 0;

    os_args(&r);

    size = r.r[2];

    image_thread_process(i, fh, i->data_so_far, size );
#if 0
    fprintf(stderr, "Calling image_thread_end() from image_process_to_end()\n");
#endif
    res = image_thread_end(i);

    r.r[0] = 0;
    r.r[1] = fh;

    os_find(&r);

    return res;
}

static void image_issue_callbacks(image i, int changed, wimp_box *box)
{
    if (i->cblist)
    {
	image_callback_str *cb = i->cblist;

	while (cb)
	{

	    IMGDBG(("Callback, handle=0x%p, uses=%d, next=0x%p, changed=%d\n",
		    cb->h, cb->use_count, cb->next, changed));

	    if (cb->cb && (cb->use_count > 0))
		cb->cb(cb->h, (void*) i, changed, box);
	    cb = cb->next;
	}
    }
}

#if NEW_WEBIMAGE == 0
static webimage_str *image_lookup_wi(int ft)
{
    webimage_str *wi;

    for (wi = translators; wi->ftype != -1; wi++)
    {
	if (wi->ftype == ft)
	    break;
    }

    return (wi->ftype == -1) ? NULL : wi;
}
#endif

static void image_progress(void *h, int status, int size, int so_far, int fh, int ftype, char *url)
{
    image i = (image) h;
    BOOL more_data;
    int rd;

    rd = i->flags & image_flag_RENDERABLE;

    IMGDBG(("Image progress in...\n"));

    if (so_far == -1)
	so_far = 0;

    i->flags &= ~(image_flag_CHANGED);

    more_data = (i->data_so_far != so_far);
    if (more_data)
    {
	if (status == status_GETTING_BODY)
	{
#if !MEMCHECK_THREADS
	    MemCheck_checking checking = MemCheck_SetChecking(0, 0);
#endif
#if NEW_WEBIMAGE == 0
	    if (i->wi == NULL)
	    {
		i->wi = image_lookup_wi(ftype);
	    }
#endif
	    
#if NEW_WEBIMAGE
	    if (i->tt == NULL)
#else
	    if (i->tt == NULL && i->wi)
#endif
	    {
		i->flags &= ~image_flag_NO_BLOCKS;
		i->file_type = ftype;
		image_thread_start(i);
	    }

	    IMGDBG(("Data arriving; file=%d, last had %d, now got %d\n",
		    fh, i->data_so_far, so_far));

	    if (i->tt)
	    {
		image_thread_process(i, fh, i->data_so_far, so_far);
	    }

#if !MEMCHECK_THREADS
	    MemCheck_RestoreChecking(checking);
#endif
	}

	i->data_so_far = so_far;
	i->data_size = size;
    }

    /* SJM: always call callbacks so that world turns */
    if (i->cblist)
    {
	int changed;

	rd ^= i->flags & image_flag_RENDERABLE;

	/* check and see if image size has changed (due to animation downloading)
	if (rd == 0 && (i->width != w || i->height != h))
	    rd = 1;
	 */

	changed = ((i->flags & image_flag_CHANGED) ?
		   (rd ? image_cb_status_REFORMAT : image_cb_status_UPDATE) :
		   image_cb_status_NOACTION);

	image_issue_callbacks(i, changed, NULL);
    }

    IMGDBG(("...image progress out.\n"));
}

static access_complete_flags image_completed(void *h, int status, char *cfile, char *url)
{
    int ft;
    image i = (image) h;
    char *err;
    int rd;

    rd = i->flags & image_flag_RENDERABLE;

    being_fetched--;		/* I guess we should do this even if the handle is broken */

    IMGDBG(("Decremented fetching count, now %d\n", being_fetched));

    image_fetch_next();

    if (i->magic != IMAGE_MAGIC)
	return 0;

    i->ah = NULL;

    if (status == status_COMPLETED_FILE)
    {
	os_filestr ofs;

	i->cfile = strdup(cfile);
	i->flags |= image_flag_FETCHED;

	i->file_type = ft = file_type(cfile);

	ofs.action = 5;
	ofs.name = i->cfile;
	os_file(&ofs);

	i->file_load_addr = ofs.loadaddr;
	i->file_exec_addr = ofs.execaddr;
	i->data_size = ofs.start;

	IMGDBG(("Got the image file '%s', type 0x%03x\n", cfile, ft));

#if NEW_WEBIMAGE == 0
	if (i->wi == NULL)
	{
	    i->wi = image_lookup_wi(ft);
	}

	if (i->wi)
#endif
	{
#if !MEMCHECK_THREADS
	    MemCheck_checking checking = MemCheck_SetChecking(0, 0);
#endif

	    visdelay_begin();

	    if (i->tt)
	    {
		err = image_process_to_end(i, cfile);
	    }
	    else
	    {
		err = image_process(i, cfile);
	    }

#if !MEMCHECK_THREADS
	    MemCheck_RestoreChecking(checking);
#endif
	    visdelay_end();

	    if (err == NULL)
	    {
		i->flags |= image_flag_RENDERABLE;

		/* If we'd used the logical size and it wasn't an animation then go back to the real size */
		/* Don't know if this will work from the cache or not */
		if ((i->flags & image_flag_USE_LOGICAL) && i->frame == NULL)
		{
		    sprite_info info;
		    sprite_readsize(i->our_area, &i->id, &info);

		    i->width = info.width;
		    i->height = info.height;
		}
	    }
	    else
	    {
		usrtrc( "Image error 1 = %s (%s)\n", err, cfile);

		if ((i->flags & image_flag_RENDERABLE) == 0)
		{
		    if (i->our_area)
		    {
			IMGDBG(("Freeing area from %p\n", i->our_area));

#if FLEX
			flex_free((flex_ptr) &i->our_area);
#else
			mm_free(i->our_area);
#endif
			i->our_area = NULL;
			/* No alarms exist at this stage */
		    }
		    image_set_error(i);
		}
	    }
	}
    }

    if ((i->flags & image_flag_FETCHED) == 0)
    {
	image_set_error(i);

	usrtrc( "Image access completed with error status: %d\n", status);
    }

    if (i->flags & image_flag_WAITING) /* Waitiong flag is still set if we came from the cache */
    {
	i->flags &= ~(image_flag_WAITING | image_flag_DEFERRED);

	/* SJM: Added callback so frontend knows image is loaded */
	if (i->cblist)
	{
	    image_issue_callbacks(i, image_cb_status_CACHE, NULL);
	}
    }
    else
    {
	rd ^= i->flags & image_flag_RENDERABLE;

	if (i->cblist)
	{
	    image_issue_callbacks(i, (rd ? image_cb_status_REFORMAT : image_cb_status_UPDATE), NULL);
	}
    }

    /* @@@@ Fire up the animation process here */
    image_startup_animation(i);

    IMGDBG(("Returning from image completed function\n"));

    return (access_CACHE | access_KEEP); /* Cache the file and try to hold on to it */
}

/* ---------------------------------------------------------- */

#ifdef IMAGE_COOLING

/* This was experimental and isn't used */

static void *cooler_table = NULL;
static BOOL use_cooling = TRUE;

void image_get_cooler_table(void)
{
    FILE *f;
    f = fopen("<NCFresco$Dir>.Tab<TV$Type>", "rb");
    if (f)
    {
        if (flex_alloc(&cooler_table, 64*1024))
            fread(cooler_table, 64*1024, 1, f);
        else
            usrtrc( "Failed to load cooler file\n");

        fclose(f);
    }
    else
	usrtrc( "Failed to open cooler file\n");
}

void image_toggle_cooling(void)
{
    use_cooling = !use_cooling;
}

static void image_cool_table(void *pt, int log2bpp)
{
    if (cooler_table && use_cooling)
    {
        int i, n_cols = 1 << (1 << log2bpp);

        short *tp = cooler_table;
        short *pp = pt;

	usrtrc( "img: Fixed up table size %d\n", n_cols);

        for (i = 0; i < n_cols; i++, pp++)
        {
            *pp = tp[*pp];
        }
    }
}

#endif

/* ---------------------------------------------------------- */

int spriteextend_version;

static void image_poll_reaper(int at, void *h)
{
#if 0
    imgreaper();
#endif
    alarm_set(alarm_timenow()+REAPER_POLL_TIME, image_poll_reaper, h);
}

os_error *image_init(void)
{
    IMGDBG(("Starting image code with debug enabled\n"));

    image_list = image_last = NULL;
    being_fetched = 0;

    os_cli("Set "PROGRAM_NAME"$Temp 99");
    os_cli("RMEnsure SpriteExtend 0.99 Set "PROGRAM_NAME"$Temp 61");
    spriteextend_version = atoi(getenv(PROGRAM_NAME"$Temp"));

    IMGDBG(("Sprite extend version %d\n", spriteextend_version));

#ifndef STBWEB
    image_poll_reaper(0, (void *) translators);	/* Use translators' address as a safe handle */
#endif
    
    return NULL;
}

os_error *image_tidyup(void)
{
    image i, ii;

    IMGDBG(("Image tidyup called\n"));

    for (i=image_list; i != NULL; i = ii)
    {
	IMGDBG(("Disposing image at 0x%p, url = '%s'\n", i, i->url ? i->url : "<none>"));

	if (i->ah)
	{
	    IMGDBGN(("Image tidyup Aborting access\n"));

	    access_abort(i->ah);
	    being_fetched--;
	}

	if (i->url)
	{
	    IMGDBGN(("Dealing with URL\n"));

	    access_unkeep(i->url);
	    mm_free(i->url);
	}

	if (i->ref)
	{
	    IMGDBGN(("Dealing with ref\n"));

	    access_unkeep(i->ref);
	    mm_free(i->ref);
	}

	if (i->tt)
	{
	    IMGDBGN(("Ending thread\n"));

	    usrtrc( "Calling image_thread_end() from image_tidyup()\n");

	    image_thread_end(i);
	}

	free_pt(i);

	if (i->frame)
	{
	    mm_free(i->frame);
	}

	alarm_removeall(i);

	ii = i->next;
	mm_free(i);
    }

    /* Remove the reaper function */
    alarm_removeall((void*) translators);

    IMGDBG(("Tidied up images\n"));

    return NULL;
}

void image_palette_change(void)
{
    image i;

    for (i=image_list; i != NULL; i = i->next)
    {
	free_pt(i);

        if (i->cache_area)
        {
#if FLEX
            flex_free((flex_ptr)&i->cache_area);
#else
            mm_free(i->cache_area);
#endif
            i->cache_area = NULL;

	    /* signal that the cahe has been dumped */
	    if (i->frames > 1)
		i->cur_frame = -1;
	}
    }
}

static void image_fetch_next(void)
{
    image i;

    IMGDBG(("Scaning image list\n"));

    for (i=image_list; (being_fetched < config_max_files_fetching) && (i != NULL); i = i->next)
    {
	/* Pick ones that are waiting and not deferred */
	if ((i->flags & (image_flag_WAITING | image_flag_DEFERRED)) == image_flag_WAITING)
	{
	    os_error *ep;
	    int reload;

	    reload = (i->flags & image_flag_TO_RELOAD) ? access_NOCACHE : 0;

	    i->flags &= ~(image_flag_WAITING | image_flag_TO_RELOAD);

	    being_fetched++;	/* In case it comes from the cache we increment this here */

	    IMGDBG(("Incremented fetching count, now %d\n", being_fetched));

	    ep = access_url(i->url, reload | access_CHECK_EXPIRE, 0, 0, i->ref,
			    &image_progress, &image_completed, i, &(i->ah));

	    if (ep)
	    {
		i->ah = NULL;
		image_set_error(i);
		usrtrc( "Error accessing image: %s\n", ep->errmess);
		being_fetched--; /* If we failed, decrement it again */
	    }
	}
    }
}

extern BOOL image_type_test(int ft)
{
    int i;

    for(i=0; translators[i].ftype != -1; i++)
    {
	if (translators[i].ftype == ft)
	    return TRUE;
    }

    return FALSE;
}

static os_error *find_area_and_info(image i, sprite_info *info)
{
    os_error *e;
    sprite_area *area = *i->areap;
    e = sprite_readsize(area, &i->id, info);
    if (e)
    {
        os_regset r;
        os_swix(Wimp_BaseOfSprites, &r);
        area = (sprite_area *) (long) r.r[1];
        e = sprite_readsize(area, &i->id, info);
        if (e)
        {
            area = (sprite_area *) (long) r.r[0];
            e = sprite_readsize(area, &i->id, info);
            if (!e)
                i->their_area = area;
        }
        else
            i->their_area = area;
    }
    return e;
}

static os_error *image_find_icontype(image i)
{
    char *path = strchr(i->url, ':') + 1;
    sprite_info info;
    os_error *e;

    i->sname[0] = 0;
    if (path[0] == ',')
    {
	sprintf(i->sname, "small_%.6s", path+1);
    }
    else if (path[0] == '.')
    {
	int ft = suffix_to_file_type(path+1);

	sprintf(i->sname, "small_%03x", ft == -1 ? 0xfff : ft);
    }
    else if (strcasecomp(path, "directory") == 0)
    {
	strcpy(i->sname, "small_dir");
    }
#ifdef STBWEB
    else if (strncasecomp(path, "nc_?", sizeof("nc_?")-1) == 0)
    {
       	strncpysafe(i->sname, path, sizeof(i->sname));
	i->sname[3] = bbc_modevar(-1, 0) & 0x100 ? 'n' : 'v';
    }
#endif

    if (i->sname[0])
    {
        /* look for file type icon */
        e = find_area_and_info(i, &info);
        if (e)
        {
            /* default to unknown file type */
            strcpy(i->sname, "small_xxx");
            e = find_area_and_info(i, &info);
        }
    }
    else
    {
        /* look for other icon */
	strncpysafe(i->sname, path, sizeof(i->sname));
	strlencat(i->sname, "icon", sizeof(i->sname));

        e = find_area_and_info(i, &info);
        if (e)
        {
	    strncpysafe(i->sname, path, sizeof(i->sname));
            e = find_area_and_info(i, &info);
        }
    }

    if (e)
    {
        IMGDBG(("find icontype: e '%s'\n", e->errmess));

        image_set_error(i);
        return e;
    }

    i->flags = image_flag_FETCHED | image_flag_RENDERABLE | image_flag_REALTHING;
    i->file_type = 0xff9;

    i->width = info.width;
    i->height = info.height;

    if (info.mask)
	i->flags |= image_flag_MASK;

    /* do the callbacks */
    if (i->cblist)
    {
	image_issue_callbacks(i, image_cb_status_CACHE, NULL);	/* SJM: changed from REFORMAT */
    }

    return NULL;
}

os_error *image_find(char *url, char *ref, int flags, image_callback cb, void *h, wimp_paletteword bgcol, image *result)
{
    image i;
    unsigned int hash;
    image_callback_str *cbs;
    os_error *ep;

    IMGDBG(("Asked to find image '%s'\n", url));

    hash = string_hash(url);

    for(i = image_list; i != NULL; i = i->next)
    {
	if (i->hash == hash && strcmp(i->url, url) == 0)
	    break;
    }

    if (i)
    {
	i->use_count++;
	if ((flags & image_find_flag_DEFER) == 0 &&
	    (i->flags & image_flag_DEFERRED) != 0 )
	{
	    i->flags &= ~image_flag_DEFERRED;
	}
    }
    else
    {
	sprite_info info;

	IMGDBG(("Making new image\n"));

	i = mm_calloc(1, sizeof(*i));

	if (image_list)
	{
	    i->prev = image_last;
	    image_last->next = i;
	    image_last = i;
	}
	else
	{
	    image_list = image_last = i;
	}

	i->magic = IMAGE_MAGIC;
	i->use_count = 1;
	i->url = strdup(url);
	i->hash = hash;
	i->flags = image_flag_WAITING;
	if (flags & image_find_flag_DEFER)
	    i->flags |= image_flag_DEFERRED;
	i->their_area = resspr_area(); /* Wimp sprite area */
	i->areap = &(i->their_area);
	strcpy(i->sname, (flags & image_find_flag_DEFER) ? "deferred" : "unknown");
	i->id.tag = sprite_id_name;
	i->id.s.name = i->sname;

	if (sprite_readsize(i->their_area, &i->id, &info) == NULL)
	{
	    i->width = info.width;
	    i->height = info.height;
	}
	else
	    i->width = i->height = 34;
/* 	i->width = 68; */
/* 	i->height = 68; */
	i->cache_bgcol = bgcol;
    }

    /* Use the first ref we see */
    if (ref && i->ref == NULL)
	i->ref = strdup(ref);

    *result = i;

    if (cb)
    {
	cbs = i->cblist;

	while (cbs && ((cb != cbs->cb) || (h != cbs->h)) )
	{
	    cbs = cbs->next;
	}

	if (cbs)
	{
	    cbs->use_count++;
	}
	else
	{
	    image_callback_str *new_cb;

	    new_cb = mm_calloc(1, sizeof(*new_cb));
	    if (new_cb)
	    {
		new_cb->use_count = 1;
		new_cb->cb = cb;
		new_cb->h = h;

		new_cb->next = i->cblist;
		i->cblist = new_cb;
	    }
	}
    }

    ep = NULL;
    if (strncmp(url, "icontype:", sizeof("icontype:")-1) == 0)
    {
        ep = image_find_icontype(i);
    }
    else if ((i->cfile == NULL) &&
	(i->ah == NULL) &&
	((i->flags & (image_flag_ERROR | image_flag_STREAMING )) == 0) )
    {
	int present = access_test_cache(url);

	IMGDBG(("image_find: %s present %d flags %x\n", url, present, flags));

	if (present == access_test_PRESENT || ((flags & image_find_flag_CHECK_EXPIRE) == 0 && present == access_test_EXPIRED))
	{
	    being_fetched++;

	    /* If the file is already around then we don't care if it was deferred, do we? */
	    i->flags &= ~(image_flag_WAITING | image_flag_DEFERRED);

	    ep = access_url(url, 0, 0, 0, i->ref, &image_progress, &image_completed, i, &(i->ah));
	    if (ep)
	    {
		i->ah = NULL;
		image_set_error(i);
		usrtrc( "Error accessing image: %s\n", ep->errmess);

		being_fetched--;
	    }
	}
	else
	{
	    IMGDBG(("Calling image_fetch_next()\n"));

	    image_fetch_next();
	}
    }

    return ep;
}

/* Image_stream does NOT increment the use count.  This is done when the image is sized */
os_error *image_stream(char *url, int ft, int *already, image *result)
{
    image i;
    unsigned int hash;

    IMGDBG(("Making stream for image '%s'\n", url));

    hash = string_hash(url);

    for(i = image_list; i != NULL; i = i->next)
    {
	if (i->hash == hash && strcmp(i->url, url) == 0)
	    break;
    }

    if (!i)
    {
	sprite_info info;
	IMGDBG(("Making new image\n"));

	i = mm_calloc(1, sizeof(*i));

	if (image_list)
	{
	    i->prev = image_last;
	    image_last->next = i;
	    image_last = i;
	}
	else
	{
	    image_list = image_last = i;
	}

	/* i->flags does NOT get a waiting flag */
	i->magic = IMAGE_MAGIC;
	i->use_count = 0;
	i->url = strdup(url);
	i->hash = hash;
	i->their_area = resspr_area(); /* Wimp sprite area */
	i->areap = &(i->their_area);
	strcpy(i->sname, "unknown");
	i->id.tag = sprite_id_name;
	i->id.s.name = i->sname;
	if (sprite_readsize(i->their_area, &i->id, &info) == NULL)
	{
	    i->width = info.width;
	    i->height = info.height;
	}
	else
	    i->width = i->height = 34;
/* 	i->width = 68; */
/* 	i->height = 68; */
    }

    *result = i;

    if ((i->cfile == NULL) &&
	(i->ah == NULL) &&
	((i->flags & (image_flag_ERROR | image_flag_STREAMING)) == 0) )
    {
	*already = FALSE;
	i->flags |= image_flag_STREAMING;
	i->file_type = ft;
#if NEW_WEBIMAGE == 0
	i->wi = image_lookup_wi(ft);
#endif
	i->flags &= ~image_flag_NO_BLOCKS;
	image_thread_start(i);
    }
    else
    {
	*already = TRUE;
    }

    return NULL;
}

os_error *image_stream_data(image i, char *buffer, int len, int update)
{
    int rd;

    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    rd = i->flags & image_flag_RENDERABLE;

    i->flags &= ~(image_flag_CHANGED);

    if (i->tt && i->tt->status == thread_ALIVE)
    {
	image_thread_data_size = len;
	image_thread_data_ptr = buffer;
	image_thread_data_more = !update;

	IMGDBG(("Running thread from stream data\n"));

	thread_run(i->tt);
    }

    i->data_so_far += len;
    i->data_size = -1;

    if (update && i->cblist)
    {
	int changed;

	rd ^= i->flags & image_flag_RENDERABLE;

	changed = ((i->flags & image_flag_CHANGED) ?
		   (rd ? image_cb_status_REFORMAT : image_cb_status_UPDATE) :
		   image_cb_status_NOACTION);

	image_issue_callbacks(i, changed, NULL);
    }

    return NULL;
}

os_error *image_stream_end(image i, char *cfile)
{
    char *res;
    os_filestr ofs;
    int rd;

    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    rd = i->flags & image_flag_RENDERABLE;

    if (i->tt)
    {
#if 0
	fprintf(stderr, "Calling image_thread_end() from image_stream_end()\n");
#endif

	res = image_thread_end(i);
    }
    else
    {
	res = "No thread to end";
    }

    i->flags |= image_flag_FETCHED;
    i->flags &= ~image_flag_STREAMING;

    if (cfile)
    {
	i->cfile = strdup(cfile);
	ofs.action = 5;
	ofs.name = i->cfile;
	os_file(&ofs);

	i->file_load_addr = ofs.loadaddr;
	i->file_exec_addr = ofs.execaddr;
	i->data_size = ofs.start;
    }

    if (res == NULL)
	i->flags |= image_flag_RENDERABLE;
    else
    {
	usrtrc( "Image error 2 = %s\n", res);

	if (i->our_area)
	{
	    IMGDBG(("Freeing area from %p\n", i->our_area));

#if FLEX
	    flex_free((flex_ptr) &i->our_area);
#else
	    mm_free(i->our_area);
#endif
	    i->our_area = NULL;
	    /* Too early to have any alarms set yet */
	}
	image_set_error(i);
    }

    if (cfile && i->cblist)
    {
	rd ^= i->flags & image_flag_RENDERABLE;

	image_issue_callbacks(i, (rd ? image_cb_status_REFORMAT : image_cb_status_UPDATE), NULL);
    }

    /* @@@@ Fire up the animation process here */
    image_startup_animation(i);

    return NULL;
}

os_error *image_loose(image i, image_callback cb, void *h)
{
    image_callback_str *cbs, *cbs2, **link;

    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    IMGDBG(("Loose image called: '%s', use count %d\n", i->url ? i->url : "", i->use_count));

    link = &(i->cblist);
    cbs = i->cblist;
    while (cbs && ((cb != cbs->cb) || (h != cbs->h)) )
    {
	link = &(cbs->next);
	cbs = cbs->next;
    }

    if (cbs)
    {
	cbs->use_count--;
	if (cbs->use_count == 0)
	{
	    *link = cbs->next;
	    mm_free(cbs);
	}
    }

    i->use_count--;

    if (i->use_count == 0)
    {
	IMGDBG(("Use count now zero\n"));

	cbs = i->cblist;
	while (cbs)
	{
	    cbs2 = cbs->next;
	    mm_free(cbs);
	    cbs = cbs2;
	}

	if (i->tt)
	{
#if 0
	    fprintf(stderr, "Calling image_thread_end() from image_loose()\n");
#endif
	    image_thread_end(i);
	}

	if (i->ref)
	    mm_free(i->ref);

	if (i->url)
	{
	    if (i->ah)
	    {
		IMGDBG(("Loose image Aborting access\n"));

		access_abort(i->ah);
		being_fetched--;
	    }
	    else
	    {
		IMGDBG(("Unkeeping the file\n"));

		access_unkeep(i->url);
	    }

	    IMGDBG(("Freeing URL\n"));

	    mm_free(i->url);
	}

	if (i->our_area)
	{
#if FLEX
	    flex_free((flex_ptr) &i->our_area);
#else
	    mm_free(i->our_area);
#endif

	    IMGDBG(("Freed area from %p\n", i->our_area));
	}

	/* Get rid of any animation alarms */
	alarm_removeall(i);

	if (i->cache_area)
	{
#if FLEX
	    flex_free((flex_ptr) &i->cache_area);
#else
	    mm_free(i->cache_area);
#endif

	    IMGDBG(("cache Freed area from %p\n", i->cache_area));
	}

	if (i->cfile)
	{
	    IMGDBG(("Freeing file name\n"));

	    mm_free(i->cfile);
	}

	IMGDBG(("Unlinking from linked list\n"));

	if (i->next)
	    i->next->prev = i->prev;
	else
	    image_last = i->prev;

	if (i->prev)
	    i->prev->next = i->next;
	else
	    image_list = i->next;

	free_pt(i);

	if (i->frame)
	    mm_free(i->frame);

	i->magic = 0;

	IMGDBG(("Freeing image object\n"));

	mm_free(i);
    }

    IMGDBG(("Loose image done\n"));

    return NULL;
}

int image_memory_panic(void)
{
    image i;
    int freed = FALSE;

    usrtrc( "Image memory panic called\n");

    for (i=image_list; i != NULL; i = i->next)
    {
	/* If we already have the image then dispose of it */
	if (i->our_area)
	{
	    freed = TRUE;

	    IMGDBG(("Need to dispose of the old image\n"));

#if FLEX
	    flex_free((flex_ptr) &i->our_area);
#else
	    mm_free(i->our_area);
#endif

	    IMGDBG(("Freed area from %p\n", i->our_area));

	    i->our_area = NULL;

            if (i->cache_area)
            {
#if FLEX
	        flex_free((flex_ptr) &i->cache_area);
#else
	        mm_free(i->cache_area);
#endif

	        IMGDBG(("Freed cache area from %p\n", i->cache_area));

	        i->cache_area = NULL;
	    }

	    if (i->cfile)
	    {
		IMGDBG(("Freeing file name\n"));

		mm_free(i->cfile);
		i->cfile = NULL;
	    }

	    free_pt(i);

	    if (i->frame)
	    {
		mm_free(i->frame);
		i->frame = NULL;
	    }

	    /* No point in animating a question mark !!!*/
	    alarm_removeall(i);

	    i->data_so_far = i->data_size = 0;
	    i->file_type = 0;
	    i->flags = image_flag_WAITING | image_flag_DEFERRED | image_flag_TO_RELOAD;
	    i->their_area = resspr_area(); /* Wimp sprite area */
	    i->areap = &(i->their_area);
	    strcpy(i->sname, "deferred");
	    i->id.tag = sprite_id_name;
	    i->id.s.name = i->sname;
	}
    }

    return freed;
}

os_error *image_flush(image i, int flags)
{
    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    if (i->tt)
    {
#if 0
	fprintf(stderr, "Calling image_thread_end() from image_flush()\n");
#endif
	image_thread_end(i);
    }

    if (i->url)
    {
	if (i->ah)
	{
	    IMGDBG(("Flush image Aborting access\n"));

	    access_abort(i->ah);
	    being_fetched--;
	    i->ah = NULL;
	}
	else
	{
	    IMGDBG(("Unkeeping the file\n"));

	    access_unkeep(i->url);
	}

	access_remove(i->url);
    }

    if (i->flags & image_flag_ERROR)
    {
	i->flags &= ~image_flag_ERROR;
	i->flags |= image_flag_TO_RELOAD | image_flag_WAITING;
    }

    /* If we already have the image then dispose of it */
    if (i->our_area)
    {
	IMGDBG(("Need to dispose of the old image\n"));

#if FLEX
	flex_free((flex_ptr) &i->our_area);
#else
	mm_free(i->our_area);
#endif

	IMGDBG(("Freed area from %p\n", i->our_area));

	i->our_area = NULL;

	if (i->frame)
	{
	    mm_free(i->frame);
	    i->frame = NULL;
	}

	/* Remove any animations */
	alarm_removeall(i);

	if (i->cache_area)
	{
#if FLEX
	    flex_free((flex_ptr) &i->cache_area);
#else
	    mm_free(i->cache_area);
#endif

	    IMGDBG(("Freed cache area from %p\n", i->cache_area));

	    i->cache_area = NULL;
	}

	if (i->cfile)
	{
	    IMGDBG(("Freeing file name\n"));

	    mm_free(i->cfile);
	    i->cfile = NULL;
	}

	free_pt(i);

	i->data_so_far = i->data_size = 0;
	i->file_type = 0;
	i->flags = image_flag_WAITING;
	i->flags |= image_flag_TO_RELOAD;
	i->their_area = resspr_area(); /* Wimp sprite area */
	i->areap = &(i->their_area);
	strcpy(i->sname, (flags & image_find_flag_DEFER) ? "deferred" : "unknown");
	i->id.tag = sprite_id_name;
	i->id.s.name = i->sname;

	if (i->cblist)
	{
	    image_issue_callbacks(i, image_cb_status_REFORMAT, NULL);
	}
    }

    if (flags & image_find_flag_DEFER)
	i->flags |= image_flag_DEFERRED;
    else
	i->flags &= ~image_flag_DEFERRED;

    /* Move the image to the front of the queue if we're not already */
    if (i->prev != NULL)
    {
	IMGDBG(("Moving the image to the front of the queue\n"));

	if (i->next)
	    i->next->prev = i->prev;
	else
	    image_last = i->prev;

	i->prev->next = i->next;

	/* We were not the front of the queue before so there must be something in the queue */

	i->next = image_list;
	i->prev = NULL;
	image_list->prev = i;
	image_list = i;
    }

    /* Have a go at forcing it to be fetched.  If we fail (too busy) at least we will be next */

    if ((flags & image_find_flag_DEFER) == 0)
    {
	IMGDBG(("Calling for another fetch\n"));

	image_fetch_next();
    }

    return NULL;
}

os_error *image_mark_to_flush(image i, int flags)
{
    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    i->flags |= image_flag_TO_RELOAD;
    if (flags & image_find_flag_DEFER)
	i->flags |= image_flag_DEFERRED;

    return NULL;
}

os_error *image_flush_marked(void)
{
    image ii, ii2;

    ii = image_list;

    while (ii)
    {
	ii2 = ii->next;

	if (ii->flags & image_flag_TO_RELOAD)
	{
	    image_flush(ii, (ii->flags & image_flag_DEFERRED) ? image_find_flag_DEFER : 0);
	}

	ii = ii2;
    }

    return NULL;
}

os_error *image_expire(image i)
{
    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    IMGDBGN(("image: expire %p\n", i));

    access_set_header_info(i->url, 1, 0, 0); /* date > expires */
    
    return NULL;
}

os_error *image_data_size(image i, image_flags *flags, int *data_so_far, int *data_size)
{
    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    if (flags)
	*flags = i->flags;
    if (data_so_far)
	*data_so_far = i->data_so_far;
    if (data_size)
	*data_size = i->data_size;

    return NULL;
}

os_error *image_file_info(image i, int *load, int *exec, int *size)
{
    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    if (load)
	*load = i->file_load_addr;
    if (exec)
	*exec = i->file_exec_addr;
    if (size)
	*size = i->data_size;

    return NULL;
}


/*
 * A version of image info that returns information on the current frame.
 * If there are no frames it just passes onto image_info().
 * Main difference is that it will get the MASK flag for the frame not the
 * main image.
 */

os_error *image_info_frame(image i, wimp_box *box, int *bpp, image_flags *flags)
{
    os_error *e = NULL;
    int dx, dy;

    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    if (box)
	image_get_scales(i, &dx, &dy);

    if (i->frame)
    {
	/* We have frame records */
	frame_rec *rec = i->frame + i->cur_frame;

	if (box)
	{
	    box->x0 = dx*(	      rec->x_off);
	    box->y1 = dy*(i->height - rec->y_off);
	    box->x1 = dx*(	      rec->x_off + rec->x);
	    box->y0 = dy*(i->height - rec->y_off - rec->y);
	}
	if (bpp)
	    *bpp = rec->depth;
	if (flags)
	    *flags = (i->flags &~ image_flag_MASK) | (rec->mask ? image_flag_MASK : 0);

	IMGDBG(("image_info_frame(): box %d,%d,%d,%d\n", box->x0, box->y0, box->x1, box->y1));
	IMGDBG(("image_info_frame(): flags=0x%x\n", flags ? *flags : 0));
    }
    else
    {
	int w, h;
	e = image_info(i, &w, &h, bpp, flags, NULL, NULL);
	if (!e && box)
	{
	    box->x0 = box->y0 = 0;
	    box->x1 = w*dx;
	    box->y1 = h*dy;
	}
    }

    return e;
}

os_error *image_info(image i, int *width, int *height, int *bpp, image_flags *flags, int *filetype, char **url)
{
    os_error *ep = NULL;
    sprite_header *sph;
    int ex, ey, l2bpp;
    /*int lbit;*/

    IMGDBG(("Asked for image info\n"));

    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    if (width || height || bpp)
    {
	MemCheck_checking checking;

	flexmem_noshift();

	if (i->id.tag == sprite_id_name)
	    ep = sprite_select_rp(*(i->areap), &(i->id), (sprite_ptr *) &sph);
	else
	    sph = (sprite_header *) i->id.s.addr;

	checking = MemCheck_SetChecking(0, 0);

	IMGDBG(("Sprite is at 0x%p, mode value is 0x%x\n", sph, sph ? sph->mode : 0));

	ex = bbc_modevar(sph->mode, bbc_XEigFactor);
	ey = bbc_modevar(sph->mode, bbc_YEigFactor);
	l2bpp = bbc_modevar(sph->mode, bbc_Log2BPP);

	MemCheck_RestoreChecking(checking);

	if (ep)
	{
	    usrtrc( "Error: %s\n", ep->errmess);
	}
	else
	{
#if 1
	    if (width)
		*width = i->width << ex;
	    if (height)
		*height = i->height << ey;
#else
	    lbit = (sph->mode > 255) ? 0 : sph->lbit;

	    if (width)
		*width = (((((sph->width+1) << 5) - lbit - (31 - sph->rbit)) >> l2bpp ) << ex);
	    if (height)
		*height = ((sph->height+1) << ey);
#endif
	    if (bpp)
		*bpp = 1 << l2bpp;
	}

	flexmem_shift();
    }

    if (flags)
	*flags = i->flags;
    if (filetype)
	*filetype = i->file_type;
    if (url)
	*url = i->url;

    IMGDBG(("Returning, width=%d, height=%d, flags=0x%x\n", width ? *width : 0, height ? *height : 0, flags ? *flags : 0));

    return ep;
}

/* ------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------ */

/* Borris: added a return type! */
static int im_gcf(int x, int y)
{
    int r;

    do
    {
	r = x % y;
	x = y;
	y = r;
    } while (y);

    return x;
}

/*
 * Returns FALSE if the scaling will end up at 1:1 in both directions
 * Otherwise reduces the scaling factor down to smallest numbers
 */

static BOOL image_reduce_scales(sprite_factors *facs)
{
    int r;

    if (facs->xmag == facs->xdiv && facs->ymag == facs->ydiv)
    {
	facs->xmag = facs->xdiv = 1;
	facs->ymag = facs->ydiv = 1;
	return FALSE;
    }
    
    r = im_gcf(facs->xmag, facs->xdiv);
    if (r != 1)
    {
	facs->xmag /= r;
	facs->xdiv /= r;
    }

    r = im_gcf(facs->ymag, facs->ydiv);
    if (r != 1)
    {
	facs->ymag /= r;
	facs->ydiv /= r;
    }

    return TRUE;
}

/*
 * If scale_image is not 100% then scale the scaling factors
 */

static void fixup_scale(sprite_factors *facs, int scale_image)
{
    if (scale_image != 100)
    {
        facs->xmag *= scale_image;
        facs->xdiv *= 100;
        facs->ymag *= scale_image;
        facs->ydiv *= 100;
    }
}

/*
 * Basically if w and h are != -1 then we have a desired size to plot at and so this
 * function rewrites the scaling factors to suit.
 * This is usually the case with the current oimage.c routines. This area could be optimised
 * bit more than it is.
 * id must contain a direct pointer.
 */

static void check_scaling(image i, sprite_id *id, int w, int h, int scale_image, sprite_factors *facs)
{
#if IMAGE_SCALE_TO_TAG
    if ((i->flags & image_flag_REALTHING) && (w != -1 && h != -1))
    {
#if 1
	sprite_info info;
	sprite_readsize((sprite_area *)0xff, id, &info);

	facs->xmag = w*2;
	facs->xdiv = info.width << bbc_modevar(-1, bbc_XEigFactor);
	facs->ymag = h*2;
	facs->ydiv = info.height << bbc_modevar(-1, bbc_YEigFactor);
#else
	int dw, dh;

	dw = i->width * facs->xmag/2;
	dh = i->height * facs->ymag/2;

	facs->xmag *= w;
	facs->xdiv *= dw;
	facs->ymag *= h;
	facs->ydiv *= dh;
#endif
    }
    else
	fixup_scale(facs, scale_image);
#else
    fixup_scale(facs, scale_image);
#endif
}

/* ------------------------------------------------------------------------------------------ */

/*
 * If we don't have a real image then we are using a filetype sprite.
 * Locate the sprite, or set up the default, and create a pixtrans and scale factors for it
 */

static os_error *image_default_image(image i, int scale_image, sprite_area **area_out, sprite_id *id_out, sprite_pixtrans **pt_out, sprite_factors *facs)
{
    char buffer[16];
    os_error *ep;
    os_regset r;
    sprite_area *area;
    sprite_header *sph;
    sprite_id id;
    sprite_pixtrans *pt;

    sprintf(buffer, "file_%03x", i->file_type);

    ep = os_swix(Wimp_BaseOfSprites, &r);

    area = (sprite_area *) (long) r.r[1];

    id.tag = sprite_id_name;
    id.s.name = buffer;

    ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);

    if (ep)
    {
	area = (sprite_area *) (long) r.r[0];
	ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);
    }

    if (ep)
    {
	strcpy(buffer, "file_xxx");
	ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);
    }

    if (ep == 0)
    {
	pt = mm_malloc(16);
	id.tag = sprite_id_addr;
	id.s.addr = sph;

	ep = wimp_readpixtrans(area, &id, facs, pt);
	if (!ep)
	{
	    fixup_scale(facs, scale_image);

	    if (area_out)
		*area_out = area;
	    if (id_out)
		*id_out = id;
	    if (pt_out)
		*pt_out = pt;
	    else
		mm_free(pt);
	}
	else
	    mm_free(pt);
    }

    return ep;
}

/* ------------------------------------------------------------------------------------------ */

static int cceil(int top, int bottom)
{
    return (top + bottom - 1)/bottom;
}

struct os_mode_selector
   {  int flags;
      int xres;
      int yres;
      int log2_bpp;
      int frame_rate;
   };

/*
 * Get a sprite type corresponding to the current mode
 * if sprite extend >= 99 then always use a new style sprite so
 * that the mask is smaller.
 */

static int build_mode_number(int log2bpp)
{
    return 1 + (log2bpp+1)*0x08000000 +
	(180 >> bbc_modevar(-1, bbc_YEigFactor))*0x00004000 +
	(180 >> bbc_modevar(-1, bbc_XEigFactor))*0x00000002;
}

static int get_mode_number(void)
{
    int mode;

    /* Try for new style sprite (3.6) */
    if (spriteextend_version >= 99)
	return build_mode_number(bbc_modevar(-1, bbc_Log2BPP));

    /* Else try and read screen mode - new way (3.5) */
    if (os_swi2r(os_X|OS_ScreenMode, 1, 0, 0, &mode) != NULL)
    {
	/* Else try and read screen mode - old way (3.1) */
        os_regset r;
        r.r[0] = 135;
        os_swix(OS_Byte, &r);
        mode = r.r[2];
    }

    /* If new style mode must use new style sprites (3.5) */
    if ((unsigned)mode > 255)
    {
        struct os_mode_selector *m = (struct os_mode_selector *)(long)mode;
	mode = build_mode_number(m->log2_bpp);
    }

    IMGDBG(("img: current sprite mode %08x\n", mode));

    return mode;
}

/*
 * Calculate the size of sprite area needed for the
 * sprite area, header, image and mask
 */

static int image_size_needed(int c_w, int c_h, int bpp, BOOL mask)
{
    int data_size = ((c_w*bpp + 31)/32)*4*c_h;
    int mask_size = 0;

    if (mask)
    {
	int mask_data_size;
	if (spriteextend_version >= 99 || bpp >= 16)	/* mask bpp = 1 bpp */
	    mask_data_size = ((c_w + 31)/32)*4*c_h;
	else						/* mask bpp = image bpp */
	    mask_data_size = data_size;

	mask_size = sizeof(sprite_header) + data_size + mask_data_size;
    }

    return sizeof(sprite_area) + (sizeof(sprite_header) + data_size) + mask_size;
}

/*
 * Convert a sprite id in 'id_in' (name or ptr) into a definite ptr in 'id_out'
 * anmd return the sprite pointer.
 * If null then the sprite couldn't be find and an error was printed to stderr.
 * It's OK if id_in == id_out
 */

static sprite_header *image_get_sprite_ptr(sprite_area *area, const sprite_id *id_in, sprite_id *id_out)
{
    sprite_ptr sptr;

    if (id_in->tag == sprite_id_name)
    {
	os_error *ep;
	ep = sprite_select_rp(area, (sprite_id *)id_in, &sptr);

	if (ep)
	{
	    usrtrc( "image: select sprite '%s' error '%s'\n", id_in->s.name, ep->errmess);
	    IMGDBGN(( "image: from %s,%s\n", caller(1), caller(2)));
	    return NULL;
	}

	if (id_out)
	{
	    id_out->tag = sprite_id_addr;
	    id_out->s.addr = sptr;
	}
    }
    else
    {
	sptr = id_in->s.addr;

	if (id_out && id_out != id_in)
	    *id_out = *id_in;
    }

    return sptr;
}

/*
 * Try and build a cache sprite. If the sprite has a mask then build a second one without
 * a mask of the same dimensions (this is used in animation). id written out is a name
 * and corresponds to the first sprite built if there are two (ie the one with the mask).

 * If the space needed is greater than the limit passed in then this is treated as if
 * we had run out of memory. Nothing is built and the tile code will use the original sprite.
 */

static void image_animation_clear_cache(image i)
{
    if (i->cache_mask)
    {
	sprite_header *shdr;
	sprite_id id;

	id.tag = sprite_id_name;
	id.s.name = CACHE_SPRITE_BUILD_NAME;

	flexmem_noshift();

	/* clear mask to completely transparent (0) */
	shdr = image_get_sprite_ptr(i->cache_area, &id, NULL);

	if (shdr)
	    memset((char *)shdr + shdr->mask, 0x0, (shdr->image > shdr->mask ? shdr->image : shdr->next) - shdr->mask);

	flexmem_shift();
    }
}

static os_error *image_init_cache_sprite(image i, int w, int h, int limit, BOOL mask, sprite_id *id_out)
{
    os_error *e;
    sprite_id id;
    int bpp = 1 << bbc_modevar(-1, bbc_Log2BPP);
    int size;

#if 0
    if (h <= 1)
	h = 2;
#endif

    size = image_size_needed(w, h, bpp, mask);
    size += 2*4*256;
    
    IMGDBG(("image: build cache size %d limit %d mask %d dims %dx%d\n", size, limit, mask, w, h));

    if (size > limit)
	return makeerror(ERR_NO_MEMORY);

    /* allocate and init sprite area */
#if FLEX
    if (flex_alloc((flex_ptr) &(i->cache_area), size + FLEX_FUDGE) == FALSE)
#else
    if ((i->cache_area = mm_malloc(size)) == NULL)
#endif
    {
	return makeerror(ERR_NO_MEMORY);
    }

    sprite_area_initialise(i->cache_area, size);

    /* create sprite */
    /* 24/10/96: add palette as this seems to fix some crashes (plotting 1bit+palette into 8bit */
    e = sprite_create(i->cache_area, CACHE_SPRITE_PLOT_NAME, sprite_nopalette, w, h, get_mode_number());
    if (!e && bbc_modevar(-1, bbc_Log2BPP) <= 3)
    {
	int buf[256];
	IMGDBGN(("image: add palette to sprite\n"));
	e = (os_error *)_swix(ColourTrans_ReadPalette, _INR(0,4), -1, -1, buf, sizeof(buf), 0);
	if (!e) e = (os_error *)_swix(ColourTrans_WritePalette, _INR(0,4), i->cache_area, CACHE_SPRITE_PLOT_NAME, buf, 0, 0);
	IMGDBGN(("image: added palette to sprite error '%s'\n", e ? e->errmess : ""));
    }
    
#if NEW_ANIMATION
    i->cache_mask = 0;
    if (!e && mask)
    {
	/* Create another sprite, with mask */
	int mode = get_mode_number();
	e = sprite_create(i->cache_area, CACHE_SPRITE_BUILD_NAME, sprite_nopalette, w, h, mode);
	if (!e)
	{
	    id.tag = sprite_id_name;
	    id.s.name = CACHE_SPRITE_BUILD_NAME;

	    e = sprite_create_mask(i->cache_area, &id);

	    /* Save the value needed to set a mask value (1 if new style sprite) */
/* 	    i->cache_mask = bpp >= 16 ? 1 : (1 << bpp) - 1; */
 	    i->cache_mask = mode > 255 ? 1 : (1 << bpp) - 1;

	    if (!e) image_animation_clear_cache(i); /* must be after cache_mask is set */

	    IMGDBG(("animation: add mask cache bpp %d mask %d\n", bpp, i->cache_mask));
	}
    }
#endif

    if (!e && id_out)
    {
	id_out->tag = sprite_id_name;
	id_out->s.name = CACHE_SPRITE_PLOT_NAME;
    }

    return e;
}

/*
 * For a sprite area and ptr find either or both of
 *  the pixel translation table needed
 *  the scaling factors needed
 * Assumes that pt is clean pointer
 */

static os_error *image_get_trans(sprite_area *area, sprite_ptr sptr, sprite_pixtrans **pixtrans, image_pixtrans_type *table_type, sprite_factors *factors)
{
    os_error *e = NULL;
    os_regset r;
    sprite_header *shdr = (sprite_header *)sptr;
    int sprite_bpp = 1 << bbc_modevar(shdr->mode, bbc_Log2BPP);

    /* get plot details */

    IMGDBGN(("img: get trans for %p/%p\n", area, sptr));

    /* If it is 16 colours or less and no palette use a WIMP palette */
    if (sprite_bpp <= 4 &&
	(shdr->mask == sizeof(sprite_header) || shdr->image == sizeof(sprite_header)) )
    {
	sprite_pixtrans dummy_p[16];
	sprite_factors dummy_f;
	sprite_id id;

	IMGDBGN(("img: using WIMP table\n"));

	if (pixtrans)
	{
	    *pixtrans = mm_malloc(16);
	    *table_type = pixtrans_NARROW;
	}

	id.tag = sprite_id_addr;
	id.s.addr = sptr;

	e = wimp_readpixtrans(area, &id, factors ? factors : &dummy_f, pixtrans ? *pixtrans : dummy_p);
    }
    else
    {
	if (pixtrans)
	{
	    int table_size = -1;

	    *pixtrans = NULL;
	    *table_type = pixtrans_UNKNOWN;

	    r.r[0] = (int) (long) area;
	    r.r[1] = (int) (long) sptr;
	    r.r[2] = -1;
	    r.r[3] = -1;
	    r.r[4] = 0;
	    r.r[5] = (1 << 0);
	    if (spriteextend_version >= 99)
		r.r[5] |= (1 << 4);   /* SJM: allow wide tables */

	    e = os_swix(ColourTrans_GenerateTable, &r);
	    if (!e)
	    {
		table_size = r.r[4];

		if (r.r[4])
		{
		    *pixtrans = mm_malloc(table_size);
		    *table_type = (table_size >> (sprite_bpp)) > 1 ? pixtrans_WIDE : pixtrans_NARROW;

		    r.r[4] = (int) (long) *pixtrans;
		    e = os_swix(ColourTrans_GenerateTable, &r);
		}
		else
		{
		    *table_type = sprite_bpp == (1 << bbc_modevar(-1, bbc_Log2BPP)) ? pixtrans_NONE_NEEDED : pixtrans_NONE_SCALE;
		}
	    }

	    IMGDBGN(("img: got table @ %p %d table size %d bpp %d\n", *pixtrans, *table_type, table_size, sprite_bpp));
	}

	if (factors)
	{
	    factors->xmag = 1 << bbc_modevar(shdr->mode, bbc_XEigFactor);
	    factors->xdiv = 1 << bbc_modevar(-1, bbc_XEigFactor);
	    factors->ymag = 1 << bbc_modevar(shdr->mode, bbc_YEigFactor);
	    factors->ydiv = 1 << bbc_modevar(-1, bbc_YEigFactor);
	}
    }

    return e;
}

/*
 * Build a cached tile sprite. Tiles never have masks. The background colour is plotted
 * in first if the original sprite had a mask.
 * We can call this with a sprite already built in which case it just re renders
 */

static BOOL image_build_cache_tile_sprite(image i, int scale_image)
{
    int i_w, c_w, i_h, c_h;
    int bpp;
    sprite_id id;
    os_error *e;

    int i_w_os, c_w_os, i_h_os, c_h_os;
    sprite_factors facs;

    sprite_id our_id;
    sprite_header *our_sph;

    /* see if we need to build the image
     * we may be being called because
     *  a) there is no cache image at all (first time or flushed)
     *  b) the bg colour has changed and we need to rerender
     */
    IMGDBGN(("img: building cache sprite orig size %dx%d bg %x\n", i->width, i->height, i->cache_bgcol.word));

    /* get the size of out cache image, use up to MAX_CACHE_BYTES on it */
    i_w = i->width*scale_image/100;
    c_w = i_w*cceil(128, i_w);

    bpp = 1 << bbc_modevar(-1, bbc_Log2BPP);

    i_h = i->height*scale_image/100;
    c_h = i_h * cceil(MAX_CACHE_BYTES*8, i_h*c_w*bpp);

    if (i->cache_area == NULL)
    {
	IMGDBGN(("img: scaled input %dx%d cache %dx%d size %d bytes\n", i_w, i_h, c_w, c_h, image_size_needed(c_w, c_h, bpp, FALSE)));

	/* allocate memory and init the blank sprite */
	e = image_init_cache_sprite(i, c_w, c_h, ABS_MAX_CACHE_BYTES, FALSE, &id);
    }
    else
    {
	id.tag = sprite_id_name;
	id.s.name = CACHE_SPRITE_PLOT_NAME;

	e = NULL;
    }

    flexmem_noshift();

    /* get original sprite details */
    our_sph = image_get_sprite_ptr(*i->areap, &i->id, &our_id);

    /* do main plot into cache sprite */
    i_w_os = c_w_os = i_h_os = c_h_os = 0;
    if (!e && our_sph)
    {
        sprite_state state;

	IMGDBGN(("image: cache output to sprite %p\n", id.s.name));

 	e = sprite_outputtosprite(i->cache_area, &id, NULL, &state);
        if (!e)
        {
            sprite_pixtrans *pt;
            image_pixtrans_type table_type;

            /* clear background if masked image */
            if (i->flags & image_flag_MASK)
            {
                int junk;
                colourtran_setGCOL(i->cache_bgcol, (1<<8) | (1<<7), 0, &junk);
                bbc_clg();
            }

	    e = image_get_trans(*i->areap, our_sph, &pt, &table_type, &facs);

	    IMGDBGN(("image: cache got trans error '%s'\n", e ? e->errmess : ""));

	    if (!e)
	    {
		int flags, q, p;
		BOOL scale_needed;

                /* get os sizes */  /* pdh, these were the other way round */
                i_w_os = i_w << bbc_modevar(our_sph->mode, bbc_XEigFactor);
                i_h_os = i_h << bbc_modevar(our_sph->mode, bbc_XEigFactor);
                c_w_os = c_w << bbc_modevar(-1, bbc_XEigFactor);
                c_h_os = c_h << bbc_modevar(-1, bbc_YEigFactor);

                /* adjust for display_scale */
                fixup_scale(&facs, scale_image);
		scale_needed = image_reduce_scales(&facs);

                flags = (i->flags & image_flag_MASK ? 0x8 : 0) | (table_type == pixtrans_WIDE ? 0x20 : 0);

		IMGDBGN(("image: cache do plot scale %d table %d @ %p\n", scale_needed, table_type, pt));
		
		for (q = 0; q < c_h_os; q += i_h_os)
	            for (p = 0; p < c_w_os; p += i_w_os)
		    {
			if (!scale_needed && table_type == pixtrans_NONE_NEEDED)
			    sprite_put_given(*i->areap, &our_id, flags, p, q);
			else
			{
/* 			    IMGDBGN(("image: area %p id %d/%p f %x @ %d,%d scale %p pt %p\n", *i->areap, our_id.tag, our_id.s.addr, flags, p, q, */
/* 					  scale_needed ? &facs : NULL, */
/* 					  table_type == pixtrans_NONE_NEEDED ? NULL : pt)); */

			    sprite_put_scaled(*i->areap, &our_id, flags, p, q,
 					  scale_needed ? &facs : NULL,
					  table_type == pixtrans_NONE_NEEDED ? NULL : pt);

/* 			    IMGDBGN(("image: done a plot\n")); */
			}
		    }
            }

	    IMGDBGN(("image: cache about to restore state\n"));

            /* discard pixtrans here */
	    mm_free(pt);
            sprite_restorestate(state);


	    IMGDBGN(("image: restored state\n"));

#if 0
            IMGDBGN(("img: os sizes in %d,%d out %d,%d\n", i_w_os, i_h_os, c_w_os, c_h_os));
            IMGDBGN(("img: factors %d/%d %d/%d\n", facs.xmag, facs.xdiv, facs.ymag, facs.ydiv));
            IMGDBGN(("img: pixtrans pt %p size %d wide %d\n", pt, table_size, wide_table));

            {
                int j;
                for (j = 0; j < table_size; j+=4)
                    IMGDBGN(("%08x \n", *(int *)(pt+j)));
            }
#endif
        }
	IMGDBGN(("img: plot main\n"));
    }

    /* tidy up */
    if (!e)
    {
#if 0
        sprite_area_save(i->our_area, "<NCFresco$Dir>.^.in");
        sprite_area_save(i->cache_area, "<NCFresco$Dir>.^.out");
#endif
    }
    else
    {
	if (i->cache_area)
	{

#if FLEX
	    flex_free((flex_ptr)&i->cache_area);
#else
	    mm_free(i->cache_area);
#endif
	    i->cache_area = NULL;
	}

        IMGDBGN(("img: error building cache sprite %x %s\n", e->errnum, e->errmess));
    }

    flexmem_shift();

    return e == NULL;
}

/* ------------------------------------------------------------------------------------------ */

static void image__render(image i, int x, int y, int w, int h, int scale_image)
{
    os_error *ep = NULL;
    sprite_pixtrans *pt = NULL;
    sprite_factors facs;
    sprite_area *area;
    sprite_id id;
    image_pixtrans_type table_type;
    int sp_op;
    BOOL need_scaling;

    if ( (i->flags & (image_flag_FETCHED | image_flag_RENDERABLE)) == image_flag_FETCHED)
    {
	ep = image_default_image(i, scale_image, &area, &id, &pt, &facs);

	table_type = pixtrans_NARROW;
    }
    else
    {
	MemCheck_checking checking;
	sprite_header *sph;

	IMGDBGN(("img: __render frame %p cache %p\n", i->frame, i->cache_area));

	if (i->frame && i->cache_area)
	{
	    /* if using an animation cache */
	    area = i->cache_area;
	    id.tag = sprite_id_name;
	    id.s.name = CACHE_SPRITE_PLOT_NAME;
	}
	else
	{
	    /* if using an uncached animation or plain image */
	    area = *(i->areap);
	    id = i->id;
	}

	if ((sph = image_get_sprite_ptr(area, &id, &id)) == NULL)
	    return;

	MemCheck_SetChecking(0, 0);

	IMGDBGN(("Pix trans table. bpp=%d, mask %x, image %x (sprite header %x)\n",
		 bbc_modevar(sph->mode, bbc_Log2BPP),
		 sph->mask, sph->image,
		 sizeof(sprite_header)));

	MemCheck_RestoreChecking(checking);

	/* If we don't have a pixtrans then build one, unless plotting from the cache which is
 	 * always in the current mode/palette
	 */
	ep = image_get_trans(area, sph,
			    i->table_type == pixtrans_UNKNOWN && area != i->cache_area ? &i->pt : NULL,
			    &i->table_type, &facs);

	if (area == i->cache_area)
	{
	    pt = NULL;
	    table_type = pixtrans_NONE_NEEDED;
	}
	else
	{
	    pt = i->pt;
	    table_type = i->table_type;
	}

	/* update scaling factors for desired width and height */
	check_scaling(i, &id, w, h, scale_image, &facs);
    }

    if (ep)
    {
	usrtrc( "Error3: %s\n", ep->errmess);
	return;
    }

    sp_op = table_type == pixtrans_WIDE ? 0x20 : 0;

    if (i->frame && i->cache_area == NULL)
    {
	/* Plotting a frame direct, not cached */
	frame_rec *rec = i->frame + i->cur_frame;

	x += rec->x_off * 2;
	y += (i->height - rec->y_off - rec->y) * 2;

	if (rec->mask)
	    sp_op |= 0x8;
    }
    else
    {
	/* Plotting a main image */
	if (i->flags & image_flag_MASK)
	    sp_op |= 0x8;
    }

    need_scaling = image_reduce_scales(&facs);

    IMGDBGN(("img: __render need scale %d table_type %d\n", need_scaling, table_type));

#if 0
    {
	char buffer[64];
	sprintf(buffer, "<Wimp$ScrapDir>.Sprites.%08x", (int)i);
	sprite_area_save(area, buffer);
	IMGDBG(("img: id %d/%p flags %x @%d,%d scale %d/%d %d/%d pixtrans %p\n", id.tag, id.s.addr, sp_op, x, y, 
		facs.xmag, facs.xdiv, facs.ymag, facs.ydiv, pt));
    }
#endif

    if (!need_scaling && table_type == pixtrans_NONE_NEEDED)
	ep = sprite_put_given(area, &id, sp_op, x, y);
    else
    {
#if 0
	if (i->width != 2 || i->height != 1)
#endif
	
	    ep = sprite_put_scaled(area, &id, sp_op, x, y, &facs, pt);
    }

    IMGDBGN(("img: __rendered\n"));

    if (pt && pt != i->pt)	/* Only free it if you have not remembered it */
    {
	IMGDBGN(("About to free pixel translation table\n"));

	mm_free(pt);
    }
}

/*
 * x,y is the position of the image on the page in screen coords
 * w,h is the size at which to plot the image in pixels, -1,-1 if default size
 * scale_image is a %age to scale the image to if w,h = -1,-1
 * plot_bg, handle is a function to plot the background to this page
 */

void image_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy)
{
    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	usrtrc( "Bad magic\n");
	return;
    }

    IMGDBG(("Asked to render image handle 0x%p at %d,%d\n", i, x, y));

    IMGDBGN(("Sprite area is 0x%p, image pointer is 0x%p, name is %s\n",
	    *(i->areap), i->id.s.name, i->id.tag == sprite_id_name ? i->id.s.name : "<none>"));

    flexmem_noshift();

#if NEW_ANIMATION
    if (i->frame && i->cache_area)
    {
	if (i->cache_mask)
	    image_animation_render(i, x, y, w, h, scale_image, plot_bg, handle, ox, oy);

	image__render(i, x, y, w, h, scale_image);

	if (i->cache_mask == 0 && i->frame[i->cur_frame].removal == webremove_PREVIOUS)
	    image_animation_render_frame(i, image_frame_PLOT_IMAGE);
    }
    else
#endif
    {
	image__render(i, x, y, w, h, scale_image);
    }

    flexmem_shift();
}

/*
 * returns TRUE if the image tile has changed for whatever reason.
 */

int image_tile(image i, int x, int y, wimp_box *bb, wimp_paletteword bgcol, int scale_image)
{
    sprite_area *area;
    sprite_id id;
    sprite_info info;
    int minp, minq, p, q, width, height;
    BOOL translate;
    image_pixtrans_type table_type;
    sprite_factors factors;
    sprite_pixtrans *pt;
    BOOL changed, must_rebuild;

    IMGDBGN(("Asked to tile image handle 0x%p at %d,%d\n", i, x, y));

    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	usrtrc( "Bad magic\n");
	return FALSE;
    }

    /* Borris: added additional clarifying parentheses */
    if ((i->flags & (image_flag_FETCHED | image_flag_RENDERABLE)) != (image_flag_FETCHED | image_flag_RENDERABLE))
	return FALSE;

    flexmem_noshift();

    /* if we have a mask and the bgcol changes then we need to re render cache sprite */
    must_rebuild = FALSE;
    if (i->cache_area == NULL)
    {
	must_rebuild = TRUE;
        i->cache_bgcol = bgcol;
    }
    else if (bgcol.word != i->cache_bgcol.word)
    {
        if (i->flags & image_flag_MASK)
        {
	    IMGDBGN(("img: bgcol changed\n"));
	    must_rebuild = TRUE;
        }

        i->cache_bgcol = bgcol;
    }

    /* build cache sprite if not in existence or bg changed */
    /* if it fails then use the original sprite */
    area = i->cache_area;

    if (!must_rebuild || image_build_cache_tile_sprite(i, scale_image))
    {
	changed = must_rebuild;

	area = i->cache_area;
	id.tag = sprite_id_name;
	id.s.name = CACHE_SPRITE_PLOT_NAME;

    	translate = FALSE;
    }
    else
    {
	changed = area != i->cache_area;

	/* If image is transparent then clear background first */
        if (i->flags & image_flag_MASK)
	{
	    int junk;
	    colourtran_setGCOL(bgcol, (1<<8) | (1<<7), 0, &junk);
	    bbc_move(bb->x0, bb->y0);
	    bbc_plot(bbc_RectangleFill + bbc_DrawAbsBack, bb->x1, bb->y1);
	}

	area = *i->areap;
	id = i->id;

	translate = TRUE;
    }

    /* get the sprite direct pointer */
    if (image_get_sprite_ptr(area, &id, &id))
    {
	/* read the sprite info */
	sprite_readsize(area, &id, &info);
	/* pdh: was bbc_modevar( -1, ... ) */
	width  = info.width << bbc_modevar(info.mode, bbc_XEigFactor);
	height = info.height << bbc_modevar(info.mode, bbc_YEigFactor);

	/* get sprite plot phase */
	minp = bb->x0 - ((bb->x0 - x) % width);
	if (bb->x0 < x)
	    minp -= width;

	minq = bb->y0 - ((bb->y0 - y) % height);
	if (bb->y0 < y)
	    minq -= height;

	pt = NULL;
	table_type = pixtrans_NONE_NEEDED;

	if (translate)
	{
	    /* only get pixtrans if we don't have one */
	    (void)image_get_trans(area, id.s.addr, i->table_type == pixtrans_UNKNOWN ? &i->pt : NULL, &i->table_type, &factors);

	    pt = i->pt;
	    table_type = i->table_type;

	    fixup_scale(&factors, scale_image);
	    image_reduce_scales(&factors);
	}

	IMGDBG(("image_tile(): ptr %p translate %d pt %p type %d mask %d\n", id.s.addr, translate, pt, table_type, i->flags & image_flag_MASK ? 1 : 0));
	
	/* plot the sprite */
	for (q=minq; q < bb->y1; q+= height)
	    for (p=minp; p < bb->x1; p+= width)
	    {
		if (translate)
		    sprite_put_scaled(area, &id,
				      (i->flags & image_flag_MASK ? 0x8 : 0) | (table_type == pixtrans_WIDE ? 0x20 : 0),
				      p, q, &factors, pt);
		else
 		    sprite_put_given(area, &id, 0, p, q);
/* 		    sprite_put_scaled(area, &id, 0, p, q, NULL, NULL); */
	    }
    }

    IMGDBG(("image_tile(): end\n"));

    flexmem_shift();

    return changed;
}

os_error *image_save_as_sprite(image i, char *fname)
{
    os_filestr fs;
    int *area;
    os_error *ep;

    flexmem_noshift();

    area = (int*) *(i->areap);

    fs.action =10;		/* Save with file type */
    fs.name = fname;
    fs.loadaddr = FILETYPE_SPRITE;
    fs.start = ((int) (long) area) + 4;
    fs.end = ((int) (long) area) + *area;

    ep = os_file(&fs);

    flexmem_shift();

    return ep;
}

void image_get_scales(image i, int *dx, int *dy)
{
    sprite_header *sph;
/*     sprite_area *area; */
/*     sprite_id id; */
    int mode;

    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	usrtrc( "Bad magic\n");
	return;
    }

    if ((i->flags & image_flag_RENDERABLE) == 0)
    {
	*dx = 2;
	*dy = 2;
	return;
    }

    flexmem_noshift();
#if 1
    sph = image_get_sprite_ptr(*(i->areap), &i->id, NULL);
#else
    area = *(i->areap);
    id = i->id;
    if (id.tag == sprite_id_name)
    {
	sprite_select_rp(area, &(id), (sprite_ptr *) &sph);
	id.s.addr = sph;
	id.tag = sprite_id_addr;
    }
    else
	sph = (sprite_header *) id.s.addr;
#endif
    mode = sph ? sph->mode : 27;
    flexmem_shift();

    if (dx)
	*dx = 1 << bbc_modevar(mode, bbc_XEigFactor);
    if (dy)
	*dy = 1 << bbc_modevar(mode, bbc_YEigFactor);
}

void image_os_to_pixels(image im, int *px, int *py, int scale_image)
{
    int dx, dy;
    int x, y;

    image_get_scales(im, &dx, &dy);

    x = *px / dx;
    y = *py / dy;

    if (x < 0)
	x = 0;
    if (y < 0)
	y = 0;

    if (scale_image != 100)
    {
        x = x*100/scale_image;
        y = y*100/scale_image;
    }

    *px = x;
    *py = y;
}

/* -------------------------------------------------------------------------- */

static int image_ave_col_n(image im, int n)
{
    int count[256];
    int palette[256];
    sprite_header *sphp;
    int lbit, rbit;
    int i,j,k;
    int cols = 1 << n;
    int mask = cols - 1;
    int *linep;
    os_regset r;
    os_error *ep;
    int reds, greens, blues;
    int pixcount;

    IMGDBG(("Calculating average over a %d colour image\n", cols));

    flexmem_noshift();

    if (im->id.tag == sprite_id_name)
	sprite_select_rp(*(im->areap), &(im->id), (sprite_ptr *) &sphp);
    else
	sphp = (sprite_header *) im->id.s.addr;

    lbit = (sphp->mode > 255) ? 0 : sphp->lbit;
    rbit = sphp->rbit;

    for (i = 0; i < cols; i++)
	count[i] = 0;

    linep = (int*) (((char*) sphp) + sphp->image);
    for (j=0; j < (sphp->height+1); j++)
    {
	for (i=0; i <= sphp->width; i++)
	{
	    int word = linep[i];
	    for (k=0; k < 32; k += n)
	    {
		int val;

		if ((i==0 && k < lbit) || (i == sphp->width && k > rbit))
		    continue;

		val = (word >> k) & mask;
		count[val]++;
	    }
	}
	linep += sphp->width + 1;
    }

    r.r[0] = (int) (long) *(im->areap);
    r.r[1] = (int) (long) im->id.s.addr;
    r.r[2] = (int) (long) palette;
    r.r[3] = sizeof(palette);
    r.r[4] = ((im->id.tag == sprite_id_name) ? 0 : 1);

#if 1
    ep = os_swix(ColourTrans_ReadPalette, &r);
#endif

    flexmem_shift();

    pixcount = reds = greens = blues = 0;

    for (i=0; i < cols; i++)
    {
	IMGDBGN(("col %d, count %d, palette 0x%08x\n", i, count[i], palette[i]));

	pixcount += count[i];

	reds += ((palette[i] >> 8) & 0xff) * count[i];
	greens += ((palette[i] >> 16) & 0xff) * count[i];
	blues += ((palette[i] >> 24) & 0xff) * count[i];
    }

    reds += (pixcount >> 1);
    reds /= pixcount;
    greens += (pixcount >> 1);
    greens /= pixcount;
    blues += (pixcount >> 1);
    blues /= pixcount;

    return (reds << 8) + (greens << 16) + (blues << 24);
}

int image_average_colour(image i)
{
    os_error *ep = NULL;
    sprite_header *sphp, sph;
    int l2bpp;
    int ave_col;

    IMGDBG(("Calculating average colour\n"));

    if (i == NULL || i->magic != IMAGE_MAGIC)
	return (int) config_colours[render_colour_BACK].word;

    flexmem_noshift();

    if (i->id.tag == sprite_id_name)
	ep = sprite_select_rp(*(i->areap), &(i->id), (sprite_ptr *) &sphp);
    else
	sphp = (sprite_header *) i->id.s.addr;

    sph = *sphp;

    flexmem_shift();

    if (ep)
    {
	usrtrc( "Error: %s\n", ep->errmess);
	ave_col = (int) config_colours[render_colour_BACK].word;
    }
    else
    {
	l2bpp = bbc_modevar(sph.mode, bbc_Log2BPP);

	IMGDBG(("Sprite mode value is 0x%x, l2bpp = %d\n", sph.mode, l2bpp));

	switch (l2bpp)
	{
	case 0:			/* 1bpp, 2 colours */
	case 1:			/* 2bpp, 4 colours */
	case 2:			/* 4bpp, 16 colours */
	case 3:			/* 8bpp, 256 colours */
	    ave_col = image_ave_col_n(i, 1 << l2bpp);
	    break;
	case 4:			/* 16bpp, 32K colours */
	case 5:			/* 32bpp, 16M colours */
	default:
	    ave_col = (int) config_colours[render_colour_BACK].word;
	    break;
	}
    }

    return ave_col;
}

/*
 * image_white_byte actually matches a 24bit colour passed in 0xbbggrrxx
 */

static int image_white_byte(image im, wimp_paletteword colour)
{
    int palette[256];
    sprite_header *sphp;
    int l2bpp;
    int cols;
    int best;
    int rr, gg, bb;
    os_regset r;
    os_error *ep = NULL;

    IMGDBG(("Finding colour closest to white\n"));

    flexmem_noshift();

    if (im->id.tag == sprite_id_name)
	sprite_select_rp(*(im->areap), &(im->id), (sprite_ptr *) &sphp);
    else
	sphp = (sprite_header *) im->id.s.addr;

    l2bpp = bbc_modevar(sphp->mode, bbc_Log2BPP);

    if (l2bpp <= 3)
    {
	IMGDBG(("Reading palette data.\n"));

	r.r[0] = (int) (long) *(im->areap);
	r.r[1] = (int) (long) im->id.s.addr;
	r.r[2] = (int) (long) palette;
	r.r[3] = sizeof(palette);
	r.r[4] = ((im->id.tag == sprite_id_name) ? (0<<0) : (1<<0) );

	ep = os_swix(ColourTrans_ReadPalette, &r);
    }
    flexmem_shift();

    bb = colour.bytes.blue;
    gg = colour.bytes.green;
    rr = colour.bytes.red;

    /* return colour as short/word for 16/32 bit */
    if (l2bpp == 4)
    {
        int c = ((bb & 0xf8) << (10-3)) | ((gg & 0xf8) << (5-3)) | ((rr & 0xf8) >> 3);
	return c | (c << 16);
    }
    else if (l2bpp == 5)
    {
	return colour.word;
    }

    if (ep)
    {
	usrtrc( "Error in best colour: %s\n", ep->errmess);
    }

    cols = 1 << (1 << l2bpp);

    best = find_closest_colour(colour.word, palette, cols);

    IMGDBG(("Best colour is 0x%x\n", best));

    while (l2bpp < 3)
    {
	best |= (best << (1 << l2bpp));
	l2bpp++;
    }

    IMGDBG(("Best colour byte is 0x%x\n", best));

    return best;
}

/* -------------------------------------------------------------------------- */

void image_save_as_draw(image i, int fh, wimp_box *bb, int *fileoff)
{
    draw_objhdr obj;
    char buffer[12];
    sprite_header *sph;
    sprite_area *area;
    sprite_id id;
    os_regset r;
    os_error *ep = NULL;

    flexmem_noshift();

    if ( (i->flags & (image_flag_FETCHED | image_flag_RENDERABLE)) == image_flag_FETCHED)
    {
	sprintf(buffer, "file_%03x", i->file_type);

	ep = os_swix(Wimp_BaseOfSprites, &r);

	area = (sprite_area *) (long) r.r[1];

	id.tag = sprite_id_name;
	id.s.name = buffer;

	ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);

	if (ep)
	{
	    area = (sprite_area *) (long) r.r[0];
	    ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);
	}

	if (ep)
	{
	    strcpy(buffer, "file_xxx");
	    ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);
	}
    }
    else
    {
	area = *(i->areap);
	id = i->id;
	if (id.tag == sprite_id_name)
	{
	    ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);
	    id.s.addr = sph;
	    id.tag = sprite_id_addr;
	}
	else
	    sph = (sprite_header *) id.s.addr;
    }

    if (ep)
    {
	usrtrc( "Error while saving sprite as draw: %s\n", ep->errmess);
	goto wayout;
    }

    obj.tag = draw_OBJSPRITE;
    obj.size = sizeof(obj) + sph->next;
    obj.bbox.x0 = bb->x0 << 8;
    obj.bbox.y0 = bb->y0 << 8;
    obj.bbox.x1 = bb->x1 << 8;
    obj.bbox.y1 = bb->y1 << 8;

    df_write_data(fh, *fileoff, &obj, sizeof(obj));
    *fileoff += sizeof(obj);
    df_write_data(fh, *fileoff, sph, sph->next);
    *fileoff += sph->next;

 wayout:
    flexmem_shift();
}

/* -------------------------------------------------------------------------- */

#if NEW_ANIMATION

/*
 * If the main image has a mask, or
 * if any frame both has a mask and wants to be cleared to the background
 * then we need a mask.

 * 15/10/96: Changed this to use a mask if any of them redraw to the baclground
 * to cope with the case where you have a frame smaller than the original being
 * plotted over the cleared background. This is effectively a masked image.

 */

static BOOL image_animation_need_mask(image i)
{
    frame_rec *rec;
    int f;

    IMGDBG(("anim: checking for mask\n"));

    if (i->flags & image_flag_MASK)
	return TRUE;

    rec = i->frame;

    /* if the first frame is not the size of the whole thing it needs a mask */
    if (rec->x != i->width || rec->y != i->height)
	return TRUE;

    /* if any frame wipes to background then it may need a mask */
    for (f = 0; f < i->frames; f++, rec++)
	if (rec->removal == webremove_BACKGROUND/*  && rec->mask */)
	    return TRUE;

    IMGDBG(("anim: no mask\n"));

    return FALSE;
}

#if DEBUG
/* static int animation_delay_is_fixed_to_1_second; */

static void image_animation_dump_info(image i)
{
    frame_rec *rec;
    int f;

    IMGDBG(("image animation '%s'\n", i->sname));

    for (f = 0, rec = i->frame; f < i->frames; f++, rec++)
    {
/* 	rec->delay = 100; */
	IMGDBG(("image %2d m %d r %d @ %d,%d d %dcs\n", f, rec->mask, rec->removal, rec->x_off, rec->y_off, rec->delay));
    }

#if 0
    sprite_area_save(i->our_area, "<"PROGRAM_NAME"$Dir>.^.Sprite");
#endif
}

#else
#define image_animation_dump_info(i)
#endif

static os_error *image_animation_init_cache(image i)
{
    sprite_info info;
    os_error *e;

    i->cur_frame = 0;

    /* create a cache sprite to draw into, possibly with a mask */
    e = sprite_readsize(*i->areap, &i->id, &info);
    if (!e)
    {
	int w = i->width << bbc_modevar(info.mode, bbc_XEigFactor) >> bbc_modevar(-1, bbc_XEigFactor);
	int h = i->height << bbc_modevar(info.mode, bbc_YEigFactor) >> bbc_modevar(-1, bbc_YEigFactor);

	if (w > 1 && h > 1)
	    e = image_init_cache_sprite(i, w, h, INT_MAX, image_animation_need_mask(i), NULL);
    }

#if 0
    sprite_area_save(i->cache_area, "<"PROGRAM_NAME"$Dir>.^.Cache");
#endif

    /* render the first frame in and copy its mask */
    if (!e && i->cache_area)
	image_animation_render_frame(i, image_frame_PLOT_MASK | image_frame_PLOT_IMAGE);

    return e;
}

static void image_animation_render_frame(image i, int flags)
{
    sprite_state state;
    sprite_id cache_id;
    sprite_id our_id;
    os_error *e;

    sprite_factors facs;
    sprite_pixtrans *pt;
    image_pixtrans_type table_type;

    const frame_rec *rec = i->frame + i->cur_frame;
    int x, y;

    IMGDBG(("animation: render frame %d flags %x mask %d\n", i->cur_frame, flags, rec->mask));

    flexmem_noshift();

    /* get ptr to this frame */
    if (!image_get_sprite_ptr(*i->areap, &i->id, &our_id))
    {
	flexmem_shift();
	return;
    }

    IMGDBGN(("animation: sprite ptr %p\n", our_id.s.addr));

    /* get translation for this frame
     * cache sprites are created in the current mode/palette so we can do this here
     */
    pt = NULL;
    table_type = pixtrans_NONE_NEEDED;
    e = image_get_trans(*i->areap, our_id.s.addr, flags & image_frame_PLOT_IMAGE ? &pt : NULL, &table_type, &facs);

    IMGDBGN(("animation: pt %p type %d scale %d/%d %d/%d\n", pt, table_type, facs.xmag, facs.xdiv, facs.ymag, facs.ydiv));

    /* calculate offsets - used in image and mask */
    x = rec->x_off * 2;
    y = (i->height - rec->y_off - rec->y) * 2;

    /* plot the image */
    if (!e && (flags & image_frame_PLOT_IMAGE))
    {
	/* set up id of image cache, either straight into plot or into build if there is a mask */
	cache_id.tag = sprite_id_name;
	cache_id.s.name = (flags & image_frame_TO_PLOT_SPRITE) || !i->cache_mask ? CACHE_SPRITE_PLOT_NAME : CACHE_SPRITE_BUILD_NAME;

	IMGDBGN(("animation: plot to image %p '%.12s' area %p sprite ptr %p\n", i->cache_area, cache_id.s.name, *i->areap, our_id.s.addr));

	e = sprite_outputtosprite(i->cache_area, &cache_id, NULL, &state);
	if (!e)
	{
	    sprite_put_scaled(*i->areap, &our_id,
 			      (rec->mask ? 8 : 0) | (table_type == pixtrans_WIDE ? 0x20 : 0),
			      x, y,
			      &facs, pt);

	    sprite_restorestate(state);
	}
    }

    /* Plot to the mask */
    IMGDBGN(("animation: mask %d flags %x\n", i->cache_mask, flags & (image_frame_PLOT_MASK|image_frame_CLEAR_MASK)));
    if (!e && i->cache_mask && (flags & (image_frame_PLOT_MASK|image_frame_CLEAR_MASK)))
    {
	/* set up id of image cache */
	cache_id.tag = sprite_id_name;
	cache_id.s.name = CACHE_SPRITE_BUILD_NAME;

	IMGDBGN(("animation: plot to mask\n"));
	e = sprite_outputtomask(i->cache_area, &cache_id, NULL, &state);
	if (!e)
	{
	    if (flags & image_frame_CLEAR_MASK)
	    {
		e = os_swi2(os_X | OS_SetColour, 0, 0);	/* set fg colour */
		if (!e) e = bbc_rectanglefill(x, y, rec->x*2, rec->y*2);
	    }
	    else
	    {
		e = os_swi2(os_X | OS_SetColour, (1<<4) + 0, i->cache_mask); /* set bg colour */
		if (!e) e = sprite_put_mask_scaled(*i->areap, &our_id, x, y, &facs);
	    }

	    sprite_restorestate(state);
	}
    }

    IMGDBGN(("animation: \n"));

    /* discard pixtrans here */
    mm_free(pt);

    flexmem_shift();

#if 0
    {
	char buffer[64];
	sprintf(buffer, "<NCFresco$Dir>.^.spr.%02d", i->cur_frame);
	sprite_area_save(i->cache_area, buffer);
    }
#endif

    /* report errors */
    if (e)
    {
	usrtrc( "img: animation render error '%s'\n", e->errmess);
    }
}

static int image_animation_render_background(image i, image_rectangle_fn plot_bg, void *handle, int scx, int scy)
{
    sprite_state state;
    wimp_redrawstr r;
    sprite_id cache_id;
    os_error *e;
    int bg_id = -1, save_size, *save_area;

    IMGDBG(("animation: render background fn 0x%p handle 0x%p scroll %d,%d\n", plot_bg, handle, scx, scy));

    if (plot_bg == 0)
	return bg_id;

    cache_id.tag = sprite_id_name;
    cache_id.s.name = CACHE_SPRITE_PLOT_NAME;

    r.box.x0 = r.box.y0 = 0;
    r.box.x1 = i->width*2;
    r.box.y1 = i->height*2;

    r.g = r.box;
    r.scx = scx;
    r.scy = scy;

    save_area = NULL;
    e = sprite_sizeof_spritecontext(i->cache_area, &cache_id, &save_size);
    if (!e) save_area = mm_calloc(save_size, 1);

    flexmem_noshift();

    if (!e) e = sprite_outputtosprite(i->cache_area, &cache_id, save_area, &state);
    if (!e)
    {
	bg_id = plot_bg(&r, handle, 0);

	sprite_restorestate(state);
    }

    flexmem_shift();

    mm_free(save_area);

    return bg_id;
}

/* Need to render into the plot sprite
 * a) the background
 * b) the build sprite
 * c) the current frame if it is a PREVIOUS

 * This needn't be done if all of the following are true.
 *  1) the frame hasn't changed (recorded in cache_frame)
 *  either
 *  2a) no bg tile and bgcolour is the same
 *  2b) bg tile with no mask, bg tile is the same, offsets are the same
 *  2c) bg tile with mask, bg tile is the same, offsets are the same, bg colour is the same
 */

static void image_animation_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy)
{
    sprite_state state;
    sprite_id id;
    os_error *e;

    /* see if it is necessary to re render the plot sprite*/
    if (i->cur_frame == i->cache_frame)
    {
	int bgid;

	/* if no background plot function assume not necessary */
	if (plot_bg == NULL)
	    return;

	bgid = plot_bg(NULL, handle, 0);
	if (bgid == i->cache_bgid)
	{
	    /* if bg type is not a pointer then this match is good enough */
	    if ((bgid & 3) != 0)
		return;

	    /* otherwise need to check that the phase/plot position is the same */
	    if (i->cache_x == x - ox && i->cache_y == y + h*2 - oy)
		return;
	}
    }

    /* set up id of image plot cache */
    id.tag = sprite_id_name;
    id.s.name = CACHE_SPRITE_PLOT_NAME;

    flexmem_noshift();

    e = sprite_outputtosprite(i->cache_area, &id, NULL, &state);
    if (!e)
    {
	frame_rec *rec;

	/* clear out the background */
	i->cache_bgid = image_animation_render_background(i, plot_bg, handle, x - ox, y + h*2 - oy);

	/* plot the current state animation sprite */
	id.s.name = CACHE_SPRITE_BUILD_NAME;
	sprite_put_given(i->cache_area, &id, 8, 0, 0);

	/* if PREVIOUS then plot the current sprite */
	rec = i->frame + i->cur_frame;
	if (rec->removal == webremove_PREVIOUS)
	    image_animation_render_frame(i, image_frame_PLOT_IMAGE | image_frame_TO_PLOT_SPRITE);

	sprite_restorestate(state);

	i->cache_frame = i->cur_frame;
	i->cache_x = x;
	i->cache_y = y;
    }

#if 0
    {
	char buffer[64];
	sprintf(buffer, "<NCFresco$Dir>.^.spr.%02d", i->cur_frame);
	sprite_area_save(i->cache_area, buffer);
    }
#endif

    flexmem_shift();
}

#endif

static void image_startup_animation(image i);

static void image_animation_alarm(int at, void *h)
{
    image i = (image) h;
    int redraw = 1;
    frame_rec *rec = i->frame + i->cur_frame;

#if NEW_ANIMATION
    wimp_box box_1, box_2, box_u;

    /* if we'd cancelled the cache (on palette change) then just call startup again */
    if (i->cur_frame == -1)
    {
	image_startup_animation(i);
	return;
    }
    
    switch (rec->removal)
    {
    case webremove_NODISPOSE:
    case webremove_NONE:
	/* No action necessary - give box x1,y1 < x0,y0 */
	memset(&box_1, 0, sizeof(box_1));
	box_1.x1 = box_1.y1 = -1;
	break;

    case webremove_BACKGROUND:
	/* redraw the area of this frame with the page background */
	/* ie clear the mask in the shape of this object */
	if (i->cache_area)
	    image_animation_render_frame(i, image_frame_CLEAR_MASK);

	image_info_frame(i, &box_1, NULL, NULL);
	break;

    case webremove_PREVIOUS:
	/* redraw the area of this frame with the buffered value */
	image_info_frame(i, &box_1, NULL, NULL);
	break;
    }
    IMGDBG(("animation: remove frame %d type %d box %d,%d %d,%d\n", i->cur_frame, rec->removal, box_1.x0, box_1.y0, box_1.x1, box_1.y1));
#endif

    /* increment the frame number */
    i->cur_frame++;
    if (i->cur_frame == i->frames)
    {
	i->cur_repeat++;
	if (i->cur_repeat == i->repeats)
	{
	    i->cur_frame--;
	    redraw = 0;		/* Cancel the redraw if this is the last frame */
				/* FIXME: need to remove last frame first */
	}
	else
	    i->cur_frame = 0;
    }

    /* handle the redraw */
    if (redraw)
    {
	rec = i->frame + i->cur_frame;

	if (i->cur_frame == 0)
	{
	    flexmem_noshift();
	    strncpy(i->sname, ((sprite_header *) (i->our_area + 1))->name, 12);
	    flexmem_shift();
	}
	else
	{
	    /* This will fail on animations with a million or more frames; will anyone notice? */
	    sprintf(i->sname, "image%d", i->cur_frame + 1);
	}

#if NEW_ANIMATION
	/* Update cache sprite if one exists */
	/* PREVIOUS removal types are not recorded in the cache, but are rendered on the fly */
	if (i->cache_area)
	{
	    /* if first frame then clear all the mask bits so we are invisible again */
	    if (i->cur_frame == 0)
		image_animation_clear_cache(i);

	    if (rec->removal != webremove_PREVIOUS)
		image_animation_render_frame(i, image_frame_PLOT_IMAGE | image_frame_PLOT_MASK);

	    /* Use one or two updates, depending on whether old and new boxes overlap */
	    image_info_frame(i, &box_2, NULL, NULL);

	    IMGDBG(("animation: add frame %d box %d,%d %d,%d\n", i->cur_frame, box_2.x0, box_2.y0, box_2.x1, box_2.y1));

	    if (coords_union(&box_1, &box_2, &box_u))
	    {
		IMGDBG(("animation: redraw union box %d,%d %d,%d\n", box_u.x0, box_u.y0, box_u.x1, box_u.y1));
		image_issue_callbacks(i, image_cb_status_UPDATE_ANIM, &box_u);
	    }
	    else
	    {
		IMGDBG(("animation: redraw separates\n"));
		image_issue_callbacks(i, image_cb_status_UPDATE_ANIM, &box_1);
		image_issue_callbacks(i, image_cb_status_UPDATE_ANIM, &box_2);
	    }
	}
	else
	{
	    free_pt(i);

	    image_issue_callbacks(i, (rec->mask && (rec->removal == webremove_BACKGROUND) ?
				  image_cb_status_REDRAW :
				  image_cb_status_UPDATE ), NULL );
	}
#else
	/* Oh what a pain, the new frame can have different palette so we have to slow ourself */
	free_pt(i);

	image_issue_callbacks(i, (rec->mask && (rec->removal == webremove_BACKGROUND) ?
				  image_cb_status_REDRAW :
				  image_cb_status_UPDATE ), NULL );
#endif
	IMGDBGN(("Setting animation alarm for %dcs from now.\n", rec->delay));

	alarm_set(alarm_timenow() + rec->delay, image_animation_alarm, i);
    }
}

static void image_startup_animation(image i)
{
    IMGDBG(("Animation starting, %d frames, %d cycles.\n", i->frames, i->repeats));

    if (i->frames > 1)
    {
#if NEW_ANIMATION
	os_error *e;

	image_animation_dump_info(i);

	e = image_animation_init_cache(i);
	
	if (e) usrtrc( "animation: start error %x %s\n", e->errnum, e->errmess);
#endif

	IMGDBGN(("Setting animation alarm for %dcs from now.\n", i->frame->delay));

	alarm_set(alarm_timenow() + i->frame->delay, image_animation_alarm, i);
    }
}

/* -------------------------------------------------------------------------- */

/* eof images.c */
