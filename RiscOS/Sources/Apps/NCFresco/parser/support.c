/* support.c - assorted support routines */
/* (C) Copyright ANT Limited 1996. All rights reserved. */

#include "sgmlparser.h"

#ifdef STDALONE
#include "htmlcheck.h"
#endif

#include "gbf.h"
#include "util.h"

STRING empty_string = { NULL, 0 },
    space_string = { " ", 1 },
    eol_string = { "\n", 1};


#define decode_element_start	0x01
#define decode_element_body	0x02
#define decode_attribute_start	0x04
#define decode_attribute_body	0x08
#define decode_value_start	0x10
#define decode_value_body	0x20
#define decode_whitespace	0x40
#define decode_entity		0x80

static char char_decode[256];

/* DL: Not sure the following should be available from this module,
   but it is necessary to define the policy with effects this early.
 */
#include "webfonts.h"

#define STYLE_XOR_MASK (WEBFONT_FLAG_ITALIC << STYLE_WF_INDEX_SHIFT)

#define STYLE_OR_MASK  (~STYLE_XOR_MASK)
/*
                       ((WEBFONT_FLAG_BOLD << STYLE_WF_INDEX_SHIFT) | \
                       (WEBFONT_FLAG_FIXED << STYLE_WF_INDEX_SHIFT) | \
                       (STYLE_BOLD << STYLE_BOLD_SHIFT) | \
                       (STYLE_UNDERLINE << STYLE_UNDERLINE_SHIFT) | \
                       (STYLE_STRIKE << STYLE_STRIKE_SHIFT))
 */

/*****************************************************************************/

#define DEBUG_STACK DEBUG

#if DEBUG

static void dump_elements_open(BITS *bits)
{
    /* DAF: Enable this when wanted - it consumes lots of resources and distorts profiling values. */
#if 1
    int i;

    for (i = 0; i < NUMBER_SGML_ELEMENTS; i++)
    {
	if ( bits[i >> 5] & (1 << (i & 0x1f)) )
	{
	    PRSDBGN((" %.*s", elements[i].name.bytes, elements[i].name.ptr));
	}
    }
#endif
}

extern void dump_stack(SGMLCTX *ctx)
{
    STACK_ITEM *item = ctx->tos;
    char *dir = "vvvv";

#if !DEBUG_STACK
    return;
#endif

    PRSDBGN(("dump_stack(%p): tos %p\n", ctx, item));

    if (item == NULL)
	return;

    /* Go to top (outermost, least recently pushed) of stack */

    while (item->outer != NULL)
	item = item->outer;

    /* Dump stack */

    while (item != NULL)
    {
	if (item == ctx->tos)
	    dir = "    ";
#if 0
	PRSDBGN(("This %p, outer %p(%p), inner %p(%p)\n",
		item, item->outer, &item->outer, item->inner, &item->inner));
#else
	PRSDBGN(("%s %p, fx %08x, outer %p, inner %p: ## %s ##\n",
		dir, item, item->effects_active[0], item->outer, item->inner, ctx->elements[abs (item->element)].name.ptr));
#endif

	PRSDBGN(("%s OPEN:", dir));
	dump_elements_open(item->elements_open);
	PRSDBGN(("\n%s SEEN:", dir));
	dump_elements_open(item->elements_seen);
	PRSDBGN(("\n"));

	if (ctx->tos == item)
	    dir = "^^^^";
	item = item->inner;
    }
}
#endif

/*****************************************************************************/

extern void nullfree(void **vpp)
{
    if (vpp != NULL)
    {
	if (*vpp != NULL)
	{
	    mm_free(*vpp);
	    *vpp = NULL;
	}
    }
}


/*****************************************************************************/

/*	With count limit
**	----------------
*/

extern int strnicmp(const char *a, const char *b, int n)
{
    const char *p =a;
    const char *q =b;

    for(p=a, q=b;; p++, q++) {
	int diff;
	if (p == a+n) return 0;	/*   Match up to n characters */
	if (!(*p && *q)) return *p - *q;
	diff = tolower(*p) - tolower(*q);
	if (diff) return diff;
    }
    /*NOTREACHED*/
}

/*****************************************************************************/

/* FROM STRING TO CHAR* VIA ALLOC */

extern char *stringdup(STRING s)
{
    char *ptr = mm_calloc(1, s.bytes + 1);

    ASSERT(ptr != NULL);

    if (s.bytes)
	memcpy(ptr, (const void *) s.ptr, (size_t) s.bytes);
    ptr[s.bytes] = 0;

    return ptr;
}

/* FROM STRING TO CHAR* VIA ALLOC WITH SPACES STRIPPED */

