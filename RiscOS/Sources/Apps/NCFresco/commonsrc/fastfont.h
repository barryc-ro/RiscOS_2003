/* fastfont.h */

void fastfont_open( char *buffer, int nBufSize );
void fastfont_set_colours( wimp_paletteword paper, wimp_paletteword pen,
                           BOOL antialias );
void fastfont_setfont( int font );
void fastfont_paint( char *string, int flags, int x, int y, int strwidth );
void fastfont_flush(void);
void fastfont_close(void);
