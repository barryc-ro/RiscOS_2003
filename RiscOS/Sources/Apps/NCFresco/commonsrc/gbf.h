/* gbf.h -*-C-*- Global Behaviour Flags
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 */

#ifndef included_gbf_h
#define included_gbf_h

#define GBF_TABLES_UNEXPECTED		0x00000001
#define GBF_FVPR			0x00000002

extern int gbf_flags;
extern int gbf_active(int gbf);
extern void gbf_init(void);	/* Don't ask why needed! */

#endif /* included_gbf_h */
