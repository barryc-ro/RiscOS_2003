/* genproc.c - generic HTML parser support */

/*#include "sgmlparser.h"*/
#include "htmlparser.h"

#ifdef PLOTCHECK
#include "../plotcheck/rectplot.h"
#endif

static void sgml_print_value(FILE *output, ATTRIBUTE *attribute, VALUE *value);

#ifdef SGML_REPORTING

static char *stack_indentation(SGMLCTX *context)
{
#if defined(HTMLCHECK) || defined(PLOTCHECK)
    return "";
#else
    static char buf[256];

    STACK_ITEM *ip = context->tos;

    while (ip && ip->outer != NULL)
	ip = ip->outer;

    *buf = 0;

    if (ip)
    {
	ip = ip->inner;

	while (ip != NULL)
	{
	    char *a;
	    
	    if (ip == context->tos)
		a = "   ";
	    else if (ip->element < 0)
		a = "---";
	    else
		a = " | ";
	    
	    strcat(buf, a);
	    
	    if (ip == context->tos)
		break;
	    ip = ip->inner;
	}
    }
	
    return buf;
#endif /* HTMLCHECK */
}

static void sgml_print_value(FILE *output, ATTRIBUTE *attribute, VALUE *value)
{
    STRING *sp;

    switch ( value->type )
    {
    case value_none:
	break;
    case value_integer:
	fprintf(output, " %s=%d", attribute->name.ptr, value->u.i);
	break;
    case value_tuple:
	fprintf(output, " %s=#%06X", attribute->name.ptr, 0xffffff & value->u.i);
	break;
    case value_string:
	/* forces "s when might have had '"' */
	fprintf(output, " %s=\"%.*s\"", attribute->name.ptr, value->u.s.nchars, value->u.s.ptr);
	break;
    case value_enum:
	sp = &attribute->templates[value->u.i-1];
	fprintf(output, " %s=%.*s", attribute->name.ptr, sp->nchars, sp->ptr);
	break;
    case value_void:
	fprintf(output, " %s", attribute->name.ptr);
	break;
    case value_absunit:
	fprintf(output, " %s=%d", attribute->name.ptr, (int) ceil(value->u.f) / 2 );
	break;
    case value_relunit:
	fprintf(output, " %s=%g*", attribute->name.ptr, value->u.f );
	break;
    case value_pcunit:
	fprintf(output, " %s=%g%%", attribute->name.ptr, value->u.f );
	break;
    case value_stdunit_list:
    {
	int i;
	fprintf(output, " %s=", attribute->name.ptr);
	for (i = 0; i < value->u.l.num; i++)
	{
	    VALUE *v = &value->u.l.items[i];

	    if (i)
		fprintf(output, ",");

	    switch (v->type)
	    {
	    case value_absunit:
		fprintf(output, "%d", (int) ceil(v->u.f) / 2 );
		break;
	    case value_relunit:
		fprintf(output, "%g*", v->u.f );
		break;
	    case value_pcunit:
		fprintf(output, "%g%%", v->u.f );
		break;
            default:
	        fprintf(output, "<%d>", v->type);
	        break;
	    }
	}
	break;
    }
    default:
	    fprintf(output, "<%d>", value->type);
	    break;
    }
}
#endif /* SGML_REPORTING */



extern void report_start (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
#if SGML_REPORTING
    int ix, j;
    FILE *fp = context->report.output;
    int array[SGML_MAXIMUM_ATTRIBUTES + 1]; /* Might be spare or all used */

    fprintf(fp, "%s<%s", stack_indentation(context), element->name.ptr);
    fflush(context->report.output);

    /* per-sort emission order to avoid boring N**2 overheads (N=20ish) */

    for (ix = 0; ix <= SGML_MAXIMUM_ATTRIBUTES; ix++)
    {
	array[ix] = -1;
    }

    for (ix = 0; ix < SGML_MAXIMUM_ATTRIBUTES; ix++)
    {
	VALUE *value = &attributes->value[ix];
	const int z = value->type == value_none ? -1 : value->tag;

	if (z < 0)
	    continue;

	ASSERT( z >= 0 );
	ASSERT( z <= SGML_MAXIMUM_ATTRIBUTES );
	array[ z ] = ix;
    }

    for (ix = 1; (j = array[ix]) != -1 && ix <= SGML_MAXIMUM_ATTRIBUTES; ix++)
    {
	VALUE *value = &attributes->value[j = array[ix]];
	ATTRIBUTE *attribute = element->attributes[j];

	ASSERT(value->type != value_none); /* I think */

	if (value->type == value_none)
	    continue;

	sgml_print_value(context->report.output, attribute, value);
    }

#if !defined(HTMLCHECK) && !defined(PLOTCHECK)
    fprintf(fp, ">\n");
#else
    fprintf(fp, ">");
#endif

#endif /* SGML_REPORTING */
}

extern void report_finish (SGMLCTX * context, ELEMENT * element)
{
#if SGML_REPORTING
#if !defined(HTMLCHECK) && !defined(PLOTCHECK)
    fprintf (context->report.output, "%s</%s>\n", stack_indentation(context), element->name.ptr);
#else
    fprintf (context->report.output, "</%s>", element->name.ptr);
#endif
#endif
}


extern void generic_start (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
#ifdef RECONS
    if (delivery)
	report_start(context, element, attributes);
#endif
}

extern void generic_finish (SGMLCTX * context, ELEMENT * element)
{
#ifdef RECONS
    if (delivery)
	report_finish(context, element);
#endif
}

