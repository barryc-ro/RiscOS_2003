/* > internal.c

 *

 * Code realting to the handling of ncfrescointernal URL's and generated web pages.


 */


#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "bbc.h"
#include "msgs.h"
#include "swis.h"

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
#include "frameutils.h"
#include "memflex.h"
#include "printing.h"
#include "gbf.h"

#if UNICODE
#include "Unicode/charsets.h"
#endif

/* ----------------------------------------------------------------------------------------------------- */

static fe_view get_source_view(const char *query, BOOL default_top);

/* ----------------------------------------------------------------------------------------------------- */

#define PPrimer_ChangePrinter	0x4B100

static char *auth_code = "U21hcnQga2lkUw==";

fe_passwd fe_current_passwd = NULL;
static fe_ssl fe_current_ssl = NULL;

static char *loadurl_last = NULL;

/* ----------------------------------------------------------------------------------------------------- */

static char *checked(int flag)
{
    return flag ? "CHECKED" : "";
}

static char *selected(int flag)
{
    return flag ? "SELECTED" : "";
}

static int limit(int val, int low, int high)
{
    if (val < low)
	return low;
    if (val > high)
	return high;
    return val;
}    

static void get_form_size(int *width, int *height)
{
    int char_height;

    *width = (text_safe_box.x1 - text_safe_box.x0) / webfonts[WEBFONT_TTY].space_width - 8;
/*  *width = webfont_tty_width(text_safe_box.x1 - text_safe_box.x0, 0) - 8; */

    char_height = webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down;
    *height = (text_safe_box.y1 - text_safe_box.y0)/char_height - 12;
}

static void write_charset(FILE *f)
{
#if UNICODE
    fprintf(f, "<META NAME=Content-Type CONTENT='text/html; charset=%s'>\n",
	    config_encoding_internal == 0 ? "ISO-8859-1" :
	    config_encoding_internal == 1 || config_encoding_internal == 2 ? "UTF-8" :
	    config_encoding_internal == 3 ? "SHIFT_JIS" :
	    config_encoding_internal == 4 ? "EUC-JP" : "US-ASCII");
#endif
}

/* ----------------------------------------------------------------------------------------------------- */

static BOOL should_we_display_url(const char *url)
{
    return url &&
	strncasecomp(url, "ncfrescointernal:", sizeof("ncfrescointernal:")-1) != 0 &&
	strncasecomp(url, "ncint:", sizeof("ncint:")-1) != 0 &&
	strncasecomp(url, "file:/cache:", sizeof("file:/cache:")-1) != 0;
}

/* This function write s a string to the file with potential line
 * breaks at punctuation, whitespace or every 16 characters
 * This should make the errors format better.
 * The NOBR/WBR scheme is whaqt happens to work at the moment.
 */

static void write_url_with_breaks(FILE *f, const char *url)
{
    const char *s = url;
    int c, count = 0;
    BOOL in_tag = FALSE;

/*     fputs("<NOBR>", f); */
    for (c = *s++; c; c = *s++)
    {
	fputc(c, f);

	if (in_tag)
	{
	    if (c == '>')
		in_tag = FALSE;
	}
	else if (c == '<')
	{
	    in_tag = TRUE;
	}
	else
	{
/* 	    if (ispunct(c) || isspace(c) || count == 16) */
 	    if (ispunct(c) || count == 16)
	    {
		fputs("<WBR>", f);
		count = 0;
	    }
	    else
	    {
		count++;
	    }
	}
    }
/*     fputs("</NOBR>", f); */
}

#define TIME_FORMAT	"%a, %d %b %Y %H:%M:%S %Z"

