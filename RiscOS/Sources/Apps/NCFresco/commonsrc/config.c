/* -*-c-*- */

/* config.c */

/* The data stored in variables in this file are to be loaded from
 * some sort of a config file at start-up.  In general they are given
 * default values here so that I do not have to write the config file
 * reading code yet.

 * ChangeLog
 * NvS: 26/01/96:	Incorporated Si's changes and split out gui code
 */

#ifdef CBPROJECT
#define CONFIG_FILE_NAME    "Resources:$.ThirdParty.Fresco.Config"
#else
#define CONFIG_FILE_NAME    "<"PROGRAM_NAME"$Config>"
#endif

#define CONFIG_FILE_NAME_2  PROGRAM_NAME":Config"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "memwatch.h"

#include "swis.h"
#include "os.h"
#include "config.h"
#include "configfile.h"
#include "wimp.h"

#include "util.h"
#include "filetypes.h"
#include "rcolours.h"
#include "url.h"
#include "myassert.h"
#include "version.h"

#ifndef DEBUG
#define DEBUG 0
#endif

static void config_default_first(void);
static void config_read_bool(const char *p, config_item *citem);

#ifdef STBWEB
static void config_read_variables(void);
#endif

int config_has_been_changed;
/**********************************************************************/
/* This is the global config variables */

struct config_str config_array;

/**********************************************************************

 !!    !!    !!    !!    !!    !!    !!    !!    !!    !!    !!    !!

 NOTE:  This table MUST be kept in step with citemsdbs in configgui.c

 !!    !!    !!    !!    !!    !!    !!    !!    !!    !!    !!    !!

**********************************************************************/

