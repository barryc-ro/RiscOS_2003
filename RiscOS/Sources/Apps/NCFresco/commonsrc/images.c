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
 * 29/5/97: pdh: always use logical sizes
 *  1/7/97: pdh: sort out kortinkised GIFs once and for all (required gif dll
 *               changes). If the LSD size is less than the FD size, Netscape
 *               uses the LSD size if it's a GIF89 and the FD size if it's a
 *               GIF87! Now Fresco does too.
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
#include "gbf.h"

#include "threads.h"
#include "version.h"
#include "unwind.h"
#include "auth.h"
#include "files.h"

/* ------------------------------------------------------------------------- */

#ifndef NEW_WEBIMAGE
#define NEW_WEBIMAGE	2
#endif

#ifndef ITERATIVE_PANIC
#define ITERATIVE_PANIC 0
#endif

#ifdef STBWEB_BUILD
#  include "webimage.h"
#else
# include "../webimage/webimage.h"
#endif

/* Set debug to 2 or more for loads of detail on the image translation */

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef ADD_PALETTE
# define ADD_PALETTE 0
#endif

#define REAPER_POLL_TIME 500	/* Images arriving more than 5 seconds apart need to reload */

#define MAX_CACHE_BYTES		(32*1024)	/* target size for cached bg image */
#define ABS_MAX_CACHE_BYTES	(128*1024)	/* size beyond which we won't create cached bg image */
#define CACHE_SPRITE_PLOT_NAME	"cacheplot"
#define CACHE_SPRITE_BUILD_NAME	"cachebuild"

#define FLEX_FUDGE		16		/* get round sprite extend bug with reading over end of sprite areas */

#define SPRITE_NAME_ERROR	"sprerror"
#define SPRITE_NAME_DEFERRED	"deferred"
#define SPRITE_NAME_UNKNOWN	"unknown"

#define THREAD_STACK_SIZE	(4096+2048)	/* SJM: upped it a bit as PNG seems to need a bit more */

#define IMAGE_THREAD_BUFFER_SIZE	4096

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

typedef struct image_callback_str
{
    int use_count;
    image_callback cb;
    void *h;
    struct image_callback_str *next;
} image_callback_str;

#define IMAGE_MAGIC 0x78e6b2c9

typedef struct
{
    int ftype;
} webimage_str;

typedef enum
{
    pixtrans_UNKNOWN,		/* No table allocated, may need one, pixtrans is NULL */
    pixtrans_NONE_NEEDED,	/* No table is needed, pixtrans is NULL */
    pixtrans_NONE_SCALE,	/* No table allocated, pixtrans is NULL, OS will do bit packing/stretching */
    pixtrans_NARROW,		/* Table is a narrow <= 1 byte */
    pixtrans_WIDE		/* Table is wide > 1 byte */
} image_pixtrans_type;

typedef struct _image_info
{
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
    int find_flags;
    sprite_area **areap;
    sprite_area *our_area, *their_area, *cache_area;
    wimp_paletteword cache_bgcol;

    int cache_mask;		/* if 0 then the cache sprite has no mask and there is no build sprite */
    int cache_frame;		/* last frame number built from build sprite into plot sprite */
    int cache_bgid;		/* a value uniquely identifying the last background rendered */
    int cache_x, cache_y;	/* phase of last bg plotting in cache */

    sprite_id id;
    char sname[13];		/* Used when we point to a file icon or the default icon */

    sprite_pixtrans *pt;		/* If non-NULL points to a pixel translation table */
    image_pixtrans_type table_type;	/* See defines above */

    int width, height;		/* in pixels */
    int fh;			/* Used during reading the data */
    thread tt;
    int put_offset;
    image_callback_str *cblist;
    access_handle ah;

    frame_rec *frame;
    int frames;
    int repeats;
    int cur_frame, cur_repeat;
    char errbuf[256];

    void *data_area;
    int plotter;
    int xdpi, ydpi;
    int dx, dy;
    int offset_x, offset_y;	/* for DrawFile only */
} image_info_str;

#define wi_flag_MASK	0x01

/* ------------------------------------------------------------------------- */

static image image_list, image_last;
static int being_fetched;

static int image_thread_data_size;
static char *image_thread_data_ptr;
static int image_thread_data_more;
static int image_thread_data_status;
static int do_memory_panic = FALSE;
static int disallow_memory_panic = 0;
static int in_image_find = 0;

/* extern for use of NCFresco frontend */
int spriteextend_version;

/* ------------------------------------------------------------------------- */

static int image_white_byte(image im, wimp_paletteword colour);
static void image_fetch_next(void);
static void image_issue_callbacks(image i, int changed, wimp_box *box);
static void image_animation_alarm(int at, void *h);
static void image_startup_animation(image i);
static os_error *find_area_and_info(image i, sprite_info *info);
static char *image_thread_end(image i);
static void image_set_error(image i);

/* ------------------------------------------------------------------------- */

#define image_frame_PLOT_IMAGE	(1<<0)
#define image_frame_PLOT_MASK	(1<<1)
#define image_frame_CLEAR_MASK	(1<<2)
#define image_frame_TO_PLOT_SPRITE	(1<<3)

static void image_animation_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy);
static void image_animation_render_frame(image i, int flags);

/* ------------------------------------------------------------------------- */

#define DrawFile_Render		0x45540
#define DrawFile_BBox		0x45541

#define JPEG_Info		0x49980
#define JPEG_PlotScaled		0x49982

#define plotter_UNKNOWN		0
#define plotter_SPRITE		1
#define plotter_OSJPEG		2
#define plotter_DRAWFILE	3

typedef struct
{
    void (*render)(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy);
    os_error *(*save_as_sprite)(image i, char *fname);
    void (*save_as_draw)(image i, int fh, wimp_box *bb, int *fileoff);
} plotter_str;

static void image_sprite_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy);
static void image_jpeg_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy);
static void image_drawfile_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy);

static os_error *image_sprite_save_as_sprite(image i, char *fname);

static void image_sprite_save_as_draw(image i, int fh, wimp_box *bb, int *fileoff);
static void image_jpeg_save_as_draw(image i, int fh, wimp_box *bb, int *fileoff);
static void image_drawfile_save_as_draw(image i, int fh, wimp_box *bb, int *fileoff);

static plotter_str plotters[] =
{
    { 0, 0, 0 },
    { image_sprite_render,	image_sprite_save_as_sprite, image_sprite_save_as_draw },
    { image_jpeg_render,	0,			     image_jpeg_save_as_draw },
    { image_drawfile_render,	0,			     image_drawfile_save_as_draw },
};

static webimage_str translators[] = {
{ FILETYPE_SPRITE },
{ FILETYPE_GIF	},
{ FILETYPE_XBM	},
{ FILETYPE_JPEG	},
{ FILETYPE_PNG	},
{ FILETYPE_TIFF },
{ FILETYPE_DRAWFILE },
{ -1		}
};

typedef enum
{
    image_cache_SOLID,
    image_cache_MASK,
    image_cache_MASK_SEPARATE
} image_cache_t;

/* ------------------------------------------------------------------------- */

static char *image_strdup(const char *s)
{
    char *copy;
    disallow_memory_panic++;
    copy = strdup(s);
    disallow_memory_panic--;
    return copy;
}

static void *image_alloc(int n)
{
    void *copy;
    disallow_memory_panic++;
    copy = mm_calloc(n, 1);
    disallow_memory_panic--;
    return copy;
}

static void fetching_dec(image i)
{
    if (i->flags & image_flag_FETCHING)
    {
	being_fetched--;
	i->flags &= ~image_flag_FETCHING;
	IMGDBG(("im%p: fetching-- %d (%s)\n", i, being_fetched, caller(1)));
    }
}

static void fetching_inc(image i)
{
    if ((i->flags & image_flag_FETCHING) == 0)
    {
	being_fetched++;
	i->flags |= image_flag_FETCHING;
	IMGDBG(("im%p: fetching++ %d (%s)\n", i, being_fetched, caller(1)));
    }
}

static void free_pt(image i)
{
    if (i->pt)
    {
	mm_free(i->pt);
	i->pt = NULL;
    }
    i->table_type = pixtrans_UNKNOWN;
}

static void free_area(sprite_area **anchor)
{
    if (*anchor)
    {
	flex_free((void **)anchor);
	*anchor = NULL;
    }
}

static void free_data_area(void **anchor)
{
    if (*anchor)
    {
	flex_free(anchor);
	*anchor = NULL;
    }
}

