/* > stbtb.c

 * Toolbox support for the web browser

 * We only have one menu tree and one status bar.
 * The status bar is considered to be statically independent of all windows and what its
 * context relates to is decided by the calling functions (ie stbfe mostly).

 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "akbd.h"
#include "alarm.h"
#include "bbc.h"
#include "coords.h"
#include "colourtran.h"
#include "msgs.h"
#include "swis.h"
#include "wimp.h"

#include "config.h"
#include "interface.h"
#include "memwatch.h"
#include "util.h"
#include "version.h"
#include "pluginfile.h"

#include "stbtb.h"
#include "stbutils.h"
#include "stbfe.h"
#include "stbview.h"
#include "stbhist.h"
#include "stbopen.h"
#include "fevents.h"
#include "hotlist.h"

/* --------------------------------------------------------------------------*/

#define LIGHT_OFF_DELAY     (2*100)

/* --------------------------------------------------------------------------*/

typedef void (*tb_bar_entry_fn)(fe_view v);
typedef void (*tb_bar_exit_fn)();

typedef struct
{
    char *tv_name;
    char *monitor_name;

    tb_bar_entry_fn entry_fn;
    tb_bar_exit_fn exit_fn;

    int initial_component;	/* when moving onto it */
    int open_component;		/* when opening it */
    int return_component;

    int can_grey;

    int return_bar;		/*  -1 means stack */
} tb_bar_descriptor;

#define BAR_CANT	(-2)
#define BAR_STACK	(-1)
#define BAR_MAIN	0
#define BAR_EXTRAS	2
#define BAR_STATUS	8
#define BAR_CODEC	9

/* --------------------------------------------------------------------------*/

#define Toolbox_CreateObject                    0x44EC0
#define Toolbox_DeleteObject                    0x44EC1
#define Toolbox_ShowObject                      0x44EC3
#define Toolbox_HideObject                      0x44EC4
#define Toolbox_GetObjectInfo                   0x44EC5
#define Toolbox_ObjectMiscOp                    0x44EC6
#define Toolbox_SetClientHandle                 0x44EC7
#define Toolbox_GetClientHandle                 0x44EC8
#define Toolbox_GetObjectClass                  0x44EC9
#define Toolbox_GetParent                       0x44ECA
#define Toolbox_GetAncestor                     0x44ECB
#define Toolbox_GetTemplateName                 0x44ECC
#define Toolbox_RaiseToolboxEvent               0x44ECD
#define Toolbox_GetSysInfo                      0x44ECE
#define Toolbox_Initialise                      0x44ECF
#define Toolbox_TemplateLookUp                  0x44EFB

#define Window_WimpToToolbox                    0x82884
#define Gadget_GetHelpMessage                   0x43

#define Menu_GetClickEvent			15

/* --------------------------------------------------------------------------*/

/* Some toolbox headers from OSLib for convenience*/

struct toolbox_resource_file_object
{
    int class_no;
    int flags;
    int version;
    char name [12];
    int size;
    int header_size;
    int body_size;
/*  int object [UNKNOWN];*/
};

typedef char *toolbox_msg_reference;
typedef char *toolbox_string_reference;
typedef void *toolbox_sprite_area_reference;
typedef int *toolbox_object_offset;

struct menu_object
{
    int flags;
    toolbox_msg_reference title;
    int title_limit;
    toolbox_msg_reference help;
    int help_limit;
    int show_action;
    int hide_action;
    int entry_count;
/*  menu_entry_object entries [UNKNOWN];*/
};

typedef struct
{
    int x, y;
} os_coord;

typedef struct
{
    int     size;
    int     ref_no;
    int     action_no;
    int     flags;
    union
    {
	char          bytes [212];
	int           words [53];
    } data;
} toolbox_action;

union window_icon_data
{
    struct
    {
	toolbox_msg_reference text;
	toolbox_string_reference validation;
	int size;
    }
    indirected_text;
};

struct window_window
{
    wimp_box visible;
    int xscroll;
    int yscroll;
    wimp_w next;
    int flags;
    char title_fg;
    char title_bg;
    char work_fg;
    char work_bg;
    char scroll_outer;
    char scroll_inner;
    char highlight_bg;
    char reserved;
    wimp_box extent;
    int title_flags;
    int work_flags;
    toolbox_sprite_area_reference sprite_area;
    short xmin;
    short ymin;
    union window_icon_data title_data;
    int icon_count;
};

struct window_object
{
    int flags;
    toolbox_msg_reference help_message;
    int help_limit;
    toolbox_string_reference sprite_name;
    int pointer_limit;
    os_coord hotspot;
    toolbox_string_reference menu_name;
    int shortcut_count;
    toolbox_object_offset shortcuts;
    int gadget_count;
    toolbox_object_offset gadgets;
    int default_focus;
    int show_action;
    int hide_action;
    toolbox_string_reference toolbar_ibl;
    toolbox_string_reference toolbar_itl;
    toolbox_string_reference toolbar_ebl;
    toolbox_string_reference toolbar_etl;
    struct window_window window;
/*  int data [UNKNOWN];*/
};

struct menu_entry_object
{
    int flags;
    int cmp;
    char *text;
    int text_limit;
    char *click_object_name;
    char *sub_menu_object_name;
    int sub_menu_action;
    int click_action;
    char *help;
    int help_limit;
};

struct gadget_object
{   int flags;
    int class_no;
    wimp_box bbox;
    int cmp;
    toolbox_msg_reference help_message;
    int help_limit;
/*  int gadget [UNKNOWN]; */
};

/* --------------------------------------------------------------------------*/
/* --------------------------------------------------------------------------*/

typedef enum
{
    status_CLOSED,
    status_OPEN,
    status_OPEN_SMALL
} tb_status_state_t;

/* --------------------------------------------------------------------------*/

#define I_URL       	0x10
#define I_STATUS        0x12
#define I_WORLD       	0x13
#define I_WORLD_BORDER	0x14
#define I_LIGHTS_GREEN  0x100
#define I_LIGHTS_YELLOW 0x101
#define I_LIGHTS_RED    0x102
#define I_SECURE        0x103
#define I_DIRECTION	0x104

/* --------------------------------------------------------------------------*/

typedef struct tb_bar_info tb_bar_info;

typedef struct tb_button_info tb_button_info;

struct tb_bar_info
{
    tb_bar_info *prev, *next;

    int object_handle;
    wimp_w window_handle;
    int num;
    int height;

    tb_button_info *buttons;	/* the buttons visible in the window, sorted */
    int n_buttons;

    int return_bar;		/* index of bar to open when closing */
    int return_component;	/* component to highlight when closing */
};

struct tb_button_info
{
    int x_pos;			/* position of left edge */
    int cmp;			/* component number */
};

/* linked list of tool bars open. The one at the head of the list (*bar_list)
 * is the one currently visible
 */

static tb_bar_info *bar_list = NULL;
static void *tile_sprite = NULL;
static int secure_light = FALSE;

/* --------------------------------------------------------------------------*/
/* --------------------------------------------------------------------------*/

/* messagetrans and toolbox id blocks*/
static int m_block[4];
static int tb_block[6];

/* list of supported toolbox messages*/
static int tb_list[] =
{
    0
};

/* toolbox object handles*/
static int menu_object[2] = { 0, 0 };

/* --------------------------------------------------------------------------*/

/* which sprite from the animation*/

#define TURN_SPEED	8	/* frames per second */

static int turn_ctr = 0;
static int turn_start = -1;

/* --------------------------------------------------------------------------*/

static tb_status_state_t status_state = status_CLOSED;

/* --------------------------------------------------------------------------*/

static char *status_messages[6] = { 0, 0, 0, 0, 0, 0, };
static char *status_prefixes[6] = { 0, "url", "ttl", "lnk", 0, 0, };
static int status_current_type = status_type_URL;
static char status_changed[6] = { 0, 0, 0, 0, 0, 0, };

/* --------------------------------------------------------------------------*/

/* toolbox utility functions*/

static os_error *mfade(int obj, int cmp, int fade)
{
    return (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, 2, cmp, fade);
}

static os_error *mtick(int obj, int cmp, int tick)
{
    return (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, 0, cmp, tick);
}

static os_error *gfade(int obj, int cmp, int fade)
{
    unsigned flags;
    os_error *e;
    e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,3)|_OUT(0), 0, obj, 0x40, cmp, &flags);
    if (!e)
    {
        int old_fade = (flags & 0x80000000) != 0;
        if (old_fade != fade)
            e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, 0x41, cmp, flags ^ 0x80000000);
    }
    return e;
}

#if 0
static int faded(int obj, int cmp)
{
    unsigned flags;
    os_error *e;
    e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,3)|_OUT(0), 0, obj, 0x40, cmp, &flags);
    return e == NULL && (flags & 0x80000000) != 0;
}
#endif

