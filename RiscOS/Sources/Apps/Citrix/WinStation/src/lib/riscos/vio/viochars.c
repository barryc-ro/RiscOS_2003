/* > chars.c

 * Character definition for DOS font

 */

#include "windows.h"

#include <string.h>

#include "swis.h"

#include "../../../inc/client.h"
#include "../../../inc/debug.h"

#include "vio.h"

// CHAR 0H

static const char font_definition[] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x7E,0x81,0xA5,0x81,0xBD,0x99,0x81,0x7E,
        0x7E,0xFF,0xDB,0xFF,0xC3,0xE7,0xFF,0x7E,
        0x6C,0xFE,0xFE,0xFE,0x7C,0x38,0x10,0x00,
        0x10,0x38,0x7C,0xFE,0x7C,0x38,0x10,0x00,
        0x38,0x7C,0x38,0xFE,0xFE,0x7C,0x38,0x7C,
        0x10,0x10,0x38,0x7C,0xFE,0x7C,0x38,0x7C,
        0x00,0x00,0x18,0x3C,0x3C,0x18,0x00,0x00,
// CHAR 8H
        0xFF,0xFF,0xE7,0xC3,0xC3,0xE7,0xFF,0xFF,
        0x00,0x3C,0x66,0x42,0x42,0x66,0x3C,0x00,
        0xFF,0xC3,0x99,0xBD,0xBD,0x99,0xC3,0xFF,
        0x0F,0x07,0x0F,0x7D,0xCC,0xCC,0xCC,0x78,
        0x3C,0x66,0x66,0x66,0x3C,0x18,0x7E,0x18,
        0x3F,0x33,0x3F,0x30,0x30,0x70,0xF0,0xE0,
        0x7F,0x63,0x7F,0x63,0x63,0x67,0xE6,0xC0,
        0x99,0x5A,0x3C,0xE7,0xE7,0x3C,0x5A,0x99,
// CHAR 10H
        0x80,0xE0,0xF8,0xFE,0xF8,0xE0,0x80,0x00,
        0x02,0x0E,0x3E,0xFE,0x3E,0x0E,0x02,0x00,
        0x18,0x3C,0x7E,0x18,0x18,0x7E,0x3C,0x18,
        0x66,0x66,0x66,0x66,0x66,0x00,0x66,0x00,
        0x7F,0xDB,0xDB,0x7B,0x1B,0x1B,0x1B,0x00,
        0x3E,0x63,0x38,0x6C,0x6C,0x38,0xCC,0x78,
        0x00,0x00,0x00,0x00,0x7E,0x7E,0x7E,0x00,
        0x18,0x3C,0x7E,0x18,0x7E,0x3C,0x18,0xFF,
// CHAR 18H
        0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x00,
        0x18,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00,
        0x00,0x18,0x0C,0xFE,0x0C,0x18,0x00,0x00,
        0x00,0x30,0x60,0xFE,0x60,0x30,0x00,0x00,
        0x00,0x00,0xC0,0xC0,0xC0,0xFE,0x00,0x00,
        0x00,0x24,0x66,0xFF,0x66,0x24,0x00,0x00,
        0x00,0x18,0x3C,0x7E,0xFF,0xFF,0x00,0x00,
        0x00,0xFF,0xFF,0x7E,0x3C,0x18,0x00,0x00,
// CHAR 20H
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x30,0x78,0x78,0x30,0x30,0x00,0x30,0x00,
        0x6C,0x6C,0x6C,0x00,0x00,0x00,0x00,0x00,
        0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00,
        0x30,0x7C,0xC0,0x78,0x0C,0xF8,0x30,0x00,
        0x00,0xC6,0xCC,0x18,0x30,0x66,0xC6,0x00,
        0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76,0x00,
        0x60,0x60,0xC0,0x00,0x00,0x00,0x00,0x00,
