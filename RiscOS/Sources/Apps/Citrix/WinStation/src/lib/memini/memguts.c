/*=============================================================================
==   Global Variables Used
=============================================================================*/

#include "../bini/biniguts.c"

PCHAR gIniProfile;   //MEMINI-styled buffered profile for miGets
PCHAR gIniSection;   //BINI-styled buffered section bgets

/*=============================================================================
==   Local Functions Used
=============================================================================*/

//////////////////////////////////////////////////////////////////////////////
// Routines to manipulate entire profiles

/********************************************************************************
 *
 * miSetProfilePointer
 *
 *    if the Magic Number in the buffer (the first character) is set to NULL
 *       then we know that we have a MEMINI-styled buffered profile which
 *       is needed for the MiGet Routines. Make a copy of this buffer since
 *       the IPC layer will free it after loading the session.
 *    if the Magic Number is not NULL then we have an old-styled Buffered INI
 *       top-end which will need to use the bINI routines to do gets.
 *       signify this by setting out global profile pointer to NULL
 *
 *******************************************************************************/

int WFCAPI miSetProfilePointer( PCHAR pProfile ) 
{
   int   BufferSize = 0;
   PCHAR pTemp = NULL;

   if (pProfile[0]== '\0') { 
      
      /*
       * Determine the size of the Buffered Profile 
       */ 
      pTemp = (pProfile + 1);    //plus one to skip over the magic number
      for (BufferSize = 0; ; BufferSize++) {
         if ( !*pTemp++ && !*pTemp ) {
            BufferSize += 2;
            break;
         }
      }
      
      gIniProfile = (PCHAR) malloc(BufferSize);
      memcpy(gIniProfile, (pProfile + 1), BufferSize);
   
   } else {
      gIniProfile = NULL;
   }

   return(BufferSize);
}

/********************************************************************************
 *
 * miReleaseProfile
 *
 *    Frees the Global Profile Buffer.
 *
 *******************************************************************************/

int WFCAPI miReleaseProfile()
{
   if (gIniProfile != NULL) {
      free(gIniProfile);
   }
   return(0);
}

/********************************************************************************
 *
 * miSetSectionPointer
 *
 *    set the global buffered section pointer for BINI routines to the
 *    location passed in by the caller. The caller will free the memory 
 *    after used.
 *
 *******************************************************************************/

int WFCAPI miSetSectionPointer( PCHAR pSection ) 
{
   gIniSection = pSection;

   return(0);
}

//////////////////////////////////////////////////////////////////////////////
// Top-to-bottom info fetch routines

/********************************************************************************
 *
 * miGetPrivateProfileString
 *
 *    Returns the size of the ReturnBuffer Parameter
 *
 *******************************************************************************/ 

