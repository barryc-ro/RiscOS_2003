/* > pluginfile.c

 * CHANGELOG

 * 05/09/96: SJM: Created

 */

#include <stdio.h>
#include <string.h>

#include "memwatch.h"
#include "util.h"

#include "pluginfile.h"
#include "config.h"

/* ----------------------------------------------------------------------------- */

typedef struct plugin_list_info plugin_list_info;

struct plugin_list_info
{
    plugin_list_info *next;

    plugin_info info;
};

/* ----------------------------------------------------------------------------- */

#define PLUGIN_LIST_FORMAT	1
#define FORMAT_STRING		"Format:"
#define SEPARATORS		" \t\n\r"

/* ----------------------------------------------------------------------------- */

static plugin_list_info *plugin_list = NULL;

/* ----------------------------------------------------------------------------- */


static void plugin_list__add(const plugin_info *new_info)
{
    plugin_list_info *info;

    /* create and link in new entry */
    info = mm_calloc(1, sizeof(*info));
    info->info = *new_info;

    info->next = plugin_list;
    plugin_list = info;

    OBJDBGN(( "pluginlist: add %p (%03x)\n", info, info->info.file_type));
}

/* ----------------------------------------------------------------------------- */

void plugin_list_add(const plugin_info *new_info)
{
    plugin_list__add(new_info);
    
    /* keep uptodate on disc if asked */
    if (config_plugin_uptodate)
	plugin_list_write_file();
}

void plugin_list_write_file(void)
{
    FILE *f;

    OBJDBGN(( "pluginlist: write file %p '%s'\n", config_plugin_file, config_plugin_file));

    if (!gstrans_not_null(config_plugin_file))
	return;
    
/*     if (ensure_modem_line() != NULL) */
/* 	return; */
    
    f = mmfopen(config_plugin_file, "w");
    if (f)
    {
	plugin_list_info *info;
	
	fprintf(f, "# Plugin config\n");
	fprintf(f, FORMAT_STRING " %d\n", PLUGIN_LIST_FORMAT);
	
	for (info = plugin_list; info; info = info->next)
	{
	    OBJDBGN(( "pluginlist: write %p (%03x)\n", info, info->info.file_type));

	    fprintf(f, "%03x\t%c\n", info->info.file_type, info->info.flags & plugin_info_flag_DISABLED ? 'D' : 'd');
	}
	mmfclose(f);
    }
}

void plugin_list_read_file(void)
{
    FILE *f;

    OBJDBGN(( "pluginlist: read file %p '%s'\n", config_plugin_file, config_plugin_file));

    if (file_type(config_plugin_file) == -1)
	return;
    
    f = mmfopen(config_plugin_file, "r");
    if (f)
    {
	char buffer[256];
	int format = 1;
	
	do
	{
	    if (fgets(buffer, sizeof(buffer), f) == NULL)
		break;

	    if (buffer[0] == '#')
		continue;

	    if (strncmp(buffer, FORMAT_STRING, sizeof(FORMAT_STRING)-1) == 0)
	    {
		format = atoi(buffer + sizeof(FORMAT_STRING)-1);
	    
		if (format > PLUGIN_LIST_FORMAT)
		{
		    usrtrc( "Unsupported plugin list format (%d)\n", format);
		    break;
		}
		continue;
	    }
	    
	    if (buffer[0]) switch (format)
	    {
	    case 1:
	    {
		char *s;
		plugin_info info;

		if ((s = strtok(buffer, SEPARATORS)) == NULL)
		    continue;
		
		info.file_type = (int)strtoul(s, NULL, 16);
		
		info.flags = 0;
		if ((s = strtok(NULL, SEPARATORS)) == NULL)
		    continue;
		
		if (strchr(s, 'D') != NULL)
		    info.flags |= plugin_info_flag_DISABLED;

		plugin_list__add(&info);
		break;		
	    }
	    }
	}
	while (!feof(f));

	mmfclose(f);
    }
}

plugin_info *plugin_list_get_info(int file_type)
{
    plugin_list_info *info;
    for (info = plugin_list; info; info = info->next)
	if (info->info.file_type == file_type)
	    return &info->info;

    return NULL;
}

void plugin_list_toggle_flags(int file_type, int bic_flags, int eor_flags)
{
    plugin_info *info = plugin_list_get_info(file_type);

    OBJDBGN(( "pluginlist: toggle %p BIC %x EOR %x\n", info, bic_flags, eor_flags));

    if (info)
    {
	info->flags = (info->flags &~ bic_flags) ^ eor_flags;
    
	/* keep uptodate on disc if asked */
	if (config_plugin_uptodate)
	    plugin_list_write_file();
    }
}

/* ----------------------------------------------------------------------------- */

void plugin_list_dispose(void)
{
    plugin_list_info *info = plugin_list;
    
    OBJDBGN(( "pluginlist: dispose all from %p\n", plugin_list));
    
    while (info)
    {
	plugin_list_info *next = info->next;
	mm_free(info);
	info = next;
    }
    plugin_list = NULL;
}

/* ----------------------------------------------------------------------------- */

/* eof pluginfile.c */
