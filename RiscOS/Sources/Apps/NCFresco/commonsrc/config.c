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

static void config_default_first(void);
static int config_read_bool(const char *p);
static void config__read_file(void);

#ifdef STBWEB
static void config_read_variables(void);
#endif

int config_has_been_changed;

static config_filter_fn filter_fn = 0;

/**********************************************************************/
/* This is the global config variables */

struct config_str config_array;

/**********************************************************************

 !!    !!    !!    !!    !!    !!    !!    !!    !!    !!    !!    !!

 NOTE:  This table MUST be kept in step with citemsdbs in configgui.c

 !!    !!    !!    !!    !!    !!    !!    !!    !!    !!    !!    !!

**********************************************************************/

config_item citems[] = {
{ config_COMMENT, NULL, NULL, "Config file for "PROGRAM_NAME"\256", NULL },

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_INT,
      "image.max",
      (void *)offsetof(struct config_str, max_files_fetching),
      "Maximum number of image files in transit at one time",
      (void *) 6  },
{ config_BOOL,
      "image.deep",
      (void *)offsetof(struct config_str, deep_images),
      "Do we want deep colour for images?",
#ifdef STBWEB
      (void *) 1
#else
      (void *) 0
#endif
      },
{ config_BOOL,
      "image.defer",
      (void *)offsetof(struct config_str, defer_images),
      "Don't load images until the user asks",
      (void *) 0  },

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_BOOL,
      "display.links.underlined",
      (void *)offsetof(struct config_str, display_links_underlined),
      "Underline links?",
      (void *) 0  },
{ config_BOOL,
      "display.body_colours",
      (void *)offsetof(struct config_str, display_body_colours),
      "Use colour and background directives in BODY tags",
      (void *) 1  },
{ config_BOOL,
      "display.antialias",
      (void *)offsetof(struct config_str, display_antialias),
      "Antialias the displayed text",
      (void *) 1  },
{ config_BOOL,
      "display.control.buttons",
      (void *)offsetof(struct config_str, display_control_buttons),
      "Display button bar",
      (void *) 1  },
{ config_BOOL,
      "display.control.urlline",
      (void *)offsetof(struct config_str, display_control_urlline),
      "Display URL line",
      (void *) 1  },
{ config_BOOL,
      "display.control.status",
      (void *)offsetof(struct config_str, display_control_status),
      "Display status line",
      (void *) 1  },
#ifdef STBWEB
{ config_BOOL,
      "display.control.top",
      (void *)offsetof(struct config_str, display_control_top),
      "Display tool bar at top",
      (void *) 1  },
{ config_INT,
      "display.control.initial",
      (void *)offsetof(struct config_str, display_control_initial),
      "Initial tool bar",
      (void *) 0  },
#endif
{ config_BOOL,
      "display.fancy_ptr",
      (void *)offsetof(struct config_str, display_fancy_ptr),
      "Use different pointer shapes depending on what the pointer is over",
      (void *) 1 },
{ config_INT,
      "display.scale",
      (void *)offsetof(struct config_str, display_scale),
      "Scaling factor (%)",
      (void *) 100  },
{ config_INT,
      "display.scale.image",
      (void *)offsetof(struct config_str, display_scale_image),
      "Scaling factor for images (%)",
      (void *) 100 },
{ config_INT,
      "display.scale.1",
      (void *)offsetof(struct config_str, display_scales[0]),
      "Scale level 1",
      (void *) 100 },
{ config_INT,
      "display.scale.2",
      (void *)offsetof(struct config_str, display_scales[1]),
      "Scale level 2",
      (void *) 125 },
{ config_INT,
      "display.scale.3",
      (void *)offsetof(struct config_str, display_scales[2]),
      "Scale level 3",
      (void *) 150 },
{ config_BOOL,
      "display.scale.fit",
      (void *)offsetof(struct config_str, display_scale_fit),
      "Force page to fit",
      (void *) 0  },
{ config_INT,
      "display.scale.fit.mode",
      (void *)offsetof(struct config_str, display_scale_fit_mode),
      "Force page to fit by scaling, 0 = images, 1 = images+text",
      (void *) 0  },
{ config_INT,
      "display.width",
      (void *)offsetof(struct config_str, display_width),
      "Default page width in characters",
      (void *) 71  },
{ config_BOOL,
      "display.blending",
      (void *)offsetof(struct config_str, display_blending),
      "Blend fonts to background",
#ifdef STBWEB
      (void *) 1
#else
      (void *) 0
#endif
      },
#ifdef STBWEB
{ config_INT,
      "display.margin.left",
      (void *)offsetof(struct config_str, display_margin.x0),
      "Margin left (%)",
      (void *) 0 },
{ config_INT,
      "display.margin.right",
      (void *)offsetof(struct config_str, display_margin.x1),
      "Margin right (%)",
      (void *) 0 },
{ config_INT,
      "display.margin.top",
      (void *)offsetof(struct config_str, display_margin.y1),
      "Margin top (%)",
      (void *) 0 },
{ config_INT,
      "display.margin.bottom",
      (void *)offsetof(struct config_str, display_margin.y0),
      "Margin bottom (%)",
      (void *) 0 },
{ config_BOOL,
      "display.margin.auto",
      (void *)offsetof(struct config_str, display_margin_auto),
      "Use ROM values",
      (void *) 1 },
#endif
{ config_BOOL,
      "display.frames",
      (void *)offsetof(struct config_str, display_frames),
      "Display frames",
      (void *)1  },
#ifdef STBWEB
{ config_BOOL,
      "display.frames.scrollbars",
      (void *)offsetof(struct config_str, display_frames_scrollbars),
      "Display scrollbars with frames",
      (void *)1  },
{ config_BOOL,
      "display.frames.top_level",
      (void *)offsetof(struct config_str, display_frames_top_level),
      "Display top level as frame",
      (void *)0  },
{ config_BOOL,
      "display.smooth_scrolling",
      (void *)offsetof(struct config_str, display_smooth_scrolling),
      "Use smooth scrolling",
      (void *)0  },
#endif
{ config_INT,
      "display.time.activate",
      (void *)offsetof(struct config_str, display_time_activate),
      "Time to activate links for",
      (void *)50 },
{ config_INT,
      "display.time.background",
      (void *)offsetof(struct config_str, display_time_background),
      "Time for which to wait for background",
      (void *)0 },