config_item citems[] = {
{ config_COMMENT, NULL, NULL, "Config file for Fresco\256", NULL },

{ config_COMMENT, NULL, NULL, "", NULL },
{ config_INT,	"image.max", 		(void *)offsetof(struct config_str, max_files_fetching),	"Maximum number of image files in transit at one time", (void *) 6  },
{ config_BOOL,	"image.deep", 		(void *)offsetof(struct config_str, deep_images),		"Do we want deep colour for images?", (void *) 0  },
{ config_BOOL,	"image.defer", 		(void *)offsetof(struct config_str, defer_images),		"Don't load images until the user asks", (void *) 0  },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_BOOL,	"display.links.underlined",	(void *)offsetof(struct config_str, display_links_underlined),	"Underline links?", (void *) 0  },
{ config_BOOL,	"display.body_colours",		(void *)offsetof(struct config_str, display_body_colours),		"Use colour and background directives in BODY tags", (void *) 1  },
{ config_BOOL,	"display.antialias",		(void *)offsetof(struct config_str, display_antialias),		"Antialias the displayed text", (void *) 1  },
{ config_BOOL,	"display.control.buttons",	(void *)offsetof(struct config_str, display_control_buttons),	"Display button bar", (void *) 1  },
{ config_BOOL,	"display.control.urlline",	(void *)offsetof(struct config_str, display_control_urlline),	"Display URL line", (void *) 1  },
{ config_BOOL,	"display.control.status",	(void *)offsetof(struct config_str, display_control_status),		"Display status line", (void *) 1  },
{ config_BOOL,	"display.control.top",	(void *)offsetof(struct config_str, display_control_top),		"Display tool bar at top", (void *) 1  },
{ config_INT,	"display.control.initial",	(void *)offsetof(struct config_str, display_control_initial),		"Initial tool bar", (void *) 8  },
{ config_BOOL,	"display.fancy_ptr",		(void *)offsetof(struct config_str, display_fancy_ptr),		"Use different pointer shapes depending on what the pointer is over", (void *) 1 },
{ config_INT,	"display.scale",	(void *)offsetof(struct config_str, display_scale),		"Scaling factor (%)", (void *) 100  },
{ config_INT,	"display.scale.image",	(void *)offsetof(struct config_str, display_scale_image),	"Scaling factor for images (%)", (void *) 100 },
{ config_INT,	"display.scale.1",	(void *)offsetof(struct config_str, display_scales[0]),	"Scale level 1", (void *)  80 },
{ config_INT,	"display.scale.2",	(void *)offsetof(struct config_str, display_scales[1]),	"Scale level 2", (void *) 100 },
{ config_INT,	"display.scale.3",	(void *)offsetof(struct config_str, display_scales[2]),	"Scale level 3", (void *) 125 },
{ config_INT,	"display.width",	(void *)offsetof(struct config_str, display_width),		"Default page width in characters", (void *) 71  },
{ config_BOOL,	"display.blending",	(void *)offsetof(struct config_str, display_blending),	"Blend fonts to background", (void *) 0  },
{ config_INT,	"display.margin.left",	(void *)offsetof(struct config_str, display_margin.x0),		"Margin left (%)", (void *) 0 },
{ config_INT,	"display.margin.right",	(void *)offsetof(struct config_str, display_margin.x1),		"Margin right (%)", (void *) 0 },
{ config_INT,	"display.margin.top",	(void *)offsetof(struct config_str, display_margin.y1),	"Margin top (%)", (void *) 0 },
{ config_INT,	"display.margin.bottom",	(void *)offsetof(struct config_str, display_margin.y0),	"Margin bottom (%)", (void *) 0 },
{ config_BOOL,	"display.margin.auto",	(void *)offsetof(struct config_str, display_margin_auto),	"Use ROM values", (void *) 1 },
{ config_BOOL,  "display.frames", 	        (void *)offsetof(struct config_str, display_frames),	"Display frames", (void *)0  },
{ config_BOOL,  "display.frames.scrollbars",    (void *)offsetof(struct config_str, display_frames_scrollbars),	"Display scrollbars with frames", (void *)1  },
{ config_BOOL,  "display.smooth_scrolling", 	(void *)offsetof(struct config_str, display_smooth_scrolling),	"Use smooth scrolling", (void *)0  },
{ config_INT,   "display.time.activate", 	(void *)offsetof(struct config_str, display_time_activate),	"Time to activate links for", (void *)50 },
{ config_INT,   "display.time.background", 	(void *)offsetof(struct config_str, display_time_background),	"Time for which to wait for background", (void *)0 },
{ config_INT,   "display.time.fade", 	(void *)offsetof(struct config_str, display_time_fade),	"Time to fade page out", (void *)0 },
{ config_BOOL,	"display.map_coords",		(void *)offsetof(struct config_str, display_map_coords),	"Display map co-ordinates on the status line",	(void *) 0 },
{ config_INT,	"display.jpeg",	(void *)offsetof(struct config_str, display_jpeg),		"0,1,3: on the fly, 2: decompress", (void *) 2 },
{ config_INT,	"display.leading",	(void *)offsetof(struct config_str, display_leading),		"half leading", (void *) 25 },
{ config_BOOL,	"display.leading.percent",	(void *)offsetof(struct config_str, display_leading_percent), "% or OS units", (void *) 1 },
{ config_INT,	"display.char.password",	(void *)offsetof(struct config_str, display_char_password), "password char", (void *) '-' },
{ config_INT,	"display.encoding",	(void *)offsetof(struct config_str, display_encoding), "wide font encoding", (void *)0 },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_URL,	"document.default",	(void *)offsetof(struct config_str, doc_default),		"Default document",		"Welcome"  },
{ config_URL,	"document.handler.user",	(void *)offsetof(struct config_str, document_handler_user),	       "Handler URL",		NULL  },
{ config_URL,	"document.handler.related",	(void *)offsetof(struct config_str, document_handler_related),	       "URL to fetch related stuff",		NULL  },
{ config_URL,	"document.search",	(void *)offsetof(struct config_str, document_search),	       "URL to search from",		NULL  },
{ config_URL,	"document.offline",	(void *)offsetof(struct config_str, document_offline),	       "URL of offline menu",		NULL  },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_FILE,	"files.hots", 		(void *)offsetof(struct config_str, hotlist_file),		"Hotlist file",		"InetDBase:Hotlist"  },
{ config_FILE,	"files.passwords", 	(void *)offsetof(struct config_str, auth_file),		"File to store passwords in",	"<Fresco$Dir>.Users"  },
{ config_BOOL,	"files.passwords.encrypted", 	(void *)offsetof(struct config_str, auth_file_crypt),"Store passwords scrambled",	(void *) 0  },
{ config_FILE,	"files.runable",	(void *)offsetof(struct config_str, runable_file),		"This file lists the file types of files that will be run when fetched.",	"<"PROGRAM_NAME"$Dir>.Runables" },
{ config_FILE,	"files.allow", 	(void *)offsetof(struct config_str, allow_file),		"If given, only allow access to sites listed in this file.",	NULL },
{ config_FILE,	"files.deny", 	(void *)offsetof(struct config_str, deny_file),		"If given, don't allow access to sites listed in this file.",	NULL },
{ config_FILE, "files.Help",	(void *)offsetof(struct config_str, help_file),              "URL of help pages",	NULL  }, /* SJM */
{ config_FILE,  "files.cookie",	(void *)offsetof(struct config_str, cookie_file),              "Where to store the users cookies",	"<"PROGRAM_NAME"$Dir>.Cookies" },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_INT,	"truncate.length", 	(void *)offsetof(struct config_str, truncate_length),	"Number of chars to truncate file names to", 0  },
{ config_BOOL,	"truncate.suffix", 	(void *)offsetof(struct config_str, truncate_suffix),	"Do we remove the file extension when we choose a file name?", 0  },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_STRING, "user.email", 		(void *)offsetof(struct config_str, email_addr),		"User's email address", 0  },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_STRING,"animation.name",	(void *)offsetof(struct config_str, animation_name),		"Prefix string for animation sprite names", "prog"  },
{ config_INT,	"animation.frames",	(void *)offsetof(struct config_str, animation_frames),	"Number of frames in the progress amination", (void *) 12  },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_INT,	"font.size.1", 		(void *)offsetof(struct config_str, font_sizes[0]), 		"Point sizes for fonts",	(void *) 8  },
{ config_INT,	"font.size.2", 		(void *)offsetof(struct config_str, font_sizes[1]),		0,				(void *) 10  },
{ config_INT,	"font.size.3", 		(void *)offsetof(struct config_str, font_sizes[2]),		0,				(void *) 12  },
{ config_INT,	"font.size.4", 		(void *)offsetof(struct config_str, font_sizes[3]),		0,				(void *) 14  },
{ config_INT,	"font.size.5",		(void *)offsetof(struct config_str, font_sizes[4]),		0,				(void *) 16  },
{ config_INT,	"font.size.6", 		(void *)offsetof(struct config_str, font_sizes[5]),		0,				(void *) 18  },
{ config_INT,	"font.size.7",		(void *)offsetof(struct config_str, font_sizes[6]),		0,				(void *) 24  },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_FONT, "font.base",	(void *)offsetof(struct config_str, font_names[0]),	"Name of standard font",	"Trinity.Medium"  },
{ config_FONT, "font.base.b",	(void *)offsetof(struct config_str, font_names[1]),	"... the bold form",	"Trinity.Bold"  },
{ config_FONT, "font.base.i",	(void *)offsetof(struct config_str, font_names[2]),	"... the italic form",	"Trinity.Medium.Italic"  },
{ config_FONT, "font.base.bi",	(void *)offsetof(struct config_str, font_names[3]),	"... bold and italic",	"Trinity.Bold.Italic"  },
{ config_FONT, "font.fixed",	(void *)offsetof(struct config_str, font_names[4]),	"Name of fixed space font",	"Corpus.Medium"  },
{ config_FONT, "font.fixed.b",	(void *)offsetof(struct config_str, font_names[5]),	"... the bold form",	"Corpus.Bold"  },
{ config_FONT, "font.fixed.i",	(void *)offsetof(struct config_str, font_names[6]),	"... the italic form",	"Corpus.Medium.Oblique"  },
{ config_FONT, "font.fixed.bi",	(void *)offsetof(struct config_str, font_names[7]),	"... bold and italic",	"Corpus.Bold.Oblique"  },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_COLOUR, "colour.plain",	(void *)offsetof(struct config_str, colours[0]), "Colour for normal text", 				(void *) 0x00000000  },
{ config_COLOUR, "colour.anchor",	(void *)offsetof(struct config_str, colours[1]), "Colour for text in hypertext links",		(void *) 0xdd220000  },
{ config_COLOUR, "colour.visited",	(void *)offsetof(struct config_str, colours[2]), "Colour for text in links that have been visited",	(void *) 0x88220000  },
{ config_COLOUR, "colour.back",		(void *)offsetof(struct config_str, colours[3]), "Background colour",				(void *) 0xdddddd00  },
{ config_COLOUR, "colour.action",	(void *)offsetof(struct config_str, colours[4]), "Colour for for action buttons",			(void *) 0x00bbff00  },
{ config_COLOUR, "colour.write",	(void *)offsetof(struct config_str, colours[5]), "Colour for writable areas",			(void *) 0xffffff00  },
{ config_COLOUR, "colour.line.light",	(void *)offsetof(struct config_str, colours[6]), "Colour for the light side of rules",		(void *) 0xffffff00  },
{ config_COLOUR, "colour.line.dark",	(void *)offsetof(struct config_str, colours[7]), "Colour for the dark side of rules",		(void *) 0x55555500  },
{ config_COLOUR, "colour.button.text",	(void *)offsetof(struct config_str, colours[8]), "Colour for text on buttons",			(void *) 0x00000000  },
{ config_COLOUR, "colour.button.back",	(void *)offsetof(struct config_str, colours[9]), "Background colour for buttons",			(void *) 0xdddddd00  },
{ config_COLOUR, "colour.highlight",	(void *)offsetof(struct config_str, colours[10]), "Colour for highlighted link",			(void *) 0xff000000  },
{ config_COLOUR, "colour.activated",	(void *)offsetof(struct config_str, colours[10]), "Colour for activating link",			(void *) 0x0000ff00  },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_BOOL,	"proxy.http.enable",	(void *)offsetof(struct config_str, proxy_http_on),	"Enable proxies",	(void *) 0  },
{ config_BOOL,	"proxy.https.enable",	(void *)offsetof(struct config_str, proxy_https_on),	NULL,	                (void *) 0  },
{ config_BOOL,	"proxy.gopher.enable",	(void *)offsetof(struct config_str, proxy_gopher_on),	NULL,		        (void *) 0  },
{ config_BOOL,	"proxy.ftp.enable",	(void *)offsetof(struct config_str, proxy_ftp_on),	NULL,			(void *) 0  },
{ config_BOOL,	"proxy.mailto.enable",	(void *)offsetof(struct config_str, proxy_mailto_on),	NULL,			(void *) 0  },
{ config_STRING, "proxy.http", 		(void *)offsetof(struct config_str, proxy_http),	"Proxy server name (and port)",	0  },
{ config_STRING, "proxy.https", 	(void *)offsetof(struct config_str, proxy_https),	NULL,	                        0  },
{ config_STRING, "proxy.gopher", 	(void *)offsetof(struct config_str, proxy_gopher),	NULL,				0  },
{ config_STRING, "proxy.ftp", 		(void *)offsetof(struct config_str, proxy_ftp),	NULL,				0  },
{ config_URL, "proxy.mailto", 		(void *)offsetof(struct config_str, proxy_mailto),	"Proxy URL",				0  },
{ config_STRING, "proxy.http.ignore", 	(void *)offsetof(struct config_str, proxy_http_ignore),	"No proxy on these domains",	0  },
{ config_STRING, "proxy.https.ignore", 	(void *)offsetof(struct config_str, proxy_https_ignore),        NULL,	                0  },
{ config_STRING, "proxy.gopher.ignore", (void *)offsetof(struct config_str, proxy_gopher_ignore),	NULL,	                0  },
{ config_STRING, "proxy.ftp.ignore", 	(void *)offsetof(struct config_str, proxy_ftp_ignore),	NULL,				0  },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_INT,	"cache.size",	(void *)offsetof(struct config_str, cache_size),	"Size in K for cache (0 = unlimited)",		(void *) 0  },
{ config_INT,	"cache.items",	(void *)offsetof(struct config_str, cache_items),	"Max number of items in the cache",		(void *) 48  },
{ config_BOOL,	"cache.keep",	(void *)offsetof(struct config_str, cache_keep),	"Should the cache be kept across closedowns?",	(void *) 0  },
{ config_BOOL,	"cache.keep.uptodate",	(void *)offsetof(struct config_str, cache_keep_uptodate), "Save the cachedump on every update",	(void *) 0 },
{ config_COMMENT, NULL, NULL, "", NULL },
{ config_INT,	"print.scale",	(void *)offsetof(struct config_str, print_scale),	"Printing magnification in %",		(void *) 100  },
{ config_INT,	"print.border",	(void *)offsetof(struct config_str, print_border),	"Border, in points, around the printed page",	(void *) 36  },
{ config_BOOL,	"print.nopics",	(void *)offsetof(struct config_str, print_nopics),	"Surpress pictures in printout",	(void *) 0  },
{ config_BOOL,	"print.noback",	(void *)offsetof(struct config_str, print_nobg),	"Surpress backgrouds in printout",	(void *) 0  },
{ config_BOOL,	"print.nocols",	(void *)offsetof(struct config_str, print_nocol),	"Surpress text colours in printout",	(void *) 0  },
{ config_BOOL,	"print.sideways",	(void *)offsetof(struct config_str, print_sideways),	"Print pages sideways",		(void *) 0  },
{ config_BOOL,	"print.collated",	(void *)offsetof(struct config_str, print_collated),	"Collate multiple copies",	(void *) 0  },
{ config_BOOL,	"print.reversed",	(void *)offsetof(struct config_str, print_reversed),	"Print pages in reverse order",	(void *) 0  },
{ config_STRING, "sound.click", 	(void *)offsetof(struct config_str, sound_click),	"Command to run when selecting", (void *)0  },
{ config_BOOL, "sound.fx", 	(void *)offsetof(struct config_str, sound_fx),	"User audio feedback sounds", (void *)0  },
{ config_BOOL, "sound.background", 	(void *)offsetof(struct config_str, sound_background),	"Play background sounds", (void *)0  },
{ config_INT, "mode.keyboard", 	(void *)offsetof(struct config_str, mode_keyboard),	"Are we mouseless", (void *)1  },
{ config_BOOL, "mode.cursor.toolbar", 	(void *)offsetof(struct config_str, mode_cursor_toolbar),	"Move cursor to toolbar", (void *)1  },
{ config_INT, "mode.platform", 	(void *)offsetof(struct config_str, mode_platform),	"Platform type", (void *)0  },
{ config_INT,	"history.length",	(void *)offsetof(struct config_str, history_length),	"The number of pages in the per-view history list",	(void *) 50 },
{ config_INT,	"history.global.length",	(void *)offsetof(struct config_str, history_global_length),	"The number of unique pages in the global history list",	(void *) 250 },
{ config_BOOL, "broken.formpost", 	(void *)offsetof(struct config_str, broken_formpost),	"Set this to use Netscape POST redirections", (void *)0  },
{ config_BOOL, "cookie.enable", 	(void *)offsetof(struct config_str, cookie_enable),	"Shall we receive and send cookies", (void *)1 },
{ config_BOOL, "cookie.uptodate", 	(void *)offsetof(struct config_str, cookie_uptodate),	"Save cookies on every update", (void *)0 },
{ config_BOOL, "passwords.uptodate", 	(void *)offsetof(struct config_str, passwords_uptodate),	"Save passwords on every update", (void *)0 },
{ config_FILE, "plugin.file", 	(void *)offsetof(struct config_str, plugin_file),	"Configure file for plugins", "<"PROGRAM_NAME"$Dir>.PlugIns" },
{ config_BOOL, "plugin.uptodate", 	(void *)offsetof(struct config_str, plugin_uptodate),	"Keep plugin file up to date", (void *)0 },
{ config_BOOL, "netscape.fake", 	(void *)offsetof(struct config_str, netscape_fake),	"Alter UserAgent header", (void *)0 },

