/* -*-c-*- */

/* dir2html.c */

/* Given a RISC OS directory make an HTML file for its contents */

/* CHANGELOG

 * 01/08/96: SJM: Added compile option to display path in unixy form
 * 13/09/96: SJM: when above set then ensure  / to . happens in leaf namkes

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "memwatch.h"

#include "os.h"
#include "swis.h"

#include "filetypes.h"
#include "util.h"
#include "url.h"
#include "makeerror.h"
#include "dir2html.h"
#include "msgs.h"
#include "version.h"

#ifndef DIR2HTML_UNIX_PATH
#ifdef STBWEB
#define DIR2HTML_UNIX_PATH 1
#else
#define DIR2HTML_UNIX_PATH 0
#endif
#endif

/* static char *d2h_fmt_title = "Index of %s"; */
#ifdef STBWEB
static char *d2h_fmt_footer = "Listing generated %s\n";
#else
static char *d2h_fmt_footer = "Listing generated %s by ANT Fresco&#174;.\n";
#endif

static char *pads = "                        ";
#define NPADS 24

/* As yet no flags are defined */

typedef struct {
    int load;
    int exec;
    int len;
    int attr;
    int otype;
    char name[256];
} d2h_finfo;

extern os_error *dir2html(char *path, char *filename, int flags)
{
    os_error *ep = NULL;
    os_gbpbstr gps;
    FILE *fh;
    d2h_finfo *info;
    int plen;
    time_t now;
    char *ppath;

    info = mm_malloc(sizeof(*info));
    
#if 1
    /* SJM: made safe from long filenames */
    plen = strlen(path);
    ppath = mm_malloc(plen + 2);
    strcpy(ppath, path);
    if (ppath[plen-1] != '.')
    {
	ppath[plen++] = '.';
	ppath[plen] = 0;
    }
#else
    strcpy(buffer, path);
    plen = strlen(buffer);
    if (buffer[plen-1] != '.')
	buffer[plen++] = '.';
    buffer[plen] = 0;
#endif

#if 0
    fprintf(stderr, "Reading directory %s into file %s\n", buffer, filename);
#endif

    fh = mmfopen(filename, "w");
    if (fh)
    {
/* 	char lbuffer[256]; */
	char *newpath;
	char *url;

	gps.action = 10;
	gps.file_handle = (int) (long) path;
	gps.data_addr = info;
	gps.seq_point = 0;
	gps.buf_len = sizeof(*info);
	gps.wild_fld = "*";

	newpath = url_riscos_to_path(ppath);
/* #if DIR2HTML_UNIX_PATH */
/* 	sprintf(lbuffer, d2h_fmt_title, newpath); */
/* #else */
/* 	sprintf(lbuffer, d2h_fmt_title, path); */
/* #endif */
	url = url_unparse("file", 0, newpath, 0, 0, 0);

#if DIR2HTML_UNIX_PATH
	fprintf(fh, "<head><title>Index of %s</title><base href=\"%s\"></head>\r\n", newpath, url);
	fprintf(fh, "<body><h1>Index of %s</h1><p><pre>\r\n", newpath);
#else
	fprintf(fh, "<head><title>Index of %s</title><base href=\"%s\"></head>\r\n", path, url);
	fprintf(fh, "<body><h1>Index of %s</h1><p><pre>\r\n", path);
#endif
	fprintf(fh, "<img src=\"icontype:blank\"> Name                     Last modified     Size<hr>\r\n");
	fprintf(fh, "<img src=\"icontype:back\"> <a href=\"../\">../</a>                     Parent directory\r\n");

	mm_free(url);
	mm_free(newpath);

#if 0
	fprintf(stderr, "Looping...\n");
#endif

	while (ep == NULL && gps.seq_point != -1)
	{
	    gps.number = 1;

#if 0
	    fprintf(stderr, "Calling gbpb, action=%d, dir=%s, buffer=%p, n=%d, off=%d, len=%d, wild=%s\n", gps.action, (char*) (long) gps.file_handle, gps.data_addr, gps.number, gps.seq_point, gps.buf_len, gps.wild_fld);
#endif

	    ep = os_gbpb(&gps);
#if 0
	    fprintf(stderr, " gbpb returned, number = %d, ep = %p\n", gps.number, ep);
#endif

	    if ((ep == NULL) && (gps.number != 0))
	    {
		char iconname[32];
		char sizeinfo[16];
		char dateinfo[20];
		char datetmp[8];
		char *name;
		int llen;
		os_regset r;
		char *path_and_name;

#if 0
		fprintf(stderr, "Info on %s\n", info->name);
#endif
		path_and_name = mm_malloc(plen + strlen(info->name) + 1);
		strcpy(path_and_name, ppath);
		strcpy(path_and_name + plen, info->name);
/* 		strcpy(buffer + plen, info->name); */

		newpath = url_riscos_to_path(path_and_name);
		url = url_unparse("file", 0, newpath, 0, 0, 0);

		if (info->otype == 2)
		{
		    strcpy(iconname, "directory");
		    strcpy(sizeinfo, "&lt;DIR&gt;");
		}
		else
		{
		    sprintf(iconname, ",%03x", file_type(path_and_name));
		    if (info->len > (2 << 20))
		    {
			sprintf(sizeinfo, "%4dM", info->len >> 20);
		    }
		    else if (info->len > (2 << 10))
		    {
			sprintf(sizeinfo, "%4dK", info->len >> 10);
		    }
		    else
		    {
			sprintf(sizeinfo, "%4d ", info->len);
		    }
		}

		*((int*)datetmp) = info->exec;
		datetmp[4] = info->load & 0xff;

		r.r[0] = (int) (long) datetmp;
		r.r[1] = (int) (long) dateinfo;
		r.r[2] = sizeof(dateinfo);
		r.r[3] = (int) (long) "%DY-%M3-%CE%YR %24:%MI";

		os_swix(OS_ConvertDateAndTime, &r);

		llen = strlen(info->name);
#if DIR2HTML_UNIX_PATH
		name = strrchr(newpath, '/');
		if (name) name++; else name = newpath;
#else
		name = info->name;
#endif

		fprintf(fh, "<img src=\"icontype:%s\"> <a href=\"%s\">%s</a>%s%s  %s\r\n",
			iconname, url, name,
			pads + (llen < NPADS ? llen : NPADS-1), dateinfo, sizeinfo);

		mm_free(url);
		mm_free(newpath);
		mm_free(path_and_name);
#if 0
		fprintf(stderr, "Done processing\n");
#endif
	    }
#if 0
	    fprintf(stderr, "While loop end\n");
#endif
	}
#if 0
	fprintf(stderr, "Loop done\n");
#endif
	fprintf(fh, "<hr>");
	now = time(NULL);
	{
	    char buffer[32];
	    strftime(buffer, sizeof(buffer), msgs_lookup("dirdate"), localtime(&now));
	    fprintf(fh, d2h_fmt_footer, buffer);
	}
	fprintf(fh, "</pre></body>\r\n");

	mmfclose(fh);

	set_file_type(filename, FILETYPE_HTML);

#if 0
	fprintf(stderr, "All done\n");
#endif
    }
    else
    {
	ep = makeerror(ERR_CANT_GET_URL);
    }
#if 0
    fprintf(stderr, "Dir2html finnished\n");
#endif

    mm_free(ppath);
    mm_free(info);

    return ep;
}