// CHAR 28H
        0x18,0x30,0x60,0x60,0x60,0x30,0x18,0x00,
        0x60,0x30,0x18,0x18,0x18,0x30,0x60,0x00,
        0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,
        0x00,0x30,0x30,0xFC,0x30,0x30,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x60,
        0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,
        0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00,
// CHAR 30H
        0x7C,0xC6,0xCE,0xDE,0xF6,0xE6,0x7C,0x00,
        0x30,0x70,0x30,0x30,0x30,0x30,0xFC,0x00,
        0x78,0xCC,0x0C,0x38,0x60,0xCC,0xFC,0x00,
        0x78,0xCC,0x0C,0x38,0x0C,0xCC,0x78,0x00,
        0x1C,0x3C,0x6C,0xCC,0xFE,0x0C,0x1E,0x00,
        0xFC,0xC0,0xF8,0x0C,0x0C,0xCC,0x78,0x00,
        0x38,0x60,0xC0,0xF8,0xCC,0xCC,0x78,0x00,
        0xFC,0xCC,0x0C,0x18,0x30,0x30,0x30,0x00,
// CHAR 38H
        0x78,0xCC,0xCC,0x78,0xCC,0xCC,0x78,0x00,
        0x78,0xCC,0xCC,0x7C,0x0C,0x18,0x70,0x00,
        0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x00,
        0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x60,
        0x18,0x30,0x60,0xC0,0x60,0x30,0x18,0x00,
        0x00,0x00,0xFC,0x00,0x00,0xFC,0x00,0x00,
        0x60,0x30,0x18,0x0C,0x18,0x30,0x60,0x00,
        0x78,0xCC,0x0C,0x18,0x30,0x00,0x30,0x00,
// CHAR 40H
        0x7C,0xC6,0xDE,0xDE,0xDE,0xC0,0x78,0x00,
        0x30,0x78,0xCC,0xCC,0xFC,0xCC,0xCC,0x00,
        0xFC,0x66,0x66,0x7C,0x66,0x66,0xFC,0x00,
        0x3C,0x66,0xC0,0xC0,0xC0,0x66,0x3C,0x00,
        0xF8,0x6C,0x66,0x66,0x66,0x6C,0xF8,0x00,
        0xFE,0x62,0x68,0x78,0x68,0x62,0xFE,0x00,
        0xFE,0x62,0x68,0x78,0x68,0x60,0xF0,0x00,
        0x3C,0x66,0xC0,0xC0,0xCE,0x66,0x3E,0x00,
// CHAR 48H
        0xCC,0xCC,0xCC,0xFC,0xCC,0xCC,0xCC,0x00,
        0x78,0x30,0x30,0x30,0x30,0x30,0x78,0x00,
        0x1E,0x0C,0x0C,0x0C,0xCC,0xCC,0x78,0x00,
        0xE6,0x66,0x6C,0x78,0x6C,0x66,0xE6,0x00,
        0xF0,0x60,0x60,0x60,0x62,0x66,0xFE,0x00,
        0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0x00,
        0xC6,0xE6,0xF6,0xDE,0xCE,0xC6,0xC6,0x00,
        0x38,0x6C,0xC6,0xC6,0xC6,0x6C,0x38,0x00,
// CHAR 50H
        0xFC,0x66,0x66,0x7C,0x60,0x60,0xF0,0x00,
        0x78,0xCC,0xCC,0xCC,0xDC,0x78,0x1C,0x00,
        0xFC,0x66,0x66,0x7C,0x6C,0x66,0xE6,0x00,
        0x78,0xCC,0xE0,0x70,0x1C,0xCC,0x78,0x00,
        0xFC,0xB4,0x30,0x30,0x30,0x30,0x78,0x00,
        0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xFC,0x00,
        0xCC,0xCC,0xCC,0xCC,0xCC,0x78,0x30,0x00,
        0xC6,0xC6,0xC6,0xD6,0xFE,0xEE,0xC6,0x00,
