/* -*-c-*- */

/* view.h */

/* ----------------------------------------------------------------------------- */

typedef struct frame_link frame_link;

#define frame_link_flag_SELECTED	0x01

struct frame_link
{
    frame_link *next;		/* next link in chain */
    fe_view v;			/* the frame this link goes to */
    int side;			/* the side that this link is on (so we know which arrow to display) */
    wimp_box box;		/* bounding box of this link, in parent relative coordinates */
    int flags;			/* see above */
};

/* ----------------------------------------------------------------------------- */

typedef struct fe_message_handler_item fe_message_handler_item;

struct fe_message_handler_item
{
    fe_message_handler_item *next;

    frontend_message_handler fn;
    void *handle;
};

/* ----------------------------------------------------------------------------- */

struct _frontend_passwd_handle
{
    backend_passwd_callback cb; /* values passed in originally */
    void *h;
    char *realm, *site, *user;
};

struct _frontend_ssl_handle
{
    backend_ssl_callback cb; /* values passed in originally */
    void *h;
    char *issuer, *subject;
};

/* ----------------------------------------------------------------------------- */

typedef struct fe_history_item fe_history_item;
typedef struct _frontend_view frontend_view;

/* ----------------------------------------------------------------------------- */

/* This describes what is in a view.  Only the front end needs to know. */

#define ANTWEB_VIEW_MAGIC	0x469034fb

struct _frontend_view
{
    int magic;          /* Magic number */
    frontend_view *parent;      /* up */
    frontend_view *prev;        /* along */
    frontend_view *next;        /* along */
    frontend_view *children;        /* down */
    frontend_view *children_last;   /* where to add new frames */
    int flags;          /* passed (modified) to backend_open_url */

    char *name;

/*    int width;		// the size of the actual background window (ie full screen) */
/*    int height; */

    int doc_width;      /* the extent of the document (from backend) */
    int doc_height;

    wimp_w w;           /* wimp window handle */

    wimp_box margin;    /* this is the offsets to the definitely visible bit of the browser screen */
    wimp_box backend_margin;    /* any margin defined by a FRAMESET tag */
    wimp_box box;       /* this is the box as defined by the frameset */

    int dbox_x, dbox_y;		/* sizes for dbox thingies */
    
/*    int scroll_x;       // distance from lwa origin to left of screen +ve right */
/*    int scroll_y;       // distance from lwa origin to top of screen  +ve up */

    be_doc displaying;
    be_doc fetching;
    fe_history_item *first, *last, *hist_at;
    int hist_count;

    char last_search[64];

    int current_object_type;

    int status;             /* as defined in status.h and interface.h */

    int fetch_status;       /* see below */
    int fetch_document;     /* from 0-256 */
    int fetch_images;       /* from 0-256 */

    int fetch_counter;
    int fetch_counter_inc;

    int slider_start;
    int slider_end;

/*     be_item current_link; */   /* currently highlighted link */

    int x_scroll_bar;
    int y_scroll_bar;
    int scrolling;          /* 0 = auto, 1 = yes, 2 = no */

/*     int pending_scroll; */

    int dont_add_to_history;
    int pending_download_finished;

    be_item find_last_item; /* set to top of screen when opened */

    int resize_handle;

    int stretch_document;   /* +ve value */

    char *return_page;      /* record control pages so we can return to it */
    char *app_return_page;  /* last page used before running an app */
    
    int browser_mode;
    int status_open;

    int pending_mode_change;

    /* from here the completion status is calculated */
    int fetch_rx;           /* used by tb status */
    int fetch_of;

    int had_completed;

    int images_had;
    int images_waiting;

    int from_frame;

    int threaded;
    int delete_pending;

    char *selected_id;

    int is_selected;		/* this view is the selected one */

    int open_transient;
    int frame_index;		/* frame number from 0 */

    int dividers[4];
    int dividers_max;

    frame_link *frame_links;

    int fast_load;

    int transient_position;

    struct
    {
	int xscroll, yscroll;	/* scroll positions to move to - used in check_pending_scroll */
	fe_history_item *hist;	/* history item to be current  - used in visit */
    } fetching_data;

    char *real_url;		/* with NCOPTIONS pages this will give the url to put in the history list */
    int offline_mode;
    int external_open;		/* opened from an external message */

    int pending_user;
    char *pending_user_name;

    char *onload, *onunload,	/* ncfrescointernal functions to be called after loading and unloading */
	*onblur;

    char *submitonunload;

    int select_button;		/* event number of probable triggering event, button should be selected */
    char *specialselect;	/* ID of element to select when special event received */
};

/* ----------------------------------------------------------------------------- */

#define PROFILE_NUM_VAR		"Current$User"
#define PROFILE_NAME_VAR	"User$Name"

#define NVRAM_FONTS_TAG		"BrowserFontSize"
#define NVRAM_SOUND_TAG		"BrowserMusicStatus"
#define NVRAM_BEEPS_TAG		"BrowserBeepStatus"
#define NVRAM_SCALING_TAG	"BrowserScaling"
#define NVRAM_PRINT_COLOUR_TAG	"PrinterColour"
#define NVRAM_PRINT_ORIENTATION_TAG "PrinterOrientation"
#define NVRAM_PRINT_BG_TAG	"PrinterPrintBackgrounds"
#define NVRAM_PRINT_IMAGES_TAG	"PrinterPrintImages"
#define NVRAM_DISPLAY_BG_TAG	"BrowserDisplayBackgrounds"
#define NVRAM_DISPLAY_IMAGES_TAG "BrowserDisplayImages"

/* ----------------------------------------------------------------------------- */

#define fe_keyboard_UNKNOWN	(-1)
#define fe_keyboard_ONLINE	0
#define fe_keyboard_OFFLINE	1

extern int keyboard_state;

#define fe_object_type_NONE      0
#define fe_object_type_LINK      1
#define fe_object_type_MAP       2
#define fe_object_type_FORM      3

#define fe_fetch_status_CONNECTING      0
#define fe_fetch_status_FETCHING        1
#define fe_fetch_status_COMPLETED       2
#define fe_fetch_status_ABORTED         3

#define STATUS_TOP_MARGIN		0 /* 16 */

extern fe_view frameutils_find_top(fe_view v);

/* ----------------------------------------------------------------------------- */

#define fe_position_TOOLBAR_WITH_COORDS		5
#define fe_position_CENTERED_WITH_COORDS	4
#define fe_position_COORDS	3
#define fe_position_TOOLBAR	2
#define fe_position_CENTERED	1
#define fe_position_FULLSCREEN	0
#define fe_position_UNSET	(-1)
	    
/* ----------------------------------------------------------------------------- */

/* extra scrolling code in addition to those in interface.h
 */
#define fe_scrolling_INVISIBLE	(0xff) /* no scroll bars but can scroll page */

/* ----------------------------------------------------------------------------- */

/* eof stbview.h */
