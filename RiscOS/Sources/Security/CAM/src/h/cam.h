/* first version is 1.1 */
#define MAJOR_VERSION		1
#define MINOR_VERSION		1

/* PDU types */
#define PDU_NCAS_INIT		1
#define PDU_CAM_INIT		2
#define PDU_CAM_CHALL		3
#define PDU_CAM_RESP		4
#define PDU_CAM_STATUS		5
#define PDU_EVENT			6

/* event PDU and ADU status codes */
#define NCAS_STATUS_OK		0
#define CAM_STATUS_OK		0
#define EPDU_CARD			1
#define EPDU_AUT			2
#define EPDU_SEQ			3
#define EPDU_FORMAT			4
#define EPDU_SYS			5
#define EPDU_SW				6

/* state function return codes */
#define CAM_OK				0
#define CAM_ERROR			-1
#define CAM_SOCK_ERR		-2
#define CAM_CARD_ERR		-3
#define CAM_SYS_ERR			-4
#define CAM_NCAS_AUT_ERR	-5
#define CAM_FMT_ERR			-6
#define CAM_SEQ_ERR			-7

/* states */
#define S_CAM_CONN_INIT_PD	1
#define S_CAM_CONN_PD		2
#define S_NCAS_OVER_PD		3
#define S_CAM_CHALLENGE_PD	4
#define S_CAM_STATUS_PD		5
#define S_NCAS_CONN_CONF_PD	6
#define S_NO_CHALLENGE		7
#define S_FAILED			8
#define MAX_STATES			8

/* events */
#define E_PDU_RCVD			1
#define E_CONN_RQ			2
#define E_CONN_CONF			3
#define E_NCAS_DISC			4
#define E_TIMER_EXP			5
#define MAX_EVENTS			5

#define NCAS_SOCK_MSG_ID	"NCAS_SOCK_MSG_ID"
#define DEFAULT_NCAS_HOST	"ncas_host"

#ifdef _DEBUG
#define NCAS_LISTEN_TIME	10000		/* 30 secs in milli-secs */
#define NCAS_CONNECT_TIME	10000		/* 10 secs in milli-secs */
#define NCAS_FIRST_PDU_TIME	10000		/* 10 secs in milli-secs */
#define NCAS_PDU_TIME		10000		/* 10 secs in milli_secs */
#define NCAS_NO_CHL_TIME	2000		/* 2 secs in milli_secs */
#define NCAS_OVERLAP_TIME	3000		/* 3 secs in milli_secs */
#define NCAS_NEXT_CHL_TIME	(3000 + NCAS_OVERLAP_TIME)		/* 3 mins + overlap in milli_secs */
#else	/* ! _DEBUG */
#define NCAS_NO_CHL_TIME	120000		/* 2 mins in milli_secs */
#define NCAS_OVERLAP_TIME	120000		/* 2 mins in milli_secs */
#define NCAS_NEXT_CHL_TIME	(180000 + NCAS_OVERLAP_TIME)		/* 3 mins + overlap in milli_secs */
#define NCAS_LISTEN_TIME	30000		/* 30 secs in milli-secs */
#define NCAS_CONNECT_TIME	10000		/* 10 secs in milli-secs */
#define NCAS_FIRST_PDU_TIME	10000		/* 10 secs in milli-secs */
#define NCAS_PDU_TIME		10000		/* 10 secs in milli_secs */
#endif	/* _DEBUG */
#define CAM_TIMER_ID		1996
#define ONE_MINUTE			60000		/* 1 minute in milli-secs */

/* encryption key range */
#define KEY_LOWER			0
#define KEY_UPPER			7

/* buffer lengths */
#define PDU_HDR_LEN			3
#define PDU_DATA_LEN		128
#define PDU_MAX_LEN			(PDU_DATA_LEN + PDU_HDR_LEN)

#define MAX_PLAIN_LEN		128
#define MAX_CYPHR_LEN		128
#define MAX_SRN_LEN			16
#define ADU_STATUS_LEN		1
#define ADU_LEN_LEN			1
#define ADU_VER_LEN			1
#define KEY_NUM_LEN			1
#define PDU_TYPE_POS		0
#define PDU_LEN_POS			1

/* event PDU type values */
#define EPDU_ERR			1
#define EPDU_WARN			2

/* global variables */
extern UINT	ncas_Msg;
extern HWND	ncas_Window;
extern char	*NcasHost;

extern int	ActPlainLen;
extern int	ActCyphrLen;
extern int	ActSrnLen;
extern int	ActKeyLen;

extern LONG	TimeToGo;

/* global functions */
int		StateInit(HWND hWnd);
int		InvokeStateFunc(int event, HWND hWnd, WORD wParam, LONG lParam);
void	StateClose(void);

void	cam_InitTrace(char *dir_name);
int		cam_OpenTraceFile(void);
void	cam_Trace(char *format, ...);
void	cam_CloseTraceFile(void);
void	cam_Log(char *format, ...);
void	WriteCAMBlock(LPSTR, int);

int		StartTimer(HWND hWnd, LONG tim);
int		ContinueTimer(HWND hWnd);
int		CancelTimer(HWND hWnd);