#define SELECTABLE_AND		0x80000001 /* was 0x80000001 to mask out faded*/
#define SELECTABLE_MATCH	0x00000001

static int selectable(int obj, int cmp)
{
    unsigned flags;
    os_error *e;
    e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,3)|_OUT(0), 0, obj, 0x40, cmp, &flags);
    return e == NULL && (flags & SELECTABLE_AND) == SELECTABLE_MATCH;
}

#if 0
static os_error *sethelp(int obj, int cmp, const char *msg)
{
    os_error *e;

    if (cmp == -1)
        e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,3), 0, obj, 0x7, msg);
    else
        e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, 0x42, cmp, msg);

    return e;
}
#endif

static os_error *setfield(int obj, int cmp, const char *msg, int check)
{
    int type;
    _kernel_oserror *e;
    e = _swix(Toolbox_ObjectMiscOp, _INR(0,3)|_OUT(0), 0, obj, 70, cmp, &type);
    if (!e)
    {
        int read = 0, write = 0;
        switch (type & 0xffff)
        {
            case 128:   /* action*/
                write = 128;
                read = 129;
                break;
            case 448:   /* display*/
                write = 448;
                read = 449;
                break;
            case 512:   /* writable*/
                write = 512;
                read = 513;
                break;
            case 960:   /* button*/
                write = 962;
                read = 963;
                break;
            case 0x4014:   /* toolaction */
                write = 0x140140;
                read = 0x140141;
                break;
        }
        if (write)
        {
            if (msg == NULL)
                msg = "";

            if (check)
            {
                char buf[256];
                e = _swix(Toolbox_ObjectMiscOp, _INR(0,5), 0, obj, read, cmp, buf, sizeof(buf));
                buf[sizeof(buf)-1] = 0;
                if (!e && strcmp(buf, msg) == 0)
                    return NULL;
            }
            if (!e)
                e = _swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, write, cmp, msg);
        }
    }

    return (os_error *)e;
}

static char *getwriteable(int obj, int cmp)
{
    int size;
    char *s;
    _kernel_oserror *e;
    e = _swix(Toolbox_ObjectMiscOp, _INR(0,5)|_OUT(5), 0, obj, 513, cmp, 0, 0, &size);
    if (e)
        return strdup(e->errmess);

    s = mm_malloc(size);
    e = _swix(Toolbox_ObjectMiscOp, _INR(0,5), 0, obj, 513, cmp, s, size);
    if (e)
    {
        mm_free(s);
        return strdup(e->errmess);
    }
    return s;
}

#if 0
static int getval(int obj, int cmp)
{
    int type, val;
    _kernel_oserror *e;

    if (cmp == -1)
        return NULL;

    e = _swix(Toolbox_ObjectMiscOp, _INR(0,3)|_OUT(0), 0, obj, 70, cmp, &type);
    if (!e)
    {
        int read = 0;
        switch (type & 0xffff)
        {
	case 192:   /* option button*/
	    read = 197;
	    break;
	case 384:   /* radio button*/
	    read = 389;
	    break;
	case 576:   /* slider button*/
	    read = 577;
	    break;
	case 832:   /* numberrange button*/
	    read = 833;
	    break;
	case 0x4014:    /* toolaction*/
	    read = 0x140147;
	    break;
	case 512:		/* writeable, check focus */
	    break;
        }
        e = _swix(Toolbox_ObjectMiscOp, _INR(0,3) | _OUT(0), 0, obj, read, cmp, &val);
    }
    return e ? 0 : val;
}
#endif

static os_error *setval(int obj, int cmp, int val)
{
    int type;
    _kernel_oserror *e;

    if (cmp == -1)
        return NULL;

    e = _swix(Toolbox_ObjectMiscOp, _INR(0,3)|_OUT(0), 0, obj, 70, cmp, &type);
    if (!e)
    {
        int write = 0;
        switch (type & 0xffff)
        {
	case 192:   /* option button*/
	    write = 196;
	    break;
	case 384:   /* radio button*/
	    write = 388;
	    break;
	case 576:   /* slider button*/
	    write = 576;
	    break;
	case 832:   /* numberrange button*/
	    write = 832;
	    break;
	case 0x4014:    /* toolaction*/
	    write = 0x140146;
	    break;
	case 512:		/* writeable, does a set focus */
	    if (val)
		write = 69;
	    break;
        }
	if (write)
	    e = _swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, write, cmp, val);
    }
    return (os_error *)e;
}
#if 0
static int getstate(int obj, int cmp)
{
    int val = getval(obj, cmp);
    return val != 0;
}
#endif
static os_error *setstate(int obj, int cmp, int state)
{
    return setval(obj, cmp, state);
}

static os_error *movegadget(int obj, int cmp, int x0diff, int x1diff)
{
    _kernel_oserror *e;
    wimp_box box;
    e = _swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, 72, cmp, &box);
    if (!e)
    {
        box.x0 += x0diff;
        box.x1 += x1diff;
        e = _swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, 71, cmp, &box);
    }
    return (os_error *)e;
}
#if 0
static os_error *thickenborder(int obj, int cmp, const wimp_redrawstr *r)
{
    wimp_box box;
    wimp_paletteword col;
    int gcol;
    os_error *e;

    e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, 72, cmp, &box);

    col.word = 0;
    colourtran_setGCOL(col, 0, 0, &gcol);

    bbc_rectangle(
        box.x0-2 - r->scx + r->box.x0,
        box.y0-2 - r->scy + r->box.y1,
        box.x1-box.x0+2, box.y1-box.y0+2);
    return e;
}
#endif

static wimp_w window_handle(int obj)
{
    wimp_w w;
    if (obj == 0)
        return 0;

    frontend_fatal_error((os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,2)|_OUT(0), 0, obj, 0, &w));
    return w;
}

static void remove_title_bar(const char *name)
{
    struct toolbox_resource_file_object *obj;
    struct menu_object *mobj;

    if (_swix(Toolbox_TemplateLookUp, _INR(0,1) | _OUT(0), 0, name, &obj) == NULL && obj)
    {
	MemCheck_checking checking = MemCheck_SetChecking(0, 0);

	mobj = (struct menu_object *)obj->header_size;
	mobj->title = NULL;

	MemCheck_RestoreChecking(checking);
    }
}

static os_error *setfocus(int obj)
{
    wimp_caretstr cs;
    cs.w = window_handle(obj);
    cs.i = -1;
    cs.height = -1;
    cs.x = cs.y = 0;
    return wimp_set_caret_pos(&cs);
}


#if 0
static os_error *toolactionsetpair(int obj, int cmp, const char *off, const char *on)
{
    os_error *e;
    e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, 0x140140, cmp, off);
    if (!e) e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,4), 1, obj, 0x140140, cmp, on);
    return e;
}
#endif

static os_error *toolactionpress(int obj, int cmp, int pressed)
{
    os_error *e;
    int was_pressed;
    e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,3) | _OUT(0), 0, obj, 0x140149, cmp, &was_pressed);
    if (!e && was_pressed != pressed)
	e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, 0x140148, cmp, pressed);
    return e;
}

/* --------------------------------------------------------------------------*/

static void tb_bar_set_highlight(tb_bar_info *tbi, int val, BOOL is_index)
{
    if (val != -1)
    {
	wimp_box box;
	wimp_wstate state;

	STBDBG(("tb_bar_set_highlight(): bar %d index %d obj %x, cmp %x\n", tbi ? tbi->num : -1, val, tbi->object_handle, is_index ? tbi->buttons[val].cmp : val));
	    
	_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, tbi->object_handle, 72, is_index ? tbi->buttons[val].cmp : val, &box);
	wimp_get_wind_state(tbi->window_handle, &state);
	coords_box_toscreen(&box, (coords_cvtstr *)&state.o.box);

	if (pointer_mode == pointermode_OFF)
	    frontend_pointer_set_position(NULL, (box.x0 + box.x1)/2, (box.y0 + box.y1)/2);
    }
}

static int tb_bar_cmp_to_index(tb_bar_info *tbi, int cmp)
{
    int i;
    tb_button_info *but;
    for (i = 0, but = tbi->buttons; i < tbi->n_buttons; i++, but++)
	if (but->cmp == cmp)
	    return i;
    return -1;
}

static BOOL havefocus(tb_bar_info *tbi)
{
    wimp_caretstr cs;
    wimp_get_caret_pos(&cs);
    return cs.w == tbi->window_handle;
}

static int get_highlighted(tb_bar_info *tbi)
{
    int i, pressed;
    for (i = 0; i < tbi->n_buttons; i++)
	if (_swix(Toolbox_ObjectMiscOp, _INR(0,3) | _OUT(0), 0, tbi->object_handle, 0x140149, tbi->buttons[i].cmp, &pressed) == NULL &&
	    pressed)
	    return i;
    return -1;    
}

