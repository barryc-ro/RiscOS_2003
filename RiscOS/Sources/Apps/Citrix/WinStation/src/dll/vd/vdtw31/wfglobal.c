
/*******************************************************************************
*
*   WFGLOBAL.C
*
*   Thin Wire Windows - Global Data
*
*   Copyright (c) Citrix Systems Inc. 1995-1996
*
*   Author: Jeff Krantz (jeffk)
*
*   $Log$
*  
*     Rev 1.13   15 Apr 1997 18:16:58   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.12   08 May 1996 14:53:18   jeffm
*  update
*  
*     Rev 1.11   03 Jan 1996 13:34:04   kurtp
*  update
*  
*******************************************************************************/

#include "wfthin.h"

//this is the table used to get a colorref for the protocol color index
//we will fill this array dynamically when we start dealing with
//true palette indexes for hardware layers that support palettes
//this enhancement will be done for performance reasons

//DIB_ColorMode needs to be set to either DIB_RGB_COLORS or
//DIB_PAL_COLORS
//if THINPAL is defined then it should be set to DIB_PAL_COLORS

UINT  DIB_ColorMode;

#ifdef THINPAL
COLORREF  twsolidcolor[16] = {
   PALETTEINDEX( 0 ),
   PALETTEINDEX( 1 ),
   PALETTEINDEX( 2 ),
   PALETTEINDEX( 3 ),
   PALETTEINDEX( 4 ),
   PALETTEINDEX( 5 ),
   PALETTEINDEX( 6 ),
   PALETTEINDEX( 12 ),
   PALETTEINDEX( 7 ),
   PALETTEINDEX( 13 ),
   PALETTEINDEX( 14 ),
   PALETTEINDEX( 15 ),
   PALETTEINDEX( 16 ),
   PALETTEINDEX( 17 ),
   PALETTEINDEX( 18 ),
   PALETTEINDEX( 19 )
};
#else
STATIC COLORREF  twsolidcolor[16] = {
   PALETTERGB(0,     0,    0),
   PALETTERGB(0X80,  0,    0),
   PALETTERGB(0,     0X80, 0),
   PALETTERGB(0X80,  0X80, 0),
   PALETTERGB(0,     0,    0X80),
   PALETTERGB(0X80,  0,    0X80),
   PALETTERGB(0,     0X80, 0X80),
   PALETTERGB(0X80,  0X80, 0X80),
   PALETTERGB(0XC0,  0XC0, 0XC0),
   PALETTERGB(0XFF,  0,    0),
   PALETTERGB(0,     0XFF, 0),
   PALETTERGB(0XFF,  0XFF, 0),
   PALETTERGB(0,     0,    0XFF),
   PALETTERGB(0XFF,  0,    0XFF),
   PALETTERGB(0,     0XFF, 0XFF),
   PALETTERGB(0XFF,  0XFF, 0XFF)
};
#endif

BYTE  convert_default16to256[16] = {
   0, 1, 2, 3, 4, 5, 6, 0xF8, 7, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};


//This is the BITMAPINFO structure that is used to display a 16 color dib
//Some of the fields in BITMAPINFOHEADER change based on the operation
//The thing we must track is the RGBQUAD being setup correctly.
//Initially we will use absolute RGB values.
//Then later for optimization reasons we will do the palette thing
//and set it up differently depending on the color capabilities of the client
//when setup for a palette, the RGBQUAD entries become
//16 bit unsigned integers

BITMAPINFO_4BPP_RGBQUAD bitmapinfo_4BPP_RGBQUAD = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
    4,                           //fixed   biBitCount - 4 for 16 colors
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {
      {0,   0,    0,    0  },
      {0,   0,    0x80, 0  },
      {0,   0x80, 0,    0  },
      {0,   0x80, 0x80, 0  },
      {0x80, 0,   0,    0  },
      {0x80, 0,   0x80, 0  },
      {0x80, 0x80, 0,   0  },
      {0x80, 0x80, 0x80, 0 },
      {0xC0, 0xC0, 0xC0, 0 },
      {0,   0,    0xFF, 0  },
      {0,   0xFF, 0,    0  },
      {0,   0xFF, 0xFF, 0  },
      {0xFF, 0,   0,    0  },
      {0xFF, 0,   0xFF, 0  },
      {0xFF, 0xFF, 0,   0 },
      {0xFF, 0xFF, 0xFF, 0 }
   }
};