int WFCAPI miGetPrivateProfileString(
    PCHAR     lpszSection,        // [in]     name of section to query
    PCHAR     lpszEntry,          // [in]     name of entry to query
    PCHAR     lpszDefault,        // [in]     string to use if no entry
    PCHAR     lpszReturnBuffer,   // [out]    pointer to buffer for data
    int       cbSize              // [in]     maximum number of characters that can be written to lpszReturnBuffer
    )
{
   int      rc = strlen(lpszDefault);
   int      offset, len;
   
   char     CurrSectionName[MAX_INI_LINE];
   char     CurrEntryName[MAX_INI_LINE];

   BOOL     SectionFound = FALSE;
   BOOL     EndProfile   = FALSE;
   BOOL     EndEntryLoop = FALSE;
   PCHAR    pTempSec, pTempEntry, pRight, pEqual;

   //initialize the return buffer to default
   memcpy(lpszReturnBuffer, lpszDefault, rc);
   lpszReturnBuffer[rc] = 0;

   if (gIniProfile == NULL) {
      
      rc = bGetPrivateProfileString(gIniSection, lpszEntry, lpszDefault, 
                                    lpszReturnBuffer, cbSize);
   } else {

      pTempSec = gIniProfile;
      while (!EndProfile && !SectionFound) {   

         //search for the section 
         strcpy(CurrSectionName, (pTempSec + 1));  //plus 1 to skip the [
         pRight = strchr(CurrSectionName, ']');
         *pRight++ = 0;
         
         offset = atoi(pRight);
         
         if(!stricmp( CurrSectionName, lpszSection)) {

            SectionFound = TRUE;
            pTempEntry = pTempSec + strlen(pTempSec) + 1;

            while (!EndEntryLoop) { 
            
               //search for entry
               len = strlen(pTempEntry);
               strcpy(CurrEntryName, pTempEntry);
               pEqual = strchr(CurrEntryName, '=');
               *pEqual++ = 0;
               
               if (!stricmp( CurrEntryName, lpszEntry)) {
                  
                  EndEntryLoop = TRUE;       //exit since we got entry we need
                  rc = strlen(pEqual);
                  if (rc <= cbSize) {
                     memcpy(lpszReturnBuffer, pEqual, rc);
                     lpszReturnBuffer[rc] = 0;
                  }
               }
               
               pTempEntry += (len + 1);
               
               if(!*pTempEntry) {
                  EndEntryLoop = TRUE;          //end of profile altogether
               } else {
                  if(pTempEntry[0] == '[') {
                     EndEntryLoop = TRUE;       //reached next section entry unfound
                  }
               }

            }
         }
         if (offset) {
            pTempSec += (offset + strlen(pTempSec));
         } else {
            EndProfile = TRUE;
         }
      }
   }

   return(rc);                                   
}


/********************************************************************************
 *
 * miGetPrivateProfileInt
 *
 *******************************************************************************/ 

int WFCAPI miGetPrivateProfileInt(
    PCHAR       lpszSection,        // [in]     name of section to query
    PCHAR       lpszEntry,          // [in]     name of entry to query
    int         iDefault            // [in]     default value to use
    )
{
   int   iReturn = iDefault;
   PCHAR buffer;

   buffer = (PCHAR) malloc(MAX_INI_LINE);
   
   if (miGetPrivateProfileString(lpszSection, lpszEntry,
                                 "", buffer, MAX_INI_LINE)) {
      //key found, convert the string into an integer
        iReturn = _htoi(buffer);
   }
   
   free(buffer);
   return(iReturn);                            
}

/********************************************************************************
 *
 * miGetPrivateProfileLong
 *
 *******************************************************************************/ 

long WFCAPI miGetPrivateProfileLong(
     PCHAR       lpszSection,        // [in]     name of section to query
     PCHAR       lpszEntry,          // [in]     name of entry to query
     long        lDefault            // [in]     default value to use
    )
{
   ULONG lReturn = lDefault;
   PCHAR buffer;
   
   buffer = (PCHAR) malloc(MAX_INI_LINE);

   if (miGetPrivateProfileString(lpszSection, lpszEntry,
                                 "", buffer, MAX_INI_LINE)) {
      //key found, convert the string into a long
      lReturn = _htol(buffer);
   }

   free(buffer);
   return(lReturn);
}

/********************************************************************************
 *
 * miGetPrivateProfileBool
 *
 *******************************************************************************/ 

BOOL WFCAPI miGetPrivateProfileBool(
     PCHAR       lpszSection,        // [in]     name of section to query
     PCHAR       lpszEntry,          // [in]     name of entry to query
     BOOL        fDefault            // [in]     default value
    )
{
   BOOL  fReturn = fDefault;
   PCHAR pBuffer;

   pBuffer = (PCHAR) malloc(MAX_INI_LINE);

   if (miGetPrivateProfileString(lpszSection, lpszEntry,
                                 "", pBuffer, MAX_INI_LINE)) {
      if ( !stricmp(pBuffer,"on") || !stricmp(pBuffer,"yes") ||
           !stricmp(pBuffer,"true") ) {
       fReturn = TRUE;
      } else {
       fReturn = FALSE;
      }
   }

   free(pBuffer);
   return(fReturn);
}


//////////////////////////////////////////////////////////////////////////////
// Internal functions
//////////////////////////////////////////////////////////////////////////////

