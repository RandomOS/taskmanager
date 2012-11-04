#ifndef _MAIN_H_
#define _MAIN_H_
#include "common.h"
#include <uxtheme.h>

#ifdef _MSC_VER
#include <winsafer.h>
#else
#define SAFER_SCOPEID_MACHINE  1
#define SAFER_SCOPEID_USER     2

#define SAFER_LEVELID_FULLYTRUSTED 0x40000
#define SAFER_LEVELID_NORMALUSER   0x20000
#define SAFER_LEVELID_CONSTRAINED  0x10000
#define SAFER_LEVELID_UNTRUSTED    0x01000
#define SAFER_LEVELID_DISALLOWED   0x00000

DECLARE_HANDLE(SAFER_LEVEL_HANDLE);

typedef BOOL (WINAPI *pfnSaferCreateLevel)(DWORD, DWORD, DWORD, SAFER_LEVEL_HANDLE *, LPVOID);
typedef BOOL (WINAPI *pfnSaferCloseLevel)(SAFER_LEVEL_HANDLE);
typedef BOOL (WINAPI *pfnSaferComputeTokenFromLevel)(SAFER_LEVEL_HANDLE, HANDLE, PHANDLE, DWORD, LPVOID);
#endif

#define ON_COMMAND(CtrlId, fn) case (CtrlId): (fn)((hwnd),(id),(hwndCtl),(codeNotify)); break
BOOL WINAPI Main_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Main_OnSize(HWND hwnd, UINT state, int cx, int cy);
LRESULT Main_OnNotify(HWND hwnd, int idCtrl, LPNMHDR pNMHdr);
BOOL Main_OnWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos);
void Main_OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
void Main_OnClose(HWND hwnd);
void Main_OnDestroy(HWND hwnd);
#endif