#if 0 /*notcalled?*/
extern char *strip_stringdup(STRING s)
{
    char *ptr = s.ptr, *x;
    int bytes = s.bytes;

    while (bytes > 0 && *ptr < 33)
    {
	bytes--;
	ptr++;
    }

    while (bytes > 0 && ptr[bytes - 1] < 33)
    {
	bytes--;
    }

    x = mm_calloc(1, bytes + 1);

    if (x != NULL)
    {
	if (bytes)
	    memcpy(x, ptr, bytes);
	x[bytes] = 0;
    }

    return x;
}
#endif


/* From VALUE to malloced string iff of type string */

char *valuestringdup(const VALUE *v)
{
    switch (v->type)
    {
    case value_string:
	return stringdup(v->u.s);
    case value_void:		/* for some items we need to be able to distinguish between "" and NULL, ie empty and not present */
	return mm_calloc(1, 1);
    }
    return NULL;
}

/*****************************************************************************/

/* FROM CHAR* TO STRING VIA ALLOC */

extern STRING mkstring(char *ptr, int n)
{
    STRING s;

    if ( (s.bytes = n) == 0 )
    {
	s.ptr = NULL;
    }
    else
    {
	if ( (s.ptr = mm_malloc(n)) == NULL )
	{
	    usrtrc( "Failed to allocate %d bytes of string storage\n", n);
	    s.bytes = 0;
	}
	else
	{
	    memcpy(s.ptr, ptr, n);
	}
    }

    return s;
}

/*****************************************************************************/

/* STRING, STRING TO STRING, VIA ALLOC. NO FREES! */

extern STRING stringcat(STRING a, STRING b)
{
    STRING r;

    r.bytes = a.bytes + b.bytes;
    r.ptr = mm_malloc(r.bytes);

    if (r.ptr == NULL)
    {
	usrtrc( "stringcat(): not enough memory\n");
	r.bytes = 0;
    }
    else
    {
	memcpy( (char*)r.ptr, a.ptr, a.bytes );
	memcpy( (char*)r.ptr + a.bytes, b.ptr, b.bytes );
    }

    return r;
}

/*****************************************************************************/

#if 0 /*not called?*/
STRING string_skip_chars(STRING s, const char *p)
                                        /* find first char in s not in p */
{
    STRING out;
    out.ptr = s.ptr;
    out.bytes = s.bytes;
    for (; out.bytes; out.ptr++, out.bytes--)
    {
	const char *pp;
        for ( pp = p; ; )
        {
	    char c1 = *pp++;
            if (c1 == 0)
		return out;
            if (c1 == *out.ptr)
		break;
        }
    }
    return out;
}
#endif

#if 0 /*not called?*/
STRING string_skip_to_chars(STRING s, const char *p)
                                        /* find first char in s not in p */
{
    STRING out;
    out.ptr = s.ptr;
    out.bytes = s.bytes;
    for (; out.bytes; out.ptr++, out.bytes--)
    {
	const char *pp;
	char c1;
        for ( pp = p; (c1 = *pp++) != 0; )
            if (c1 == *out.ptr)
		return out;
    }
    return out;
}
#endif

#if 0 /*not called?*/
STRING stringtok(STRING *s1, const char *s2)
{
    STRING s, ss;

    s = string_skip_chars(*s1, s2);
    if (s.bytes == 0)
    {
	*s1 = s;
	return s;
    }

    /* ss is the remaining string */
    ss = string_skip_to_chars(s, s2);
    *s1 = ss;

    /* set length of return */
    s.bytes = ss.ptr - s.ptr;

    return s;
}
#endif


/*****************************************************************************/

/* strip spaces from the start of a string, modifying the STRING */

extern STRING string_strip_start(STRING s)
{
    while (s.bytes > 0 && isspace( (int) s.ptr[0] ) )
    {
        s.ptr++;
	s.bytes--;
    }
    return s;
}

/* strip spaces from the end of a string, modifying the STRING */

extern STRING string_strip_end(STRING s)
{
    while (s.bytes > 0 && isspace( (int) s.ptr[s.bytes - 1] ) )
	s.bytes--;
    return s;
}

extern STRING string_strip_space(STRING in)
{
    return string_strip_end(string_strip_start(in));
}

/* count occurrences of character c */

#if 0 /*not called?*/
extern int string_count_elements(STRING s, int c)
{
    const char *ss;
    int n, i;

    n = 0;
    for (ss = s.ptr, i = 0; i < s.bytes; ss++, i++)
        if (*ss == c)
            n++;

    return n;
}
#endif

/* int strcspn(char *s, char *sep) returns the length of initial segment of chars not from sep */
/* int strspn(char *s, char *sep) returns the length of initial segment of chars from sep */

extern int string_count_tokens(STRING s, char *breaks)
{
    char *ss;
    int n, i;

    n = 0;
    ss = s.ptr;

    do
    {
	ss += strspn(ss, breaks);

	i = strcspn(ss, breaks);
	if (i)
	    n++;
	ss += i;
    } while (ss < s.ptr + s.bytes);

    return n;
}

