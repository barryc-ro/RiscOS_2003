/* > internal.c

 *

 * Code realting to the handling of ncfrescointernal URL's and generated web pages.


 */


#include <stdio.h>
#include <string.h>

#include "bbc.h"
#include "msgs.h"

#include "access.h"
#include "config.h"
#include "filetypes.h"
#include "interface.h"
#include "makeerror.h"
#include "memwatch.h"
#include "url.h"
#include "util.h"
#include "webfonts.h"

#include "fevents.h"
#include "hotlist.h"
#include "stbview.h"
#include "stbfe.h"
#include "stbhist.h"
#include "stbopen.h"
#include "stbutils.h"
#include "stbtb.h"

/* ----------------------------------------------------------------------------------------------------- */

static char *auth_code = "U21hcnQga2lkUw==";

fe_passwd fe_current_passwd = NULL;

/* ----------------------------------------------------------------------------------------------------- */

static char *checked(int flag)
{
    return flag ? "CHECKED" : "";
}

static void get_form_size(int *width, int *height)
{
    int char_height;

    *width = webfont_tty_width(text_safe_box.x1 - text_safe_box.x0, 0) - 8;

    char_height = webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down;
    *height = (text_safe_box.y1 - text_safe_box.y0)/char_height - 12;
}

/* ----------------------------------------------------------------------------------------------------- */

