/* -*-c-*- */

/* sprt2sprt.c */

/* 'Convert' a sprite file to a sprite in the same form as the WebImage library */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sprite.h"
#include "bbc.h"

#ifndef NEW_WEBIMAGE
#define NEW_WEBIMAGE	1
#endif

#if NEW_WEBIMAGE
static int dummy;
#else

#ifdef STBWEB_BUILD
#include "libs/webim_old/webimage.h"
#else
#include "../webimage/webimage.h"
#endif

#include "sprt2sprt.h"

#ifndef DEBUG
#define DEBUG 0
#endif

image_rec *sprt2sprite_info(int handle);
char *sprt2sprite_error(int handle);
BOOL sprt2sprite_iterate(int handle);
void sprt2sprite_exit(int handle);

extern char *sprt2sprite(getsrc_func getsrc, putdst_func putdst, seekdst_func seekdst, imagerec_func imr,
			void *user_get, void *user_put, BOOL block_detail)
{
    char buffer[1024];
    int got;
    char *err = NULL;
    int l2bpp, lbit;
    image_rec im;
    int so_far = 0;
    int sp_start;
    int needed;
    sprite_header *sph;

    while (so_far < 12)
    {
	so_far += getsrc(buffer + so_far, (12 - so_far), user_get);
    }
    sp_start = *((int*) (buffer+4)) - 4;

#if DEBUG
    fprintf(stderr, "Sprite starts %d bytes into the file\n", sp_start);
#endif

    while (so_far < sp_start)
    {
	so_far += getsrc(buffer,
			 (sp_start - so_far) > sizeof(buffer) ? sizeof(buffer) : (sp_start - so_far),
			 user_get);
    }

#if DEBUG
    fprintf(stderr, "Moved %d bytes into the file (starts at %d)\n", so_far, sp_start);
#endif

    while (so_far < (sp_start + sizeof(sprite_header)) )
    {
	so_far += getsrc(buffer + so_far - sp_start,
			 sizeof(sprite_header) - (so_far - sp_start),
			 user_get);
    }

#if DEBUG
    fprintf(stderr, "Read header\n");
#endif

    sph = (sprite_header *) buffer;

    l2bpp = bbc_modevar(sph->mode, bbc_Log2BPP);

    lbit = (sph->mode > 255) ? 0 : sph->lbit;

    im.x = (((sph->width+1) << 5) - lbit - (31 - sph->rbit)) >> l2bpp;
    im.y = sph->height+1;
    im.depth = 1 << l2bpp;
    im.palette = (sph->image != sizeof(sprite_header));
    im.size = sph->next;
    im.interlaced = 0;
    im.mask = (sph->image != sph->mask);

#if DEBUG
    fprintf(stderr, "Image is %d by %d, %dbpp, %d bytes\n", im.x, im.y, im.depth, im.size);
#endif

    needed = so_far + im.size - sizeof(sprite_header);

#if DEBUG
    fprintf(stderr, "About to tell the client\n");
#endif

    if ( ! imr(&im, user_put))
    {
	err = "Client abouted at imagerec_func";
	goto done;
    }

#if DEBUG
    fprintf(stderr, "Writing sprite header\n");
#endif

    putdst(buffer, sizeof(sprite_header), user_put);

#if DEBUG
    fprintf(stderr, "Copying the (%d bytes of) data\n", needed);
#endif

    do
    {
	got = getsrc(buffer, (needed - so_far) > sizeof(buffer) ? sizeof(buffer) : (needed - so_far), user_get);
	if (got > 0)
	{
	    putdst(buffer, got, user_put);
	    so_far += got;
	}
    } while (got != -1 && so_far < needed);

    if (got == -1)
	err = "Data get function failed";

 done:

    return err;
}

#endif