/*****************************************************************************/

extern void string_free(STRING *ptr)
{
    nullfree((void**)&ptr->ptr);
    ptr->bytes = 0;
}

extern void string_list_free(STRING_LIST *ptr)
{
    STRING_LIST *p, *p2;

    for(p = ptr; p; p = p2)
    {
	p2 = p->prev;		/* SJM: Changed from next */

	string_free(&(p->string));
	mm_free(p);
    }
}

/*****************************************************************************/

/*
 * Take 'item', append to 'inhand' and expand its tabs
 * Returns a new malloced string
 */

STRING get_tab_expanded_string(STRING item, STRING inhand)
{
    STRING t;
    int i, extra = inhand.bytes;

    for (i = 0; i < item.bytes; i++)
    {
	extra++;
	    
	if (item.ptr[i] == '\t')
	{
	    PRSDBG(("Performing tab expansion\n"));
	    while ( (extra & 7) != 0 )
		extra++;
	}
    }

    t.bytes = extra;
    t.ptr = mm_malloc(extra + 1);

    if (inhand.bytes)
	memcpy(t.ptr, inhand.ptr, inhand.bytes);
    extra = inhand.bytes;

    for (i = 0; i < item.bytes; i++)
    {
	if (item.ptr[i] == '\t')
	{
	    t.ptr[extra++] = ' ';
	    while ( (extra & 7) != 0 )
		t.ptr[extra++] = ' ';
	}
	else
	{
	    t.ptr[extra++] = item.ptr[i];
	}
    }

    t.ptr[extra] = 0;

    return t;
}

/*****************************************************************************/

static void set_char_decode(char * chars, BITS pattern)
{
	while (*chars)
		char_decode[(int) *chars++] |= pattern;
}


extern void sgml_support_initialise(void)
{
    PRSDBGN(("sgml_support_initialise()\n"));

    memset( char_decode, 0, sizeof(char_decode) );

    set_char_decode(
	"\t\r\n ",
	decode_whitespace );
    set_char_decode(
	"0123456789",
	decode_value_start |
	decode_element_body |
	decode_attribute_body |
	decode_value_body |
	decode_entity );
    set_char_decode(
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
	decode_element_start |
	decode_attribute_start |
	decode_value_start |
	decode_element_body |
	decode_attribute_body |
	decode_value_body |
	decode_entity );
    set_char_decode(
	"-._",
	decode_element_body |
	decode_attribute_body |
	decode_value_start |	/* added this as SIZE=-1, HREF=./home.html and TARGET=_self are all common */
	decode_value_body );
#if 1
    /* quotes are not used when they should so accept anything non harmful */
    /*  -._ are handled above */
    /* > is obviously bad */
    set_char_decode(
	"!\"#$%&'()*+,/:;=?"	/* Note quotes are there deliberately as sites use them... */
				/* Borris thinks this is wrong. */
	"@[\\]^"
	"`{|}~",
	decode_value_start |
	decode_value_body);
#else
    set_char_decode(
	"%@.,~;*+/-#[]",
	decode_value_start |
	decode_value_body);
    set_char_decode(
	":?",
	decode_value_body);

    /* SJM: _ has to be able to start attribute values for people who forget the quotes around frame names _top etc. */
    set_char_decode(
	"_",
	decode_value_start );
#endif
    set_char_decode(
	"#",
	decode_entity );

    sgml_do_parser_fixups();
}

extern BOOL is_whitespace(char c)
{
    return char_decode[(int)c] & decode_whitespace;
}

extern BOOL is_element_start_character(char c)
{
    return char_decode[(int)c] & decode_element_start;
}

extern BOOL is_element_body_character(char c)
{
    return char_decode[(int)c] & decode_element_body;
}

extern BOOL is_attribute_start_character(char c)
{
    return char_decode[(int)c] & decode_attribute_start;
}

extern BOOL is_attribute_body_character(char c)
{
    return char_decode[(int)c] & decode_attribute_body;
}

extern BOOL is_value_start_character(char c)
{
    return char_decode[(int)c] & decode_value_start;
}

extern BOOL is_value_body_character(char c)
{
    return char_decode[(int)c] & decode_value_body;
}

extern BOOL is_entity_character(char c)
{
    return char_decode[(int)c] & decode_entity;
}


/*****************************************************************************/

extern BOOL element_bit_clear(BITS *elems, int tag)
{
    const int word = tag >> 5, bit = 1 << (tag & 0x1f);
    return (elems[word] & bit) == 0;
}

extern BOOL element_bit_set(BITS *elems, int tag)
{
    const int word = tag >> 5, bit = 1 << (tag & 0x1f);
    return (elems[word] & bit) != 0;
}