// CHAR 58H
        0xC6,0xC6,0x6C,0x38,0x38,0x6C,0xC6,0x00,
        0xCC,0xCC,0xCC,0x78,0x30,0x30,0x78,0x00,
        0xFE,0xC6,0x8C,0x18,0x32,0x66,0xFE,0x00,
        0x78,0x60,0x60,0x60,0x60,0x60,0x78,0x00,
        0xC0,0x60,0x30,0x18,0x0C,0x06,0x02,0x00,
        0x78,0x18,0x18,0x18,0x18,0x18,0x78,0x00,
        0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
// CHAR 60H
        0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x78,0x0C,0x7C,0xCC,0x76,0x00,
        0xE0,0x60,0x60,0x7C,0x66,0x66,0xDC,0x00,
        0x00,0x00,0x78,0xCC,0xC0,0xCC,0x78,0x00,
        0x1C,0x0C,0x0C,0x7C,0xCC,0xCC,0x76,0x00,
        0x00,0x00,0x78,0xCC,0xFC,0xC0,0x78,0x00,
        0x38,0x6C,0x60,0xF0,0x60,0x60,0xF0,0x00,
        0x00,0x00,0x76,0xCC,0xCC,0x7C,0x0C,0xF8,
// CHAR 68H
        0xE0,0x60,0x6C,0x76,0x66,0x66,0xE6,0x00,
        0x30,0x00,0x70,0x30,0x30,0x30,0x78,0x00,
        0x0C,0x00,0x0C,0x0C,0x0C,0xCC,0xCC,0x78,
        0xE0,0x60,0x66,0x6C,0x78,0x6C,0xE6,0x00,
        0x70,0x30,0x30,0x30,0x30,0x30,0x78,0x00,
        0x00,0x00,0xCC,0xFE,0xFE,0xD6,0xC6,0x00,
        0x00,0x00,0xF8,0xCC,0xCC,0xCC,0xCC,0x00,
        0x00,0x00,0x78,0xCC,0xCC,0xCC,0x78,0x00,
// CHAR 70H
        0x00,0x00,0xDC,0x66,0x66,0x7C,0x60,0xF0,
        0x00,0x00,0x76,0xCC,0xCC,0x7C,0x0C,0x1E,
        0x00,0x00,0xDC,0x76,0x66,0x60,0xF0,0x00,
        0x00,0x00,0x7C,0xC0,0x78,0x0C,0xF8,0x00,
        0x10,0x30,0x7C,0x30,0x30,0x34,0x18,0x00,
        0x00,0x00,0xCC,0xCC,0xCC,0xCC,0x76,0x00,
        0x00,0x00,0xCC,0xCC,0xCC,0x78,0x30,0x00,
        0x00,0x00,0xC6,0xD6,0xFE,0xFE,0x6C,0x00,
// CHAR 78H
        0x00,0x00,0xC6,0x6C,0x38,0x6C,0xC6,0x00,
        0x00,0x00,0xCC,0xCC,0xCC,0x7C,0x0C,0xF8,
        0x00,0x00,0xFC,0x98,0x30,0x64,0xFC,0x00,
        0x1C,0x30,0x30,0xE0,0x30,0x30,0x1C,0x00,
        0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00,
        0xE0,0x30,0x30,0x1C,0x30,0x30,0xE0,0x00,
        0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x10,0x38,0x6C,0xC6,0xC6,0xFE,0x00,
// CHAR 80H
        0x3C,0x66,0x60,0x66,0x3C,0x0C,0x06,0x3C,
        0x00,0x66,0x00,0x66,0x66,0x66,0x3F,0x00,
        0x0E,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00,
        0x7E,0xC3,0x3C,0x06,0x3E,0x66,0x3F,0x00,
        0x66,0x00,0x3C,0x06,0x3E,0x66,0x3F,0x00,
        0x70,0x00,0x3C,0x06,0x3E,0x66,0x3F,0x00,
        0x18,0x18,0x3C,0x06,0x3E,0x66,0x3F,0x00,
        0x00,0x00,0x3C,0x60,0x60,0x3C,0x06,0x1C,
