/* -*-c-*- */

/* Write a block of data to the given file */
os_error *df_write_data(int fh, int at, void *base, int size);

/* Write a file header to the file */
void df_write_fileheader(int fh, int width, int dheight, int *writepoint, char *program_name);

/* Write the header for a group object, returning its base position in the file */
int df_write_box(int fh, char *title, int *writepoint);

/* Fix up a group object after all the items have been added */
void df_write_box_fix(int fh, int boxbase, wimp_box *bb, int *writepoint);

/* Write a rectangle with a thin border and a fill colour */
void df_write_filled_rect(int fh, wimp_box *bb,
			  wimp_paletteword col, wimp_paletteword fill, int *writepoint);

/* Make the box bb encompass the box ob */
void df_stretch_bb(wimp_box *bb, wimp_box *ob);

/* Write a thick line INSIDE the box bb */
void df_write_border(int fh, wimp_box *bb, wimp_paletteword col, int width, int *writepoint);

/* Draw a plinth inside the box with top/left and bottom/right colours */
void df_write_plinth(int fh, wimp_box *bb,
		     wimp_paletteword tlcol, wimp_paletteword brcol,
		     int *writepoint);

/* Output a sprite from the WIMP sprite area into a draw file */
void df_write_sprite(int fh, char *sprite, int x, int y, int *fileoff);

extern void df_write_text(int fh, char *s, int font, int x, int y, int *fileoff);
