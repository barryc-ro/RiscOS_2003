//
// File: mzalgctn.idl - Logical Content interfaces
//
// Description:
//
// Modification History:
//   Date	Name	Comments
//   06-Jun-97  plord   Add BlobMgmt interface 
//   14-May-97  plord   Add suggested buffer size
//   27-Mar-97  plord   Add some comments
//   25-Mar-97  plord   Changed msecs on updateStats to long
//   21-Mar-97  plord   Add Ctnt_setAtr back in for vstagpatch
//   20-Mar-97  plord   Added new methods for updating Content for vstag
//   18-Mar-97  plord   Finalize new interface for Versio 3.0.1.0
//   11-Dec-96  plord   copied from ao vob, commented and consolidated
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Oracle Corporation							     
// Oracle Media Server (TM)						     
// All Rights Reserved
// Copyright (C) 1993-1996						     
//--------------------------------------------------------------------
 
#ifndef MZALGCTN_ORACLE
#define MZALGCTN_ORACLE
 
#ifndef MZACOM_ORACLE
#include "mzacom/idl"
#endif
#ifndef MKD_IDL
#include "mkd/idl"
#endif
#ifndef MKCF_IDL
#include "mkcf/idl"
#endif
//--------------------------------------------------------------------
// Module: mza
// Function:
//   This module is responsible for all application leve operations  
//   for the video server.
//--------------------------------------------------------------------
module mza
{
 
    //-------------------------------------------------------------------    
    // FORWARD DECLARATIONS
    //-------------------------------------------------------------------    
   interface Ctnt;
   interface CtntPvdr;
   interface LgCtnt;
   interface Clip;

    //-------------------------------------------------------------------   
    //   EXCEPTIONS
    //-------------------------------------------------------------------    
   
   //----------------------------------------------------------------------
   // Exception: NoLgCtnt
   // Function:
   //   
   //----------------------------------------------------------------------
   exception NoLgCtnt
   {
     opstatus   status;
     string     description;
     long       errorCode;
   };
    
   //----------------------------------------------------------------------
   // Exception: BadPosition
   // Function:
   //   
   //----------------------------------------------------------------------
   exception BadPosition
   {
     opstatus   status;
     string     description;
     string     position;
     long       errorCode;

   };
    
   //----------------------------------------------------------------------
   // Exception: BadProhib
   // Function:
   //   
   //----------------------------------------------------------------------
   exception BadProhib
   {
     opstatus   status;
     string     description;
     string     prohib;
     long       errorCode;

   };
    
    
   //-------------------------------------------------------------------
   // TYPEDEFS
   //------------------------------------------------------------------- 
  
   //-------------------------------------------------------------------   
   // Typedef: ClipAtr
   // Function: This structure represents a clip's attributes
   // They are defined as follows:
   //    clipOR        - The clip's Object Reference
   //    ctntOR        - The Object Reference of the Content this clip is from
   //    name          - A name given to the clip
   //    desc          - A description of the given to the clip (optional)
   //	 ctntNm	       - The name of the content	
   //    createDate    - The date the clip was created
   //    startPos      - The starting position of this clip (see mkd.idl)
   //    stopPos       - The ending position of this clip (see mkd.idl)
   //    clipAssigned  - Is this clip assigned to any logical content.
    //------------------------------------------------------------------- 

  struct ClipAtr
   {
      Clip     clipOR;
      Ctnt     ctntOR; 
      string   name;
      string   desc; 
      string   ctntNm;
      mkd::pos startPos;
      mkd::pos stopPos;
      boolean  assigned;
   };

    //-------------------------------------------------------------------
    // Typedef: ClipAtrLst
    // Function: A list of ClipAtr structures
    //-------------------------------------------------------------------   
    typedef sequence <ClipAtr> ClipAtrLst;

