/*******************************************************************************
*
*   WFBRUSH.C
*
*   Thin Wire Windows - Brush Code
*
*   Copyright (c) Citrix Systems Inc. 1993-1996
*
*   Author: Jeff Krantz (jeffk)
*
*   $Log$
*  
*     Rev 1.6   15 Apr 1997 18:16:50   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.5   03 Jan 1996 13:33:48   kurtp
*  update
*  
*******************************************************************************/

#include "wfglobal.h"



//makes the objobj entry having index number "index"
//and pointer lpbrushobjobj the mru on the list
//assumes already on the list!!!!
VOID  brush_makemru(LPBRUSHOBJOBJ lpbrushobjobj, BYTE index)
{

   //if not already first on chain then take off and put back in
   if (lpbrushobjobj->previous != 112) {
      //take off chain
      brushobjobj[lpbrushobjobj->previous].next = lpbrushobjobj->next;
      brushobjobj[lpbrushobjobj->next].previous = lpbrushobjobj->previous;
      //put back in
      lpbrushobjobj->previous = 112;
      brushobjobj[brushobjobj[112].next].previous = index;
      lpbrushobjobj->next = brushobjobj[112].next;
      brushobjobj[112].next = index;
   }
}

//makes the objobj entry having index number "index"
//and pointer lpbrushobjobj the mru on the list
//assumes not on the list so only has to be put on!!
VOID  brush_newmru(LPBRUSHOBJOBJ lpbrushobjobj, BYTE index)
{
   lpbrushobjobj->previous = 112;
   brushobjobj[brushobjobj[112].next].previous = index;
   lpbrushobjobj->next = brushobjobj[112].next;
   brushobjobj[112].next = index;

}


//we need to delete the least recently used brush object handle
//because we have the maximum number of brushes realized
//take last brush in chain off of the list and deleteobject it
//count maintained by the caller
//NOTE: BECAUSE OF MANAGEMENT OF BrushDIB, we should only use this
//when MAXBRUSHREALIZED brushes have been realized
//The objobj entry of the deleted entry is returned.
//That dib_index of this objobj entry should be used to realize the next brush
BYTE brush_deletelru()
{
   BYTE  btemp1, btemp2;
   BOOL  jretcode;

   btemp1 = brushobjobj[113].previous;       //removing index btemp1
   btemp2 = brushobjobj[btemp1].previous;
   brushobjobj[btemp2].next = 113;
   brushobjobj[113].previous = btemp2;
   jretcode = DeleteObject(brushobjobj[btemp1].hbr);
   ASSERT(jretcode,0);
   brushobjobj[btemp1].hbr = NULL;
   TRACE((TC_TW, TT_TW_BRUSH,"TW: Deleting lru brush object objobj %u\n", (int) btemp1));
   return(btemp1);
}

//we need to delete a brush object handle entry
//because we are going to reuse the objobj entry
//lpbrushobjobj and index define the entry to be deleted
//count maintained by the caller
//the dib_index used by this entry should be reused
void brush_deletebrush(LPBRUSHOBJOBJ lpbrushobjobj, BYTE index)
{
   BYTE  btemp1, btemp2;
   BOOL  jretcode;

   btemp1 = lpbrushobjobj->previous;
   btemp2 = lpbrushobjobj->next;

   brushobjobj[btemp1].next = btemp2;
   brushobjobj[btemp2].previous = btemp1;

   jretcode = DeleteObject(lpbrushobjobj->hbr);
   ASSERT(jretcode,0);
   lpbrushobjobj->hbr = NULL;
   TRACE((TC_TW, TT_TW_BRUSH,"TW: Deleting reused brush object objobj %u\n", (int) index));
   return;
}



//realize_brush realizes a brush handle based on data pointed to by lpbrush...
//rotation, and color.  Rotation and color same format as brushobjobj
//It returns the brush handle to the caller.
//the dib_handle and dib_address are used to create the 16 color
//dib that the brush is made up of.

//it updates the rotation state of the DC if required and keeps state
//in dcstate.

