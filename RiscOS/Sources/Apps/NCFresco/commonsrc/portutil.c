/* portutil.c - portable utility functions */
/* some of this was originally in util.c */

/*
 * CHANGELOG
 * 10/07/96: SJM: fixed translate_escaped_text to behave as it used to.
 * 29/08/96: SJM: changed isalpha to isalnum in parse_http_header
 * 18/09/96: DAF: Added shuffle_array.
 * 25/02/97: DAF: lookup_key_action moved here from util.c
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include "memwatch.h"
#include "util.h"
#include "sgmlparser.h"
#include "akbd.h"

#define MAX_LINE	256

/* pmatch2 always does a case sensitive match and calls itself recursively */
#if 0 /*not called?*/
extern int pmatch2(char *s, char *p)
{
    char *q, *r;
    int len;
    char t;

    /* Spot the trivial */
    if (*s == 0 && *p == 0)
	return 1;

    q = strpbrk(p, "*?");

    /* No wild cards?  Return whether we match */
    if (q == NULL)
	return (strcmp(s,p) == 0);

    /* Some before the wild card?  Give up now if that does not match */
    if (q != p && strncmp(s,p, (q-p)) != 0)
	return 0;

    /* Move up to the wild card */
    s += (q-p);
    p = q;

    /* Strip off ant question marks */
    while (*p == '?')
    {
	/* A question mark does not match an empty string */
	if (*s == 0)
	    return 0;
	/* Skip the next letter */
	s++;
	p++;
    }

    /* If we are at the end off the patten, it ok if we are at the end of the srting to */
    if (*p == 0)
	return (*s == 0);

    /* We are not on a question mark, if we are not on an asterisk just call again */
    if (*p != '*')
	return pmatch2(s,p);

    /* We are now on an asterisk in the patern */
    /* Find out what is after the asterisk */
    q = p+1;

    /* Ignore strings of wild cards */
    while (*q == '*' || *q == '?')
    {
	/* except that ever question mark must tack up a character at some stage */
	if (*q == '?')
	{
	    if (*s == 0)
		return 0;
	    else
		s++;
	}
	q++;
    }

    /* If there is nothing after the asterisk the tail must match */
    if (*q == 0)
	return 1;

    /* Check for any more wide cards down the line */
    r = strpbrk(q, "*?");

    /* Work out how much we look for */
    if (r == NULL)
	len = strlen(q);
    else
	len = (r-q);

    /* Look for matches of the next stretch */
    do {
	t = q[len];
	q[len] = 0;
	r = strstr(s, q);
	q[len] = t;

	if (r)
	{
	    /* If we match the stretch... */
	    /* We know the head matches, so we can skip that */
	    if (pmatch2(r + len, q + len))
		/* A recusive match might work */
		return 1;
	    else
		/* Otherwise skip this match and try again */
		s = r+1;
	}
	else
	{
	    /* If we can't match the stretch then we fail */
	    return 0;
	}
	/* Keep going as long as there is something to look for a match in */
    } while (*s);

    return 0;
}
#endif

#if 0 /*not called?*/
extern int pattern_match(char *s, char *pat, int cs)
{
    char *ss, *pp;
    char *p;
    int match = 1;

    ss = strdup(s);
    pp = strdup(pat);

    if (ss == NULL || pp == NULL)
	goto done;

    if (cs == 0)
    {
	for(p = ss; *p; p++)
	    *p = toupper(*p);

	for(p = pp; *p; p++)
	    *p = toupper(*p);
    }

    match = pmatch2(ss, pp);

 done:
    mm_free(ss);
    mm_free(pp);
    return match;
}
#endif


extern char *strdup(const char *s)
{
    char *ss;

    if (s == NULL)
	return NULL;

    ss = mm_malloc(strlen(s)+1);
    if (ss)
	strcpy(ss, s);

    return ss;
}

#ifdef STBWEB
extern char *strndup(const char *s, size_t maxlen)
{
    char *s1 = NULL;
    if (s)
    {
        const char *end = memchr(s, 0, maxlen);
        int len = end ? end - s : maxlen;
        s1 = mm_malloc(len + 1);
        if (s1)
        {
            memcpy(s1, s, len);
            s1[len] = '\0';
        }
    }
    return s1;
}
#endif

void strncpysafe(char *s1, const char *s2, int n)
{
    strncpy(s1, s2, n-1);

    s1[n-1] = 0;
}

void strlencat(char *s1, const char *s2, int len)
{
    int len2;

    len2 = len - strlen(s1) - 1;

    if (len2 > 1)
    {
	strncat(s1, s2, len2);
	s1[len-1] = 0;
    }
}

unsigned int string_hash(const char *s)
{
    unsigned int h;

    h = 0;

    while (*s)
    {
	h = (h << 13) | (h >> 19);

	h ^= *s++;
    }

    return h;
}

