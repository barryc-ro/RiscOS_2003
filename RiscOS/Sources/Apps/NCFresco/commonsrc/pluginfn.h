/* pluginfn.h */

#ifndef __pluginfn_h
# define __pluginfn_h

/* ----------------------------------------------------------------------------- */

typedef struct plugin_private *plugin;

extern plugin plugin_new(struct rid_object_item *obj, be_doc doc, be_item ti);
extern void plugin_destroy(plugin pp);
extern int plugin_send_open(plugin pp, wimp_box *box, int open_flags);
extern int plugin_send_close(plugin pp);
extern int plugin_send_reshape(plugin pp, wimp_box *box);
extern int plugin_send_focus(plugin pp);
extern int plugin_send_action(plugin pp, int action);
extern int plugin_send_abort(plugin pp);

extern void plugin_get_info(plugin pp, int *flags, int *state);
extern plugin plugin_helper(const char *url, int ftype, const char *mime_type, void *parent, const char *cfile);

extern int plugin_message_handler(wimp_eventstr *e, void *handle);
extern BOOL plugin_is_there_enough_memory(int ft, int size);
extern void plugin_dump(void);


/* ----------------------------------------------------------------------------- */

#endif

/* eof plugin.h */