static int get_active(tb_bar_info *tbi)
{
    int i, active;
    for (i = 0; i < tbi->n_buttons; i++)
	if (_swix(Toolbox_ObjectMiscOp, _INR(0,3) | _OUT(0), 0, tbi->object_handle, 0x140147, tbi->buttons[i].cmp, &active) == NULL &&
	    active &&
	    tbi->buttons[i].cmp != I_SECURE &&	/* security light on is returned as ACTIVE but it's not what we meant */
	    tbi->buttons[i].cmp != I_DIRECTION)	/* arrow pointing up is returned as ACTIVE but it's not what we meant */
	    return i;
    return -1;    
}

static BOOL return_highlight(fe_view v, tb_bar_info *tbi, int flags)
{
    int cmp;
    wimp_box box;
    wimp_wstate state;
    int highlight = get_highlighted(tbi);
    int active = get_active(tbi);

    /* if there is an active highlight then can only move off it - unless on the codec toolbar */
    if (active != -1 && active != highlight && tbi->num != BAR_CODEC)
	return FALSE;
    
    /* get the position of the item we are moving off */
    cmp = tbi->buttons[highlight].cmp;

    _swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, tbi->object_handle, 72, cmp, &box);
    wimp_get_wind_state(tbi->window_handle, &state);
    coords_box_toscreen(&box, (coords_cvtstr *)&state.o.box);

    if (on_screen_kbd)
    {
	int x;

	/* move pointer back into keyboard and restart iconhigh */
	x = (state.o.box.x0 + state.o.box.x1) / 2;

	if (x < on_screen_kbd_pos.x0 + 8)
	    x = on_screen_kbd_pos.x0 + 8;
	if (x > on_screen_kbd_pos.x1 - 8)
	    x = on_screen_kbd_pos.x1 - 8;
	
	if (flags & be_link_BACK)
	    frontend_pointer_set_position(NULL, x, on_screen_kbd_pos.y0 + 16);
	else
	    frontend_pointer_set_position(NULL, x, on_screen_kbd_pos.y1 - 16);

	/* It is now up to the OSK to enable iconhigh */
/*  	_swix(IconHigh_Start, _IN(0), 0); */

	/* move highlight back into main window at old position */
	fe_move_highlight(fe_selected_view(), flags | be_link_INCLUDE_CURRENT | be_link_VISIBLE | be_link_TEXT);
    }
    else
    {
	fe_move_highlight_xy(v, &box, flags | be_link_XY);
    }
    
    return TRUE;
}

/* --------------------------------------------------------------------------*/

static void *sprite_load(const char *file_name)
{
    int type, size;
    int *area = NULL;
    if (_swix(OS_File, _INR(0,1) | _OUT(0)|_OUT(4), 0x17, file_name, &type, &size) == NULL)
    {
        if ((type & 1) && size > 0)
        {
            area = mm_malloc(size+4);

            area[0] = size+4;
            area[1] = 0;
            area[2] = 16;
            area[3] = 16;
            if (_swix(OS_SpriteOp, _INR(0,2), 0x10A, area, file_name) != NULL)
            {
                mm_free(area);
                area = NULL;
            }
        }
    }
    return area;
}

/* Check for the tile in a global place and in our own directory for standalone releases */

static void *sprite_load_tile(const char *suffix)
{
    char buffer[80];
    void *sprite_area;

    strcpy(buffer, "WindowManager:Tile");
    strcat(buffer, suffix);

    STBDBG(("sprite_load_tile: '%s'\n", buffer));
    
    if ((sprite_area = sprite_load(buffer)) == NULL)
    {
	strcpy(buffer, PROGRAM_NAME":Tile");
	strcat(buffer, suffix);
	sprite_area = sprite_load(buffer);

	STBDBG(("sprite_load_tile: '%s'\n", buffer));
    }

    STBDBG(("sprite_load_tile: returns %p\n", sprite_area));

    return sprite_area;
}

/* --------------------------------------------------------------------------*/

static int compare_info(const void *o1, const void *o2)
{
    const tb_button_info *b1 = (const tb_button_info *)o1;
    const tb_button_info *b2 = (const tb_button_info *)o2;
    return b1->x_pos - b2->x_pos;
}

static int tb_bar_create(const char *template_name, void *new_sprite_area, tb_button_info **list, int *n_buttons)
{
    struct toolbox_resource_file_object *obj;
    struct window_object *wobj;
    int object, i;
    struct gadget_object *gadget;
    tb_button_info *info, *info_list;
    
    /* create the window, fixing up the sprite area */
    frontend_fatal_error((os_error *)_swix(Toolbox_TemplateLookUp, _INR(0,1) | _OUT(0), 0, template_name, &obj));
    if (obj == NULL)
    {
	*list = NULL;
	return 0;
    }
    
    wobj = (struct window_object *)obj->header_size;
    if (new_sprite_area)
	wobj->window.sprite_area = new_sprite_area;

    STBDBG(("tb_bar_create(): %d buttons sprite_area %p\n", wobj->gadget_count, wobj->window.sprite_area));

    /* create a list of the gadgets in the window */
    gadget = (struct gadget_object *)wobj->gadgets;
    info = info_list = mm_calloc(sizeof(*info_list), wobj->gadget_count);
    for (i = 0; i < wobj->gadget_count; i++, info++)
    {
	info->x_pos = gadget->bbox.x0;
	info->cmp = gadget->cmp;

	if (gadget->cmp == I_WORLD)
	    gadget->flags |= 0x40000000;
	
	gadget = (struct gadget_object *)((char *)gadget + (gadget->class_no >> 16));

	STBDBG(("  x %d cmp %d\n", info->x_pos, info->cmp));
    }

    /* now sort the list into horizontal order */
    qsort(info_list, wobj->gadget_count, sizeof(*info_list), compare_info);
    
    /* create object after fixing up flags */
    frontend_fatal_error((os_error *)_swix(Toolbox_CreateObject, _INR(0,1) | _OUT(0), 0, template_name, &object));

    /* return pointer */
    *list = info_list;
    *n_buttons = wobj->gadget_count;
    
    return object;
}

/* --------------------------------------------------------------------------*/

static void tb_bar_favs_exit_fn(void)
{
    frontend_complain(hotlist_flush_pending_delete());
}

static void tb_bar_history_exit_fn(void)
{
}

static void tb_bar_details_exit_fn(void)
{
}

static void tb_bar_codec_exit_fn(void)
{
    fevent_handler(fevent_CODEC_CLOSE, NULL);
}

static void tb_bar_custom_exit_fn(void)
{
}

static void tb_bar_details_entry_fn(fe_view v)
{
}

/* --------------------------------------------------------------------------*/

/* This must agree with the defs in fevents.h */

static tb_bar_descriptor bar_names[] =
{
    { "mainT", NULL,
      0, 0,
      I_DIRECTION, fevent_HOME, -1,
      FALSE, BAR_CANT },
    { "favsT", NULL,
      0, tb_bar_favs_exit_fn,
      I_DIRECTION, fevent_HOTLIST_ADD, fevent_TOOLBAR_FAVS,
      FALSE, BAR_MAIN },
    { "extrasT", NULL,
      0, 0,
      I_DIRECTION, fevent_TOOLBAR_HISTORY, fevent_TOOLBAR_EXTRAS,
      FALSE, BAR_MAIN },
    { "historyT", NULL,
      0, tb_bar_history_exit_fn,
      I_DIRECTION, fevent_HISTORY_SHOW_ALPHA, fevent_TOOLBAR_HISTORY,
      FALSE, BAR_EXTRAS },
    { "printT", NULL,
      0, 0,
      I_DIRECTION, fevent_PRINT_LETTER, fevent_TOOLBAR_PRINT,
      FALSE, BAR_EXTRAS },
    { "detailsT", NULL,
      tb_bar_details_entry_fn, tb_bar_details_exit_fn,
      I_DIRECTION, fevent_HOTLIST_ADD, fevent_TOOLBAR_DETAILS,
      FALSE, BAR_EXTRAS },
    {  0 },
    {  0 },
    { "statusWn", "statusW",
      0, 0,
      fevent_MENU, 0, 0,
      TRUE, BAR_CANT },
    { "codecT", NULL,
      0, tb_bar_codec_exit_fn,
      I_DIRECTION, -1, -1,
      FALSE, BAR_STACK },
    { "customT", NULL,
      0, tb_bar_custom_exit_fn,
      I_DIRECTION, fevent_OPEN_FONT_SIZE, fevent_TOOLBAR_CUSTOM,
      FALSE, BAR_EXTRAS }
};

