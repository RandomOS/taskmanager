#ifndef _ONCOMMAND_H_
#define _ONCOMMAND_H_
#include "common.h"
#define RFN_VALIDATE (-510)
#define RF_OK        (0x0)
#define RF_CANCEL    (0x1)
#define RF_RETRY     (0x2)

void OnRefresh(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnKill(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnRun(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnRunAsLimit(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnExit(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnTopMost(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnAutoRefresh(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnShowCmdLine(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnSysInfo(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnEmptyAll(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnAbout(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnOpenLocation(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnEmptyMem(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnSuspend(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnResume(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnCopyCmdLine(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnProperties(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
#endif