//jk256 - change color to a word. same format as brushobjobj
//        add color_type parm to end. if 0 16 color and if 1 256 color.
//          if color parm is nonzero then its a 2 color and ignore  color_type
//          color_type is not used in 16 color mode

HBRUSH   realize_brush(HDC device, LPBYTE lpbrushdata, BYTE rotation,
                       WORD color, HGLOBAL dib_handle, LPVOID dib_address,
                       HWND hwnd, UINT color_type)
{
   UINT  xRotate, yRotate, oneColor, zeroColor, i, j, mask;
   UINT  dcxRotate,dcyRotate;
   BOOL  jRetcode;
   LPBYTE lpDest, lpDestTemp;
   BYTE  input, output;
   HBRUSH hbr;

   xRotate = 0x07 & rotation;
   yRotate = ((UINT) (0x38 & rotation)) >> 3;

   if (vColor == Color_Cap_256) {      //256 color mode path
      lpDest = ((LPBYTE) dib_address) + sizeof(bitmapinfo_8BPP_PALETTE);

      lpDest += 56;     //because of inverted dib madness

      //different logic to bring image information in depending on
      //whether 16 color or 2 color

      if ((color == 0) && (color_type == 0)) {
         //16 color bitmap
         //need to translate through the default
         //16 color palette map

         TRACE((TC_TW, TT_TW_BRUSH,"TW: 16 color brush\n"));

         for (i=0; i<8 ; i++ ) {

            for (j=0; j<4 ;j++ ) {
               oneColor = *(lpbrushdata + j);

               *(lpDest + (2*j)) = convert_default16to256[oneColor >> 4];
               *(lpDest + ((2*j) + 1)) = convert_default16to256[oneColor & 0x0f];
            }

            lpDest -= 8;
            lpbrushdata += 4;
         }

      }
      else if ((color == 0) && (color_type == 1)) {
         //256 color bitmap - indexes are absolute

         TRACE((TC_TW, TT_TW_BRUSH,"TW: 256 color brush\n"));

         for (i=0; i<8 ; i++ ) {
            memcpy(lpDest, lpbrushdata, 8);
            lpDest -= 8;
            lpbrushdata += 8;
         }

      }
      else {
         //its a 2 color dib
         //i will have the byte count - 8 bytes
         //mask will keep track of where we are in the byte
         //lpDest will point to the current output byte dealing with

         oneColor =  (BYTE) color;
         zeroColor = (BYTE) (color >> 8);

         TRACE((TC_TW, TT_TW_BRUSH,"TW: 2 color brush oneColor = %u, zeroColor = %u\n",
                           oneColor,zeroColor));

         for (i=0 ; i < 8 ; i++ ) {
            input = *(lpbrushdata + i);
            mask = 0x80;

            lpDestTemp = lpDest;

            while (mask != 0) {

               if ( input & mask) {
                  *lpDestTemp = (BYTE) oneColor;
               }
               else *lpDestTemp = (BYTE) zeroColor;

               mask >>= 1;
               lpDestTemp++;
            }
            lpDest -= 8;
         }

      }
   }
   else {   //16 color mode
#ifdef THINPAL
      lpDest = ((LPBYTE) dib_address) + sizeof(bitmapinfo_4BPP_PALETTE);
#else
      lpDest = ((LPBYTE) dib_address) + sizeof(bitmapinfo_4BPP_RGBQUAD);
#endif

      lpDest += 28;     //because of inverted dib madness

      //different logic to bring image information in depending on
      //whether 16 color or 2 color

      if (color == 0) {
         //its 16 color
         TRACE((TC_TW, TT_TW_BRUSH,"TW: 16 color brush\n"));
         //we can't use the following line of code because WIN16 does not support
         //a right side up DIB so we have to put the scanlines in backwards
         //memcpy(lpDest, lpbrushdata, 32);

         for (i=0; i<8 ; i++ ) {
            memcpy(lpDest, lpbrushdata, 4);
            lpDest -= 4;
            lpbrushdata += 4;
         }

      }
      else {
         //its a 2 color dib
         //i will have the byte count - 8 bytes
         //mask will keep track of where we are in the byte
         //lpDest will point to the current output byte dealing with

         oneColor = ((UINT) color) >> 4;
         zeroColor = ((UINT) color) & 0x0f;

         TRACE((TC_TW, TT_TW_BRUSH,"TW: 2 color brush oneColor = %u, zeroColor = %u\n",
                           oneColor,zeroColor));

         for (i=0 ; i < 8 ; i++ ) {
            input = *(lpbrushdata + i);
            mask = 0x80;

            lpDestTemp = lpDest;

            while (mask != 0) {
               output = 0;

               if ( input & mask) {
                  output |= ((BYTE) oneColor) << 4;
               }
               else output |= ((BYTE) zeroColor) << 4;

               mask >>= 1;
               if ( input & mask) {
                  output |= (BYTE) oneColor;
               }
               else output |= (BYTE) zeroColor;

               mask >>= 1;
               *lpDestTemp = output;
               lpDestTemp++;
            }
            lpDest -= 4;
         }

      }
   }


   hbr = CreateDIBPatternBrush(dib_handle,DIB_ColorMode);

   //win16 rotation logic needs to take window position into account#@!$%
   //for win16 the real rotation is dcstate.?BrushOrg combined with current window position

#ifdef WIN32
   dcxRotate = dcstate.xBrushOrg;
   dcyRotate = dcstate.yBrushOrg;

#else
   dcstate.screenULH.x = 0;
   dcstate.screenULH.y = 0;
   ClientToScreen(hwnd, (POINT FAR*) &(dcstate.screenULH));
   dcxRotate = (dcstate.xBrushOrg + dcstate.screenULH.x) & 7;
   dcyRotate = (dcstate.yBrushOrg + dcstate.screenULH.y) & 7;
   TRACE((TC_TW,TT_TW_BRUSH,"TW: current screen ULH of client area is x=%u, y=%u\n",
                  (UINT) dcstate.screenULH.x, (UINT) dcstate.screenULH.y));

#endif

   if ( (dcxRotate != xRotate) ||
                                 (dcyRotate !=  yRotate) ) {
      TRACE((TC_TW, TT_TW_BRUSH,"TW: changing DC brush origin to x = %u, y = %u\n",
                        xRotate, yRotate));

      dcstate.xBrushOrg = xRotate;
      dcstate.yBrushOrg = yRotate;

#ifdef WIN32
      jRetcode = SetBrushOrgEx(device, xRotate, yRotate, NULL);

      ASSERT(jRetcode,0);

#else
      jRetcode = UnrealizeObject(hbr);    //so rotate works for WIN16.  benign for WIN32
      ASSERT(jRetcode,0);

      SetBrushOrg(device, (xRotate + (BYTE) dcstate.screenULH.x) & 7,
                          (yRotate + (BYTE) dcstate.screenULH.y) & 7);

#endif

   }

   return(hbr);

}