static tb_bar_info *tb_bar_init(int bar_num)
{
    tb_bar_info *tbi;
    char *name;
    wimp_box box, wbox;
    int object_handle;
    tb_button_info *button_list;
    int n_buttons;

    /* check for legal bar number */
    if (bar_num < 0 || bar_num >= sizeof(bar_names)/sizeof(bar_names[0]))
	return NULL;
    
    /* create window */
    if (is_a_tv())	/* check for interlace bit set */
    {
	name = bar_names[bar_num].tv_name;
    }
    else
    {
	name = bar_names[bar_num].monitor_name;
	if (!name)
	    name = bar_names[bar_num].tv_name;
    }

    /* create object */
    object_handle = tb_bar_create(name, tile_sprite, &button_list, &n_buttons);
    
    STBDBG(("tb_bar_init(): bar %d '%s' creates object %x, %d buttons\n", bar_num, name, object_handle, n_buttons));
    
    tbi = NULL;
    if (object_handle)
    {
	tbi = mm_calloc(sizeof(*tbi), 1);

	tbi->num = bar_num;
	tbi->object_handle = object_handle;
	tbi->window_handle = window_handle(object_handle);

	tbi->return_bar = bar_names[bar_num].return_bar;
	tbi->return_component = bar_names[bar_num].return_component;

#if DEBUG
	{
	    wimp_winfo info;
	    info.w = tbi->window_handle;
	    wimp_get_wind_info((wimp_winfo *)((int)&info | 1));

	    STBDBG(("tb_bar_init(): sprite_area %p\n", info.info.spritearea));
	}
#endif
    
	tbi->buttons = button_list;
	tbi->n_buttons = n_buttons;
    
	/* link in to list */
	if (bar_list)
	{
	    tbi->next = bar_list;
	    bar_list->prev = tbi;
	}
	bar_list = tbi;

	/* get window extent */
	_swix(Toolbox_ObjectMiscOp, _INR(0,5), 0, tbi->object_handle, 16, &box);
	tbi->height = box.y1 - box.y0;

	/* move gadgets if necessary */
	if (_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, bar_list->object_handle, 72, I_WORLD_BORDER, &wbox) == NULL ||
	    _swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, bar_list->object_handle, 72, I_WORLD, &wbox) == NULL)
	    tb_status_resize((text_safe_box.x1 - text_safe_box.x0) - wbox.x1, 0);
    
	/* extend one extent up or down to cover safe area */
	if (config_display_control_top)
	    box.y1 = 0x4000;
	else
	    box.y0 = -0x4000;
	
	box.x0 = -0x4000;
	box.x1 = 0x4000;
	_swix(Toolbox_ObjectMiscOp, _INR(0,5), 0, tbi->object_handle, 15, &box);

	/* do a reset to ensure the sprites are set up correctly */
	tb_status_rotate_reset();
    }
    
    return tbi;
}

static void tb_bar_dispose(void)
{
    tb_bar_info *tbi = bar_list;

    STBDBG(("tb_bar_dispose(): top is %p '%s'\n", tbi, tbi ? bar_names[tbi->num].tv_name : ""));

    if (tbi == NULL)
	return;
    
    if (bar_names[tbi->num].exit_fn)
	bar_names[tbi->num].exit_fn();

    /* unlink top item */
    bar_list = tbi->next;

    /* remove object */
    _swix(Toolbox_DeleteObject, _INR(0,1), 0, tbi->object_handle);
    
    mm_free(tbi->buttons);
    mm_free(tbi);
}

/* --------------------------------------------------------------------------*/

BOOL tb_status_unstack_possible(void)
{
    return bar_list && bar_list->return_bar != BAR_CANT;
}

BOOL tb_status_unstack(void)
{
    tb_status_state_t old_state = status_state;
    tb_bar_descriptor *tbd;
    tb_bar_info *tbi;
    int return_bar, return_cmp;
    
    STBDBG(("tb_status_unstack(): in bar_list %p return_bar %d\n", bar_list, bar_list ? bar_list->return_bar : 99));

    /* see if we can do anything */
    if (!bar_list || bar_list->return_bar == BAR_CANT)
	return FALSE;

    sound_event(snd_TOOLBAR_HIDE_SUB);

    /* get return bar */
    return_bar = bar_list->return_bar;
    return_cmp = bar_list->return_component;
    
    /* dispose of current bar */
    tb_bar_dispose();

    /* open new bar */
    tb_status_new(NULL, return_bar);
    if ((tbi = bar_list) != NULL)
    {
	/* set highlight */
	tb_status_show(old_state == status_OPEN_SMALL);

	tb_bar_set_highlight(tbi, return_cmp, FALSE);
	setfocus(tbi->object_handle);
    }
    
    STBDBGN(("tb_status_unstack(): out\n"));

    return TRUE;
}

void tb_status_unstack_all(void)
{
    tb_bar_dispose();
    tb_status_new(NULL, BAR_MAIN);
}

void tb_status_new(fe_view v, int bar_num)
{
    tb_status_state_t old_state = status_state;
    tb_bar_info *tbi;
    int old_bar = bar_list ? bar_list->num : -1;

    STBDBG(("tb_status_new(): in\n"));
    
    /* is this one already open */
    if (old_bar == bar_num)
	return;

    /* dipose of what's there currently */
    tb_bar_dispose();
    
    /* create a new one */
    if ((tbi = tb_bar_init(bar_num)) != NULL)
    {
	tb_bar_descriptor *tbd = &bar_names[tbi->num];

	if (tbd->entry_fn)
	    tbd->entry_fn(v);

	/* record the return point */
	tbi->return_bar = tbd->return_bar == BAR_STACK ? old_bar : tbd->return_bar;
	tbi->return_component = tbd->return_bar == BAR_STACK ? tbd->open_component : tbd->return_component;

	/* set the highlight appropriately */
	tb_status_show(old_state == status_OPEN_SMALL);

	if (tbd->open_component != -1)
	{
	    tb_bar_set_highlight(tbi, tbd->open_component, FALSE);
	    setfocus(tbi->object_handle);
	}

	if (secure_light)
	    setstate(tbi->object_handle, I_SECURE, TRUE);
    }

    sound_event(snd_TOOLBAR_SHOW_SUB);
    
    STBDBG(("tb_status_new(): out\n"));
}

BOOL tb_status_highlight(BOOL gain)
{
    tb_bar_info *tbi = bar_list;

    STBDBG(("tb_status_highlight: bar %p gain %d\n", tbi, gain));

    if (tbi)
    {
	if (gain && !havefocus(tbi))/* get_highlighted(tbi) == -1) */
	{
	    int active = get_active(tbi);

	    STBDBG(("tb_status_highlight: active %d\n", active));

	    /* if a popup is open then move to the selected button */
	    if (active != -1)
		tb_bar_set_highlight(tbi, tbi->buttons[active].cmp, FALSE);
	    else
		tb_bar_set_highlight(tbi, bar_names[tbi->num].initial_component, FALSE);

	    setfocus(tbi->object_handle);
	}
	return TRUE;
    }
    return FALSE;
}

void tb_status_button(int cmp, int active)
{
    tb_bar_info *tbi = bar_list;
    if (tbi)
    {
	switch (active)
	{
	case tb_status_button_INACTIVE:
	    gfade(tbi->object_handle, cmp, FALSE);
	    setstate(tbi->object_handle, cmp, FALSE);
	    break;
	case tb_status_button_ACTIVE:
#if 1
	    gfade(tbi->object_handle, cmp, FALSE);
	    setstate(tbi->object_handle, cmp, TRUE);
#else
	    int i;
	    for (i = 0; i < tbi->n_buttons; i++)
	    {
		int this_cmp = tbi->buttons[i].cmp;
		setstate(tbi->object_handle, this_cmp, cmp == this_cmp);
	    }
#endif
	    break;
	case tb_status_button_FADED:
	    gfade(tbi->object_handle, cmp, TRUE);
	    break;
	case tb_status_button_UNPRESSED:
	    toolactionpress(tbi->object_handle, cmp, FALSE);
	    break;
	case tb_status_button_PRESSED:
	    tb_bar_set_highlight(tbi, tb_bar_cmp_to_index(tbi, cmp), TRUE);
	    toolactionpress(tbi->object_handle, cmp, TRUE);
	    break;
	}
    }
}

void tb_status_box(wimp_box *box)
{
    tb_bar_info *tbi = bar_list;
    if (tbi)
    {
	wimp_wstate state;
	wimp_get_wind_state(tbi->window_handle, &state);
	*box = state.o.box;
    }
    else
	memset(box, 0, sizeof(*box));
}

/* --------------------------------------------------------------------------*/

static char *menu_list[] =
{
    "editM",
    "findM",
    "hotlistM",
    "mainM",
    "navigateM",
    "pageM",
    "viewM"
#if DEBUG
    ,"debugM"
#endif
};

/* returns wimp task handle */

