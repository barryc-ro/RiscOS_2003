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
#define GBF_NEW_FORMATTER		0x00000040 /* pdh: won't work with this off */
#define GBF_AUTOFIT			0x00000080
#define GBF_NETSCAPE_OVERLAPS		0x00000100 /* DAF: At your own peril! */
#define GBF_HARD_TABLES			0x00000200
#define GBF_EARLYIMGFETCH		0x00000400
#define GBF_LOW_MEMORY			0x00000800 /* have started discarding images/using emergency cache */
#define GBF_ANTI_TWITTER		0x00001000 /* using s/w anti-twitter */
#define GBF_VERY_LOW_MEMORY		0x00002000 /* have almost exhausted emergency cache */
#define GBF_SI1_PCT			0x00004000 /* Hack for Si for %ages and minwidth */
#define GBF_CAPTIONS			0x00008000 /* Do captions in tables - might be unstable */
#define GBF_AUTOFIT_ALL_TEXT		0x00010000 /* shrink all text on autofit */
#define GBF_FILES_IN_ONE_GO             0x00020000 /* local files loaded in one lump */
#define GBF_RELATIVE_TABLE		0x00040000 /* Handle relative widths in tables */
#define GBF_UTF8			0x00080000 /* Store UTF8 in  */

/*****************************************************************************/

#define GBF_MINWIDTH_A			0x10000000 /* NCFresco */
#define GBF_MINWIDTH_B			0x20000000 /* Fresco */
#define GBF_MINWIDTH_C			0x40000000 /* Reserved */
#define GBF_MINWIDTH_D			0x80000000 /* Reserved */

/*

A = max(LM) + max(CW) + max(RM)
    [tends to be wide]
B = max(LM + CW + RM)
    [tends to be narrow]

*****************************************************************************

   Production builds of Fresco try to resolve gbf_active() usage at
   compile time. Other variants leave it until runtime (the original
   intention of the facility, certainly during development). */

#if defined(FRESCO) && defined(PRODUCTION)

#define gbf_active(arg) ( (arg==GBF_GUESS_ATTRIBUTES)           \
                          || (arg==GBF_GUESS_ENUMERATIONS)      \
                          || (arg==GBF_NEW_FORMATTER) 		\
			  || (arg==GBF_TABLES_UNEXPECTED)       \
			  || (arg==GBF_MINWIDTH_B)              \
			  || (arg==GBF_SI1_PCT)                 \
			  || (arg==GBF_FILES_IN_ONE_GO) )

#else

extern int gbf_flags;
extern int gbf_active(int gbf);

#endif

extern void gbf_init(void);	/* Don't ask why needed! */

/*****************************************************************************/

#endif /* included_gbf_h */