    //-------------------------------------------------------------------
    // Typedef: CtntAtr
   // Function: This structure represents a Ctnt object's attributes
   // They are defined as follows:
   //    ctntOR         - The Ctnt's Object Reference
   //    ctntPvdrOR     - The provider who owns the content (optional)
   //    name           - A name given to the Ctnt (optional)
   //    desc           - A description of the given to the Ctnt (optional)
   //    createDate     - Date the content was created
   //    filename       - The actual file name of the content. Usually an mds
   //			  tag filename
   //    len            - The Size of the content in bytes
   //    msecs          - The total runlength of the content in milliseconds. 
   //			  A long is 32 bits and corresponds to about 25 days 
   //			  worth of time.
   //    rate           - The encoding rate of the content in bps
   //    firstTime      - The First time stamp in file
   //    lastTime       - The last time stamp in the file
   //    format         - All the format information (see mkd.idl)
   //    prohibFlags    - Restrictions on seek, scan, pause, etc. 
   //			  (see mkd.idl) 
   //    tagsFlag       - Are there tags in the file?
   //    multiRateFlag  - Is this multirate content?
   //    reliableFlag   - Reliable transport required?
   //    volLocation    - Volume location of the content (optional)
   //    contStatus     - Location status of the content (tape, disk), 
   //    assigned       - Is this content object assigned to any clips?
   //    sugBufSz       - Suggested Buffer size for the client in bytes
   //			  if Unknown it will be set to BufSzUnknown
   //-------------------------------------------------------------------  
   const long BufSzUnknown = 0; // Don't know the buffer size
 
   struct CtntAtr
   {
     Ctnt              ctntOR;        
     CtntPvdr          ctntPvdrOR;
     string            name;          
     string            desc;	      
     string            createDate;    
     string            filename;      
     long long         len;           
     long              msecs;         
     long              rate;        
     long long         firstTime;    
     long long         lastTime;    
     mkd::contFormat   format;       
     mkd::prohib       prohibFlags;  
     boolean           tagsFlag;      
     boolean           multiRateFlag; 
     boolean           reliableFlag;  
     string            volLocation; 
     mkd::contStatus   contStatus;    
     boolean	       assigned;
     long              sugBufSz;
   };
    
    //-------------------------------------------------------------------
    // Typedef: CtntAtrLst
    // Function: A list of CtntAtr structures
    //------------------------------------------------------------------- 
    typedef sequence <CtntAtr> CtntAtrLst;

    //-------------------------------------------------------------------
    // Typedef: CtntPvdrAtr
   // Function: This structure represents a Content Provider object's 
   //           attributes.
   // They are defined as follows:
   //    ctntPvdrOR     - The CtntPvdr's Object Reference
   //    name           - A name given to the CtntPvdr
   //    desc           - A description of the given to the CtntPvdr (optional)
    //------------------------------------------------------------------- 
   struct  CtntPvdrAtr
   {
     CtntPvdr ctntPvdrOR; 
     string   name;   
     string   desc;   
   };
   
    //-------------------------------------------------------------------
    // Typedef: CtntPvdrAtrLst
    // Function: A list of CtntPvdrAtr structures
    //-------------------------------------------------------------------
    typedef sequence <CtntPvdrAtr> CtntPvdrAtrLst;

   //-------------------------------------------------------------------
   // Typedef: LgCtntAtr
   // Function: Contains information about logical content
   // They are defined as follows:
   //    lgCtntOR    - The logical content's Object Reference
   //    name        - A name given to the logical content
   //    desc        - An optional description of the given to the 
   //		       logical content
   //    createDate  - The date the logical content was created
   //    msecs       - The runtime length of all the clips in milliseconds
   //		       (calculated)
   //	 maxSugBufSz - The maximum suggested buffer size of the clips.
   //    maxRate     - The maximum bitrate of all the clips.
   //    numClips    - The number of clips in the logical content (calculated)
   //    cookie      - The assetCookie for this logical Content.
   //    longFmt     - If false, then ONLY the name, lgCtntOR, cookie 
   //		       and numClips fields are valid. If true, everything 
   //		       is valid including the clip and content attribut lists.
   //    clipAtrLst  - If longFmt is true, this list is filled in with a list 
   //  	 	       of all the clips assigned to this logical content.
   //    ctntAtrLst  - If longFmt is true, this list is filled in with the 
   //                  content information which corresponds to the clips in 
   //                  clipAtrLst. There is a one to one correspondence of 
   //		       the items in this list weith the items in the clipAtrLst 
   //		       list.
   //-------------------------------------------------------------------   
   struct LgCtntAtr
   {
     LgCtnt lgCtntOR; 
     string name;
     string desc;             // Not valid when longFmt is false
     string createDate;       // Not valid when longFmt is false
     long msecs;              // Not valid when longFmt is false
     long maxSugBufSz;        // Not valid when longFmt is false
     long maxRate;            // Not valid when longFmt is false
     long numClips; 
     mkd::assetCookie cookie;
     boolean longFmt;
     ClipAtrLst   clipAtrLst; // Not valid when longFmt is false
     CtntAtrLst   ctntAtrLst; // Not valid when longFmt is false
   };

    
    //-------------------------------------------------------------------
    // Typedef: LgCtntAtrLst
    // Function: A list of LgCtnt attribute structures
    //-------------------------------------------------------------------
    typedef sequence <LgCtntAtr> LgCtntAtrLst;
    


};                                                             /* module mza */
    
   
//--------------------------------------------------------------------
// Module: mza
// Function:
//   This module is responsible for all database operations for the 
//   video server.
//--------------------------------------------------------------------
module mza 
{

