/* -*-c-*- */

/* femenu.h */

#define menustate_CLOSED    0
#define menustate_PENDING   1
#define menustate_OPEN      2

struct _frontend_menu_handle {
    wimp_w wh;
    int n;
    int size;
    int width;
    int flags;
    fe_menu_item *items;
    be_menu_callback cb;
    void *h;
    BOOL vscroll;		/* does this menu have a scroll bar */
        
    int scroll_y;
    wimp_box visible;   /* screen box */
    int state;          /* closed, pending, open */
    int highlight;      /* item number highlighted currently */

    fe_view parent;
    int line_space;
};

extern BOOL stbmenu_check_mouse(wimp_mousestr *mp);
/*extern BOOL stbmenu_check_key(wimp_caretstr *c, int chcode); */
extern BOOL stbmenu_check_redraw(wimp_w w);
extern BOOL stbmenu_is_open(void);
extern void stbmenu_event_handler(int event);
extern void stbmenu_close(void);
extern BOOL stbmenu_check_pointer(wimp_mousestr *mp);

/* eof stbmenu.h */

