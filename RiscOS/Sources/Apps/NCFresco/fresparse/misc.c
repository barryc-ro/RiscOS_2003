/* fresparser/misc.c - Assorted elements for HTML parser */

#include "htmlparser.h"


extern void startsgml (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
extern void startxmp (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    startpre(context, element, attributes); }





extern void startabbrev (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}

#if 0
extern void startabstract (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

extern void startacronym (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}

#if 0
extern void startadded (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

extern void startaddress (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

#ifdef STBWEB
    add_italic_to_font(context);
#endif
}

extern void finishabbrev (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishabstract (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishacronym (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishadded (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishaddress (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

/*****************************************************************************/


#if 0
extern void startarg (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

#if 0
extern void finisharg (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif



#if 0
extern void startbox (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif




#if 0
extern void finishbox (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

/*****************************************************************************/



/*****************************************************************************/

#if 0
extern void startbyline (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

#if 0
extern void startchanged (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

#if 0
extern void startcmd (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif


#if 0
extern void startcomment (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

extern void startfig (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}


extern void startfootnote (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}


#if 0
extern void starthtmlplus (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

#if 0
extern void startimage (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

#if 0
extern void startl (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

extern void startlisting (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    startpre (context, element, attributes);
}

#if 0
extern void startlit (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

#if 0
extern void startmargin (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

extern void startmath (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}

extern void startnextid (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}

extern void startnote (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}

#if 0
extern void startover (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

/*****************************************************************************

  A paragraph break is just any other block level element. Multiple lines
  of whitespace are created with <BR>.
  */

extern void startp (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    text_item_ensure_break(htmlctxof(context));

    std_lcr_align(context, &attributes->value[HTML_P_ALIGN]);
}

extern void startperson (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}

extern void startplaintext (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}

extern void startq (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}

#if 0
extern void startquote (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

#if 0
extern void startremoved (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

#if 0
extern void startrender (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

#if 0
extern void starttab (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif


#if 0
extern void startu (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{    generic_start (context, element, attributes);	}
#endif

/*****************************************************************************

  ARTIFICIAL TAGS

  */


/*****************************************************************************/


#if 0
extern void finishbyline (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif


extern void finishcenter (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishcentre (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishchanged (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishcite (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishcmd (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishcode (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}


#if 0
extern void finishcomment (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishdd (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishdfn (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishdir (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishdl (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishdt (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishem (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishfig (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishfont (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishfootnote (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishhead (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishhr (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishhtml (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishhtmlplus (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishi (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishimage (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

#if 0
extern void finishimg (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

#if 0
extern void finishinput (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

#if 0
extern void finishisindex (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishkbd (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishl (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishli (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishlink (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishlisting (SGMLCTX * context, ELEMENT * element)
{
    finishpre(context, element);
}

#if 0
extern void finishlit (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishmap (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishmargin (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishmath (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishmenu (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishmeta (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

#if 0
extern void finishnextid (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishnote (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishol (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishover (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishp (SGMLCTX * context, ELEMENT * element)
{
    generic_finish (context, element);

/*     text_item_ensure_break(htmlctxof(context)); */
}

extern void finishperson (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishplaintext (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}


extern void finishq (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishquote (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

#if 0
extern void finishremoved (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

#if 0
extern void finishrender (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

extern void finishs (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishsamp (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishscript (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *htmlctx = htmlctxof(context);

    generic_finish (context, element);

    PRSDBG(("finishscript(): Grabbed '%.*s' %d %p\n",
	    htmlctx->inhand_string.nchars,
	    htmlctx->inhand_string.ptr,
	    htmlctx->inhand_string.nchars,
	    htmlctx->inhand_string.ptr));

    string_free(&htmlctx->inhand_string);

    select_fmt_mode(htmlctx);
}

extern void finishstrong (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishsub (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishsup (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

#if 0
extern void finishtab (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif



extern void finishtt (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishu (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishul (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishvar (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishbig (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishdiv (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishsmall (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishstrike (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void finishstyle (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *htmlctx = htmlctxof(context);
    generic_finish (context, element);

    PRSDBG(("Collected <STYLE> string is:\n'%.*s'\n",
	    htmlctx->inhand_string.nchars,
	    htmlctx->inhand_string.ptr));

    string_free(&htmlctx->inhand_string);

    /* pdh: stuck this in, patterning it on </script> */
    select_fmt_mode(htmlctx);
}

extern void finishxmp (SGMLCTX * context, ELEMENT * element)
{    finishpre (context, element);	}



/* eof fresparser/misc.c */
