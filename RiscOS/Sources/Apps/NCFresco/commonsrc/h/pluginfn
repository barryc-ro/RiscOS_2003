/* pluginfn.h */

#ifndef __pluginfn_h
# define __pluginfn_h

/* ----------------------------------------------------------------------------- */

typedef struct plugin_private *plugin;

extern plugin plugin_new(struct rid_object_item *obj, be_doc doc);
extern void plugin_destroy(plugin pp);
extern int plugin_send_open(plugin pp, wimp_box *box);
extern int plugin_send_reshape(plugin pp, wimp_box *box);

extern int plugin_message_handler(wimp_eventstr *e, void *handle);

/* ----------------------------------------------------------------------------- */

#endif

/* eof plugin.h */