   //----------------------------------------------------------------------
   // Interface: LgCtnt
   // Function:
   //   Allows for manipulation of LgCtnt objects. Logical content objects
   //   contains clips. Each clip contained in a logical content has a 
   //   position. The position defines its sequence relative to other clips 
   //   in the logical content. The position starts at 1 and increases to  
   //   the number of clips assigned to the logical content. 
   //----------------------------------------------------------------------
   
   interface LgCtnt 
   { 
    
    //-------------------------------------------------------------------
    //   ATTRIBUTES
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //   Attribute: name
    //		A unique name for identifying the logical content.
    //   Attribute: desc
    //		An optional description about the logical content.
    //   Attribute: msecs
    //		The total runtime in milliseconds of the entire logical 
    //          content.
    //          This is calculated based on the clips contained in the logical 
    //          content and is therefore readonly.
    //   Attribute: numClips
    //		The number of clips assigned to the logical content.
    //-------------------------------------------------------------------
    attribute string name;
    attribute string desc;
    readonly attribute long msecs; 
    readonly attribute long numClips; 

    //-------------------------------------------------------------------
    //   METHODS
    //-------------------------------------------------------------------
   
    //-------------------------------------------------------------------
    // Name: mza::LgCtnt::getAtr
    // Function:
    //   Gets all of a LgCtnt's attributes. 
    // Input:
    //   longFmt - If true, the msecs, desc, createDate, clip and content  
    //             information is filled in as well.
    // Output:
    //   lcAtr  - Place holder for the attributes. The attribute structure 
    //		  itself (but not its elements) should be allocated prior 
    //		  to passing it in to this call.
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError, NoLgCtnt
    //--------------------------------------------------------------------
     void getAtr(in boolean longFmt, out LgCtntAtr lcAtr)
       raises (PersistenceError, NoLgCtnt, BadProhib, BadPosition);
   
    //-------------------------------------------------------------------
    // Name: mza::LgCtnt::destroy
    // Function:
    //   Destroys a LgCtnt Object. Will throw an exception if you try 
    //   and destroy Logical Content which is referenced by a clip.
    // Input:
    //   None
    // Output:
    //   None
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     void destroy()
       raises (PersistenceError);
   
    //-------------------------------------------------------------------
    // Name: mza::LgCtnt::getAtrClipByPos
    // Function:
    //   Gets the attributes of a clip at the specified postion. 
    // Input:
    //   position - The position of the clip to return the attributes for.
    // Output:
    //   clipAtr - structure containing the attributes of the clip.
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError, NoLgCtnt
    //--------------------------------------------------------------------
     void getAtrClipByPos(in long position,
			  out ClipAtr clipAtr)
       raises (PersistenceError, NoLgCtnt, BadPosition);
   
    //-------------------------------------------------------------------
    // Name: mza::LgCtnt::lstAtrClips
    // Function:
    //   Lists all the attributes of all the clips in the logical content. 
    // Input:
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of Clip attribute structures in the LgCtnt. If there are no 
    //   clips, an empty list is returned, but no exception is thrown.
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     ClipAtrLst lstAtrClips(inout Itr iterator)
       raises (PersistenceError, BadPosition);
   
    //-------------------------------------------------------------------
    // Name: mza::LgCtnt::addClip
    // Function:
    //   Adds a new clip to the end of the logical content's list of clips. 
    // Input:
    //   clipOR   - Object Reference of the clip to add.
    // Output:
    //   None
    // Returns:
    //   The position the clip was added to.
    // Raises:
    //   PersistenceError, NoLgCtnt
    //--------------------------------------------------------------------
     long addClip(in Clip clipOR)
       raises (PersistenceError, NoLgCtnt);
   
