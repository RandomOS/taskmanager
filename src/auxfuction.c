#include "auxfunction.h"

/* 移除字符串中的引号 */
void RemoveQuotes(LPTSTR szCommand)
{
    int nGet = 0, nSet = 0;
    while (szCommand[nGet] != _T('\0'))
    {
        if (szCommand[nGet] == _T('\"'))
        {
            nGet++;
        }
        else
        {
            szCommand[nSet] = szCommand[nGet];
            nSet++;
            nGet++;
        }
    }
    szCommand[nSet] = _T('\0');
}

/* 处理特殊路径 */
void BeautyPath(LPTSTR szFullPath)
{
    UINT uLen;
    TCHAR szTemp[MAX_PATH];

    uLen = lstrlen(szFullPath);
    if (uLen <= 4)
    {
        return;
    }

    /* lstrcpyn函数会自动在字符串末尾补0 */
    lstrcpyn(szTemp, szFullPath, 5);
    if (lstrcmp(szTemp, _T("\\??\\")) == 0)
    {
        lstrcpy(szFullPath, szFullPath + 4);
    }

    if (uLen <= 12)
    {
        return;
    }

    lstrcpyn(szTemp, szFullPath, 13);
    if (lstrcmpi(szTemp, _T("\\systemroot\\")) == 0)
    {
        GetWindowsDirectory(szTemp, MAX_PATH);
        lstrcat(szTemp, szFullPath + 11);
        lstrcpy(szFullPath, szTemp);
    }
}

/* 将DosDevicePath转换为Win32Path */
void DosDevicePathToWin32Path(LPTSTR szFullPath)
{
    TCHAR szDriveStrings[MAX_PATH];
    TCHAR szDosDevice[MAX_PATH];
    TCHAR szTemp[3] = {0};
    TCHAR *pszDrive;
    UINT uDosDeviceLen;
    BOOL bFound = FALSE;

    ZeroMemory(szDriveStrings, MAX_PATH);
    GetLogicalDriveStrings(MAX_PATH - 1, szDriveStrings);
    pszDrive = (TCHAR *)szDriveStrings;

    do
    {
        CopyMemory(szTemp, pszDrive, 2 * sizeof(TCHAR));
        if (!QueryDosDevice(szTemp, szDosDevice, MAX_PATH))
        {
            dprintf(_T("QueryDosDevice error: %d"), GetLastError());
            return;
        }

        uDosDeviceLen = lstrlen(szDosDevice);
        bFound = (_tcsncmp(szFullPath, szDosDevice, uDosDeviceLen) == 0);
        if (bFound)
        {
            wsprintf(szFullPath, _T("%s%s"), szTemp, szFullPath + uDosDeviceLen);
            return;
        }
        pszDrive += (lstrlen(pszDrive) + sizeof(TCHAR));
    }
    while (*pszDrive != _T('\0'));
}
