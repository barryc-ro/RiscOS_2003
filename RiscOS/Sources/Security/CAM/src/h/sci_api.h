/*---------------------------------------------------------------------------

	File: sci_api.h

	Purpose:
		To provide definition of constants and functions required by an
		application using the smart card interface API

---------------------------------------------------------------------------

	Written by A.S.E Consulting for Card Dynamics
	Copyright (c) 1996 A.S.E Consulting  All Rights Reserved.

---------------------------------------------------------------------------*/

/* function return success/failure values */

#define SCI_SUCCESS		0	/* function successful return value */
#define SCI_FAIL		-1	/* function failed return code */

/* error codes which may be returned by sci_Error() */

#define SCI_E_SEQ		1	/* function called out of sequence */
#define SCI_E_INVAL		2	/* function argument is invalid */                                                                        
#define SCI_E_NODATA	3	/* response data not yet available */
#define SCI_E_TRANS		4	/* card locked by calling sci_StartTrans() */
#define SCI_E_TRUNC		5	/* buffer not long enough for available data */
#define SCI_E_SYS		6	/* unexpected error from system  software */
#define SCI_E_API		7	/* unexpected error detected within the API */
#define SCI_E_TOOMANY	8	/* limit on number connections exceeded */
#define SCI_E_PROTO		9	/* smart card protocol error */
#define SCI_E_COMMS		10	/* error in communications with SCIP */
#define SCI_E_OUT		11	/* card has been removed */

/* events which may be asynchronously reported */

#define SCI_REMOVED		1	/* card removed */
#define SCI_INSERTED	2	/* card inserted */
#define SCI_TRANS_END	3	/* card no longer locked */
#define SCI_CMD_DONE	4	/* response, and any data waiting to be read */
#define SCI_ERROR		5	/* unrecoverable error condition detected */

/* misc */

/* value for msg_id on sci_Open() if synchronous mode to be used */
#define SCI_SYNC			(UINT)0
#define SCI_HIST_LEN		16		/* max length of historical data */
#define SCI_MAX_DATA_LEN	255		/* max length data to be sent to card */
#define SCI_MAX_EXP_LEN		256		/* max length data from card */

/* custom data types */

typedef unsigned long		ULONG;	/* unsigned long (32-bit) */
typedef unsigned int		UINT;	/* unsigned int (16-bit) */
typedef unsigned char		BYTE;	/* unsigned char (8-bit) */

/* construct event message from connection id and something yet to be decided */
/* this allows for future expansion, eg adding comms failure
	reason etc */
#define MKEVMSG(conn, rfu)	((LONG)(((UINT)(conn)) | \
				(((ULONG)((UINT)(rfu))) << 16)))

/* get connection id from 32-bit event data */
#define SCI_GETCONN(l)		((UINT)(ULONG)(l))
/* get reserved part of event data */
/* #define SCI_GETRFU(l)		((UINT)((((ULONG)(l)) >> 16) & 0xFFFF)) */

/* library function declarations */
#ifdef ARM
int 	FAR PASCAL sci_Open(UINT msg_id, int *card_hnd);
#else	/* ARM */
int		FAR PASCAL sci_Open(UINT msg_id, HWND hWnd, int *card_hnd);
#endif	/* ARM */
int 	FAR PASCAL sci_RetrieveATR(int card_hnd, BYTE FAR *hist,
							int hist_len, int FAR *rcv_len);
int 	FAR PASCAL sci_Close(int card_hnd);
int 	FAR PASCAL sci_IssueCommand(int card_hnd, BYTE FAR *cmd_hdr,
							int data_len, BYTE FAR *data, int exp_len);
int 	FAR PASCAL sci_ReadResponse(int card_hnd, BYTE FAR *buff,
							int buff_len, int FAR *rcv_len, BYTE FAR *resp);
int		FAR PASCAL sci_StartTrans(int card_hnd);
int		FAR PASCAL sci_EndTrans(int card_hnd);
int		FAR PASCAL sci_Error(int card_hnd);
char FAR *	FAR PASCAL sci_GetErrorText(int err, BOOL short_name);

#ifdef NO_SCIP
int 	FAR PASCAL sci_SetEventWord(UINT ev_word, BOOL send_msg);
#endif	/* NO_SCIP */
