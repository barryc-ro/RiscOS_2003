/* > serial.c

 *

 */

#include "windows.h"

#include "../../../inc/client.h"

#include "swis.h"

#include "tdasync.h"

/*
 * 'handle' is currently always passed as '1' and so can be ignored.
 * 'irq' and 'io_addr' are not relevant for RISC OS and so are ignored
 * 'flag' is passed as 'FALSE' and I don't know what it means.
 */

int AsyncOpen( int handle, unsigned int irq , unsigned int io_addr, int flag)
{
    TRACE((TC_TD,TT_API2, "AsyncOpen: "));

    /* enable serial input whilst keeping keyboard active */
    return LOGERR(_swix(OS_Byte, _INR(0,1), 2, 2)) != NULL;
}

int AsyncClose( int handle )
{ 
    TRACE((TC_TD,TT_API2, "AsyncClose: "));

    /* don't disable serial device in case someone else wanted it */
    return 0;
}

int AsyncRead( int handle, char * buf, int bufsize, USHORT *bytes_read )
{
    int i;

    DTRACE((TC_TD,TT_API4, "AsyncRead: %p (%d)", buf, bufsize));

    for (i = 0; i < bufsize; i++)
    {
	int c, flags;

	if (LOGERR(_swix(OS_SerialOp, _IN(0) | _OUT(1) | _OUT(_FLAGS), 4, &c, &flags)) != NULL ||
	    (flags & _C))
	{
	    break;
	}

	buf[i] = c;
    }

    DTRACE((TC_TD,TT_API4, "AsyncRead: read %d bytes", i));

    *bytes_read = i;
    
    return 0;
}

/* actual baud rate */

static int rates[] =
{
    9600, 75, 150, 300, 1200, 2400, 4800, 9600, 19200,
    50, 110, 135, 600, 1800, 3600, 7200,
    38400, 57600, 115200
};

#define NRATES (sizeof(rates)/sizeof(rates[0]))

int AsyncSetBaud( int handle, long baud_rate)
{
    int i;

    TRACE((TC_TD,TT_API2, "AsyncSetBaud: %ld", baud_rate));

    for (i = 0; i < NRATES; i++)
    {
	if (rates[i] == baud_rate)
	{
	    LOGERR(_swix(OS_SerialOp, _INR(0,1), 5, i));
	    LOGERR(_swix(OS_SerialOp, _INR(0,1), 6, i));

	    return 0;
	}
    }
    
    ASSERT(0, (int)baud_rate);
    return 1;
}


/*
 * Handshaking:
 * Default DTR_CONTROL
 * If HardwareFlowControl is set
 *    Rx can be DTR or or RTS
 *    Tx can be CTR or DSR
 */

#define ro_SW_FLOW	(1<<0)
#define ro_IGNORE_DCD	(1<<1)
#define ro_IGNORE_DSR	(1<<2)
#define ro_DTR_OFF	(1<<3)
#define ro_IGNORE_CTS	(1<<4)
#define ro_NO_RTS_SHAKE	(1<<5)
#define ro_INPUT_SUPPRESSED (1<<6)
#define ro_RTS_HIGH	(1<<7)
#define ro_MASK		(0xff)