#ifdef STBWEB
{ config_INT,
      "display.time.fade",
      (void *)offsetof(struct config_str, display_time_fade),
      "Time to fade page out",
      (void *)50 },
#endif
{ config_BOOL,
      "display.map_coords",
      (void *)offsetof(struct config_str, display_map_coords),
      "Display map co-ordinates on the status line",
      (void *) 0 },
{ config_INT,
      "display.jpeg",
      (void *)offsetof(struct config_str, display_jpeg),
      "0,1,3: on the fly, 2: decompress",
      (void *) 2 },
{ config_INT,
      "display.leading",
      (void *)offsetof(struct config_str, display_leading),
      "half leading",
#ifdef STBWEB
      (void *) 4
#else
      (void *) 25
#endif
      },
{ config_BOOL,
      "display.leading.percent",
      (void *)offsetof(struct config_str, display_leading_percent),
      "% or OS units",
#ifdef STBWEB
      (void *) 0
#else
      (void *) 1
#endif
      },
{ config_INT,
      "display.char.password",
      (void *)offsetof(struct config_str, display_char_password),
      "password char",
#ifdef STBWEB
      (void *) '*'
#else
      (void *) '-'
#endif
      },
#ifdef STBWEB
{ config_INT,
      "display.highlight.style",
      (void *)offsetof(struct config_str, display_highlight_style),
      "Style for drawing highlight",
      (void *) 0  },
{ config_INT,
      "display.highlight.width",
      (void *)offsetof(struct config_str, display_highlight_width),
      "Width of highlight",
      (void *) 1  },
#endif
{ config_BOOL,
      "display.tables",
      (void *)offsetof(struct config_str, display_tables),
      "Display tables",
      (void *)1  },

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_URL,
      "document.default",
      (void *)offsetof(struct config_str, doc_default),
      "Default document",
#ifdef STBWEB
      "<"PROGRAM_NAME"$Home>"
#else
      "Welcome"
#endif
      },
#ifdef STBWEB
{ config_URL,
      "document.handler.related",
      (void *)offsetof(struct config_str, document_handler_related),
      "URL to fetch related stuff",
      "<"PROGRAM_NAME"$Related>" },
{ config_URL,
      "document.search",
      (void *)offsetof(struct config_str, document_search),
      "URL to search from",
      "<"PROGRAM_NAME"$Search>" },
{ config_URL,
      "document.offline",
      (void *)offsetof(struct config_str, document_offline),
      "URL of offline menu",
      "<"PROGRAM_NAME"$Offline>" },
#endif
{ config_INT,
      "encoding.user",
      (void *)offsetof(struct config_str, encoding_user),
      "default encoding MIB number",
#ifdef STBWEB
      (void *)2252
#else
      (void *)0
#endif
      },
{ config_BOOL,
      "encoding.user.override",
      (void *)offsetof(struct config_str, encoding_user_override),
      "Use config rather than document encoding?",
      (void *)0 },
{ config_INT,
      "encoding.internal",
      (void *)offsetof(struct config_str, encoding_internal),
      "0 = Latin1, 1 = UTF8",
      (void *)0 },
{ config_STRING,
      "encoding.accept",
      (void *)offsetof(struct config_str, encoding_accept),
      "Accept-Charset header text",
      (void *)0 },
{ config_COMMENT, NULL, NULL, "", NULL },

{ config_FILE,
      "files.hots",
      (void *)offsetof(struct config_str, hotlist_file),
      "Hotlist file",
#ifdef STBWEB
      "<"PROGRAM_NAME"$Hotlist>"
#else
      "InetDBase:Hotlist"
#endif
      },
{ config_FILE,
      "files.passwords",
      (void *)offsetof(struct config_str, auth_file),
      "File to store passwords in",
#ifdef STBWEB
      "<"PROGRAM_NAME"$Users>"
#else
      "<Fresco$Dir>.Users"
#endif
      },
{ config_BOOL,
      "files.passwords.encrypted",
      (void *)offsetof(struct config_str, auth_file_crypt),
      "Store passwords scrambled",
      (void *) 0  },
{ config_FILE,
      "files.runable",
      (void *)offsetof(struct config_str, runable_file),
      "This file lists the file types of files that will be run when fetched.",
#ifdef STBWEB
      "<"PROGRAM_NAME"$Runable>"
#else
      "<"PROGRAM_NAME"$Dir>.Runables"
#endif
      },
{ config_FILE,
      "files.allow",
      (void *)offsetof(struct config_str, allow_file),
      "If given, only allow access to sites listed in this file.",
#ifdef STBWEB
      "<"PROGRAM_NAME"$Allow>"
#else
      NULL
#endif
      },
{ config_FILE,
      "files.deny",
      (void *)offsetof(struct config_str, deny_file),
      "If given, don't allow access to sites listed in this file.",
#ifdef STBWEB
      "<"PROGRAM_NAME"$Deny>"
#else
      NULL
#endif
      },
{ config_FILE,
      "files.Help",
      (void *)offsetof(struct config_str, help_file),
      "URL of help pages",
#ifdef STBWEB
      "<"PROGRAM_NAME"$Help>"
#else
      NULL
#endif
      },
{ config_FILE,
      "files.cookie",
      (void *)offsetof(struct config_str, cookie_file),
      "Where to store cookies",
#ifdef STBWEB
      "<"PROGRAM_NAME"$Cookies>"
#else
      "<"PROGRAM_NAME"$Dir>.Cookies"
#endif
      },
{ config_FILE,
      "files.history",
      (void *)offsetof(struct config_str, history_file),
      "Where to store the persistent history",
      "<"PROGRAM_NAME"$Dir>.History" },

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_INT,
      "truncate.length",
      (void *)offsetof(struct config_str, truncate_length),
      "Number of chars to truncate file names to",
      0  },
{ config_BOOL,
      "truncate.suffix",
      (void *)offsetof(struct config_str, truncate_suffix),
      "Do we remove the file extension when we choose a file name?",
      0  },

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_STRING,
      "user.email",
      (void *)offsetof(struct config_str, email_addr),
      "User's email address",
      0  },

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_STRING,
      "animation.name",
      (void *)offsetof(struct config_str, animation_name),
      "Prefix string for animation sprite names",
      "prog"  },
{ config_INT,
      "animation.frames",
      (void *)offsetof(struct config_str, animation_frames),
      "Number of frames in the progress amination",
      (void *) 12  },

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_INT,
      "font.size.1",
      (void *)offsetof(struct config_str, font_sizes[0]),
      "Point sizes for fonts",
      (void *) 8  },
{ config_INT,
      "font.size.2",
      (void *)offsetof(struct config_str, font_sizes[1]),
      0,
      (void *) 10  },
{ config_INT,
      "font.size.3",
      (void *)offsetof(struct config_str, font_sizes[2]),
      0,
      (void *) 12  },
{ config_INT,
      "font.size.4",
      (void *)offsetof(struct config_str, font_sizes[3]),
      0,
      (void *) 14  },
{ config_INT,
      "font.size.5",
      (void *)offsetof(struct config_str, font_sizes[4]),
      0,
      (void *) 16  },
{ config_INT,
      "font.size.6",
      (void *)offsetof(struct config_str, font_sizes[5]),
      0,
      (void *) 18  },
{ config_INT,
      "font.size.7",
      (void *)offsetof(struct config_str, font_sizes[6]),
      0,
      (void *) 24  },

