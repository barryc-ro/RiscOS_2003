/* history.c */

/* History list for Fresco
 */

#ifndef fresco_history_h
#define fresco_history_h

typedef struct {
    int nSize;                  /* configured history length */
    int nContents;              /* number of entries currently in list */
    int nAt;                    /* where current view is */
    int nStringSize;
    char *pEntries;             /* flex anchor */
    unsigned int hash[1];       /* or more */
} historyliststr;

typedef historyliststr *historylist;

#define	history_FIRST	0
#define	history_PREV	1
#define	history_NEXT	2
#define	history_LAST	3

historylist history_new( int nEntries );
historylist history_clone( historylist hl );
void history_add( historylist hl, const char *url, const char *title );
void history_visit( historylist hl, const char *url, const char *title );
BOOL history_test( historylist hl, const char *url );
void history_destroy( historylist *phl );

os_error *history_make_menu( historylist hl, fe_view v, be_item ti, int ctx);
void      history_kill_menu( void );
os_error *history_hit_menu(historylist hl, fe_view v, int param);
os_error *history_move(historylist hl, int param, fe_view v);

/*
void history_dispose(fe_view v);
void history_truncate(fe_view v);
void history_add(fe_view v, char *url, char *title);
void history_add_global(char *url, char *title);
void history_visit(fe_view v, char *url, char *title);
os_error *history_clone(fe_view old, fe_view new);
*/

#endif