#ifdef STBWEB
extern char *strcasestr(const char *s1, const char *s2)
{
    if (s1 && s2)
    {
        int i,
            s2n = strlen(s2),
            last_pos = strlen(s1) - s2n;

        for (i = 0; i <= last_pos; i++, s1++)
            if (strncasecomp(s1, s2, s2n) == 0)
                return (char *)s1;
    }
    return NULL;
}
#endif


/*---------------------------------------------------------------------------*
 * strnearly(s1,s2,n)                                                        *
 * Returns TRUE if s1 is within edit distance n of s2, where edit distance   *
 * is defined as number of additions or deletions of characters. O(mn) where *
 * m is length of shorter string and n is n.                                 *
 *---------------------------------------------------------------------------*/

BOOL strnearly( const char *input, size_t ilen,
                const char *pattern, size_t plen, size_t hownear )
{
    while ( ilen && plen )
    {
        if ( toupper(*input) != toupper(*pattern) )
        {
            if ( !hownear )
                return FALSE;

            /* case bgXcolor */
            if ( strnearly( input+1, ilen-1, pattern, plen, hownear-1 ) )
                return TRUE;
            /* case bgcolr */
            if ( strnearly( input, ilen, pattern+1, plen-1, hownear-1 ) )
                return TRUE;

            /* case bgcoXor */
            hownear--;
        }
        input++;
        ilen--;
        pattern++;
        plen--;
    }

    return hownear >= (ilen + plen);
}

char *strlwr(char *s)
{
    int c;
    char *ss = s;
    while ((c = *ss) != 0)
	*ss++ = tolower(c);
    return s;
}


char encode_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void uuencode(char *in, char *out, int out_len)
{
    int inbytes, outbytes;
    int acc;
    int shift;
    unsigned char *ii;
    char *oout = out;

    ii = (unsigned char *) in;

    inbytes = strlen(in);

    outbytes = ((inbytes*4)+2)/3;
    if (outbytes >= out_len)
	outbytes = out_len - 1;

    acc = shift = 0;

    while (outbytes && inbytes)
    {
	acc += *(ii++) << (8 - shift);
	inbytes--;
	shift += 8;

	while (shift >= 6)
	{
	    *(out++) = encode_table[acc >> 10];
	    outbytes--;
	    acc <<= 6;
	    acc &= 0xffff;
	    shift -= 6;
	}
    }

    if (shift != 0)
    {
	    *(out++) = encode_table[acc >> 10];
	    outbytes--;
	    acc <<= 6;
	    acc &= 0xffff;
	    shift -= 6;
    }

    while ((out-oout)&3)
    {
	/* Pad with equals signs */
	*out++ = '=';
    }

    *out = 0;
}

int pr2six[256]={
    64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64, 64,64,64,62,64,64,64,63,
    52,53,54,55,56,57,58,59, 60,61,64,64,64,64,64,64,
    64, 0, 1, 2, 3, 4, 5, 6,  7, 8, 9,10,11,12,13,14,
    15,16,17,18,19,20,21,22, 23,24,25,64,64,64,64,64,
    64,26,27,28,29,30,31,32, 33,34,35,36,37,38,39,40,
    41,42,43,44,45,46,47,48, 49,50,51,64,64,64,64,64,
    64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
    64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64
};

void uudecode(char *bufcoded, unsigned char *bufplain, int outbufsize)
{
    int nbytesdecoded;
    char *bufin = bufcoded;
    unsigned char *bufout = bufplain;
    int nprbytes;

    /* Strip leading whitespace. */

    while(*bufcoded==' ' || *bufcoded == '\t')
	bufcoded++;

    /* Figure out how many characters are in the input buffer.
     * If this would decode into more bytes than would fit into
     * the output buffer, adjust the number of input bytes downwards.
     */
    bufin = bufcoded;
    while(pr2six[(int)*(bufin++)] <= 63)
	;
    nprbytes = bufin - bufcoded - 1;
    nbytesdecoded = ((nprbytes+3)/4) * 3;
    if(nbytesdecoded > outbufsize)
    {
        nprbytes = (outbufsize*4)/3;
    }

    bufin = bufcoded;

    while (nprbytes > 0)
    {
        *(bufout++) =
            (unsigned char) (pr2six[(int)*bufin] << 2 | pr2six[(int)bufin[1]] >> 4);
        *(bufout++) =
            (unsigned char) (pr2six[(int)bufin[1]] << 4 | pr2six[(int)bufin[2]] >> 2);
        *(bufout++) =
            (unsigned char) (pr2six[(int)bufin[2]] << 6 | pr2six[(int)bufin[3]]);
        bufin += 4;
        nprbytes -= 4;
    }

    if(nprbytes & 03)
    {
        if(pr2six[(int)bufin[-2]] > 63)
            nbytesdecoded -= 2;
        else
            nbytesdecoded -= 1;
    }
    bufplain[nbytesdecoded] = '\0';
}

/**********************************************************************/
/*
 * The bcmp and bzero functions are needed by various internet library
 * calls.  I just pass them through to the memXXX versions.
 */

