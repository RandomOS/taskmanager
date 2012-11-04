#include "uifunction.h"

/* ��ʾ���ڶԻ��� */
void ShowAboutDlg(HWND hwnd)
{
    TCHAR szApp[] = _T("���������");
    TCHAR szOtherStuff[] = _T("CopyRight (C) GYQM 2011-12-04");
    HMODULE hModule;
    BOOL bIsLoaded = TRUE;

    hModule = GetModuleHandle(_T("Shell32.dll"));
    if (hModule == NULL)
    {
        bIsLoaded = FALSE;
        hModule = LoadLibrary(_T("Shell32.dll"));
        if (hModule == NULL)
        {
            return;
        }
    }

    ShellAbout(hwnd, szApp, szOtherStuff, LoadIcon(hModule, MAKEINTRESOURCE(47)));
    if (!bIsLoaded)
    {
        FreeLibrary(hModule);
    }
}

/* ��ʾ���жԻ��� */
void ShowRunDlg(HWND hwnd, DWORD dwFlags)
{
    HMODULE hModule;
    pfnRunFileDlg RunFileDlg;
    WCHAR szDir[MAX_PATH];
    BOOL bIsLoaded = TRUE;
    WCHAR szTitle[] = L"���У������û���";
    WCHAR szPrompt[] = L"Windows ������������������ƣ�Ϊ������Ӧ�ĳ��򣬳����Ի����û���Ȩ�����С�";

    hModule = GetModuleHandle(_T("Shell32.dll"));
    if (hModule == NULL)
    {
        bIsLoaded = FALSE;
        hModule = LoadLibrary(_T("Shell32.dll"));
        if (hModule == NULL)
        {
            return;
        }
    }
    GetWindowsDirectoryW(szDir, MAX_PATH);

    RunFileDlg = (pfnRunFileDlg)GetProcAddress(hModule, MAKEINTRESOURCEA(61));
    if (RunFileDlg == NULL)
    {
        if (!bIsLoaded)
        {
            FreeLibrary(hModule);
        }
        return;
    }

    if (SelectedRunAsMode == RUNAS_MODE_LIMITED)
    {
        RunFileDlg(hwnd, NULL, NULL, szTitle, szPrompt, dwFlags);
    }
    else
    {
        RunFileDlg(hwnd, NULL, NULL, NULL, NULL, dwFlags);
    }
    /* RunFileDlg(hwnd, LoadIcon(hModule, MAKEINTRESOURCE(160)), szDir, szTitle, szPrompt, dwFlags); */
    if (!bIsLoaded)
    {
        FreeLibrary(hModule);
    }
}

/* ���ļ�λ�� */
BOOL OpenFileLocation(HWND hwnd, LPTSTR szFullPath)
{
    int ret;
    TCHAR szCommand[256];

    PathUnquoteSpaces(szFullPath);
    /* PathFileExists����·��ǰ���ܰ������� */
    if (!PathFileExists(szFullPath))
    {
        return FALSE;
    }

    wsprintf(szCommand, _T("/select, \"%s\""), szFullPath);
    ret = (int)ShellExecute(hwnd, _T("Open"), _T("explorer.exe"), szCommand, NULL, SW_SHOWNORMAL);

    if (ret == ERROR_FILE_NOT_FOUND || ret == ERROR_PATH_NOT_FOUND
            || ret == ERROR_BAD_FORMAT || ret == SE_ERR_ACCESSDENIED)
    {
        return FALSE;
    }
    return TRUE;
}

/* ��ʾ�ļ�����ѡ��ҳ */
BOOL ShowFileProperties(HWND hwnd, LPTSTR szFullPath)
{
    SHELLEXECUTEINFO ShExecInfo = {0};

    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_INVOKEIDLIST ;
    ShExecInfo.hwnd = hwnd;
    ShExecInfo.lpVerb = _T("properties");
    ShExecInfo.lpFile = szFullPath;
    ShExecInfo.lpParameters = NULL;
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_SHOW;
    ShExecInfo.hInstApp = NULL;

    PathUnquoteSpaces(szFullPath);
    if (!PathFileExists(szFullPath))
    {
        return FALSE;
    }
    return ShellExecuteEx(&ShExecInfo);
}