int tb_init(int *m_list, int *wimp_version)
{
    int i, t;
    MemCheck_checking checking = MemCheck_SetChecking(0, 0);

    STBDBG(("tb_init():\n"));
    
    frontend_fatal_error((os_error *)_swix(Toolbox_Initialise, _INR(0,6) | _OUTR(0,1),
					   0, *wimp_version, m_list, tb_list, "<" PROGRAM_NAME "$Dir>",
					   m_block, tb_block,
					   wimp_version, &t));

    /* register the message block with MemCheck for debugging */
    if (m_block[2])
    {
	MemCheck_RegisterMiscBlock((void *)m_block[2], ((int *)m_block[2])[-1]);
	MemCheck_SetBlockWritable((void *)m_block[2], 0);
    }

    /* create the menu tree */
    for (i = 0; i < sizeof(menu_list)/sizeof(menu_list[0]); i++)
        remove_title_bar(menu_list[i]);

    _swix(Toolbox_CreateObject, _INR(0,1) | _OUT(0), 0, "mainM", &menu_object[0]);
#if DEBUG
    _swix(Toolbox_CreateObject, _INR(0,1) | _OUT(0), 0, "debugM", &menu_object[1]);
#endif

    MemCheck_RestoreChecking(checking);

    return t;
}

void tb_cleanup(void)
{
    int i;
    for (i = 0; i < sizeof(status_messages)/sizeof(status_messages[0]); i++)
    {
	mm_free(status_messages[i]);
	status_messages[i] = NULL;
    }
}

/* --------------------------------------------------------------------------*/

int tb_menu_showing(void)
{
    int flags;
    return menu_object[0] &&
	_swix(Toolbox_GetObjectInfo, _INR(0,1) | _OUT(0), 0, menu_object[0], &flags) == NULL &&
        (flags & 1);
}

void tb_menu_hide(void)
{
    if (menu_object[0])
        _swix(Toolbox_HideObject, _INR(0,1), 0, menu_object[0]);
}

void tb_menu_show(fe_view v, int which_menu)
{
    int obj;

    obj = menu_object[which_menu];
    if (obj == NULL)
        return;

    if (tb_menu_showing())
    {
        frontend_complain((os_error *)_swix(Toolbox_HideObject, _INR(0,1), 0, obj));
    }
    else
    {
        os_coord coord;
        coord.x = text_safe_box.x0;

	if (config_display_control_top)
	{
	    coord.y = text_safe_box.y1 - (bar_list ? bar_list->height : 0);
	}
	else
	{
	    coord.y = text_safe_box.y0;
	    if (bar_list)
	    {
		int height;
		_swix(Toolbox_ObjectMiscOp, _INR(0,2) | _OUT(0), 0, bar_list->object_handle, 0x16, &height);
		coord.y += bar_list->height + height;
	    }
	}

        frontend_complain((os_error *)_swix(Toolbox_ShowObject, _INR(0,5), 1, obj, 2, &coord, 0, -1)); /*tb_block[4], tb_block[5]));*/
        frontend_complain((os_error *)_swix(Toolbox_SetClientHandle, _INR(0,2), 0, obj, v));
    }
}

#define PLUGIN_TYPE_PREFIX "Alias$@PlugInType_"
#define PLUGIN_NAME_PREFIX "PlugIn$Type_"

#define FILETYPE_NAME_PREFIX "File$Type_"
	    
#define error_MENU_NO_SUCH_COMPONENT            0x80AA14u

static void tb_menu_do_plugins(int obj)
{
    char *runtype = NULL;
    os_error *e;
    char buffer[256];
    
    do
    {
	e = (os_error *)_swix(OS_ReadVarVal, _INR(0,4) | _OUT(3),
		   PLUGIN_TYPE_PREFIX "*",
		   buffer, sizeof(buffer),
		   runtype,
		   0,
		   &runtype);

	STBDBG(("tb plugin: readvarval returns runtype '%s' e %x '%s'\n",
		strsafe(runtype), e ? e->errnum : 0, e ? e->errmess : ""));

	if (!e && runtype)
	{
	    int filetype = (int)strtoul(runtype + sizeof(PLUGIN_TYPE_PREFIX)-1, NULL, 16);
	    plugin_info *info = plugin_list_get_info(filetype);
	    int tick_state = info ? (info->flags & plugin_info_flag_DISABLED) == 0 : TRUE;

	    /* Try and set tick option */
	    e = mtick(obj, fevent_PLUGIN_TOGGLE + filetype, tick_state);
	    
	    /*, if it fails then add the component to the menu */
	    STBDBG(("tb plugin: filetype %03x info %p flags %x tick returns %x '%s'\n",
		    filetype, info, tick_state, e ? e->errnum : 0, e ? e->errmess : ""));

	    if (e && (e->errnum &~ 0xff000000) == error_MENU_NO_SUCH_COMPONENT)
	    {
		char nameval[32];
		char *name;
		strcpy(nameval, PLUGIN_NAME_PREFIX);
		strcat(nameval, runtype + sizeof(PLUGIN_TYPE_PREFIX)-1);
	    
		name = getenv(nameval);

		STBDBG(("tb plugin: nameval '%s' name '%s'\n", nameval, strsafe(name)));

		if (!name)
		{
		    strcpy(nameval, FILETYPE_NAME_PREFIX);
		    strcat(nameval, runtype + sizeof(PLUGIN_TYPE_PREFIX)-1);
		    name = getenv(nameval);
		    STBDBG(("tb plugin: nameval '%s' name '%s'\n", nameval, strsafe(name)));
		}
		
		if (!name)
		{
		    sprintf(nameval, "PlugIn%03x", filetype);
		    name = nameval;
		}

		{
		    struct menu_entry_object descr;
		
		    descr.flags = tick_state ? 1 : 0;
		    descr.cmp = fevent_PLUGIN_TOGGLE + filetype;
		    descr.text = strdup(name);
		    descr.text_limit = strlen(descr.text) + 1;
		    descr.click_object_name = 0;
		    descr.sub_menu_object_name = 0;
		    descr.sub_menu_action = 0;
		    descr.click_action = fevent_PLUGIN_TOGGLE + filetype;
		    descr.help = msgs_lookup("hpm");
		    descr.help_limit = strlen(descr.help) + 1;

		    e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, obj, 20, -2, &descr);

		    mm_free(descr.text);
		}
	    }
	    
	    /* if not in list then add it */
	    if (info == NULL)
	    {
		plugin_info newinfo;

		newinfo.flags = 0;
		newinfo.file_type = filetype;

		plugin_list_add(&newinfo);
	    }
	}
    }
    while (!e && runtype);
}

void tb_menu_help(void)
{
    int obj = menu_object[0], this_obj;
    char msg[256];
    int *bp = (int *)msg;	/* reuse msg block to save space */

    if (obj == NULL)
        return;

    _swix(Wimp_GetMenuState, _INR(0,1), 0, bp);
    if (bp[0] == -1)
	return;

    /* descend menu tree until we reach last entry */
    this_obj = obj;
    while (bp[1] != -1)
    {
	_swix(Toolbox_ObjectMiscOp, _INR(0,3)|_OUT(0), 0, this_obj, 9, bp[0], &this_obj);
	bp++;
    }

    /* get help message for this entry */
    if (_swix(Toolbox_ObjectMiscOp, _INR(0,5), 0, this_obj, 19, bp[0], msg, sizeof(msg)) == NULL)
	tb_status_set_message(status_type_HELP, msg);
}


/* --------------------------------------------------------------------------*/

/*
 * The menu setup functions all take an fe_view to get their context from
 */

static void mshow_file(fe_view v, int obj)
{
    int have_printer = fe_print_possible(v);
    mfade(obj, 0 /* fevent_PRINT */, !have_printer);
    mfade(obj, 2 /* fevent_STOP_LOADING */, !fe_abort_fetch_possible(v));
    mfade(obj, 3 /* fevent_RELOAD */, !fe_reload_possible(v));
}

static void mshow_edit(fe_view v, int obj)
{
    mfade(obj, 0 /* fevent_COPY_TEXT */, !fe_copy_text_possible(v));
#if 0
    mfade(obj, 1 /* fevent_PASTE */, !fe_paste_possible(v));
#endif
}

static void mshow_view(fe_view v, int obj)
{
#if 0
    mtick(obj, 0 /* fevent_TOGGLE_STATUS */, fe_status_open(v));
#endif
    mtick(obj, 0 /* fevent_TOGGLE_COLOURS */, fe_doc_flag_state(v, be_openurl_flag_BODY_COLOURS));
    mtick(obj, 1 /* fevent_TOGGLE_IMAGES */, !fe_doc_flag_state(v, be_openurl_flag_DEFER_IMAGES));

    tb_menu_do_plugins(obj);
}

static void mshow_navigate(fe_view v, int obj)
{
    mfade(obj, 2 /* fevent_HISTORY_BACK */, !fe_history_possible(v, history_PREV));
    mfade(obj, 3 /* fevent_HISTORY_FORWARD */, !fe_history_possible(v, history_NEXT));
}