//create_brush reads protocol to eventually
//select a brush object into the DC that is passed in as a parm
//replaces assembler routine create_brush, create_nonsolid_brush
//and create_solid_brush
//
//All the solid color brushes are realized at initialization time
//and handles exist for them.
//For the objobj, the last MAXBRUSHREALIZED brushes are realized
//and handles exist for them.  We use lru to figure out which realized brush
//to delete if we need to realize another one.
//
//jk256 - in 256 color mode indexes 0 - 9, and F6 - FF are realized always.
//    only 1 other solid color is realized at a time.  Its handle is kept
//    in hbrsolid256[20] and the solid color index that it represents is in
//    lasthbrsolid256index
//
/////////////////////////////////////////////////////////////////////////////////////
// THIS LOGIC FOR WIN16 ONLY
//NOTE:!!!! In order to get win16 to work all rotations need to be computed relative to the
//       screen instead of the client window.
//we keep track of the current screen coordinates of client ULH in DCstate.
//whenever we need to check desired rotation against current rotation we recompute the
//current rotation based off of the current screen coordinates plus the currently
//selected DC rotation.  This may cause us to have to rerotate.  We only need to update
//the screen coordinates in DCstate whenever we are going to do the check.
//
//in addition, we see if the screen coordinates have changed when we are told to use the last
//realized brush; which may cause us to have to rerotate the brush even though the host
//told us to use it blindly
//
//////////////////////////////////////////////////////////////////////////////////
//
//returns TRUE is no error

