/* mtz.h - media net tools header file */

#ifndef MTZ_ORACLE
#define MTZ_ORACLE

#define MAX_NAME_LEN 256
#define DBUF_SIZE    1024

extern HINSTANCE hInst;

typedef struct mtzctx mtzctx;

struct mtzctx
{
  char mtzMnProt[MAX_NAME_LEN];
  char mtzMnHost[MAX_NAME_LEN];
  char mtzMnPort[MAX_NAME_LEN];
  char mtzMnAddr[MAX_NAME_LEN];
  void (*mtzSyslp)(CONST char *buf);
  void (*mtzSysle)(CONST char *buf);
  HANDLE mtzHdlThrd;              /* mnping thread handle */
};

extern mtzctx mtz;

/* function prototype */
BOOL CALLBACK mtzDlgMain (HWND, UINT, UINT, LPARAM);
BOOL CALLBACK mtzDlgMnSvr (HWND, UINT, UINT, LPARAM);
BOOL CALLBACK mtzDlgMnLs (HWND, UINT, UINT, LPARAM);
#endif /* MTZ_ORACLE */