//same as bitmapinfo_4BPP_RGBQUAD but the colors are palette indices
//instead of RGBQUAD

BITMAPINFO_4BPP_PALETTE bitmapinfo_4BPP_PALETTE = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
    4,                           //fixed   biBitCount - 4 for 16 colors
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {0, 1, 2, 3, 4, 5, 6, 12, 7, 13, 14, 15, 16, 17, 18, 19 }
};

//jk256
BITMAPINFO_4BPP_PALETTE bitmapinfo_4BPP_PALETTE_default256 = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
    4,                           //fixed   biBitCount - 4 for 16 colors
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {0, 1, 2, 3, 4, 5, 6, 0xF8, 7, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF }
};

//jk256 - used to hold the last non default 16 color palette
BITMAPINFO_4BPP_PALETTE bitmapinfo_4BPP_PALETTE_last256 = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
    4,                           //fixed   biBitCount - 4 for 16 colors
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {0, 1, 2, 3, 4, 5, 6, 0xF8, 7, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF }
};

//same as bitmapinfo_4BPP_PALETTE but for 256 color bitmaps
//used when the palette is trivial

BITMAPINFO_8BPP_PALETTE bitmapinfo_8BPP_PALETTE = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
    8,                           //fixed   biBitCount - 4 for 256 colors
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
   21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
   61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
   81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
   101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
   111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
   121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
   131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
   141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
   151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
   161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
   171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
   181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
   191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
   201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
   211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
   221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
   231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
   241, 242, 243, 244, 245, 246, 247, 248, 249, 250,
   251, 252, 253, 254, 255 }
};


//same as bitmapinfo_8BPP_PALETTE but for nontrivial palettes
//palette area used to stage the incoming palette because
//cannot cache the palette until after the operation is complete
//in case there is a conflict between the bitmap cache area
//and the palette cache area

BITMAPINFO_8BPP_PALETTE bitmapinfo_8BPP_PALETTE_MAP = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
    8,                           //fixed   biBitCount - 4 for 256 colors
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
   21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
   61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
   81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
   101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
   111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
   121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
   131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
   141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
   151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
   161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
   171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
   181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
   191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
   201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
   211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
   221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
   231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
   241, 242, 243, 244, 245, 246, 247, 248, 249, 250,
   251, 252, 253, 254, 255 }
};

//we won't be needed these 1BPP DIB because we need to use
//ddb for 2 color bitmaps so the colors come out correctly!!!!!!!!
#if 0
//This is the BITMAPINFO structure that is used to display a 2 color dib
//Some of the fields in BITMAPINFOHEADER change based on the operation
//The thing we must track is the RGBQUAD being setup correctly.
//Initially we will use absolute RGB values.
//Then later for optimization reasons we will do the palette thing
//and set it up differently depending on the color capabilities of the client
//when setup for a palette, the RGBQUAD entries become
//16 bit unsigned integers
//
//NOTE: for 2 color bitmaps, we are assuming that the DC foreground and
//background color do not matter and we will dynamically fill in the bmiColors
//entries before displaying a 2 color bitmap
//
//jktest - test 2 color bitmap color assumptions

BITMAPINFO_1BPP_RGBQUAD bitmapinfo_1BPP_RGBQUAD = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
    1,                           //fixed   biBitCount - 1 for 16 colors
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {
      {0 , 0, 0, 0},          //initial values don't matter
      {0 , 0, 0, 0}
   }
};


//same as bitmapinfo_4BPP_RGBQUAD but the colors are palette indices
//instead of RGBQUAD
BITMAPINFO_1BPP_PALETTE bitmapinfo_1BPP_PALETTE = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
    1,                           //fixed   biBitCount - 1 for 16 colors
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {0,   0}        //initial values don't matter
};
#endif


//this is used for the GetDIBits of savescreenbitmap
//we need to have a BITMAPINFO for the DIB that we are getting
//the api description says that the color table is filled in so we keep a separate
//area so there is no pollution with the bitmapinfo variables.
//Also, if go to palettes, we couldn't use the same area