// CHAR 88H
        0x7E,0xC3,0x3C,0x66,0x7E,0x60,0x3C,0x00,
        0x66,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00,
        0x70,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00,
        0x66,0x00,0x38,0x18,0x18,0x18,0x3C,0x00,
        0x7C,0xC6,0x38,0x18,0x18,0x18,0x3C,0x00,
        0x70,0x00,0x38,0x18,0x18,0x18,0x3C,0x00,
        0x63,0x1C,0x36,0x63,0x7F,0x63,0x63,0x00,
        0x18,0x18,0x00,0x3C,0x66,0x7E,0x66,0x00,
// CHAR 90H
        0x0E,0x00,0x7E,0x30,0x3C,0x30,0x7E,0x00,
        0x00,0x00,0x7F,0x0C,0x7F,0xCC,0x7F,0x00,
        0x1F,0x36,0x66,0x7F,0x66,0x66,0x67,0x00,
        0x3C,0x66,0x00,0x3C,0x66,0x66,0x3C,0x00,
        0x00,0x66,0x00,0x3C,0x66,0x66,0x3C,0x00,
        0x00,0x70,0x00,0x3C,0x66,0x66,0x3C,0x00,
        0x3C,0x66,0x00,0x66,0x66,0x66,0x3F,0x00,
        0x00,0x70,0x00,0x66,0x66,0x66,0x3F,0x00,
// CHAR 98H
        0x00,0x66,0x00,0x66,0x66,0x3E,0x06,0x7C,
        0xC3,0x18,0x3C,0x66,0x66,0x3C,0x18,0x00,
        0x66,0x00,0x66,0x66,0x66,0x66,0x3C,0x00,
        0x18,0x18,0x7E,0xC0,0xC0,0x7E,0x18,0x18,
        0x1C,0x36,0x32,0x78,0x30,0x73,0x7E,0x00,
        0x66,0x66,0x3C,0x7E,0x18,0x7E,0x18,0x18,
        0xF8,0xCC,0xCC,0xFA,0xC6,0xCF,0xC6,0xC7,
        0x0E,0x1B,0x18,0x3C,0x18,0x18,0xD8,0x70,
// CHAR A0H
        0x0E,0x00,0x3C,0x06,0x3E,0x66,0x3F,0x00,
        0x1C,0x00,0x38,0x18,0x18,0x18,0x3C,0x00,
        0x00,0x0E,0x00,0x3C,0x66,0x66,0x3C,0x00,
        0x00,0x0E,0x00,0x66,0x66,0x66,0x3F,0x00,
        0x00,0x7C,0x00,0x7C,0x66,0x66,0x66,0x00,
        0x7E,0x00,0x66,0x76,0x7E,0x6E,0x66,0x00,
        0x3C,0x6C,0x6C,0x3E,0x00,0x7E,0x00,0x00,
        0x38,0x6C,0x6C,0x38,0x00,0x7C,0x00,0x00,
// CHAR A8H
        0x18,0x00,0x18,0x30,0x60,0x66,0x3C,0x00,
        0x00,0x00,0x00,0x7E,0x60,0x60,0x00,0x00,
        0x00,0x00,0x00,0x7E,0x06,0x06,0x00,0x00,
        0xC3,0xC6,0xCC,0xDE,0x33,0x66,0xCC,0x0F,
        0xC3,0xC6,0xCC,0xDB,0x37,0x6F,0xCF,0x03,
        0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x00,
        0x00,0x33,0x66,0xCC,0x66,0x33,0x00,0x00,
        0x00,0xCC,0x66,0x33,0x66,0xCC,0x00,0x00,
