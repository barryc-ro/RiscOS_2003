/* webimage.h
 * public function prototypes for webimage library.
 */

#ifndef _webimage_h
#define _webimage_h

#ifndef BOOL
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif


/* Structure for each frame of a GIF89 animation */
typedef struct
{
  int		x;		/* X physical size of image, in pixels */
  int		y;		/* Y physical size of image, in pixels */
  int		depth;          /* Bits per pixel (1..24) */
  int		size;           /* Max size of sprite inc. headers + palette */
  BOOL		mask;		/* TRUE if mask in output sprite */
  int		delay;		/* Delay in cs before displaying next frame */
  int		removal;	/* Type of removal for this image (see below) */
  int		x_off;		/* X offset into logical size of image (0=left edge) */
  int		y_off;		/* Y offset into logical size of image (1=top edge) */
} frame_rec;

/* Defined GIF89 image removal types */
#define webremove_NODISPOSE	0	/* No action required */
#define webremove_NONE		1	/* Do not dispose. Leave graphic in place */
#define webremove_BACKGROUND	2	/* Restore to background colour */
#define webremove_PREVIOUS	3	/* Restore to area 'underneath' the graphic */


/* Structure for information on translation process. A copy of this should be made
 * by the calling application (including the frame_rec array, if defined) since the
 * library copy will only exist for the duration of the image translation.
 */
typedef struct
{
  /* Input image details */
  int		format;		/* Image format, (-2..+3) defined below */
  int		filetype;	/* RISC OS equivalent filetype for this image format */
  char		name[8];	/* Textual name of image format (eg, 'JPEG') */
  /* Output sprite details */
  int		x;		/* X physical size of image, in pixels */
  int		y;		/* Y physical size of image, in pixels */
  int		depth;          /* Bits per pixel (1..24) */
  BOOL		palette;	/* TRUE if palette in output sprite */
  int		size;           /* Max size of output sprite area inc. headers + palette */
  BOOL		interlaced;	/* TRUE if interlaced source image (or progressive JPEG) */
  BOOL		mask;		/* TRUE if mask in output sprite */
  /* GIF89 animation */
  int		x_logical;	/* X logical size of image, in pixels */
  int		y_logical;	/* Y logical size of image, in pixels */
  int		frames;		/* Number of frames in animation, or 1 for still image */
  int		repeat;		/* Number of iterations for animation (0 = infinite) */
  unsigned int	background;	/* Background as a true 0xRRGGBB00 colour (for removal) */
  frame_rec 	*frame;		/* Array of frame_rec details (or NULL) */
  /* JPEG */
  BOOL          jfif;           /* JFIF header included in file */
} image_rec;


/* User-supplied fn that gets up to 'buflen' bytes from source image stream.
 * The return code is the number of bytes actually read into the buffer. If no
 * bytes could be read due to a fatal error or End Of File, then -1 should be
 * returned, otherwise getsrc_func() will be repeatedly called until 'buflen'
 * bytes have been fetched.
 *
 * If this fn has temporarily run out of data (because it is waiting on a remote
 * process) it should set *flush to TRUE, to indicate that it is worth the image
 * library flushing any data it has buffered to the output data stream (although
 * the library may ignore the contents of this flag for some image formats).
 */
typedef int (*getsrc_func) (char *buf, int buflen, void *user_get, BOOL *flush);

/* User-supplied fn that puts 'buflen' bytes to destination image data stream. */
typedef void (*putdst_func) (char *buf, int buflen, void *user_put);

/* User-supplied fn that seeks to 'pos' bytes from the destination stream start */
typedef void (*seekdst_func) (int pos, void *user_put);

/* User-supplied fn that passes 'image_rec' to the calling program.
 * Returns FALSE if calling program doesn't like this size of image, and wants to
 * abort.
 *
 * Note that this function will always be called before any attempt to write 
 * the output sprite via putdst() is made. Note also that for some image formats
 * (eg, GIF animations) this function will be called multiple times, one for 
 * each frame of the animation as it it fetched from the input stream.
 */
typedef BOOL (*imagerec_func) (image_rec *irec, void *user_put);


/* User flags word, for control of output sprite.
 *   bit 0 = behaviour of interlaced GIFs, PNGs & progressive JPEGs (ignored otherwise)
 *         = 0, render line-by-line as normal
 *         = 1, render in 'block mode' where lines of 8,4,2, and 1 are displayed
 *              or render in multiple passes (Progressive-JPEG)
 *   bit 1 = sprite colour depth for JPEGs, TIFFs and PiNGs (ignored otherwise)
 *         = 0, always produce FS-dithered 8bpp sprites with fixed desktop palette
 *         = 1, always produce 16bpp sprites for RISC OS 3.50 or later
 *   bits 2..31, reserved, must be 0.
 */
