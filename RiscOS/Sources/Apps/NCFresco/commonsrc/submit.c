/* > submit.c

 *

 */

#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "memwatch.h"
#include "makeerror.h"
#include "rid.h"
#include "antweb.h"
#include "interface.h"
#include "url.h"
#include "util.h"

#if UNICODE
#include "encoding.h"
#include "utf8.h"
#else
typedef struct Encoding Encoding;
#endif

/* ---------------------------------------------------------------------------------------------------- */

extern int otextarea_line_length(rid_textarea_item *tai, int line, BOOL *terminated);

/* ---------------------------------------------------------------------------------------------------- */

#define LEEWAY 64
void be_ensure_buffer_space(char **buffer, int *len, int more)
{
    int curlen = strlen(*buffer) + 1;

    if (curlen + more >= *len)
    {
	*len = *len + more + LEEWAY;
	*buffer = (char*) mm_realloc(*buffer, *len);
    }
}

/* ---------------------------------------------------------------------------------------------------- */

#if UNICODE
typedef struct
{
    FILE *f;
    char **bufout;
    int *len;
} submit_info;

static os_error *submit_fn(const char *text, BOOL last, void *handle)
{
    submit_info *si = handle;

    BENDBGN(("submit_fn: '%s' last %d\n", text, last));

    if (si->f)
	url_escape_to_file(text, si->f);

    if (si->bufout)
    {
	be_ensure_buffer_space(si->bufout, si->len, strlen(text) * 3);

	url_escape_cat(*(si->bufout), text, *(si->len));
    }

    return NULL;
}
#endif

static void append_encoded_n(Encoding *enc, char **bufout, int *len, const char *val, int val_n)
{
    if (val == NULL)
	return;

#if UNICODE
    if (enc)
    {
	submit_info si;
	memset(&si, 0, sizeof(si));

	si.bufout = bufout;
	si.len = len;

	BENDBGN(("append_encoded_n: enc %p '%.*s'\n", enc, val_n, val));
	
	process_utf8(val, val_n, enc, submit_fn, &si);
    }
    else
#endif
    {
	be_ensure_buffer_space(bufout, len, strlen(val) * 3);

	url_escape_cat_n(*bufout, val, *len, val_n);
    }
}

static void append_encoded(Encoding *enc, char **buffer, int *len, const char *val)
{
    append_encoded_n(enc, buffer, len, val, INT_MAX);
}

static void append_ascii(char **buffer, int *len, const char *val)
{
    be_ensure_buffer_space(buffer, len, strlen(val));
    strcat(*buffer, val);
}

/* ---------------------------------------------------------------------------------------------------- */

static void write_encoded_n(Encoding *enc, FILE *f, const char *val, int val_n)
{
    if (val == NULL)
	return;

#if UNICODE
    if (enc)
    {
	submit_info si;

	memset(&si, 0, sizeof(si));
	si.f = f;

	process_utf8(val, val_n, enc, submit_fn, &si);
    }
    else
#endif
    {
	url_escape_to_file_n(val, f, val_n);
    }
}

static void write_encoded(Encoding *enc, FILE *f, const char *val)
{
    write_encoded_n(enc, f, val, INT_MAX);
}

/* ---------------------------------------------------------------------------------------------------- */

static void otextarea_append_to_buffer(Encoding *enc, rid_textarea_item *tai, char **buffer, int *blen)
{
    int i;

    flexmem_noshift();
    
    for (i = 0; i < tai->n_lines; i++)
    {
	BOOL terminated;
	int len = otextarea_line_length(tai, i, &terminated);

	append_encoded_n(enc, buffer, blen, tai->text.data + tai->lines[i], len);

	if (i != tai->n_lines-1 &&
	    (tai->wrap == rid_ta_wrap_HARD || terminated))
	{
	    append_ascii(buffer, blen, "%0D%0A");
	}
    }

    flexmem_shift();
}

static void otextarea_write_to_file(Encoding *enc, rid_textarea_item *tai, FILE *f)
{
    int i;

    flexmem_noshift();
    
    for (i = 0; i < tai->n_lines; i++)
    {
	BOOL terminated;
	int len = otextarea_line_length(tai, i, &terminated);

	write_encoded_n(enc, f, tai->text.data + tai->lines[i], len);

	if (i != tai->n_lines-1 &&
	    (tai->wrap == rid_ta_wrap_HARD || terminated))
	    fputs("%0D%0A", f);
    }

    flexmem_shift();
}

/* ---------------------------------------------------------------------------------------------------- */

static void antweb_append_query(Encoding *enc, char **buffer, char *name, char *value, int *len)
{
    append_ascii(buffer, len, "&");

    if (name)
    {
	append_encoded(enc, buffer, len, name);
	append_ascii(buffer, len, "=");
    }

    append_encoded(enc, buffer, len, value);
}