#ifndef UNIX

int bcmp(void *s1, void *s2, size_t s)
{
    return memcmp(s1, s2, s);
}

void bzero(void *p, int s)
{
    memset(p, 0, s);
}

#endif /* UNIX */

extern int strncasecomp(const char *a, const char *b, size_t n)
{
    if (a == NULL || b == NULL)
	return -1;

    while (n > 0)
    {
	const char aa = tolower(*a);
	const char bb = tolower(*b);

	if (aa != bb)
	    return aa - bb;

	a++;
	b++;
	n--;
    }

    return 0;
}

extern int strcasecomp (const char *a, const char *b)
{
    const char *p =a;
    const char *q =b;
    for(p=a, q=b; *p && *q; p++, q++) {
	int diff = tolower(*p) - tolower(*q);
	if (diff) return diff;
    }
    if (*p) return 1;	/* p was longer than q */
    if (*q) return -1;	/* p was shorter than q */
    return 0;		/* Exact match */
}


/* ---------------------------------------------------------------------------------------------------------- */

static int find_tag(const char *tags[], const char *name)
{
    int i;

    for (i = 0; tags[i]; i++)
        if (strcasecomp((char *) tags[i], (char *) name) == 0)
            return i;

    return -1;
}

/*
 * This function takes a list of tags and a header string and writes out an
 * array of values. The output values are pointers into the original data.
 * The origiinal data will have beenmodified to insert 0's at appropriate points

 * A single value (no =) is initially assumed to be a flag and is matched
 * against the tag list passed in. If found the output value is set to "".
 * If not found then it is assumed to be an unnamed value and a name of ""
 * is searched for and the value filled in if found.

 * VALUEs can be enclosed within double quotes NAME="VALUE" so parsing has
 * to take this into account.

 */

#if 1

static int add_tag(char *name, int name_len, char *value, int value_len, const char *tags[], name_value_pair *output, int free_offset)
{
    int tag_num;
    int used;

    if (name)
	name[name_len] = 0;
    if (value)
	value[value_len] = 0;

    used = 0;
    tag_num = find_tag(tags, name);
    if (tag_num != -1)
    {				/* If we found the tag then write pointer into values[]  */
	output[tag_num].value = strsafe(value);
    }
    else if (free_offset > 0)
    {				/* Otherwise write into spare pointers (if available) */
	output[free_offset].name = name;
	output[free_offset].value = strsafe(value);
	used = 1;
    }

    PRSDBGN(("parse_http_header: add_tag: '%s'='%s' tag %d free %d used %d\n", strsafe(name), strsafe(value), tag_num, free_offset, used));

    return used;
}

enum lookingfor_state
{
    lookingfor_NAME,
    lookingfor_NAME_END,
    lookingfor_EQUALS,
    lookingfor_VALUE,
    lookingfor_QUOTE_END,
    lookingfor_SINGLE_QUOTE_END,
    lookingfor_VALUE_END,
    lookingfor_SEPARATOR
};