static void mshow_hotlist(fe_view v, int obj)
{
}

static void mshow_tools(fe_view v, int obj)
{
#if 0
    int find_poss = fe_find_possible(v);
    mfade(obj, /* fevent_OPEN_FIND */, !find_poss);
    mfade(obj, /* fevent_FIND_AGAIN */, !find_poss);
#endif
}

static void mshow_encoding(fe_view v, int obj)
{
    int encoding = fe_encoding(v, be_encoding_READ);
    int i;

/*     STBDBG(("mshow_encoding: v %p encoding %d\n", v, encoding)); */

    for (i = 0; i < 4; i++)
    {
	int event;
	_swix(Toolbox_ObjectMiscOp, _INR(0,3) | _OUT(0), 0, obj, Menu_GetClickEvent, i, &event);
	 mtick(obj, i, (event & fevent_ENCODING_MASK) == encoding);
    }
}

#if 0
static void mshow_mouse(fe_view v, int obj)
{
/*    mfade(obj, fevent_OPEN_PRINT_OPTIONS, fe_print_possible(v));*/
    mfade(obj, fevent_COPY_TEXT, !fe_copy_text_possible(v));
    mfade(obj, fevent_COPY_IMAGE, !fe_copy_image_possible(v));
    mfade(obj, fevent_OPEN_FIND, !fe_find_possible(v));
}

static void mshow_ir(fe_view v, int obj)
{
    int have_printer = fe_print_possible(v);
    mfade(obj, fevent_PRINT, !have_printer);
/*    mfade(obj, fevent_OPEN_PRINT_OPTIONS, !have_printer);*/
    mfade(obj, fevent_COPY_TEXT, !fe_copy_text_possible(v));
    mfade(obj, fevent_COPY_IMAGE, !fe_copy_image_possible(v));
    mfade(obj, fevent_OPEN_FIND, !fe_find_possible(v));
    mfade(obj, fevent_FIND_AGAIN, !fe_find_again_possible(v));
    mfade(obj, fevent_RELOAD, !fe_reload_possible(v));
}

#endif

static void mshow__handler(int event, fe_view v, int obj)
{
    switch (event)
    {
        case 0xf00:     /* Main menu about to be shown*/
            break;
        case 0xf01:     /* File menu about to be shown*/
            mshow_file(v, obj);
            break;
        case 0xf02:     /* Edit menu about to be shown*/
            mshow_edit(v, obj);
            break;
        case 0xf03:     /* View menu about to be shown*/
            mshow_view(v, obj);
            break;
        case 0xf04:     /* Navigate menu about to be shown*/
            mshow_navigate(v, obj);
            break;
        case 0xf05:     /* Hotlist menu about to be shown*/
            mshow_hotlist(v, obj);
            break;
        case 0xf06:     /* Tools menu about to be shown*/
            mshow_tools(v, obj);
            break;
        case 0xf07:     /* Encoding menu about to be shown*/
            mshow_encoding(v, obj);
            break;
#if 0
        case 0xf07:     /* irM about to be shown*/
            mshow_ir(v, obj);
            break;
        case 0xf08:     /* mouseM about to be shown*/
            mshow_mouse(v, obj);
            break;
#endif
    }
}

static void mshow_handler(int event, int *tb_block)
{
    fe_view v;
    frontend_complain((os_error *)_swix(Toolbox_GetClientHandle, _INR(0,1) | _OUT(0), 0, tb_block[4], &v)); /* ancestor*/
    if (!v && tb_block[0])
        frontend_complain((os_error *)_swix(Toolbox_GetClientHandle, _INR(0,1) | _OUT(0), 0, tb_block[0], &v)); /* ancestor*/
    mshow__handler(event, v, tb_block[4]);
}

/* --------------------------------------------------------------------------*/

void tb_menu_refresh(fe_view v)
{
    int obj = menu_object[0], this_obj;
    char msg[256];
    int *bp = (int *)msg;	/* reuse msg block to save space */

    if (obj == NULL)
        return;

    _swix(Wimp_GetMenuState, _INR(0,1), 0, bp);
    if (bp[0] == -1)
	return;

    /* descend menu tree until we reach last entry */
    this_obj = obj;
    while (bp[1] != -1)
    {
	_swix(Toolbox_ObjectMiscOp, _INR(0,3)|_OUT(0), 0, this_obj, 9, bp[0], &this_obj);
	bp++;
    }

    /* refresh this item, by reshowing it */
    _swix(Toolbox_ShowObject, _INR(0,5), 2, this_obj, 0, NULL, 0, -1);
}

/* --------------------------------------------------------------------------*/

/*
 * The status update function takes an fe_view to get its context from.
 */

void tb_status_update_fades(fe_view v)
{
    if (v && bar_list && bar_names[bar_list->num].can_grey)
    {
	int obj = bar_list->object_handle;
	gfade(obj, fevent_HISTORY_BACK, !fe_history_possible(v, history_PREV));
	gfade(obj, fevent_HISTORY_FORWARD, !fe_history_possible(v, history_NEXT));
	gfade(obj, fevent_RELOAD, !fe_reload_possible(v));
	gfade(obj, fevent_HOTLIST_ADD, !fe_hotlist_add_possible(v));
	gfade(obj, fevent_PRINT, !fe_print_possible(v));
	gfade(obj, fevent_STOP_LOADING, !fe_abort_fetch_possible(v));

	gfade(obj, fevent_TOOLBAR_EXIT, !fe_status_unstack_possible(v));
    }
}

void tb_status_show(int small_only)
{
    wimp_openstr o;
    int open = FALSE;

    if (!small_only)
    {
        o.x = - margin_box.x0;

        o.box.x0 = 0;
        o.box.x1 = screen_box.x1;

	if (config_display_control_top)
	{
	    o.box.y0 = text_safe_box.y1 - bar_list->height;
	    o.box.y1 = screen_box.y1;

	    o.y = - margin_box.y1;
	}
	else
	{
	    o.box.y0 = screen_box.y0;
	    o.box.y1 = text_safe_box.y0 + bar_list->height;

	    o.y = 0;
	}

        open = TRUE;
        status_state = status_OPEN;

	if (status_state != status_OPEN)
	    sound_event(snd_TOOLBAR_SHOW);
    }
    else
    {
        if (status_state != status_OPEN)
        {
	    if (_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, bar_list->object_handle, 72, I_WORLD_BORDER, &o.box) == NULL ||
		_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, bar_list->object_handle, 72, I_WORLD, &o.box) == NULL)
	    {
		wimp_box lbox;
		if (_swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, bar_list->object_handle, 72, I_LIGHTS_GREEN, &lbox) != NULL)
		    lbox = o.box;
	    
		o.box.x0 += text_safe_box.x0;
		o.box.x1 += text_safe_box.x0;

		o.x = - text_safe_box.x0 + o.box.x0;

		if (config_display_control_top)
		{
		    o.box.y0  = lbox.y0 + text_safe_box.y1;
		    o.box.y1 += text_safe_box.y1;

		    o.y = - text_safe_box.y1 + o.box.y1;
		}
		else
		{
		    o.box.y0 += text_safe_box.y0 + bar_list->height;
		    o.box.y1 += text_safe_box.y0 + bar_list->height;

		    o.y = lbox.y1;
		}

		open = TRUE;
		status_state = status_OPEN_SMALL;
	    }
	}
    }

    STBDBG(("tb_status_show open %d state %d at %d,%d,%d,%d %d,%d\n", open, status_state, o.box.x0, o.box.y0, o.box.x1, o.box.y1, o.x, o.y));

    if (open)
    {
        o.behind = (wimp_w)-1;
        frontend_complain((os_error *)_swix(Toolbox_ShowObject, _INR(0,5), 0, bar_list->object_handle, 1, &o.box, 0, 0));/* tb_block[4], tb_block[5]));*/
    }
}

void tb_status_hide(int only_if_small)
{
    STBDBG(("tb_status_hide(): only_if_small %d state %d\n", only_if_small, status_state));

    if ((status_state != status_CLOSED && !only_if_small) || status_state == status_OPEN_SMALL)
    {
	BOOL focus = havefocus(bar_list);
	
	sound_event(snd_TOOLBAR_HIDE);

        frontend_complain((os_error *)_swix(Toolbox_HideObject, _INR(0,1), 0, bar_list->object_handle));
        status_state = status_CLOSED;

	if (focus)
	    return_highlight(NULL, bar_list, 0);

	/* if we are closing big toolbar and still spinning then reopen ickle status bar */
	if (turn_ctr != -1 && !only_if_small)
	    tb_status_show(TRUE);
    }
}

int tb_is_status_showing(void)
{
    return status_state == status_OPEN;
}

int tb_status_height(void)
{
    return bar_list ? bar_list->height : 0;
}

int tb_status_w(void)
{
    return bar_list ? window_handle(bar_list->object_handle) : 0;
}

