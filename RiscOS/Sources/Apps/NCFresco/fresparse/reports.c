/* reports.c - Record and emit reports from parsing */
/* (C) Copyright ANT Limited 1996. All rights reserved. */

/* CHANGELOG
 * 19/07/06: SJM: changed some variable names to stop compiler wingeing
 */

#include <string.h>
#include <stdarg.h>

#ifdef STDALONE
#include "sgmlparser.h"
#include "htmlcheck.h"
#else
#include "memwatch.h"
#include "htmlparser.h"
#endif

/* This is all mandated on the caller restricting the maximum output size */
/* to MAXSTRING bytes.   If necessary, use things like %1024s to do this. */

#if SGML_REPORTING

static STRING print(const char *fmt, va_list arg)
{
	STRING result;

	result.nchars = MAXSTRING + 128;
	result.ptr = mm_calloc(1, result.nchars);

	vsprintf(result.ptr, fmt, arg);

	result.nchars = strlen( (const char *) result.ptr) + 1;
	result.ptr = mm_realloc(result.ptr, result.nchars);

	return result;
}

extern void my_sgml_reset_report(SGMLCTX * context)
{
    context->report.has_tables = FALSE;
    context->report.has_forms = FALSE;
    context->report.has_frames = FALSE;
    context->report.has_scripts = FALSE;
    context->report.has_address = FALSE;
        
    context->report.antiquated_features = 0;
    context->report.badly_formed_comments = 0;
    context->report.stack_overflows = 0;
    context->report.stack_underflows = 0;
    context->report.unexpected_characters = 0;

    memset( &context->report.good_opens , 0, sizeof(context->report.good_opens) );
    memset( &context->report.good_closes , 0, sizeof(context->report.good_closes) );
    memset( &context->report.implied_opens , 0, sizeof(context->report.implied_opens) );
    memset( &context->report.implied_closes , 0, sizeof(context->report.implied_closes) );
    memset( &context->report.missing_opens , 0, sizeof(context->report.missing_opens) );
    memset( &context->report.missing_closes , 0, sizeof(context->report.missing_closes) );
    memset( &context->report.close_without_opens , 0, sizeof(context->report.close_without_opens) );
    memset( &context->report.had_unknown_attributes , 0, sizeof(context->report.had_unknown_attributes) );
    memset( &context->report.had_bad_attributes , 0, sizeof(context->report.had_bad_attributes) );
    memset( &context->report.not_repeatables , 0, sizeof(context->report.not_repeatables) );

    context->report.messages = NULL;
    context->report.unknown_elements = NULL;
    context->report.unknown_attributes = NULL;
    context->report.bad_attributes = NULL;
}

/*****************************************************************************/

static void print_bool (SGMLCTX *context, BOOL b, char *msg, char *pre, char *post)
{
    if (b)
    {
	fprintf(context->report.output, "%s%-64s%s", pre, msg, post);
    }
}

static void print_int (SGMLCTX *context, int i, char *msg, char *pre, char *post)
{
    if (i == 0)
    {
	;
    }
    else if (i == 1)
    {
	fprintf(context->report.output, "%s   1 %-59s%s", pre, msg, post);
    }
    else
    {
	char buffer[80];
	sprintf(buffer, "%ss", msg);
	fprintf(context->report.output, "%s%4d %-59s%s", pre, i, buffer, post);
    }
}


static void print_row (SGMLCTX *context, int *row, int elems, char *msg, char *pre, char *post, BOOL close)
{
    char bufa[80], bufb[80];
    char *ptra = bufa, *ptrb = bufb, *ptrt;
    BOOL worth_it = FALSE;
    int x;
    
    for (x = 0; !worth_it && x < elems; x++)
    {
	worth_it = row[x] != 0 ;
    }
    
    if (! worth_it)
	return;
    
    fprintf(context->report.output, "%s%64s%s", pre, "", post);
    fprintf(context->report.output, "%s%-64s%s", pre, msg, post);
    
    *ptra = 0;
    *ptrb = 0;
    
    for (x = 0; x < elems; x ++)
    {
	ELEMENT *element = &context->elements[x];
	
	if (row[x] == 0 || (element->flags & FLAG_ARTIFICIAL) != 0 )
	    continue;
	
	if (close)
	    sprintf(ptrb, "%s%4d /%-10s", ptra, row[x], element->name.ptr);
	else
	    sprintf(ptrb, "%s%4d %-11s", ptra, row[x], element->name.ptr);
	ptrt = ptra;
	ptra = ptrb;
	ptrb = ptrt;
	
	if (strlen(ptra) > 48)
	{
	    fprintf(context->report.output, "%s%s%s", pre, ptra, post);
	    *ptra = 0;
	    *ptrb = 0;
	}
    }
    
    if (strlen(ptra) > 0)
    {
	fprintf(context->report.output, "%s%-64s%s", pre, ptra, post);
	*ptra = 0;
	*ptrb = 0;
    }
}