int parse_http_header(char *header_data, const char *tags[], name_value_pair *output, int output_size)
{
    enum lookingfor_state state = lookingfor_NAME;
    char *name = NULL;
    char *name_end = NULL;
    char *value = NULL;
    char *value_end = NULL;
    int i;
    char *s;
    int unknown = 0;		/* Number of unknown tags encountered */
    int ntags;
    int c;

    /* zero the output array first */
    for (i = 0; i < output_size; i++)
        output[i].name = output[i].value = NULL;

    /* count the number of tags */
    for (ntags = 0; tags[ntags]; ntags++)
	;

    s = header_data;
    do
    {
	int unknown_offset = ntags + unknown < output_size ? ntags + unknown : -1;

	c = *s++;

	switch (state)
	{
	case lookingfor_NAME:
	    if (isalnum(c))
	    {
		name = s-1;
		state = lookingfor_NAME_END;
	    }
	    break;

	case lookingfor_NAME_END:
	    if (c == 0 || c == ';')
	    {
		name_end = s-1;

		/* we've completed a NAME/VALUE without an equals */
		unknown += add_tag(name, name_end - name, NULL, 0, tags, output, unknown_offset);

		state = lookingfor_NAME;
		name = name_end = NULL;
	    }
	    else if (isspace(c))
	    {
		state = lookingfor_EQUALS;
		name_end = s-1;
	    }
	    else if (c == '=')
	    {
		state = lookingfor_VALUE;
		name_end = s-1;
	    }
	    break;

	case lookingfor_EQUALS:
	    if (c == '=')
	    {
		state = lookingfor_VALUE;
	    }
	    else if (c == 0 || c == ';' || isalnum(c))
	    {
		/* we've completed a NAME/VALUE without an equals */
		unknown += add_tag(name, name_end - name, NULL, 0, tags, output, unknown_offset);

		state = c == ';' ? lookingfor_NAME : lookingfor_SEPARATOR;
		name = name_end = NULL;
	    }
	    break;

	case lookingfor_VALUE:
	    if (c == ';')
	    {
		/* had NAME= */
		unknown += add_tag(name, name_end - name, NULL, 0, tags, output, unknown_offset);

		state = lookingfor_NAME;
		name = name_end = NULL;
	    }
	    else if (c == 0)
	    {
		/* had NAME= */
		unknown += add_tag(name, name_end - name, NULL, 0, tags, output, unknown_offset);
	    }
	    else if (c == '\"')
	    {
		state = lookingfor_QUOTE_END;
		value = s;
	    }
	    else if (c == '\'')
	    {
		state = lookingfor_SINGLE_QUOTE_END;
		value = s;
	    }
	    else if (!isspace(c))
	    {
		state = lookingfor_VALUE_END;
		value = s-1;
	    }
	    break;

	case lookingfor_SINGLE_QUOTE_END:
	    if (c == '\'')
	    {
		value_end = s-1;

		/* completed NAME=VALUE */
		unknown += add_tag(name, name_end - name, value, value_end - value, tags, output, unknown_offset);

		name = name_end = NULL;
		value = value_end = NULL;

		state = lookingfor_SEPARATOR;
	    }
	    break;

	case lookingfor_QUOTE_END:
#if 0
	    /* SJM: removed this 'cos I think it's wrong! */
	    if (c == ';')
	    {
		state = lookingfor_NAME;
		name = name_end = NULL;
	    }
#endif
	    if (c == '\"')
	    {
		value_end = s-1;

		/* completed NAME=VALUE */
		unknown += add_tag(name, name_end - name, value, value_end - value, tags, output, unknown_offset);

		name = name_end = NULL;
		value = value_end = NULL;

		state = lookingfor_SEPARATOR;
	    }
	    break;

	case lookingfor_VALUE_END:
	    if (c == 0 || c == ';')
	    {
		value_end = s-1;

		/* Strip space from end of string */
		while (value_end > value && isspace(value_end[-1]))
		    value_end--;

		/* completed NAME=VALUE */
		unknown += add_tag(name, name_end - name, value, value_end - value, tags, output, unknown_offset);

		name = name_end = NULL;
		value = value_end = NULL;

		state = c == ';' ? lookingfor_NAME : lookingfor_SEPARATOR;
	    }
	    break;

	case lookingfor_SEPARATOR:
	    if (c == ';')
	    {
		state = lookingfor_NAME;
	    }
	    break;
	}
    }
    while (c != 0);

    return unknown;
}

#else

#define SEPARATORS ";\n\r"


void parse_http_header(char *header_data, const char *tags[], char *values[])
{
    char *s;
    int i;

    /* zero the output array first */
    for (i = 0; tags[i]; i++)
        values[i] = NULL;

    s = strtok(header_data, SEPARATORS);
    if (s) do
    {
        char *name;
        char *equals, *value;
        int tag_num;

        /* s is either NAME or VALUE or NAME=VALUE, whitespace can be anywhere */
        name = skip_space(s);
        value = "";
        equals = strchr(s, '=');

        if (equals)
        {
            *equals = '\0';
            value = equals + 1;
        }

        tag_num = find_tag(tags, name);
        if (tag_num != -1)
        {
            values[tag_num] = skip_space(value);
        }
        else
        {
            /* if searching for the null entry use full NAME=VALUE */
            if (equals)
                *equals = '=';

            tag_num = find_tag(tags, "");
            if (tag_num != -1)
                values[tag_num] = name;
        }
    }
    while ((s = strtok(NULL, SEPARATORS)) != NULL);
}

#endif

/* ---------------------------------------------------------------------------------------------------------- */

char *skip_space(const char *s)
{
    if (s) while (*s && isspace(*s))
	s++;
    return (char *)s;
}

/* ---------------------------------------------------------------------------------------------------- */

/* As originally used.
 * 'src' is a null-terminated string
 * 'dest' is an output buffer of size 'len' bytes.
 */

extern void translate_escaped_text(char *src, char *dest, int len)
{
    int new_len;

    strncpysafe(dest, src, len);

    new_len = sgml_translation(NULL,
			       dest,
			       strlen(dest),
			       SGMLTRANS_PERCENT | SGMLTRANS_HASH );

    dest[new_len] = 0;
}

extern char *strdup_unescaped(const char *src)
{
    char *dest = strdup(src);
    int new_len;

    new_len = sgml_translation(NULL,
			       dest,
			       strlen(dest), SGMLTRANS_PERCENT | SGMLTRANS_HASH );
    dest[new_len] = 0;

    return strtrim(dest);
}


#ifdef STBWEB
extern void translate_escaped_form_text(char *src, char *dest, int len)
{
    int new_len;

    strncpysafe(dest, src, len);

    new_len = sgml_translation(NULL,
			       dest,
			       strlen(dest),
			       SGMLTRANS_PERCENT | SGMLTRANS_HASH | SGMLTRANS_PLUS_TO_SPACE);

    dest[new_len] = 0;
}
#endif


