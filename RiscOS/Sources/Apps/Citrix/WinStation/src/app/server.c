/* > app/server.c
 *
 */

#include "tboxlibs/menu.h"


static char *server_section = NULL;
static char **server_list = NULL;
static int server_count = 0;

void servermenu_create(void)
{
    char *buf;
    if (GetSection( INI_APPSERVERLIST, APPSRV_FILE, &buf) == CLIENT_STATUS_SUCCESS)
    {
	char *s, *end, **list;
	int count;
	MenuTemplateHeader *menu;
	MenuTemplate *item;

	/* count number of entries */
	count = 0;
	s = buf;
	do
	{
	    end = strchr(s, '=');
	    if (end)
	    {
		count++;
		s = end + 1;
	    }
	}
	while (end);

	menu = NULL;
	list = NULL;

	if (count &&
	    (menu = sizeof(MenuTemplateHeader) + count * sizeof(MenuTemplate)) != NULL &&
	    (list = malloc(count * sizeof(char *))) != NULL)
	{
	    int cmp;

	    /* setup header */
	    menu->flags = 0;
	    menu->title = msgs_lookup("MserverT");
	    menu->max_title = strlen(menu->title);
	    menu->help_message = msgs_lookup("MserverH");
	    menu->max_help = strlen(menu->help_message);
	    menu->show_event = 0;
	    menu->hide_event = 0;
	    menu->num_entries = count;

	    item = (MenuTemplate *)(menu + 1);
	    
	    s = buf;
	    cmp = 0;
	    do
	    {
		/* end the server name at the equals */
		end = strchr(s, '=');
		if (end)
		{
		    /* terminate string for convenience */
		    *end = '\0';

		    /* process string at 's' */
		    item->flags = 0;
		    item->component_id = cmp;
		    item->text = s;
		    item->max_text = strlen(item->text);
		    item->click_show = NULL;
		    item->submenu_show = NULL;
		    item->submenu_event = 0;
		    item->click_event = tbres_event_SERVER_CONNECT + cmp;
		    item->help_message = msgs_lookup("MserverHi");
		    item->max_entry_help = strlen(item->help_message);

		    /* save pointer to string */
		    list[cmp] = s;
		    
		    /* increment component */
		    cmp++;

		    /* skip whitespace to start of next string */
		    for (s = end+1; *s && isspace(*s); s++)
			;
		}
	    }
	    while (*s);
	}
	else
	{
	    free(menu);
	    free(list);
	}
    }

    server_count = count;
    server_section = buf;
    server_list = list;
}

/* eof app/server.c */