static os_error *fe_version_write_file(FILE *f, be_doc doc)
{
    char *url, *title;
    const char *s;
    char *s1, *s2;

    if (doc == NULL)
	return NULL;

    fputs(msgs_lookup("versionT"), f);
    fputc('\n', f);

    url = title = NULL;
    backend_doc_info(doc, NULL, NULL, &url, &title);

    fprintf(f, msgs_lookup("version1"), fresco_version);
    if (title)
	fprintf(f, msgs_lookup("version2"), title);
    if (url)
	fprintf(f, msgs_lookup("version3"), url);

    s1 = strdup(backend_check_meta(doc, "last-modified"));
    s2 = strdup(backend_check_meta(doc, "expires"));
    if (s1 || s2)
    {
	fprintf(f, msgs_lookup("version4"), s1 ? s1 : msgs_lookup("Unknown"), s2 ? s2 : msgs_lookup("Unknown"));
	mm_free(s1);
	mm_free(s2);
    }

    if ((s = backend_check_meta(doc, "author")) != NULL)
	fprintf(f, msgs_lookup("version6"), s);

    if ((s = backend_check_meta(doc, "description")) != NULL)
	fprintf(f, msgs_lookup("version7"), s);

    if ((s = backend_check_meta(doc, "copyright")) != NULL)
	fprintf(f, msgs_lookup("version8"), s);

    fputs(msgs_lookup("versionF"), f);
    fputc('\n', f);

    return NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

static void internal_decode_display_options(const char *query)
{
    fe_view v;
    char *s;
    BOOL cancel;

    s = extract_value(query, "action=");
    cancel = strcasestr(s, "cancel") != 0;
    mm_free(s);

    if (!cancel)
    {
	config_display_body_colours = strstr(query, "opt=bg") != 0;
	config_defer_images = strstr(query, "opt=img") == 0;

	v = main_view;
	v->flags &= ~(be_openurl_flag_BODY_COLOURS | be_openurl_flag_DEFER_IMAGES);
	v->flags |= (config_display_body_colours ? be_openurl_flag_BODY_COLOURS : 0) |
	    (config_defer_images ? be_openurl_flag_DEFER_IMAGES : 0);

	if (v->browser_mode == fe_browser_mode_WEB)
	{
	    backend_doc_set_flags(v->displaying,
				  be_openurl_flag_BODY_COLOURS | be_openurl_flag_DEFER_IMAGES,
				  v->flags & (be_openurl_flag_BODY_COLOURS | be_openurl_flag_DEFER_IMAGES));
	    fe_refresh_screen(NULL);
	}
    }
}

static os_error *fe_display_options_write_file(FILE *f)
{
    fputs(msgs_lookup("dispT"), f);
    fputc('\n', f);

    fprintf(f, msgs_lookup("disp1"), checked(config_display_body_colours));
    fprintf(f, msgs_lookup("disp2"), checked(!config_defer_images));

    fputs(msgs_lookup("dispF"), f);
    fputc('\n', f);

    return NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

static void internal_decode_print_options(const char *query)
{
    char *s;
    BOOL cancel;

    s = extract_value(query, "action=");
    cancel = strcasestr(s, "cancel") != 0;
    mm_free(s);

    if (!cancel)
    {
	s = extract_value(query, "copies=");
	print__copies = atoi(s);
	mm_free(s);

	if (print__copies < 1)
	    print__copies = 1;

	s = extract_value(query, "scaling=");
	config_print_scale = atoi(s);
	mm_free(s);

	if (config_print_scale < 5)
	    config_print_scale = 5;

	s = extract_value(query, "orient=");
	config_print_sideways = strcmp(s, "side") == 0;
	mm_free(s);

	config_print_nopics = strstr(query, "f=pic") == 0;
/* 	config_print_nocol = strstr(query, "f=col") == 0; */
	config_print_nobg = strstr(query, "f=bg") == 0;
	config_print_collated = strstr(query, "f=collate") != 0;
	config_print_reversed = strstr(query, "f=rev") != 0;

	print__ul = strstr(query, "f=ul") != 0;
    }
}

static os_error *fe_print_options_write_file(FILE *f)
{
    fputs(msgs_lookup("printT"), f);
    fputc('\n', f);

    fprintf(f, msgs_lookup("print1"), print__copies, checked(config_print_collated), checked(config_print_reversed));
    fprintf(f, msgs_lookup("print2"), checked(!config_print_sideways), checked(config_print_sideways));
    fprintf(f, msgs_lookup("print3"), config_print_scale);
    fprintf(f, msgs_lookup("print4"), checked(!config_print_nopics));
    fprintf(f, msgs_lookup("print5"), checked(!config_print_nocol));
    fprintf(f, msgs_lookup("print6"), checked(!config_print_nobg));
    fprintf(f, msgs_lookup("print7"), checked(print__ul));

    fputs(msgs_lookup("printF"), f);
    fputc('\n', f);

    return NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

static os_error *fe_hotlist_and_openurl_write_file(FILE *f)
{
    int width, height;

    fputs(msgs_lookup("openhT"), f);
    fputc('\n', f);

    get_form_size(&width, &height);

    width -= 12;
    fprintf(f, msgs_lookup("openh1"));
    fprintf(f, msgs_lookup("openh2"), width, msgs_lookup("opendef"));
    fprintf(f, msgs_lookup("openh3"));

    hotlist_write_list(f, FALSE);

    fputs(msgs_lookup("openhF"), f);
    fputc('\n', f);

    return NULL;
}

static os_error *fe_hotlist_write_file(FILE *f)
{
    fputs(msgs_lookup("hotsT"), f);
    fputc('\n', f);

    fprintf(f, msgs_lookup("hots1"));

    hotlist_write_list(f, FALSE);

    fputs(msgs_lookup("hotsF"), f);
    fputc('\n', f);

    return NULL;
}

static os_error *fe_hotlist_delete_write_file(FILE *f)
{
    fputs(msgs_lookup("hotsdT"), f);
    fputc('\n', f);

    fprintf(f, msgs_lookup("hotsd1"));

    hotlist_write_list(f, TRUE);

    fputs(msgs_lookup("hotsdF"), f);
    fputc('\n', f);

    return NULL;
}

static os_error *fe_openurl_write_file(FILE *f)
{
    int width, height;

    fputs(msgs_lookup("openT"), f);
    fputc('\n', f);

    get_form_size(&width, &height);

    width -= 12;
    fprintf(f, msgs_lookup("open1"));
    fprintf(f, msgs_lookup("open2"), width, msgs_lookup("opendef"));
    fprintf(f, msgs_lookup("open3"));

    fputs(msgs_lookup("openF"), f);
    fputc('\n', f);

    return NULL;
}


/* ----------------------------------------------------------------------------------------------------- */

/*
 * Old hacky method of handling playback of local Replay files.
 * Strictly for demos only!
 */

static void fe_handle_playmovie(const char *query)
{
    int x, y;
    fe_view v;
    be_item ti;
    wimp_box box;
    os_error *e;

    x = last_click_x;
    y = last_click_y;
    v = last_click_view;

    ti = NULL;
    e = NULL;
    if (v)
	e = backend_doc_locate_item(v->displaying, &x, &y, &ti);

    if (!e && ti)
	e = backend_doc_item_bbox(v->displaying, ti, &box);

    if (!e && ti)
    {
	char *file, *args, *offset;
	char *scheme1, *netloc1, *path1, *params1, *query1, *fragment1;
	char buffer[256];

	file = extract_value(query, "url=");
	args = extract_value(query, "args=");
	offset = extract_value(query, "offset=");
	url_parse(file, &scheme1, &netloc1, &path1, &params1, &query1, &fragment1);

	if (strcasecomp(scheme1, "file") == 0)
	{
	    char *path2 = url_path_to_riscos(path1);
	    coords_cvtstr cvt = fe_get_cvt(v);
	    coords_pointstr off;

	    off.x = off.y = 0;
	    if (offset)
		sscanf(offset, "%d,%d", &off.x, &off.y);
		
	    coords_point_toscreen((coords_pointstr *)&box.x0, &cvt);

	    sprintf(buffer, "/%s -at %d,%d", path2, box.x0 + off.x/2, box.y0 + off.y/2);
	    if (args)
	    {
		strcat(buffer, " ");
		strcat(buffer, args);
	    }
	    fe_start_task(buffer, NULL);
	    mm_free(path2);
	}

	url_free_parts(scheme1, netloc1, path1, params1, query1, fragment1);
	mm_free(file);
	mm_free(args);
	mm_free(offset);
    }
}

/* ----------------------------------------------------------------------------------------------------- */
/*
 *
 */

static char *find__string = NULL;
static int find__backwards;
static int find__casesense;
static fe_view find__v = NULL;

static int internal_decode_find(const char *query)
{
    char *text, *dir, *casesense, *s;
    BOOL cancel;
    
    if (!find__v || find__v->magic != ANTWEB_VIEW_MAGIC)
	return fe_internal_url_ERROR;

    s = extract_value(query, "action=");
    cancel = strcasestr(s, "cancel") != 0;
    mm_free(s);

    if (!cancel)
    {
	BOOL back, cases;
	char buffer[16];

	text = extract_value(query, "for=");

	dir = extract_value(query, "dir=");
	back = dir && strcmp(dir, "back") == 0;

	casesense = extract_value(query, "case=");
	cases = casesense && strcmp(casesense, "sense") == 0;

        uudecode(auth_code, (unsigned char *)buffer, sizeof(buffer));
	if (find__v->browser_mode == fe_browser_mode_HISTORY && cases && back && strcmp(buffer, text) == 0)
	{
	    ncreg_decode();
	}
	else
	{
	    fe_find(find__v, text, back, cases);
	}
    
	mm_free(text);
	mm_free(dir);
	mm_free(casesense);
    }

    find__v = NULL;

    return fe_internal_url_NO_ACTION;
}

static os_error *fe_find_write_file(FILE *f)
{
    if (!find__v)
	return NULL;

    fputs(msgs_lookup("findT"), f);
    fputc('\n', f);

    fprintf(f, msgs_lookup("find1"));
    fprintf(f, msgs_lookup("find2"), strsafe(find__string), checked(find__casesense));
    fprintf(f, msgs_lookup("find3"), checked(!find__backwards),
        checked(find__backwards));

    fputs(msgs_lookup("findF"), f);
    fputc('\n', f);

    return NULL;
}

int fe_find_again_possible(fe_view v)
{
    return v && v->displaying && find__string && find__string[0];
}

void fe_find_again(fe_view v)
{
    char *s = strdup(find__string);
    fe_find(v, s, find__backwards, find__casesense);
    mm_free(s);
}

int fe_find_possible(fe_view v)
{
    return v && v->displaying;
}

os_error *fe_find_open(fe_view v)
{
    os_error *e = NULL;
    if (fe_find_possible(v))
    {
        wimp_box bb;

        frontend_view_bounds(v, &bb);

        STBDBG(( "find: bounds %d,%d %d,%d\n", bb.x0, bb.y0, bb.x1, bb.y1));

        bb.x0 = -1000;		/* Get the first on the line ignoring margins */
        backend_doc_locate_item(v->displaying, &bb.x0, &bb.y1, &v->find_last_item);

	/* store the frame that we started in */
	find__v = v;	
	e = frontend_open_url("ncfrescointernal:openpanel?name=find", v, TARGET_DBOX, NULL, fe_open_url_NO_CACHE);
    }
    return e;
}

void fe_find(fe_view v, const char *text, int backwards, int casesense)
{
    if (!v || !v->displaying)
        return;

    STBDBG(( "find: start item %p\n", v->find_last_item));

    mm_free(find__string);
    find__string = strdup(text);
    find__backwards = backwards;
    find__casesense = casesense;

    v->find_last_item = backend_find(v->displaying, v->find_last_item, (char *)text,
        (backwards ? be_find_BACKWARDS : 0) | (casesense ? 0 : be_find_CASELESS));

    STBDBG(( "find: find item %p\n", v->find_last_item));

    if (v->find_last_item == NULL)
    {
        bbc_vdu(7);
        frontend_complain(makeerror(ERR_FIND_FAILED));
    }
}

/* ------------------------------------------------------------------------------------------- */
  
static int vals_to_bits(int n_vals)
{
    int n_bits = 0;
    do
    {
	n_vals >>= 1;
	n_bits++;
    }
    while (n_vals > 1);
    return n_bits;
}

static int nvram_do(int bit_start, int n_bits, int new_val)
{
    int mask, byte, offset, r, old;

    mask = (1<<n_bits) - 1;
    byte = bit_start/8;
    offset = bit_start%8;

    /* read current value */
    r = _kernel_osbyte(0xA1, byte, 0);
    if (r == _kernel_ERROR)
	return -1;

    r = (r >> 8) & 0xff;
    old = (r & mask) >> offset;

    if (new_val != -1)
    {
	r &= ~(mask << offset);
	r |= new_val << offset;
	_kernel_osbyte(0xA2, byte, r);
    }
    
    return old;
}

static int nvram_read(int bit_start, int n_bits)
{
    return nvram_do(bit_start, n_bits, -1);
}

static void nvram_write(int bit_start, int n_bits, int val)
{
    nvram_do(bit_start, n_bits, val);
}

#define NVRAM_FONTS	(131*8 + 0)
#define NVRAM_SOUND	(131*8 + 2)
#define NVRAM_BEEPS     (131*8 + 3)

/* ------------------------------------------------------------------------------------------- */

static os_error *fe_custom_write_file(FILE *f, const char *tag, int bit_start, int n_vals)
{
    char tag_buf[8];
    int val, i;

    val = nvram_read(bit_start, vals_to_bits(n_vals));
    
    sprintf(tag_buf, "m%sT", tag);
    fputs(msgs_lookup(tag_buf), f);

    for (i = 0; i < n_vals; i++)
    {
	sprintf(tag_buf, "m%s%d", tag, i);
 	fprintf(f, msgs_lookup(tag_buf), val == i ? "radioon" : "radiooff");
/* 	fprintf(f, msgs_lookup(tag_buf), val == i ? "CHECKED" : ""); */
    }

    sprintf(tag_buf, "m%sF", tag);
    fputs(msgs_lookup(tag_buf), f);

    return NULL;
}

static int internal_decode_custom(const char *query, char **url, int *flags)
{
    char *font = extract_value(query, "fonts.");
    char *sound = extract_value(query, "sound.");
    char *beeps = extract_value(query, "beeps.");
    int generated = fe_internal_url_NO_ACTION;
    
    if (font)
    {
	int font_val = atoi(font);
	nvram_write(NVRAM_FONTS, 2, font_val);

	fe_font_size_set(font_val, TRUE);

	*url = strdup("ncfrescointernal:openpanel?name=customfonts");
	generated = fe_internal_url_REDIRECT;
    }

    if (sound)
    {
	int sound_val = atoi(sound);
	nvram_write(NVRAM_SOUND, 1, sound_val);

	*url = strdup("ncfrescointernal:openpanel?name=customsound");
	generated = fe_internal_url_REDIRECT;
    }
    
    if (beeps)
    {
	int beeps_val = atoi(beeps);
	nvram_write(NVRAM_BEEPS, 1, beeps_val);

	*url = strdup("ncfrescointernal:openpanel?name=custombeeps");
	generated = fe_internal_url_REDIRECT;
    }

    *flags = access_NOCACHE;
    
    mm_free(font);
    mm_free(sound);
    mm_free(beeps);

    return generated;
}

/* ------------------------------------------------------------------------------------------- */
/* password handling    */
/* ------------------------------------------------------------------------------------------- */

static int internal_decode_password(const char *query)
{
    char *name, *pass, *s;
    fe_passwd pw;
    BOOL cancel;

    if (!fe_current_passwd)
        return fe_internal_url_ERROR;

    s = extract_value(query, "action=");
    cancel = strcasestr(s, "cancel") != 0;
    mm_free(s);

    if (cancel)
	fe_passwd_abort();
    else
    {	
	name = extract_value(query, "name=");
	pass = extract_value(query, "pass=");

	STBDBG(( "name='%s'\n", name));
	STBDBG(( "pass='%s'\n", pass));

	pw = fe_current_passwd;

	/* do the callback and return control   */
	if (pw->cb)
	    (pw->cb)(pw, pw->h, name, pass);

	frontend_passwd_dispose(pw);

	mm_free(name);
	mm_free(pass);
    }

    return fe_internal_url_NO_ACTION;
}

static os_error *fe_passwd_write_file(FILE *f)
{
    fe_passwd pw = fe_current_passwd;

    if (pw)
    {
	int width, height;

	fputs(msgs_lookup("passwdT"), f);
	fputc('\n', f);

	get_form_size(&width, &height);

	fprintf(f, msgs_lookup("passwd1"), pw->realm ? pw->realm : "unknown", strsafe(pw->site));
	fprintf(f, msgs_lookup("passwd2"), width, width);

	fputs(msgs_lookup("passwdF"), f);
	fputc('\n', f);
    }

    return NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

void fe_passwd_abort(void)
{
    fe_passwd pw = fe_current_passwd;

    if (pw)
    {
        if (pw->cb)
            (pw->cb)(pw, pw->h, NULL, NULL);

        frontend_passwd_dispose(pw);
    }
}

fe_passwd frontend_passwd_raise(backend_passwd_callback cb, void *handle,
				char *user, char *realm, char *site)
{
    fe_passwd pw;

    STBDBG(("passwd_raise user '%s' realm '%s' site '%s'\n",
	    user ? user : "", realm ? realm : "", site ? site : ""));

    pw = mm_calloc(sizeof(*pw), 1);

    pw->cb = cb;
    pw->h = handle;

    pw->user = strdup(user);
    pw->realm = strdup(realm);
    pw->site = strdup(site);

    fe_current_passwd = pw;

    frontend_open_url("ncfrescointernal:openpanel?name=password", NULL, TARGET_PASSWORD, NULL, fe_open_url_NO_CACHE);

    return pw;
}

void frontend_passwd_dispose(fe_passwd pw)
{
    STBDBG(( "passwd_dispose %p\n", pw));

    fe_dispose_view(fe_locate_view(TARGET_PASSWORD));

    mm_free(pw->user);
    mm_free(pw->realm);
    mm_free(pw->site);

    mm_free(pw);
    fe_current_passwd = NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

static os_error *fe_mem_dump_write_file(FILE *f)
{
    int us = -1, next = -1, free;

    fprintf(f, "<TITLE>NCFresco memory dump</TITLE>\n");
    fprintf(f, "<META NAME=BrowserMode CONTENT=DBox\n");
    fprintf(f, "<H1>NCFresco memory dump</H1>\n");

    fprintf(f, "<MENU><LI><A HREF='#wimp'>Wimp slot</A><LI><A HREF='#malloc'>Malloc heap</A><LI><A HREF='#image'>Image heap</A></MENU>\n");

    wimp_slotsize(&us, &next, &free);
    fprintf(f, "<H2><A NAME='wimp'>Wimp slot usage</A></H2><TABLE>");
    fprintf(f, "<TR><TD>Us<TD ALIGN=RIGHT>%d bytes<TD ALIGN=RIGHT>%d K", us, us/1024);
    fprintf(f, "<TR><TD>Next<TD ALIGN=RIGHT>%d bytes<TD ALIGN=RIGHT>%d K", next, next/1024);
    fprintf(f, "<TR><TD>Free<TD ALIGN=RIGHT>%d bytes<TD ALIGN=RIGHT>%d K", free, free/1024);
    fprintf(f, "</TABLE>");

    fprintf(f, "<H2><A NAME='malloc'>Malloc heap</A></H2><PRE>");
    mm__dump(f);
    fprintf(f, "</PRE>");

    fprintf(f, "<H2><A NAME='image'>Image heap</A></H2><PRE>");
#if !MEMLIB
    heap__dump(f);
#endif
    fprintf(f, "</PRE>");

    return NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

static fe_view get_source_view(const char *query, BOOL default_top)
{
    char *source = extract_value(query, "source=");
    fe_view v;

    /* which view are we pulling data from */
    if (source == NULL)
	v = default_top ? main_view : fe_selected_view();
    else
	v = fe_find_target(main_view, source);

    mm_free(source);

    return v;
}

static char *get_url(fe_view v)
{
    char *url;

    if (v &&
	v->displaying &&
	backend_doc_info(v->displaying, NULL, NULL, &url, NULL) == NULL &&
		url)
    {
	return url;
    }

    return NULL;
}
/* ----------------------------------------------------------------------------------------------------- */

#if 0
typedef struct
{
    char *name;
    openpanel_fn fn;
    BOOL default_top_frame;
} openpanel_info;

static openpanel_info openpanel_array[] =
{
};
#endif
/*
 * New internal URL handlers
 */

static int internal_url_openpanel(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    char *panel_name = extract_value(query, "name=");
    char *mode = extract_value(query, "mode=");
    int generated = fe_internal_url_ERROR;
    fe_view v;

    if (panel_name == NULL)
	return generated;
    
    if (strcasecomp(panel_name, "related") == 0)
    {
	v = get_source_view(query, FALSE);

	if (config_document_handler_related && v && v->displaying)
	{
	    char *match;
	    if (backend_doc_info(v->displaying, NULL, NULL, NULL, &match) == NULL &&
		match)
	    {
		int related_len = strlen(config_document_handler_related);
		int total_len = related_len + 3*strlen(match) + 2;
		char *s = mm_malloc(total_len);

		strcpy(s, config_document_handler_related);

/* 		if (s[related_len-1] != '?' && s[related_len-1] != '&') */
/* 		    strcat(s, "?"); */
/* 		strcat(s, "url="); */
		url_escape_cat(s, match, total_len);

		*new_url = s;
		generated = fe_internal_url_REDIRECT;

		tb_status_button(fevent_OPEN_RELATED_STUFF, TRUE);
	    }
	}
    }
    else
    {
	FILE *f = fopen(file, "w");
	os_error *e = NULL;

	if (strcasecomp(panel_name, "displayoptions") == 0)
	{
	    tb_status_button(fevent_OPEN_DISPLAY_OPTIONS, TRUE);
	    e = fe_display_options_write_file(f);
	}
	else if (strcasecomp(panel_name, "favs") == 0)
	{
	    tb_status_button(fevent_HOTLIST_SHOW, TRUE);
	    e = fe_hotlist_write_file(f);
	}    
	else if (strcasecomp(panel_name, "favsdelete") == 0)
	{
	    tb_status_button(fevent_HOTLIST_SHOW_DELETE, TRUE);
	    e = fe_hotlist_delete_write_file(f);
	}    
	else if (strcasecomp(panel_name, "find") == 0)
	{
	    tb_status_button(fevent_OPEN_FIND, TRUE);
	    e = fe_find_write_file(f);
	}
	else if (strcasecomp(panel_name, "historyalpha") == 0)
	{
	    tb_status_button(fevent_HISTORY_SHOW_ALPHA, TRUE);
	    e = fe_global_write_list(f);
	}    
	else if (strcasecomp(panel_name, "historyrecent") == 0)
	{
	    v = get_source_view(query, TRUE);
	    if (v)
	    {
		tb_status_button(fevent_HISTORY_SHOW_RECENT, TRUE);
		e = fe_history_write_list(f, v->first);
	    }
	}
	else if (strcasecomp(panel_name, "historycombined") == 0)
	{
	    v = get_source_view(query, TRUE);
	    if (v)
	    {
		tb_status_button(fevent_HISTORY_SHOW, TRUE);
		e = fe_history_write_combined_list(f, v->first);
	    }
	}    
	else if (strcasecomp(panel_name, "info") == 0)
	{
	    v = get_source_view(query, TRUE);
	    if (v)
	    {
		tb_status_button(fevent_INFO_PAGE, TRUE);
		e = fe_version_write_file(f, v->displaying);
	    }
	}
	else if (strcasecomp(panel_name, "memdump") == 0)
	{
	    e = fe_mem_dump_write_file(f);
	}
	else if (strcasecomp(panel_name, "printoptions") == 0)
	{
	    tb_status_button(fevent_OPEN_PRINT_OPTIONS, TRUE);
	    e = fe_print_options_write_file(f);
	}
	else if (strcasecomp(panel_name, "password") == 0)
	{
	    e = fe_passwd_write_file(f);
	}
	else if (strcasecomp(panel_name, "url") == 0)
	{
	    tb_status_button(fevent_OPEN_URL, TRUE);
	    e = fe_openurl_write_file(f);
	}    
	else if (strcasecomp(panel_name, "urlfavs") == 0)
	{
	    tb_status_button(fevent_HOTLIST_SHOW_WITH_URL, TRUE);
	    e = fe_hotlist_and_openurl_write_file(f);
	}    
	else if (strcasecomp(panel_name, "customfonts") == 0)
	{
	    tb_status_button(fevent_OPEN_FONT_SIZE, TRUE);
	    e = fe_custom_write_file(f, "fonts", NVRAM_FONTS, 3);
	}    
	else if (strcasecomp(panel_name, "customsound") == 0)
	{
	    tb_status_button(fevent_OPEN_SOUND, TRUE);
	    e = fe_custom_write_file(f, "sound", NVRAM_SOUND, 2);
	}    
	else if (strcasecomp(panel_name, "custombeeps") == 0)
	{
	    tb_status_button(fevent_OPEN_BEEPS, TRUE);
	    e = fe_custom_write_file(f, "beeps", NVRAM_BEEPS, 2);
	}    
    
	fclose(f);

	if (!e)
	{
	    set_file_type(file, FILETYPE_HTML);
	    generated = fe_internal_url_NEW;
	}
    }
    mm_free(panel_name);
    mm_free(mode);

    return generated;
    NOT_USED(referer);
}

static int internal_url_loadurl(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    char *url = extract_value(query, "url=");
    char *nocache = extract_value(query, "opt=");
    char *remove = extract_value(query, "remove=");

    if (nocache && strcasestr(nocache, "nocache"))
	*flags |= access_NOCACHE;
    
    *new_url = check_url_prefix(url);

    if (remove)
	fe_dispose_view(fe_locate_view(remove));

    tb_status_button(fevent_OPEN_URL, TRUE);
       
    mm_free(url);
    mm_free(nocache);
    mm_free(remove);
    return fe_internal_url_REDIRECT;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
}

static int internal_url_openpage(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    char *page_name = extract_value(query, "name=");
    int generated = fe_internal_url_ERROR; 

    if (page_name == NULL)
	return generated;

    if (strcasecomp(page_name, "home") == NULL)
    {
	if (config_doc_default)
	{
	    fe_file_to_url(config_doc_default, new_url);
	    generated = fe_internal_url_REDIRECT;
	}
    }
    else if (strcasecomp(page_name, "search") == NULL)
    {
	if (config_document_search)
	{
	    fe_file_to_url(config_document_search, new_url);
	    generated = fe_internal_url_REDIRECT;
	}
    }
    else if (strcasecomp(page_name, "offline") == NULL)
    {
	if (config_document_offline)
	{
	    fe_file_to_url(config_document_offline, new_url);
	    generated = fe_internal_url_REDIRECT;
	}
    }

    return generated;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
}

/*
 * For backwards compatibility
 */

static int internal_url_home(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    return internal_url_openpage("name=home", bfile, referer, file, new_url, flags);
    NOT_USED(query);
}

static int internal_url_hotlist(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    return internal_url_openpanel("name=favs", bfile, referer, file, new_url, flags);
    NOT_USED(query);
}

#define SENDURL_PREFIX	"mailto:?url="

static int internal_url_sendurl(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);
    char *url = get_url(v);

    if (url)
    {
	int total_len = sizeof(SENDURL_PREFIX)-1 + 3*strlen(url) + 1;
	char *s = mm_malloc(total_len);

	strcpy(s, SENDURL_PREFIX);
	url_escape_cat(s, url, total_len);

	*new_url = s;
	*flags |= access_NOCACHE;

	return fe_internal_url_REDIRECT;
    }

    return fe_internal_url_ERROR;
}

/* ----------------------------------------------------------------------------------------------------- */

static int internal_action_back(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
    {
	*new_url = strdup(fe_history_get_url(v, history_PREV));
	*flags &= ~access_CHECK_EXPIRE;
    }

    return *new_url ? fe_internal_url_REDIRECT : fe_internal_url_ERROR;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
}

static int internal_action_forward(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
    {
	*new_url = strdup(fe_history_get_url(v, history_NEXT));
	*flags &= ~access_CHECK_EXPIRE;
    }

    return *new_url ? fe_internal_url_REDIRECT : fe_internal_url_ERROR;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
}

static int internal_action_close(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
	fevent_handler(fevent_CLOSE, v);

    return fe_internal_url_HELPER;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

static int internal_action_playmovie(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_handle_playmovie(query);
    
    return fe_internal_url_NO_ACTION;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

static int internal_action_stop(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
	fevent_handler(fevent_STOP_LOADING, v);
    
    return fe_internal_url_HELPER;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

static int internal_action_reload(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
	fevent_handler(fevent_RELOAD, v);
    
    return fe_internal_url_HELPER;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

static int internal_action_favoritesadd(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
	fevent_handler(fevent_HOTLIST_ADD, v);

    /* FIXME: add to messages file */
    fe_report_error("Site added to favorites list");
    
    return fe_internal_url_HELPER;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

static int internal_action_favoritesremove(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
	fevent_handler(fevent_HOTLIST_REMOVE, v);

    /* FIXME: add to messages file */
    fe_report_error("Site removed from favorites list");
    
    return fe_internal_url_HELPER;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

static int internal_action_printpage(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);
    char *size = extract_value(query, "size=");
    BOOL legal = size && strcasecomp(size, "legal") == 0;

    if (use_toolbox)
	tb_status_button(legal ? fevent_PRINT_LEGAL : fevent_PRINT_LETTER, TRUE);
    
    if (v)
	fe_print(v);

    if (use_toolbox)
	tb_status_button(legal ? fevent_PRINT_LEGAL : fevent_PRINT_LETTER, FALSE);
    
    mm_free(size);
    
    return fe_internal_url_NO_ACTION;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

static int internal_action_keyboard(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
	fevent_handler(fevent_OPEN_KEYBOARD, v);

    return fe_internal_url_HELPER;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

static int internal_action_select(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, FALSE);
    char *id = extract_value(query, "id=");

    if (v && v->displaying && id)
    {
	be_item ti = backend_locate_id(v->displaying, id);
	if (ti)
	    backend_activate_link(v->displaying, ti, 0);
    }

    mm_free(id);
    
    return fe_internal_url_NO_ACTION;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

/* ----------------------------------------------------------------------------------------------------- */


static int internal_action_opentoolbar(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    char *bar = extract_value(query, "name=");
    int generated = fe_internal_url_ERROR;
    int event = -1;

    if (bar == NULL)
	return generated;

    if (strcasecomp(bar, "main") == NULL)
    {
	event = fevent_TOOLBAR_MAIN;
    }
    else if (strcasecomp(bar, "favs") == NULL)
    {
	event = fevent_TOOLBAR_FAVS;
    }
    else if (strcasecomp(bar, "extras") == NULL)
    {
	event = fevent_TOOLBAR_EXTRAS;
    }
    else if (strcasecomp(bar, "history") == NULL)
    {
	event = fevent_TOOLBAR_HISTORY;
    }
    else if (strcasecomp(bar, "print") == NULL)
    {
	event = fevent_TOOLBAR_PRINT;
    }
    else if (strcasecomp(bar, "details") == NULL)
    {
	event = fevent_TOOLBAR_DETAILS;
    }
    else if (strcasecomp(bar, "vcr") == NULL)
    {
	event = fevent_TOOLBAR_CODEC;
    }
    else if (strcasecomp(bar, "custom") == NULL)
    {
	event = fevent_TOOLBAR_CUSTOM;
    }

    if (event != -1)
    {
	fe_view v = get_source_view(query, TRUE);

	if (v)
	    fevent_handler(event, v);

	generated = fe_internal_url_HELPER;
    }
    
    return generated;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

/* ----------------------------------------------------------------------------------------------------- */

static int internal_decode_cancel(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_dbox_cancel();

    return fe_internal_url_NO_ACTION;
    NOT_USED(query);
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

/*
 * Format of hotlist delet query data is
 * i=n1&i=n2&i=n3... for however many sites we have selected to delete
 * We know they will be in order
 */

static int internal_decode_process(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    char *page = extract_value(query, "name=");
    int generated = fe_internal_url_NO_ACTION;

    if (strcasecomp(page, "custom") == 0)
    {
	generated = internal_decode_custom(query, new_url, flags);
    }
    else if (strcasecomp(page, "favsdelete") == 0)
    {
	hotlist_remove_list(query);
    }
    else if (strcasecomp(page, "displayoptions") == 0)
    {
	internal_decode_display_options(query);
    }
    else if (strcasecomp(page, "printoptions") == 0)
    {
	internal_decode_print_options(query);
    }
    else if (strcasecomp(page, "find") == 0)
    {
	generated = internal_decode_find(query);
    }
    else if (strcasecomp(page, "password") == 0)
    {
	generated = internal_decode_password(query);
    }
    
    return generated;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
}

/* ------------------------------------------------------------------------------------------- */

typedef struct
{
    const char *name;
    int (*fn)(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags);
} internal_url_str;

static internal_url_str internal_url_info[] =
{
    { "loadurl", internal_url_loadurl },
    { "openpage", internal_url_openpage },
    { "openpanel", internal_url_openpanel },
    { "sendurl", internal_url_sendurl },

    { "home", internal_url_home },
    { "hotlist", internal_url_hotlist },

    { "opentoolbar", internal_action_opentoolbar },
    { "printpage", internal_action_printpage },
    { "keyboard", internal_action_keyboard },
    { "select", internal_action_select },

    /* These are used to process the results of generated pages. Not usually set by the user */
    { "cancel", internal_decode_cancel },
    { "process", internal_decode_process },

    { "back", internal_action_back },
    { "forward", internal_action_forward },
    { "close", internal_action_close },
    { "playmovie", internal_action_playmovie },
    { "stop", internal_action_stop },
    { "reload", internal_action_reload },
    { "favsadd", internal_action_favoritesadd },
    { "favsremove", internal_action_favoritesremove },

    { 0, 0 }
};

int frontend_internal_url(const char *path, const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    internal_url_str *uu;
    BOOL generated = FALSE;

    STBDBG(("frontend_internal_url(): action '%s'\n", path));
    
    for (uu = internal_url_info; uu->name; uu++)
    {
	if (strcasecomp(uu->name, path) == 0)
	{
	    STBDBG(("frontend_internal_url(): calling fn %p\n", uu->fn));

	    generated = uu->fn(query, bfile, referer, file, new_url, flags);
	    break;
	}
    }

    return generated;
}

/* ------------------------------------------------------------------------------------------- */

/* Old style open-via-temporary-file functions */

os_error *fe_open_version(fe_view v)
{
    return frontend_open_url("ncfrescointernal:openpanel?name=info", v, TARGET_INFO, NULL, fe_open_url_NO_CACHE);
}

os_error *fe_display_options_open(fe_view v)
{
    return frontend_open_url("ncfrescointernal:openpanel?name=displayoptions", v, TARGET_DBOX, NULL, fe_open_url_NO_CACHE);
}


os_error *fe_print_options_open(fe_view v)
{
    return frontend_open_url("ncfrescointernal:openpanel?name=printoptions", v, TARGET_DBOX, NULL, fe_open_url_NO_CACHE);
}


os_error *fe_hotlist_open(fe_view v)
{
    return frontend_open_url("ncfrescointernal:openpanel?name=favs", v, TARGET_FAVS, NULL, fe_open_url_NO_CACHE);
}

os_error *fe_hotlist_and_url_open(fe_view v)
{
    return frontend_open_url("ncfrescointernal:openpanel?name=urlfavs", v, TARGET_FAVS, NULL, fe_open_url_NO_CACHE);
}

os_error *fe_url_open(fe_view v)
{
    return frontend_open_url("ncfrescointernal:openpanel?name=url", v, TARGET_OPEN, NULL, fe_open_url_NO_CACHE);
}

void fe_show_mem_dump(void)
{
    frontend_open_url("ncfrescointernal:openpanel?name=memdump", NULL, TARGET_DBOX, NULL, fe_open_url_NO_CACHE);
}

/* ------------------------------------------------------------------------------------------- */

/* eof internal.c */
