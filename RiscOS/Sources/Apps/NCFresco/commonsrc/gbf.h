/* gbf.h -*-C-*- Global Behaviour Flags
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 */

#ifndef included_gbf_h
#define included_gbf_h

#ifdef NEW_UNEXP_TABLE
#error "This approach is obsolete"
#endif

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

extern int gbf_flags;

#if defined(STBWEB) || defined(BUILDERS)
extern int gbf_active(int gbf);
#elif defined(FRESCO)
/* pdh: try and make it compile-time constant */
/* daf: BTW, this is already out of date :-) */
#define gbf_active(arg) ( (arg==GBF_GUESS_ATTRIBUTES)           \
                          || (arg==GBF_GUESS_ENUMERATIONS)      \
                          || (arg==GBF_NEW_FORMATTER) )
#endif

extern void gbf_init(void);	/* Don't ask why needed! */

#endif /* included_gbf_h */