extern void element_clear_bit(BITS *elems, int tag)
{
    const int word = tag >> 5, bit = 1 << (tag & 0x1f);
    elems[word] &= ~bit;
}

extern void element_set_bit(BITS *elems, int tag)
{
    const int word = tag >> 5, bit = 1 << (tag & 0x1f);
    elems[word] |= bit;
}

extern void element_bitset_or(BITS *elem_inout, BITS *elem_in, const size_t n_elems)
{
    int x;

    for (x = 0; x < n_elems; x++)
	elem_inout[x] |= elem_in[x];
}

/*****************************************************************************

  Element matching now does a best-guess, much as attribute matching does.

 */

#if 0
extern int string_str_cmp(const void *a, const void *b)
{
    STRING *ap = (STRING *)a;
    char *bp = (char *)b;

    return strnicmp(ap->ptr, bp, ap->bytes);
}
#endif

static int element_search_fn(const void *a, const void *b)
{
    STRING *sp = (STRING *)a;
    ELEMENT *ep = (ELEMENT *)b;
    const int sl = sp->bytes;
    const int el = ep->name.bytes;
    const int n = strnicmp(sp->ptr, ep->name.ptr, sl < el ? sl : el );

    if (n == 0 && sl != el)
    {
	/* Spurious match due to partial length */

	if (sl < el)
	    return -1;
	else
	    return 1;
    }

    return n;
}

extern int find_element(SGMLCTX *context, STRING s)
{
    ELEMENT *element;

    element = bsearch(&s,
		      context->elements,
		      NUMBER_SGML_ELEMENTS,
		      sizeof(ELEMENT),
		      element_search_fn);

    if (gbf_active(GBF_GUESS_ELEMENTS))
    {
	if (element == NULL)
	{
	    /* Linear search, but only in user-error cases */

	    int best_ix, best_len, ix;

	    PRSDBGN(("Attempting to guess-match '%.*s'\n", s.bytes, s.ptr));

	    for (ix = 0, element = context->elements, best_ix = best_len = -1;
		 ix < NUMBER_SGML_ELEMENTS;
		 ix++, element++)
	    {
		int x;

		for (x = 1; x <= s.bytes; x++)
		{
		    if (x > element->name.bytes)
			break;

		    if ( strnicmp(element->name.ptr, s.ptr, x) != 0 )
			continue;

		    PRSDBG(("%.*s against %.*s, x %d, best_ix %d, best_len %d\n",
			    s.bytes, s.ptr,
			    element->name.bytes, element->name.ptr,
			    x, best_ix, best_len));

		    if (x > best_len)
		    {
			best_len = x;
			best_ix = ix;
		    }
		}
	    }

	    if (best_ix != -1)
	    {
		element = &context->elements[best_ix];
		PRSDBGN(("Guessed '%.*s' best matches '%.*s'\n",
			 element->name.bytes, element->name.ptr, s.bytes, s.ptr));
	    }
	    else
	    {
		PRSDBGN(("No guess what element '%.*s' is meant to be\n", s.bytes, s.ptr));
	    }
	}
    }

    return element == NULL ? SGML_NO_ELEMENT : element->id;
}

/*****************************************************************************

  For a given element, which attribute BEST matches the one spelt by
  the user. We should always return some element if the 1st character
  of the users spelling is also a 1st character of a valid attribute
  to the element supplied. As the alternative is to simply ignore
  unknown attributes, this is believed to be beneficial in nearly all
  circumstances. Note that this might cause us to match the wrong
  attribute if we encounter HTML to a later standard: eg imagine
  <FONT> gets the CORPUS attribute. We don't know this and will
  believe it is COLOR. This SHOULD be safe, as browsing is meant to be
  a non-destructive operation. However, keep a note of caution!

  */