static void antweb_append_textarea(Encoding *enc, char **buffer, rid_textarea_item *tai, int *len)
{
    if (tai->name == NULL)
	return;

    append_ascii(buffer, len, "&");
    append_encoded(enc, buffer, len, tai->name);

    append_ascii(buffer, len, "=");
    otextarea_append_to_buffer(enc, tai, buffer, len);
}

static void antweb_write_query(Encoding *enc, FILE *f, char *name, char *value, int *first)
{
    BENDBG(("write_query: enc %p first %d name '%s' value '%s'\n", enc, *first, name, value));

    if (*first)
	*first = FALSE;
    else
	fputc('&', f);

    if (name)
    {
	write_encoded(enc, f, name);
	fputc('=', f);
    }

    write_encoded(enc, f, value);
}

static void antweb_write_textarea(Encoding *enc, FILE *f, rid_textarea_item *tai, int *first)
{
    if (tai->name == NULL)
	return;

    if (*first)
	*first = FALSE;
    else
	fputc('&', f);

    write_encoded(enc, f, tai->name);
    fputc('=', f);

    otextarea_write_to_file(enc, tai, f);
}

/* ---------------------------------------------------------------------------------------------------- */

#ifdef STBWEB
BOOL backend_submit_form(be_doc doc, const char *id, int right)
{
    rid_form_item *form;

    for (form = doc->rh->form_list; form; form = form->next)
    {
	if (strcmp(form->id, id) == 0)
	{
	    antweb_submit_form(doc, form, right);
	    return TRUE;
	}
    }

    return FALSE;
}
#endif

/* ---------------------------------------------------------------------------------------------------- */

/* The 'right' flag indicates a right click */