static void fillin_scales(image i, int mode)
{
    /* fill in scaling values */
    i->dx = 1 << bbc_modevar(mode, bbc_XEigFactor);
    i->dy = 1 << bbc_modevar(mode, bbc_YEigFactor);
    i->xdpi = 180 / i->dx;
    i->ydpi = 180 / i->dy;
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

static int new_image_get_bytes(char *buf, int buf_len, void *h, BOOL *flush)
{
    int rc;

    IMGDBGN(("get_bytes: in: Asked for %d bytes, have %d bytes\n", buf_len, image_thread_data_size));

    if (image_thread_data_size == 0)
    {
	thread_wait("Need more data");

	IMGDBGN(("im%p: getbytes(%d), have none\n", h, buf_len));
    }

    if (image_thread_data_size == 0)
    {
	IMGDBGN(("get_bytes: out: rc=-1\n"));
	return -1;
    }

    rc = (image_thread_data_size > buf_len) ? buf_len : image_thread_data_size;

    memcpy(buf, image_thread_data_ptr, rc);

    image_thread_data_ptr += rc;
    image_thread_data_size -= rc;

    *flush = (image_thread_data_size == 0);

    IMGDBGN(("im%p: getbytes(%d) gave %d\n", h, buf_len, rc));

    return rc;
}

static void image_put_bytes(char *buf, int buf_len, void *h)
{
    image i = (image) h;

    IMGDBGN(("put_bytes: in: Putting 0x%x bytes at 0x%p to offset 0x%x flags 0x%x\n", buf_len, buf, i->put_offset, i->flags));

    if ( !i->our_area )
        return;

    if ( (i->put_offset + buf_len)  > i->our_area->size)
    {
	usrtrc( "i%p: Too much image data: %x + %x > %x\n",
		i, i->put_offset, buf_len, i->our_area->size);
    }
    else
    {
	flexmem_noshift();

	IMGDBGN(("put_bytes: copying %d bytes from %p to %p\n", buf_len, buf, ((char*) i->our_area) + i->put_offset));

	memcpy(((char*) i->our_area) + i->put_offset, buf, buf_len);

	IMGDBGN(("put_bytes: copied\n"));
	flexmem_shift();

	i->put_offset += buf_len;
    }

    if ((i->flags & image_flag_REALTHING) == 0)
    {
	int fill;

	i->areap = &(i->our_area);
	i->id.tag = sprite_id_name;
	i->id.s.name = i->sname;

	flexmem_noshift();
	IMGDBGN(("put_bytes: copying sprite name %.12s\n", ((sprite_header *) (i->our_area + 1))->name));
	strncpy(i->sname, ((sprite_header *) (i->our_area + 1))->name, 12);
	flexmem_shift();

	/* Fill with zero if we have no palette yet, use white if we can */
	fill = ((i->flags & image_flag_MASK) ?
		0 :		/* With a mask fill with zero */
		((i->put_offset < (sizeof(sprite_area) + sizeof(sprite_header))) ?
		 0xff :		/* Without a mask fill with 0xff */
		 image_white_byte(i, i->cache_bgcol)));	/* unless we can do better */

	IMGDBGN(("image fill: bg %08x fill %08x mask %d\n", i->cache_bgcol.word, fill, i->flags & image_flag_MASK ? 1 : 0));

        if (fill & 0xffffff00)
	{
            int *ip = (int *)(((char*) i->our_area) + ((i->put_offset + 3) &~ 3));
            int *end = (int *)(((char*) i->our_area) + i->our_area->size);
            while (ip < end)
                *ip++ = fill;
	}
	else
	{
	    IMGDBGN(("image fill: memset %p %x %d\n", ((char*) i->our_area) + i->put_offset, fill, i->our_area->size - i->put_offset));

	    /* I think this may be necessary */
	    if (i->our_area->size - i->put_offset > 0)
	    {
		IMGDBGN(("memset: from %p =%02x %d bytes\n", ((char*) i->our_area) + i->put_offset, fill, i->our_area->size - i->put_offset));

		flexmem_noshift();
		memset(((char*) i->our_area) + i->put_offset, fill, i->our_area->size - i->put_offset);
		flexmem_shift();
	    }
	}

	free_pt(i);

	i->flags |= image_flag_REALTHING/* | image_flag_RENDERABLE */;

 	IMGDBG(("New sprite area at %p, sprite name %s, width = %d, height = %d\n",
		i->our_area, i->id.s.name, i->width, i->height));
    }

    /* only set renderable bit when palette has arrived */
    if ((i->flags & image_flag_RENDERABLE) == 0)
    {
        sprite_header *sph;
        int pal_end;

/* 	IMGDBG(("put_bytes: try to set renderable\n")); */

	flexmem_noshift();

	sph = (sprite_header *) ((char *)i->our_area + i->our_area->sproff);
	pal_end = sph->mask > sph->image ? sph->mask : sph->image;

        if ((i->put_offset >= sizeof(sprite_area) + pal_end) &&
	    (i->flags & image_flag_USE_LOGICAL) == 0)
        {
            i->flags |= image_flag_RENDERABLE;
	    free_pt(i);

	    fillin_scales(i, sph->mode);
	}

	flexmem_shift();
    }

    i->flags |= image_flag_CHANGED;

    IMGDBGN(("im%p: put_bytes: out: flags %x\n", i, i->flags));
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

    IMGDBGN(("img: im %p given image_rec %p\n", i, ir));

    /* if we want OS jpeg and the OS can handle it, then abort transfer */
    if (ir->format == webformat_JPEG && !ir->interlaced && ir->jfif &&
	config_display_jpeg != 2 && spriteextend_version >= 99)
    {
	i->width = ir->x_logical;	/* changed to logical sizes */
	i->height = ir->y_logical;
	i->plotter = plotter_OSJPEG;
	i->flags |= image_flag_CHANGED;

	IMGDBG(("im%p: choosing OSJPEG\n", i ));

	IMGDBGN(("img: im %p leaving to OS size %dx%d\n", i, i->width, i->height));

	return FALSE;
    }

    size = ir->size;


    IMGDBG(("im%p: rec_fn: fmt %d size %dx%d logical size %dx%d\n", i,
            ir->format, ir->x, ir->y, ir->x_logical, ir->y_logical));

    i->frames = ir->frames;

    if (ir->frame)
    {
	int size = i->frames * sizeof(frame_rec);
	frame_rec *f;

	/* pdh: this is still needed even if i->frames = 1 */

	disallow_memory_panic++;
	f = mm_realloc(i->frame, size);
	disallow_memory_panic--;

	if (!f)
	    return FALSE;

	i->frame = f;
	memcpy(i->frame, ir->frame, size);
    }

    IMGDBG(("im%p: rec_fn: frames %d frame=%p repeats=%d\n", i,
            i->frames, ir->frame, ir->repeat ));

    i->repeats = ir->repeat;

    if (i->frames > 1)
	i->flags |= image_flag_ANIMATION;

    if (i->our_area)
    {
	int OK;

	OK = flex_extend((flex_ptr) &(i->our_area), size + 16 + FLEX_FUDGE);

	if (OK)			/* SJM: only fill in any of this if we got the memory */
	{
	    i->our_area->size = size+16;
	    i->our_area->freeoff = size + 16;
            i->our_area->number = ir->frames;
	}
	else
	{
	    IMGDBG(("im%p: flex_extend FAILED\n", i ));
/*	    SJM: don't free here - wait for panic to do it so that the deferred sprite is set correctly */
/* 	    flex_free( &i->our_area ); */
/* 	    i->our_area = NULL; */

	    /* This is a Bad Situation -- we'd like to kill the thread, but
	     * we're **IN** the thread!
	     */

            image_thread_data_size = 0;
   	    image_thread_data_ptr = 0;
            image_thread_data_more = 0;
            do_memory_panic = TRUE;

	    i->flags |= (image_flag_ERROR | image_flag_CHANGED);
	}

	return OK;
    }

    if (flex_alloc((flex_ptr) &(i->our_area), size + 16 + FLEX_FUDGE) == FALSE)
    {
        IMGDBG(("im%p: flex_alloc FAILED, panicking\n", i ));

        /* Similarly Bad Situation
         */

        image_thread_data_size = 0;
   	image_thread_data_ptr = 0;
        image_thread_data_more = 0;
        do_memory_panic = TRUE;

        i->our_area = NULL;

	i->flags |= (image_flag_ERROR | image_flag_CHANGED);
	return FALSE;
    }
    else
    {
	IMGDBG(("Flex alloc for %d bytes gave ptr %p\n", size+16, i->our_area));
    }

    memset(i->our_area, 0, sizeof(sprite_area) + sizeof(sprite_header));

    i->our_area->size = size+16;
    i->our_area->number = 1;
    i->our_area->sproff = 16;
    i->our_area->freeoff = size + 16;

    IMGDBGN(("Sprite area is %d bytes at %p\n", size+16, i->our_area));

    i->put_offset = 16;

    IMGDBGN(("rec_fn: out: have info; x=%d, y=%d\n", ir->x, ir->y));

    i->width = ir->x_logical;	/* changed to logical sizes */
    i->height = ir->y_logical;

    if ( i->width == 0 || i->height == 0 )
    {
	image_set_error(i);
/*      i->flags |= image_flag_ERROR; */
        return FALSE;
    }

    if (ir->x != ir->x_logical || ir->y != ir->y_logical)
	i->flags |= image_flag_USE_LOGICAL;

    if (ir->interlaced)
	i->flags |= image_flag_INTERLACED;

    if (ir->mask)
	i->flags |= image_flag_MASK;

/*     access_pause(i->ah); */

    return TRUE;
}

/* ------------------------------------------------------------------------- */

static int bastard_main(int argc, char **argv)
{
    char *result;
    void *i;
    int flags;

    i = (void *) argv;

    IMGDBG(("im%p: bastard_main called\n", i));

    flags = (config_deep_images ? webimage_DEEPSPRITE : 0) +
	((((image)i)->flags & image_flag_NO_BLOCKS) ? 0 : webimage_BLOCKDETAIL);

#ifdef STBWEB
    /* Disable deep-spriting if not in highcolor mode */
    if (bbc_vduvar(bbc_Log2BPP) <= 3)
	flags &= ~webimage_DEEPSPRITE;
#endif

    result = img2sprite(&new_image_get_bytes,	/* Get bytes from source */
			&image_put_bytes,	/* Put bytes to dest */
			&image_seek_fn,	/* Seek to where to put bytes */
			&image_rec_fn,	/* Tell us about the image */
			i, i,		/* Handles for get and put */
			flags,
			((image)i)->errbuf);	/* Flags */

    IMGDBG(("im%p: bastard_main done r=%d %s%s\n", i, (int)result,
            result ? "= " : "", result ? result : "" ));

    return (int) (long) result;
}

/* ------------------------------------------------------------------------- */

static void image_handle_internal(image i, int fh, void *buffer, int from, int to)
{
    BOOL success;

    IMGDBGN(("image_handle_internal: i%p fh %d buffer %p from %d to %d\n", i, fh, buffer, from, to));

    if (i->data_area == NULL)
    {
	success = flex_alloc(&i->data_area, to);
	from = 0;
    }
    else
    {
	success = flex_extend(&i->data_area, to);
    }

    IMGDBGN(("image_handle_internal: flex has returned\n"));

    if (success)
    {
	char *in;

	flexmem_noshift();

	in = (char *)i->data_area + from;
	if (buffer)
	    memcpy(in, buffer, to - from);
	else
	    ro_freadpos(in, 1, to - from, fh, from);

	if ((i->flags & image_flag_REALTHING) == 0)
	{
	    switch (i->plotter)
	    {
	    case plotter_OSJPEG:
		if (_swix(JPEG_Info, _INR(0,2) | _OUTR(2,5),
			  1, i->data_area, to,
			  &i->width, &i->height, &i->xdpi, &i->ydpi) == NULL)
		{
		    i->flags |= image_flag_CHANGED;

		    i->dx = /* i->xdpi ? (180 + i->xdpi/2) / i->xdpi : */ 2;
		    i->dy = /* i->ydpi ? (180 + i->ydpi/2) / i->ydpi : */ 2;
		}
		break;

	    case plotter_DRAWFILE:
	    {
		wimp_box box;
		if (_swix(DrawFile_BBox, _INR(0,4), 0, i->data_area, to, NULL, &box) == NULL)
		{
		    i->width = (box.x1 - box.x0) >> 9;	/* convert to nominal pixels */
		    i->height = (box.y1 - box.y0) >> 9;
		    i->offset_x = box.x0 >> 8;
		    i->offset_y = box.y0 >> 8;

		    i->xdpi = i->ydpi = 90;
		    i->dx = i->dy = 2;

		    i->flags |= image_flag_REALTHING | image_flag_MASK | image_flag_CHANGED;
		}
		break;
	    }

	    case plotter_UNKNOWN:
	    case plotter_SPRITE:
		if (from == 0)
		{
		    if (in[0] == 0xff && in[1] == 0xd8 && in[2] == 0xff)
			i->plotter = plotter_OSJPEG;
		    else if (*(int *)in == 0x77617244)
			i->plotter = plotter_DRAWFILE; /* 'Draw' */
		    else
			i->plotter = plotter_UNKNOWN;

                    IMGDBG(("im%p: setting plotter=%d in ihi\n", i, i->plotter));
		}
		break;
	    }
	}

	IMGDBGN(("image_handle_internal: plotter %d size %dx%d\n", i->plotter, i->width, i->height));

	flexmem_shift();
    }
    else
    {
	IMGDBG(("im%p: can't allocate %d-byte buffer\n", i, to ));

	image_set_error(i);
#ifndef STBWEB
	mm_can_we_recover(FALSE);
	image_flush( i, 0 );
#endif
    }

    if (i->plotter == plotter_UNKNOWN)
	free_data_area(&i->data_area);
}

/* ------------------------------------------------------------------------- */

static int image_thread_start(image i)
{
    IMGDBG(("im%p: starting thread\n",i));

    image_thread_data_size = 0;
    image_thread_data_ptr = 0;
    image_thread_data_more = 0;
    image_thread_data_status = 0;

    MemCheck_SetChecking(1,1);

    i->tt = thread_start(&bastard_main, 0, (char**) i, THREAD_STACK_SIZE);

    IMGDBG(("im%p: thread %p started\n", i, i->tt));

    return (i->tt != 0);
}

static char thread_buffer[IMAGE_THREAD_BUFFER_SIZE];

static int image_thread_process(image i, int fh, int from, int to)
{
    char *buffer = thread_buffer;
    int from_base = from;

    IMGDBGN(("image_thread_process: in: i %p fh %d from %d to %d\n", i, fh, from, to));

    if ( !i->tt )
        return FALSE;

/*  buffer = mm_malloc(IMAGE_THREAD_BUFFER_SIZE); */	/* taken off stack so doesn't affect wimpslot */

    while (from < to && i->tt->status == thread_ALIVE)
    {
	int len;

	len = (to-from) > IMAGE_THREAD_BUFFER_SIZE ? IMAGE_THREAD_BUFFER_SIZE : (to-from);

	IMGDBGN(("Reading %d bytes from file %d.\n", len, fh));

	/* this should check the values more carefully */
	if (ro_freadpos(buffer, 1, len, fh, from) == len)
	{
	    IMGDBGN(("Sending %d bytes to the image thread.\n", len));

	    image_thread_data_size = len;
	    image_thread_data_ptr = buffer;
	    image_thread_data_more = ((from + len) < to);

	    IMGDBGN(("im%p: running thread again (%d..%d, %s)\n",
	            i, from, from+len, ((from+len)<to) ? "more" : "final"));

	    MemCheck_RegisterMiscBlock(buffer, IMAGE_THREAD_BUFFER_SIZE);
	    thread_run(i->tt);
	    MemCheck_UnRegisterMiscBlock(buffer);
	}

	from += len;
    }

    /* only check this if the thread has died, we were reading from the start
     * and no sprite has been created
     * pdh: *And* there wasn't an error.

     * Unfortunately we can't tell what is a decode error and what is
     * an unknown format currently so this code gets called when nothing can be done
     */
    if (!do_memory_panic &&
	i->tt->status == thread_DEAD &&
	(from_base == 0 || (i->plotter != plotter_SPRITE/*  && i->plotter != plotter_UNKNOWN */)) &&
	i->our_area == NULL &&
	!(i->flags & image_flag_ERROR) )
    {
	image_handle_internal(i, fh, NULL, from_base, to);
    }

    IMGDBGN(("im%p: image_thread_process out: status %d\n", i, i->tt->status));

/*  mm_free(buffer); */

    if ( do_memory_panic )
    {
#ifdef STBWEB
	IMGDBG(("im%p: destroy thread %p due to panic\n", i, i->tt));

	image_thread_end(i);

	if (i->ah)
	{
	    access_abort(i->ah);
	    i->ah = NULL;

	    fetching_dec(i);
	}

	image_set_error(i);

	do_memory_panic = FALSE;
#else
        image_memory_panic();
#endif
        return FALSE;
    }

    return (i->tt->status == thread_ALIVE);
}

static char *image_thread_end(image i)
{
    char *res;

    IMGDBG(("im%p: image_thread_end (status=%d)\n", i, i->tt ? i->tt->status : -1));

    if (!i->tt)
	return NULL;

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

    if (i->tt->status == thread_ALIVE)
	res = "Thread did not finish";
    else
	res = (char *) (long) i->tt->rc;

    /* Clear up the thread */

    IMGDBG(("im%p: about to destroy thread 0x%p\n", i, i->tt));

    thread_destroy(i->tt);

    i->tt = NULL;

    IMGDBG(("image_thread_end: out: res='%s'\n", strsafe(res)));

    return res;
}

/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */

static void set_default_image(image i, const char *name, BOOL set_size)
{
    sprite_info info;

    strcpy(i->sname, name);
    i->id.tag = sprite_id_name;
    i->id.s.name = i->sname;

    i->plotter = plotter_SPRITE;
    i->areap = &(i->their_area);

#ifndef BUILDERS
    i->their_area = resspr_area();	/* start with the private area */
    find_area_and_info(i, &info);	/* update their_area correctly for where it is found */

    if (set_size)
    {
	i->width = info.width;
	i->height = info.height;

	fillin_scales(i, info.mode);
    }
#else
    if (set_size)
    {
	i->width = i->height = 34;

	fillin_scales(i, 27);
    }
#endif

    i->frames = 1;
}

static void image_set_error(image i)
{
    IMGDBG(("im%p: in set_error\n", i));
    i->flags |= (image_flag_ERROR | image_flag_CHANGED);
    i->flags &= ~image_flag_RENDERABLE | image_flag_REALTHING;

    set_default_image(i, SPRITE_NAME_ERROR, FALSE);
    free_pt(i);

    if (i->cfile)
    {
	mm_free(i->cfile);
	i->cfile = NULL;
    }
}

static void image_issue_callbacks(image i, int changed, wimp_box *box)
{
    if (i->cblist)
    {
	image_callback_str *cb = i->cblist;

	while (cb)
	{

	    IMGDBGN(("Callback, handle=0x%p, uses=%d, next=0x%p, changed=%d\n",
		    cb->h, cb->use_count, cb->next, changed));

	    if (cb->cb && (cb->use_count > 0))
		cb->cb(cb->h, (void*) i, changed, box);
	    cb = cb->next;
	}
    }
}

/* ------------------------------------------------------------------------- */

static void image_progress(void *h, int status, int size, int so_far, int fh, int ftype, char *url)
{
    image i = (image) h;
    BOOL more_data;
    int rd;

    if ( i->flags & image_flag_ERROR )
        return;

    rd = i->flags & image_flag_RENDERABLE;

    IMGDBGN(("im%p: progress in, status %d, data %d/%d\n",i,status,so_far,size));

    if (so_far == -1)
	so_far = 0;

    i->flags &= ~(image_flag_CHANGED);

    if ( status == status_COMPLETED_PART && so_far == size )
    {
        /* We can't rewind webimage, so we must start again */
        if ( i->tt )
	{
            image_thread_end(i);

	    /* SJM: 7Jul97: if internal then we need to force a plot */
	    if (i->plotter != plotter_SPRITE && i->plotter != plotter_UNKNOWN && i->data_area)
	    {
		i->flags |= image_flag_RENDERABLE | image_flag_REALTHING;
		image_issue_callbacks(i, image_cb_status_UPDATE, NULL);
	    }
	}

        IMGDBG(("im%p: completed a part size %d\n",i,size));

        rd = 0;

	/* set the size correctly before issuing callbacks */
        i->data_size = size;

        i->data_so_far = 0;
        i->put_offset = 16;     /* make sure image restarts */
        i->flags &= ~image_flag_NO_BLOCKS;
        image_thread_start(i);
    }

    more_data = (i->data_so_far != so_far);
    if (more_data)
    {
	if (status == status_GETTING_BODY
	    || status == status_COMPLETED_PART)
	{
	    if (i->tt == NULL)
	    {
		i->flags &= ~image_flag_NO_BLOCKS;
		i->file_type = ftype;
		IMGDBG(("im%p: calling thread_start from progress\n",i));
		image_thread_start(i);
	    }

	    IMGDBGN(("im%p: data arriving; file=%d, last had %d, now got %d\n",
		    i, fh, i->data_so_far, so_far));

	    if (i->tt)
	    {
		image_thread_process(i, fh, i->data_so_far, so_far);
	    }
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

    IMGDBGN(("...image progress out.\n"));
}

static access_complete_flags image_completed(void *h, int status, char *cfile, char *url)
{
    int ft;
    image i = (image) h;
    char *err = NULL;
    int rd;

    rd = i->flags & (image_flag_RENDERABLE | image_flag_ERROR);

    fetching_dec(i);

    if (!in_image_find)
	image_fetch_next();

    if (i->magic != IMAGE_MAGIC)
	return 0;

    i->ah = NULL;

    /* SJM: kill thread */
    if (i->tt)
	image_thread_end(i);

    if (status == status_COMPLETED_FILE)
    {
	os_filestr ofs;

	i->cfile = image_strdup(cfile);
	i->flags |= image_flag_FETCHED;

	i->file_type = ft = file_type(cfile);

	ofs.action = 5;
	ofs.name = i->cfile;
	os_file(&ofs);

	i->file_load_addr = ofs.loadaddr;
	i->file_exec_addr = ofs.execaddr;
	i->data_size = ofs.start;

	IMGDBG(("im%p: got file '%s', type 0x%03x, flags 0x%08x\n", i, cfile, ft, i->flags));

	/* if we had some data then assume it is renderable */
	if (i->our_area || i->data_area)		/* SJM: added i->data_area and simplified as err is always NULL now */
	{
	    i->flags |= image_flag_RENDERABLE | image_flag_REALTHING;

	    if (i->plotter == plotter_SPRITE)
	    {
		sprite_info info;
		os_error *ep;

		ep = sprite_readsize(i->our_area, &i->id, &info);

		IMGDBG(( "image_completed: sprite_readsize gives %dx%d, e %s\n",
			 info.width, info.height, ep ? ep->errmess : "" ));

		if ( ep )
		{
		    /* For instance, New Sprite on RiscOS 3.1 */
		    free_data_area(&i->data_area);
		    free_area(&i->our_area);

		    image_set_error(i);
		}
	    }
	}
	else
	{
	    usrtrc( "Image error 1 on %s\n", i->url );

	    if ((i->flags & image_flag_RENDERABLE) == 0)
	    {
		free_data_area(&i->data_area);

		free_area(&i->our_area);

		/* No alarms exist at this stage */
		image_set_error(i);
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
	rd ^= i->flags & (image_flag_RENDERABLE | image_flag_ERROR);

	if (i->cblist)
	{
	    image_issue_callbacks(i, (rd ? image_cb_status_REFORMAT : image_cb_status_UPDATE), NULL);
	}
    }

    /* @@@@ Fire up the animation process here */
    image_startup_animation(i);

    IMGDBG(("image_completed: im%p returning\n", i));

    return (access_CACHE | access_KEEP); /* Cache the file and try to hold on to it */
}

/* ---------------------------------------------------------- */

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

    /* pdh: I had my doubts, but shockingly this *does* seem to be the
     * easiest way of doing it
     */
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
	    fetching_dec(i);
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

	mm_free(i->frame);

	alarm_removeall(i);

	ii = i->next;
	mm_free(i);
    }

    /* Remove the reaper function */
    alarm_removeall((void*) translators);

    IMGDBG(("Tidied up images\n"));

    return NULL;
}

/* ------------------------------------------------------------------------- */

/*
 * This function is for the use of the formatting code when it has determined that
 * the scaling needs to change and thus the background needs to be rescaled.
 */

void image_uncache_info(image i)
{
    free_pt(i);
    free_area(&i->cache_area);

    if (i->frames > 1)
    {
	i->cache_frame = -1;
	i->cur_frame = -1;
	i->cache_mask = 0;

	flexmem_noshift();
	strncpy(i->sname, ((sprite_header *) (i->our_area + 1))->name, 12);
	flexmem_shift();
    }

    /* pdh: depth may have changed */
    i->table_type = pixtrans_UNKNOWN;
}

void image_palette_change(void)
{
    image i;

    for (i=image_list; i != NULL; i = i->next)
    {
	image_uncache_info(i);
    }
}

void image_defer( image i )
{
    i->flags |= image_flag_DEFERRED;
}

static void image_fetch_next(void)
{
    image i;

    IMGDBG(("Scaning image list\n"));

    for (i=image_list; (being_fetched < config_max_files_fetching) && (i != NULL); i = i->next)
    {
	/* Pick ones that are waiting and not deferred or blacklisted */
	if ((i->flags & (image_flag_WAITING | image_flag_DEFERRED | image_flag_BLACKLIST)) == image_flag_WAITING)
	{
	    os_error *ep;
	    int aflags;

	    aflags = 0;
	    if (i->flags & image_flag_TO_RELOAD)
		aflags |= access_NOCACHE;

	    if (i->find_flags & image_find_flag_CHECK_EXPIRE) /* only check for expiry if image_find() was called with CHECK_EXPIRE on */
		aflags |= access_CHECK_EXPIRE;
	    if (i->find_flags & image_find_flag_URGENT)
		aflags |= access_MAX_PRIORITY;
            else if (i->find_flags & image_find_flag_NEED_SIZE)
		aflags |= access_IMAGE;

	    i->flags &= ~(image_flag_WAITING | image_flag_TO_RELOAD);

	    fetching_inc(i);	/* In case it comes from the cache we increment this here */

	    IMGDBG(("image_fetch_next: inc fetching %d\n", being_fetched));

	    ep = access_url(i->url, aflags, 0, 0, i->ref,
			    &image_progress, &image_completed, i, &(i->ah));

	    if (ep)
	    {
		i->ah = NULL;
		image_set_error(i);
		usrtrc( "Error accessing image: %s\n", ep->errmess);
		fetching_dec(i); /* If we failed, decrement it again */
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

    fillin_scales(i, info.mode);

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
	IMGDBG(("im%p: inc use %d Asked to find image '%s'\n", i, i->use_count, url));
	i->use_count++;
	if ((flags & image_find_flag_DEFER) == 0 &&
	    (i->flags & image_flag_DEFERRED) != 0 )
	{
	    i->flags &= ~image_flag_DEFERRED;
	}
    }
    else
    {
	IMGDBG(("im%p: Making new image\n", i));

	i = image_alloc(sizeof(*i));
	if (!i)
	    return makeerror(ERR_NO_MEMORY);

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
	i->url = image_strdup(url);
	i->hash = hash;
	i->flags = image_flag_WAITING;
	/* DAF: This clashed on merging. I presume the former is wanted? SJM: NOooooooooooooooooo! the latter!*/
#if 0
	if (flags & image_find_flag_DEFER)
#else
	if ((flags & image_find_flag_DEFER) || gbf_active(GBF_LOW_MEMORY) )
#endif
	    i->flags |= image_flag_DEFERRED;

	set_default_image(i, (flags & image_find_flag_DEFER) ? SPRITE_NAME_DEFERRED : SPRITE_NAME_UNKNOWN, TRUE);

        if ( blacklist_match(url) )
        {
            set_default_image( i, "black", TRUE );
            i->flags |= image_flag_BLACKLIST;
        }

	i->find_flags = flags;
	i->cache_bgcol = bgcol;
    }

    /* Use the first ref we see */
    if (ref && i->ref == NULL)
	i->ref = image_strdup(ref);

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

	    new_cb = image_alloc(sizeof(*new_cb)); /* on error? */
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
	    /* don't fetch too many at once because of the transient memory usage */
	    if ((being_fetched < config_max_files_fetching) || (flags & image_find_flag_URGENT))
	    {
		fetching_inc(i);

		IMGDBG(("image_find: inc fetching %d\n", being_fetched));

		/* If the file is already around then we don't care if it was deferred, do we? */
		i->flags &= ~(image_flag_WAITING | image_flag_DEFERRED);

		in_image_find++;

		ep = access_url( url,
				 (flags & image_find_flag_NEED_SIZE ? access_IMAGE : 0) |
				 (flags & image_find_flag_URGENT ? access_MAX_PRIORITY : 0),
				 0, 0, i->ref, &image_progress,
				 &image_completed, i, &(i->ah));
		if (ep)
		{
		    i->ah = NULL;
		    image_set_error(i);
		    usrtrc( "Error accessing image: %s\n", ep->errmess);

		    fetching_dec(i);
		}

		in_image_find--;
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
	IMGDBG(("Making new image\n"));

	i = image_alloc(sizeof(*i));
	if (!i)
	    return makeerror(ERR_NO_MEMORY);

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
	i->url = image_strdup(url);
	i->hash = hash;

	set_default_image(i, SPRITE_NAME_UNKNOWN, TRUE);
    }

    *result = i;

    if ((i->cfile == NULL) &&
	(i->ah == NULL) &&
	((i->flags & (image_flag_ERROR | image_flag_STREAMING)) == 0) )
    {
	*already = FALSE;
	i->flags |= image_flag_STREAMING;
	i->file_type = ft;
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

    IMGDBG(("image_stream_data: i%p buffer %p len %d update %d\n", i, buffer, len, update));

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

	IMGDBG(("Running thread from stream data sofar %d len %d\n", i->data_so_far, len));

	thread_run(i->tt);
    }

    /* only check this if the thread has died, we were reading from the start
     * and no sprite has been created
     * pdh: *And* there wasn't an error.

     * FIXME:
     * This won't work if the file cannot be identified in the first buffer as the previous
     * buffers aren't available and we can't back up.
     */

    if (i->tt->status == thread_DEAD &&
	(i->data_so_far == 0 || i->plotter != plotter_SPRITE) &&
	i->our_area == NULL &&
	(i->flags & (image_flag_ERROR | image_flag_LOAD_AT_END)) == 0)
    {
	if (i->data_so_far != 0 && i->data_area == NULL)
	    i->flags |= image_flag_LOAD_AT_END;
	else
	    image_handle_internal(i, 0, buffer, i->data_so_far, i->data_so_far + len);
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

    if (cfile)
    {
	i->cfile = image_strdup(cfile);

	ofs.action = 5;
	ofs.name = i->cfile;
	os_file(&ofs);

	i->file_load_addr = ofs.loadaddr;
	i->file_exec_addr = ofs.execaddr;
	i->data_size = ofs.start;

	if ( (i->flags & (image_flag_LOAD_AT_END | image_flag_ERROR)) == image_flag_LOAD_AT_END)
	{
	    int fh = ro_fopen(cfile, RO_OPEN_READ);

	    if (fh)
		image_handle_internal(i, fh, NULL, 0, i->data_size);

	    ro_fclose(fh);
	}
    }

    if (i->tt)
    {
	IMGDBGN(("Calling image_thread_end() from image_stream_end()\n"));
	res = image_thread_end(i);

	/* SJM: set up so that internal objects can be displayed */
	if (i->data_area && (i->flags & image_flag_ERROR) == 0)
	{
	    res = NULL;
	    i->flags |= image_flag_REALTHING;
	}
    }
    else
    {
        /* pdh: changed 'cos this isn't really an error situation
	res = "No thread to end";
	 */
        res = NULL;
    }

    i->flags |= image_flag_FETCHED;
    i->flags &= ~image_flag_STREAMING;

    if (res == NULL && (i->flags & image_flag_ERROR) == 0)
	i->flags |= image_flag_RENDERABLE;
    else
    {
	usrtrc( "Image error 2 = %s\n", strsafe(res));

	free_area(&i->our_area);
	flex_free(&i->data_area);
	i->data_area = NULL;

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

    IMGDBG(("im%p: Loose image called: '%s', use count %d\n", i, i->url ? i->url : "", i->use_count));

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
	    IMGDBG(("Calling image_thread_end() from image_loose()\n"));
	    image_thread_end(i);
	}

	mm_free(i->ref);

	if (i->url)
	{
	    if (i->ah)
	    {
		IMGDBG(("Loose image Aborting access\n"));

		access_abort(i->ah);
		fetching_dec(i);
	    }
	    else
	    {
		IMGDBG(("im%p: Unkeeping the file %s\n", i, i->url));

		access_unkeep(i->url);
	    }

	    IMGDBG(("Freeing URL\n"));

	    mm_free(i->url);
	}

	free_area(&i->our_area);
	free_data_area(&i->data_area);

	/* Get rid of any animation alarms */
	alarm_removeall(i);

	free_area(&i->cache_area);
	mm_free(i->cfile);

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
	mm_free(i->frame);

	i->magic = 0;

	IMGDBG(("Freeing image object\n"));

	mm_free(i);
    }

    IMGDBG(("Loose image done\n"));

    return NULL;
}

#if ITERATIVE_PANIC
/* if we have an animation then trim back to a normal image */
static BOOL image_trim_animation(image i)
{
    if (i->frames > 1 && i->our_area)
    {
	int sprite_size = ((sprite_header *)(i->our_area + 1))->next;
	int area_size = sprite_size + sizeof(sprite_area) + FLEX_FUDGE;

	i->frames = 1;
	i->our_area->number = 1;
	i->our_area->size = area_size;
	i->our_area->freeoff = sprite_size + sizeof(sprite_area);

	(void)flex_extend((flex_ptr) &(i->our_area), area_size);

	/* get rid of the cache */
	free_area(&i->cache_area);

	/* and reset appropriate variables */
	i->cache_frame = -1;
	i->cur_frame = -1;
	i->cache_mask = 0;

	flexmem_noshift();
	strncpy(i->sname, ((sprite_header *) (i->our_area + 1))->name, 12);
	flexmem_shift();

	return TRUE;
    }
    return FALSE;
}
#endif /* ITERATIVE_PANIC */

int image_memory_panic(void)
{
    image i;
    int freed = FALSE;

    IMGDBG(( "Image memory panic called from %s/%s/%s\n", caller(1), caller(2), caller(3)));

    if (disallow_memory_panic)
    {
	IMGDBG(( "Image memory panic not allowed (threaded %d)\n", disallow_memory_panic ));
	return FALSE;
    }

    do_memory_panic = FALSE;

    /* stop this function being reentered */
    disallow_memory_panic++;

#if ITERATIVE_PANIC
    for (i=image_list; i != NULL && !freed; i = i->next)
    {
	freed = image_trim_animation(i);
    }
#endif /* ITERATIVE_PANIC */

    for (i=image_list; i != NULL; i = i->next)
    {
	/* If we already have the image then dispose of it */
	if (i->our_area || i->cache_area || i->data_area)
	{
	    freed = TRUE;

	    IMGDBG(("im%p: disposal in panic", i));

	    /* call thread end to ensure that all the thread/image heap memory gets freed */
	    image_thread_end(i);	    IMGDBG(("..0"));

	    free_area(&i->our_area);        IMGDBG(("..1"));
	    free_area(&i->cache_area);      IMGDBG(("..2"));
	    free_data_area(&i->data_area);  IMGDBG(("..3"));

	    STRING_FREE(&i->cfile);         IMGDBG(("..4"));
	    free_pt(i);                     IMGDBG(("..5"));
	    nullfree((void **)&i->frame);   IMGDBG(("..6"));

	    /* No point in animating a question mark !!!*/
	    alarm_removeall(i);             IMGDBG(("..7\n"));

	    i->data_so_far = i->data_size = 0;
	    i->file_type = 0;
	    i->flags = image_flag_WAITING | image_flag_DEFERRED | image_flag_TO_RELOAD;

	    set_default_image(i, SPRITE_NAME_DEFERRED, FALSE);

#ifdef STBWEB
	    /* SJM: keep display up to date */
	    image_issue_callbacks(i, image_cb_status_REDRAW, NULL);
#endif /* STBWEB */

#if ITERATIVE_PANIC
	    break;
#endif /* ITERATIVE_PANIC */
	}
    }

    disallow_memory_panic--;

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
	IMGDBGN(("Calling image_thread_end() from image_flush()\n"));
	image_thread_end(i);
    }

    if (i->url)
    {
	if (i->ah)
	{
	    IMGDBG(("Flush image Aborting access\n"));

	    access_abort(i->ah);
	    fetching_dec(i);

	    i->ah = NULL;
	}
	else
	{
	    IMGDBG(("im%p: Unkeeping the file: '%s'\n", i, i->url));

	    access_unkeep(i->url);
	}

	access_remove(i->url);
    }

    if (i->flags & image_flag_ERROR)
    {
	i->flags &= ~image_flag_ERROR;
    }

    /* pdh: moved this outside the above if */
    i->flags |= image_flag_TO_RELOAD | image_flag_WAITING;
    i->flags &= ~(image_flag_RENDERABLE | image_flag_REALTHING);

    /* If we already have the image then dispose of it */
    /* SJM: add or ->data_area so that JPEGs can be reloaded */
    if (i->our_area || i->data_area)
    {
	IMGDBG(("im%p: flushing",i));

	free_area(&i->our_area);
	nullfree((void **)&i->frame);

	IMGDBG(("..1"));

	/* Remove any animations */
	alarm_removeall(i);
	i->cur_repeat = 0;

        IMGDBG(("..2"));

	free_data_area(&i->data_area);
	free_area(&i->cache_area);
	STRING_FREE(&i->cfile);
	free_pt(i);

	IMGDBG(("..3"));

	i->data_so_far = i->data_size = 0;
	i->file_type = 0;
	i->flags = image_flag_WAITING;
	i->flags |= image_flag_TO_RELOAD;

	set_default_image(i, (flags & image_find_flag_DEFER) ? SPRITE_NAME_DEFERRED : SPRITE_NAME_UNKNOWN, FALSE);

	IMGDBG(("..4"));

	if (i->cblist)
	{
	    image_issue_callbacks(i, image_cb_status_REFORMAT, NULL);
	}

	IMGDBG(("..5\n"));
    }

    if (flags & image_find_flag_DEFER)
	i->flags |= image_flag_DEFERRED;
    else
	i->flags &= ~image_flag_DEFERRED;

    if ( flags & image_find_flag_FORCE )
        i->flags &= ~image_flag_BLACKLIST;

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
    {
	i->flags |= image_flag_DEFERRED;
    }

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

#ifdef STBWEB
os_error *image_expire(image i)
{
    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    IMGDBGN(("image: expire %p\n", i));

    access_set_header_info(i->url, 1, 0, 0, 0); /* date > expires */

    return NULL;
}
#endif

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
	frame_rec *rec = i->frame + (i->cur_frame < 0 ? 0 : i->cur_frame);

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

	IMGDBGN(("image_info_frame(): box %d,%d,%d,%d\n", box->x0, box->y0, box->x1, box->y1));
	IMGDBGN(("image_info_frame(): flags=0x%x\n", flags ? *flags : 0));
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
    int ex = 0, ey = 0, l2bpp = 0;
    /*int lbit;*/

    IMGDBGN(("image_info: im%p\n", i));

    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	return makeerror(ERR_BAD_IMAGE_HANDLE);
    }

    if (width || height || bpp)
    {
	MemCheck_checking checking;

	flexmem_noshift();

#ifndef BUILDERS
	if (i->plotter == plotter_SPRITE)
	{
	    if (i->id.tag == sprite_id_name)
	    {
		IMGDBGN(("image_info: areap %p id '%s'\n", i->areap, i->id.s.name));
		ep = sprite_select_rp(*(i->areap), &(i->id), (sprite_ptr *) &sph);
	    }
	    else
	    {
		IMGDBGN(("image_info: add %p\n", i->id.s.addr));
		sph = (sprite_header *) i->id.s.addr;
	    }

	    if (ep == NULL && sph)
	    {
		checking = MemCheck_SetChecking(0, 0);

		IMGDBGN(("image_info: sprite is at 0x%p, mode value is 0x%x\n", sph, sph ? sph->mode : 0));

		ex = bbc_modevar(sph->mode, bbc_XEigFactor);
		ey = bbc_modevar(sph->mode, bbc_YEigFactor);
		l2bpp = bbc_modevar(sph->mode, bbc_Log2BPP);

		MemCheck_RestoreChecking(checking);
	    }
	}
	else
#endif
	{
	    ex = ey = 1;
	    l2bpp = 5;
	}

	if (ep)
	{
	    usrtrc( "Error: %s\n", ep->errmess);
	}
	else
	{
	    if (width)
		*width = i->width << ex;
	    if (height)
		*height = i->height << ey;
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

    IMGDBGN(("Returning, width=%d, height=%d, flags=0x%x\n", width ? *width : 0, height ? *height : 0, flags ? *flags : 0));

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
    /* pdh: cope with being given New Sprites on 3.1 */
    if ( i->width == 0 || i->height == 0 )
    {
        image_set_error(i);
        return;
    }

    if ((i->flags & (image_flag_REALTHING|image_flag_ERROR|image_flag_DEFERRED)) == image_flag_REALTHING && (w != -1 && h != -1))
    {
        /* pdh 2 Jul 97: like this to get it right for Mode 12 sprites */
        facs->xmag = w*2;
        facs->xdiv = i->width << bbc_modevar( -1, bbc_XEigFactor );
        facs->ymag = h*2;
        facs->ydiv = i->height << bbc_modevar( -1, bbc_YEigFactor );
    }
    else
    {
	fixup_scale(facs, scale_image);
    }
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
	/* DAF: under unix might not get initialised, so mm_malloc => mm_calloc */
	pt = image_alloc(16);
	if (!pt)
	    ep = makeerror(ERR_NO_MEMORY);

	if (!ep)
	{
	    id.tag = sprite_id_addr;
	    id.s.addr = sph;

	    ep = wimp_readpixtrans(area, &id, facs, pt);
	}

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
    int log2bpp = bbc_modevar( -1, bbc_Log2BPP );

#if 1
    /* If in a deep mode or on 3.6 or later, make a New Sprite */
    /* Otherwise, make an Old Sprite */

    if ( spriteextend_version >= 99 || log2bpp > 3 )
        mode = build_mode_number( log2bpp );
    else
        mode = 25+log2bpp;  /* Modes 25, 26, 27, 28 */
#else
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
#endif

    IMGDBGN(("img: current sprite mode %08x\n", mode));

    return mode;
}

/*
 * Calculate the size of sprite area needed for the
 * sprite area, header, image and mask
 */

static int image_size_needed(int c_w, int c_h, int bpp, image_cache_t mask)
{
    int data_size = ((c_w*bpp + 31)/32)*4*c_h;
    int mask_size = 0;

    if (mask != image_cache_SOLID)
    {
	int mask_data_size;
	if ( spriteextend_version >= 99 || bpp >= 16)	/* mask bpp = 1 bpp */
	    mask_data_size = ((c_w + 31)/32)*4*c_h;
	else						/* mask bpp = image bpp */
	    mask_data_size = data_size;

	if (mask == image_cache_MASK_SEPARATE)
	    mask_size = sizeof(sprite_header) + data_size + mask_data_size;
	else
	    mask_size = mask_data_size;
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

static os_error *image_init_cache_sprite(image i, int w, int h, int limit, image_cache_t mask, sprite_id *id_out)
{
    os_error *e;
    sprite_id id;
    int bpp = 1 << bbc_modevar(-1, bbc_Log2BPP);
    int size, mode;

    size = image_size_needed(w, h, bpp, mask);
#if ADD_PALETTE
    size += 2*4*256;
#endif
    IMGDBG(("image: build cache size %d limit %d mask %d dims %dx%d\n", size, limit, mask, w, h));

    if (size > limit)
	return makeerror(ERR_NO_MEMORY);

    /* allocate and init sprite area */
    if (flex_alloc((flex_ptr) &(i->cache_area), size + FLEX_FUDGE) == FALSE)
    {
        IMGDBG(("im%p: flex_alloc FAILED in init_cache_sprite\n", i ));
#ifndef STBWEB
        image_memory_panic();
#endif
/*         image_flush( i, image_find_flag_DEFER ); */
/*         mm_can_we_recover(FALSE); */
	return makeerror(ERR_NO_MEMORY);
    }

    sprite_area_initialise(i->cache_area, size);

    /* create sprite */
    /* 24/10/96: add palette as this seems to fix some crashes (plotting 1bit+palette into 8bit */
    mode = get_mode_number();
    e = sprite_create(i->cache_area, CACHE_SPRITE_PLOT_NAME, sprite_nopalette, w, h, mode);

#if ADD_PALETTE
    /* 14/1/97: think crashes were different problem so take out palette */
    if (!e && bbc_modevar(-1, bbc_Log2BPP) <= 3)
    {
	int buf[256];
	IMGDBGN(("image: add palette to sprite\n"));
	e = (os_error *)_swix(ColourTrans_ReadPalette, _INR(0,4), -1, -1, buf, sizeof(buf), 0);
	if (!e) e = (os_error *)_swix(ColourTrans_WritePalette, _INR(0,4), i->cache_area, CACHE_SPRITE_PLOT_NAME, buf, 0, 0);
	IMGDBGN(("image: added palette to sprite error '%s'\n", e ? e->errmess : ""));
    }
#endif

    i->cache_mask = 0;
    if (!e && mask != image_cache_SOLID)
    {
	if (mask == image_cache_MASK_SEPARATE)
	{
	    /* Create another sprite, with mask */
	    id.s.name = CACHE_SPRITE_BUILD_NAME;
	    e = sprite_create(i->cache_area, CACHE_SPRITE_BUILD_NAME, sprite_nopalette, w, h, mode);
	}
	else
	{
	    id.s.name = CACHE_SPRITE_PLOT_NAME;
	}

	id.tag = sprite_id_name;
	if (!e)
	    e = sprite_create_mask(i->cache_area, &id);

	if (!e)
	{
	    /* Save the value needed to set a mask value (1 if new style sprite) */
 	    i->cache_mask = mode > 255 ? 1 : (1 << bpp) - 1;

	    image_animation_clear_cache(i); /* must be after cache_mask is set */

	    IMGDBG(("animation: add mask cache bpp %d mask %d\n", bpp, i->cache_mask));
	}
    }

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
	    /* DAF: under unix might not get initialised, so mm_malloc => mm_calloc */
	    *pixtrans = image_alloc(16);
	    if (*pixtrans)
		*table_type = pixtrans_NARROW;
	    else
	    {
		e = makeerror(ERR_NO_MEMORY);
		*table_type = pixtrans_UNKNOWN;
	    }
	}

	if (!e)
	{
	    id.tag = sprite_id_addr;
	    id.s.addr = sptr;

	    e = wimp_readpixtrans(area, &id, factors ? factors : &dummy_f, pixtrans ? *pixtrans : dummy_p);
	}
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
	    if ( spriteextend_version >= 99 )
		r.r[5] |= (1 << 4);   /* SJM: allow wide tables */

	    e = os_swix(ColourTrans_GenerateTable, &r);
	    if (!e)
	    {
		table_size = r.r[4];

		if (r.r[4])
		{
		    /* DAF: under unix might not get initialised, so mm_malloc => mm_calloc */
		    *pixtrans = image_alloc(table_size);
		    if (!*pixtrans)
		    {
			*table_type = pixtrans_UNKNOWN;

			e = makeerror(ERR_NO_MEMORY);
		    }
		    else
		    {
			*table_type = (table_size >> (sprite_bpp)) > 1 ? pixtrans_WIDE : pixtrans_NARROW;

			r.r[4] = (int) (long) *pixtrans;
			e = os_swix(ColourTrans_GenerateTable, &r);
		    }
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

    /* pdh: we must optimise this ourselves, because 3.5 SpriteExtend
     * doesn't do it for us */

    if ( pixtrans
         && *table_type == pixtrans_NARROW
         && sprite_bpp <= 8 )
    {
        /* Test for 1:1 pixtrans case */
        int i;
        int ncol = 1 << sprite_bpp;
        BOOL ok = TRUE;

        for ( i=0; i < ncol; i++ )
        {
            if ( (*pixtrans)[i] != i )
            {
                ok = FALSE;
                break;
            }
        }

        if ( ok )
        {
            mm_free( *pixtrans );
            *pixtrans = NULL;
            *table_type = pixtrans_NONE_NEEDED;
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
    bpp = 1 << bbc_modevar(-1, bbc_Log2BPP);

    i_w = i->width*scale_image/100;
    if (i_w < 1)
	i_w = 1;
    i_h = i->height*scale_image/100;
    if (i_h < 1)
	i_h = 1;

    if (i_w > i_h)
    {
	c_w = i_w * cceil(128, i_w);
	c_h = i_h * cceil(MAX_CACHE_BYTES*8, i_h*c_w*bpp);
    }
    else
    {
	c_h = i_h * cceil(128, i_h);
	c_w = i_w * cceil(MAX_CACHE_BYTES*8, i_w*c_h*bpp);
    }

    if (i->cache_area == NULL)
    {
	IMGDBGN(("img: scaled input %dx%d cache %dx%d\n", i_w, i_h, c_w, c_h));

	/* Allocate memory and init the blank sprite
	 * If not already decompressed then must cache no matter how big it is
	 */
	e = image_init_cache_sprite(i, c_w, c_h,
				    i->plotter == plotter_SPRITE ? ABS_MAX_CACHE_BYTES : INT_MAX,
				    image_cache_SOLID, &id);
    }
    else
    {
	id.tag = sprite_id_name;
	id.s.name = CACHE_SPRITE_PLOT_NAME;

	e = NULL;
    }

    flexmem_noshift();

    /* do main plot into cache sprite */
    i_w_os = c_w_os = i_h_os = c_h_os = 0;
    if (!e)
    {
        sprite_state state;
	int *save_area;
	int save_size;

	IMGDBGN(("image: cache output to sprite %p\n", id.s.name));

	save_area = NULL;
	e = sprite_sizeof_spritecontext(i->cache_area, &id, &save_size);
	if (!e)
	{
	    save_area = image_alloc(save_size);
	    if (!save_area) e = makeerror(ERR_NO_MEMORY);
	}
 	if (!e) e = sprite_outputtosprite(i->cache_area, &id, save_area, &state);
        if (!e)
        {
            sprite_pixtrans *pt = NULL;
            image_pixtrans_type table_type;

            /* clear background if masked image */
            if (i->flags & image_flag_MASK)
            {
                int junk;
                colourtran_setGCOL(i->cache_bgcol, (1<<8) | (1<<7), 0, &junk);
                bbc_clg();
            }

	    if (i->plotter == plotter_SPRITE)
	    {
		/* get original sprite details */
		our_sph = image_get_sprite_ptr(*i->areap, &i->id, &our_id);

		e = image_get_trans(*i->areap, our_sph, &pt, &table_type, &facs);
	    }

	    IMGDBGN(("image: cache got trans error '%s'\n", e ? e->errmess : ""));

	    if (!e)
	    {
		/* DAF: is init on flags and scale_needed the right values? */
		int flags = 0, q, p;
		BOOL scale_needed = FALSE;

                i_w_os = i_w * i->dx;
                i_h_os = i_h * i->dy;

                c_w_os = c_w << bbc_modevar(-1, bbc_XEigFactor);
                c_h_os = c_h << bbc_modevar(-1, bbc_YEigFactor);

		if (i->plotter == plotter_SPRITE)
		{
		    /* adjust for display_scale - don't use image_scale as we may have corrected the values */
		    facs.xmag *= i_w;
		    facs.xdiv *= i->width;
		    facs.ymag *= i_h;
		    facs.ydiv *= i->height;

		    scale_needed = image_reduce_scales(&facs);

		    flags = (i->flags & image_flag_MASK ? 0x8 : 0) | (table_type == pixtrans_WIDE ? 0x20 : 0);
		}

		IMGDBGN(("image: cache do plot scale %d table %d @ %p\n", scale_needed, table_type, pt));

		for (q = 0; q < c_h_os; q += i_h_os)
	            for (p = 0; p < c_w_os; p += i_w_os)
		    {
			if (i->plotter == plotter_SPRITE)
			{
			    if (!scale_needed && table_type == pixtrans_NONE_NEEDED)
				sprite_put_given(*i->areap, &our_id, flags, p, q);
			    else
				sprite_put_scaled(*i->areap, &our_id, flags, p, q,
						  scale_needed ? &facs : NULL,
						  table_type == pixtrans_NONE_NEEDED ? NULL : pt);
			}
			else if (plotters[i->plotter].render)
			{
			    /* SJM: use i_w, i_h as scale_image is actually ignored by jpeg_render */
			    plotters[i->plotter].render(i, p, q, i_w, i_h, scale_image, 0, NULL, 0, 0);
			}
		    }
            }

	    IMGDBGN(("image: cache about to restore state\n"));

            /* discard pixtrans here */
	    mm_free(pt);
            sprite_restorestate(state);

	    IMGDBGN(("image: restored state\n"));
        }

	mm_free(save_area);
	IMGDBGN(("img: plot main\n"));
    }

    /* tidy up */
    if (e)
    {
	free_area(&i->cache_area);
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

    if ( (i->flags & (image_flag_FETCHED | image_flag_RENDERABLE | image_flag_ERROR)) == image_flag_FETCHED) /* SJM: added error */
    {
	ep = image_default_image(i, scale_image, &area, &id, &pt, &facs);

	table_type = pixtrans_NARROW;
    }
    else
    {
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

#if DEBUG
	MemCheck_RegisterMiscBlock(sph, sizeof(sprite_header));

	IMGDBGN(("Pix trans table. bpp=%d, mask %x, image %x (sprite header %x)\n",
		 bbc_modevar(sph->mode, bbc_Log2BPP),
		 sph->mask, sph->image,
		 sizeof(sprite_header)));

	MemCheck_UnRegisterMiscBlock(sph);
#endif

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
	frame_rec *rec = i->frame + (i->cur_frame < 0 ? 0 : i->cur_frame);

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

    IMGDBGN(("img: __render need scale %d table_type %d flags %x\n", need_scaling, table_type, sp_op));

    if (!need_scaling && table_type == pixtrans_NONE_NEEDED)
	ep = sprite_put_given(area, &id, sp_op, x, y);
    else
	ep = sprite_put_scaled(area, &id, sp_op, x, y, &facs, pt);

    IMGDBGN(("img: __rendered\n"));

    if (pt && pt != i->pt)	/* Only free it if you have not remembered it */
    {
	IMGDBGN(("About to free pixel translation table\n"));

	mm_free(pt);
    }
}

/* -------------------------------------------------------------------------------- */

/*
 * x,y is the position of the image on the page in screen coords
 * w,h is the size at which to plot the image in pixels, -1,-1 if default size
 * scale_image is a %age to scale the image to if w,h = -1,-1
 * plot_bg, handle is a function to plot the background to this page
 */

/* w, h, and i->width, i->height are all in nominal 90x90 pixels */

static void image_drawfile_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy)
{
    int trfm[6];

    if ((i->flags & image_flag_RENDERABLE) == 0)
	return;

    trfm[0] = (w << 16) / i->width;
    trfm[3] = (h << 16) / i->height;
    trfm[1] = trfm[2] = 0;
    trfm[4] = (x - i->offset_x*w/i->width) * 256;
    trfm[5] = (y - i->offset_y*h/i->height) * 256;
    _swix(DrawFile_Render, _INR(0,4), 0, i->data_area, flex_size(&i->data_area), &trfm, NULL);
}

static void image_jpeg_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy)
{
    sprite_factors facs;
    int flags;

    if ((i->flags & image_flag_RENDERABLE) == 0)
	return;

#if 0				/* If you trust the dpi in the jpeg... */
    facs.xmag = w*2 * i->xdpi;
    facs.xdiv = i->width * 180;
    facs.ymag = h*2 * i->ydpi;
    facs.ydiv = i->height * 180;
#else
    facs.xmag = w*2;
    facs.xdiv = i->width * i->dx;
    facs.ymag = h*2;
    facs.ydiv = i->height * i->dy;
#endif

    image_reduce_scales(&facs);

    IMGDBGN(("jpeg_render: size %dx%d at %dx%d dpi %dx%d flags %d\n", i->width, i->height, w, h, i->xdpi, i->ydpi, config_display_jpeg));

    /* SJM: workaround for OS bug with slow dithering on small images
     * the bug may be more extensive than this...
     * Note original workaround used plotted size of image. Must actually use original size of image!
     */
    flags = config_display_jpeg & 3;
    if (flags == 3 && (w == 1 || h == 1 || i->width == 1 || i->height == 1))
	flags = 0;

#if 0
    if (debug_get("IMGDBGN"))
    {
	IMGDBGN(("saving jpeg image %p-%p (%d)\n", i->data_area, (char *)i->data_area + i->data_size, i->data_size));
	frontend_fatal_error(_swix(OS_File, _INR(0,5), 10, "<NCFresco$Dir>.^.jpeg", 0xC85, 0, i->data_area, (char *)i->data_area + i->data_size));
    }
#endif

    _swix(JPEG_PlotScaled, _INR(0,5), i->data_area, x, y, &facs, flex_size(&i->data_area), flags);
}

static void image_sprite_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy)
{
    IMGDBGN(("Sprite area is 0x%p, image pointer is 0x%p, name is '%s' flags 0x%x\n",
	    *(i->areap), i->id.s.name, i->id.tag == sprite_id_name ? i->id.s.name : "<none>", i->flags));

    if (i->frame && i->cache_area)
    {
	if (i->cache_mask)
	    image_animation_render(i, x, y, w, h, scale_image, plot_bg, handle, ox, oy);

	image__render(i, x, y, w, h, scale_image);

	if (i->cache_mask == 0 && i->frame[i->cur_frame].removal == webremove_PREVIOUS)
	    image_animation_render_frame(i, image_frame_PLOT_IMAGE);
    }
    else
    {
	image__render(i, x, y, w, h, scale_image);
    }
}


/*
 * ox, oy is the screen coordinates of the window origin, corrected for the margins
 * x, y is the position to plot the image (bottom left)
 */

void image_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy)
{
    int plotter;

    if (i == NULL || i->magic != IMAGE_MAGIC)
    {
	usrtrc( "Bad magic\n");
	return;
    }

    IMGDBGN(("im%p: rendering at %d,%d plotter %d\n", i, x, y, i->plotter));

    if (w == 0 || h == 0)
	return;

    plotter = i->plotter;
    if ( (i->flags & (image_flag_FETCHED | image_flag_RENDERABLE)) == image_flag_FETCHED )
	plotter = plotter_SPRITE;

    if ( (i->flags & image_flag_REALTHING) == 0 )
	plotter = plotter_SPRITE;

    if (i->plotter == plotter_UNKNOWN)
	plotter = plotter_SPRITE;

    flexmem_noshift();

    if (plotters[plotter].render)
	plotters[plotter].render(i, x, y, w, h, scale_image, plot_bg, handle, ox, oy);

    flexmem_shift();
}

/* -------------------------------------------------------------------------------- */

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
	IMGDBGN(("img: have cache ok changed %d flags %x\n", changed, i->flags));

	area = i->cache_area;
	id.tag = sprite_id_name;
	id.s.name = CACHE_SPRITE_PLOT_NAME;

    	translate = FALSE;
    }
    else
    {
	changed = area != i->cache_area;
	IMGDBGN(("img: not have cache OK changed %d flags %x\n", changed, i->flags));

	/* If image is transparent then clear background first */
        if ((i->flags & (image_flag_MASK|image_flag_ERROR|image_flag_DEFERRED)) || i->plotter != plotter_SPRITE)
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
    if ((!translate || i->plotter == plotter_SPRITE) &&
	(i->flags & (image_flag_ERROR|image_flag_DEFERRED)) == 0 &&
	image_get_sprite_ptr(area, &id, &id))
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

/* 	    fixup_scale(&factors, scale_image); */
	    image_reduce_scales(&factors);
	}

	IMGDBG(("image_tile(): ptr %p translate %d pt %p type %d mask %d os_size %dx%d\n",
		id.s.addr, translate, pt, table_type, i->flags & image_flag_MASK ? 1 : 0,
		width, height));

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
	    }
    }

    IMGDBG(("image_tile(): end\n"));

    flexmem_shift();

    return changed;
}


void image_get_scales(image i, int *dx, int *dy)
{
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

    if (dx)
	*dx = i->dx;
    if (dy)
	*dy = i->dy;
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
    int *count;
    int *palette;
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

    count = image_alloc(sizeof(int)*256*2); /* moved buffers out of wimpslot */
    if (!count)
	return 0;

    flexmem_noshift();

    palette = count + 256;

    if (im->id.tag == sprite_id_name)
	sprite_select_rp(*(im->areap), &(im->id), (sprite_ptr *) &sphp);
    else
	sphp = (sprite_header *) im->id.s.addr;

    lbit = (sphp->mode > 255) ? 0 : sphp->lbit;
    rbit = sphp->rbit;

/*  for (i = 0; i < cols; i++) */
/* 	count[i] = 0; */

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
    r.r[3] = 256 * sizeof(int);
    r.r[4] = ((im->id.tag == sprite_id_name) ? 0 : 1);

    ep = os_swix(ColourTrans_ReadPalette, &r);

    flexmem_shift();

    pixcount = reds = greens = blues = 0;

    for (i=0; i < cols; i++)
    {
#if DEBUG > 2
 	IMGDBGN(("col %d, count %d, palette 0x%08x\n", i, count[i], palette[i]));
#endif
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

    mm_free(count);

    return (reds << 8) + (greens << 16) + (blues << 24);
}

int image_average_colour(image i)
{
    os_error *ep = NULL;
    sprite_header *sphp, sph;
    int l2bpp;
    int ave_col;

    IMGDBG(("Calculating average colour\n"));

    if (i == NULL || i->magic != IMAGE_MAGIC || i->plotter != plotter_SPRITE || (i->flags & image_flag_RENDERABLE) == 0)
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
    int *palette;
    sprite_header *sphp;
    int l2bpp;
    int cols;
    int best;
    int rr, gg, bb;
    os_regset r;
    os_error *ep = NULL;

    IMGDBG(("Finding colour closest to white\n"));

    palette = image_alloc(sizeof(int)*256);
    if (!palette)
	return 0;

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
	r.r[3] = 256 * sizeof(int);
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
	best = c | (c << 16);
    }
    else if (l2bpp == 5)
    {
	best = colour.word;
    }
    else
    {
	cols = 1 << (1 << l2bpp);

	best = find_closest_colour(colour.word, palette, cols);

	IMGDBG(("Best colour is 0x%x\n", best));

	while (l2bpp < 3)
	{
	    best |= (best << (1 << l2bpp));
	    l2bpp++;
	}

	IMGDBG(("Best colour byte is 0x%x\n", best));
    }

    mm_free(palette);

    return best;
}

/* -------------------------------------------------------------------------- */

static os_error *image_sprite_save_as_sprite(image i, char *fname)
{
    os_filestr fs;
    int *area;
    os_error *ep;

    area = (int*) *(i->areap);

    fs.action =10;		/* Save with file type */
    fs.name = fname;
    fs.loadaddr = FILETYPE_SPRITE;
    fs.start = ((int) (long) area) + 4;
    fs.end = ((int) (long) area) + *area;

    ep = os_file(&fs);

    return ep;
}

os_error *image_save_as_sprite(image i, char *fname)
{
    os_error *e = NULL;

    flexmem_noshift();

    if (plotters[i->plotter].save_as_sprite)
	e = plotters[i->plotter].save_as_sprite(i, fname);

    flexmem_shift();

    return e;
}

/* -------------------------------------------------------------------------- */

static void image_sprite_save_as_draw(image i, int fh, wimp_box *bb, int *fileoff)
{
    draw_objhdr obj;
    char buffer[12];
    sprite_header *sph;
    sprite_area *area;
    sprite_id id;
    os_regset r;
    os_error *ep = NULL;

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
	return;
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
}

#define file_save(f,s,e,t) _swix(OS_File,_INR(0,2)|_INR(4,5),10,f,t,s,e)

os_error *image_save_as_jpeg( image i, const char *fname )
{
    TASSERT(i);
    TASSERT(i->magic == IMAGE_MAGIC);

    if ( i->plotter != plotter_OSJPEG )
        return makeerror(ERR_NOT_IMPLEMENTED);

    if ( (i->flags & (image_flag_FETCHED|image_flag_RENDERABLE))
          != (image_flag_FETCHED|image_flag_RENDERABLE) )
        return makeerror(ERR_PAGE_NOT_WHOLE);

    IMGDBG(( "im%p: save_as_jpeg: 0x%08x +%x %s\n", i,
             (int)i->data_area, i->data_size, fname ));

    return file_save( fname, i->data_area, ((char*)i->data_area)+i->data_size,
                      FILETYPE_JPEG );
}

int image_preferred_save_filetype( image i )
{
    TASSERT(i);
    TASSERT(i->magic == IMAGE_MAGIC);

    if ( i->plotter == plotter_OSJPEG )
        return FILETYPE_JPEG;

    return FILETYPE_SPRITE;
}

void image_jpeg_save_as_draw(image i, int fh, wimp_box *bb, int *fileoff)
{
}

void image_drawfile_save_as_draw(image i, int fh, wimp_box *bb, int *fileoff)
{
}

void image_save_as_draw(image i, int fh, wimp_box *bb, int *fileoff)
{
    flexmem_noshift();

    if (plotters[i->plotter].save_as_draw)
	plotters[i->plotter].save_as_draw(i, fh, bb, fileoff);

    flexmem_shift();
}

/* -------------------------------------------------------------------------- */

/*
 * If the main image has a mask, or
 * if any frame both has a mask and wants to be cleared to the background
 * then we need a mask.

 * 15/10/96: Changed this to use a mask if any of them redraw to the baclground
 * to cope with the case where you have a frame smaller than the original being
 * plotted over the cleared background. This is effectively a masked image.

 */

static image_cache_t image_animation_need_mask(image i)
{
    frame_rec *rec;
    int f;

    IMGDBG(("anim: checking for mask\n"));

    if (i->flags & image_flag_MASK)
	return image_cache_MASK_SEPARATE;

    rec = i->frame;

    /* if the first frame is not the size of the whole thing it needs a mask */
    if (rec->x != i->width || rec->y != i->height)
	return image_cache_MASK_SEPARATE;

    /* if any frame wipes to background then it may need a mask */
    for (f = 0; f < i->frames; f++, rec++)
	if (rec->removal == webremove_BACKGROUND/*  && rec->mask */)
	    return image_cache_MASK_SEPARATE;

    IMGDBG(("anim: no mask\n"));

    return image_cache_SOLID;
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
	int w = (i->width * i->dx) >> bbc_modevar(-1, bbc_XEigFactor);
	int h = (i->height * i->dy) >> bbc_modevar(-1, bbc_YEigFactor);

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

    IMGDBGN(("animation: render frame %d flags %x mask %d\n", i->cur_frame, flags, rec->mask));

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

	IMGDBGN(("animation: plot to image %p '%.12s' area %p sprite ptr %p mask %d table %d\n", i->cache_area, cache_id.s.name, *i->areap, our_id.s.addr, rec->mask, table_type));

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
	e = (os_error *)_swix(OS_SpriteOp, _INR(0,3) | _OUTR(0,3), 0x13d, i->cache_area, cache_id.s.name, NULL, &state.r[0], &state.r[1], &state.r[2], &state.r[3]);
/* 	e = sprite_outputtomask(i->cache_area, &cache_id, NULL, &state); */
	if (!e)
	{
	    if (flags & image_frame_CLEAR_MASK)
	    {
		e = (os_error *)_swix(OS_SetColour, _INR(0,1), 0, 0);	/* set fg colour */
		if (!e) e = bbc_rectanglefill(x, y, rec->x*2, rec->y*2);
	    }
	    else
	    {
		e = (os_error *)_swix(OS_SetColour, _INR(0,1), (1<<4) + 0, i->cache_mask); /* set bg colour */
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

    IMGDBGN(("animation: render background fn 0x%p handle 0x%p scroll %d,%d\n", plot_bg, handle, scx, scy));

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
    if (!e)
    {
	save_area = image_alloc(save_size);
	if (!save_area) e = makeerror(ERR_NO_MEMORY);
    }

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
	sprintf(buffer, "<NCFresco$Dir>.^.spr1.%02d", i->cur_frame);
	sprite_area_save(i->cache_area, buffer);
    }
#endif

    flexmem_shift();
}

static void image_startup_animation(image i);

extern void image_stop_animation( image i )
{
    i->repeats = 1;
    i->cur_repeat = 2;
}

static void image_animation_alarm(int at, void *h)
{
    image i = (image) h;
    int redraw = 1;
    frame_rec *rec;
    wimp_box box_1, box_2, box_u;

    /* if we'd cancelled the cache (on palette change) then just call startup again */
    if (i->cur_frame == -1)
    {
	IMGDBGN(("animation: restart\n"));

	image_issue_callbacks(i, image_cb_status_UPDATE_ANIM, NULL);

	image_startup_animation(i);

	return;
    }

    rec = i->frame + i->cur_frame;
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

    IMGDBGN(("animation: remove frame %d type %d box %d,%d %d,%d\n", i->cur_frame, rec->removal, box_1.x0, box_1.y0, box_1.x1, box_1.y1));

    /* increment the frame number */
    i->cur_frame++;
    if (i->cur_frame == i->frames)
    {
	i->cur_repeat++;

	/* Repeats==0 means loop indefinitely */
	if ( i->repeats > 0 && (i->cur_repeat >= i->repeats) )
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

	    IMGDBGN(("animation: add frame %d box %d,%d %d,%d\n", i->cur_frame, box_2.x0, box_2.y0, box_2.x1, box_2.y1));

	    if (coords_union(&box_1, &box_2, &box_u))
	    {
		IMGDBGN(("animation: redraw union box %d,%d %d,%d\n", box_u.x0, box_u.y0, box_u.x1, box_u.y1));
		image_issue_callbacks(i, image_cb_status_UPDATE_ANIM, &box_u);
	    }
	    else
	    {
		IMGDBGN(("animation: redraw separates\n"));
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
	IMGDBGN(("Setting animation alarm for %dcs from now.\n", rec->delay));

	alarm_set(alarm_timenow() + rec->delay, image_animation_alarm, i);
    }
}

static void image_startup_animation(image i)
{
    if (i->frames > 1)
    {
	os_error *e;

	IMGDBG(("Animation starting, %d frames, %d cycles.\n", i->frames, i->repeats));

	image_animation_dump_info(i);

	e = image_animation_init_cache(i);

	if (e) usrtrc( "animation: start error %x %s\n", e->errnum, e->errmess);

	IMGDBGN(("Setting animation alarm for %dcs from now.\n", i->frame->delay));

	alarm_set(alarm_timenow() + i->frame->delay, image_animation_alarm, i);
    }
}

/* -------------------------------------------------------------------------- */

/* eof images.c */