extern int find_attribute(SGMLCTX *context, ELEMENT *element, STRING s, BOOL *guessed)
{
    int ix = 0;
    ATTRIBUTE **attributep = element->attributes, *attribute;
/*     int len; */

    while ( (attribute = *attributep)->name.ptr != NULL )
    {
	if ( s.bytes == attribute->name.bytes && strnicmp(s.ptr, attribute->name.ptr, s.bytes) == 0 )
	{
	    PRSDBGN(("Attribute '%.*s' is %d\n", s.bytes, s.ptr, ix));

	    *guessed = FALSE;
	    return ix;
	}

	ix++;
	attributep++;
    }

    if (gbf_active(GBF_GUESS_ATTRIBUTES))
    {
	int dist;
	PRSDBGN(("Attribute '%.*s' does not match - guessing\n", s.bytes, s.ptr));

#if 1
        /* pdh: less keen on giving out-of-control matches */
	/* sjm: even less keen, tries distance 1 before 2 */
	for (dist = 1; dist <= 2; dist++)
	{
	    attributep = element->attributes;
	    ix = 0;
	    while ( (attribute = *attributep)->name.ptr != NULL )
	    {
		if ( strnearly( s.ptr, s.bytes,
				attribute->name.ptr, attribute->name.bytes,
				dist ) )
		{
		    *guessed = TRUE;
		    return ix;
		}
		ix++;
		attributep++;
	    }
        }
#else
	for (len = s.bytes; len > 0; len--)
	{
	    attributep = element->attributes;
	    ix = 0;

	    while ( (attribute = *attributep)->name.ptr != NULL )
	    {
		if ( strnicmp(s.ptr, attribute->name.ptr, len) == 0 )
		{
		    PRSDBGN(("Guessed attribute '%.*s' is %d ('%.*s')\n",
			     s.bytes, s.ptr, ix, attribute->name.bytes, attribute->name.ptr));

		    *guessed = TRUE;
		    return ix;
		}

		ix++;
		attributep++;
	    }
	}
#endif

	PRSDBGN(("Failed to make any form of guess on the attribute!\n"));
    }

    return SGML_NO_ATTRIBUTE;
}

/*****************************************************************************/

extern void clear_stack_item(STACK_ITEM *stack)
{
    /*PRSDBGN(("clear_stack_item(%p)\n", stack));*/
    stack->element = SGML_NO_ELEMENT;
    stack->matching_mode = -1;	/* Invalid matching mode please */
    memset( &stack->elements_open, 0, sizeof(stack->elements_open) );
    memset( &stack->elements_seen, 0, sizeof(stack->elements_seen) );
    memset( &stack->effects_active, 0, sizeof(stack->effects_active) );
    memset( &stack->effects_applied, 0, sizeof(stack->effects_applied) );
}

extern void clear_stack(SGMLCTX *context)
{
    STACK_ITEM *tos = context->tos;

    ASSERT(context->magic == SGML_MAGIC);

    PRSDBGN(("clear_stack(%p)\n", context));

    if (tos == NULL)
    {
	PRSDBG(("clear_stack(): created an initial stack item\n"));
	tos = context->tos = mm_calloc(1, sizeof(*tos));
    }

    while (tos != NULL)
    {
	clear_stack_item(tos);
	ASSERT(tos != tos->inner);
	if (tos->inner != NULL)
	{
	    ASSERT(tos->inner->outer == tos);
	}
	tos = tos->inner;
    }

    tos = context->tos;

    if (tos != NULL)
    {
	while (tos->outer != NULL)
	{
	    clear_stack_item(tos->outer);
	    ASSERT(tos != tos->outer);
	    if (tos->outer != NULL)
	    {
		ASSERT(tos == tos->outer->inner);
	    }
	    tos = tos->outer;
	}

	context->tos = tos;
    }

    /* This is probably the best thing to do */
    context->tos->matching_mode = match_unexpected;
}

/*****************************************************************************/

extern void sgml_free_stack(STACK_ITEM *item)
{
    if (item == NULL)
    {
	return;
    }

    for (; item->outer != NULL; item = item->outer)
    {
	;
    }

    while (item != NULL)
    {
	STACK_ITEM *next = item->inner;
	mm_free(item);
	item = next;
    }
}

/*****************************************************************************/

extern void reset_lexer_state(SGMLCTX *context)
{
    PRSDBGN(("reset_lexer_state(%p)\n", context));

    ASSERT(context->magic == SGML_MAGIC);

    clear_stack(context);
    /*clear_elements_seen(context);*/
    context->apply_heuristics = FALSE;
}

#if DEBUG /*not called? - debugging and htmlcheck *can* use this */
extern char *elements_name(SGMLCTX *context, int ix)
{
    static char buf[32];

    if (ix < 0 || ix > NUMBER_SGML_ELEMENTS)
    {
	return "** BOGUS **";
    }

    sprintf(buf, "<%s>", context->elements[ix].name.ptr);
    return buf;
}
#endif

/*****************************************************************************/

extern void push_stack(SGMLCTX *context, ELEMENT *element)
{
    STACK_ITEM *from = context->tos, *to, *ta, *tb;

    PRSDBGN(("push_stack(%p, %s)\n", context, element->name.ptr));

#if 0
    dump_stack(context);
#endif

    ASSERT(from != NULL);

    if (from->inner == NULL)
    {
	from->inner = mm_calloc(1, sizeof(STACK_ITEM));
	/*PRSDBGN(("push_stack(): creating new stack item %p\n", from->inner));*/
	clear_stack_item(from->inner);
	from->inner->outer = from;
	from->inner->inner = NULL;
    }
    else
        memset( &from->inner->effects_applied, 0, sizeof(&from->inner->effects_applied) );

    to = from->inner;

#if 0
    PRSDBGN(("push_stack(): to %p, from %p, to->inner %p, to->outer %p, from->inner %p, from->outer %p\n",
	    to, from, to->inner, to->outer, from->inner, from->outer));
#endif

    ASSERT(to->outer == from);

    ta = to->inner;		/* NULL if new bottom of stack */
    tb = to->outer;
    *to = *from;                /* <--- Old stack item gets copied into new stack item */
    memset( &to->effects_applied, 0, sizeof(&to->effects_applied) );
    to->element = element->id;
    to->inner = ta;
    to->outer = tb;

    context->tos = to;

#if DEBUG
    PRSDBGN(("push_stack(): tos %p, inner %p, outer %p\n", to, to->inner, to->outer));
    dump_stack(context);
#endif
}