/*****************************************************************************

  share_span_evenly():  the supplied width is shared equally amongst the
  range of items.

  ensure_span_evenly():  On exit, the sum of the widths of the items
  spanned will equal or exceed the width value supplied.

  A negative value indicates the item has a used specified fixed size.  If
  there are items with positive values, then negative items are ignored
  when distributing size.  If only negative values are present, then we
  still need to force the minimum size.

  */

extern void share_span_evenly(int *list, const int start, const int span, const int width)
{
    const int end = start + span;
    int num_neg = 0, num_pos = 0, x, wleft = width, step;

    if (list == NULL || start < 0 || span <= 0 || width <= 0)
	return;

    for (x = start; x < end; x++)
    {
	if (list[x] < 0)
	    num_neg += 1;
	else
	    num_pos += 1;
    }

    TABDBGN(("%d negative items, %d positive items\n", num_neg, num_pos));

    TASSERT(num_neg > 0 || num_pos > 0);

    if (num_neg == 0 && num_pos != 0)
    {       /* Simple case - no notched out columns */
	TASSERT(num_pos == span);
	step = width / num_pos;
	TABDBGN(("Simple case, step %d\n", step));
	/* Course division */
	for (x = start; x < end; x++)
	{
	    list[x] += step;
	    wleft -= step;
	}
	/* Fine division */
	for (x = start; wleft > 0 && x < end ; x++)
	{
	    list[x] += 1;
	    wleft -= 1;
	}
	TASSERT(wleft == 0);
    }
    else if (num_neg != 0 && num_pos != 0)
    {       /* Share amongst columns not notched out */
	step = width / num_pos;
	TABDBGN(("Notched case, step %d\n", step));
	/* Course division */
	for (x = start; x < end; x++)
	{
	    if (list[x] >= 0)
	    {
		list[x] += step;
		wleft -= step;
	    }
	}
	/* Fine division */
	for (x = start; wleft > 0 && x < end ; x++)
	{
	    if (list[x] >= 0)
	    {
		list[x] += 1;
		wleft -= 1;
	    }
	}
	ASSERT(wleft == 0);
    }
    else    /* num_neg != 0 && num_pos == 0 */
    {       /* Only have fixed width columns - force spreading */
	ASSERT(num_neg == span);
	step = width / num_neg;
	TABDBGN(("Only notched case, step %d\n", step));
	/* Course division */
	for (x = start; x < end; x++)
	{
	    list[x] -= step;
	    wleft -= step;
	}
	/* Fine division */
	for (x = start; wleft > 0 && x < end ; x++)
	{
	    list[x] -= 1;
	    wleft -= 1;
	}
	ASSERT(wleft == 0);
    }

#if DEBUG == 3
    dump_span(list + start, span);
#endif
}

extern void ensure_span_evenly(int *list, const int start, const int span, const int width)
{
    const int end = start + span;
    int got, x;

    TABDBG(("ensure_span_evenly(%p, %d, %d, %d)\n", list, start, span, width));

    if (span < 1)
	return;

    for (got = 0, x = start; x < end; x++)
	got += abs( list[x] );

    if (got < width)
	share_span_evenly(list, start, span, width - got);
}

/* sum of all items in list, not getting confused by NOTINIT */

#if 0 /*not called?*/
extern int sum_list(int *ptr, const int num)
{
    int x, sum, *p;

    for (x = 0, sum = 0, p = ptr; x < num; x++, p++)
	if (*p != NOTINIT)
	    sum += *p;

    return sum;
}
#endif

/* Scale non-NOTINIT items to sum to max */

#if 0 /*not called?*/
extern void list_lower(int *ptr, const int num, const int max)
{
    if (num == 0)
	return;

    /* Cheaper recursion */
    while (1)
    {
	int d, x, q, *p;

	TABDBGN(("list_lower(%p, %d, %d)\n", ptr, num, max));

	for (x = 0, q = 0, p = ptr; x < num; x++, p++)
	{
	    if ( *p != NOTINIT )
		q += *p;
	}

	/* How many too large by */
	d = q - max;

	if (d <= 0)
	    return;

	/* Scale factor, assuming no NOTINIT items */
	q = d / num;
	if (q == 0)
	    q = 1;

	for (x = 0, p = ptr; x < num; x++, p++)
	{
	    if (*p != NOTINIT)
	    {
		*p -= q;
		d -= q;
		if (d == 0)
		    return;
	    }
	}

	/* Handle leftover: due to int rounding or NOTINIT items */
	/* Just iterate - equiv to recursing */
    }
}
#endif

/* Print out a list for debugging */