void tb_status_refresh_if_small(void)
{
    if (status_state == status_OPEN_SMALL)
    {
	wimp_box box;
	box.x0 = box.y0 = -0x4000;
	box.x1 = box.y1 =  0x4000;
	_swix(Toolbox_ObjectMiscOp, _INR(0,3), 0, bar_list->object_handle, 0x11, &box);
    }
}

#if 0
os_error *tb_status_set_url(const char *url)
{
    char buf[256];

    strcpy(buf, msgs_lookup("url"));
    strlencat(buf, (char *)url, sizeof(buf));

    /* set help on status and background to be the URL*/
    sethelp(bar_list->object_handle, I_STATUS, buf);
    sethelp(bar_list->object_handle, -1, buf);

/*    return setfield(bar_list->object_handle, I_URL, url, FALSE);*/
    return NULL;
}
#endif

os_error *tb_status_set_messagef(int type, const char *msg, ...)
{
    char buffer[256];
    va_list ap;

    va_start(ap, msg);
    vsprintf(buffer, msg, ap);
    va_end(ap);

    return tb_status_set_message(type, msg);
}

static os_error *tb_status_force_redraw(void)
{
    _kernel_oserror *e = NULL;
    if (bar_list && config_display_control_status)
    {
	wimp_box box;
	e = _swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, bar_list->object_handle, 72, I_STATUS, &box);
	if (!e) e = _swix(Toolbox_ObjectMiscOp, _INR(0,3), 0, bar_list->object_handle, 0x11, &box);
    }
    return (os_error *)e;
}

static int which_message_to_use(int new_type)
{
    char *msg;
    do
    {
	msg = status_messages[new_type];
	if (msg == NULL && new_type > 0)
	    new_type--;
    }
    while (new_type >= 0 && msg == NULL);
    return new_type;
}

os_error *tb_status_set_message(int type, const char *msg)
{
    os_error *e = NULL;
    char *s = status_messages[type];

    if (strcmp(strsafe(msg), strsafe(s)) != 0)
    {
	if (msg)
	    status_messages[type] = s = mm_realloc(s, msg ? strlen(msg)+1 : 0);
	else
	{
	    mm_free(s);
	    status_messages[type] = s = NULL;
	}

        if (s && msg)
            strcpy(s, msg);

	status_changed[type] = TRUE;
    }

    type = which_message_to_use(type);
    if (type != status_current_type || status_changed[type])
    {
        status_current_type = type;
        e = tb_status_force_redraw();

	STBDBGN(("stbtb: msg %d '%s'\n", type, strsafe(s)));
    }

    return e;
}

int tb_status_check_message(wimp_mousestr *mp)
{
    int obj;
    int cmp;
    int new_type = -1;

    _swix(Window_WimpToToolbox, _INR(0,2)|_OUTR(0,1), 0, mp->w, mp->i, &obj, &cmp);

    if (obj != bar_list->object_handle)
        return FALSE;

    switch (cmp)
    {
        case I_STATUS:
            new_type = status_messages[status_type_URL] != NULL && status_messages[status_type_URL][0] ?
                status_type_URL : status_type_TITLE;
            break;

        case -1:		/* background */
            new_type = status_type_TITLE;
            break;

        case I_WORLD:
            new_type = status_type_FETCH;
            break;

        default:		/* for anything else then use the wimp message */
            return FALSE;
#if 0
        {
	    wimp_msgstr msg;
            _kernel_oserror *e;
            char buf[256];
            buf[0] = 0;
            e = _swix(Gadget_GetHelpMessage, _INR(0,5), 0, obj, 67, cmp, buf, sizeof(buf));
            tb_status_set_message(status_type_HELP, e ? e->errmess : buf);
            break;
        }
#endif
    }


    STBDBG(("stbtb: new type %d old %d new changed %d\n", new_type, status_current_type, status_changed[new_type]));

    if (new_type != status_current_type)
    {
        status_current_type = new_type;
        tb_status_force_redraw();
    }
    else if (status_changed[status_current_type])
    {
        tb_status_force_redraw();
    }

    return TRUE;
}

void tb_status_rotate(void)
{
    if (bar_list)
    {
	char sprite_name1[40];

	if (turn_ctr == -1)
	{
	    turn_start = alarm_timenow();
	    sprintf(sprite_name1, "%st,%sts", config_animation_name, config_animation_name);
	    setfield(bar_list->object_handle, I_WORLD_BORDER, sprite_name1, FALSE);
	}
	
	turn_ctr = ((alarm_timenow() - turn_start) * TURN_SPEED / 100) % config_animation_frames;
	
	sprintf(sprite_name1, "%s%02d", config_animation_name, turn_ctr);
	setfield(bar_list->object_handle, I_WORLD, sprite_name1, FALSE);
    }
}


void tb_status_rotate_reset(void)
{
    turn_ctr = -1;
    if (bar_list)
    {
	char sprite_name1[40];
	sprintf(sprite_name1, "%s00", config_animation_name);
	setfield(bar_list->object_handle, I_WORLD, sprite_name1, FALSE);

	sprintf(sprite_name1, "%s,%ss", config_animation_name, config_animation_name);
	setfield(bar_list->object_handle, I_WORLD_BORDER, sprite_name1, FALSE);
    }
}

/*
 * For a neater display will wish to
 *  Move the world to the right
 *  Resize the URL field
 *  Resize the status field
 *  Move the print and stop icons to the right
 */

void tb_status_resize(int xdiff, int ydiff)
{
    if (bar_list && (xdiff || ydiff))
    {
        movegadget(bar_list->object_handle, I_WORLD, xdiff, xdiff);
        movegadget(bar_list->object_handle, I_WORLD_BORDER, xdiff, xdiff);
        movegadget(bar_list->object_handle, I_STATUS, 0, xdiff);
        movegadget(bar_list->object_handle, I_LIGHTS_GREEN, xdiff, xdiff);
        movegadget(bar_list->object_handle, I_LIGHTS_YELLOW, xdiff, xdiff);
        movegadget(bar_list->object_handle, I_LIGHTS_RED, xdiff, xdiff);
/*         movegadget(bar_list->object_handle, I_SECURE, xdiff, xdiff); */
    }
}

#define STATUS_LINE_FG      0xDEFFD100

int tb_status_redraw(wimp_redrawstr *r)
{
    STBDBGN(("stbtb: redraw %x status %x\n", r->w, tb_status_w()));

    if (config_display_control_status && r->w == tb_status_w())
    {
        wimp_box box, overlap;
        char *msg;

	STBDBG(("stbtb: redraw type %d\n", status_current_type));

        /* get the status message to display */
	msg = status_messages[status_current_type];

	/* could have no message */
        if (msg == NULL)
            return TRUE;

	status_changed[status_current_type] = FALSE;

        /* get status bounding box */
        _swix(Toolbox_ObjectMiscOp, _INR(0,4), 0, bar_list->object_handle, 72, I_STATUS, &box);
        coords_box_toscreen(&box, (coords_cvtstr *)&r->box);

        STBDBG(("stbtb: redraw g %d,%d,%d,%d box %d,%d,%d,%d\n",
            r->g.x0, r->g.y0, r->g.x1, r->g.y1,
            box.x0, box.y0, box.x1, box.y1));

	/* if need to redraw */
        if (coords_intersection(&r->g, &box, &overlap))
        {
            int status_line_font;
            char *prefix = status_prefixes[status_current_type];
            if (prefix) prefix = msgs_lookup(prefix);

	    STBDBG(("prefix '%s'\n", strsafe(prefix)));
            /* get font handle */
            if (_swix(Wimp_ReadSysInfo, _IN(0)|_OUT(0), 8, &status_line_font))
                status_line_font = 0;

            /* set graphics window to status box */
            bbc_gwindow(overlap.x0, overlap.y0, overlap.x1-frontend_dx, overlap.y1-frontend_dy);

            if (status_line_font)
            {
                int x0, y0, width = 0;

                _swix(ColourTrans_SetFontColours, _INR(0,3), status_line_font, 0, STATUS_LINE_FG, 14);

                _swix(Font_Converttopoints, _INR(1,2) | _OUTR(1,2), box.x0, box.y0, &x0, &y0);

		/* paint the prefix,  if any */
                if (prefix)
                {
                    _swix(Font_Paint, _INR(0,4), status_line_font, prefix,
                        (config_display_blending ? 0x800: 0) | (1<<8) | (1<<9),
                        x0, y0 + 4000);

                    _swix(Font_ScanString, _INR(0,2) | _OUT(3), status_line_font, prefix,
                        (1<<8) | (1<<9), &width);
                }

                _swix(Font_Paint, _INR(1,4), msg,
                    (config_display_blending ? 0x800: 0) | (1<<9),
                    x0 + width, y0 + 4000);
            }
            else
            {
                bbc_move(box.x0, box.y1 - 8);
                if (prefix)
                {
                    _swix(OS_Write0, _IN(0), prefix);
                    bbc_moveby(strlen(prefix)*8*2, 0);
                }
                _swix(OS_Write0, _IN(0), msg);
            }

            /* anti twitter */
            fe_anti_twitter(&overlap);

            /* reset graphics window */
            bbc_gwindow(r->g.x0, r->g.y0, r->g.x1-frontend_dx, r->g.y1-frontend_dy);
        }

        return TRUE;
    }
    return FALSE;
}