{ config_LAST, NULL, NULL, NULL, 0 }
};


/**********************************************************************/
/* This gets called to set up all the config */
extern void config_init(void)
{
    config_default_first();
    config_read_file();
#ifdef STBWEB
    config_read_variables();
#endif

#if ! defined(STBWEB) && ! defined(BUILDERS)
    /* If this assert fails then SOMEBODY has been adding items to the above
     * array without adding corresponding ones in the array in
     * !Fresco/configgui.c ...
     */

    {
        extern const int configgui_array_size;    /* in configgui.c */

        ASSERT( (sizeof(citems)/sizeof(config_item))
                 == configgui_array_size );
    }
#endif

    /* SJM */
    os_cli("Unset "PROGRAM_NAME"$Temp");
    os_cli("RMEnsure FontManager 3.36 Set "PROGRAM_NAME"$Temp 1");
    if (getenv(PROGRAM_NAME"$Temp") != NULL)
        config_display_blending = FALSE;
}

extern void config_tidyup(void)
{
    int i;
    for (i = 0; citems[i].type != config_LAST; i++)
    {
	switch(citems[i].type)
	{
	case config_STRING:
	case config_FILE:
	case config_URL:
	case config_FONT:
	    if (*((char**) citems[i].ptr))
		mm_free(*((char**) citems[i].ptr));
	    break;
	}
    }
}