BITMAPINFO_4BPP_RGBQUAD bitmapinfo_SSB = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
    4,                           //fixed   biBitCount - 4 for 16 colors
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {
      {0,   0,    0,    0  },
      {0,   0,    0x80, 0  },
      {0,   0x80, 0,    0  },
      {0,   0x80, 0x80, 0  },
      {0x80, 0,   0,    0  },
      {0x80, 0,   0x80, 0  },
      {0x80, 0x80, 0,   0 },
      {0x80, 0x80, 0x80, 0 },
      {0xC0, 0xC0, 0xC0, 0 },
      {0,   0,    0xFF, 0  },
      {0,   0xFF, 0,    0  },
      {0,   0xFF, 0xFF, 0  },
      {0xFF, 0,   0,    0  },
      {0xFF, 0,   0xFF, 0  },
      {0xFF, 0xFF, 0,   0 },
      {0xFF, 0xFF, 0xFF, 0 }
   }
};


//we need the stupid_rop_info to create the rop3 parameter from
//the 1 byte of protocol
WORD  stupid_rop_info[256] =
{
         0x0042,0x0289,0x0c89,0x00aa,0x0c88,0x00a9,0x0865,0x02c5,
         0x0f08,0x0245,0x0329,0x0b2a,0x0324,0x0b25,0x08a5,0x0001,
         0x0c85,0x00a6,0x0868,0x02c8,0x0869,0x02c9,0x5cca,0x1d54,
         0x0d59,0x1cc8,0x06c5,0x0768,0x06ca,0x0766,0x01a5,0x0385,
         0x0f09,0x0248,0x0326,0x0b24,0x0d55,0x1cc5,0x06c8,0x1868,
         0x0369,0x16ca,0x0cc9,0x1d58,0x0784,0x060a,0x064a,0x0e2a,
         0x032a,0x0b28,0x0688,0x0008,0x06c4,0x1864,0x01a8,0x0388,
         0x078a,0x0604,0x0644,0x0e24,0x004a,0x18a4,0x1b24,0x00ea,

         0x0f0a,0x0249,0x0d5d,0x1cc4,0x0328,0x0b29,0x06c6,0x076a,
         0x0368,0x16c5,0x0789,0x0605,0x0cc8,0x1954,0x0645,0x0e25,
         0x0325,0x0b26,0x06c9,0x0764,0x08a9,0x0009,0x01a9,0x0389,
         0x0785,0x0609,0x0049,0x18a9,0x0649,0x0e29,0x1b29,0x00e9,
         0x0365,0x16c6,0x0786,0x0608,0x0788,0x0606,0x0046,0x18a8,
         0x58a6,0x0145,0x01e9,0x178a,0x01e8,0x1785,0x1e28,0x0c65,
         0x0cc5,0x1d5c,0x0648,0x0e28,0x0646,0x0e26,0x1b28,0x00e6,
         0x01e5,0x1786,0x1e29,0x0c68,0x1e24,0x0c69,0x0955,0x03c9,

         0x02e9,0x0975,0x0c49,0x1e04,0x0c48,0x1e05,0x17a6,0x01c5,
         0x00c6,0x1b08,0x0e06,0x0666,0x0e08,0x0668,0x1d7c,0x0ce5,
         0x0c45,0x1e08,0x17a9,0x01c4,0x17aa,0x01c9,0x0169,0x588a,
         0x1888,0x0066,0x0709,0x07a8,0x0704,0x07a6,0x16e6,0x0345,
         0x00c9,0x1b05,0x0e09,0x0669,0x1885,0x0065,0x0706,0x07a5,
         0x03a9,0x0189,0x0029,0x0889,0x0744,0x06e9,0x0b06,0x0229,
         0x0e05,0x0665,0x1974,0x0ce8,0x070a,0x07a9,0x16e9,0x0348,
         0x074a,0x06e6,0x0b09,0x0226,0x1ce4,0x0d7d,0x0269,0x08c9,

         0x00ca,0x1b04,0x1884,0x006a,0x0e04,0x0664,0x0708,0x07aa,
         0x03a8,0x0184,0x0749,0x06e4,0x0020,0x0888,0x0b08,0x0224,
         0x0e0a,0x066a,0x0705,0x07a4,0x1d78,0x0ce9,0x16ea,0x0349,
         0x0745,0x06e8,0x1ce9,0x0d75,0x0b04,0x0228,0x0268,0x08c8,
         0x03a5,0x0185,0x0746,0x06ea,0x0748,0x06e5,0x1ce8,0x0d79,
         0x1d74,0x5ce6,0x02e9,0x0849,0x02e8,0x0848,0x0086,0x0a08,
         0x0021,0x0885,0x0b05,0x022a,0x0b0a,0x0225,0x0265,0x08c5,
         0x02e5,0x0845,0x0089,0x0a09,0x008a,0x0a0a,0x02a9,0x0062
};