extern void pop_stack(SGMLCTX *context)
{
    STACK_ITEM *new_tos = context->tos->outer;

    PRSDBGN(("pop_stack(%p): tos %p, inner %p, outer %p\n",
	     context, context->tos, context->tos->inner, context->tos->outer));

    ASSERT(context->tos != NULL);

    if (new_tos != NULL)
    {
	int ix;

	for (ix = 0; ix < words_of_elements_bitpack; ix++)
	{
	    new_tos->elements_seen[ix] |= context->tos->elements_seen[ix];
	}

	if (new_tos != NULL)
	{
	    context->tos = new_tos;
	}
	else
	{
	    PRSDBGN(("pop_stack(): Avoiding setting tos to NULL\n"));
	}
    }
    else
    {
	PRSDBG(("stack underflow\n"));
	sgml_note_stack_underflow(context);
    }

#if DEBUG
    new_tos = context->tos;
    PRSDBGN(("pop_stack(): tos %p, inner %p, outer %p\n",
	     new_tos, new_tos->inner, new_tos->outer));
    dump_stack(context);
#endif
}

/*****************************************************************************/

extern STACK_ITEM *find_element_in_stack (SGMLCTX *context, ELEMENT *element)
{
    STACK_ITEM *tos;

    for (tos = context -> tos; tos != NULL; tos = tos->outer)
        if (tos->element == element->id) break;

    return tos;
}

static void apply_effects (BITS *new, BITS *old, BITS *fx, BITS *app, BITS *mask)
{
    int i;
    new[0] = (new[0] & ~mask[0]) | ((old[0] | (fx[0] & app[0])) & STYLE_OR_MASK  & mask[0])
                                 | ((old[0] ^ app[0]) & STYLE_XOR_MASK & mask[0]);
    for (i = 1; i < words_of_effects_bitpack; i++)
    {
        new[i] = (new[i] & ~mask[i]) | ((old[i] | (fx[i] & app[i])) & mask[i]);
    }
}

static void apply_effects_without (STACK_ITEM *item, STACK_ITEM *tos)
/*  This describes the logic for correcting the effect of out of order closes.
    At this stage, it copes only with boolean flags and propagates
    these forward as if the item had not been there.
 */
{
    BITS *prior = item->outer->effects_active;
    BITS *mask  = item->effects_applied;
    STACK_ITEM *next;
    for (next=item->inner; next != NULL && next->outer != tos; next=next->inner)
    {
        BITS *this  = next->effects_active;
#if DEBUG_STACK
        PRSDBG(("Before apply_effects: prior %08x this %08x fx %08x app %08x mask %08x\n",
                 prior[0], this[0], next->effects_active[0], next->effects_applied[0], mask[0]));
#endif
        apply_effects (this, prior, next->effects_active, next->effects_applied, mask);
#if DEBUG_STACK
        PRSDBG(("After apply_effects: this: %08x\n", this[0]));
#endif

        prior = this;
    }
}