extern int config_colour_number(char *p)
{
    char *q;
    int v;

    if (*p == '&' || *p == '#')
    {
	v = (int) strtol(p+1, &q, 16);
    }
    else if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
    {
	v = (int) strtol(p+2, &q, 16);
    }
    else
    {
	v = (int) strtol(p, &q, 10);
	if (q && *q == '%')
	    v = ((v * 255) + 50) / 100;	/* Get the rounding right; this takes a percentage to the nearest, not the nearest lower */
    }

    if (v < 0)
	v = 0;
    if (v > 255)
	v = 255;

    CNFDBGN(("String '%s' turned into colour value 0x%02x\n", p, v));

    return v;
}

static void config_read_int(const char *p, config_item *citem)
{
    if (*p == '&' || *p == '#')
    {
	*((int*) citem->ptr) = (int) strtol(p+1, NULL, 16);
    }
    else if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
    {
	*((int*) citem->ptr) = (int) strtol(p+2, NULL, 16);
    }
    else if (isdigit(*p))
    {
	*((int*) citem->ptr) = (int) strtol(p, NULL, 10);
    }
    else
	config_read_bool(p, citem);

    CNFDBGN(("Value string '%s' taken as interger value %d\n", p, *((int*) citem->ptr)));

}

static void config_read_bool(const char *p, config_item *citem)
{
    if (strcasecomp(p, "YES") == 0 ||
	strcasecomp(p, "Y") == 0 ||
	strcasecomp(p, "ON") == 0 ||
	strcasecomp(p, "TRUE") == 0 ||
	atoi(p) != 0)
        *((int*) citem->ptr) = 1;
    else
	*((int*) citem->ptr) = 0;

    CNFDBGN(("Value string '%s' is boolean %d\n", p, *((int*) citem->ptr) ));
}

