/******************************************************************************
*
*  ICA30.H
*     This file contains definitions for the ICA 3.0 command packets
*
*     NOTE: this file should only be given to people with an ICA license
*
*
*  Copyright Citrix Systems Inc. 1997
*
*  Author: Brad Pedersen
*
*  $Log$
*  
*     Rev 1.74   Jul 01 1997 19:36:34   billm
*  Change PACKET_INIT_REQUEST version
*  
*     Rev 1.73   20 Jun 1997 20:30:06   kurtp
*  update
*  
*     Rev 1.72   12 Jun 1997 18:23:54   kurtp
*  update
*  
*     Rev 1.71   28 May 1997 10:31:56   terryt
*  client double click support
*  
*     Rev 1.71   27 May 1997 14:29:26   terryt
*  client double click support
*  
*     Rev 1.70   21 Apr 1997 16:56:48   TOMA
*  update
*  
*     Rev 1.69   Mar 25 1997 23:47:40   billm
*  Added extern C for C++
*  
*     Rev 1.68   21 Mar 1997 16:10:34   bradp
*  update
*  
*
*******************************************************************************/

#ifndef __ICA30_H__
#define __ICA30_H__

#ifdef __cplusplus
extern "C" {
#endif

//#pragma pack(1)


/*=============================================================================
==   ICA 3.0 - detect string
=============================================================================*/

#define ICA_DETECT_STRING "\x7f\x7fICA\x00"     // 0x7f 0x7f ICA 0x00



/*=============================================================================
==   ICA 3.0 - commands
=============================================================================*/

/*
 *  NOTE: when adding a new packet type the ICAData array must be updated
 *        in both the host and the client ica winstation driver.
 */

//          define              type  length    description
//-------------------------------------------------------------------------
#define PACKET_INIT_REQUEST     0x00 // nn host->client init packet
#define PACKET_INIT_RESPONSE    0x01 // nn client->host init packet
#define PACKET_INIT_CONNECT     0x02 // nn host->client connect packet
#define PACKET_CALLBACK         0x03 // n  callback (enter auto-answer)
#define PACKET_INIT_CONNECT_RESPONSE 0x04 // nn client->host connect packet
#define PACKET_TERMINATE        0x05 // n  terminate request
#define PACKET_REDRAW           0x06 // nn redraw rectangles (WD VERSION 2)
#define PACKET_STOP_REQUEST     0x07 // 0  c->h stop sending screen data
#define PACKET_STOP_OK          0x08 // 0  h->c stop accepted
#define PACKET_REDISPLAY        0x09 // 0  c->h start sending data and redisplay
#define PACKET_KEYBOARD0        0x0A // 1  keyboard data (1 scan code)
#define PACKET_KEYBOARD1        0x0B // n  keyboard data (short)
#define PACKET_KEYBOARD2        0x0C // nn keyboard data (long)
#define PACKET_MOUSE0           0x0D // 5  mouse data (1 mouse structure)
#define PACKET_MOUSE1           0x0E // n  mouse data (short)
#define PACKET_MOUSE2           0x0F // nn mouse data (long)
#define PACKET_CLEAR_SCREEN     0x10 // 1  clear screen
        // XON                  0x11 //    reserved - xon character
#define PACKET_CLEAR_EOL        0x12 // 3  clear to end of line
        // XOFF                 0x13 //    reserved - xoff character
#define PACKET_RAW_WRITE0       0x14 // 1  write 1 bytes of raw data
#define PACKET_RAW_WRITE1       0x15 // n  write n bytes of raw data (short)
#define PACKET_RAW_WRITE2       0x16 // nn write n bytes of raw data (long)
#define PACKET_WRTCHARSTRATTR1  0x17 // n  VioWrtCharStrAttr (short)
#define PACKET_WRTCHARSTRATTR2  0x18 // nn VioWrtCharStrAttr (long)
#define PACKET_WRTNCELL1        0x19 // 5  VioWrtNCell (short)
#define PACKET_WRTNCELL2        0x1A // 6  VioWrtNCell (long)
#define PACKET_BEEP             0x1B // 4  DosBeep
#define PACKET_SETMOU_POSITION  0x1C // 4  set mouse position (x,y)
#define PACKET_SETMOU_ATTR      0x1D // 1  set mouse attributes
#define PACKET_SETCUR_POSITION  0x1E // 2  set cursor position (row,column)
#define PACKET_SETCUR_ROW       0x1F // 1  set cursor row
#define PACKET_SETCUR_COLUMN    0x20 // 1  set cursor column
#define PACKET_SETCUR_SIZE      0x21 // 1  set cursor size
#define PACKET_SCROLL_SCREEN    0x22 // 1  scroll screen up 1 line
#define PACKET_SCROLLUP         0x23 // 7  VioScrollUp
#define PACKET_SCROLLDN         0x24 // 7  VioScrollDn
#define PACKET_SCROLLLF         0x25 // 7  VioScrollLf
#define PACKET_SCROLLRT         0x26 // 7  VioScrollRt
#define PACKET_SCROLLUP1        0x27 // 0  VioScrollUp - 1 line using previous
#define PACKET_SCROLLDN1        0x28 // 0  VioScrollDn - 1 line using previous
#define PACKET_SCROLLLF1        0x29 // 0  VioScrollLf - 1 line using previous
#define PACKET_SCROLLRT1        0x2A // 0  VioScrollRt - 1 line using previous
#define PACKET_SCROLLUP2        0x2B // 1  VioScrollUp - n lines using previous
#define PACKET_SCROLLDN2        0x2C // 1  VioScrollDn - n lines using previous
#define PACKET_SCROLLLF2        0x2D // 1  VioScrollLf - n lines using previous
#define PACKET_SCROLLRT2        0x2E // 1  VioScrollRt - n lines using previous
#define PACKET_VIRTUAL_WRITE0   0x2F // 2  write 2 byte of virtual data
#define PACKET_VIRTUAL_WRITE1   0x30 // n  write n bytes of virtual data (short)
#define PACKET_VIRTUAL_WRITE2   0x31 // nn write n bytes of virtual data (long)
#define PACKET_VIRTUAL_ACK      0x32 // 3  ack virtual channel (slide window)
#define PACKET_SET_GRAPHICS     0x33 // n  set graphics mode
#define PACKET_SET_TEXT         0x34 // n  set text mode
#define PACKET_SET_GLOBAL_ATTR  0x35 // 1  set attribute for raw packets
#define PACKET_SET_VIDEO_MODE   0x36 // 1  set video mode
#define PACKET_SET_LED          0x37 // 1  set keyboard LEDs
#define PACKET_VIRTUAL_FLUSH    0x38 // 2  flush specified virtual channel
#define PACKET_SOFT_KEYBOARD    0x39 // 1  raise or lower soft keyboard
#define PACKET_COMMAND_CACHE    0x3A // nn write n bytes of caching data
#define PACKET_SET_CLIENT_DATA  0x3B // set n bytes of a data type 
#define PACKET_MAXIMUM          0x3C // *** last packet type ***



/*=============================================================================
==   PACKET_TERMINATE
=============================================================================*/

/*
 *  PACKET_TERMINATE data defines
 */
#define TERMINATE_DISCONNECT    0
#define TERMINATE_LOGOFF        1
#define TERMINATE_ACK           2


/*=============================================================================
==   PACKET_INIT_REQUEST
=============================================================================*/

/*
 *  PACKET_INIT_REQUEST version level
 *
 *   0 - SouthBeach 
 *   1 - WinFrame 1.5
 *   2 - WinFrame 1.6
 *   3 - WinFrame 1.7
 *   4 - WinFrame 2.0
 */
#define INIT_REQUEST_VERSION 4


/*=============================================================================
==   PACKET_MOUSE0
==   PACKET_MOUSE1
==   PACKET_MOUSE2
=============================================================================*/

/*
 *  Mouse data structure
 */
typedef struct _MOUSEDATA {
    USHORT X;
    USHORT Y;
    BYTE cMouState;
} MOUSEDATA, * PMOUSEDATA;

// mouse status returned in MOUSEDATA structure
#define MOU_STATUS_MOVED   0x01
#define MOU_STATUS_B1DOWN  0x02
#define MOU_STATUS_B1UP    0x04
#define MOU_STATUS_B2DOWN  0x08
#define MOU_STATUS_B2UP    0x10
#define MOU_STATUS_B3DOWN  0x20
#define MOU_STATUS_B3UP    0x40
#define MOU_STATUS_DBLCLK  0x80
#define MOU_STATUS_ALL     0xFF

/*=============================================================================
==   PACKET_SOFT_KEYBOARD
=============================================================================*/

#define SOFTKEY_RAISE      0x01

//#pragma pack()

#ifdef __cplusplus
}
#endif

#endif //__ICA30_H__
