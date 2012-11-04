#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef UNICODE
#define UNICODE
#endif

#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif
#endif

#ifndef NDEBUG
#define NDEBUG
#endif

#define WINVER       0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_IE    0x0600
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tlhelp32.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <psapi.h>
#include <tchar.h>
#include "resource.h"
#include "debug.h"
#define ID_TIMER 1
#define RUNAS_MODE_CURRENT 1
#define RUNAS_MODE_LIMITED 2

/* 全局变量 */
extern HWND g_hwnd;
extern HINSTANCE g_hInst;
extern HMENU g_hmenu;
extern HWND g_hwndListView;
extern HWND g_hwndStatusBar;
extern HIMAGELIST g_hImageList;
extern BOOL IsTimerOn;
extern BOOL bShowCmdLine;
extern int SelectedRunAsMode;
#endif
