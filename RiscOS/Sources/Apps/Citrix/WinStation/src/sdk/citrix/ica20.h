
/******************************************************************************
*
*  ICA20.H
*     This file contains definitions for the ICA 2.0 command packets
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen
*
*  $Log$
*  
*     Rev 1.4   21 Apr 1997 16:56:46   TOMA
*  update
*  
*     Rev 1.3   04 Aug 1994 12:38:20   richa
*  Increased Virtual_Maxinum for now.
*
*     Rev 1.2   29 Jul 1994 08:29:14   bradp
*  update
*
*     Rev 1.1   21 Jul 1994 15:53:58   andys
*  thinwire20
*
*     Rev 1.0   12 May 1994 09:01:52   bradp
*  Initial revision.
*
*
*******************************************************************************/

/*=============================================================================
==   Virtual Drivers
=============================================================================*/

/*
 *  Virtual i/o channel ids
 *  note: don't change the order of this structure
 */
typedef enum _VIRTUALCLASS {
    Virtual_Screen     = 0,
    Virtual_Xfer       = 1,
    Virtual_ThinWire   = 2,               // remote windows data
    Virtual_Reserved1  = 3,               // reserved
    Virtual_Reserved2  = 4,               // reserved
    Virtual_Reserved3  = 5,               // reserved
    Virtual_Reserved4  = 6,               // reserved
    Virtual_Reserved5  = 7,               // reserved
    Virtual_Reserved6  = 8,               // reserved
    Virtual_Reserved7  = 9,               // reserved
    Virtual_Reserved8  = 10,              // reserved
    Virtual_Reserved9  = 11,              // reserved
    Virtual_Reserved10 = 12,              // reserved
    Virtual_Reserved11 = 13,              // reserved
    Virtual_Reserved12 = 14,              // reserved
    Virtual_Reserved13 = 15,              // reserved
    Virtual_Maximum    = 16,              // MUST BE LAST
} VIRTUALCLASS;


/*
 *  Default for ica reliable protocol
 */
#define MAXRETRIES_DEFAULT      20
#define ACKTIMEOUT_DEFAULT      20
#define NAKTIMEOUT_DEFAULT      15
#define RCVTIMEOUT_DEFAULT      5
#define WATCHDOGFREQ_DEFAULT    60

/*
 *  Worst case, number of bytes of overhead in a packet
 *   - header 1, type 2, sequence 2, crc 4
 */
#define PACKET_OVERHEAD 9

/*
 *  Special packet characters
 *  NOTE: these numbers can not be used as packet commands
 */
#define PACKET_HEADER           0x98 // packet header character
#define PACKET_RAW_HEADER       0x99 // packet raw header character
#define XPCXON                  0x65 // XPC XON character
#define XPCXOFF                 0x67 // XPC XOFF character

/*
 *  Group 2
 */