extern void print_list(int *ptr, const int num, const char *pre)
{
    int x;

    TABDBG(("%-10s", pre));

    for (x = 0; x < num; x++, ptr++)
    {
	if (*ptr == NOTINIT)
	{
	    TABDBG(("---- "));
	}
	else
	{
	    TABDBG(("%4d ", *ptr));
	}
    }

    TABDBG(("\n"));
}

/* Set all list items to a particular value */

extern void set_list(int *ptr, const int num, const int val)
{
    int x;

    TABDBGN(("set_list(%p, %d, %d)\n", ptr, num, val));

    for (x = 0; x < num; x++)
	*ptr++ = val;
}

/* Share amt out over the specified range */
/* NOTINIT items will get written to as well */

#if 0 /*not called?*/
extern void range_spread(int *ptr, const int num, const int amt)
{
    int x, q, *p;

    TABDBGN(("range_spread(%p, %d, %d): list before:\n", ptr, num, amt));

    if (num == 0)
	return;

    for (x = 0, q = amt / num, p = ptr; x < num; x++, p++)
    {
	if (*p == NOTINIT)
	    *p = q;
	else
	    *p += q;
    }

    for (x = 0, q = amt - (amt / num), p = ptr; q > 0 && x < num; x++, p++, q--)
    {
	*p += 1;
    }
}
#endif

/* Ensure the specified range accounts for amt */

#if 0 /*not called?*/
extern void ensure_range(int *ptr, const int num, const int amt)
{
    int *p, got, x;

    TABDBGN(("ensure_range(%p, %d, %d):\n", ptr, num, amt));

    for (x = 0, got = 0, p = ptr; x < num; x++, p++)
    {
	if ( *p != NOTINIT )
	    got += *p;
    }

    if (got < amt)
	RANGE_SPREAD(ptr, num, amt - got);
}
#endif

/* Use new value if lower than existing value */

#if 0 /*not called?*/
extern void min_merge(int *lhs, int *rhs, const int num)
{
    int x, *p1, *p2;

    TABDBGN(("min_merge(%p, %p, %d)\n", lhs, rhs, num));

    for (x = 0, p1 = lhs, p2 = rhs; x < num; x++, p1++, p2++)
    {
	if ( *p1 == NOTINIT || *p2 < *p1 )
	    *p1 = *p2;
    }
}
#endif

/* Use new value if higher than existing value */

#if 0 /*not called?*/
extern void max_merge(int *lhs, int *rhs, const int num)
{
    int x, *p1, *p2;

    TABDBGN(("max_merge()\n"));

    for (x = 0, p1 = lhs, p2 = rhs; x < num; x++, p1++, p2++)
    {
	if ( *p1 == NOTINIT || *p2 > *p1 )
	    *p1 = *p2;
    }
}
#endif

/* Ensure all points rightwards are at least a given size */

#if 0 /*not called?*/
extern void ensure_rightwards(int *ptr, /* base of list */
			      const int num, /* items in list */
			      const int from, /* 1st of two items */
			      const int to, /* 2nd of two items */
			      const int amt /* must increase by at least this */
    )
{
    const int d = amt - (ptr[to] - ptr[from]);
    int x;

    if (to == from)
    {
	TABDBG(("ensure_rightwards(%p, %d, %d, %d, %d):\n",
		ptr, num, from, to, amt));
	return;
    }

    ASSERT(to > from);

    if (d <= 0)
	return;

    for (x = to; x < num; x++)
	ptr[x] += d;
}
#endif

/*****************************************************************************/

/*
 * find out whether two boxes overlap and returns the common area in out_box
 * returns TRUE if there is any overlap
 */

BOOL coords_intersection(const wimp_box *box1, const wimp_box *box2, wimp_box *out_box)
{
    wimp_box out;
    out.x0 = MAX(box1->x0, box2->x0);
    out.x1 = MIN(box1->x1, box2->x1);
    out.y0 = MAX(box1->y0, box2->y0);
    out.y1 = MIN(box1->y1, box2->y1);
    if (out_box)
	*out_box = out;
    return out.x1 > out.x0 && out.y1 > out.y0;
}

/*
 * find out whether two boxes overlap and returns a box covering both in out_box
 * returns TRUE if there is any overlap
 * x1 < x0 or y1 < y0 mean empty box so return TRUE and the other box
 */

BOOL coords_union(const wimp_box *box1, const wimp_box *box2, wimp_box *out_box)
{
    wimp_box out;

    if (box1->x1 < box1->x0 || box1->y1 < box1->y0)
    {
	if (out_box)
	    *out_box = *box2;
	return TRUE;
    }

    if (box2->x1 < box2->x0 || box2->y1 < box2->y0)
    {
	if (out_box)
	    *out_box = *box1;
	return TRUE;
    }

    out.x0 = MIN(box1->x0, box2->x0);
    out.x1 = MAX(box1->x1, box2->x1);
    out.y0 = MIN(box1->y0, box2->y0);
    out.y1 = MAX(box1->y1, box2->y1);
    if (out_box)
	*out_box = out;
    return coords_intersection(box1, box2, NULL);
}

