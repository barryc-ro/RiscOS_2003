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

extern int gbf_flags;
extern int gbf_active(int gbf);
extern void gbf_init(void);	/* Don't ask why needed! */

#endif /* included_gbf_h */