void antweb_submit_form(antweb_doc *doc, rid_form_item *form, int right)
{
    os_error *ep = NULL;
    rid_form_element *fis;
    char *target = right ? "_blank" : form->target;
    Encoding *enc = NULL;

    BENDBG(( "submit_form: action='%s', method='%s'\n",
	    form->action ? form->action : "<none>",
	    (form->method == rid_fm_GET ?
	     "GET" :
	     (form->method == rid_fm_POST ?
	      "POST" :
	      "<unknown>") ) ));

#if UNICODE
    /* initialise encoding to that of the document */
    /* Check for an overriding supported charset to encode in */
    /* Only do this if we are storing internally in UTF8 - otherwise write out as it comes in */
    if (config_encoding_internal == 1 || config_encoding_internal == 2)
    {
	int enc_num = doc->rh->encoding;

	BENDBGN(("submit_form: doc encoding %d accept '%s'\n", enc_num, form->accept_charset ? form->accept_charset : "<none>"));

	if (form->accept_charset)
	{
	    char *s = form->accept_charset;
	    do
	    {
		int enc_num1;
	    
		s = skip_space_or_comma(s);

		enc_num1 = encoding_number_from_name(s);
		if (enc_num1)
		{
		    BENDBGN(("submit_form: match charset '%s' %d\n", s, enc_num1));

		    enc_num = enc_num1;
		    break;
		}

		s = skip_to_space_or_comma(s);
	    }
	    while (s && *s);
	}

	enc = encoding_new(enc_num, encoding_WRITE);
    }
#endif
    
    switch(form->method)
    {
    case rid_fm_GET:
        {
	    char *buffer;
	    char *dest, *dest2;
	    int buf_size = 1000;

	    dest = url_join(BASE(doc), form->action);

	    buffer = (char*) mm_malloc(buf_size);
	    buffer[0] = 0;
	    for (fis = form->kids; fis; fis = fis->next)
	    {
		switch (fis->tag)
		{
		case rid_form_element_INPUT:
		{
		    rid_input_item *iis = (rid_input_item *)fis;
		    switch (iis->tag)
		    {
		    case rid_it_HIDDEN:
			antweb_append_query(enc, &buffer, iis->name, iis->value, &buf_size);
			break;
		    case rid_it_IMAGE:
			if ((iis->name != NULL) && (iis->data.image.x != -1))
			{
			    char buf2[12];
			    char buf3[128];

			    strcpy(buf3, strsafe(iis->name));

			    /* pdh: duplicate NS3's behaviour with missing
			     * names
			     */
			    strcat( buf3, (*buf3) ? ".x" : "x" );
			    sprintf(buf2, "%d", iis->data.image.x);
			    antweb_append_query(enc, &buffer, buf3, buf2, &buf_size);

			    buf3[strlen(buf3)-1] = 'y';
			    sprintf(buf2, "%d", iis->data.image.y);
			    antweb_append_query(enc, &buffer, buf3, buf2, &buf_size);
			}
			break;
		    case rid_it_TEXT:
		    case rid_it_PASSWD:
			antweb_append_query(enc, &buffer, iis->name, iis->data.str, &buf_size);
			break;
		    case rid_it_CHECK:
		    case rid_it_RADIO:
			if (iis->data.radio.tick)
			{
			    antweb_append_query(enc, &buffer, iis->name, iis->value ? iis->value : "on", &buf_size);
			}
			break;
		    case rid_it_SUBMIT:
			if (iis->data.button.tick && iis->name)
			{
			    antweb_append_query(enc, &buffer, iis->name, iis->value, &buf_size);
			}
			break;
		    }
		    break;
		}

		case rid_form_element_SELECT:
		{
		    rid_select_item *sis = (rid_select_item *)fis;
		    rid_option_item *ois;
		    for(ois = sis->options; ois; ois = ois->next)
			if (ois->flags & rid_if_SELECTED)
			    antweb_append_query(enc, &buffer, sis->name,
						ois->value ? ois->value : ois->text, &buf_size);
		    break;
		}

		case rid_form_element_TEXTAREA:
		{
		    rid_textarea_item *tai = (rid_textarea_item *)fis;
		    antweb_append_textarea(enc, &buffer, tai, &buf_size);
		    break;
		}
		}
	    }
	    if (buffer[0] == 0)
		buffer[1] = 0;
	    buffer[0] = '?';
	    dest2 = url_join(dest, buffer);
	    mm_free(dest);
	    mm_free(buffer);

	    /* In theory the URL join can fail */
	    if (dest2)
	    {
		BENDBG(( "Query string is:\n'%s'\n", dest2));

		/* Never get a form query from the cache */
		ep = frontend_complain(frontend_open_url(dest2, doc->parent, target, NULL, fe_open_url_NO_CACHE));

		mm_free(dest2);
	    }
	    else
	    {
		frontend_complain(makeerror(ERR_BAD_FORM));
	    }
	}
	break;
    case rid_fm_POST:
        {
	    FILE *f;
	    char *fname;
	    int first = TRUE;
	    char *dest;

	    fname = strdup(rs_tmpnam(0));
	    dest = strrchr(fname, 'x');
	    if (dest)
		*dest = 'y';
	    f = mmfopen(fname, "w");

	    for (fis = form->kids; fis; fis = fis->next)
	    {
		switch (fis->tag)
		{
		case rid_form_element_INPUT:
		{
		    rid_input_item *iis = (rid_input_item *)fis;
		    switch (iis->tag)
		    {
		    case rid_it_HIDDEN:
			antweb_write_query(enc, f, iis->name, iis->value, &first);
			break;
		    case rid_it_IMAGE:
			if (iis->data.image.x != -1)
			{
			    char buf2[12];
			    char buf3[128];

			    strcpy(buf3, strsafe(iis->name));

			    /* pdh: duplicate NS3's behaviour with missing
			     * names
			     */
			    strcat( buf3, (*buf3) ? ".x" : "x" );
			    sprintf(buf2, "%d", iis->data.image.x);
			    antweb_write_query(enc, f, buf3, buf2, &first);

			    buf3[strlen(buf3)-1] = 'y';
			    sprintf(buf2, "%d", iis->data.image.y);
			    antweb_write_query(enc, f, buf3, buf2, &first);
			}
			break;
		    case rid_it_TEXT:
		    case rid_it_PASSWD:
			antweb_write_query(enc, f, iis->name, iis->data.str, &first);
			break;
		    case rid_it_CHECK:
		    case rid_it_RADIO:
			if (iis->data.radio.tick)
			{
			    antweb_write_query(enc, f, iis->name, iis->value ? iis->value : "on", &first);
			}
			break;
		    case rid_it_SUBMIT:
			if (iis->data.button.tick && iis->name)
			{
			    antweb_write_query(enc, f, iis->name, iis->value, &first);
			}
			break;
		    }
		}
		break;

		case rid_form_element_SELECT:
		{
		    rid_select_item *sis = (rid_select_item *)fis;
		    rid_option_item *ois;
		    for(ois = sis->options; ois; ois = ois->next)
			if (ois->flags & rid_if_SELECTED)
			    antweb_write_query(enc, f, sis->name, ois->value ? ois->value : ois->text, &first);
		    break;
		}

		case rid_form_element_TEXTAREA:
		{
		    rid_textarea_item *tai = (rid_textarea_item *)fis;
		    antweb_write_textarea(enc, f, tai, &first);
		    break;
		}
		}
	    }

#if 0
	    fputs("\r\n", f);
#endif
	    mmfclose(f);

	    dest = url_join(BASE(doc), form->action);

	    /* Never get a form query from the cache */
	    ep = frontend_complain(frontend_open_url(dest, doc->parent, target, fname, fe_open_url_NO_CACHE));
	    mm_free(dest);
	}
	break;
    default:
	break;
    }

#if UNICODE
    if (enc)
	encoding_delete(enc);
#endif
}

/* ---------------------------------------------------------------------------------------------------- */

/* eof submit.c */