extern void pull_stack_item_to_top (SGMLCTX *context, STACK_ITEM *item)
{
    ASSERT (context->tos != NULL);

    PRSDBG(("pull_stack_item_to_top: %s\n", elements_name(context, item->element)));

    if (item != NULL && item != context->tos)
    {
        STACK_ITEM *tos   = context->tos;
        STACK_ITEM *inner = tos->inner;
        STACK_ITEM *above = item->outer;
        STACK_ITEM *below = item->inner;
	BOOL do_clear = FALSE, do_or = FALSE;

#if DEBUG
	PRSDBG(("item: %s\n", elements_name(context, item->element)));
	if (tos) PRSDBG(("tos: %s\n", elements_name(context, tos->element)));
	if (inner) PRSDBG(("inner: %s\n", elements_name(context, inner->element)));
	if (above) PRSDBG(("above: %s\n", elements_name(context, above->element)));
	if (below) PRSDBG(("below: %s\n", elements_name(context, below->element)));

#endif

	if (elements[item->element].flags & FLAG_CONTAINER)
	{
	    PRSDBG(("pull_stack_item_to_top: going past container - orring flags\n"));
	    do_or = TRUE;
	}

	if (item->outer == NULL)
	{
	    PRSDBG(("pull_stack_item_to_top: no outer, so do_clear is set\n"));
	    do_clear = TRUE;
	}
	else if ( element_bit_set(item->outer->elements_open, item->element) )
	{
	    PRSDBG(("pull_stack_item_to_top: item is open in outer as well - do_clear is clear\n"));
	}
	else
	{
	    PRSDBG(("pull_stack_item_to_top: item is closed in outer - do_clear is set\n"));
	    do_clear = TRUE;
	}

	if (do_or)
	{
	    /* Should only ever be non-NULL, but can be safer for
               production code */
	    BITS *in = item->outer == NULL ?
		tos->elements_open :
		item->outer->elements_open;

#if DEBUG
	    ASSERT(item->outer != NULL);
	    PRSDBG(("LHS ORR bitset: "));
	    dump_elements_open(tos->elements_open);
	    PRSDBG(("\nRHS ORR bitset: "));
	    dump_elements_open(in);
#endif
	    element_bitset_or(tos->elements_open, 
			      in,
			      sizeof(item->elements_open) / sizeof(item->elements_open[0]) );
#if DEBUG
	    PRSDBG(("\nYields        : "));
	    dump_elements_open(tos->elements_open);
	    PRSDBG(("\n"));
#endif

	}

	if (do_clear)
	{
#if DEBUG
	    ASSERT(item->outer != NULL);
	    PRSDBG(("LHS CLR bitset: "));
	    dump_elements_open(tos->elements_open);
	    PRSDBG(("\nRHS CLR bitset: %s\nYields       :", elements[item->element].name.ptr));
#endif
	    element_clear_bit(tos->elements_open, item->element);
#if DEBUG
	    dump_elements_open(tos->elements_open);
	    PRSDBG(("\n"));
#endif
	}

        /*  We must get the elements_seen vaguely right - since
            the close appears instead of the expected we can use the
            TOS context to get the effect of elements_seen if everything
            had correctly unstacked. This will not be correct where there
            is a 'post clear seen' flag intervening, but if it was relevant
            it would be hard to make sense of it anyway.
         */

        memcpy (item->elements_seen, tos->elements_seen, sizeof (item->elements_seen));
#if DEBUG_STACK
        PRSDBG(("Stack before pull of %s (%p):\n", elements[item->element].name.ptr, item));
        dump_stack (context);
#endif
        if (above != NULL) above->inner = below;
        if (below != NULL) below->outer = above;

        item->outer = tos;
        item->inner = inner;

        tos->inner = item;

        if (inner != NULL) inner->outer = item;

        context->tos = item;
#if DEBUG_STACK
        PRSDBG(("Stack after pull:\n"));
        dump_stack (context);
#endif
    }
}

extern void pull_stack_item_to_top_correcting_effects (SGMLCTX *context, STACK_ITEM *item)
{
    ASSERT (context->tos != NULL);

    PRSDBG(("pull_stack_item_to_top_correcting_effects: %s\n", elements_name(context, item->element)));

    if (item != NULL && item != context->tos)
    {
        memcpy (item->effects_active, context->tos->effects_active, sizeof (item->effects_active));
        apply_effects_without (item, context->tos);

        pull_stack_item_to_top (context, item);
    }

    PRSDBGN(("pull_stack_item_to_top_correcting_effects: afterwards:\n"));
    /*dump_stack(context);*/
}


/*****************************************************************************/

extern void clear_inhand(SGMLCTX *context)
{
    if ( context->inhand.data == NULL )
    {
	PRSDBG(("clear_inhand(%p): creating initial inhand buffer\n", context));
	context->inhand.data = (char *) mm_malloc(256);
	context->inhand.max = 256;
	ASSERT(context->inhand.data != NULL);
    }
    context->inhand.ix = 0;
}

extern void reset_tokeniser_state(SGMLCTX *context)
{
    PRSDBG(("reset_tokeniser_state(%p)\n", context));
    context->line = 1;
    clear_inhand(context);
    context->state = state_markup_only;
    /* This default is correct, even it HotList doesn't want it */
    /* for scanning text.! */
    context->tos->matching_mode = match_unexpected;
}

/*****************************************************************************/

extern void free_buffer(BUFFER *bp)
{
    if (bp != NULL)
    {
	nullfree((void**)&bp->data);
	bp->max = 0;
	bp->ix = 0;
    }
}

/*****************************************************************************/

extern void add_to_buffer(BUFFER *buffer, char input)
{
    ASSERT( buffer->max >= buffer->ix );
    ASSERT( buffer->ix >= 0 );

    if (buffer->max == buffer->ix)
    {
	char *newptr = mm_realloc( buffer->data, buffer->max + 256 );
	if (newptr == NULL)
	{
	    usrtrc( "Not enough memory to extend a string\n");
	    return;
	}
	buffer->data = newptr;
	buffer->max += 256;
    }

    buffer->data[ buffer->ix++ ] = input;
}

