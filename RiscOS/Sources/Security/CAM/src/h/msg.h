
#define MSG_CARD_ERR		1
#define MSG_NO_NCAS			2
#define MSG_NCAS_AUT_ERR	3
#define MSG_NC_AUT_ERR		4
#define MSG_NCAS_DIS		5
#define MSG_NCAS_LOST		6
#define MSG_SOCK_ERR		7
#define MSG_PDU_ERR			8
#define MSG_SW_ERR			9
#define MSG_SYS_ERR			10

#define MSG_CARD			"Your card is faulty,"
#define MSG_AUTH			"There has been an authentication problem," 
#define MSG_CONTACT			"please contact the Support Centre, quoting error number"
#define MSG_NUM_PREFIX		"AUTH"

#define MSG_LINE_LEN		30
#define MAX_MSG_LEN			256

#define ERR_BOX_HEAD		"Client Authentication Module Fatal Error"

void	DisplayMsg(HWND hWnd, int msg_num);