{ config_COMMENT, NULL, NULL,  "", NULL },

{ config_FONT, "font.base",
      (void *)offsetof(struct config_str, font_names[0]),
      "Name of standard (body) font",
      "Trinity.Medium"  },
{ config_FONT,
      "font.base.b",
      (void *)offsetof(struct config_str, font_names[1]),
      "... the bold form",
      "Trinity.Bold"  },
{ config_FONT,
      "font.base.i",
      (void *)offsetof(struct config_str, font_names[2]),
      "... the italic form",
      "Trinity.Medium.Italic"  },
{ config_FONT,
      "font.base.bi",
      (void *)offsetof(struct config_str, font_names[3]),
      "... bold and italic",
      "Trinity.Bold.Italic"  },
{ config_FONT,
      "font.head",
      (void *)offsetof(struct config_str, font_names[8]),
      "Name of headings font",
      "Homerton.Medium"  },
{ config_FONT,
      "font.head.b",
      (void *)offsetof(struct config_str, font_names[9]),
      "... the bold form",
      "Homerton.Bold"  },
{ config_FONT,
      "font.head.i",
      (void *)offsetof(struct config_str, font_names[10]),
      "... the italic form",
      "Homerton.Medium.Oblique"  },
{ config_FONT,
      "font.head.bi",
      (void *)offsetof(struct config_str, font_names[11]),
      "... bold and italic",
      "Homerton.Bold.Oblique"  },
{ config_FONT,
      "font.fixed",
      (void *)offsetof(struct config_str, font_names[4]),
      "Name of fixed space font",
      "Corpus.Medium"  },
{ config_FONT,
      "font.fixed.b",
      (void *)offsetof(struct config_str, font_names[5]),
      "... the bold form",
      "Corpus.Bold"  },
{ config_FONT,
      "font.fixed.i",
      (void *)offsetof(struct config_str, font_names[6]),
      "... the italic form",
      "Corpus.Medium.Oblique"  },
{ config_FONT,
      "font.fixed.bi",
      (void *)offsetof(struct config_str, font_names[7]),
      "... bold and italic",
      "Corpus.Bold.Oblique"  },
{ config_FONT,
      "font.symbol",
      (void *)offsetof(struct config_str, font_names[12]),
      "Name of symbol font",
      "Selwyn"  },
{ config_FONT,
      "font.internal",
      (void *)offsetof(struct config_str, font_names[13]),
      "Name of internal menu font",
      "Homerton.Medium"  },
{ config_FONT,
      "font.ja",
      (void *)offsetof(struct config_str, font_names[14]),
      "Name of japanese font",
      "HeiBold"  },
{ config_FONT,
      "font.zh",
      (void *)offsetof(struct config_str, font_names[15]),
      "Name of chinese font",
      "HeiBold"  },
{ config_FONT,
      "font.ko",
      (void *)offsetof(struct config_str, font_names[16]),
      "Name of korean font",
      "HeiBold"  },
{ config_FONT,
      "font.ru",
      (void *)offsetof(struct config_str, font_names[17]),
      "Name of russian font",
      "Homerton.Medium"  },
{ config_FONT,
      "font.el",
      (void *)offsetof(struct config_str, font_names[18]),
      "Name of greek font",
      "Homerton.Medium"  },
{ config_FONT,
      "font.he",
      (void *)offsetof(struct config_str, font_names[19]),
      "Name of hebrew font",
      "Homerton.Medium"  },

{ config_INT,
      "font.base.scale",
      (void *)offsetof(struct config_str, font_scales[0]),
      "Scale factor of base font (%)",
      (void *)100  },
{ config_INT,
      "font.base.aspect",
      (void *)offsetof(struct config_str, font_aspects[0]),
      "Aspect ratio of base font (%)",
      (void *)100  },

{ config_INT,
      "font.head.scale",
      (void *)offsetof(struct config_str, font_scales[2]),
      "Scale factor of heading font (%)",
      (void *)100  },
{ config_INT,
      "font.head.aspect",
      (void *)offsetof(struct config_str, font_aspects[2]),
      "Aspect ratio of heading font (%)",
      (void *)100  },

{ config_INT,
      "font.fixed.scale",
      (void *)offsetof(struct config_str, font_scales[1]),
      "Scale factor of fixed font (%)",
      (void *)100  },
{ config_INT,
      "font.fixed.aspect",
      (void *)offsetof(struct config_str, font_aspects[1]),
      "Aspect ratio of fixed font (%)",
      (void *)100  },

{ config_INT,
      "font.ja.scale",
      (void *)offsetof(struct config_str, font_scales[3]),
      "Scale factor of ja font (%)",
      (void *)100  },
{ config_INT,
      "font.ja.aspect",
      (void *)offsetof(struct config_str, font_aspects[3]),
      "Aspect ratio of ja font (%)",
      (void *)100  },