// CHAR B0H
        0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,
        0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,
        0xDB,0x77,0xDB,0xEE,0xDB,0x77,0xDB,0xEE,
        0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
        0x18,0x18,0x18,0x18,0xF8,0x18,0x18,0x18,
        0x18,0x18,0xF8,0x18,0xF8,0x18,0x18,0x18,
        0x36,0x36,0x36,0x36,0xF6,0x36,0x36,0x36,
        0x00,0x00,0x00,0x00,0xFE,0x36,0x36,0x36,
// CHAR B8H
        0x00,0x00,0xF8,0x18,0xF8,0x18,0x18,0x18,
        0x36,0x36,0xF6,0x06,0xF6,0x36,0x36,0x36,
        0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
        0x00,0x00,0xFE,0x06,0xF6,0x36,0x36,0x36,
        0x36,0x36,0xF6,0x06,0xFE,0x00,0x00,0x00,
        0x36,0x36,0x36,0x36,0xFE,0x00,0x00,0x00,
        0x18,0x18,0xF8,0x18,0xF8,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0xF8,0x18,0x18,0x18,
// CHAR C0H
        0x18,0x18,0x18,0x18,0x1F,0x00,0x00,0x00,
        0x18,0x18,0x18,0x18,0xFF,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0xFF,0x18,0x18,0x18,
        0x18,0x18,0x18,0x18,0x1F,0x18,0x18,0x18,
        0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,
        0x18,0x18,0x18,0x18,0xFF,0x18,0x18,0x18,
        0x18,0x18,0x1F,0x18,0x1F,0x18,0x18,0x18,
        0x36,0x36,0x36,0x36,0x37,0x36,0x36,0x36,
// CHAR C8H
        0x36,0x36,0x37,0x30,0x3F,0x00,0x00,0x00,
        0x00,0x00,0x3F,0x30,0x37,0x36,0x36,0x36,
        0x36,0x36,0xF7,0x00,0xFF,0x00,0x00,0x00,
        0x00,0x00,0xFF,0x00,0xF7,0x36,0x36,0x36,
        0x36,0x36,0x37,0x30,0x37,0x36,0x36,0x36,
        0x00,0x00,0xFF,0x00,0xFF,0x00,0x00,0x00,
        0x36,0x36,0xF7,0x00,0xF7,0x36,0x36,0x36,
        0x18,0x18,0xFF,0x00,0xFF,0x00,0x00,0x00,
// CHAR D0H
        0x36,0x36,0x36,0x36,0xFF,0x00,0x00,0x00,
        0x00,0x00,0xFF,0x00,0xFF,0x18,0x18,0x18,
        0x00,0x00,0x00,0x00,0xFF,0x36,0x36,0x36,
        0x36,0x36,0x36,0x36,0x3F,0x00,0x00,0x00,
        0x18,0x18,0x1F,0x18,0x1F,0x00,0x00,0x00,
        0x00,0x00,0x1F,0x18,0x1F,0x18,0x18,0x18,
        0x00,0x00,0x00,0x00,0x3F,0x36,0x36,0x36,
        0x36,0x36,0x36,0x36,0xFF,0x36,0x36,0x36,
// CHAR D8H
        0x18,0x18,0xFF,0x18,0xFF,0x18,0x18,0x18,
        0x18,0x18,0x18,0x18,0xF8,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x1F,0x18,0x18,0x18,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
        0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,
        0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
        0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
// CHAR E0H
        0x00,0x00,0x3B,0x6E,0x64,0x6E,0x3B,0x00,
        0x00,0x3C,0x66,0x7C,0x66,0x7C,0x60,0x60,
        0x00,0x7E,0x66,0x60,0x60,0x60,0x60,0x00,
        0x00,0x7F,0x36,0x36,0x36,0x36,0x36,0x00,
        0x7E,0x66,0x30,0x18,0x30,0x66,0x7E,0x00,
        0x00,0x00,0x3F,0x6C,0x6C,0x6C,0x38,0x00,
        0x00,0x33,0x33,0x33,0x33,0x3E,0x30,0x60,
        0x00,0x3B,0x6E,0x0C,0x0C,0x0C,0x0C,0x00,
