#define CARD_CMD_LEN		4
#define CARD_RESP_LEN		2

int	CardIntAuth(BYTE *plain, int pl_len, int key_num, BYTE *cyphr,
					int max_len, int *cy_len);
int	CardReadSrn(BYTE *srn, int buff_len, int *len);
int	CardOpen(HWND hWnd);
int	CardOpened();
int	CardClose();