static void config_read_colour(char *p, char *q, char *s, config_item *citem)
{
    CNFDBGN(("colour strings '%s '%s '%s'\n", p, strsafe(q), strsafe(s)));
    if (q)
    {
        int r,g,b;

	r = config_colour_number(p);
	g = config_colour_number(q);
	if (s)
	    b = config_colour_number(s);
	else
	    b = 0;

	*((int*) citem->ptr) = (b << 24) + (g << 16) + (r << 8);
    }
    else
    {
	if (p[0] == '#' || p[0] == '&')
	    p += 1;
	else if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
	    p += 2;

	*((unsigned long int*) citem->ptr) = strtoul(p, NULL, 16);

	CNFDBGN(("Value string '%s' is for a colour\n", p));

    }

    CNFDBGN(("Colour is 0x%08x\n", *((int*) citem->ptr)));
}

static void config_read_string(char *p, config_item *citem)
{
    char *q;
    while(*p && isspace(*p))
	p++;
    if ((*p == '"' && (q = strrchr(p, '"')) != p) ||
	(*p == '\'' && (q = strrchr(p, '\'')) != p) )
    {
	p++;
	*q = 0;
	*((char**) citem->ptr) = strdup(p);
    }
    else
    {
	q = p + strlen(p);
	while (q != p && isspace(q[-1]))
	    q--;
	*q = 0;
	if (*p)
	    *((char**) citem->ptr) = strdup(p);
    }

    CNFDBGN(("Value string '%s' taken as string value '%s'\n", p,
	    *((char**) citem->ptr) ? *((char**) citem->ptr) : "<none>" ));
}