{ config_INT,
      "font.zh.scale",
      (void *)offsetof(struct config_str, font_scales[4]),
      "Scale factor of zh font (%)",
      (void *)100  },
{ config_INT,
      "font.zh.aspect",
      (void *)offsetof(struct config_str, font_aspects[4]),
      "Aspect ratio of zh font (%)",
      (void *)100  },

{ config_INT,
      "font.ko.scale",
      (void *)offsetof(struct config_str, font_scales[5]),
      "Scale factor of ko font (%)",
      (void *)100  },
{ config_INT,
      "font.ko.aspect",
      (void *)offsetof(struct config_str, font_aspects[5]),
      "Aspect ratio of ko font (%)",
      (void *)100  },

{ config_INT,
      "font.ru.scale",
      (void *)offsetof(struct config_str, font_scales[6]),
      "Scale factor of ru font (%)",
      (void *)100  },
{ config_INT,
      "font.ru.aspect",
      (void *)offsetof(struct config_str, font_aspects[6]),
      "Aspect ratio of ru font (%)",
      (void *)100  },

{ config_INT,
      "font.el.scale",
      (void *)offsetof(struct config_str, font_scales[7]),
      "Scale factor of el font (%)",
      (void *)100  },
{ config_INT,
      "font.el.aspect",
      (void *)offsetof(struct config_str, font_aspects[7]),
      "Aspect ratio of el font (%)",
      (void *)100  },

{ config_INT,
      "font.he.scale",
      (void *)offsetof(struct config_str, font_scales[8]),
      "Scale factor of he font (%)",
      (void *)100  },
{ config_INT,
      "font.he.aspect",
      (void *)offsetof(struct config_str, font_aspects[8]),
      "Aspect ratio of he font (%)",
      (void *)100  },

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_COLOUR,
      "colour.plain",
      (void *)offsetof(struct config_str, colours[render_colour_PLAIN]),
      "Colour for normal text",
      (void *) 0x00000000  },
{ config_COLOUR,
      "colour.anchor",
      (void *)offsetof(struct config_str, colours[render_colour_AREF]),
      "Colour for text in hypertext links",
      (void *) 0xdd220000  },
{ config_COLOUR,
      "colour.visited",
      (void *)offsetof(struct config_str, colours[render_colour_CREF]),
      "Colour for text in links that have been visited",
      (void *) 0x88220000  },
{ config_COLOUR,
      "colour.back",
      (void *)offsetof(struct config_str, colours[render_colour_BACK]),
      "Background colour",
      (void *) 0xdddddd00  },
{ config_COLOUR,
      "colour.action",
      (void *)offsetof(struct config_str, colours[render_colour_ACTION]),
      "Colour for activating buttons",
      (void *) 0x00bbff00  },
{ config_COLOUR,
      "colour.write",
      (void *)offsetof(struct config_str, colours[render_colour_WRITE]),
      "Colour for writable areas",
      (void *) 0xffffff00  },
{ config_COLOUR,
      "colour.line.light",
      (void *)offsetof(struct config_str, colours[render_colour_LINE_L]),
      "Colour for the light side of rules",
      (void *) 0xffffff00  },
{ config_COLOUR,
      "colour.line.dark",
      (void *)offsetof(struct config_str, colours[render_colour_LINE_D]),
      "Colour for the dark side of rules",
      (void *) 0x55555500  },
{ config_COLOUR,
      "colour.button.text",
      (void *)offsetof(struct config_str, colours[render_colour_INPUT_F]),
      "Colour for text on buttons",
      (void *) 0x00000000  },
{ config_COLOUR,
      "colour.button.back",
      (void *)offsetof(struct config_str, colours[render_colour_INPUT_B]),
      "Background colour for buttons",
      (void *) 0xdddddd00  },
{ config_COLOUR,
      "colour.button.select",
      (void *)offsetof(struct config_str, colours[render_colour_INPUT_S]),
      "Colour for selected button",
      (void *) 0xdddddd00  },
{ config_COLOUR,
      "colour.highlight",
      (void *)offsetof(struct config_str, colours[render_colour_HIGHLIGHT]),
      "Colour for highlighted link",
      (void *) 0xff000000  },
{ config_COLOUR,
      "colour.activated",
      (void *)offsetof(struct config_str, colours[render_colour_ACTIVATED]),
      "Colour for activating link",
      (void *) 0x0000ff00  },

{ config_COLOUR,
      "colour.bevel",
      (void *)offsetof(struct config_str, colours[render_colour_BEVEL]),
      "Colour for frame bevel",
      (void *) 0x66666600 },

{ config_COLOUR,
      "colour.button.text.highlight",
      (void *)offsetof(struct config_str, colours[render_colour_INPUT_FS]),
      "Colour for highlighted text on buttons",
      (void *) 0x00000000  },

#ifdef STBWEB
{ config_COLOUR,
      "colour.menu.text",
      (void *)offsetof(struct config_str, colours[render_colour_MENU_F]),
      "Colour for menu foreground",
      (void *) 0 },
{ config_COLOUR,
      "colour.menu.back",
      (void *)offsetof(struct config_str, colours[render_colour_MENU_B]),
      "Colour for menu background",
      (void *) 0xffffff00  },
{ config_COLOUR,
      "colour.menu.text.highlight",
      (void *)offsetof(struct config_str, colours[render_colour_MENU_FS]),
      "Colour for menu foreground highlighted",
      (void *) 0xffffff00  },
{ config_COLOUR,
      "colour.menu.back.highlight",
      (void *)offsetof(struct config_str, colours[render_colour_MENU_BS]),
      "Colour for menu background highlighted",
      (void *) 0  },
#endif

{ config_COLOUR_LIST,
      "colour.border.bevel",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_BEVEL]),
      "Colours for frame bevel",
      (void *) "0x87878700, 0x87878700, 0x75757500, 0x75757500, 0x21212100, 0x21212100, 0x42424200, 0x42424200" },

#ifdef STBWEB
{ config_COLOUR_LIST,
      "colour.border.write",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_WRITE]),
      "Colours for write border",
      (void *) NULL  },
{ config_COLOUR_LIST,
      "colour.border.write.highlight",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_WRITE_HIGHLIGHT]),
      "Colours for write border highlighted",
      (void *) NULL  },

