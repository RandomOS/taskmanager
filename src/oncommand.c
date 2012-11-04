#include "oncommand.h"
#include "listview.h"
#include "process.h"
#include "uifunction.h"

void OnRefresh(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    if (!IsWindow(g_hwndListView))
    {
        return;
    }
    UpdateListView();
    SetFocus(g_hwndListView);
}

void OnKill(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    if (!IsWindow(g_hwndListView))
    {
        return;
    }
    KillSelectProcess();
    SetFocus(g_hwndListView);
}

void OnRun(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	SelectedRunAsMode = RUNAS_MODE_CURRENT;
    ShowRunDlg(hwnd, 0);
}

void OnRunAsLimit(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	SelectedRunAsMode = RUNAS_MODE_LIMITED;	
    ShowRunDlg(hwnd, 0);
}

void OnExit(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    DestroyWindow(hwnd);
}

void OnTopMost(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    HMENU hMenu;
    DWORD dwExStyle;

    hMenu = GetMenu(hwnd);
    hMenu = GetSubMenu(hMenu, 1);

    dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (dwExStyle & WS_EX_TOPMOST)
    {
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        CheckMenuItem(hMenu, IDM_TOPMOST, MF_BYCOMMAND | MF_UNCHECKED);
    }
    else
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        CheckMenuItem(hMenu, IDM_TOPMOST, MF_BYCOMMAND | MF_CHECKED);
    }
}

void OnAutoRefresh(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    HMENU hMenu;

    hMenu = GetMenu(hwnd);
    hMenu = GetSubMenu(hMenu, 1);

    if (IsTimerOn)
    {
        CheckMenuItem(hMenu, IDM_AUTOREFRESH, MF_BYCOMMAND | MF_UNCHECKED);
        KillTimer(hwnd, ID_TIMER);
        IsTimerOn = FALSE;
    }
    else
    {
        CheckMenuItem(hMenu, IDM_AUTOREFRESH, MF_BYCOMMAND | MF_CHECKED);
        SetTimer(hwnd, ID_TIMER, 1000, NULL);
        IsTimerOn = TRUE;
    }
}

void OnShowCmdLine(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    HMENU hMenu;
    LVCOLUMN lvc = {0};

    hMenu = GetMenu(hwnd);
    hMenu = GetSubMenu(hMenu, 1);

    if (bShowCmdLine)
    {
        bShowCmdLine = FALSE;
        CheckMenuItem(hMenu, IDM_SHOWCMDLINE, MF_BYCOMMAND | MF_UNCHECKED);
        ListView_DeleteColumn(g_hwndListView, lvProcessCmdLine);
    }
    else
    {
        lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
        lvc.iSubItem = lvProcessCmdLine;
        lvc.pszText = _T("进程命令行");
        lvc.cx = 550;
        lvc.fmt = LVCFMT_LEFT;
        ListView_InsertColumn(g_hwndListView, lvProcessCmdLine, &lvc);
        CheckMenuItem(hMenu, IDM_SHOWCMDLINE, MF_BYCOMMAND | MF_CHECKED);
        bShowCmdLine = TRUE;
        UpdateListView();
    }
}

void OnSysInfo(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    ShellExecute(hwnd, _T("Open"), _T("msinfo32.exe"), NULL, NULL, SW_SHOWNORMAL);
}

void OnEmptyAll(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    if (!IsWindow(g_hwndListView))
    {
        return;
    }
    EmptyAllProcessWorkingSet();
}

void OnAbout(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    ShowAboutDlg(hwnd);
}

