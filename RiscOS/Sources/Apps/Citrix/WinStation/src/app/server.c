/* > app/server.c
 *
 */

#include "windows.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "tboxlibs/toolbox.h"
#include "tboxlibs/menu.h"

#include "../inc/client.h"
#include "../inc/debug.h"

#include "utils.h"
#include "tbres.h"
#include "server.h"

/* -------------------------------------------------------------------------------- */

static char *server_section = NULL;
static const char **server_list = NULL;
static int server_count = 0;

/* -------------------------------------------------------------------------------- */

extern int GetSection( PCHAR, PCHAR, PCHAR * );

/* -------------------------------------------------------------------------------- */

static int count_entries(const char *s)
{
    int count = 0;

    while (*s)
    {
	s += strlen(s) + 1;
	count++;
    }

    return count;
}

static int list_servers( const char *file, char **buf_out, const char ***list_out )
{
    int count = 0;
    char *buf = NULL;
    const char **list = NULL;

    TRACE((TC_UI, TT_API1, "list_servers: '%s'", file));

    if (GetSection( INI_APPSERVERLIST, (PCHAR)file, &buf) == CLIENT_STATUS_SUCCESS)
    {
	TRACE((TC_UI, TT_API1, "list_servers: Section @%p", buf));

	count = count_entries(buf);

	TRACE((TC_UI, TT_API1, "list_servers: %d entries", count));
	
	if (count)
	{
	    if ((list = malloc(count * sizeof(char *))) != NULL)
	    {
		char *s = buf, *end;
		int i = 0;

		while (*s)
		{
		    char *next = s + strlen(s) + 1;

		    TRACE((TC_UI, TT_API1, "list_servers: '%s'", s));

		    /* end the server name at the equals */
		    end = strchr(s, '=');
		    if (end)
			*end = '\0';

		    /* save pointer to string */
		    list[i++] = s;

		    s = next;
		}
	    }
	}
    }

    FlushPrivateProfileCache();

    *buf_out = buf;
    *list_out = list;

    return count;
}

static MenuTemplate *create_menu_structure(int count, const char *list[], int *menu_size)
{
    MenuTemplate *menu = NULL;

    TRACE((TC_UI, TT_API1, "create_menu_structure: count %d strings %p", count, list));
    
    *menu_size = sizeof(MenuTemplateHeader) + (count ? count : 1) * sizeof(MenuTemplateEntry);

    if ((menu = malloc(*menu_size)) != NULL)
    {
	MenuTemplateHeader *hdr;
	MenuTemplateEntry *item;
	char *help;
	int cmp, help_len;

	/* setup header */
	hdr = &menu->hdr;
	hdr->flags = 0;
	hdr->title = utils_msgs_lookup("MserverT");
	hdr->max_title = strlen(hdr->title) + 1;
	hdr->help_message = utils_msgs_lookup("MserverH");
	hdr->max_help = strlen(hdr->help_message) + 1;
	hdr->show_event = 0;
	hdr->hide_event = 0;

	help = utils_msgs_lookup("MserverHi");
	help_len = strlen(help) + 1;
	
	item = menu_template_entry(menu, 0);

	if (count == 0)
	{
	    hdr->num_entries = 1;

	    item->flags = Menu_Entry_Faded;
	    item->component_id = 0;
	    item->text = (char *)utils_msgs_lookup("MserverNone");
	    item->max_text = strlen(item->text) + 1;
	    item->click_show = NULL;
	    item->submenu_show = NULL;
	    item->submenu_event = 0;
	    item->click_event = 0;
	    item->help_message = help;
	    item->max_entry_help = help_len;
	}
	else
	{
	    hdr->num_entries = count;
	    
	    /* process string at 's' */
	    for (cmp = 0; cmp < count; cmp++, item++)
	    {
		const char *s = list[cmp];

		TRACE((TC_UI, TT_API1, "create_menu_structure: string %d %p", cmp, s));
		TRACE((TC_UI, TT_API1, "create_menu_structure: '%s'", s));

		item->flags = 0;
		item->component_id = cmp;
		item->text = (char *)s;
		item->max_text = strlen(s) + 1;
		item->click_show = NULL;
		item->submenu_show = NULL;
		item->submenu_event = 0;
		item->click_event = tbres_event_SERVER_CONNECT;
		item->help_message = help;
		item->max_entry_help = help_len;
	    }
	}
    }

    TRACE((TC_UI, TT_API1, "create_menu_structure: returns %p", menu));

    return menu;
}

/* -------------------------------------------------------------------------------- */

ObjectId create_menu_object(const char *object_name, const MenuTemplate *menu, int menu_size)
{
    ObjectTemplateHeader templ;
    ObjectId id;

    templ.object_class = Menu_ObjectClass;
    templ.flags = Object_Shared;
    templ.version = 102;
    strcpy(templ.name, object_name);
    templ.total_size = sizeof(templ);
    templ.body = (MenuTemplate *)menu;
    templ.body_size = menu_size;
	
    id = NULL_ObjectId;
    LOGERR(toolbox_create_object(Toolbox_CreateObject_InCore, &templ, &id));

    return id;
}

/* -------------------------------------------------------------------------------- */

ObjectId serverlist_create_menu( const char *file )
{
    MenuTemplate *menu;
    int menu_size;
    ObjectId id;

    TRACE((TC_UI, TT_API1, "serverlist_create_menu: '%s'", file));

    server_count = list_servers( file, &server_section, &server_list);
    menu = create_menu_structure( server_count, server_list, &menu_size );

    id = NULL;
    if (menu)
	id = create_menu_object( "serverM", menu, menu_size );

    TRACE((TC_UI, TT_API1, "serverlist_create_menu: returns id %p", id));

    free(menu);
    /* don't free the names as we need to be able to look them up later */
    
    return id;
}

const char *serverlist_get_name( int index )
{
    if (index < 0 || index >= server_count)
	return NULL;

    if (server_list == NULL)
	return NULL;
    
    return server_list[index];
}

void serverlist_uncache(void)
{
    free(server_section);
    server_section = NULL;

    free(server_list);
    server_list = NULL;

    server_count = 0;
}

/* -------------------------------------------------------------------------------- */

/* eof app/server.c */