{ config_COLOUR_LIST,
      "colour.border.select",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_SELECT]),
      "Colours for select border",
      (void *) NULL  },
{ config_COLOUR_LIST,
      "colour.border.select.highlight",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_SELECT_HIGHLIGHT]),
      "Colours for select border highlighted",
      (void *) NULL  },

{ config_COLOUR_LIST,
      "colour.border.button",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_BUTTON]),
      "Colours for button border",
      (void *) NULL  },
{ config_COLOUR_LIST,
      "colour.border.button.highlight",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_BUTTON_HIGHLIGHT]),
      "Colours for button border highlighted",
      (void *) NULL  },

{ config_COLOUR_LIST,
      "colour.border.highlight",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_HIGHLIGHT]),
      "Colours for highlighted link",
      (void *) NULL  },
{ config_COLOUR_LIST,
      "colour.border.highlight.text",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_TEXT_HIGHLIGHT]),
      "Colours for highlighted text link",
      (void *) NULL  },
{ config_COLOUR_LIST,
      "colour.border.activated",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_ACTIVATED]),
      "Colours for activated link",
      (void *) NULL  },

{ config_COLOUR_LIST,
      "colour.border.window",
      (void *)offsetof(struct config_str, colour_list[render_colour_list_WINDOW_BORDER]),
      "Colours for window border",
      (void *) NULL  },
#endif

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_BOOL,
      "proxy.http.enable",
      (void *)offsetof(struct config_str, proxy_http_on),
      "Enable proxies",
      (void *) 0  },
{ config_BOOL,
      "proxy.https.enable",
      (void *)offsetof(struct config_str, proxy_https_on),
      NULL,
      (void *) 0  },
{ config_BOOL,
      "proxy.gopher.enable",
      (void *)offsetof(struct config_str, proxy_gopher_on),
      NULL,
      (void *) 0  },
{ config_BOOL,
      "proxy.ftp.enable",
      (void *)offsetof(struct config_str, proxy_ftp_on),
      NULL,
      (void *) 0  },
{ config_BOOL,
      "proxy.mailto.enable",
      (void *)offsetof(struct config_str, proxy_mailto_on),
      NULL,
      (void *) 0  },
{ config_STRING,
      "proxy.http",
      (void *)offsetof(struct config_str, proxy_http),
      "Proxy server name (and port)",
      0  },
{ config_STRING,
      "proxy.https",
      (void *)offsetof(struct config_str, proxy_https),
      NULL,
      0  },
{ config_STRING,
      "proxy.gopher",
      (void *)offsetof(struct config_str, proxy_gopher),
      NULL,
      0  },
{ config_STRING,
      "proxy.ftp",
      (void *)offsetof(struct config_str, proxy_ftp),
      NULL,
      0  },
{ config_URL,
      "proxy.mailto",
      (void *)offsetof(struct config_str, proxy_mailto),
      "Proxy URL",
      0  },
{ config_STRING,
      "proxy.http.ignore",
      (void *)offsetof(struct config_str, proxy_http_ignore),
      "No proxy on these domains",
      0  },
{ config_STRING,
      "proxy.https.ignore",
      (void *)offsetof(struct config_str, proxy_https_ignore),
      NULL,
      0  },
{ config_STRING,
      "proxy.gopher.ignore",
      (void *)offsetof(struct config_str, proxy_gopher_ignore),
      NULL,
      0  },
{ config_STRING,
      "proxy.ftp.ignore",
      (void *)offsetof(struct config_str, proxy_ftp_ignore),
      NULL,
      0  },

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_INT,
      "cache.size",
      (void *)offsetof(struct config_str, cache_size),
      "Size in K for cache (0 = unlimited)",
      (void *) 0  },
{ config_INT,
      "cache.items",
      (void *)offsetof(struct config_str, cache_items),
      "Max number of items in the cache",
#ifdef STBWEB
      (void *) 5625
#else
      (void *) 48
#endif
      },
{ config_BOOL,
      "cache.keep",
      (void *)offsetof(struct config_str, cache_keep),
      "Should the cache be kept across closedowns?",
#ifdef STBWEB
      (void *) 1
#else
      (void *) 0
#endif
      },
{ config_BOOL,
      "cache.keep.uptodate",
      (void *)offsetof(struct config_str, cache_keep_uptodate),
      "Save the cachedump on every update",
#ifdef STBWEB
      (void *) 1
#else
      (void *) 0
#endif
      },

{ config_COMMENT, NULL, NULL, "", NULL },

{ config_INT,
      "print.scale",
      (void *)offsetof(struct config_str, print_scale),
      "Printing magnification in %",
      (void *) 100  },
{ config_INT,
      "print.border",
      (void *)offsetof(struct config_str, print_border),
      "Border, in points, around the printed page",
      (void *) 36  },
{ config_BOOL,
      "print.nopics",
      (void *)offsetof(struct config_str, print_nopics),
      "Suppress pictures in printout",
      (void *) 0  },
{ config_BOOL,
      "print.noback",
      (void *)offsetof(struct config_str, print_nobg),
      "Suppress backgrouds in printout",
      (void *) 1  },
{ config_BOOL,
      "print.nocols",
      (void *)offsetof(struct config_str, print_nocol),
      "Suppress text colours in printout",
      (void *) 0  },
{ config_BOOL,
      "print.sideways",
      (void *)offsetof(struct config_str, print_sideways),
      "Print pages sideways",
      (void *) 0  },
{ config_BOOL,
      "print.collated",
      (void *)offsetof(struct config_str, print_collated),
      "Collate multiple copies",
      (void *) 0  },
{ config_BOOL,
      "print.reversed",
      (void *)offsetof(struct config_str, print_reversed),
      "Print pages in reverse order",
      (void *) 0  },

