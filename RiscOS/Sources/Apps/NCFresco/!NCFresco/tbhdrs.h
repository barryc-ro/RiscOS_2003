/* > tbhdrs.c

 *

 */

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

/* pdh: one-man crusade to make toolbox code look more sensible starts here */
#define toolactionflag_HASFADESPRITE 0x00000100
#define gadgetflag_FADED             0x80000000

/* eof tbhdrs.c */

