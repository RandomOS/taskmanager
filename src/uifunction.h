#ifndef _UIFUCTION_H_
#define _UIFUCTION_H_
#include "common.h"

typedef int (WINAPI *pfnRunFileDlg)(HWND, HICON, LPWSTR, LPWSTR, LPWSTR, DWORD);

void ShowAboutDlg(HWND hwnd);
void ShowRunDlg(HWND hwnd, DWORD dwFlags);
BOOL OpenFileLocation(HWND hwnd, LPTSTR szFullPath);
BOOL ShowFileProperties(HWND hwnd, LPTSTR szFullPath);
#endif
