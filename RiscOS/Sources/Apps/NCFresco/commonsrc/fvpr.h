/* -*-C-*-
 *
 * fvpr.h - Final Value Progressive Rendering
 *
 * (C) ANT Limited 1997. All rights reserved.
 */

#ifndef included_fvpr_h
#define included_fvpr_h

extern BOOL fvpr_progress_stream(rid_text_stream *stream);
extern BOOL fvpr_progress_stream_flush(rid_text_stream *stream);

extern void fvpr_forget( rid_text_stream *stream );

#endif /* included_fvpr_h */

/* eof fvpr.h */