int AsyncSetDCB( int handle, PDCBINFO dcb)
{
    int eor = 0;
    int bic = 0;
    
    TRACE((TC_TD,TT_API1, "AsyncSetDCB:"));

    // cannot do DTR handshaking
    ASSERT((dcb->fbCtlHndShake & MODE_DTR_HANDSHAKE) == 0, dcb->fbCtlHndShake);
    ASSERT((dcb->fbCtlHndShake & MODE_DTR_CONTROL), dcb->fbCtlHndShake);

    // not sure whether we can do DCD handshaking but driver doesn't seem to use it
    ASSERT((dcb->fbCtlHndShake & MODE_DCD_HANDSHAKE) == 0, dcb->fbCtlHndShake);

    // no idea what this does so check it isn't set
    ASSERT((dcb->fbCtlHndShake & MODE_DSR_SENSITIVITY) == 0, dcb->fbCtlHndShake);

    // ignore MODE_DTR_CONTROL as it will always be set

    // check whether to use CTS or DSR handshaking
    bic |= ro_IGNORE_CTS;
    if ((dcb->fbCtlHndShake & MODE_CTS_HANDSHAKE) == 0)
	eor |= ro_IGNORE_CTS;

    bic |= ro_IGNORE_DSR;
    if ((dcb->fbCtlHndShake & MODE_DSR_HANDSHAKE) == 0)
	eor |= ro_IGNORE_DSR;

    bic |= ro_NO_RTS_SHAKE | ro_RTS_HIGH;
    if ((dcb->fbFlowReplace & MODE_RTS_HANDSHAKE) == 0)
	eor |= ro_NO_RTS_SHAKE;
    
    // s/w flow control
    bic |= ro_SW_FLOW;
    if ((dcb->fbFlowReplace & (MODE_AUTO_TRANSMIT | MODE_AUTO_RECEIVE)) == (MODE_AUTO_TRANSMIT | MODE_AUTO_RECEIVE))
    {
	eor |= ro_SW_FLOW;
	
	ASSERT((dcb->bXONChar == 0x11), dcb->bXONChar);
	ASSERT((dcb->bXOFFChar == 0x13), dcb->bXOFFChar);
    }

    // bErrorReplacementChar, bBreakReplacementChar, usWriteTimeout, usReadTimeout are all unused

    // fbTimeout is set to MODE_WAIT_READ_TIMEOUT. We can't change it so hope it's OK.

    LOGERR(_swix(OS_SerialOp, _INR(0,2), 0, eor, (~bic) & ro_MASK ));
    
    return 0;
}

int AsyncSetLineCtrl( int handle, PLINECONTROL line)
{
    int f = 0;

    TRACE((TC_TD,TT_API1, "AsyncSetLineCtrl:"));

    switch (line->bDataBits)
    {
    case 8:
	f |= 0;
	break;
    case 7:
	f |= 1;
	break;
    case 6:
	f |= 2;
	break;
    case 5:
	f |= 3;
	break;
    default:
	return 1;
    }

    switch (line->bParity)
    {
    case 0:
	break;
    case 1:/* ODD */
	f |= (1<<3) | (0<<4);
	break;
    case 2:/* EVEN */
	f |= (1<<3) | (1<<4);
	break;
    case 3:/* MARK */
	f |= (1<<3) | (2<<4);
	break;
    case 4:/* SPACE */
	f |= (1<<3) | (3<<4);
	break;
    default:
	return 1;
    }

    switch (line->bStopBits)
    {
    case DOS_STOP1:
	break;

    case DOS_STOP15:
    case DOS_STOP2:
	f |= 1<<2;
	break;

    default:
	return 1;
    }

    // ignore break;
    
    LOGERR(_swix(OS_SerialOp, _INR(0,1), 1, f));
    return 0;
}

/* only cmd used is DTR_OFF */
int AsyncSetModemCtrl( int handle, int cmd)
{
    TRACE((TC_TD,TT_API1, "AsyncSetModemCtrl:"));

    switch (cmd)
    {
    case DTR_ON:
    case DTR_OFF:
	LOGERR(_swix(OS_SerialOp, _INR(0,2), 0, 1<<3, cmd == DTR_ON ? 0 : 1<<3));
	break;

    default:
	ASSERT(0, cmd);
	break;
    }
    return 0;
}

int AsyncWrite( int handle, char *buf, int bufsize, PUSHORT bytes_written)
{
    int i;

    TRACE((TC_TD,TT_API1, "AsyncWrite: %p(%d)", buf, bufsize));

    for (i = 0; i < bufsize; i++)
    {
	int flags;

	if (LOGERR(_swix(OS_SerialOp, _INR(0,1) | _OUT(_FLAGS), 3, buf[i], &flags)) != NULL ||
	    (flags & _C))
	{
	    break;
	}
    }

    *bytes_written = i;
    
    return 0;
}

int AsyncWriteFlush( int handle )
{
    TRACE((TC_TD,TT_API1, "AsyncWriteFlush:"));

    /* no equivalent on RISC OS */
    return 0;
}

int AsyncSendBreak( int handle, int ms_time )
{
    TRACE((TC_TD,TT_API1, "AsyncSendBreak: %dms", ms_time));

    return LOGERR(_swix(OS_SerialOp, _INR(0,1), 2, ms_time/10)) != NULL;
}

/* eof serial.c */