// CHAR E8H
        0x7E,0x18,0x3C,0x66,0x66,0x3C,0x18,0x7E,
        0x1C,0x36,0x63,0x7F,0x63,0x36,0x1C,0x00,
        0x1C,0x36,0x63,0x63,0x36,0x36,0x77,0x00,
        0x0E,0x18,0x0C,0x3E,0x66,0x66,0x3C,0x00,
        0x00,0x00,0x7E,0xDB,0xDB,0x7E,0x00,0x00,
        0x06,0x0C,0x7E,0xDB,0xDB,0x7E,0x60,0xC0,
        0x1C,0x60,0xC0,0xFC,0xC0,0x60,0x1C,0x00,
        0x3C,0x66,0x66,0x66,0x66,0x66,0x66,0x00,
// CHAR F0H
        0x00,0x7E,0x00,0x7E,0x00,0x7E,0x00,0x00,
        0x18,0x18,0x7E,0x18,0x18,0x00,0x7E,0x00,
        0x30,0x18,0x0C,0x18,0x30,0x00,0x7E,0x00,
        0x0C,0x18,0x30,0x18,0x0C,0x00,0x7E,0x00,
        0x0E,0x1B,0x1B,0x18,0x18,0x18,0x18,0x18,
        0x18,0x18,0x18,0x18,0x18,0xD8,0xD8,0x70,
        0x18,0x18,0x00,0x7E,0x00,0x18,0x18,0x00,
        0x00,0x76,0xDC,0x00,0x76,0xDC,0x00,0x00,
// CHAR F8H
        0x38,0x6C,0x6C,0x38,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,
        0x0F,0x0C,0x0C,0x0C,0xEC,0x6C,0x3C,0x1C,
        0x78,0x6C,0x6C,0x6C,0x6C,0x00,0x00,0x00,
        0x70,0x18,0x30,0x60,0x78,0x00,0x00,0x00,
        0x00,0x00,0x3C,0x3C,0x3C,0x3C,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/*
 * We program the VDU4 font to be the DOS font for the following characters
 * 0x20 - 0x7E, 0x80 - 0xFE.
 *
 * Whenever any of the characters not in the above range are needed then
 * they are temporarily programmed into character 0xFF and that is then
 * displayed.
 */

#define REDEF_CHAR	0xFF

static int last_redef = -1;

static void define_char(char c, char to)
{
    char b[10];

    b[0] = 23;
    b[1] = to;
    memcpy(&b[2], &font_definition[c * 8], 8);

    LOGERR(_swix(OS_WriteN, _INR(0,1), b, sizeof(b)));
}

int read_char(void)
{
    return VioScreen[ (cursor_y * usMaxCol + cursor_x) * 2 ];
}

void write_char(char c)
{
    // update screen array
    char *p = &VioScreen[ (cursor_y * usMaxCol + cursor_x) * 2 ];
    p[0] = c;
    p[1] = current_attr;

    // write character, redefining if necessary
    if (c < 0x20 || c == 0x7F || c == REDEF_CHAR)
    {
	if (c != last_redef)
	{
	    define_char(c, REDEF_CHAR);
	    last_redef = c;
	}
	LOGERR(_swix(OS_WriteI + REDEF_CHAR, 0));
    }
    else
    {
	LOGERR(_swix(OS_WriteI + c, 0));
    }

    // update cursor position
    cursor_x++;
}

void setup_char_defs(void)
{
    int i;
    for (i = 32; i < 255; i++)
    {
	define_char(i, i);
	last_redef = REDEF_CHAR;
    }
}

void reset_char_defs(void)
{
    /* reset all chars 32-255 to their default shapes */
    LOGERR(_swix(OS_Byte, _INR(0,1), 25, 0));
    last_redef = -1;
}

/* eof chars.c */