#ifdef STBWEB
{ config_STRING,
      "sound.click",
      (void *)offsetof(struct config_str, sound_click),
      "Command to run when selecting",
      (void *)0  },
{ config_BOOL,
      "sound.fx",
      (void *)offsetof(struct config_str, sound_fx),
      "User audio feedback sounds",
      (void *)0  },
{ config_BOOL,
      "sound.background",
      (void *)offsetof(struct config_str, sound_background),
      "Play background sounds",
      (void *)0  },
{ config_INT,
      "mode.keyboard",
      (void *)offsetof(struct config_str, mode_keyboard),
      "Do we have a full keyboard?",
      (void *)1  },
{ config_BOOL,
      "mode.cursor.toolbar",
      (void *)offsetof(struct config_str, mode_cursor_toolbar),
      "Move cursor to toolbar",
      (void *)1  },
{ config_INT,
      "mode.platform",
      (void *)offsetof(struct config_str, mode_platform),
      "Platform type",
      (void *)0  },
{ config_INT,
      "mode.mouse.adjust",
      (void *)offsetof(struct config_str, mode_mouse_adjust),
      "Adjust click action",
      (void *)0x203E  },
{ config_INT,
      "mode.mouse.menu",
      (void *)offsetof(struct config_str, mode_mouse_menu),
      "Menu click action",
      (void *)0x2001  },
{ config_INT,
      "mode.errors",
      (void *)offsetof(struct config_str, mode_errors),
      "Action on errors",
      (void *)0  },
{ config_INT,
      "event.smartcard.in",
      (void *)offsetof(struct config_str, event_smartcard_in),
      "Event on smartcard insertion",
      (void *)0  },
{ config_INT,
      "event.smartcard.out",
      (void *)offsetof(struct config_str, event_smartcard_out),
      "Event on smartcard removal",
      (void *)0  },
{ config_INT,
      "event.standby.in",
      (void *)offsetof(struct config_str, event_standby_in),
      "Event on entering standby",
      (void *)0  },
{ config_INT,
      "event.standby.out",
      (void *)offsetof(struct config_str, event_standby_out),
      "Event on leaving standby",
      (void *)0  },
#endif
{ config_INT,
      "history.length",
      (void *)offsetof(struct config_str, history_length),
      "The number of pages in the per-view history list",
      (void *) 50 },
{ config_INT,
      "history.global.length",
      (void *)offsetof(struct config_str, history_global_length),
      "The number of unique pages in the global history list",
      (void *) 250 },
{ config_BOOL,
      "history.persist",
      (void *)offsetof(struct config_str, history_persist),
      "Should the history list be kept across closedowns?",
      (void*) 0 },
{ config_BOOL,
      "broken.formpost",
      (void *)offsetof(struct config_str, broken_formpost),
      "Set this to use Netscape POST redirections",
#ifdef STBWEB
      (void *)1
#else
      (void *)0
#endif
      },
{ config_BOOL,
      "cookie.enable",
      (void *)offsetof(struct config_str, cookie_enable),
      "Shall we receive and send cookies",
      (void *)1 },
{ config_BOOL,
      "cookie.uptodate",
      (void *)offsetof(struct config_str, cookie_uptodate),
      "Save cookies on every update",
#ifdef STBWEB
      (void *)1
#else
      (void *)0
#endif
      },
{ config_BOOL,
      "passwords.uptodate",
      (void *)offsetof(struct config_str, passwords_uptodate),
      "Save passwords on every update",
#ifdef STBWEB
      (void *)1
#else
      (void *)0
#endif
      },
{ config_FILE,
      "plugin.file",
      (void *)offsetof(struct config_str, plugin_file),
      "Configure file for plugins",
#ifdef STBWEB
      "<"PROGRAM_NAME"$PlugIns>"
#else
      "<"PROGRAM_NAME"$Dir>.PlugIns"
#endif
      },
{ config_BOOL,
      "plugin.uptodate",
      (void *)offsetof(struct config_str, plugin_uptodate),
      "Keep plugin file up to date",
      (void *)1 },
{ config_BOOL,
      "netscape.fake",
      (void *)offsetof(struct config_str, netscape_fake),
      "Alter UserAgent header",
#ifdef STBWEB
      (void *)1
#else
      (void *)0
#endif
      },
{ config_INT,
      "hots.length",
      (void *)offsetof(struct config_str, hots_length),
      "Max number in hotlist",
      (void *)50 },

{ config_STRING,
      "language.preference",
      (void *)offsetof(struct config_str, language_preference),
      "Which (human) languages you'd prefer, e.g. 'en-gb,en,en-us'",
      (void*)0 },
{ config_INT,
      "browser.version",
      (void *)offsetof(struct config_str, browser_version),
      "Which version of the browser wrote this file?",
      (void*)0 },
{ config_STRING,
      "image.blacklist",
      (void *)offsetof(struct config_str, image_blacklist),
      "Substrings of image URLs not to fetch (e.g. adverts)",
      (void*)0 },
{ config_STRING,
      "publishing.fetch.prefix",
      (void *)offsetof(struct config_str, publishing_fetch_prefix),
      "HTTP (fetch) prefix of editable pages",
      (void*)0 },
{ config_STRING,
      "publishing.send.prefix",
      (void *)offsetof(struct config_str, publishing_send_prefix),
      "FTP (send) prefix of editable pages",
      (void*)0 },
{ config_STRING,
      "publishing.more",
      (void *)offsetof(struct config_str, publishing_more),
      "File containing <fetchprefix> <sendprefix> [<indexpage>] lines",
      (void*)0 },
{ config_STRING,
      "publishing.index.page",
      (void *)offsetof(struct config_str, publishing_index_page),
      "Index page (if not 'index.html')",
      (void*)0 },
{ config_BOOL,
      "publishing.active",
      (void *)offsetof(struct config_str, publishing_active),
      "Is active (server-parsed) HTML used?",
      (void*)0 },
{ config_BOOL,
      "hots.run.hotlist",
      (void *)offsetof(struct config_str, hots_run_hotlist),
      "'Show hotlist' calls !Hotlist",
      (void*)0 },

#ifdef STBWEB
{ config_INT_LIST,
      "toolbar",
      (void *)offsetof(struct config_str, toolbar),
      "Toolbar info",
      (void *)0 },
{ config_STRING,
      "toolbar.name",
      (void *)offsetof(struct config_str, toolbar_name),
      "Toolbar name",
      (void *)0 },
{ config_INT_LIST,
      "key",
      (void *)offsetof(struct config_str, key),
      "Key info",
      (void *)0 },
#endif

{ config_STRING_LIST,
      "url.suffix",
      (void *)offsetof(struct config_str, url_suffix),
      "List of possible URL suffixes",
      (void*)0 },

