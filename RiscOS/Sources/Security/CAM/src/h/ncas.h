/*
** Header:			sock.h	
** Description:		General Socket Library, take from Internetworking with 
**					TCP/IP Volume III BSD Socket Version  D. Comer / D. Stevens
** System:			STAP/x
** Author:			Marc Grossman
** Designed By:		Marc Grossman
** Date Written:	96/08/12
** Date Amended:	
** Installation:	Applied Systems Engineering Ltd, Lytham, Lancs.
** Revision:		0.1
**
** Assumptions:		
** Notes:			
*/
  
#ifndef FALSE
#error FALSE not defined
#endif

#ifndef TRUE
#error TRUE not defined
#endif    

#define SOCKL_ERROR					INT_MAX

#define SOCKET_RCV_FRAGMENT_LIMIT	512
#define SOCKET_LINGER_TIME			10	/* seconds */
#define NCAS_SOCK_BACKLOG			1	/* FOR THE TIME BEING */
#define NCAS_TCP_PROTO				"tcp"
#define NCAS_SERVICE				"ncas"
#define NCAS_HOST					"ncas_host"
#define NCAS_DAEMON_SERVICE			"ncasd"
#define NCAS_READ_NEXT				1
#define NCAS_SEND_NEXT				2

#ifndef PATH_MAX
#define PATH_MAX					256
#endif	/* PATH_MAX */

#ifndef NAME_MAX
#define NAME_MAX					14
#endif	/* NAME_MAX */

#define NCAS_TRACE_DIR				"c:\\"
#define NCAS_TRACE_FILE_NAME		"camtrace"
#define NCAS_MAX_INS				10

#define SOCKET_LINGER_TIME			10	/* seconds */
#define NO_FLAGS_SET				0 /* Used with recv()/send() */

#define NCAS_INTERNAL_MSG			WM_USER + 16

#define MAKEWORD(a, b) \
    ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))

extern int			G_comms_failed;

int		ncas_InitSockets();
int		ncas_Disconnect(BOOL aut, BOOL trigger);
int		ncas_ConnectionAccepted(HWND hWnd, WORD wParam, LONG lParam);
int		ncas_Connect(void);
int		ncas_ReceiveData(BYTE *data_buffer, int buff_len);
int		ncas_SendData(BYTE *data_buffer, int len);
int		ncas_StartListener(char *service);
int		ncas_SockInit();
int		ncas_SocklTerm();