#define PACKET_CHAR_HEADER      0x0001 // send PACKET_HEADER character
#define PACKET_CHAR_RAW_HEADER  0x0002 // send PACKET_RAW_HEADER character
#define PACKET_CHAR_XPCXON      0x0003 // send scan code XON (0x65) character
#define PACKET_CHAR_XPCXOFF     0x0004 // send scan code XOFF (0x67) character
#define PACKET_RAW_WRITE0       0x0005 // write 1 bytes of raw data
#define PACKET_RAW_WRITE1       0x0006 // write n bytes of raw data (short)
#define PACKET_RAW_WRITE2       0x0007 // write n bytes of raw data (long)
#define PACKET_NEWLINE          0x0008 // carriage return / line feed
#define PACKET_CLEAR_SCREEN     0x0009 // home cursor and clear screen
#define PACKET_SETCURPOS        0x000a // VioSetCurPos
#define PACKET_WRTCELLSTR1      0x0010 // VioWrtCellStr (short)
#define PACKET_WRTCELLSTR2      0x0011 // VioWrtCellStr (long)
#define PACKET_WRTCHARSTR1      0x0012 // VioWrtCharStr (short)
#define PACKET_WRTCHARSTR2      0x0013 // VioWrtCharStr (long)
#define PACKET_WRTCHARSTRATTR1  0x0014 // VioWrtCharStrAttr (short)
#define PACKET_WRTCHARSTRATTR2  0x0015 // VioWrtCharStrAttr (long)
#define PACKET_WRTNATTR1        0x0016 // VioWrtNAttr (short)
#define PACKET_WRTNATTR2        0x0017 // VioWrtNAttr (long)
#define PACKET_WRTNCELL1        0x0018 // VioWrtNCell (short)
#define PACKET_WRTNCELL2        0x0019 // VioWrtNCell (long)
#define PACKET_WRTNCHAR1        0x001a // VioWrtNChar (short)
#define PACKET_WRTNCHAR2        0x001b // VioWrtNChar (long)
#define PACKET_SCROLLUP         0x001c // VioScrollUp
#define PACKET_SCROLLDN         0x001d // VioScrollDn
#define PACKET_SCROLLLF         0x001e // VioScrollLf
#define PACKET_SCROLLRT         0x001f // VioScrollRt
#define PACKET_SCROLLUP1        0x0020 // VioScrollUp - 1 line using previous
#define PACKET_SCROLLDN1        0x0021 // VioScrollDn - 1 line using previous
#define PACKET_SCROLLLF1        0x0022 // VioScrollLf - 1 line using previous
#define PACKET_SCROLLRT1        0x0023 // VioScrollRt - 1 line using previous
#define PACKET_SCROLLUP2        0x0024 // VioScrollUp - n lines using previous
#define PACKET_SCROLLDN2        0x0025 // VioScrollDn - n lines using previous
#define PACKET_SCROLLLF2        0x0026 // VioScrollLf - n lines using previous
#define PACKET_SCROLLRT2        0x0027 // VioScrollRt - n lines using previous
// HACK Alert: PACKET_ENABLE_KEYBOARD and PACKET_DISABLE_RELIABLE were switched
#define PACKET_ENABLE_KEYBOARD  0x802B // unlock keyboard
#define PACKET_DISABLE_KEYBOARD 0x0031 // lock keyboard
#define PACKET_SET_GLOBAL_ATTR  0x0032 // set global attribute
#define PACKET_QUERY_CAPABILITY 0x8001 // query communication capabilities.
//       ICARevision;                  // ICA packet revision level
#define    ICA_REVISION_LEVEL   0x03   // ... current revision level
//       ICACapability;                // ICA capability flags
#define    ICA_COMPRESS_WIN     0x0001 // ... windows data is compressed
#define    ICA_COMPRESS_TEXT    0x0002 // ... text ica data is compressed
#define    ICA_RELIABLE         0x0004 // ... error recovery is supported
//       ICACommandl;                  // ICA commands available
#define    ICA_G2               0x00000001L // ... group 2 commands
#define    ICA_G3               0x00000002L // ... group 3 commands
#define    ICA_G4               0x00000004L // ... group 4 commands
#define    ICA_G5               0x00000008L // SerView Drive Mapping commands
#define    ICA_G6               0x00000010L // Local Print Buffering commands
#define PACKET_RESET_DEFAULTS   0x8002 // reset to default settings
#define PACKET_SET_COLOR_MODE   0x8010 // set color/monochrome mode
#define PACKET_SET_ROWS         0x8011 // set number of text rows
#define PACKET_SET_COLUMNS      0x8012 // set number of text columns
#define PACKET_SET_COLOR_REG    0x8013 // set color register
#define PACKET_SET_PALETTE_REG  0x8014 // set palette registers
#define PACKET_SET_OVERSCAN     0x8015 // set the overscan (border) color
#define PACKET_SET_INTENSITY    0x8016 // set the blink/background intensity
#define PACKET_SET_UNDERLINE    0x8017 // set scan line for underline
#define PACKET_SETCURTYPE       0x8018 // VioSetCurType
#define PACKET_BEEP             0x8019 // DosBeep
#define PACKET_SETCP            0x801a // VioSetCP
#define PACKET_SET_LED          0x801b // set keyboard LEDs
                                       // available
#define PACKET_QUERY_LED        0x801d // query keyboard LED status
#define PACKET_QUERY_DEFAULTS   0x801e // query default values
#define PACKET_INIT_UPLOAD      0x801f // initiate file upload
#define PACKET_INIT_DOWNLOAD    0x8020 // initiate file download
#define PACKET_ENABLE_AUX       0x8021 // enable AUX port
#define PACKET_DISABLE_AUX      0x8022 // disable AUX port
#define PACKET_QUERY_SERIAL     0x8023 // query serial number
#define    SERIAL_MULTIPLE        0x01 // ... allow multi connects with this SN
#define    SERIAL_LANLINK         0x02 // ... this is LANLINK
#define    SERIAL_RLINK           0x04 // ... this is Remote Link
#define    SERIAL_ADMIN           0x08 // ... admin user's only
#define    SERIAL_WINCRED         0x10 // ... wincredible capable
#define    SERIAL_HANGUP          0x20 // ... hangup on logout flag

/*
 *  Group 3
 */