static void print_list (SGMLCTX *context, STRING_LIST *the_list, char *msg, char *pre, char *post )
{
        STRING_LIST *list = the_list;

        if (the_list == NULL)
        {
                return;
        }

	while ( list->prev != NULL )
	{
        	list->prev->next = list;
		list = list->prev;
	}

	fprintf(context->report.output, "%s%64s%s", pre, "", post);
	fprintf(context->report.output, "%s%-64s%s", pre, msg, post);

	while ( list != NULL )
	{
                fprintf(context->report.output, "%s%-64s%s", pre, list->string.ptr, post);
		list = list->next;
	}
}

extern void print_report (SGMLCTX * context)
{
        char buffer[80];
	const BOOL filter = context->mode == mode_filter;
	char *pre = filter ? "<!-- " : "";
        char *post = filter ? " -->\r\n" : "<br>\r\n";
	char *rating;
	int good, bad, x;
	double ratio;

	REPORT *report = &context->report;

        fprintf(context->report.output, "\r\n");
        fprintf(context->report.output, "%s%-64s%s", pre, "This document has been automatically", post);
        fprintf(context->report.output, "%s%-64s%s", pre, "corrected by software from ANT Limited.", post);
        fprintf(context->report.output, "%s%-64s%s", pre, "Email sales@ant.co.uk for further information", post);
        fprintf(context->report.output, "%s%-64s%s", pre, "on this and our other products.", post);
        fprintf(context->report.output, "%s%-64s%s", pre, "", post);
    	
    	print_bool (context, report->has_tables, "Uses tables", pre, post);
    	print_bool (context, report->has_forms, "Contains forms", pre, post);
    	print_bool (context, report->has_frames, "Uses frames", pre, post);
    	print_bool (context, report->has_scripts, "Uses scripts", pre, post);
    	print_bool (context, report->has_address, "Contains address", pre, post);

    	print_int (context,  report->antiquated_features, "antiquated feature", pre, post);
    	print_int (context,  report->badly_formed_comments, "badly formed comment", pre, post);
    	print_int (context,  report->unexpected_characters, "badly placed character", pre, post);
    	print_int (context,  report->stack_overflows, "stack overflow", pre, post);
    	print_int (context,  report->stack_underflows, "stack underflow", pre, post);

    	print_row (context,  report->good_opens, NUMBER_SGML_ELEMENTS, "Opening markup used in the document" , pre, post, FALSE);
    	print_row (context,  report->good_closes, NUMBER_SGML_ELEMENTS, "Closing markup used in the document" , pre, post, TRUE);
    	print_row (context,  report->implied_opens, NUMBER_SGML_ELEMENTS, "Implied opening markup" , pre, post, FALSE);
    	print_row (context,  report->implied_closes, NUMBER_SGML_ELEMENTS, "Implied closing markup" , pre, post, TRUE);
    	print_row (context,  report->missing_opens, NUMBER_SGML_ELEMENTS, "Missing opening markup that was inserted" , pre, post, FALSE);
    	print_row (context,  report->missing_closes, NUMBER_SGML_ELEMENTS, "Missing closing markup that was inserted" , pre, post, TRUE);
    	print_row (context,  report->close_without_opens, NUMBER_SGML_ELEMENTS, "Markup closed without being open" , pre, post, TRUE);
    	print_row (context,  report->had_unknown_attributes, NUMBER_SGML_ELEMENTS, "Markup where unknown attributes are used" , pre, post, FALSE);
    	print_row (context,  report->had_bad_attributes, NUMBER_SGML_ELEMENTS, "Markup where badly formed attributes are used" , pre, post, FALSE);
    	print_row (context,  report->not_repeatables, NUMBER_SGML_ELEMENTS, "Markup that should not be repeated" , pre, post, FALSE);

    	print_list (context,  report->unknown_elements, "Unknown element names" , pre, post);
    	print_list (context,  report->unknown_attributes, "Unknown attribute names" , pre, post);
    	print_list (context,  report->bad_attributes, "Unknown enumerated values" , pre, post);
    	print_list (context,  report->messages, "General messages" , pre, post);

	for (good = bad = x = 0; x < NUMBER_SGML_ELEMENTS; x++)
	{
        	good += report->good_opens[x];
        	good += report->good_closes[x];
	        bad += report->missing_opens[x];
        	bad += report->missing_closes[x];
	}

	good -= bad;

	if (bad < 1)
		bad = 1;
	if (good < 1)
		good = 1;

	ratio = (double)bad / (double)good;

	if (ratio > 1.0)
	{      	rating = "ABYSMAL";}
	else if (ratio > 0.1)
	{	rating = "POOR";}
	else if (ratio > 0.01)
	{	rating = "REASONABLE";}
	else
	{	rating = "EXCELLENT";}

        fprintf(context->report.output, "%s%-64s%s", pre, "", post);

        sprintf(buffer, "This document scored: %f", ratio);
        fprintf(context->report.output, "%s%-64s%s", pre, buffer, post);

	sprintf(buffer, "from %d good points and %d bad points.", good, bad);
        fprintf(context->report.output, "%s%-64s%s", pre, buffer, post);

	sprintf(buffer, "This merits a rating of %s.", rating);
	fprintf(context->report.output, "%s%-64s%s", pre, buffer, post);

	fprintf(context->report.output, "%s%-64s%s", pre, "", post);
	fprintf(context->report.output, "%s%-64s%s", pre, "A score of 0.0 is best. Larger scores are worst.", post);
	fprintf(context->report.output, "%s%-64s%s", pre, "A score over 1.0 suggests badly written HTML.", post);
	fprintf(context->report.output, "%s%-64s%s", pre, "", post);
}