    //-------------------------------------------------------------------
    // Name: mza::LgCtnt::addClipByPos
    // Function:
    //   Adds a new clip at the specified position. The clip is 
    //   inserted at the specified position and the remaining clips 
    //   are reordered.
    // Input:
    //   clipOR		- OR of the clip to add.
    //   position 	- The position to added it in.
    // Output:
    //   None
    // Returns:
    //   The position the clip was added to.
    // Raises:
    //   PersistenceError, NoLgCtnt
    //--------------------------------------------------------------------
     void addClipByPos(in Clip clipOR,
		       in long position)
       raises (PersistenceError, NoLgCtnt);
   
    //-------------------------------------------------------------------
    // Name: mza::LgCtnt::delClip
    // Function:
    //   Deletes the specified clip from the LgCtnt. 
    // Input:
    //   clipOR		- OR of the clip to delete.
    // Output:
    //   None
    // Returns:
    //   None
    // Raises:
    //   Standard MZA Exceptions as defined in mzacom.idl
    //--------------------------------------------------------------------
     void delClip(in Clip clipOR)
       raises (PersistenceError, NoLgCtnt);
   
    //-------------------------------------------------------------------
    // Name: mza::LgCtnt::delClipByPos
    // Function:
    //   Deletes the clip at the specified position. The clip is 
    //   deleted and the remaining clips are reordered.
    // Input:
    //   position 	- The position of the clip to delete
    // Output:
    //   None
    // Returns:
    //   None.
    // Raises:
    //   Standard MZA Exceptions as defined in mzacom.idl
    //--------------------------------------------------------------------
     void delClipByPos(in long position)
       raises (PersistenceError, NoLgCtnt);
      
   };
    
   //----------------------------------------------------------------------
   // Interface: LgCtntFac
   // Function:
   //   Allows for creation of LgCtnt objects. 
   //----------------------------------------------------------------------
   interface LgCtntFac 
   { 
    //-------------------------------------------------------------------
    // Name: mza::LgCtntFac::create
    // Function:
    //   Creates a new LgCtnt Object with no clips. 
    //   Clips may be added using the addClip methods in the LgCtnt 
    //   interface.
    // Input:
    //   name	- A name from the LgCtnt Object
    //   desc	- An optional description for the object
    // Output:
    //   None
    // Returns:
    //   A new LgCtnt Object Reference
    // Raises:
    //   DataConversion, PersistenceError
    //--------------------------------------------------------------------
      LgCtnt create(in string name, 
		    in string desc)
       raises (DataConversion, PersistenceError);

    //-------------------------------------------------------------------
    // Name: mza::LgCtntFac::createCtnt
    // Function:
    //   Creates a new LgCtnt Object with one clip which references the 
    //   entire content object, and a content object. They will all have the 
    //   same name and a default description. This way one call can be made 
    //   to create a logical content object which can be played.
    // Input:
    //   CtntAtr	- All the stuff to create the objects
    // Output:
    //   clipOR		- The clip object which was created.
    //   ctntOR		- The content object which was created
    // Returns:
    //   A new LgCtnt Object Reference
    // Raises:
    //   DataConversion, PersistenceError
    //--------------------------------------------------------------------
      LgCtnt createCtnt(in CtntAtr ctntAtr, out Clip clipOR, out Ctnt ctntOR)
       raises (DataConversion, PersistenceError);
   }; 
    
   //----------------------------------------------------------------------
   // Interface: LgCtntMgmt
   // Function:
   //   Allows for operations on groups of LgCtnt objects. 
   //----------------------------------------------------------------------
   interface LgCtntMgmt 
   { 

    //-------------------------------------------------------------------
    // Name: mza::LgCtntMgmt::lstAtr
    // Function:
    //   Returns an attribute list of all the LgCtnt objects in the system
    // Input:
    //   longFmt	- If true fills in the Clip and Content information
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of LgCtnt attributes. If no objects are found, an empty list 
    //   is returned and no exception is thrown.
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
      LgCtntAtrLst lstAtr(in boolean longFmt, inout Itr itr)
       raises (PersistenceError);
   

