#include "statusbar.h"
#include "listview.h"

/* ��ʼ��״̬�� */
void InitStatusBar(void)
{
    int ColWidth[] = {100, 220, -1};
    //��״̬������
    SendMessage(g_hwndStatusBar, SB_SETPARTS, 3, (LPARAM)ColWidth);
}

/* ����״̬�� */
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
    //��ȡ�ڴ�ռ����
    GlobalMemoryStatusEx(&memstatus);

    dwAvailPhys = (DWORD)(memstatus.ullAvailPhys / 1024 / 1024);
    dwTotalPhys = (DWORD)(memstatus.ullTotalPhys / 1024 / 1024);
    wsprintf(szProcCount, _T("������: %u"), dwProcCount);
    wsprintf(szMemPercent, _T("�����ڴ�: %u%%"), memstatus.dwMemoryLoad);
    wsprintf(szMemInfo, _T("����: %uMB / %uMB"), dwAvailPhys, dwTotalPhys);

    //���ø������ı�
    SendMessage(g_hwndStatusBar, SB_SETTEXT, ProcessCount, (LPARAM)szProcCount);
    SendMessage(g_hwndStatusBar, SB_SETTEXT, AvailPhysMemPercent, (LPARAM)szMemPercent);
    SendMessage(g_hwndStatusBar, SB_SETTEXT, AvailPhysMem, (LPARAM)szMemInfo);
}