#endif /* SGML_REPORTING */

/*****************************************************************************/

#if SGML_REPORTING

extern void my_sgml_note_good_open (SGMLCTX * context, ELEMENT * element)
{    (context->report.good_opens) [element->id] += 1;	}

extern void my_sgml_note_good_close (SGMLCTX * context, ELEMENT * element)
{    (context->report.good_closes) [element->id] += 1;	}

extern void my_sgml_note_implied_open (SGMLCTX * context, ELEMENT * element)
{    (context->report.implied_opens) [element->id] += 1;	}

extern void my_sgml_note_implied_close (SGMLCTX * context, ELEMENT * element)
{    (context->report.implied_closes) [element->id] += 1;	}

extern void my_sgml_note_missing_open (SGMLCTX * context, ELEMENT * element)
{    (context->report.missing_opens) [element->id] += 1;	}

extern void my_sgml_note_missing_close (SGMLCTX * context, ELEMENT * element)
{    (context->report.missing_closes) [element->id] += 1;	}

extern void my_sgml_note_close_without_open (SGMLCTX * context, ELEMENT * element)
{    (context->report.close_without_opens) [element->id] += 1;	}

extern void my_sgml_note_had_unknown_attribute (SGMLCTX * context, ELEMENT * element)
{    (context->report.had_unknown_attributes) [element->id] += 1;	}

extern void my_sgml_note_had_bad_attribute (SGMLCTX * context, ELEMENT * element)
{    (context->report.had_bad_attributes) [element->id] += 1;	}

extern void my_sgml_note_not_repeatable (SGMLCTX * context, ELEMENT * element)
{    (context->report.not_repeatables) [element->id] += 1;	}

#endif /* SGML_REPORTING */

/*****************************************************************************/

#if SGML_REPORTING

extern void my_sgml_note_antiquated_feature (SGMLCTX * context)
{    context->report.antiquated_features += 1;	}

extern void my_sgml_note_badly_formed_comment (SGMLCTX * context)
{    context->report.badly_formed_comments += 1;	}

extern void my_sgml_note_stack_overflow (SGMLCTX * context)
{    context->report.stack_overflows += 1;	}

extern void my_sgml_note_stack_underflow (SGMLCTX * context)
{    context->report.stack_underflows += 1;	}
                                                                    	
