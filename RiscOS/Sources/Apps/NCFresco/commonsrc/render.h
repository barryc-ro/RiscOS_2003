/* -*-c-*- */

/* render.c */

/* Bits of code to help with rendering */

#define render_plinth_IN	0x01
#define render_plinth_RIM	0x02
#define render_plinth_NOFILL	0x04
#define render_plinth_DOUBLE	0x08
#define render_plinth_THIN	0x10
#define render_plinth_DOUBLE_RIM 0x20

void render_plinth(int bcol, int flags, int x, int y, int w, int h, be_doc doc);
extern void render_plinth_full(int bcol, int rim_col, int top_col, int bottom_col, int flags, int x, int y, int w, int h, be_doc doc);

int render_set_colour(int colour, be_doc doc);
int render_set_bg_colour(int colour, be_doc doc);
void render_set_font_colours(int f, int b, be_doc doc);
int render_link_colour(be_item ti, be_doc doc);
os_error *render_plot_icon(char *sprite, int x, int y);
wimp_paletteword render_get_colour(int colour, be_doc doc);
/* SJM */
void render_item_outline(be_item ti, int hpos, int bline);
int render_text_link_colour(be_item ti, be_doc doc);

int render_cell_background_colour(int c);

int render_background( be_item ti, be_doc doc );
int render_caret_colour(be_doc doc, int back, int cursor);
extern int render_text(be_doc doc, int font_index, const char *text, int x, int y);
extern int render_text_full(be_doc doc, int font_index, const char *text, int x, int y, int *coords, int n);
extern void *render_sprite_locate(const char *sprite, void **sptr_out);
extern void render_plinth_from_list(int bcol, wimp_paletteword *cols, int flags, int x, int y, int w, int h, be_doc doc);

