/***************************************************/
/* File   : FileOps.c                              */
/*                                                 */
/* Purpose: ROM image file operations. Done via.   */
/*          RISC OS SWIs as fopen etc. do not      */
/*          reliably fill in errors through        */
/*          _kernel_last_oserror in ROM CLib       */
/*          for RISC OS 3.71 or earlier; and yet,  */
/*          the application must report sensible   */
/*          errors, and the machine it runs on     */
/*          must be as "out of the box" as it can  */
/*          be - so no softloading an amended      */
/*          SharedCLibrary module... :-/           */
/*                                                 */
/* Author : A.D.Hodgkinson                         */
/*                                                 */
/* History: 19-Jan-1999: Created.                  */
/***************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "kernel.h"
#include "swis.h"

#include "FileOps.h"

/* Local statics */

static char * rotbuf = NULL;

/*************************************************/
/* fileops_open_image()                          */
/*                                               */
/* Open a ROM image for reading. Checks the      */
/* header to see if it looks like 'the real      */
/* thing' - see FileOps_IDWord, FileOps.h.       */
/*                                               */
/* Parameters: Pathname of the image file;       */
/*                                               */
/*             Pointer to an unsigned int in     */
/*             which the RISC OS file handle for */
/*             the now-open image is written. If */
/*             the file could not be opened or   */
/*             appears to not be a ROM image in  */
/*             which case the file will be       */
/*             closed after access, zero is      */
/*             returned instead.                 */
/*                                               */
/* Returns:    If zero is returned as the file   */
/*             handle because an error occurred  */
/*             when trying to open the file,     */
/*             this is returned. If zero is      */
/*             returned because the file is not  */
/*             a ROM image, NULL is returned.    */
/*             This is how you distinguish       */
/*             between the two cases of error or */
/*             not an image.                     */
/*************************************************/

_kernel_oserror * fileops_open_image(const char * path, unsigned int * handle)
{
  _kernel_oserror * e;
  char              buffer[4];

  if (!path || !*path || !handle) return NULL;

  memset(buffer, 0, sizeof(buffer));

  /* Open the file */

  RetError(_swix(OS_Find,
                 _INR(0,2) | _OUT(0),

                 0x4f,
                 path,
                 NULL,

                 handle));

  /* Read the four byte header */

  e = _swix(OS_GBPB,
            _INR(0,3),

            4,
            *handle,
            buffer,
            sizeof(buffer));

  if (e) goto fileops_open_image_abort; /* See below */

  /* Is the header OK? */

  if (*((unsigned int *) buffer) == FileOps_IDWord) return NULL;

  /* Nope; close the file, zero the handle, return with no error */

  e = NULL;

fileops_open_image_abort:

  _swix(OS_Find,
        _INR(0,1),

        0,
        *handle);

  *handle = 0;

  return e;
}                                                                                                                        

/*************************************************/
/* fileops_close_image()                         */
/*                                               */
/* Close a ROM image.                            */
/*                                               */
/* Parameters: RISC OS file handle as returned   */
/*             by fileops_open_image or any      */
/*             similar function.                 */
/*************************************************/

_kernel_oserror * fileops_close_image(unsigned int handle)
{
  /* A tricky one, this */

  return _swix(OS_Find,
               _INR(0,1),

               0,
               handle);
}