void OnOpenLocation(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    int iIdx;
    TCHAR szFullPath[MAX_PATH];

    if (!IsWindow(g_hwndListView))
    {
        return;
    }

    iIdx = ListView_GetSelectionMark(g_hwndListView);
    ListView_GetItemText(g_hwndListView, iIdx, lvProcessPath, szFullPath, sizeof(szFullPath) / sizeof(TCHAR));
    if (!OpenFileLocation(hwnd, szFullPath))
    {
        MessageBox(hwnd, _T("无法打开文件位置。"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
    }
}

void OnEmptyMem(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    DWORD dwProcessId;

    if (!IsWindow(g_hwndListView))
    {
        return;
    }

    dwProcessId = GetPIDFromSelectItem();
    if (dwProcessId == 0)
    {
        return;
    }

    if (!ReduceProcessWorkingSet(dwProcessId))
    {
        MessageBox(hwnd, _T("拒绝访问。"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
    }
}

void OnSuspend(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    int iIdx;
    DWORD dwProcessId;

    if (!IsWindow(g_hwndListView))
    {
        return;
    }

    dwProcessId = GetPIDFromSelectItem();
    if (dwProcessId == 0)
    {
        return;
    }

    iIdx = ListView_GetSelectionMark(g_hwndListView);
    if (!IsProcessSuspend(dwProcessId))
    {
        if (!SuspendProcess(dwProcessId))
        {
            MessageBox(hwnd, _T("无法挂起进程。"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
        }
        else
        {
            SetListViewItemParam(g_hwndListView, iIdx, 1);
        }
    }
    else
    {
        SetListViewItemParam(g_hwndListView, iIdx, 1);
    }
}

void OnResume(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    int iIdx;
    DWORD dwProcessId;

    if (!IsWindow(g_hwndListView))
    {
        return;
    }

    dwProcessId = GetPIDFromSelectItem();
    if (dwProcessId == 0)
    {
        return;
    }

    iIdx = ListView_GetSelectionMark(g_hwndListView);
    if (IsProcessSuspend(dwProcessId))
    {
        if (!ResumeProcess(dwProcessId))
        {
            MessageBox(hwnd, _T("无法恢复挂起进程。"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
        }
        else
        {
            SetListViewItemParam(g_hwndListView, iIdx, 0);
        }
    }
    else
    {
        SetListViewItemParam(g_hwndListView, iIdx, 0);
    }
}

void OnCopyCmdLine(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    int cch;
    HGLOBAL hMem;
    HANDLE hClipData;
    LPTSTR lptstrCopy;
    TCHAR szCmdLine[1024];

    if (!IsWindow(g_hwndListView))
    {
        return;
    }

    if (!OpenClipboard(NULL))
    {
        MessageBox(g_hwnd, _T("打开剪贴板失败！"), _T("提示"), MB_OK);
        return;
    }
    EmptyClipboard();

    if (!GetProcessCommandLineByPID(GetPIDFromSelectItem(), szCmdLine, sizeof(szCmdLine) / sizeof(TCHAR)))
    {
        MessageBox(g_hwnd, _T("获取进程命令行失败！"), _T("提示"), MB_OK);
        CloseClipboard();
        return;
    }
    //RemoveQuotes(szCmdLine);

    cch = lstrlen(szCmdLine);
    hMem = GlobalAlloc(GMEM_MOVEABLE, (cch + 1) * sizeof(TCHAR));
    if (hMem == NULL)
    {
        CloseClipboard();
        return;
    }

    lptstrCopy = (LPTSTR)GlobalLock(hMem);
    lstrcpy(lptstrCopy, szCmdLine);
    // Once done, I unlock the memory - remember you
    // don't call GlobalFree because Windows will free the
    // memory automatically when EmptyClipboard is next
    // called.
    GlobalUnlock(hMem);
#ifdef UNICODE
    hClipData = SetClipboardData(CF_UNICODETEXT, hMem);
#else
    hClipData = SetClipboardData(CF_TEXT, hMem);
#endif
    CloseClipboard();
    if (hClipData != NULL)
    {
        MessageBox(g_hwnd, _T("进程命令行已复制到剪贴板！"), _T("提示"), MB_ICONINFORMATION);
    }
    else
    {
        MessageBox(g_hwnd, _T("设置进程命令行到剪贴板失败！"), _T("提示"), MB_OK);
    }
}

void OnProperties(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    int iIdx;
    TCHAR szFullPath[MAX_PATH];

    if (!IsWindow(g_hwndListView))
    {
        return;
    }

    iIdx = ListView_GetSelectionMark(g_hwndListView);
    ListView_GetItemText(g_hwndListView, iIdx, lvProcessPath, szFullPath, sizeof(szFullPath) / sizeof(TCHAR));
    if (!ShowFileProperties(hwnd, szFullPath))
    {
        MessageBox(hwnd, _T("无法显示文件属性。"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
    }
}