void tb_status_set_secure(int on)
{
    if (bar_list)
	setstate(bar_list->object_handle, I_SECURE, on);
    secure_light = on;
}

static int light_state = light_state_OFF;

static void set_lights_off(int called_at, void *handle)
{
    if (light_state == light_state_FINISHED)
	tb_status_set_lights(light_state_OFF);

    called_at = called_at;
    handle = handle;
}

void tb_status_set_lights(int state)
{
    static int components[] =
    {
        -1,
        I_LIGHTS_GREEN,
        I_LIGHTS_YELLOW,
        I_LIGHTS_RED
    };

    if (bar_list && state != light_state)
    {
        setstate(bar_list->object_handle, components[light_state], 0);
        setstate(bar_list->object_handle, components[state], 1);

        light_state = state;

        if (state == light_state_FINISHED)
            alarm_set(alarm_timenow() + LIGHT_OFF_DELAY, set_lights_off, NULL);
    }
}

void tb_status_init(void)
{
    if (config_display_control_initial == BAR_STATUS)
	tile_sprite = sprite_load_tile(is_a_tv() ? "N" : "V");

    STBDBG(("tb_init():tile sprite %p\n", tile_sprite));

    tb_bar_init(config_display_control_initial);
}

/* --------------------------------------------------------------------------*/

/* open the contents of the URL writable field*/
/* open in our default top window*/

void tb_open_url(void)
{
    if (bar_list)
    {
	char *url, *url1;

	url = getwriteable(bar_list->object_handle, I_URL);

	url1 = check_url_prefix(url);
	mm_free(url);

	frontend_complain(frontend_open_url(url1, NULL, TARGET_VERY_TOP, NULL, 0));

	mm_free(url1);
    }
}

void tb_open_url_and_close(void)
{
    tb_open_url();
}

/* --------------------------------------------------------------------------*/


static int movehighlight(tb_bar_info *tbi, int direction)
{
    int initial, next;

    if (tbi == NULL)
	return FALSE;

    initial = next = get_highlighted(tbi);

    do
    {
	next += direction;
    }
    while (next >= 0 && next < tbi->n_buttons && !selectable(tbi->object_handle, tbi->buttons[next].cmp));
    
    if (next < 0)
	next = 0;
    if (next > tbi->n_buttons-1)
	next = tbi->n_buttons-1;
    
    STBDBG(("movehighlight(): dir %d from %d to %d\n", direction, initial, next));

    if (initial == next)
	return FALSE;
    
    tb_bar_set_highlight(tbi, next, TRUE);
    return TRUE;
}

void tb_events(int *event, fe_view v)
{
    toolbox_action *e = (toolbox_action *)event;

    switch (e->action_no)
    {
        case 0x44EC0:   /* toolbox error*/
            frontend_complain((os_error *)e->data.bytes);
            break;

        default:
            if ((e->action_no & 0xff0) == 0xf00)
                mshow_handler(e->action_no, tb_block);
            else if (e->action_no < 0x10000)
                fevent_handler(e->action_no, v);
            break;
    }
}

void tb_event_handler(int event, fe_view v)
{
    tb_bar_info *tbi = bar_list;
    int claimed = FALSE;

    STBDBG(("tb_event_handler(): v %p event %d\n", v, event));
    
    if (tbi)
    {
/* 	fe_view v = fe_selected_view(); */
/* 	if (!v) */
/* 	    v = main_view; */

	claimed = TRUE;

/* 	fe_pointer_mode_update(pointermode_OFF); */

	switch (event)
	{
	case fevent_TOOLBAR_MOVE_LEFT:
	    /* move to previous icon */
	    if (!movehighlight(tbi, -1))
		sound_event(snd_WARN_BAD_KEY);
	    break;

	case fevent_TOOLBAR_MOVE_RIGHT:
	    /* move to next icon */
	    if (!movehighlight(tbi, +1))
		sound_event(snd_WARN_BAD_KEY);
	    break;

	case fevent_TOOLBAR_PAGE_UP:
	    pointer_mode = pointermode_ON;
	    fevent_handler(fevent_SCROLL_PAGE_UP, v);
	    pointer_mode = pointermode_OFF;
	    break;

	case fevent_TOOLBAR_PAGE_DOWN:
	    pointer_mode = pointermode_ON;
	    fevent_handler(fevent_SCROLL_PAGE_DOWN, v);
	    pointer_mode = pointermode_OFF;
	    break;

	case fevent_TOOLBAR_MOVE_UP:
	    /* transfer focus back to main window */
	    if (config_display_control_top || !return_highlight(v, tbi, be_link_VERT | be_link_BACK))
	    {
		pointer_mode = pointermode_ON;
		fevent_handler(fevent_SCROLL_UP, v);
		pointer_mode = pointermode_OFF;
	    }
	    break;

	case fevent_TOOLBAR_MOVE_DOWN:
	    /* transfer focus back to main window */
	    if (!config_display_control_top || !return_highlight(v, tbi, be_link_VERT))
	    {
		pointer_mode = pointermode_ON;
		fevent_handler(fevent_SCROLL_DOWN, v);
		pointer_mode = pointermode_OFF;
	    }
	    break;

	case fevent_TOOLBAR_ACTIVATE:
	{
	    int cmp = tbi->buttons[get_highlighted(tbi)].cmp;
	    int action;
	    os_error *e = (os_error *)_swix(Toolbox_ObjectMiscOp, _INR(0,3) | _OUT(0), 0, tbi->object_handle, 0x140143, cmp, &action);

	    STBDBG(("tb_event_handler(): err '%s' action %x\n", e ? e->errmess : "", action));

	    if (!e)
		fevent_handler(action, v);
	    break;
	}
	}
    }
}

/* --------------------------------------------------------------------------*/

BOOL tb_status_check_pointer(wimp_mousestr *mp)
{
#if 0
    tb_bar_info *tbi = bar_list;

    STBDBG(("tb_status_check_pointer(): w %x i %d\n", mp->w, mp->i));

    if (tbi && tbi->window_handle == mp->w)
    {
	int cmp;

	if (_swix(Window_WimpToToolbox, _INR(0,2) | _OUT(1), 0, mp->w, mp->i, &cmp) == NULL)
	    tbi->highlight = tb_bar_cmp_to_index(tbi, cmp);

	return TRUE;
    }
#endif    
    return  FALSE;
}

/* --------------------------------------------------------------------------*/

void tb_status_set_direction(int up)
{
    if (bar_list)
    {
	setstate(bar_list->object_handle, I_DIRECTION, up ? 1 : 0);
    }
}

/* --------------------------------------------------------------------------*/

static int codec_component[] =
{
    fevent_CODEC_STOP,
    fevent_CODEC_PLAY,
    fevent_CODEC_PAUSE,
    fevent_CODEC_REWIND,
    fevent_CODEC_FAST_FORWARD,
    fevent_CODEC_RECORD
};

void tb_codec_state_change(int state, BOOL opening, BOOL closing)
{
    if (opening && (bar_list == NULL || bar_list->num != BAR_CODEC))
	tb_status_new(NULL, BAR_CODEC);

    if (bar_list && bar_list->num == BAR_CODEC)
    {
	int i;
	
	for (i = 0; i < sizeof(codec_component)/sizeof(codec_component[0]); i++)
	    setstate(bar_list->object_handle, codec_component[i], state == i);

	if (closing)
	    tb_status_unstack();
    }
}

/* --------------------------------------------------------------------------*/

static void *tb_resspr_sprite_area = NULL;

void *tb_resspr_area(void)
{
    return tb_resspr_sprite_area;
}

void tb_resspr_init(void)
{
    (os_error *)_swix(Toolbox_GetSysInfo, _IN(0)|_OUT(0), 4, &tb_resspr_sprite_area);
}

/* --------------------------------------------------------------------------*/

void tb_msgs_init(void)
{
}

char *tb_msgs_lookup(char *tag)
{
    char *out;

    if ((os_error *)_swix(MessageTrans_Lookup, _INR(0,7) | _OUT(2),
        m_block, tag, NULL, 0,
        NULL, NULL, NULL, NULL,
        &out))
        return tag;

    return out;
}

/* --------------------------------------------------------------------------*/

void tb_res_init(char *program_name)
{
    program_name = program_name;
}

/* --------------------------------------------------------------------------*/