#define webimage_BLOCKDETAIL	0x00000001
#define webimage_DEEPSPRITE	0x00000002


/* Defined image format types.
 */
#define webformat_NOTSET	-2	/* Not yet determined by library */
#define webformat_UNKNOWN	-1      /* Not recognised by library */
#define webformat_XBM		0       /* UNIX .xbm ASCII encoded monochrome images */
#define webformat_GIF           1	/* CompuServe GIF87/89 images & animations */
#define webformat_JPEG          2	/* JPEG JFIF baseline & progressive encoding */
#define webformat_PNG           3       /* PNG 'PiNG' images */
#define webformat_SPRITE        4       /* RISCOS 'Sprite' images */
#define webformat_TIFF		5	/* TIFF images */


/*
 * CompuServe GIF to Sprite translation routines.
 * GIF images are LZW compressed, with between 1 and 8 bits of colour depth.
 * Output sprites are of an appropriate number of bits depth, with mask and
 * palette set as required.
 */

/*
 * UNIX .xbm to Sprite translation routines.
 * .xbm images are ASCII encoded monochrome images. The output sprite file 
 * always has a mask & palette.
 */

/*
 * JPEG JFIF to Sprite translation routines.
 * JPEG images are lossy, with between 1 and 24 bits of colour depth.
 * The output sprites depend largely on the 'flags' word on entry.
 *
 * Based on Release 6beta1 of the Independant JPEG Group's code,
 * (C) 1991-1995, Thomas G. Lane.
 */

/*
 * PNG to Sprite translation routines.
 * PNG 'PiNG' images are lossless, with between 1 and 48 bits of colour depth,
 * or up to 16 bits of greyscale depth.
 * The output sprites depend largely on the 'flags' word on entry.
 *
 * Based on Release 0.87-beta of the PNG library (C) 1996 Guy Eric Schalnat, Group 42, Inc.
 * and release 1.0 of the zLib library (C) 1996 Jean-loup Gailly and Mark Adler.
 */

/* 
 * RISCOS Sprite to Sprite copy routines.
 * RISCOS Sprite images are lossless, with between 1 and 24 bits of colour depth,
 * or up to 8 bits of greyscale depth.
 * The output sprite is an exact copy of the input data stream.
 */ 

/* 
 * TIFF to Sprite translation routines.
 * TIFF images are lossless, with between 1 and 24 bits of colour depth. Images
 * can be uncompressed, or compressed with one of a number of schemes. eg;
 *   CCITT Group 3 & 4 fax, Macintosh PackBits, LZW, ThunderScan 4-bit RLE,
 *   NeXT 2-bit RLE, 6.0-style JPEG DCT, Deflate, Pixar log-format.
 * This translation code only copes with uncompressed, PackBits and LZW.
 * The output sprites depend largely on the 'flags' word on entry.
 *
 * Based on Release 3.4beta032 of the libtiff library (c) 1988-1996 Sam Leffler.
 */ 
  
/* Run translation.
 *
 * Inputs are user functions to read the source image, write the output (Sprite),
 * and seek to a position in the output stream. This version of the library
 * is NOT suitable for OUTPUT streams that cannot support non-sequential access
 * (eg, a network TCP stream).
 *
 * The function 'irec' passes an image_rec struct pointer when enough of the
 * source image has been processed to obtain that information.
 *
 * The user handles 'user_get' and 'user_put' are passed to the user functions
 * (see above). The user flags word controls various options on the processing
 * of the output sprite (see above #define's).
 * 
 * A buffer for any error (at least 256 chars) is passed on entry.
 * On exit, an error string is returned, or NULL if there was no error.
 */
extern char *img2sprite(getsrc_func getsrc, putdst_func putdst, seekdst_func seekdst,
			imagerec_func irec, void *user_get, void *user_put, int flags,
			char *error);

/* Initialise webimage library (sets DLL path).
 *
 * Set directory path to load image DLLs from. Defaults to "<Fresco$Dir>.RMStore.".
 */
extern void imginit(char *path);

/* Flush inactive DLLs from memory ('grum reaper').
 */
extern void imgreaper(void);

#endif