static os_error *fe_version_write_file(FILE *f, be_doc doc, const char *query)
{
    char *qlink, *qtitle;
    const char *lang;
    
    qlink = extract_value(query, "url=");
    qtitle = extract_value(query, "title=");

    lang = lang_num_to_name(backend_doc_item_language(doc, NULL));
    
    fputs(msgs_lookup("versionT"), f);
    fprintf(f, msgs_lookup("version1"), ""/*fresco_version*/);

    if (doc)
    {
	char *url, *title;
	const char *s;
	unsigned last_modified, expires;

	url = title = NULL;
	backend_doc_info(doc, NULL, NULL, &url, &title);

	if (title)
	    fprintf(f, msgs_lookup("version2"), lang, title);

	if (should_we_display_url(url))
	{
	    fputs(msgs_lookup("version3"), f);
	    write_url_with_breaks(f, url);
	}

	if (access_get_header_info(url, NULL, &last_modified, &expires, NULL))
	{
	    char rbuf[32];
	    int dst, timezone;

	    /* get current daylight savings time flag */
	    dst = _kernel_osbyte(161, 0xdc, 0) & (1 << 15);

	    /* offset the GMT to the local time so when we convert it it displays correctly */
	    _swix(Territory_ReadTimeZones, _IN(0) | _OUT(dst ? 3 : 2), -1, &timezone);
	    timezone /= 100;

	    if (last_modified)
	    {
		last_modified += timezone;
		strftime(rbuf, sizeof(rbuf), TIME_FORMAT, localtime((const time_t *)&last_modified));
		fprintf(f, msgs_lookup("version4"), rbuf);
	    }
	    if (expires && expires != UINT_MAX)
	    {
		expires += timezone;
		strftime(rbuf, sizeof(rbuf), TIME_FORMAT, localtime((const time_t *)&expires));
		fprintf(f, msgs_lookup("version5"), rbuf);
	    }
	}

	if ((s = backend_check_meta(doc, "author")) != NULL)
	    fprintf(f, msgs_lookup("version6"), lang, s);

	if ((s = backend_check_meta(doc, "description")) != NULL)
	    fprintf(f, msgs_lookup("version7"), lang, s);

	if ((s = backend_check_meta(doc, "copyright")) != NULL)
	    fprintf(f, msgs_lookup("version8"), lang, s);

#if UNICODE
	{
	    char buf[32];
	    int enc = backend_doc_encoding(doc, NULL, NULL);
	    sprintf(buf, "encoding%d:", enc);
	    s = msgs_lookup(buf);
	    if (!s || !s[0])
	    {
		const char *ctype = backend_check_meta(doc, "content-type");
		if (ctype)
		{
		    char *enc = parse_content_type_header_charset(s);
		    if (enc)
		    {
			sprintf(buf, msgs_lookup("encodingX"), enc);
			mm_free(enc);
			s = buf;
		    }
		}
	    }

	    if (s && s[0])
		fprintf(f, msgs_lookup("version9"), s);
	}
#endif
    }

    if (qlink)
    {
	char *link;

	if (strncasecomp(qlink, "ncfrescointernal:", sizeof("ncfrescointernal:")-1) == 0 ||
	    strncasecomp(qlink, "ncint:", sizeof("ncint:")-1) == 0)
	    link = extract_value(qlink, "url=");
	else
	    link = qlink;

	if (should_we_display_url(link))
	{
	    fputs(msgs_lookup("version3a"), f);
	    write_url_with_breaks(f, link);
	}

	if (qtitle)
	    fprintf(f, msgs_lookup("version2"), lang, qtitle);

	if (link != qlink)
	    mm_free(link);
    }

    fputs(msgs_lookup("versionF"), f);

    mm_free(qlink);
    mm_free(qtitle);

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

static int internal_decode_print_options(const char *query, char **url)
{
    char *s;
    BOOL cancel, print;

    s = extract_value(query, "action=");
    cancel = strcasestr(s, "cancel") != 0;
    print = strcasestr(s, "print") != 0;
    mm_free(s);

    if (!cancel)
    {
	int old_bw = config_print_nocol;
	int old_orient = config_print_sideways;

	if ((s = extract_value(query, "orient=")) != NULL)
	{
	    config_print_sideways = strcmp(s, "side") == 0;
	    mm_free(s);

	    nvram_write(NVRAM_PRINT_ORIENTATION_TAG, config_print_sideways);
	}

	if ((s = extract_value(query, "color=")) != NULL)
	{
	    config_print_nocol = strcmp(s, "bw") == 0;
	    mm_free(s);

	    nvram_write(NVRAM_PRINT_COLOUR_TAG, !config_print_nocol);
	}
	
	if ((s = extract_value(query, "bg=")) != NULL)
	{
	    config_print_nobg = strcmp(s, "yes") != 0;
	    mm_free(s);

	    nvram_write(NVRAM_PRINT_BG_TAG, !config_print_nobg);
	}

	if ((s = extract_value(query, "images=")) != NULL)
	{
	    config_print_nopics = strcmp(s, "yes") != 0;
	    mm_free(s);

	    nvram_write(NVRAM_PRINT_IMAGES_TAG, !config_print_nopics);
	}

	if ((old_bw != config_print_nocol || old_orient != config_print_sideways) && 
	    frontend_complain(_swix(PPrimer_ChangePrinter, 0)) != NULL)
	{
	    /* If PPrimer fails then reset the colour NVRAM bits */
	    config_print_nocol = old_bw;
	    nvram_write(NVRAM_PRINT_COLOUR_TAG, !config_print_nocol);
	}
    }

    /* return the print head - ignore errors */
    awp_command(printer_command_RETURN_HEAD);

    if (print)
	fe_pending_event = fevent_PRINT;
    
    return fe_internal_url_NO_ACTION;
}

static os_error *fe_print_options_write_file(FILE *f)
{
    fputs(msgs_lookup("print.T"), f);

    fprintf(f, msgs_lookup("print.1"), checked(!config_print_sideways), checked(config_print_sideways));
    fprintf(f, msgs_lookup("print.2"), checked(!config_print_nocol), checked(config_print_nocol));
    fprintf(f, msgs_lookup("print.3"), checked(!config_print_nobg), checked(config_print_nobg));
    fprintf(f, msgs_lookup("print.4"), checked(!config_print_nopics), checked(config_print_nopics));

    fputs(msgs_lookup("print.F"), f);

    /* center the print head - ignore errors */
    awp_command(printer_command_CENTRE_HEAD);

    return NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

static int internal_decode_options(const char *query, char **new_url)
{
    char *option = extract_value(query, "option=");

    if (strcasecomp(option, "text") == NULL)
    {
	char *scale = extract_value(query, "scale=");
	char *fit = extract_value(query, "fit=");

	config_display_scale = config_display_scales[limit(atoi(scale), 0, 2)];
	config_display_scale_fit = atoi(fit);
	if (config_display_scale_fit)
	    gbf_flags |= GBF_AUTOFIT;
	else
	    gbf_flags &= ~GBF_AUTOFIT;

	mm_free(scale);
	mm_free(fit);
    }
    else if (strcasecomp(option, "fonts") == NULL)
    {
	char *encoding = extract_value(query, "encoding=");
	char *override = extract_value(query, "override=");

	config_encoding_user = atoi(encoding);
	config_encoding_user_override = atoi(override);

	mm_free(encoding);
	mm_free(override);
    }

    mm_free(option);

    *new_url = strdup("ncint:current");

    return fe_internal_url_REDIRECT;
}

static os_error *fe_options_write_file(FILE *f, const char *option)
{
    fprintf(f, msgs_lookup("options.T"), option);
    fprintf(f, msgs_lookup("options.1"), option);

    if (strcasecomp(option, "text") == NULL)
    {
	fprintf(f, msgs_lookup("options.text.1"),
		checked(config_display_scale == config_display_scales[0]),
		checked(config_display_scale == config_display_scales[1]),
		checked(config_display_scale == config_display_scales[2]));

	fprintf(f, msgs_lookup("options.text.2"),
		checked(config_display_scale_fit),
		checked(!config_display_scale_fit));
    }
    else if (strcasecomp(option, "fonts") == NULL)
    {
#if UNICODE
	fprintf(f, msgs_lookup("options.fonts.1"),
		selected(config_encoding_user == csWindows1252),
		selected(config_encoding_user == csAutodetectJP),
		selected(config_encoding_user == csEUCPkdFmtJapanese),
		selected(config_encoding_user == csShiftJIS),
		selected(config_encoding_user == csISO2022JP),
		selected(config_encoding_user == csGB2312),
		selected(config_encoding_user == csBig5),
		selected(config_encoding_user == csEUCKR),
		selected(config_encoding_user == csISO2022KR));
	fprintf(f, msgs_lookup("options.fonts.2"),
		checked(config_encoding_user_override),
		checked(!config_encoding_user_override));
#endif
    }

    fprintf(f, msgs_lookup("options.F"), option);

    return NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

static const char *print_size = NULL;
static BOOL print__first;

static void fe__print_frames(FILE *f, const char *spec, int w, int h)
{
    fe_view v = fe_frame_specifier_decode(main_view, spec);

    STBDBG(("fe__printframes: spec %s v%p children %p\n", spec, v, v ? v->children : NULL));

    if (!v)
	return;

    if (v->children)
	backend_layout_write_table(f, v->displaying, fe__print_frames, spec, w, h);
    else
    {
	fprintf(f, msgs_lookup("printf1"), spec, print__first ? "CHECKED" : "");
	print__first = FALSE;
    }
}

static os_error *fe_print_frames_write_file(FILE *f, fe_view v, const char *size)
{
    int button = 0;

    STBDBG(("fe__print_frames_write_file: size %s v%p children %p\n", size, v, v->children));

    print__first = TRUE;
    
    /* Yuk! But can't think of a better way at the moment */
    if (size == NULL || size[0] == '\0')
	button = fevent_PRINT;
    else if (strcmp(size, "legal") == 0)
	button = fevent_PRINT_LEGAL;
    else if (strcmp(size, "letter") == 0)
	button = fevent_PRINT_LETTER;

    fprintf(f, msgs_lookup("printfT"), button, size, size);

    print_size = size;
    backend_layout_write_table(f, v->displaying, fe__print_frames, "_0", DBOX_SIZE_X-40, DBOX_SIZE_Y - 160);
    print_size = NULL;

    fputs(msgs_lookup("printfF"), f);

    return NULL;
}

static int internal_decode_print_frames(const char *query, char **new_url)
{
    char *size, *source;
    BOOL cancel;
    char *s, buffer[128];

    /* when called from the printframes dialogue box, this may have a
       cancel button */
    s = extract_value(query, "action=");
    cancel = strcasestr(s, "cancel") != 0;
    mm_free(s);

    if (cancel)
	return fe_internal_url_NO_ACTION;
    
    size = extract_value(query, "size=");
    source = extract_value(query, "source=");

    sprintf(buffer, "ncint:printpage?source=%s&size=%s", source, size);

    *new_url = strdup(buffer);

    mm_free(size);
    mm_free(source);

    return fe_internal_url_REDIRECT;
}

/* ----------------------------------------------------------------------------------------------------- */

static const char *msgs_lookup_null(const char *tag)
{
    const char *s = msgs_lookup((char *)tag);
    if (s && strcmp(s, tag) == 0)
	return NULL;
    return s;
}

void fe_internal_write_page(FILE *f, const char *base_tag, int initial, int frame, write_list_fn write_list, void *handle)
{
    char tag[16];
    int tag_point;
    const char *m;

    strcpy(tag, base_tag);
    tag_point = strlen(tag);
    if (frame != -1)
	tag_point += sprintf(tag + tag_point, ".%d", frame);

    strcpy(tag + tag_point, ".T");
    m = msgs_lookup_null(tag);
    if (m)
	fprintf(f, m, initial);

    strcpy(tag + tag_point, ".1");
    m = msgs_lookup_null(tag);
    if (m)
    {
	char *s = getenv(PROFILE_NAME_VAR);
	fprintf(f, m, strsafe(s));
    }

    strcpy(tag + tag_point, ".2");
    m = msgs_lookup_null(tag);
    if (m)
	fprintf(f, m);

    if ((frame == -1 || frame == 2) && write_list)
	write_list(f, handle);

    strcpy(tag + tag_point, ".F");
    m = msgs_lookup_null(tag);
    if (m)
	fprintf(f, m);
}


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

    hotlist_write_list(f, NULL);

    fputs(msgs_lookup("openhF"), f);
    fputc('\n', f);

    return NULL;
}

static os_error *fe_hotlist_write_file(FILE *f, BOOL switchable, int frame)
{
    fe_internal_write_page(f, switchable ? "favsw" : "favs", 0, frame, hotlist_write_list, NULL);
    return NULL;
}

static os_error *fe_hotlist_delete_write_file(FILE *f, BOOL switchable, int frame)
{
    fe_internal_write_page(f, switchable ? "favsdw" : "favsd", 0, frame, hotlist_write_delete_list, NULL);
    return NULL;
}

static os_error *fe_openurl_write_file(FILE *f, const char *def, const char *current)
{
    int width, height;

    fputs(msgs_lookup("open.T"), f);

    get_form_size(&width, &height);

    width -= 12;
    fprintf(f, msgs_lookup("open.1"), width, def);
    fprintf(f, msgs_lookup("open.2"), strsafe(loadurl_last), strsafe(current));
    fprintf(f, msgs_lookup("open.3"), strsafe(current), strsafe(current));

    fputs(msgs_lookup("open.F"), f);

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
	    coords_cvtstr cvt = frameutils_get_cvt(v);
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

static int internal_decode_find(const char *query)
{
    char *text, *dir, *casesense, *s, *source;
    BOOL cancel;
    fe_view v;

    source = extract_value(query, "source=");
    v = get_source_view(source, TRUE);
    mm_free(source);

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
	if (v->browser_mode == fe_browser_mode_HISTORY && cases && back && strcmp(buffer, text) == 0)
	{
	    ncreg_decode();
	}
	else
	{
	    fe_find(v, text, back, cases);
	}

	mm_free(text);
	mm_free(dir);
	mm_free(casesense);
    }

    return fe_internal_url_NO_ACTION;
}

static os_error *fe_find_write_file(FILE *f, const char *query)
{
    char *source = extract_value(query, "source=");

    fputs(msgs_lookup("findT"), f);
    fputc('\n', f);

    fprintf(f, msgs_lookup("find1"), strsafe(source));
    fprintf(f, msgs_lookup("find2"), strsafe(find__string), checked(find__casesense));
    fprintf(f, msgs_lookup("find3"), checked(!find__backwards),
        checked(find__backwards));

    fputs(msgs_lookup("findF"), f);
    fputc('\n', f);

    mm_free(source);

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

	e = fe_internal_url_with_source(v, "openpanel?name=find", TARGET_FIND);
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
        sound_event(snd_FIND_FAILED);
        frontend_complain(makeerror(ERR_FIND_FAILED));
    }
}

/* ------------------------------------------------------------------------------------------- */

#if 0
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
#endif

#if 0
#define NVRAM_FONTS	(0x131*8 + 0)
#define NVRAM_SOUND	(0x131*8 + 2)
#define NVRAM_BEEPS     (0x131*8 + 3)
#define NVRAM_SCALING     (0x131*8 + 3)
#endif

/* ------------------------------------------------------------------------------------------- */

static os_error *fe_custom_write_file(FILE *f, const char *tag, const char *nvram_tag, int n_vals, int def)
{
    char tag_buf[8];
    int val, i;

    if (!nvram_read(nvram_tag, &val))
	val = def;

    /* binary wotsits are written in reverse order (on,off rather than offf,on) */
    if (n_vals == 2)
	val = !val;

    sprintf(tag_buf, "m%sT", tag);
    fprintf(f, msgs_lookup(tag_buf), val);

    for (i = 0; i < n_vals; i++)
    {
	sprintf(tag_buf, "m%s%d", tag, i);
 	fprintf(f, msgs_lookup(tag_buf),
		val == i ? "radioon" : "radiooff",
		val == i ? "radioon1" : "radiooff1");
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
    char *scaling = extract_value(query, "scaling.");
    int generated = fe_internal_url_NO_ACTION;

    if (font)
    {
	int font_val = atoi(font);
	nvram_write(NVRAM_FONTS_TAG, font_val);

	fe_font_size_set(font_val, TRUE);

	*url = strdup("ncint:openpanel?name=customfonts");
	generated = fe_internal_url_REDIRECT;
    }

    if (sound)
    {
	int sound_val = !atoi(sound);
/* 	nvram_write(NVRAM_SOUND_TAG, sound_val); moved into fe_bgsound_set */

	fe_bgsound_set(sound_val);

	*url = strdup("ncint:openpanel?name=customsound");
	generated = fe_internal_url_REDIRECT;
    }

    if (beeps)
    {
	int beeps_val = !atoi(beeps);
	nvram_write(NVRAM_BEEPS_TAG, beeps_val);

	fe_beeps_set(beeps_val, FALSE);

	*url = strdup("ncint:openpanel?name=custombeeps");
	generated = fe_internal_url_REDIRECT;
    }

    if (scaling)
    {
	int scaling_val = !atoi(scaling);
	nvram_write(NVRAM_SCALING_TAG, scaling_val);

	fe_scaling_set(scaling_val);

	*url = strdup("ncint:openpanel?name=customscaling");
	generated = fe_internal_url_REDIRECT;
    }

    *flags = access_NOCACHE;

    mm_free(font);
    mm_free(sound);
    mm_free(beeps);
    mm_free(scaling);

    return generated;
}

static int internal_decode_hotlist_delete(const char *query)
{
    char *id = extract_value(query, "select.");
    char *source = extract_value(query, "source=");
    char *action = extract_value(query, "action=");

    fe_view v = fe_find_target(main_view, source);
    if (!v) v = fe_selected_view();
    if (!v) v = main_view;

    if (id)
    {

	strtok(id, "=");

	if (v && v->displaying)
	{
	    be_item ti = backend_locate_id(v->displaying, id);

	    STBDBG(("internal_decode_histlist_delete: v %p id '%s' ti %p\n", v, id, ti));

	    if (ti)
		backend_activate_link(v->displaying, ti, 0);
	}
    }
    else
    {
	hotlist_remove_list(query);

	if (action)
	{
	    hotlist_flush_pending_delete();
	    fe_dispose_view(v);
	}
    }

    mm_free(action);
    mm_free(id);
    mm_free(source);
    return fe_internal_url_NO_ACTION;
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
	name = extract_value(query, "uname=");
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

BOOL fe_passwd_abort(void)
{
    fe_passwd pw = fe_current_passwd;

    if (pw)
    {
        if (pw->cb)
            (pw->cb)(pw, pw->h, NULL, NULL);

        frontend_passwd_dispose(pw);

	return TRUE;
    }

    return FALSE;
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

    frontend_open_url("ncint:openpanel?name=password", NULL, TARGET_PASSWORD, NULL, fe_open_url_NO_CACHE);

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

#if SSL_UI
static int internal_decode_ssl(const char *query)
{
    char *s;
    fe_ssl fe;
    BOOL cancel;

    fe = fe_current_ssl;
    if (!fe)
        return fe_internal_url_ERROR;

    s = extract_value(query, "action=");
    cancel = strcasestr(s, "cancel") != 0;
    mm_free(s);

    if (fe->cb)
	fe->cb(fe->h, !cancel);
    
    frontend_ssl_dispose(fe);

    return fe_internal_url_NO_ACTION;
}

static os_error *fe_ssl_write_file(FILE *f)
{
    fe_ssl fe = fe_current_ssl;

    if (fe)
    {
	fputs(msgs_lookup("sslT"), f);
	fputc('\n', f);

	fprintf(f, msgs_lookup("ssl1"),
		fe->issuer ? fe->issuer : "unknown", 
		fe->subject ? fe->subject : "unknown");

	fputs(msgs_lookup("sslF"), f);
	fputc('\n', f);
    }

    return NULL;
}

fe_ssl frontend_ssl_raise(backend_ssl_callback cb, const fe_ssl_info *info, void *handle)
{
    fe_ssl fe;

    STBDBG(("ssl_raise subject '%s' issuer '%s' cipher '%s'\n",
	    strsafe(info->subject), strsafe(info->issuer), strsafe(info->cipher) ));

    fe = mm_calloc(sizeof(*fe), 1);

    fe->cb = cb;
    fe->h = handle;

    fe->subject = strdup(info->subject);
    fe->issuer = strdup(info->issuer);

    fe_current_ssl = fe;

    frontend_open_url("ncint:openpanel?name=ssl", NULL, TARGET_SSL, NULL, fe_open_url_NO_CACHE);

    return fe;
}

void frontend_ssl_dispose(fe_ssl fe)
{
    STBDBG(( "ssl_dispose %p\n", fe));

    fe_dispose_view(fe_locate_view(TARGET_SSL));

    mm_free(fe->subject);
    mm_free(fe->issuer);

    mm_free(fe);
    fe_current_ssl = NULL;
}
#endif

/* ----------------------------------------------------------------------------------------------------- */

static os_error *fe_error_write_file(FILE *f, const char *query)
{
    char *which = extract_value(query, "error=");
    char *again = extract_value(query, "again=");
    char *message = extract_value(query, "message=");
    char buffer[32], *s;

    STBDBG(("error: query '%s'\n", query));
    STBDBG(("error: which '%s'\n", which));
    STBDBG(("error: again '%s'\n", strsafe(again)));

    /* write out header, including error for reference on return */
    fprintf(f, msgs_lookup("errorT"), which, strsafe(again));

    /* write message */
    s = msgs_lookup(which);
    if (s && strcmp(s, which) == 0)
	write_url_with_breaks(f, message);
/* 	fprintf(f, "%s", message); */
    else if (s)
	write_url_with_breaks(f, s);
/* 	fprintf(f, strsafe(s)); */

    /* write button 1 */
    fputs(msgs_lookup("error1"), f);

    sprintf(buffer, "%s_0:", which);
    s = msgs_lookup(buffer);
    fprintf(f, msgs_lookup("errorB"), 0, s && s[0] ? s : msgs_lookup("continue"));

    /* write button 2 */
    sprintf(buffer, "%s_1:", which);
    if ((s = msgs_lookup(buffer)) != NULL && s[0])
    {
	fputs(msgs_lookup("error2"), f);
	fprintf(f, msgs_lookup("errorB"), 1, s);
    }

    fputs(msgs_lookup("errorF"), f);

    mm_free(which);
    mm_free(again);
    mm_free(message);

    return NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

static os_error *fe_mem_dump_write_file(FILE *f)
{
    int us = -1, next = -1, free;

    extern char   *flexptr__base;
    extern char   *flexptr__free;
    extern char   *flexptr__slot;
    extern void   *heap__base;
    extern int malloc_size, malloc_da, heap__size, heap__da, flex__da;

    int area = -1;


    fprintf(f, "<TITLE>NCFresco memory dump</TITLE>\n");
    fprintf(f, "<META NAME=ncbrowsermode CONTENT='position=fullscreen'>\n");
    fprintf(f, "<H1>NCFresco memory dump</H1>\n");

    fprintf(f, "<MENU><LI><A HREF='#wimp'>Wimp slot</A><LI><A HREF='#malloc'>Malloc heap</A><LI><A HREF='#image'>Image heap</A><LI><A HREF='#data'>Data heap</A><LI><A HREF='#da'>Dynamic areas</A></MENU>\n");

    wimp_slotsize(&us, &next, &free);
    fprintf(f, "<H2><A NAME='wimp'>Wimp slot usage</A></H2><TABLE>");
    fprintf(f, "<TR><TD>Us<TD ALIGN=RIGHT>%d bytes<TD ALIGN=RIGHT>%d K", us, us/1024);
    fprintf(f, "<TR><TD>Next<TD ALIGN=RIGHT>%d bytes<TD ALIGN=RIGHT>%d K", next, next/1024);
    fprintf(f, "<TR><TD>Free<TD ALIGN=RIGHT>%d bytes<TD ALIGN=RIGHT>%d K", free, free/1024);
    fprintf(f, "</TABLE>");

    fprintf(f, "<H2><A NAME='malloc'>Malloc heap</A></H2><PRE>");
    mm__dump(f);
    fprintf(f, "\nsummary: area %dK size %dK\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), malloc_da)/1024, malloc_size/1024);
    fprintf(f, "</PRE>");

    fprintf(f, "<H2><A NAME='image'>Image heap</A></H2><PRE>");
    heap__dump(f);
    fprintf(f, "\nsummary: area %dK size %dK top %dK\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), heap__da)/1024, heap__size/1024, ((int *)heap__base)[3]/1024);
    fprintf(f, "</PRE>");

    fprintf(f, "<H2><A NAME='data'>Flex memory</A></H2><PRE>");
    MemFlex_Dump(f);
    fprintf(f, "\nsummary: area %dK size %dK top %dK\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), flex__da)/1024, (flexptr__slot - flexptr__base)/1024, (flexptr__free - flexptr__base)/1024);
    fprintf(f, "</PRE>");

    fprintf(f, "<H2><A NAME='da'>Dynamic areas</A></H2><PRE>");
   
    do
    {
	_swix(OS_DynamicArea, _INR(0,1) | _OUT(1), 3, area, &area);
	if (area != -1)
	{
	    int size;
	    char *name;
	    _swix(OS_DynamicArea, _INR(0,1) | _OUT(2) | _OUT(8), 2, area, &size, &name);
	    
	    fprintf(f, "da: size %10dK name '%s'\n", size/1024, name);
	}
    }
    while (area != -1);
	
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
    else if (source[0] == '_' && isdigit(source[1]))
	v = fe_frame_specifier_decode(main_view, source);
    else
	v = fe_find_target(main_view, source);

    /* if it is a transient then fall back to main view */
    if (v && v->open_transient)
	v = main_view;

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

/*
 * New internal URL handlers
 */

static int internal_url_openpanel(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    char *panel_name = extract_value(query, "name=");
    char *mode = extract_value(query, "mode=");
    int frame;
    int generated = fe_internal_url_ERROR;
    fe_view v;

    if (panel_name == NULL)
	return generated;

    {
	char *frame_name = extract_value(query, "frame=");
	frame = frame_name ? atoi(frame_name) : -1;
	mm_free(frame_name);
    }

    if (strcasecomp(panel_name, "related") == 0)
    {
	int event = snd_WARN_BAD_KEY;
	char *url = NULL;

	if (fe_file_to_url(config_document_handler_related, &url) != NULL)
	{
	    sound_event(event);
	    return generated;
	}

	v = get_source_view(query, TRUE);

	STBDBG(("internal_url: related %s v%p handler '%s'\n", query, v, strsafe(url)));

	if (url && v && v->displaying)
	{
	    const char *match;

	    STBDBG(("internal_url: related %s\n", config_document_handler_related));

	    if ((match = backend_check_meta(v->displaying, "KEYWORDS")) != NULL ||
		(backend_doc_info(v->displaying, NULL, NULL, NULL, (char **)&match) == NULL && match))
	    {
		int related_len = strlen(url);
		int total_len = related_len + 3*strlen(match) + 2;
		char *s = mm_malloc(total_len);

		strcpy(s, url);

		url_escape_cat(s, match, total_len);

		*new_url = s;
		*flags |= access_NOCACHE;
		generated = fe_internal_url_REDIRECT;

		event = snd_RELATED_SHOW;
		STBDBG(("internal_url: related %s\n", s));
	    }
	}

	sound_event(event);

	mm_free(url);
    }
    else
    {
	FILE *f = mmfopen(file, "w");
	os_error *e = NULL;

	/* add a meta tag with the internal encoding used */
	write_charset(f);
	
	if (strcasecomp(panel_name, "displayoptions") == 0)
	{
	    sound_event(snd_DISPLAY_OPTIONS_SHOW);
	    e = fe_display_options_write_file(f);
	}
	else if (strcasecomp(panel_name, "favs") == 0)
	{
	    if (frame <= 0)
	    {
		sound_event(snd_HOTLIST_SHOW);
	    }

	    e = fe_hotlist_write_file(f, FALSE, frame);
	}
	else if (strcasecomp(panel_name, "favsdelete") == 0)
	{
	    if (frame <= 0)
	    {
		sound_event(snd_HOTLIST_DELETE_SHOW);
	    }
	    
	    e = fe_hotlist_delete_write_file(f, FALSE, frame);
	}
	else if (strcasecomp(panel_name, "favswitch") == 0)
	{
	    if (mode && strcasecomp(mode, "delete") == 0)
	    {
		if (frame <= 0)
		    sound_event(snd_HOTLIST_DELETE_SHOW);

		e = fe_hotlist_delete_write_file(f, TRUE, frame);
	    }
	    else
	    {
		if (frame <= 0)
		    sound_event(snd_HOTLIST_SHOW);
		
		e = fe_hotlist_write_file(f, TRUE, frame);
	    }
	}
	else if (strcasecomp(panel_name, "find") == 0)
	{
	    sound_event(snd_FIND_SHOW);
	    e = fe_find_write_file(f, query);
	}
	else if (strcasecomp(panel_name, "historyswitch") == 0)
	{
	    if (frame <= 0)
	    {
		sound_event(snd_HISTORY_SHOW);
	    }

	    if (mode && strcasecomp(mode, "alpha") == 0)
	    {
		e = fe_global_write_list(f, TRUE, frame);
	    }
	    else
	    {
		v = get_source_view(query, TRUE);
		if (v)
		    e = fe_history_write_list(f, v->last, v->hist_at, TRUE, frame);
	    }
	}
	else if (strcasecomp(panel_name, "historyalpha") == 0)
	{
	    if (frame <= 0)
	    {
		sound_event(snd_HISTORY_SHOW);
	    }
	    e = fe_global_write_list(f, FALSE, frame);
	}
	else if (strcasecomp(panel_name, "historyrecent") == 0)
	{
	    v = get_source_view(query, TRUE);
	    if (v)
	    {
		if (frame <= 0)
		{
		    sound_event(snd_HISTORY_SHOW);
		}
		e = fe_history_write_list(f, v->last, v->hist_at, FALSE, frame);
	    }
	}
	else if (strcasecomp(panel_name, "historycombined") == 0)
	{
	    v = get_source_view(query, TRUE);
	    if (v)
	    {
		sound_event(snd_HISTORY_SHOW);
		e = fe_history_write_combined_list(f, v->first, v->hist_at);
	    }
	}
	else if (strcasecomp(panel_name, "info") == 0)
	{
	    v = get_source_view(query, FALSE);
	    sound_event(snd_INFO_SHOW);
	    e = fe_version_write_file(f, v ? v->displaying : NULL, query);
	}
	else if (strcasecomp(panel_name, "memdump") == 0)
	{
	    e = fe_mem_dump_write_file(f);
	}
	else if (strcasecomp(panel_name, "printoptions") == 0)
	{
	    sound_event(snd_PRINT_OPTIONS_SHOW);

	    e = fe_print_options_write_file(f);
	}
	else if (strcasecomp(panel_name, "printframes") == 0)
	{
	    char *size = extract_value(query, "size=");

	    sound_event(snd_PRINT_FRAMES_SHOW);

	    v = get_source_view(query, TRUE);

 	    e = fe_print_frames_write_file(f, v, size);

	    mm_free(size);
	}
	else if (strcasecomp(panel_name, "password") == 0)
	{
	    sound_event(snd_PASSWORD_SHOW);
	    e = fe_passwd_write_file(f);
	}
#if SSL_UI
	else if (strcasecomp(panel_name, "ssl") == 0)
	{
	    sound_event(snd_PASSWORD_SHOW);
	    e = fe_ssl_write_file(f);
	}
#endif
	else if (strcasecomp(panel_name, "url") == 0)
	{
	    char *def = extract_value(query, "def=");
	    char *url = extract_value(query, "current=");

	    if (url == NULL)
	    {
		v = get_source_view(query, TRUE);
		if (v && v->displaying)
		    backend_doc_info(v->displaying, NULL, NULL, &url, NULL);
	    }
	    
	    sound_event(snd_OPEN_URL_SHOW);

	    e = fe_openurl_write_file(f, def ? def : msgs_lookup("opendef"), url);

	    mm_free(def);
	}
	else if (strcasecomp(panel_name, "urlfavs") == 0)
	{
	    sound_event(snd_HOTLIST_SHOW);
	    e = fe_hotlist_and_openurl_write_file(f);
	}
	else if (strcasecomp(panel_name, "customfonts") == 0)
	{
	    sound_event(snd_MENU_SHOW);
	    e = fe_custom_write_file(f, "fonts", NVRAM_FONTS_TAG, 3, 0);
	}
	else if (strcasecomp(panel_name, "customsound") == 0)
	{
	    sound_event(snd_MENU_SHOW);
	    e = fe_custom_write_file(f, "sound", NVRAM_SOUND_TAG, 2, config_sound_background);
	}
	else if (strcasecomp(panel_name, "custombeeps") == 0)
	{
	    sound_event(snd_MENU_SHOW);
	    e = fe_custom_write_file(f, "beeps", NVRAM_BEEPS_TAG, 2, config_sound_fx);
	}
	else if (strcasecomp(panel_name, "customscaling") == 0)
	{
	    sound_event(snd_MENU_SHOW);
	    e = fe_custom_write_file(f, "scaling", NVRAM_SCALING_TAG, 2, config_display_scale_fit);
	}
	else if (strcasecomp(panel_name, "error") == 0)
	{
	    sound_event(snd_ERROR);
	    e = fe_error_write_file(f, query);
	}
	else if (strcasecomp(panel_name, "options") == 0)
	{
	    char *option = extract_value(query, "option=");

	    sound_event(snd_MENU_SHOW);

 	    e = fe_options_write_file(f, option);

	    mm_free(option);
	}

	mmfclose(f);

	if (!e)
	{
	    set_file_type(file, FILETYPE_HTML);
	    generated = fe_internal_url_NEW;
	    *flags |= access_NOCACHE | access_MAX_PRIORITY;
	}
	frontend_complain(e);
    }
    mm_free(panel_name);
    mm_free(mode);

    return generated;
    NOT_USED(referer);
}

static int internal_url_loadurl(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    char *url = extract_value(query, "url=");
    int generated = fe_internal_url_NO_ACTION;

    if (url == NULL)
    {
	url = extract_value(query, "url.");
	if (url)
	    strtok(url, "=");
    }

    if (url && url[0])
    {
	char *nocache = extract_value(query, "opt=");
	char *remember = extract_value(query, "remember=");

	if (nocache && strcasestr(nocache, "nocache"))
	    *flags |= access_NOCACHE;
	else
	    *flags &= ~access_NOCACHE;

	*new_url = check_url_prefix(url);

	if (remember)
	{
	    mm_free(loadurl_last);
	    loadurl_last = strdup(*new_url);
	}

	mm_free(nocache);
	mm_free(remember);

	generated = fe_internal_url_REDIRECT;
    }

    mm_free(url);
    return generated;
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
	    sound_event(snd_HOME);
	    fe_file_to_url(config_doc_default, new_url);
	    generated = fe_internal_url_REDIRECT;
	}
	else
	{
	    sound_event(snd_WARN_BAD_KEY);
	}
    }
    else if (strcasecomp(page_name, "search") == NULL)
    {
	if (config_document_search)
	{
	    sound_event(snd_SEARCH);
	    fe_file_to_url(config_document_search, new_url);
	    generated = fe_internal_url_REDIRECT;
	}
	else
	{
	    sound_event(snd_WARN_BAD_KEY);
	}
    }
    else if (strcasecomp(page_name, "offline") == NULL)
    {
	if (config_document_offline)
	{
	    sound_event(snd_OFFLINE);
	    fe_file_to_url(config_document_offline, new_url);
	    generated = fe_internal_url_REDIRECT;
	}
	else
	{
	    sound_event(snd_WARN_BAD_KEY);
	}
    }

    mm_free(page_name);
    
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

static int internal_action_current(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
    {
	if (fe_history_move_recent_steps(main_view, 0, new_url))
	{
	    /* set history flag on current document */
	    backend_doc_set_flags(main_view->displaying, be_openurl_flag_HISTORY, be_openurl_flag_HISTORY);
	
	    /* clear check expire flag on current fetch */
	    *flags &= ~access_CHECK_EXPIRE;
	    
	    return fe_internal_url_REDIRECT;
	}
    }
    return fe_internal_url_NO_ACTION;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
}

static int internal_action_back(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
    {
	if (fe_history_move_recent_steps(main_view, -1, new_url))
	{
	    /* set history flag on current document */
	    backend_doc_set_flags(main_view->displaying, be_openurl_flag_HISTORY, be_openurl_flag_HISTORY);
	
	    /* clear check expire flag on current fetch */
	    *flags &= ~access_CHECK_EXPIRE;
	    
	    return fe_internal_url_REDIRECT;
	}
    }
    return fe_internal_url_NO_ACTION;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
}

static int internal_action_forward(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);

    if (v)
    {
	if (fe_history_move_recent_steps(main_view, +1, new_url))
	{
	    /* set history flag on current document */
	    backend_doc_set_flags(main_view->displaying, be_openurl_flag_HISTORY, be_openurl_flag_HISTORY);
	
	    /* clear check expire flag on current fetch */
	    *flags &= ~access_CHECK_EXPIRE;
	    
	    return fe_internal_url_REDIRECT;
	}
    }
    return fe_internal_url_NO_ACTION;
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

    if (v == main_view)
	fe_pending_event = fevent_RELOAD;
    else
	fevent_handler(fevent_RELOAD, v);

    return fe_internal_url_NO_ACTION;
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

    return fe_internal_url_NO_ACTION;
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

    return fe_internal_url_NO_ACTION;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

static int internal_action_printpage(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    fe_view v = get_source_view(query, TRUE);
    char *size;
    int generated = fe_internal_url_NO_ACTION;
    BOOL legal;
    char *s;
    BOOL cancel;

    if (!v)
	return fe_internal_url_NO_ACTION;

    /* when called from the printframes dialogue box, this may have a
       cancel button */
    s = extract_value(query, "action=");
    cancel = strcasestr(s, "cancel") != 0;
    mm_free(s);

    if (cancel)
	return fe_internal_url_NO_ACTION;
    
    size = extract_value(query, "size=");
    legal = size && strcasecomp(size, "legal") == 0;
    frontend_complain(fe_print(v, legal ? fe_print_LEGAL : fe_print_LETTER));

    mm_free(size);

    return generated;
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
    char *id = extract_value(query, "id=");
    char *source = extract_value(query, "source=");
    fe_view v = fe_find_target(main_view, source);
    if (!v) v = fe_selected_view();
    if (!v) v = main_view;

    if (v && v->displaying && id)
    {
	be_item ti = backend_locate_id(v->displaying, id);

	STBDBG(("internal_action_select: v %p id '%s' ti %p\n", v, id, ti));

	if (ti)
	    backend_activate_link(v->displaying, ti, 0);
    }

    mm_free(id);
    mm_free(source);

    return fe_internal_url_NO_ACTION;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

static int internal_action_submit(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    char *id = extract_value(query, "id=");
    char *source = extract_value(query, "source=");
    fe_view v = fe_find_target(main_view, source);
    if (!v) v = fe_selected_view();
    if (!v) v = main_view;

    if (v && v->displaying && id)
    {
	STBDBG(("internal_action_submit: v %p id '%s'\n", v, id));

	backend_submit_form(v->displaying, id, FALSE);
    }

    mm_free(id);
    mm_free(source);

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
    int event;

    if (bar == NULL || !use_toolbox)
	return generated;

    event = tb_bar_get_num_from_name(bar);

    if (event != -1)
    {
	fe_view v = get_source_view(query, TRUE);

	if (v)
	    fevent_handler(event + fevent_TOOLBAR_MAIN, v);

	generated = fe_internal_url_HELPER;
    }

    return generated;
    NOT_USED(bfile);
    NOT_USED(referer);
    NOT_USED(file);
    NOT_USED(flags);
}

/* ------------------------------------------------------------------------------------------- */

static int internal_action_crash(const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
#if DEBUG
    *(int *)(-1) = 0;
#endif
    return fe_internal_url_NO_ACTION;
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

/* ----------------------------------------------------------------------------------------------------- */

static int internal_decode_error(const char *query, char **new_url, int *flags)
{
    char *which = extract_value(query, "error=");
    char *action = extract_value(query, "action=");
    int generated = fe_internal_url_NO_ACTION;

    if (strcmp(which, "E80acf") == 0)
    {
	if (strcasecomp(action, "cancel") != 0)
	{
	    /* try the print again */
	    *new_url = extract_value(query, "again=");
	    *flags |= access_NOCACHE;
	    generated = fe_internal_url_REDIRECT;
	}
    }

    mm_free(action);
    mm_free(which);

    return generated;
}

/* ----------------------------------------------------------------------------------------------------- */

static int internal_decode_hotlist(const char *query, char **new_url, int *flags)
{
    char *which = extract_value(query, "url.");

    hotlist_return_url(atoi(which), new_url);

    mm_free(which);

    return fe_internal_url_REDIRECT;
}

/* ----------------------------------------------------------------------------------------------------- */

static int internal_decode_history_alpha(const char *query, char **new_url, int *flags)
{
    char *which = extract_value(query, "url.");
    int generated = fe_internal_url_NO_ACTION;

    if (fe_history_move_alpha_index(main_view, atoi(which), new_url))
    {
	/* set history flag on current document */
	backend_doc_set_flags(main_view->displaying, be_openurl_flag_HISTORY, be_openurl_flag_HISTORY);

	/* clear check expire flag on current fetch */
	*flags &= ~access_CHECK_EXPIRE;

	generated = fe_internal_url_REDIRECT;
    }

    mm_free(which);

    return generated;
}

/* ----------------------------------------------------------------------------------------------------- */

static int internal_decode_history_recent(const char *query, char **new_url, int *flags)
{
    char *which = extract_value(query, "url.");
    int generated = fe_internal_url_NO_ACTION;

    if (fe_history_move_recent_index(main_view, atoi(which), new_url))
    {
	/* set history flag on current document */
	backend_doc_set_flags(main_view->displaying, be_openurl_flag_HISTORY, be_openurl_flag_HISTORY);
	
	/* clear check expire flag on current fetch */
	*flags &= ~access_CHECK_EXPIRE;

	generated = fe_internal_url_REDIRECT;
    }

    mm_free(which);

    return generated;
}

/* ----------------------------------------------------------------------------------------------------- */

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
    else if (strcasecomp(page, "favs") == 0)
    {
	generated = internal_decode_hotlist(query, new_url, flags);
    }
    else if (strcasecomp(page, "favsdelete") == 0)
    {
	internal_decode_hotlist_delete(query);
    }
    else if (strcasecomp(page, "displayoptions") == 0)
    {
	internal_decode_display_options(query);
    }
    else if (strcasecomp(page, "printoptions") == 0)
    {
	generated = internal_decode_print_options(query, new_url);
    }
    else if (strcasecomp(page, "printframes") == 0)
    {
	generated = internal_decode_print_frames(query, new_url);
    }
    else if (strcasecomp(page, "find") == 0)
    {
	generated = internal_decode_find(query);
    }
    else if (strcasecomp(page, "password") == 0)
    {
	generated = internal_decode_password(query);
    }
#if SSL_UI
    else if (strcasecomp(page, "ssl") == 0)
    {
	generated = internal_decode_ssl(query);
    }
#endif
    else if (strcasecomp(page, "error") == 0)
    {
	generated = internal_decode_error(query, new_url, flags);
    }
    else if (strcasecomp(page, "historyalpha") == 0)
    {
	generated = internal_decode_history_alpha(query, new_url, flags);
    }
    else if (strcasecomp(page, "historyrecent") == 0)
    {
	generated = internal_decode_history_recent(query, new_url, flags);
    }
    else if (strcasecomp(page, "options") == 0)
    {
	generated = internal_decode_options(query, new_url);
    }

    mm_free(page);

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
    { "submit", internal_action_submit },

    /* These are used to process the results of generated pages. Not usually set by the user */
    { "cancel", internal_decode_cancel },
    { "process", internal_decode_process },

    { "back", internal_action_back },
    { "forward", internal_action_forward },
    { "current", internal_action_current },
    { "close", internal_action_close },
    { "playmovie", internal_action_playmovie },
    { "stop", internal_action_stop },
    { "reload", internal_action_reload },
    { "favsadd", internal_action_favoritesadd },
    { "favsremove", internal_action_favoritesremove },

    { "crash", internal_action_crash },

    { 0, 0 }
};

int frontend_internal_url(const char *path, const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags)
{
    internal_url_str *uu;
    int generated = fe_internal_url_ERROR;
    char *remove = extract_value(query, "remove=");
    char *toolbar = extract_value(query, "toolbar=");

    STBDBG(("frontend_internal_url(): action '%s'\n", path));

    if (toolbar)
    {
	int tb = atoi(toolbar);
	int off = strcasecomp(toolbar, "off") == 0;
	fe_status_state(main_view, !off);
	fe_status_open_toolbar(main_view, fevent_TOOLBAR_MAIN + tb);
	mm_free(toolbar);
    }

    for (uu = internal_url_info; uu->name; uu++)
    {
	if (strcasecomp(uu->name, path) == 0)
	{
	    STBDBG(("frontend_internal_url(): calling fn %p\n", uu->fn));

	    generated = uu->fn(query, bfile, referer, file, new_url, flags);
	    break;
	}
    }

    if (remove)
    {
	fe_dispose_view(fe_locate_view(remove));
	mm_free(remove);
	remove = NULL;

	/* if we are doing nothing else other than removing something set the action to none */
	if (path[0] == '\0')
	    generated = fe_internal_url_NO_ACTION;
    }
    
    return generated;
}

/* ------------------------------------------------------------------------------------------- */

/*
 * special internal actions on removeall of particular pages
 */

void fe_internal_deleting_view(fe_view v)
{
    /* check for unload action */
    if (v->onunload)
    {
	frontend_open_url(v->onunload, v, NULL, NULL, 0);
	mm_free(v->onunload);
	v->onunload = NULL;
    }

    if (v->submitonunload)
    {
 	backend_submit_form(v->displaying, v->submitonunload, FALSE); 
	mm_free(v->submitonunload);
	v->submitonunload = NULL;
    }

    /* generic button unhighlighter */
    if (v->select_button && use_toolbox)
    {
	tb_status_button(v->select_button, tb_status_button_INACTIVE);

	if (pointer_mode == pointermode_OFF && tb_is_status_showing() && !tb_status_has_highlight())
	    tb_status_button(v->select_button, tb_status_button_PRESSED);
    }
    
    /* check for special stuff */
    if (strcasecomp(v->name, TARGET_ERROR) == 0 ||
	strncmp(v->name, TARGET_DBOX, sizeof(TARGET_DBOX)-1) == 0)
    {
	/* move highlight to default spot on toolbar (down arrow) */
 	if (pointer_mode == pointermode_OFF)
	{
	    if (use_toolbox && tb_is_status_showing())
		tb_status_highlight(TRUE);
	    else
		fe_ensure_highlight(v->prev, 0);
	}
    }
}

/* void fe_internal_opening_view(fe_view v, const char *url) */
/* { */
/* } */

os_error *fe_internal_toggle_panel(const char *panel_name, int clear)
{
    return fe_internal_toggle_panel_args(panel_name, NULL, clear);
}

os_error *fe_internal_toggle_panel_args(const char *panel_name, const char *args, int clear)
{
    char url[48];
    char target[16];
    fe_view v;
    os_error *e = NULL;

    strcpy(url, "ncint:openpanel?name=");
    strcat(url, panel_name);
    if (args)
    {
	strcat(url, "&");
	strcat(url, args);
    }

    strcpy(target, "__");
    strcat(target, panel_name);

    /* if this view name is already open then close it */
    if ((v = fe_locate_view(target)) != NULL)
    {
	sound_event(snd_MENU_HIDE);
	fe_dispose_view(v);

	/* if the on screen keyboard is up at the same time as a panel
           then close them together */
	if (on_screen_kbd)
	    fe_keyboard_close();

	/* if the user didn't request the status bar open then we must
           have opened it with the popup in which case we want to
           close it again */
	if (use_toolbox && tb_is_status_showing() && !fe_status_open(main_view))
	    fe_status_state(main_view, FALSE);

	return NULL;
    }

    /* if any different popup is open then close it */
    if (fe_internal_check_popups(clear))
    {
	/* force open the toolbar */
	if (use_toolbox && !tb_is_status_showing())
	    fe_status_state(main_view, TRUE);
	    
	e = frontend_open_url(url, NULL, target, NULL, fe_open_url_NO_CACHE | fe_open_url_NO_ENCODING_OVERRIDE);
    }

    return e;
}

/* ------------------------------------------------------------------------------------------- */

BOOL fe_internal_check_popups(BOOL clear)
{
    fe_view popup;

    /* check for closing other window or aborting */
    popup = main_view->next;
    if (clear)
    {
	if (popup)
	    fe_dispose_view(popup);
	if (on_screen_kbd)
	    fe_keyboard_close();
    }
    else if (popup || on_screen_kbd != 0)
    {
	sound_event(snd_WARN_BAD_KEY);
	return FALSE;
    }
    return TRUE;
}

/* ------------------------------------------------------------------------------------------- */

os_error *fe_open_version(fe_view v)
{
    fe_open_info(v, backend_read_highlight(v->displaying, NULL), 0, 0, TRUE);
    return NULL;
}

/* ------------------------------------------------------------------------------------------- */

os_error *fe_internal_open_page(fe_view v, const char *page_name, int clear)
{
    char url[48];

    strcpy(url, "ncint:openpage?name=");
    strcat(url, page_name);

    if (fe_internal_check_popups(clear))
	return frontend_open_url(url, v, NULL, NULL, fe_open_url_NO_REFERER);

    return NULL;
}

/* ------------------------------------------------------------------------------------------- */

/* Dispose of saved strings. This is to allow better garbage
 * collection and for user privacy. */

void fe_internal_flush(void)
{
    mm_free(loadurl_last);
    loadurl_last = NULL;

    mm_free(find__string);
    find__string = NULL;

    find__backwards = find__casesense = FALSE;
}

void fe_internal_optimise(void)
{
    loadurl_last = optimise_string(loadurl_last);
    find__string = optimise_string(find__string);
}

/* ------------------------------------------------------------------------------------------- */

void fe_special_select(fe_view v)
{
    if (v && v->displaying && v->specialselect)
    {
	be_item ti = backend_locate_id(v->displaying, v->specialselect);
	if (ti)
	    frontend_complain(backend_activate_link(v->displaying, ti, 0));
	else
	    backend_submit_form(v->displaying, v->specialselect, 0);
    }
}

/* ------------------------------------------------------------------------------------------- */

/* eof internal.c */