#define PACKET_TERMINATE        0x8024 // terminate connection
#define    TERMINATE_NOHANGUP     0x01 // ... don't hangup connection
#define    TERMINATE_EXIT         0x02 // ... exit program
#define    TERMINATE_EXITCODE     0x04 // ... 2 byte exit code is available
#define    TERMINATE_RETRY        0x08 // ... excessive error retries
#define PACKET_QUERY_INITPGM    0x8025 // query initial program

/*
 *  Group 4
 */
#define PACKET_WINDOWS_DISPLAY1 0x0033 // windows display data (short)
#define PACKET_WINDOWS_DISPLAY2 0x0034 // windows display data (long)
#define PACKET_WINDOWS_SYSTEM   0x0035 // windows system command
#define PACKET_XOFF             0x0036 // stop transmiter channel
#define PACKET_XON              0x0037 // start transmiter channel
#define PACKET_ACK              0x0038 // ack packet
#define PACKET_KEYBOARD         0x0039 // keyboard packet (2-n scan codes)
#define PACKET_DATA_WRITE1      0x003A // write data packet (short)
#define PACKET_DATA_WRITE2      0x003B // write data packet (long)
#define PACKET_WRITE_AUX1       0x003C // write data to AUX port (short)
#define PACKET_DATA_INPUT       0x003D // data input packet
#define PACKET_FRAGMENT         0x003E // packet fragment - more to come
#define PACKET_MOUSE_INPUT      0x003F // mouse data command
#define PACKET_SYSTEM_INPUT     0x0040 // windows system command
#define PACKET_NAK              0x0041 // nak packet
#define    NAK_NAKTIMEOUT         0x01 // ... no response from nak
#define    NAK_READTIMEOUT        0x02 // ... partial packet timeout
#define    NAK_BADSEQUENCE        0x04 // ... bad packet sequence
#define    NAK_SHORTPACKET        0x08 // ... premature packet header
#define    NAK_BADCRC             0x10 // ... bad packet crc
#define PACKET_INIT_WINDOWS     0x8026 // initiate windows protocol
#define PACKET_SUSPEND_WINDOWS  0x8027 // suspend windows protocol
#define PACKET_END_WINDOWS      0x8028 // end windows protocol
#define PACKET_CALLBACK         0x8029 // callback (enter auto-answer)
#define PACKET_WRITE_AUX2       0x802A // write data to AUX port (long)
// HACK Alert: PACKET_ENABLE_KEYBOARD and PACKET_DISABLE_RELIABLE were switched
#define PACKET_DISABLE_RELIABLE 0x0030 // turn off reliable protocol
#define PACKET_COMPRESS_FLUSH   0x802C // flush compression string table
#define PACKET_ENABLE_RELIABLE  0x802D // enable error correction
#define PACKET_KILLFOCUS        0x802E // kill input focus of current session
#define PACKET_SETFOCUS         0x802F // redisplay current session data
#define PACKET_NAK_REPLY        0x8030 // reply to nak of unsent packet sequence

/*
 *  Data packet commands
 *  - first byte of ica commands DATA_WRITE1, DATA_WRITE2, and DATA_INPUT
 */
#define DATA_XFER_START         1   // start file transfer(s)
#define DATA_XFER_END           2   // end file transfer(s)
#define DATA_XFER_SEND_DIR      3   // send information on files in directory
#define DATA_XFER_SEND_FILES    4   // send one or more files
#define DATA_XFER_SEND_CANCEL   5   // cancel send file / dir
#define DATA_XFER_FILE_OPEN     6   // open file
#define DATA_XFER_FILE_WRITE    7   // write file data
#define DATA_XFER_FILE_CLOSE    8   // close file
#define DATA_XFER_FILE_CANCEL   9   // cancel file (skip file)
#define DATA_CALLBACK           10  // hangup modem and enter autoanswer mode
#define DATA_USERHOOK           11  // user software interrupt hook
#define DATA_XFER_SPACE_AVAIL   12  // get disk space available
#define DATA_XFER_QFILEMODE     13  // get file attribute
#define DATA_XFER_CURDIR        14  // get/set current drive/dir, get drivemap
#define DATA_XFER_SEND_FILES2   15  // send one or more files with browse

/*
 *  Group 6
 */
#define PACKET_PRINTER_STATUS    0x8031 // return printer status to host
#define    PRINTER_BUFFER_EMPTY  0x0001 // ... printer is ready
#define    PRINTER_OFF_LINE      0x0002 // ... printer is off line
#define    PRINTER_OUT_OF_PAPER  0x0004 // ... printer is out of paper
#define    PRINTER_IO_ERROR      0x0008 // ... printer i/o error
#define    PRINTER_ROUTE_REMOVED 0x0010 // ... printer route removed

