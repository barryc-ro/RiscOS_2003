/* -*-c-*- */

/* Image cache for images in documents */

#ifndef __images_h
#define __images_h

#ifndef __wimp_h
#include "wimp.h"
#endif

#define image_cb_status_WORLD	        (-1) /* The size hasn't changed just turn world */
#define image_cb_status_NOACTION	0 /* The size has changed but there is nothing to draw */
#define image_cb_status_UPDATE		1 /* Update the image without clearing */
#define image_cb_status_REDRAW		2 /* Redraw the image area from scratch */
#define image_cb_status_REFORMAT	3 /* Image size MAY have change reformat and redraw */
#define image_cb_status_CACHE		4 /* Image loaded from cache or icontype: */
#define image_cb_status_UPDATE_ANIM	5 /* update due to an animation */

/*
 * Added a box to the callback. If non null then this box is the area of the
 * image, in os coords, as offset from the bottom LH corner of the image,
 * that has changed.
 */

typedef void (*image_callback)(void *h, void *i, int status, wimp_box *box);

typedef struct _image_info *image;

os_error *image_init(void);
os_error *image_tidyup(void);

void image_palette_change(void);

/* Call this function when memory gets very low.  It returns TRUE if it has freed some space. */
int image_memory_panic(void);

/* Test if we can take this file type */
BOOL image_type_test(int ft);

/* Find an image */
os_error *image_find(char *url, char *ref, int flags, image_callback cb, void *h, wimp_paletteword bg, image *result);
#define image_find_flag_DEFER	   (1 << 0) /* Make the image structure but don't start fetching the image */
#define image_find_flag_CHECK_EXPIRE (1 << 1) /* if coming from cache check expiry date first */
#define image_find_flag_URGENT     (1 << 2) /* Add to head of queue, not tail */
#define image_find_flag_NEED_SIZE  (1 << 3) /* We need the image size */
#define image_find_flag_FORCE      (1 << 4) /* Fetch even if blacklisted */

/* Loose an image */
os_error *image_loose(image i, image_callback cb, void *h);

/* Flush fetched image.  Dispose of the image and then find it again.
 * May cause lots of windows to be redrawn.  If the image was defered
 * this may force the image to get fetched if the defer dlag is not
 * set. */
os_error *image_flush(image i, int flags);

/* Defer an image. Doesn't abort the fetch if it has already started.
 */
void image_defer( image i );

/* If we are flushing all the images on a page we only want to issue
 * tht flush once for each image.  These two functions let us mark
 * images for flushing and then flush them all in one go.  The is no
 * drawback to marking an image more than once */
os_error *image_mark_to_flush(image i, int flags);
os_error *image_flush_marked(void);

os_error *image_stream(char *url, int ft, int *already, image *result);
os_error *image_stream_data(image i, char *buffer, int len, int update);
os_error *image_stream_end(image i, char *cfile);

/* An image can be in one of four fetch states: waiting, fetching,
 * fetched and error.  The linked list of images is kept in order and
 * when an opertunity arrises the earliest image in the list that is
 * waiting gets fetched.  When the completed function is called first
 * the image is marked either fetched or error and second the list is
 * checked for any remaining images to be fetched.  The image is then
 * translated for display and marked renderable if this is successful.
 * When a request is made inittially a test is made to see if the file
 * is in the cache so that already cached images come up instantly.
 */

typedef int image_flags;
#define image_flag_FETCHED	0x01	/* Set only once we have the object */
#define image_flag_RENDERABLE	0x02	/* Indicates we can render the object onto the screen */
#define image_flag_WAITING	0x04	/* Waiting for a chance to be fetched */
#define image_flag_ERROR	0x08	/* Fetch tried and failed */
#define image_flag_DEFERRED	0x10	/* User requested deferred fetching */
#define image_flag_TO_RELOAD	0x20	/* Marked for reload */
#define image_flag_BLACKLIST    0x40    /* Was on advert blacklist */

#define image_flag_INTERLACED	0x00010000	/* Image is interlaced */
#define image_flag_MASK		0x00020000	/* Image has a mask */
#define image_flag_ANIMATION	0x00040000	/* Image has multiple frames */
#define image_flag_USE_LOGICAL	0x00080000	/* logical size is different from first frame size */
#define image_flag_LOAD_AT_END	0x00100000	/* we missed the start of the stream, wait till the end and load it all in one */

#define image_flag_FETCHING	0x08000000	/* for purposes of being_fetched count only */

#define image_flag_STREAMING	0x10000000	/* Image is being streamed in from parser code */
#define image_flag_NO_BLOCKS	0x20000000	/* Don't do image blocking, we have all the data */
#define image_flag_REALTHING	0x40000000	/* Image is not a dummy 'question mark' */
#define image_flag_CHANGED	(~0x7fffffff)	/* Image has changed since flag was last cleared */

os_error *image_file_info(image i, int *load, int *exec, int *size);
os_error *image_info(image i, int *width, int *height, int *bpp,
		     image_flags *flags, int *filetype, char **url);
os_error *image_info_frame(image i, wimp_box *box, int *bpp, image_flags *flags);
os_error *image_data_size(image i, image_flags *flags, int *data_so_far, int *data_size);

/* This function is as defined in interface.h */
typedef int (*image_rectangle_fn)(wimp_redrawstr *r, void *h, int update);

/* Plot the image on the screen at a given position at a given size (-1's mean use actual sprite size) */
void image_render(image i, int x, int y, int w, int h, int scale_image, image_rectangle_fn plot_bg, void *handle, int ox, int oy);

/* Tile an image across an area of the screen, with the origin at the given position
 * returns true if tile sprite has changed
 */
int image_tile(image i, int x, int y, wimp_box *bb, wimp_paletteword bgcol, int scale_image);

os_error *image_save_as_sprite(image i, char *fname);

os_error *image_save_as_jpeg(image i, const char *fname);

int image_preferred_save_filetype( image i );

/* Find out the X and Y scaling factors for the given image */
void image_get_scales(image i, int *dx, int *dy);

/* convert an os coordinate offset into corrected pixel coords */
void image_os_to_pixels(image im, int *px, int *py, int scale_image);

/* Work out the average colour of an image, return a colour word (0xBBGGRR00) */
int image_average_colour(image i);

/* Write out a draw sprite object containing the image */
void image_save_as_draw(image i, int fh, wimp_box *box, int *fileoff);

extern BOOL image_type_test(int ft);

/* forcibly set the expiry time of an image to 0 so that it will be refetched */
extern os_error *image_expire(image i);

/* force the cached version (for tiles and animations) and translation tables to be refetched */
extern void image_uncache_info(image i);

/* Stop the animation */
extern void image_stop_animation( image i );

#endif /* __images_h */
