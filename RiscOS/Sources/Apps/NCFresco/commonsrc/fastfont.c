/* fastfont.c */

#include <string.h>

#include "os.h"
#include "colourtran.h"
#include "font.h"

#include "fastfont.h"

typedef struct {
    char *buffer;
    char *ptr;
    char *end;
    int  font;
    wimp_paletteword paper;
    wimp_paletteword pen;
    int  x, y;
    int  startx, starty;
    int  flags;
    BOOL started;
} fastfont_state;

static fastfont_state ffs = { 0 };
static int nOpens = 0;

#define SPACE (ffs.end - ffs.ptr - 1)
#define MAKESPACE(n) if ( ffs.ptr + n >= ffs.end ) fastfont_flush();

void fastfont_open( char *buffer, int nBufSize )
{
    nOpens++;
    if ( nOpens == 1 )
    {
        ffs.buffer = buffer;
        ffs.end = buffer + nBufSize;
        ffs.ptr = buffer;
        ffs.font = -1;
        ffs.paper.word = -1;
        ffs.pen.word = -1;
        ffs.started = FALSE;
    }
}

void fastfont_set_colours( wimp_paletteword paper, wimp_paletteword pen,
                           BOOL antialias )
{
    int maxcols = antialias ? 14 : 0;

    if ( paper.word != ffs.paper.word || pen.word != ffs.pen.word )
    {
        if ( ffs.buffer )
        {
            MAKESPACE(8);
            *ffs.ptr++ = 19;
            *ffs.ptr++ = paper.word>>8;
            *ffs.ptr++ = paper.word>>16;
            *ffs.ptr++ = paper.word>>24;
            *ffs.ptr++ = pen.word>>8;
            *ffs.ptr++ = pen.word>>16;
            *ffs.ptr++ = pen.word>>24;
            *ffs.ptr++ = maxcols;
        }
        else
        {
            int fh = 0;
            colourtran_setfontcolours( &fh, &pen, &paper, &maxcols );
        }

        ffs.paper = paper;
        ffs.pen = pen;
    }
}

void fastfont_setfont( int font )
{
    if ( font != ffs.font )
    {
        if ( ffs.buffer )
        {
            MAKESPACE(2);
            *ffs.ptr++ = 26;
            *ffs.ptr++ = font;
        }
        else
            font_setfont( font );

        ffs.font = font;
    }
}

static void fastfont__move( int ch, int diff )
{
    if ( !diff )
        return;
    *ffs.ptr++ = ch;
    *ffs.ptr++ = diff;
    *ffs.ptr++ = (diff>>8);     /* signed shifts here */
    *ffs.ptr++ = (diff>>16);
}

void fastfont_paint( char *s, int f, int x, int y, int w )
{
    int len = strlen(s);

    if ( !ffs.buffer )
    {
        font_paint( s,f,x,y );
        return;
    }

    if ( ffs.started )
    {
        /* Try and carry on */
        if ( f == ffs.flags && SPACE > len+8 && ffs.y == y )
        {
            if ( f & font_OSCOORDS )
            {
                fastfont__move(  9, (x - ffs.x)*400 );
                fastfont__move( 11, (y - ffs.y)*400 );
            }
            else
            {
                fastfont__move(  9, x - ffs.x );
                fastfont__move( 11, y - ffs.y );
            }
            strcpy( ffs.ptr, s );
            ffs.ptr += len;
            ffs.x = x + w;
            ffs.y = y;
            return;
        }

        /* Not compatible so start again */
        fastfont_flush();
    }

    ffs.started = TRUE;
    ffs.startx = x;
    ffs.starty = y;
    ffs.flags = f;
    strcpy( ffs.ptr, s );
    ffs.ptr += len;
    ffs.x = x + w;
    ffs.y = y;
}

void fastfont_flush( void )
{
    if ( !ffs.buffer )
        return;

    *ffs.ptr = 0;
    if ( ffs.ptr > ffs.buffer )
        font_paint( ffs.buffer, ffs.flags, ffs.startx, ffs.starty );
    ffs.started = FALSE;
    ffs.ptr = ffs.buffer;
}

void fastfont_close(void)
{
    fastfont_flush();
    nOpens--;
    if ( nOpens == 0 )
    {
        ffs.buffer = 0;
    }
}