/* ---------------------------------------------------------------------------------------------- */

/* Time parsing code nicked from LibWWW, WWWStr.c */

static char * months[12] = {
    "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

/* ---------------------------------------------------------------------------------------------------- */

/*	Date and Time Parser
**	--------------------
**	These functions are taken from the server written by Ari Luotonen
*/

static int make_num (const char *  s)
{
    if (*s >= '0' && *s <= '9')
	return 10 * (*s - '0') + *(s+1) - '0';
    else
	return *(s+1) - '0';
}

static int make_month (const char *  s)
{
    int i;
    for (i=0; i<12; i++)
	if (!strncasecomp(months[i], s, 3))
	    return i;
    return 0;
}

static char *skip_to_num(char *s)
{
    while (*s && !isdigit(*s))
	s++;
    return s;
}

/* ---------------------------------------------------------------------------------------------------- */

/*
**	Parse a str in GMT format to a GMT time time_t representation
**	Three formats are accepted:
**
**		Wkd, 00 Mon 0000 00:00:00 GMT		(rfc1123)
**		Weekday, 00-Mon-00 00:00:00 GMT		(rfc850)
**		00-Mon-00 00:00:00			(oracle old)
**		Weekday, 00-Mon-0000 00:00:00		(oracle new)

**		Wkd Mon 00 00:00:00 0000 GMT		(ctime)
*/

unsigned long HTParseTime (const char *str)
{
    char * s;
    struct tm tm;
    unsigned long t;

    if (!str) return 0;

    if (strchr(str, ',') || strchr(str, '-'))
    {
	s = (char *)str;

	/* decode mday number */
	s = skip_to_num(s);
	tm.tm_mday = (int)strtoul(s, &s, 10);

	/* skip to month name */
	while (*s && !isalpha(*s))
	    s++;

	/* decode year */
	tm.tm_mon = make_month(s);

	/* decode year number */
	s = skip_to_num(s);
	tm.tm_year = (int)strtoul(s, &s, 10);
	if (tm.tm_year < 70)				/* 00 to 69 means 2000 to 2069 */
	    tm.tm_year += 100;				/* 70 to 99 means 1970 to 1999 */
	else if (tm.tm_year > 99)
	    tm.tm_year -= 1900;				/* else it is a four digit year */

	/* skip to time numbers */
	s = skip_to_num(s);
	tm.tm_hour = (int)strtoul(s, &s, 10);

	s = skip_to_num(s);
	tm.tm_min = (int)strtoul(s, &s, 10);

	s = skip_to_num(s);
	tm.tm_sec = (int)strtoul(s, &s, 10);
    }
    else {	      /* Try the other format:  Wed Jun  9 01:29:59 1993 GMT */
	s = (char *)str;
	while (*s && *s==' ') s++;
	if ((int)strlen(s) < 24) {
	    return 0;
	}
	tm.tm_mday = make_num(s+8);
	tm.tm_mon = make_month(s+4);
	tm.tm_year = (100*make_num(s+20) - 1900) + make_num(s+22);
	tm.tm_hour = make_num(s+11);
	tm.tm_min = make_num(s+14);
	tm.tm_sec = make_num(s+17);
    }
    if (tm.tm_sec  < 0  ||  tm.tm_sec  > 59  ||
	tm.tm_min  < 0  ||  tm.tm_min  > 59  ||
	tm.tm_hour < 0  ||  tm.tm_hour > 23  ||
	tm.tm_mday < 1  ||  tm.tm_mday > 31  ||
	tm.tm_mon  < 0  ||  tm.tm_mon  > 11  ||
	tm.tm_year <70  ||  tm.tm_year >120) {
	return 0;
    }

    tm.tm_isdst = 0;

    t = (unsigned long)mktime(&tm);

    return t;
}

/* ---------------------------------------------------------------------------------------------------- */

int find_closest_colour(int colour, const int *palette, int n_entries)
{
    int i, best, besterr;
    int bb, gg, rr;

    best = -1;
    besterr = 0x7fffffff;

    bb = (colour >> 24) & 0xff;
    gg = (colour >> 16) & 0xff;
    rr = (colour >>  8) & 0xff;

    for (i=0; i < n_entries; i++)
    {
	int dist, err;

/* 	IMGDBGN(("col %d, palette 0x%08x\n", i, palette[i])); */

	dist = bb - ((palette[i] >> 24) & 0xff);
	err  = dist * dist;
	dist = gg - ((palette[i] >> 16) & 0xff);
	err += dist * dist;
	dist = rr - ((palette[i] >>  8) & 0xff);
	err += dist * dist;

/* 	IMGDBGN(("Err %d, best err %d, best %d\n", err, besterr, best)); */

	if (err < besterr)
	{
	    besterr = err;
	    best = i;
	}
    }

    return best;
}

/* ----------------------------------------------------------------------------------------------------

   Shuffle an array around.

  +---+
  | A |
  +---+
  | B |
  |   |
  +---+

  base is where array begins
  The size of individual elements is esize - typically sizeof(ptr*), etc
  The x dimension is xsize.
  asize is height of first section.
  bsize is height of second section


 */

extern void shuffle_array(void *base, size_t esize, size_t xsize, size_t asize, size_t bsize)
{
    char *cbase= (char*)base;
    void *stash = mm_malloc(esize * xsize * asize);

    memcpy(stash,
	   base,
	   esize * xsize * asize);

    /* Overlapping copy */
    memmove(base,
	    cbase + esize * xsize * asize,
	    esize * xsize * bsize);

    memcpy(cbase + esize * xsize * bsize,
	   stash,
	   esize * xsize * asize);

    mm_free(stash);
}



/*****************************************************************************/

static input_key_map *key_map = NULL;

extern void set_input_key_map(input_key_map *map)
{
    key_map = map;
}

extern input_key_action lookup_key_action(int key)
{
    input_key_map *ikm = key_map;

    if (ikm == NULL)
	return key_action_NO_ACTION;

    while (ikm->key != -1 && ikm->key != key)
	ikm++;
    return ikm->action;
}

/*****************************************************************************/

char *strcatx_with_leeway(char *s, const char *s1, int leeway)
{
    char *new_s = NULL;
    int slen, actuallen, newlen;
    
    if (s)
    {
	slen = strlen(s);
	actuallen = (slen + 1 + leeway) &~ leeway;
    }
    else
    {
	slen = actuallen = 0;
    }
    
    newlen = slen + (s1 ? strlen(s1) : 0) + 1;

 /*  DBG(("strcatx: s %p s1 %p leeway %d: slen %d s1len %d newlen %d (%d) actuallen %d\n", s, s1, leeway, slen, strlen(s1), newlen, (newlen + leeway) &~ leeway, actuallen)); */

    /* treat empty string as null string */
    if (newlen == 1)
	newlen = 0;

    if (newlen > actuallen)
    {
	new_s = mm_realloc(s, (newlen + leeway) &~ leeway);
	if (!new_s)
	    return s;

	s = new_s;
    }
    
    if (s1 && s)
	strcpy(s + slen, s1);

    return s;
}

#define LEEWAY 63		/* must be power of 2 minus 1 */

char *strcatx1(char *s, const char *s1)
{
    return strcatx_with_leeway(s, s1, LEEWAY);
}

char *strcatx(char *s, const char *s1)
{
    return strcatx_with_leeway(s, s1, 0);
}

char *strtrim(char *s)
{
    return mm_realloc(s, strlen(s)+1);
}

char *xfgets(FILE *in)
{
    char *s = NULL;
    BOOL finished = FALSE;

    do
    {
	int blen;
	char buffer[128];

	if (fgets(buffer, sizeof(buffer), in) == NULL)
	    return s;

	blen = strlen(buffer);
	if (buffer[blen-1] == '\n')
	{
	    buffer[blen-1] = '\0';
	    finished = TRUE;
	}
	
	s = strcatx(s, buffer);

/* 	DBG(("xfgets: read '%s' finished %d gives %s\n", buffer, finished, s)); */
    }
    while (!finished && !feof(in) && !ferror(in));

    return s;
}

void fskipline(FILE *in)
{
    int c;
    do
	c = fgetc(in);
    while (c != EOF && c != '\n');
}

/* ---------------------------------------------------------------------------------------------------- */

#if 0
BOOL quoted_printable_necessary_n(const char *s, int len)
{
    while (len-- && (c = *s++) != 0)
    {
	BOOL encode = (c < 32 && c != 9) || c == 61 || c > 126;
	if (encode)
	    return TRUE;
    }
    return FALSE;
}

BOOL quoted_printable_necessary(const char *s)
{
    return quoted_printable_necessary(s, INT_MAX);
}

/* **** This needs to cope with spaces at the end of lines ****
 *
 * It also assumes that it won't be given CRLF sequences. It won't go wrong
 * if it does it will just generate encoded characters rather than hard line breaks.
 */

void quoted_printable_to_file_n(FILE *f, const char *s, int len, int *pos)
{
    int c;
    int count = pos ? *pos : 0;

    while (len-- && (c = *s++) != 0)
    {
	BOOL encode = (c < 32 && c != 9) || c == 61 || c > 126;

	if (count >= (encode ? 72 : 74))
	{
	    fputs("=\r\n", f);
	    count = 0;
	}

	if (encode)
	{
	    fprintf(f, "=%02X", c);
	    count += 3;
	}
	else
	{
	    fputc(c, f);
	    count++;
	}
    }

    if (pos)
	*pos = count;
}

void quoted_printable_to_file(FILE *f, const char *s, int *pos)
{
    quoted_printable_to_file(f, s, INT_MAX, pos);    
}
#endif

/* ---------------------------------------------------------------------------------------------------- */

/* eof portutil.c */