    //-------------------------------------------------------------------
    // Name: mza::LgCtntMgmt::lstAtrByNm
    // Function:
    //   Gets all of a LgCtnt's attributes for a LgCtnt with a  
    //   name like the one provided. May have regex select type wildcards
    //   '*' or '.'. 
    //   If no pattern matching characters given in the name, it will return 
    //   only an exact match.
    // Input:
    //   longFmt	- If true fills in the Clip and Content information
    //   lgCtntName     - The name of the LgCtnt to retrieve
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of LgCtnt attributes. If no objects are found, an empty list 
    //   is returned and no exception is thrown.
    // Raises:
    //   DataConversion, PersistenceError
    //--------------------------------------------------------------------
      LgCtntAtrLst lstAtrByNm(in string lgCtntName, in boolean longFmt, 
			        inout Itr itr)
       raises (DataConversion, PersistenceError, BadPosition, BadProhib);
      
    //-------------------------------------------------------------------
    // Name: mza::LgCtntMgmt::usingDB
    // Function:
    //   Determines if the logical content server is currently using a 
    //   database or not. If the server is not connected to a database, 
    //   behavior is modified significantly. There are only a few methods 
    //   in the LgCtntMgmt interface which do anything. All other methods 
    //   in the other interfaces in this file do nothing. They just return 
    //   with out performing any action. The methods which actually do 
    //   something are: LgCtntMgmt::lstAtr and LgCtntMgmt::lstAtrByNm. 
    //
    //   When the content server is running without a database, it will query 
    //   mds to find available tag files. It turns these tag files into 
    //   psuedo logical content. The logical content created will have one 
    //   clip which includes the entire tag file. Queries by name must use 
    //   mds filename syntax (/mds/<volume>/<tagfilename>.
    //
    // Input:
    //   None
    // Output:
    //   None
    // Returns:
    //   TRUE if connected to a database
    // Raises:
    //   Standard MZA Exceptions as defined in mzacom.idl
    //--------------------------------------------------------------------
      boolean usingDB();

   }; 
 
   //----------------------------------------------------------------------
   // Interface: Clip
   // Function:
   //   Allows for manipulation of clip objects. Clips are segments of 
   //   physical content defined by a start and stop position.
   //----------------------------------------------------------------------

   interface Clip
   { 
    
    //-------------------------------------------------------------------
    //   ATTRIBUTES
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //    Attribute: clipOR   
    //		The clip's Object Reference
    //    Attribute: ctntOR   
    //		The Object Reference of the Content this clip is from
    //    Attribute: name     
    //		A name given to the clip
    //    Attribute: desc     
    //		A description of the given to the clip (optional)
    //    Attribute: startPos 
    //		The starting position of this clip (see mkd.idl).
    //    Attribute: stopPos  
    //		The ending position of this clip (see mkd.idl).
    //-------------------------------------------------------------------
    attribute Ctnt ctntOR;
    attribute string name;
    attribute string desc;
    attribute mkd::pos startPos;
    attribute mkd::pos stopPos;

     //-------------------------------------------------------------------
    //   METHODS
    //-------------------------------------------------------------------

   
    //-------------------------------------------------------------------
    // Name: mza::Clip::getAtr
    // Function:
    //   Retrieves a clip's attributes.
    // Input:
    //   None
    // Output:
    //   clipAtr	A filled in Clip Attribute structure. 
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     void getAtr(out ClipAtr clipAtr)
       raises (PersistenceError);
   
    //-------------------------------------------------------------------
    // Name: mza::Clip::destroy
    // Function:
    //   Destroys a clip. If a clip is used in any logical content, it 
    //   will not be destroyed and will throw mza::PersistentStore Exception
    // Input:
    //   None
    // Output:
    //   None
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     void destroy()
       raises (PersistenceError);
   }; 
    
   //----------------------------------------------------------------------
   // Interface: ClipFac
   // Function:
   //   Allows for creation of clip objects. 
   //----------------------------------------------------------------------
   interface ClipFac
   { 

     //-------------------------------------------------------------------
     // Name: mza::ClipFac::create
     // Function:
     //   Creates a new clip. 
     // Input:
     //   ctnt		A content Object Reference that the clip is for.
     //   name		The name of the new clip
     //   desc		An optional description of what the clip is.
     //   startPos	The start position of the clip.  
     //   stopPos	The stop position of the clip. 
     // Output:
     //   None
     // Returns:
     //   A new Clip Object Reference
     // Raises:
     //   DataConversion, PersistenceError
     //--------------------------------------------------------------------
     Clip create(in Ctnt     ctnt,
		 in string   name, 
		 in string   desc,
		 in mkd::pos startPos,
		 in mkd::pos stopPos)
       raises (DataConversion, PersistenceError);
   }; 
    
