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

extern int gbf_flags;
extern int gbf_active(int gbf);
extern void gbf_init(void);	/* Don't ask why needed! */

#endif /* included_gbf_h */