extern void my_sgml_note_unexpected_character (SGMLCTX * context)
{    context->report.unexpected_characters += 1;	}

#endif /* SGML_REPORTING */

/*****************************************************************************/

#if SGML_REPORTING

extern void my_sgml_note_has_tables (SGMLCTX * context)
{    context->report.has_tables = TRUE;	}

extern void my_sgml_note_has_forms (SGMLCTX * context)
{    context->report.has_forms = TRUE;	}

extern void my_sgml_note_has_scripts (SGMLCTX * context)
{    context->report.has_scripts = TRUE;	}

extern void my_sgml_note_has_frames (SGMLCTX * context)
{    context->report.has_frames = TRUE;	}

extern void my_sgml_note_has_address (SGMLCTX * context)
{    context->report.has_address = TRUE;	}

#endif /* SGML_REPORTING */

/*****************************************************************************/

#if SGML_REPORTING

extern void my_sgml_note_unknown_element (SGMLCTX * context, const char *fmt, va_list arglist) 
{
	STRING msg;
	STRING_LIST *new_s;
	char buffer[32];

	msg = print(fmt, arglist);
	sprintf(buffer, "Line %d: ", context->line);

       	new_s = mm_calloc(1, sizeof(STRING_LIST));
	new_s->string.nchars = strlen(buffer) + msg.nchars + 1;
	new_s->string.ptr = mm_calloc(1, new_s->string.nchars);
	strcpy(new_s->string.ptr, buffer);
	strncat(new_s->string.ptr, msg.ptr, msg.nchars);

	new_s->prev = context->report.unknown_elements;
	new_s->next = NULL;
	context->report.unknown_elements = new_s;

	string_free(&msg);
}

extern void my_sgml_note_unknown_attribute (SGMLCTX * context, const char *fmt, va_list arglist) 
{
	STRING msg;
	STRING_LIST *new_s;
	char buffer[32];

	msg = print(fmt, arglist);
	sprintf(buffer, "Line %d: ", context->line);

       	new_s = mm_calloc(1, sizeof(STRING_LIST));
	new_s->string.nchars = strlen(buffer) + msg.nchars + 1;
	new_s->string.ptr = mm_calloc(1, new_s->string.nchars);
	strcpy(new_s->string.ptr, buffer);
	strncat(new_s->string.ptr, msg.ptr, msg.nchars);

	new_s->prev = context->report.unknown_attributes;
	new_s->next = NULL;
	context->report.unknown_attributes = new_s;

	string_free(&msg);
}

extern void my_sgml_note_bad_attribute (SGMLCTX * context, const char *fmt, va_list arglist)
{
	STRING msg;
	STRING_LIST *new_s;
	char buffer[32];

	msg = print(fmt, arglist);
	sprintf(buffer, "Line %d: ", context->line);

       	new_s = mm_calloc(1, sizeof(STRING_LIST));
	new_s->string.nchars = strlen(buffer) + msg.nchars + 1;
	new_s->string.ptr = mm_calloc(1, new_s->string.nchars);
	strcpy(new_s->string.ptr, buffer);
	strncat(new_s->string.ptr, msg.ptr, msg.nchars);

	new_s->prev = context->report.bad_attributes;
	new_s->next = NULL;
	context->report.bad_attributes = new_s;

	string_free(&msg);
}

extern void my_sgml_note_message(SGMLCTX * context, const char *fmt, va_list arglist )
{
	STRING msg;
	STRING_LIST *new_s;
	char buffer[32];

	msg = print(fmt, arglist);
	sprintf(buffer, "Line %d: ", context->line);

       	new_s = mm_calloc(1, sizeof(STRING_LIST));
	new_s->string.nchars = strlen(buffer) + msg.nchars + 1;
	new_s->string.ptr = mm_calloc(1, new_s->string.nchars);
	strcpy(new_s->string.ptr, buffer);
	strncat(new_s->string.ptr, msg.ptr, msg.nchars);

	string_free(&msg);

        if ( context->report.messages == NULL || strcmp(new_s->string.ptr, context->report.messages->string.ptr) != 0 )
        {
                new_s->prev = context->report.messages;
                new_s->next = NULL;
                context->report.messages = new_s;
        }
        else
        {
                mm_free(new_s->string.ptr);
                mm_free(new_s);
        }
}

#endif /* SGML_REPORTING */

/*****************************************************************************/

/* eof reports.c */