extern void add_char_to_inhand(SGMLCTX *context, char input)
{
    ASSERT(context->magic == SGML_MAGIC);

#if 1
    add_to_buffer(&context->inhand, input);
#else
    ASSERT( context->inhand.max >= context->inhand.ix );
    ASSERT( context->inhand.ix >= 0 );

    if ( context->inhand.max == context->inhand.ix )
    {
	char *newptr = mm_realloc( context->inhand.data, context->inhand.max + 256 );
	ASSERT( newptr != NULL );
	context->inhand.data = newptr;
	context->inhand.max += 256;
    }

    context->inhand.data[ context->inhand.ix++ ] = input;
#endif
#if 0
    fprintf(stderr, "Inhand now '%.*s' %d\n",
	    context->inhand.ix, context->inhand.data, context->inhand.ix);
#endif
}

extern void push_inhand(SGMLCTX *context)
{
    STRING s;

    ASSERT(context->magic == SGML_MAGIC);

    s.ptr = context->inhand.data;
    s.bytes = context->inhand.ix;

    if (context->strip_initial_newline && s.bytes > 0 && (s.ptr[0] == '\n' || s.ptr[0] == '\r'))
    {
	s.ptr++;
	s.bytes--;
    }

    if (context->strip_final_newline && s.bytes > 0 && (s.ptr[s.bytes-1] == '\n' || s.ptr[s.bytes-1] == '\r'))
    {
	s.bytes--;
    }

    /* chopper doesn't free strings itself */
    /* Avoid empty strings when might not be appropriate */
    /* to flush chopper state */
    if (s.bytes > 0)
	(*context->chopper) (context, s);

    context->inhand.ix = 0;
}

extern void push_bar_last_inhand(SGMLCTX *context)
{
    ASSERT(context->magic == SGML_MAGIC);

    if (context->inhand.ix > 1)
    {
	STRING s;

	s.ptr = context->inhand.data;
	s.bytes = context->inhand.ix - 1;

	(*context->chopper) (context, s);

	context->inhand.data[0] = context->inhand.data[context->inhand.ix - 1];
	context->inhand.ix = 1;
    }

}

/*****************************************************************************/

#if 0
#if DEBUG
#if PARSE_DEBUG > 0

extern void sgml_parsedebug(char *fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);
    vfprintf(stderr, fmt, arglist);
    fflush(stderr);
    va_end(arglist);
}

#endif
#endif
#endif

/*****************************************************************************/

/* Hard coded machine architecture details! */

extern void pack_fn(BITS *ptr, unsigned shift, unsigned mask, BITS value)
{
    TASSERT( (shift >> 5) < words_of_effects_bitpack );

    ptr += shift >> 5;
    shift &= 0x1f;

    *ptr &= ~(mask << shift);
    *ptr |= value << shift;
}

extern BITS unpack_fn(BITS *ptr, unsigned shift, unsigned mask)
{
    BITS r;

    TASSERT( (shift >> 5) < words_of_effects_bitpack );

    ptr += shift >> 5;
    shift &= 0x1f;

    r = *ptr;

    r >>= shift;
    r &= mask;

    return r;
}

extern void set_effects_fn (STACK_ITEM *st, unsigned shift, unsigned mask, BITS value)
{
    pack_fn (st->effects_active, shift, mask, value);
    pack_fn (st->effects_applied, shift, mask, mask);
#if 0
    PRSDBGN(("set_effects_fn(): st: %p shift: %d mask: %08x value: %08x\n -> %08x %08x %08x\n",
             st, shift, mask, value, st->effects_active[0], st->effects_active[1], st->effects_active[2]));
#endif
}

extern void set_effects_wf_flag_fn (STACK_ITEM *st, BITS value)
{
    BITS v;
    v = unpack_fn (st->effects_active, STYLE_WF_INDEX_SHIFT, STYLE_WF_INDEX_MASK);
    v |= (value & (STYLE_OR_MASK >> STYLE_WF_INDEX_SHIFT));
    v ^= (value & (STYLE_XOR_MASK >> STYLE_WF_INDEX_SHIFT));
    pack_fn (st->effects_active, STYLE_WF_INDEX_SHIFT, STYLE_WF_INDEX_MASK, v);
    pack_fn (st->effects_applied, STYLE_WF_INDEX_SHIFT, STYLE_WF_INDEX_MASK, value);
#if 0
    PRSDBGN(("set_effects_wf_flag_fn(): st: %p value: %08x\n -> %08x %08x %08x\n",
             st, value, st->effects_active[0], st->effects_active[1], st->effects_active[2]));
#endif
}

/*****************************************************************************/

/* eof support.c */