//we need rop3 to rop2 because the protocol has rop3 code and some
//windows functions wants a rop2 code that we need to select into
//the dc

BYTE Rop3ToRop2[256] =
{
       R2_BLACK, 0, 0, 0, 0, R2_NOTMERGEPEN, 0, 0,
       0, 0, R2_MASKNOTPEN, 0, 0, 0, 0, R2_NOTCOPYPEN,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       R2_MASKPENNOT, 0, 0, 0, 0, R2_NOT, 0, 0,
       0, 0, R2_XORPEN, 0, 0, 0, 0, R2_NOTMASKPEN,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       R2_MASKPEN, 0, 0, 0, 0, R2_NOTXORPEN, 0, 0,
       0, 0, R2_NOP, 0, 0, 0, 0, R2_MERGENOTPEN,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       R2_COPYPEN, 0, 0, 0, 0, R2_MERGEPENNOT, 0, 0,
       0, 0, R2_MERGEPEN, 0, 0, 0, 0, R2_WHITE
};


//initialized at InitOnce with handles representing the 16 solid color
//brushes in 16 color mode
HBRUSH   hbrsolid[16];

//jk256
//initialized at Init256Color with handles representing the first 10 and last
//10 solid color brushes (the 20 system colors).
//the 21st entry is not null if the currently realized brush is a solid color
//that is not the first 10 or last 10.
//the rule for deletion is as follows: make sure the default brush is
//selected into the DC and then if the handle is not null, then delete
//during dynamic operation, if the 21st entry is not null, then need to delete
//before create the new one
//
//lasthbrsolid256index has the index of the last realized solid color brush
//for 256 color mode whose handle is in hbrsolid256[20]

HBRUSH   hbrsolid256[21];
ULONG    lasthbrsolid256index;


//initialized at InitOnce with handles representing the 16 solid color
//pens
HBRUSH   hpensolid[16];

//initialized at Init256Color with handles representing the 21 solid color
//pens
HBRUSH   hpensolid256[21];

LPBYTE  lptiny_cache;

//has the current state of the device context
DCSTATE dcstate;

//has the brushobjobj table
BRUSHOBJOBJ brushobjobj[115];

//has the handle and pointer information for the dibindex entry in
//brushobjobj
BRUSHDIB BrushDIB[MAXBRUSHREALIZED + 1];

//used to know when need to delete a realized brush before
//should realize a new brush
INT number_nonsolidbrushes_realized;

//buffers used by many thinwire guys
//buffer may not contain history accross a thinwire packet boundary!!!!
//gives a doubleword aligned buffer which is 6K bytes long
//for bitblt we use this buffer many different ways:
//1)If rle, we move the rle data here.  Use 2048+4 size
//2)If not caching, we need to be able to be able to expand the rle data
// into a buffer.  If a 16 color bitmap then that buffer needs to be
// 2048+4.  Then if data does not have each scanline of modula 4 size then
// we need another 2048 buffer to put the bitmap data in a format where
// each scanline is modula 4 for dib purposes!!!  So for a 16 color bitmap
// we need 4096+8 buffer.
// If a 2 color bitmap and not caching then we have the same problem as
// the 16 color bitmap but the worst case is that the 2 color data has only
// 1 byte per scanline and the bitmap can be 1024 pixels high.  However
// to blt we need 4 bytes per scanline or 4096 bytes.  So we need 2K to
// hold the rle expanded data and 4K to hold the dib for blitting!!!
//3)If caching, we still need up to 4K for the 2 color bitmap because
// we probably wont be able to blt out of the cache area because of scan
// line length quanta requirements.
//THIS BUFFER MUST START ON A 4 BYTE BOUNDARY!!!!
LPDWORD lpstatic_buffer;



//we cannot create compatDC during initialization because the DC does not exist.
//so the rule is that any code that wants to use it must check to see if it is NULL.
//if it is null then it should be created with CreateCompatibleDC with the device context
//of the client window/
//the state of this DC must always be left in the default state when exiting the
//packet cracking routine
HDC   compatDC = NULL;

//global color mode
COLOR_CAPS  vColor = Color_Cap_16;
