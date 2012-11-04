#include "statusbar.h"
#include "listview.h"

/* 初始化状态栏 */
void InitStatusBar(void)
{
    int ColWidth[] = {100, 220, -1};
    //给状态栏分栏
    SendMessage(g_hwndStatusBar, SB_SETPARTS, 3, (LPARAM)ColWidth);
}

/* 更新状态栏 */
void UpdateStatusBar(void)
{
    DWORD dwProcCount, dwAvailPhys, dwTotalPhys;
    TCHAR szProcCount[20], szMemPercent[20], szMemInfo[100];
    MEMORYSTATUSEX memstatus;

    memstatus.dwLength = sizeof(MEMORYSTATUSEX);

    if (!IsWindow(g_hwndStatusBar))
    {
        return;
    }

    dwProcCount = ListView_GetItemCount(g_hwndListView);
    //获取内存占用率
    GlobalMemoryStatusEx(&memstatus);

    dwAvailPhys = (DWORD)(memstatus.ullAvailPhys / 1024 / 1024);
    dwTotalPhys = (DWORD)(memstatus.ullTotalPhys / 1024 / 1024);
    wsprintf(szProcCount, _T("进程数: %u"), dwProcCount);
    wsprintf(szMemPercent, _T("物理内存: %u%%"), memstatus.dwMemoryLoad);
    wsprintf(szMemInfo, _T("可用: %uMB / %uMB"), dwAvailPhys, dwTotalPhys);

    //设置各栏的文本
    SendMessage(g_hwndStatusBar, SB_SETTEXT, ProcessCount, (LPARAM)szProcCount);
    SendMessage(g_hwndStatusBar, SB_SETTEXT, AvailPhysMemPercent, (LPARAM)szMemPercent);
    SendMessage(g_hwndStatusBar, SB_SETTEXT, AvailPhysMem, (LPARAM)szMemInfo);
}
