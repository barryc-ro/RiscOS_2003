/***************************************************/
/* File   : FileOps.h                              */
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

#ifndef FileOps_Included

  /* This was originally in a different header file, but for */
  /* purposes of this demo code I've included it here.       */

  struct repository_entry;

  typedef struct repository_entry
  {
    struct repository_entry * prev;
    struct repository_entry * next;

    char                    * path;
    char                    * name;
    char                    * version;
    char                    * build_date;
    char                    * build_master;

    unsigned int              crc_pre;
    unsigned int              crc_post;

  } repository_entry;

  /* File format definitions. This MUST be kept in sync with */
  /* the output produced by the MakeNCimg tool!              */

  #define FileOps_IDWord             0x474B5052 /* 'RPKG' */

  #define FileOps_NameFieldID        'a'
  #define FileOps_VersionFieldID     'b'
  #define FileOps_BuildDateFieldID   'c'
  #define FileOps_BuildMasterFieldID 'd'
  #define FileOps_CRCPreFieldID      'e'
  #define FileOps_CRCPostFieldID     'f'

  #define FileOps_DataFieldID        '0'

  /* Function prototyps */

  _kernel_oserror * fileops_open_image              (const char * path, unsigned int * handle);
  _kernel_oserror * fileops_close_image             (unsigned int handle);
  _kernel_oserror * fileops_read_image_field        (unsigned int handle, char field_id, char ** value);

  _kernel_oserror * fileops_check_pre_crc           (repository_entry * check, unsigned int * result, unsigned int dots);
  _kernel_oserror * fileops_check_post_crc          (repository_entry * check, unsigned int * result, unsigned int dots);

#endif