   //----------------------------------------------------------------------
   // Interface: ClipMgmt
   // Function:
   //   Allows for retrieving Clip objects. 
   //----------------------------------------------------------------------
   interface ClipMgmt
   { 

    //-------------------------------------------------------------------
    // Name: mza::ClipMgmt::lstAtr
    // Function:
    //   returns an attribute list of all the Clip objects in the system
    // Input:
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of Clip attributes. If no objects are found, an empty list 
    //   is returned and no exception is thrown.
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     ClipAtrLst lstAtr(inout Itr iterator)
      raises (PersistenceError, BadPosition);
   
    //-------------------------------------------------------------------
    // Name: mza::ClipMgmt::lstAtrByCtnt
    // Function:
    //   returns an attribute list of all the Clip objects in the system 
    //   which reference a specified piece of content
    // Input:
    //   ctnt		- The Ctnt to which the clips point. 
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of Clip attributes. If no objects are found, an empty list 
    //   is returned and no exception is thrown.
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     ClipAtrLst lstAtrByCtnt(in Ctnt ctnt, 
			     inout Itr iterator)
      raises (PersistenceError, BadPosition);
   
    //-------------------------------------------------------------------
    // Name: mza::ClipMgmt::lstAtrByNm
    // Function:
    //   Gets all of a Clip's attributes for a Clip with the specified 
    //   name. If pattern matching characters are used, more than clip 
    //   may be returned. Will recognize only the '*' and '.' pattern 
    //   matching expressions. Escapes for these are not recognized when 
    //   using a database.
    // Input:
    //   name - The name of the Clip to retrieve
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of Clip attributes. If no objects are found, an empty list 
    //   is returned and no exception is thrown.
    // Raises:
    //   DataConversion, PersistenceError
    //--------------------------------------------------------------------
     ClipAtrLst lstAtrByNm(in string name, inout Itr iterator)
       raises (DataConversion, PersistenceError, BadPosition);
   };


   //----------------------------------------------------------------------
   // Interface: Ctnt
   // Function:
   //   Methods for Content. The Ctnt Object describes the physical content. 
   //----------------------------------------------------------------------
   interface Ctnt
   { 

    //-------------------------------------------------------------------
    //   METHODS
    //-------------------------------------------------------------------
  
   
    //-------------------------------------------------------------------
    // Name: mza::Ctnt::updateStats
    // Function:
    //   Updates the Content Length, Size and Status
    // Input:
    //   len      - Total Length of the file in Bytes
    //   msecs    - Total duration of the content in milliseconds
    //   sugBufSz - The suggested buffer size for the content
    //   status   - Status of the content, (static, rolling, feed, ...)
    // Output:
    //   None
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     void updateStats(in long long len, in long msecs, 
		in long sugBufSz, in string status)
       raises (PersistenceError);
   
    //-------------------------------------------------------------------
    // Name: mza::Ctnt::updateSugBufSz
    // Function:
    //   Updates the Content suggested buffer size
    // Input:
    //   sugBufSz - The suggested buffer size for the content
    // Output:
    //   None
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     void updateSugBufSz(in long sugBufSz)
       raises (PersistenceError);

    //-------------------------------------------------------------------
    // Name: mza::Ctnt::updateTimes
    // Function:
    //   Updates the First and Last Time Tags for the content
    // Input:
    //   firstTime - Time of the first tag in the file
    //   lastTime  - Time of the last tag in the file
    // Output:
    //   None
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     void updateTimes(in long long firstTime, in long long lastTime)
       raises (PersistenceError);


    //-------------------------------------------------------------------
    // Name: mza::Ctnt::setAtr
    // Function:
    //   Sets all the attributes for the Ctnt object
    // Input:
    //   ctntAtr - structure containing the Ctnt's attributes
    // Output:
    //   None
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     void setAtr(in CtntAtr ctntAtr)
       raises (PersistenceError, DataConversion);
   
    //-------------------------------------------------------------------
    // Name: mza::Ctnt::getAtr
    // Function:
    //   Gets all the attributes for the Ctnt object
    // Input:
    //   None
    // Output:
    //   ctntAtr - structure containing the Ctnt's attributes
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     void getAtr(out CtntAtr ctntAtr)
       raises (PersistenceError);
   
