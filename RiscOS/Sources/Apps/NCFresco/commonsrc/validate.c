/* -*-C-*-

   Examine a constructed internal rid_ tree and perform whatever
   consistency checks and validation that we can perform.  This isn't
   intended to be a mechanism for verbosely printing the structures.

 */

typedef struct
{
    intxy		size;
} stream_info;


void get_stream_info(rid_text_stream *stream, stream_info *si)
{
    rid_text_item *ti;
    rid_pos_item *pi;

    memset(si, 0, sizeof(*si));

    for (pi = stream->pos_list; pi != NULL; pi = pi->next)
    {
    }

}


/* eof commonsrc/validate.c */