{ config_STRING,
      "header.useragent.0",
      (void *)offsetof(struct config_str, header_useragent[0]),
      "Standard User-Agent header",
      (void *)"%s (ANTFresco/%s; %.*s)" },
{ config_STRING,
      "header.useragent.1",
      (void *)offsetof(struct config_str, header_useragent[1]),
      "Faking User-Agent header",
#ifdef STBWEB
      (void *)"Mozilla/2.0NC-1 (compatible; %s; ANTFresco/%s; %.*s)" },
#else
      (void *)"Mozilla/2.0 (compatible; ANTFresco/%s; %.*s)" },
#endif
{ config_LAST, NULL, NULL, NULL, 0 }
};

#ifdef __acorn
/* According to the Dr Smiths documentation, there's a bug in Norcroft 5
 * when zpc1 is active which stops *((int*)foo) from working properly.
 * Sadly we must thus turn off checking in this file (or everything ends
 * up shades of light blue -- no, that's not a metaphor, it *does*).
 */
#pragma -c0
#endif

/**********************************************************************/
/* This gets called to set up all the config */
extern void config_init(void)
{
    int font_version;

    config_default_first();

    if (filter_fn)
	filter_fn(config_filter_phase_START, NULL, NULL);

    config__read_file();
#ifdef STBWEB
    config_read_variables();
#endif

    if (filter_fn)
	filter_fn(config_filter_phase_STOP, NULL, NULL);

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

#ifdef RISCOS
    /* get font version number the nicer way */
    _swix(Font_CacheAddr, _OUT(0), &font_version);
    if (font_version < 336)
        config_display_blending = FALSE;
    if (font_version < 340)
    {
	/* 16bit fonts not available */
    }
#endif
}

static void config_free_item(config_item *cp)
{
    switch(cp->type)
    {
    case config_STRING:
    case config_FILE:
    case config_URL:
    case config_FONT:
    case config_INT_LIST:
    case config_COLOUR_LIST:
	if (*((char**) cp->ptr))
	{
	    mm_free(*((char**) cp->ptr));
	    *(char **)cp->ptr = NULL;
	}
	break;
    case config_STRING_LIST:
    {
	int *list = *((int**) cp->ptr);
	if (list)
	{
	    int i;
	    for (i = 1; i <= list[0]; i++)
		mm_free((char *)list[i]);
	    mm_free(list);
	    *(char **)cp->ptr = NULL;
	}
	break;
    }
    }
}

extern void config_tidyup(void)
{
    int i;
    for (i = 0; citems[i].type != config_LAST; i++)
	config_free_item(&citems[i]);
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

static int config_read_int(const char *p)
{
    int val;
    if (*p == '&' || *p == '#')
    {
	val = (int) strtol(p+1, NULL, 16);
    }
    else if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
    {
	val = (int) strtol(p+2, NULL, 16);
    }
    else if (isdigit(*p) || *p == '-')
    {
	val = (int) strtol(p, NULL, 10);
    }
    else
	val = config_read_bool(p);

    CNFDBGN(("Value string '%s' taken as integer value %d\n", p, val));
    return val;
}

static int config_read_bool(const char *p)
{
    int val;
    if (strcasecomp(p, "YES") == 0 ||
	strcasecomp(p, "Y") == 0 ||
	strcasecomp(p, "ON") == 0 ||
	strcasecomp(p, "TRUE") == 0 ||
	atoi(p) != 0)
        val = 1;
    else
	val = 0;

    CNFDBGN(("Value string '%s' is boolean %d\n", p, val) );
    return val;
}

static int config_read_colour(char *p, char *q, char *s)
{
    int val;
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

	val = (b << 24) + (g << 16) + (r << 8);
    }
    else
    {
	if (p[0] == '#' || p[0] == '&')
	    p += 1;
	else if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
	    p += 2;

	val = (int) strtoul(p, NULL, 16);
    }

    CNFDBGN(("Value string '%s' is for colour 0x%08x\n", p, val));
    return val;
}

static char *config_read_string(char *p)
{
    char *q;
    char *val = NULL;

    while(*p && isspace(*p))
	p++;

    if ((*p == '"' && (q = strrchr(p, '"')) != p) ||
	(*p == '\'' && (q = strrchr(p, '\'')) != p) )
    {
	p++;
	*q = 0;
	val = strdup(p);
    }
    else
    {
	q = p + strlen(p);
	while (q != p && isspace(q[-1]))
	    q--;
	*q = 0;
	if (*p)
	    val = strdup(p);
    }

    CNFDBGN(("Value string '%s' taken as string value '%s'\n", p,
	     val ? val : "<none>" ));

    return val;
}

#define LIST_CHUNK	8

static int *config_read_list(char *p, int list_type)
{
    int count, *list;

    count = 0;
    list = NULL;
    do
    {
	if ((count % LIST_CHUNK) == 0)
	    list = mm_realloc(list, (count + LIST_CHUNK + 1) * sizeof(int));

	switch (list_type)
	{
	case config_INT_LIST:
	    list[++count] = config_read_int(p);
	    break;

	case config_COLOUR_LIST:
	    list[++count] = config_read_colour(p, NULL, NULL);
	    break;

	case config_STRING_LIST:
	    list[++count] = (int)config_read_string(p);
	    break;
	}

	CNFDBG(("config_read_list:val %08x\n", list[count]));
    }
    while ((p = strtok(NULL, " ,\t")) != NULL);

    /* shrink down to what is needed */
    list = mm_realloc(list, (count + 1) * sizeof(int));

    /* fill in the actual length */
    list[0] = count;

    return list;
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

static void config_read_item(char *p, config_item *citem)
{
    switch(citem->type)
    {
    case config_INT:
	if (p)
	    *(int *)citem->ptr = config_read_int(p);
	break;
    case config_BOOL:
	if (p)
    	    *(int *)citem->ptr = config_read_bool(p);
	break;
    case config_COLOUR:
	if (p)
	{
	    char *q1 = strtok(NULL, " \t\n\r,");
	    char *q2 = strtok(NULL, " \t\n\r,");
  	    *(int *)citem->ptr = config_read_colour(p, q1, q2);
	}
	break;

    case config_STRING:
    case config_FILE:
    case config_URL:
    case config_FONT:
	/* unlike above cases, free the string even if there's nothing
          valid to replace it with */
	if (*((char**) citem->ptr))
	    mm_free(*((char**) citem->ptr));
	*((char**) citem->ptr) = 0;

	if (p)
	    *(char **)citem->ptr = config_read_string(p);
	break;

    case config_INT_LIST:
    case config_COLOUR_LIST:
    case config_STRING_LIST:
	/* unlike above cases, free the string even if there's nothing
          valid to replace it with */
	config_free_item(citem);

	if (p)
	    *(int **)citem->ptr = config_read_list(p, citem->type);
	break;
    }

    /* call a registered filter function which may want to do something with the value just read */
    if (filter_fn && filter_fn(config_filter_phase_ADD, citem->name, *(const void **)citem->ptr))
    {
	switch(citem->type)
	{
	case config_STRING:
	case config_FILE:
	case config_URL:
	case config_FONT:
	case config_INT_LIST:
	case config_COLOUR_LIST:
	case config_STRING_LIST:
	    config_free_item(citem);
	    break;
	}
    }
}

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

	if (val)
	    config_read_item(val, &citems[i]);
    }
}
#endif