/*************************************************/
/* fileops_read_image_field()                    */
/*                                               */
/* Get a field of an image by reading the given  */
/* file and fill it into the local storage       */
/* buffer. The caller must take a copy as soon   */
/* as the call returns (if it give no errors) as */
/* the buffer contents may change as soon as any */
/* other calls to fileops_.. functions are made. */
/*                                               */
/* Special case - if asking for the Data field,  */
/* FileOps_DataFieldID, no valus is returned;    */
/* the file pointer is left at the start of the  */
/* raw data, which is assumed to continue to the */
/* end of the image.                             */
/*                                               */
/* Parameters: RISC OS file handle as returned   */
/*             by fileops_open_image or any      */
/*             similar function;                 */
/*                                               */
/*             The required field ID character   */
/*             (see FileOps.h);                  */
/*                                               */
/*             Pointer to a char *, in which     */
/*             the address of the buffer with    */
/*             the NUL terminated value string   */
/*             will be returned. This returned   */
/*             value is invalid if the function  */
/*             returns an error directly. An     */
/*             empty string is pointed to if     */
/*             the Data field was found.         */
/*                                               */
/* Returns:    General memory error 40 if memory */
/*             claims fail, aside from any other */
/*             RISC OS errors.                   */
/*************************************************/

_kernel_oserror * fileops_read_image_field(unsigned int handle, char field_id, char ** value)
{
  int           lf_found, id_found, colon_found;
  unsigned int  read;
  unsigned int  flags;
  char          character;

  if (!handle || !value) return NULL;

  *value = NULL;

  /* Prepare the local buffer */

  if (rotbuf) free(rotbuf);
  rotbuf = malloc(1);
  if (!rotbuf) return make_no_memory_error(40);
  *rotbuf = '\0';

  /* Look for the ID byte for the field. ID bytes always follow */
  /* a LF character (0x0a), followed by any number of padding   */
  /* bytes and eventually a colon then LF character. The second */
  /* second LF is followed by the field value, which is either  */
  /* EOF or LF terminated (this LF forming the preceeding char  */
  /* before another ID byte).                                   */
  /*                                                            */
  /* The padding bytes are usually used as a comment which      */
  /* describes the field. To make parsing simple, we specify    */
  /* that neither a colon nor LF character may be part of this  */
  /* series of padding bytes. Meanwhile, afield value may not   */
  /* contain character code 0 or an LF char.                    */
  /*                                                            */
  /* Anyway, this'll come in handy...                           */

  #define FileOps_BGet if (                                        \
                            _swix(OS_BGet,                         \
                                  _IN(1) | _OUT(0) | _OUT(_FLAGS), \
                                                                   \
                                  handle,                          \
                                                                   \
                                  (int *) &character,              \
                                  &flags)                          \
                          )                                        \
                          break;                                   \
                                                                   \
                       if (flags & _C) break;

  /* First, place the file pointer past the 'RPKG' header */

  _swix(OS_Args,
        _INR(0,2),

        1,
        handle,
        4);

  /* Now look for the ID. */

  id_found = 0;
  lf_found = 0;

  /* Look for the LF character. If the SWI gives an error */
  /* or the carry flag is set, bail out. Carry flag clear */
  /* an no error means the byte was read OK.              */

  while (!lf_found)
  {
    FileOps_BGet;

    if (character == 0x0a) lf_found = 1;
  }

  /* Did we find it? */

  if (!lf_found)
  {
    erb.errnum = Utils_Error_Custom_Normal;

    StrNCpy0(erb.errmess,
             lookup_token("ROMFmt01:No LF after RPKG header",0,0));

    return &erb;
  }

  /* If so, enter a general loop to read ID / value pairs. */

  while (!id_found)
  {
    /* On entry here we should always be in the position of having */
    /* just read the LF that preceeds an ID byte.                  */

    FileOps_BGet;

    /* Did we find it? */

    if (character == field_id) id_found = 1;

    /* Either way, we'll need to skip on to the value, both to */
    /* read it or to get to the next ID, and to check that the */
    /* file structure looks OK so far.                         */
    /*                                                         */
    /* We're looking for 'n' bytes, then a colon, then an LF.  */
    /* The colon must be followed by the LF. No LF may preceed */
    /* the colon.                                              */

    colon_found = 0;

    while (!colon_found)
    {
      FileOps_BGet;

      if (character == 0x0a) break;
      if (character == ':')  colon_found = 1;
    }

    if (!colon_found)
    {
      erb.errnum = Utils_Error_Custom_Normal;

      StrNCpy0(erb.errmess,
               lookup_token("ROMFmt02:No ':' after ID byte and comment",0,0));

      return &erb;
    }

    FileOps_BGet;

    if (character != 0x0a)
    {
      erb.errnum = Utils_Error_Custom_Normal;

      StrNCpy0(erb.errmess,
               lookup_token("ROMFmt03:No LF after ':'",0,0));

      return &erb;
    }

    /* Right, now we're at the start of the value, which we'll go */
    /* on to read. Whether or not we record it depends on whether */
    /* or not this is the right ID. First check for the special   */
    /* case of the ID being the Data field though, and if it is,  */
    /* break out here.                                            */

    if (id_found && field_id == FileOps_DataFieldID)
    {
      (*value) = "";
      return NULL;
    }

    /* OK, we do want to read or skip past the value */

    lf_found = 0;
    read     = 0;

    while (!lf_found)
    {
      FileOps_BGet;

      if (character == 0x0a)
      {
        lf_found = 1;
      }
      else if (id_found)
      {
        char * newbuf;

        /* Welcome to the world's least efficient file reading code. On   */
        /* the plus side, it's very solid, which given the nature of the  */
        /* application is a primary requirement. Alongside sensible error */
        /* reporting of course, so no use of fopen etc. (see top of this  */
        /* source file for more).                                         */

        read++;
        newbuf = realloc(rotbuf, read + 1); /* :-) */

        if (!newbuf) return make_no_memory_error(40);

        rotbuf = newbuf;

        rotbuf[read - 1] = character;
        rotbuf[read]     = '\0';
      }
    }

    /* That's that, then. Round we go for another ID or we found */
    /* it and filled in the value, so time to exit.              */
  }

  if (id_found) *value = rotbuf;

  return NULL;
}