    //-------------------------------------------------------------------
    // Name: mza::Ctnt::destroy
    // Function:
    //   Destroys a content object. If the Ctnt is referenced by any clip, 
    //   it will not be destroyed and throw a PersistenceError Exception.
    // Input:
    //   cascade   If true then try and destroy the tagfile and it's related 
    //		   content as well. Be careful!
    // Output:
    //   None
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     void destroy(in boolean cascade)
       raises (PersistenceError);
   }; 
    
   //----------------------------------------------------------------------
   // Interface: CtntFac
   // Function:
   //   Methods for Content creation.  
   //----------------------------------------------------------------------
   interface CtntFac
   { 

    //-------------------------------------------------------------------
    // Name: mza::CtntFac::create
    // Function:
    //   Create new Ctnt objects
    // Input:
    //    ctntAtr - All the content stuff in an attribute structure.
    // Output:
    //   None
    // Returns:
    //   New Content Object Reference
    // Raises:
    //   DataConversion, PersistenceError
    //--------------------------------------------------------------------
     Ctnt create(in CtntAtr ctntAtr)
       raises (DataConversion, PersistenceError);
   }; 
    
   //----------------------------------------------------------------------
   // Interface: CtntMgmt
   // Function:
   //   methods to operate on groups of Ctnt Objects.  
   //----------------------------------------------------------------------
   interface CtntMgmt
   { 

    //-------------------------------------------------------------------
    // Name: mza::CtntMgmt::lstAtr
    // Function:
    //   returns an attribute list of all the Ctnt objects in the system
    // Input:
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of content attributes. If no objects are found, an empty list 
    //   is returned and no exception is thrown.
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     CtntAtrLst lstAtr(inout Itr itr)
		raises (PersistenceError, BadProhib);

    //-------------------------------------------------------------------
    // Name: mza::CtntMgmt::lstAtrByNm
    // Function:
    //   returns an attribute list of all the Ctnt objects in the system
    //   with a name which looks like the name provided. You may use regex
    //   style pattern matching '*' and '.' characters in the name. If no 
    //   pattern matching characters are used, only an exact match is returned.
    // Input:
    //   name           - The name to search for.
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of content attributes. If no objects are found, an empty list 
    //   is returned and no exception is thrown.
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     CtntAtrLst lstAtrByNm(in string name, inout Itr itr)
		raises (PersistenceError, BadProhib);
   
    //-------------------------------------------------------------------
    // Name: mza::CtntMgmt::lstAtrByFileNm
    // Function:
    //   returns an attribute list of all the Ctnt objects in the system
    //   with a filename which looks like the name provided. You may use regex
    //   style pattern matching '*' and '.' characters in the name. If no 
    //   pattern matching characters are used, only an exact match is returned.
    // Input:
    //   filename       - The filename to search for.
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of channel attributes. If no objects are found, an empty list 
    //   is returned and no exception is thrown.
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
     CtntAtrLst lstAtrByFileNm(in string filename, inout Itr itr)
		raises (PersistenceError, BadProhib);
      
   
   }; 

   //----------------------------------------------------------------------
   // Interface: CtntPvdr
   // Function:
   //   Implements funcionality of the CtntPvdr Object. Content Providers 
   //   are usually those organizations which develop or own a piece of 
   //   content.
   //----------------------------------------------------------------------
   interface CtntPvdr
   {
   
    //-------------------------------------------------------------------
    //   ATTRIBUTES
    //-------------------------------------------------------------------

    //-------------------------------------------------------------------
    //    Attribute: name           
    //   	 A name given to the CtntPvdr
    //    Attribute: desc           
    //   	 A description of the given to the CtntPvdr (optional)
    //-------------------------------------------------------------------

    attribute string name;   
    attribute string desc;
   
    //-------------------------------------------------------------------
    // Name: mza::CtntPvdr::getAtr
    // Function:
    //   Gets all of a CtntPvdr's attributes. 
    // Input:
    //   None
    // Output:
    //   ctntpvdrAtr  - Place holder for the attributes.
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
        void getAtr(out CtntPvdrAtr ctntpvdrAtr)
	   raises (PersistenceError);
   
    //-------------------------------------------------------------------
    // Name: mza::CtntPvdr::destroy
    // Function:
    //   Destroys a CtntPvdr object. If the CtntPvdr is referenced by any  
    //   Ctnt, it will not be destroyed.
    // Input:
    //   None
    // Output:
    //   None
    // Returns:
    //   Nothing
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
        void destroy()
	   raises (PersistenceError);
   };
   