#ifdef STBWEB
static char *config_check_var(const char *config_name)
{
    char name[64], *s_out;
    const char *s_in;
    int c;

    /* COMMENT lines have no names */
    if (config_name == NULL)
	return NULL;

    strcpy(name, PROGRAM_NAME"$Config");
    s_out = name + sizeof(PROGRAM_NAME"$Config")-1;

    s_in = config_name;

    while ((c = *s_in++) != 0)
        if (c != '.' ) *s_out++ = c;
    *s_out = 0;

    return getenv(name);
}
#endif

#ifdef STBWEB
/*
 * Scan for system variables to override the config file values
 */

static void config_read_variables(void)
{
    int i;
    for (i = 0; citems[i].type != config_LAST; i++)
    {
        char *val = config_check_var(citems[i].name);

	CNFDBG(("config: var %s returns '%s'\n", strsafe(citems[i].name), strsafe(val)));

        if (val) switch(citems[i].type)
        {
	    case config_INT:
                config_read_int(val, &citems[i]);
		break;

	    case config_BOOL:
		config_read_bool(val, &citems[i]);
		break;

	    case config_COLOUR:
	    {
		char *p = strtok(val, " \t\n\r,");
		if (p)
		{
		    char *q1 = strtok(NULL, " \t\n\r,");
		    char *q2 = strtok(NULL, " \t\n\r,");
		    config_read_colour(p, q1, q2, &citems[i]);
		}
		break;
	    }

	    case config_STRING:
	    case config_FILE:
	    case config_URL:
	    case config_FONT:
		if (*((char**) citems[i].ptr))
		    mm_free(*((char**) citems[i].ptr));
		*((char**) citems[i].ptr) = 0;

		if (val[0])
		    config_read_string(val, &citems[i]);
		break;
        }
    }
}
#endif