/*************************************************/
/* fileops_check_pre_crc()                       */
/*                                               */
/* Check the pre-compression CRC value of an     */
/* image. fileops_report_crc_result can be used  */
/* to reflect the result to the front-end if     */
/* this is required.                             */
/*                                               */
/* Parameters: Pointer to a repository_entry     */
/*             structure representing the image  */
/*             to check;                         */
/*                                               */
/*             Pointer to an int, which is       */
/*             updated with 0 if the CRC value   */
/*             matches (success) or 1 if it does */
/*             not, or there is another error    */
/*             (failure);                        */
/*                                               */
/*             1 to print a dot for each 32K     */
/*             block read, else 0.               */
/*************************************************/

_kernel_oserror * fileops_check_pre_crc(repository_entry * check, unsigned int * result, unsigned int dots)
{
  _kernel_oserror * e;
  unsigned int      handle;
  unsigned int      notread;
  unsigned int      crc;
  char            * temp;

  Byte              in  [32768]; /* At the time of writing, throwing such large objects on the stack will make */
  Byte              out [32768]; /* DDT misbehave. If you need to use DDT, temporarily declare these as static */
  z_stream          d_stream;
  int               err;

  /* Sanity checks */

  if (!result) return NULL;
  *result = 1;
  if (!check || !check->path) return NULL;

  /* Open the file, find the data */

  RetError(fileops_open_image(check->path, &handle));
  RetError(fileops_read_image_field(handle, FileOps_DataFieldID, &temp));

  crc = 0;

  d_stream.zalloc   = Z_NULL;
  d_stream.zfree    = Z_NULL;
  d_stream.opaque   = Z_NULL;

  d_stream.next_in  = in;
  d_stream.avail_in = 0;
  d_stream.next_out = out;

  err = inflateInit(&d_stream);

  if (err != Z_OK)
  {
    RetError(fileops_close_image(handle));
    return utils_make_zlib_error(err);
  }

  /* Do the CRC check */

  do
  {
    memset(in, 0, sizeof(in));

    /* Read a chunk of data */

    e = _swix(OS_GBPB,
              _INR(0,3) | _OUT(3),

              4,
              handle,
              in,
              sizeof(in),

              &notread);

    if (e)
    {
      inflateEnd(&d_stream);
      RetError(fileops_close_image(handle));
      return e;
    }

    /* Did we read anything? */

    if (notread < sizeof(in))
    {
      /* Decompress the block in 32K chunks, running a CRC check */
      /* on it all as we go                                      */

      d_stream.next_in   = in;
      d_stream.avail_in  = sizeof(in) - notread;

      do
      {
        memset(out, 0, sizeof(out));

        d_stream.next_out  = out;
        d_stream.avail_out = sizeof(out);

        err = inflate(&d_stream, Z_SYNC_FLUSH);

        if (err != Z_OK && err != Z_STREAM_END)
        {
          RetError(fileops_close_image(handle));
          return utils_make_zlib_error(err);
        }

        /* The compressor utility compresses whole 32K blocks padded */
        /* with NUL bytes if the file isn't a multiple of 32K long.  */
        /* So if this is the last bit of input file (notread > 0),   */
        /* want to make sure we use whole blocks here too, otherwise */
        /* only check as much data as zlib gave us.                  */

        if (notread > 0)
        {
          crc = (unsigned int) crc32(crc,
                                     out,
                                     sizeof(out));
        }
        else
        {
          crc = (unsigned int) crc32(crc,
                                     out,
                                     sizeof(out) - d_stream.avail_out);
        }
      }
      while (d_stream.avail_in && err != Z_STREAM_END);

      if (dots) printf(".");
    }
  }
  while (notread == 0);

  /* Close the file */

  fileops_close_image(handle);

  /* End the decompression session */

  err = inflateEnd(&d_stream);
  if (err != Z_OK) return utils_make_zlib_error(err);

  /* Was the CRC value correct? */

  if (crc == check->crc_pre) *result = 0;

  /* Finished */

  return NULL;
}