   //----------------------------------------------------------------------
   // Interface: CtntPvdrFac
   // Function:
   //   Method to create a new CtntPvdr
   //----------------------------------------------------------------------
   interface CtntPvdrFac
   {

    //-------------------------------------------------------------------
    // Name: mza::CtntPvdrFac::create
    // Function:
    //   Create new CtntPvdr objects
    // Input:
    //    name       - A name given to the CtntPvdr
    //    desc       - An optional description of the given to the 
    //		       CtntPvdr
    // Output:
    //   None
    // Returns:
    //   Returns a new CtntPvrd Object Reference
    // Raises:
    //   DataConversion, PersistenceError
    //--------------------------------------------------------------------
        CtntPvdr create(in string name,
		        in string desc)
	   raises (DataConversion, PersistenceError);
   };	
   
   //----------------------------------------------------------------------
   // Interface: CtntPvdrMgmt
   // Function:
   //   Methods to manage groups of CtntPvdr objects
   //----------------------------------------------------------------------
   interface CtntPvdrMgmt
   {
   
    //-------------------------------------------------------------------
    // Name: mza::CtntPvdrMgmt::lstAtr
    // Function:
    //   returns an attribute list of all the CtntPvdr objects in the system
    // Input:
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of CtntPvdr attributes. If no objects are found, an empty list 
    //   is returned and no exception is thrown.
    // Raises:
    //   PersistenceError
    //--------------------------------------------------------------------
      CtntPvdrAtrLst lstAtr(inout Itr itr)
	   raises (PersistenceError);
   
    //-------------------------------------------------------------------
    // Name: mza::CtntPvdr::getAtrByNm
    // Function:
    //   Gets all of a CtntPvdr's attributes for a Ctnt with the specified 
    //   name. No pattern matching is provided with this method. It 
    //   returns only and exact match.
    // Input:
    //   name - The name of the CtntPvdr to retrieve
    // Output:
    //   ctntPvdrAtr  - Place holder for the attributes.
    // Returns:
    //   Nothing
    // Raises:
    //   DataConversion, PersistenceError
    //--------------------------------------------------------------------
      void getAtrByNm(in string name,
		      out CtntPvdrAtr ctntPvdrAtr)
	   raises (DataConversion, PersistenceError);
   };

   //----------------------------------------------------------------------
   // Interface: BlobMgmt
   // Function:
   //   Allows for operations on groups of Blobs. Blob Management is currently 
   //   supported by querying mds. There is no blob support in the database, 
   //   and blobs are not necessarily tagfiles, so no operations which would 
   //   fill in information likely to come from a tagfile will be supported. 
   //   To make this as simple as possible, these two methods will return a 
   //   list of LgCtntAtr structures, but with none of the long format 
   //   information filled in. This means only the name and asset cookie data
   //   will be filled in. The longFmt will always be FALSE, and numClips is
   //   meaningless.
   //   Since information is retrieved from MDS, MDS style asset cookies will 
   //   be returned. This is then the primary interface for retrieving MDS 
   //   style asset cookies from the cotnent service.
   //----------------------------------------------------------------------
   interface BlobMgmt 
   { 

    //-------------------------------------------------------------------
    // Name: mza::BlobMgmt::lstAtrByNm
    // Function:
    //   Gets all of a Blob's attributes with a name like the one provided
    //   May have  wildcards '*' or '.'.  If no pattern matching characters 
    //   given in the name, it will return only an exact match.
    //   The name given must be an mds style file name such as:
    //   /mds/video/*.mpg ro /mds/*/*. Be careful when using wildcards as
    //   a large number of files may be returned and it may take a long time
    //   to query MDS.
    //   
    // Input:
    //   blobName        - The name of the Blob to retrieve
    //   itr		- Specifies the maximum number of items to return 
    //			  as well as which item to start with. See comments 
    //			  in mzacom on Itr.
    // Output:
    //   itr		- Updated values for the number of items 
    //			  actually returned and the new position.
    // Returns:
    //   List of LgCtnt attributes. If no objects are found, an empty list 
    //   is returned and no exception is thrown. Only the information available
    //   when longFmt is FALSE in the LgCtntAtr structure is returned.
    //   (see the description of LgCtntAtr).
    // Raises:
    //   DataConversion
    //--------------------------------------------------------------------
      LgCtntAtrLst lstAtrByNm(in string blobName, inout Itr itr)
       raises (DataConversion);
      

   }; 

 
};                                                             /* module mza */

#endif                                                         /* MZA_LGCTNT */