void config_read_file_by_name(const char *file_name)
{
    char buffer[256];
    char *p;
    int i;
    FILE *fh;

    fh = fopen(file_name, "r");

    if (fh == NULL)
    {
	usrtrc( "Config file unreadable\n");
	return;
    }

    while(!feof(fh))
    {
	if (fgets(buffer, sizeof(buffer), fh))
	{
	    CNFDBG(("Read in line:%s", buffer));

	    p = strtok(buffer, " \t:");
	    if (*p == '#')
	    {
		CNFDBGN(("Line is a comment\n"));

		continue;
	    }

	    CNFDBG(("Tag is '%s'\n", p));

	    for(i=0; citems[i].type != config_LAST; i++)
	    {
		if (citems[i].name && (strcasecomp(citems[i].name, p) == 0) )
		    break;
	    }

	    if (citems[i].type == config_LAST)
	    {
		usrtrc( "Config tag '%s' unknown.  Line ignored.\n", p);
		continue;
	    }

	    switch(citems[i].type)
	    {
	    case config_INT:
		p = strtok(NULL, " \t\n\r");
		if (p)
		    config_read_int(p, &citems[i]);
		break;
	    case config_BOOL:
		p = strtok(NULL, " \t\n\r");
		if (p)
		    config_read_bool(p, &citems[i]);
		break;
	    case config_COLOUR:
		p = strtok(NULL, " \t\n\r,");
		if (p)
		{
		    char *q1 = strtok(NULL, " \t\n\r,");
		    char *q2 = strtok(NULL, " \t\n\r,");
		    config_read_colour(p, q1, q2, &citems[i]);
		}
		break;

	    case config_STRING:
	    case config_FILE:
	    case config_URL:
	    case config_FONT:
		if (*((char**) citems[i].ptr))
		    mm_free(*((char**) citems[i].ptr));
		*((char**) citems[i].ptr) = 0;

		p = strtok(NULL, "\n\r");
		if (p)
		    config_read_string(p, &citems[i]);
		break;
	    }
	}
	else
	{
	    break;
	}
    }

    fclose(fh);

    config_has_been_changed = 0;
}

