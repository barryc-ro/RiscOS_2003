/* gbf.h -*-C-*- Global Behaviour Flags
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 */

#ifndef included_gbf_h
#define included_gbf_h

#define GBF_TABLES_UNEXPECTED		0x00000001
#define GBF_FVPR			0x00000002
#define GBF_GUESS_ELEMENTS		0x00000004
#define GBF_GUESS_ATTRIBUTES		0x00000008
#define GBF_GUESS_ENUMERATIONS		0x00000010
#define GBF_TRANSLATE_UNDEF_CHARS	0x00000020
#define GBF_NEW_FORMATTER		0x00000040
#define GBF_AUTOFIT			0x00000080
#define GBF_NETSCAPE_OVERLAPS		0x00000100 /* DAF: At your own peril! */
#define GBF_HARD_TABLES			0x00000200
#define GBF_EARLYIMGFETCH		0x00000400
#define GBF_LOW_MEMORY			0x00000800 /* have started discarding images/using emergency cache */
#define GBF_ANTI_TWITTER		0x00001000 /* using s/w anti-twitter */
#define GBF_VERY_LOW_MEMORY		0x00002000 /* have almost exhausted emergency cache */
#define GBF_SI1_PCT			0x00004000 /* Hack for Si for %ages and minwidth */

/*****************************************************************************

   Production builds of Fresco try to resolve gbf_active() usage at
   compile time. Other variants leave it until runtime (the original
   intention of the facility, certainly during development). */

#if defined(FRESCO) && defined(PRODUCTION)

#define gbf_active(arg) ( (arg==GBF_GUESS_ATTRIBUTES)           \
                          || (arg==GBF_GUESS_ENUMERATIONS)      \
                          || (arg==GBF_NEW_FORMATTER) 		\
			  || (arg==GBF_TABLES_UNEXPECTED) )

/* DAF: 970620: I suspect this is wrong WRT tables
   initialisation. Needs more though before this branch is used. */
#define gbf_init()

#else

extern int gbf_flags;
extern int gbf_active(int gbf);
extern void gbf_init(void);	/* Don't ask why needed! */

#endif

/*****************************************************************************/

#endif /* included_gbf_h */