BOOL  create_brush(HWND hwnd, HDC device)
{
   BYTE  byte1, byte2, bytetemp;
   LPBRUSHOBJOBJ lpbrushobjobj;
   int   count;
   LPBYTE lpbrushdata;
   BOOL  bretcode, jRetcode;
   UINT  xRotate,yRotate,dcxRotate,dcyRotate;
   HBRUSH tmphbr;
#ifdef WIN16
   POINT currentULH;
#endif
   //jk256
   UINT  cachecount;
   BOOL  j256Color,j256CWrite;



   GetNextTWCmdBytes(&byte1,1);

   if (byte1 == 0x8e) {
      TRACE((TC_TW, TT_TW_BRUSH,"TW: using previously realized brush\n"));

#ifdef WIN16
      //There is a problem that only exists for win16 because in win16 brushes are
      //realized relative to screen coordinates instead of client window coordinates
      //if the client window has been moved and we get this packet then the current
      //selected brush rotation is incorrect if it is a non solid color brush
      //so we have this problem that we need to keep track of the brush that this operation
      //refers to because if the window has been moved since the last operation
      //then we possibly need to rerotate the currently selected brush!!!!!!!!!!!!

      currentULH.x = 0;
      currentULH.y = 0;
      ClientToScreen(hwnd, (POINT FAR*) &currentULH);
      if ((currentULH.x != dcstate.screenULH.x) ||
                     (currentULH.y != dcstate.screenULH.y)) {
         //the client window has moved on the screen, so now we need
         //to rerotate and reselect the currently selected brush if it is nonsolid

         dcstate.screenULH.x = currentULH.x;
         dcstate.screenULH.y = currentULH.y;

         if (dcstate.lastbrush >= 0) {
            //the last selected brush was a nonsolid color brush

            TRACE((TC_TW,TT_TW_BRUSH,"TW: need to potentially rerotate currently selected brush\n"));

            lpbrushobjobj = &brushobjobj[dcstate.lastbrush];

            //this is the required rotation
            xRotate = 0x07 & (lpbrushobjobj->rotation);
            yRotate = ((UINT) (0x38 & (lpbrushobjobj->rotation))) >> 3;


            dcxRotate = (dcstate.xBrushOrg + dcstate.screenULH.x) & 7;
            dcyRotate = (dcstate.yBrushOrg + dcstate.screenULH.y) & 7;
            TRACE((TC_TW,TT_TW_BRUSH,"TW: current screen ULH of client area is x=%u, y=%u\n",
                  (UINT) dcstate.screenULH.x, (UINT) dcstate.screenULH.y));

            if ( (dcxRotate !=  xRotate) ||
                                       (dcyRotate !=  yRotate) ) {
               TRACE((TC_TW, TT_TW_BRUSH,"TW: changing DC brush origin to x = %u, y = %u\n",
                              xRotate, yRotate));

               SelectObject(device,hbrsolid[0]);   //temp operation so we can
                                                   //unrealize the brush we care about
               jRetcode = UnrealizeObject(lpbrushobjobj->hbr);    //so rotate works for WIN16.  benign for WIN32
               ASSERT(jRetcode,0);

               SetBrushOrg(device, (xRotate + (BYTE) dcstate.screenULH.x) & 7,
                          (yRotate + (BYTE) dcstate.screenULH.y) & 7);
               SelectObject(device, lpbrushobjobj->hbr);

            }

         }
      }



#endif

      return(TRUE);
   }

   //check for solid color brush - operation dependent on 16 or 256 color mode
   if (vColor == Color_Cap_256) {
      if (byte1 <= 9) {

         //its a solid color brush that is pre-realized
         SelectObject(device,dcstate.hbr = hbrsolid256[byte1]);
         TRACE((TC_TW, TT_TW_BRUSH,"TW: selecting solid color brush color = %u\n", (int) byte1));
#ifdef WIN16
         dcstate.lastbrush = - (INT) (byte1+1);
#endif
         return(TRUE);
      }

      else if ((byte1 >= 0x80) && (byte1 <= 0x89)) {

         byte1 += 0x76;       //byte1 now has the correct index

         //its a solid color brush that is pre-realized
         SelectObject(device,dcstate.hbr = hbrsolid256[byte1 - 0xec]);
         TRACE((TC_TW, TT_TW_BRUSH,"TW: selecting solid color brush color = %u\n", (int) byte1));
#ifdef WIN16
         dcstate.lastbrush = - (((INT) byte1) + 1);
#endif
         return(TRUE);
      }

      else if (byte1 == 0x8d) {

         //the next byte in the datastream is a solid color brush that is not
         //prerealized
         GetNextTWCmdBytes(&byte1,1);
         if (lasthbrsolid256index != byte1) {
            //we need to realize this solid color brush

            //dont delete previous handle until select new handle in case
            //it is currently selected into the DC

            TRACE((TC_TW, TT_TW_BRUSH,"TW: need to realize new solid color brush %u\n", (int) byte1));

            tmphbr = CreateSolidBrush( PALETTEINDEX(byte1));
            SelectObject(device,dcstate.hbr = tmphbr);
#ifdef WIN16
            dcstate.lastbrush = - (INT) (byte1+1);
#endif
            lasthbrsolid256index = byte1;

            if (hbrsolid256[20] != NULL) {
               //need to delete previously realized brush
               TRACE((TC_TW, TT_TW_BRUSH,"TW: need to delete previously realized solid color brush"));
               DeleteObject(hbrsolid256[20]);
            }
            hbrsolid256[20] = tmphbr;
            return(TRUE);
         }
         else {
            //this solid color brush is already realized

            SelectObject(device,dcstate.hbr = hbrsolid256[20]);
            TRACE((TC_TW, TT_TW_BRUSH,"TW: selecting dynamic already realized solid color brush color = %u\n", (int) byte1));
#ifdef WIN16
            dcstate.lastbrush = - (INT) (byte1+1);
#endif
            return(TRUE);
         }
      }
   }

   else {
      //16 color mode
      if (byte1 <= 0x0f) {
         //its a solid color brush
         SelectObject(device,dcstate.hbr = hbrsolid[byte1]);
         TRACE((TC_TW, TT_TW_BRUSH,"TW: selecting solid color brush color = %u\n", (int) byte1));
#ifdef WIN16
         dcstate.lastbrush = - (INT) (byte1+1);
#endif
         return(TRUE);
      }
   }

   //we can only fall through to here if its a nonsolid color brush

   if (byte1 < 0x80) {
      //its the brush_fromobjobj case
      //byte1 has everything we need to retrieve everything
      //from the brush_objobjtable
      TRACE((TC_TW, TT_TW_BRUSH,"TW: brush objobj (%u) 1 byte of datastream,", (int) byte1-16));

      byte1 -= 16;      //byte1 now has objobj index

      lpbrushobjobj = &brushobjobj[byte1];

#ifdef WIN16
      dcstate.lastbrush = (INT) byte1;
#endif

      //if the brush is already realized then we may have to change the dc brush rotation
      //and then we will select the brush into the dc

      if (lpbrushobjobj->hbr != NULL) {
         TRACE((TC_TW, TT_TW_BRUSH,"TW: brush already realized\n"));

         //this is the required rotation
         xRotate = 0x07 & (lpbrushobjobj->rotation);
         yRotate = ((UINT) (0x38 & (lpbrushobjobj->rotation))) >> 3;

         //win16 rotation logic needs to take window position into account#@!$%
         //for win16 the real rotation is dcstate.?BrushOrg combined with current window position

#ifdef WIN32
         dcxRotate = dcstate.xBrushOrg;
         dcyRotate = dcstate.yBrushOrg;

#else
         dcstate.screenULH.x = 0;
         dcstate.screenULH.y = 0;
         ClientToScreen(hwnd, (POINT FAR*) &(dcstate.screenULH));
         dcxRotate = (dcstate.xBrushOrg + dcstate.screenULH.x) & 7;
         dcyRotate = (dcstate.yBrushOrg + dcstate.screenULH.y) & 7;
         TRACE((TC_TW,TT_TW_BRUSH,"TW: current screen ULH of client area is x=%u, y=%u\n",
                  (UINT) dcstate.screenULH.x, (UINT) dcstate.screenULH.y));

#endif

         if ( (dcxRotate !=  xRotate) ||
                                       (dcyRotate !=  yRotate) ) {
            TRACE((TC_TW, TT_TW_BRUSH,"TW: changing DC brush origin to x = %u, y = %u\n",
                              xRotate, yRotate));

            dcstate.xBrushOrg = xRotate;
            dcstate.yBrushOrg = yRotate;

#ifdef WIN32
            jRetcode = SetBrushOrgEx(device, xRotate, yRotate, NULL);

            ASSERT(jRetcode,0);

#else
            jRetcode = UnrealizeObject(lpbrushobjobj->hbr);    //so rotate works for WIN16.  benign for WIN32
            ASSERT(jRetcode,0);

            SetBrushOrg(device, (xRotate + (BYTE) dcstate.screenULH.x) & 7,
                          (yRotate + (BYTE) dcstate.screenULH.y) & 7);

#endif

         }

         SelectObject(device,dcstate.hbr = lpbrushobjobj->hbr);
         //now must make mru
         brush_makemru(lpbrushobjobj, byte1);
         return(TRUE);
      }

      //we can only fall through here if we need to realize the brush
      //handle for an objobj that is already setup
      TRACE((TC_TW, TT_TW_BRUSH,"TW: realizing brush \n"));
      TRACE((TC_TW, TT_TW_BRUSH,"TW: total brushes realized not including this one=%u\n",
                        number_nonsolidbrushes_realized));
      ASSERT(number_nonsolidbrushes_realized <= MAXBRUSHREALIZED, 0);

      //note:if count not at max then use updated count-1 as dib_index
      //     else use dib_index returned from brush_deletelru
      //use byte2 to hold the needed dib_index

      if (number_nonsolidbrushes_realized < MAXBRUSHREALIZED) {
         byte2 = number_nonsolidbrushes_realized;
         number_nonsolidbrushes_realized++;   //increment realized count
      }
      else byte2 = brushobjobj[brush_deletelru()].dib_index;
                                          //need to free up an objobj entry
                                          //and get the dib_index

      lpbrushobjobj->dib_index = byte2;

      TRACE((TC_TW, TT_TW_BRUSH,"TW: Using BrushDIB index = %u\n", (int) byte2));

      //jk256 - if its a 256 color bitmap then we need to have a different call
      //        because the cache call is different (_128B vs. _32B
      //       lpbrushobjobj->cache_handle shows that it is 256 color if
      //                   the high order bit is set
      if (lpbrushobjobj->cache_handle & 0x8000) {
         //its a 256 color brush
         ASSERT(lpbrushobjobj->color == 0,0);
         SelectObject(device,dcstate.hbr = lpbrushobjobj->hbr =
              realize_brush(device,
                     lpTWCacheRead(lpbrushobjobj->cache_handle & 0x7fff,
                                          _128B,(LPUINT) &cachecount,0),
                     lpbrushobjobj->rotation, lpbrushobjobj->color,
                     BrushDIB[byte2].dib_handle, BrushDIB[byte2].dib_address, hwnd, 1));
      }
      else SelectObject(device,dcstate.hbr = lpbrushobjobj->hbr =
           realize_brush(device,
                  lp32ByteObject(lpbrushobjobj->cache_handle),
                  lpbrushobjobj->rotation, lpbrushobjobj->color,
                  BrushDIB[byte2].dib_handle, BrushDIB[byte2].dib_address, hwnd, 0));


      ASSERT(dcstate.hbr != NULL, 0);

      //o.k. now we need to put lpbrushobjobj a.k.a. byte1 as mru
      brush_newmru(lpbrushobjobj, byte1);

      return(TRUE);
   }

   //we can only fall through here if its a nonsolid color brush
   //and either there is no caching or we need to cache the
   //objobj and possibly the obj

   //first turn byte1 into the objobj index
   //note that if not caching we want the index to be 114
   //112 and 113 are reserved for forward and backward pointers


   if (byte1 == 0x8f) {
      byte1 = 114;
      TRACE((TC_TW, TT_TW_BRUSH,"TW: create brush NO CACHING\n"));
   }
   else {
      byte1 = (byte1 & 0x7f) - 16;
      TRACE((TC_TW, TT_TW_BRUSH,"TW: create brush objobj (%u)\n", (int) byte1));
   }

   lpbrushobjobj = &brushobjobj[byte1];

#ifdef WIN16
   dcstate.lastbrush = (INT) byte1;
#endif

   GetNextTWCmdBytes(&byte2,1);

   //jk256 - before continuing setup j256Color to TRUE if a 256 color brush
   j256Color = ((byte2 & 0x50) == 0x40);

   ASSERT((!j256Color) || (j256Color && (vColor == Color_Cap_256)),0 );
   ASSERT((!j256Color) || (j256Color && (!(byte2 & 0x10))),0);

   //jk256
   //the following if group is the original 16 color mode logic
   //which is left untouched in 256 color mode for non 256 color brushes
   //the logic assumes no differences between cache reads and writes
   //because it is a _32B cache object

   j256CWrite = FALSE;     //only TRUE if its a 256 color brush
                           //WRITE into the cache.
                           //used as indicator to say must call finishedTWCacheWrite
   if (!j256Color) {
      //if caching then get client object cache handle
      //setup pointer to sink/source the brush data
      if (byte1 != 114) {
         GetNextTWCmdBytes(&bytetemp,1);

         lpbrushobjobj->cache_handle =
                            (((WORD) (byte2 & 0x03)) << 8) | (WORD) bytetemp;
         lpbrushdata = lp32ByteObject(lpbrushobjobj->cache_handle);
      }
      else lpbrushdata = (LPBYTE) lpstatic_buffer; //use to temporarily hold data

      //setup color info and count of data need if receiving data

      if (!(byte2 & 0x10)) {
         lpbrushobjobj->color = 0;
         count = 32;
      }
      else {
         //if we get to here its a 2 color bitmap
         //jk256
         if (vColor == Color_Cap_256) {
            GetNextTWCmdBytes((LPWORD) &(lpbrushobjobj->color),2);
         }
         else {
            lpbrushobjobj->color = 0;     //make sure high order byte is 0
            GetNextTWCmdBytes((LPBYTE) &(lpbrushobjobj->color),1);
         }
         count = 8;                 //only 8 byte of image data for 2 colors
      }

      if (!(byte2 & 0x20)) {
         lpbrushobjobj->rotation = 0;
      }
      else GetNextTWCmdBytes((LPBYTE) &(lpbrushobjobj->rotation),1);

      //get brush image data if need to
      if ( (byte1 == 114) || (byte2 & 0x80) ) {
         TRACE((TC_TW, TT_TW_BRUSH,"TW: getting brush image data, count = %u\n", count));
         GetNextTWCmdBytes(lpbrushdata,count);
      }
   }
   else {
      ASSERT(vColor == Color_Cap_256,0);
      if (byte1 != 114) {

         GetNextTWCmdBytes(&bytetemp,1);

         //its a 12 bit handle and must set the high order bit in objobj to keep
         //track of the fact that it is a 256 color brush
         lpbrushobjobj->cache_handle =
                    (((WORD) (byte2 & 0x0f)) << 8) | (WORD) bytetemp | (WORD) 0x8000;
      }

      lpbrushobjobj->color = 0;

      if (!(byte2 & 0x20)) {
         lpbrushobjobj->rotation = 0;
      }
      else GetNextTWCmdBytes((LPBYTE) &(lpbrushobjobj->rotation),1);

      if ( (byte1 == 114) || (byte2 & 0x80) ) {
         //it is a cache write
         if (byte1 == 114) {
            lpbrushdata = (LPBYTE) lpstatic_buffer; //use to temporarily hold data
         }
         else {
            j256CWrite = TRUE;
            lpbrushdata = lpTWCacheWrite(lpbrushobjobj->cache_handle & 0x7fff,
                                                _128B,0,0);
         }

         TRACE((TC_TW, TT_TW_BRUSH,"TW: getting 256 color brush image data\n"));
         GetNextTWCmdBytes(lpbrushdata,64);
      }
      else lpbrushdata = lpTWCacheRead(lpbrushobjobj->cache_handle & 0x7fff,
                                                _128B,0,0);

   }

   //jk256 - if _128B (j256CWrite) we need to close off the cache after the write

   //get rid of any state in brushobjobj for the brush previously
   //occupying this slot
   //if no caching then we may need to delete the previously realized
   //brush object when no caching.
   //if caching then we may need to delete the previously realized brush
   //object in this slot AND take it off the lru chain

   //NOTE that in the no caching case we use the last slot in BrushDIB
   //specifically for this case

   if ( byte1 == 114) {
      //no caching case

      if (lpbrushobjobj->hbr != NULL) {
         //if previous brush realized then delete
         TRACE((TC_TW, TT_TW_BRUSH,"TW: Deleting previously realized brush index = %u\n", (int) byte1));
         bretcode = DeleteObject(lpbrushobjobj->hbr);
         ASSERT(bretcode, 0);
      }

      //dont count entry 114 as part of the realized brushes
      TRACE((TC_TW, TT_TW_BRUSH,"TW: realizing brush, no caching case \n"));

      //jk256 - need to set the last parm to reflect possibility of
      //             a 256 color bitmap
      SelectObject(device,dcstate.hbr = lpbrushobjobj->hbr =
           realize_brush(device, lpbrushdata,
                         lpbrushobjobj->rotation, lpbrushobjobj->color,
                         BrushDIB[MAXBRUSHREALIZED].dib_handle,
                         BrushDIB[MAXBRUSHREALIZED].dib_address, hwnd,
                         j256Color ? 1 : 0));

      ASSERT(dcstate.hbr != NULL, 0);
      return(TRUE);

   }

   //if we fall through here then caching is enabled so we are possibly
   //  taking over a previously used objobj entry.
   //if a brush was already realized at this entry then we must get rid of it
   //and reuse its dib_index
   //if entry never used then we need to possibly free up a realized brush
   //and the dib_index is different depending on the case
   //use byte2 to hold the dib_index to be used

   ASSERT(number_nonsolidbrushes_realized <= MAXBRUSHREALIZED, 0);

   if (lpbrushobjobj->hbr != NULL) {
      TRACE((TC_TW, TT_TW_BRUSH,"TW: deleting brush at brushobjobj index %u\n", (int) byte1));
      byte2 = lpbrushobjobj->dib_index;
      brush_deletebrush(lpbrushobjobj, byte1);
   }
   //note:if count not at max then use updated count-1 as dib_index
   //     else use dib_index returned from brush_deletelru
   else if(number_nonsolidbrushes_realized < MAXBRUSHREALIZED) {
      byte2 = number_nonsolidbrushes_realized;
      number_nonsolidbrushes_realized++;   //increment realized count

   }
   else byte2 = brushobjobj[brush_deletelru()].dib_index;
                                       //need to free up an objobj entry
                                       //and get the dib_index

   lpbrushobjobj->dib_index = byte2;   //harmless for != NULL case

   TRACE((TC_TW, TT_TW_BRUSH,"TW: realizing brush at objobj index %u and BrushDIB index %u\n",
                     (int) byte1, (int) byte2));

   //jk256 - need to setup the last parm correctly for 256 color bitmap
   SelectObject(device,dcstate.hbr = lpbrushobjobj->hbr =
        realize_brush(device, lpbrushdata,
                  lpbrushobjobj->rotation, lpbrushobjobj->color,
                  BrushDIB[byte2].dib_handle, BrushDIB[byte2].dib_address, hwnd,
                  j256Color ? 1 : 0));

   ASSERT(dcstate.hbr != NULL, 0);

   //jk256
   if (j256CWrite) {
      finishedTWCacheWrite(0);
   }


   //o.k. now we need to put lpbrushobjobj a.k.a. byte1 as mru
   brush_newmru(lpbrushobjobj, byte1);

   return(TRUE);


}