void config_read_file_by_name(const char *file_name)
{
    char buffer[512];
    char *p;
    int i;
    FILE *fh;

    fh = mmfopen(file_name, "r");

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

	    p = strtok(NULL, " \t\n\r");
	    config_read_item(p, &citems[i]);
	}
	else
	{
	    break;
	}
    }

    mmfclose(fh);

#ifdef FRESCO
    /* pdh: "trapdoor" to auto-enable frames on old frescoes */
    if ( config_browser_version < 158 )
    {
        config_display_frames = TRUE;
        config_broken_formpost = TRUE;
    }
#endif
    config_browser_version = (int)(100 * atof( VERSION_NUMBER ) + 0.1);

    config_has_been_changed = 0;
}

static void config__read_file(void)
{
    config_read_file_by_name(CONFIG_FILE_NAME_2);
}

void config_read_file(void)
{
    if (filter_fn)
	filter_fn(config_filter_phase_START, NULL, NULL);

    config__read_file();

    if (filter_fn)
	filter_fn(config_filter_phase_STOP, NULL, NULL);
}

/* fixup the array to be absolute addresses before writing out default */
/* this is necessary to allow a module build */

static void config_default_first(void)
{
    int i;

    for(i=0; citems[i].type != config_LAST; i++)
    {
	config_item *cp = &citems[i];

	switch(cp->type)
	{
	case config_INT:
	case config_BOOL:
	case config_COLOUR:
            cp->ptr = (char *)&config_array + (int)(long)cp->ptr;
	    *((int*) (long)cp->ptr) = (int)(long)(cp->def);
	    break;

	case config_FONT:
	case config_STRING:
	case config_FILE:
	case config_URL:
            cp->ptr = (char *)&config_array + (int) (long) cp->ptr;
	    if ((char*) cp->def == 0)
		*((char**) cp->ptr) = 0;
	    else
		*((char**) cp->ptr) = strdup((char*) cp->def);
	    break;

	case config_INT_LIST:
	case config_COLOUR_LIST:
	case config_STRING_LIST:
            cp->ptr = (char *)&config_array + (int) (long) cp->ptr;
	    if ((int*) cp->def == 0)
		*((int**) cp->ptr) = 0;
	    else
		*((int**) cp->ptr) = config_read_list((char *)cp->def, cp->type);
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

    fh = mmfopen(file_name, "w");

    if (fh == NULL)
    {
	usrtrc( "Config file unwritable\n");
	return;
    }

    if (filter_fn)
	filter_fn(config_filter_phase_START_WRITE, NULL, fh);

    for(i=0; citems[i].type != config_LAST; i++)
    {
	config_item *cp = &citems[i];

	if (cp->comment)
	    fprintf(fh, "# %s\n", cp->comment);

	if (filter_fn && filter_fn(config_filter_phase_WRITE, cp->name, *(const void **)cp->ptr))
	{
	}
	else switch(cp->type)
	{
	case config_INT:
	    fprintf(fh, "%s: %d\n", cp->name, *((int*) cp->ptr));
	    break;
	case config_BOOL:
	    fprintf(fh, "%s: %s\n", cp->name, *((int*) cp->ptr) ? "Yes" : "No");
	    break;
	case config_COLOUR:
	    x = *((int*) cp->ptr);
	    fprintf(fh, "%s: 0x%02x, 0x%02x, 0x%02x\n", cp->name, (x >> 8) & 0xff, (x >> 16) & 0xff, (x >> 24) & 0xff);
	    break;
	case config_FONT:
	case config_STRING:
	case config_FILE:
	case config_URL:
	    p = *((char**) cp->ptr);
	    fprintf(fh, "%s: %s\n", cp->name,  p ? (*p == 0 ? "\"\"" : p) : "" );
	    break;

	case config_INT_LIST:
	case config_COLOUR_LIST:
	{
	    int count, *xp;

	    fprintf(fh, "%s:", cp->name);

	    xp = *((int**) cp->ptr);
	    if (xp)
	    {
		for (count = *xp++; count; count--)
		{
		    if (cp->type == config_COLOUR_LIST)
			fprintf(fh, " 0x%08x", *xp++);
		    else
			fprintf(fh, " %d", *xp++);
		}
	    }

	    fputc('\n', fh);
	    break;
	}

	case config_STRING_LIST:
	{
	    int count, *xp;

	    fprintf(fh, "%s:", cp->name);

	    xp = *((int**) cp->ptr);
	    if (xp)
	    {
		for (count = *xp++; count; count--)
		{
		    fprintf(fh, " %s", (char *)*xp++);
		}
	    }

	    fputc('\n', fh);
	    break;
	}
	}
    }

    mmfclose(fh);

    if (filter_fn)
	filter_fn(config_filter_phase_STOP_WRITE, NULL, NULL);

    config_has_been_changed = 0;
}

extern void config_write_file(void)
{
    config_write_file_by_name(CONFIG_FILE_NAME_2);
}

extern void config_set_filter(config_filter_fn fn)
{
    filter_fn = fn;
}

/* eof config.c */