void config_read_file(void)
{
    config_read_file_by_name(CONFIG_FILE_NAME_2);
}

/* fixup the array to be absolute addresses before writing out default */
/* this is necessary to allow a module build */

static void config_default_first(void)
{
    int i;

    for(i=0; citems[i].type != config_LAST; i++)
    {

	switch(citems[i].type)
	{
	case config_INT:
	case config_BOOL:
	case config_COLOUR:
            citems[i].ptr = (char *)&config_array + (int)(long)citems[i].ptr;
	    *((int*) (long)citems[i].ptr) = (int)(long)(citems[i].def);
	    break;

	case config_FONT:
	case config_STRING:
	case config_FILE:
	case config_URL:
            citems[i].ptr = (char *)&config_array + (int) (long) citems[i].ptr;
	    if ((char*) citems[i].def == 0)
		*((char**) citems[i].ptr) = 0;
	    else
		*((char**) citems[i].ptr) = strdup((char*) citems[i].def);
	    break;
	}
    }
}

extern void config_write_file_by_name(const char *file_name)
{
    FILE *fh;
    int i;
    char *p;
    int x;

    fh = fopen(file_name, "w");

    if (fh == NULL)
    {
	usrtrc( "Config file unwritable\n");
	return;
    }

    for(i=0; citems[i].type != config_LAST; i++)
    {
	if (citems[i].comment)
	    fprintf(fh, "# %s\n", citems[i].comment);

	switch(citems[i].type)
	{
	case config_INT:
	    fprintf(fh, "%s: %d\n", citems[i].name, *((int*) citems[i].ptr));
	    break;
	case config_BOOL:
	    fprintf(fh, "%s: %s\n", citems[i].name, *((int*) citems[i].ptr) ? "Yes" : "No");
	    break;
	case config_COLOUR:
	    x = *((int*) citems[i].ptr);
	    fprintf(fh, "%s: 0x%02x, 0x%02x, 0x%02x\n", citems[i].name, (x >> 8) & 0xff, (x >> 16) & 0xff, (x >> 24) & 0xff);
	    break;
	case config_FONT:
	case config_STRING:
	case config_FILE:
	case config_URL:
	    p = *((char**) citems[i].ptr);
	    fprintf(fh, "%s: %s\n", citems[i].name,  p ? (*p == 0 ? "\"\"" : p) : "" );
	    break;
	}
    }

    fclose(fh);

    config_has_been_changed = 0;
}

extern void config_write_file(void)
{
    config_write_file_by_name(CONFIG_FILE_NAME_2);
}

/* eof config.c */