/*************************************************/
/* fileops_check_post_crc()                      */
/*                                               */
/* Check the post-compression CRC value of an    */
/* image. fileops_report_crc_result can be used  */
/* to reflect the result to the front-end if     */
/* this is required.                             */
/*                                               */
/* Parameters: Pointer to a repository_entry     */
/*             structure representing the image  */
/*             to check;                         */
/*                                               */
/*             Pointer to an int, which is       */
/*             updated with 0 if the CRC value   */
/*             matches (success) or 1 if it does */
/*             not, or there is another error    */
/*             (failure);                        */
/*                                               */
/*             1 to print a dot for each 32K     */
/*             block read, else 0.               */
/*************************************************/

_kernel_oserror * fileops_check_post_crc(repository_entry * check, unsigned int * result, unsigned int dots)
{
  _kernel_oserror * e;
  unsigned int      handle;
  unsigned int      notread;
  unsigned int      crc;
  char            * temp;
  Byte              buffer[32768];

  /* Sanity checks */

  if (!result) return NULL;
  *result = 1;
  if (!check || !check->path) return NULL;

  /* Open the file, find the data */

  RetError(fileops_open_image(check->path, &handle));
  RetError(fileops_read_image_field(handle, FileOps_DataFieldID, &temp));

  crc = 0;

  do
  {
    memset(buffer, 0, sizeof(buffer));

    e = _swix(OS_GBPB,
              _INR(0,3) | _OUT(3),

              4,
              handle,
              buffer,
              sizeof(buffer),

              &notread);

    if (e)
    {
      RetError(fileops_close_image(handle));
      return e;
    }

    if (notread < sizeof(buffer))
    {
      crc = (unsigned int) crc32(crc,
                                 buffer,
                                 sizeof(buffer) - notread);

      if (dots) printf(".");
    }
  }
  while (notread == 0);

  /* Was the CRC value correct? */

  if (crc == check->crc_post) *result = 0;

  /* Finished */

  return fileops_close_image(handle);
}
