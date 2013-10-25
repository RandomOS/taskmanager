#include "listview.h"
#include "statusbar.h"
#include "process.h"
#include "auxfunction.h"

void ListViewColumnClick(LPNMHDR pNMHdr)
{
    int i, count;
    HWND hwndList, hwndHeader;
    LPNMLISTVIEW pNMLV;

    hwndList = pNMHdr->hwndFrom;
    hwndHeader = ListView_GetHeader(hwndList);
    pNMLV = (NMLISTVIEW *)pNMHdr;

    count = Header_GetItemCount(hwndHeader);
    for (i = 0; i < count; i++)
    {
        HDITEM item;

        item.mask = HDI_FORMAT;
        Header_GetItem(hwndHeader, i, &item);

        if (i == pNMLV->iSubItem)
        {
            if (item.fmt & HDF_SORTDOWN
                    || (!(item.fmt & HDF_SORTUP) && !(item.fmt & HDF_SORTDOWN)))
            {
                //����
                item.fmt &= ~HDF_SORTDOWN;
                item.fmt |= HDF_SORTUP;
            }
            else if (item.fmt & HDF_SORTUP)
            {
                //����
                item.fmt &= ~HDF_SORTUP;
                item.fmt |= HDF_SORTDOWN;
            }
        }
        else
        {
            item.fmt &= ~(HDF_SORTDOWN | HDF_SORTUP);
        }
        Header_SetItem(hwndHeader, i, &item);
    }
    ListView_SortItemsEx(hwndList, CompareFunc, (LPARAM)pNMLV->iSubItem);
}

void ListViewRowRClick(LPNMHDR pNMHdr)
{
    UINT uflag;
    POINT pt;
    HWND hwndList;
    HMENU hPopupMenu;
    LPNMITEMACTIVATE pNMItemActivate;

    hwndList = pNMHdr->hwndFrom;
    pNMItemActivate = (NMITEMACTIVATE *)pNMHdr;

    /* ֻ����ListView�е������һ�����ʾ�˵� */
    if (pNMItemActivate->iItem != -1)
    {
        hPopupMenu = GetSubMenu(g_hmenu, 0);
        EnableMenuItem(hPopupMenu, IDM_SUSPEND, MF_BYCOMMAND | MF_GRAYED);
        EnableMenuItem(hPopupMenu, IDM_RESUME, MF_BYCOMMAND | MF_GRAYED);

        /* �����̹���ı�־ */
        uflag = GetListViewItemParam(hwndList, pNMItemActivate->iItem);
        if (uflag == 0)
        {
            EnableMenuItem(hPopupMenu, IDM_SUSPEND, MF_BYCOMMAND | MF_ENABLED);
        }
        else if (uflag == 1)
        {
            EnableMenuItem(hPopupMenu, IDM_RESUME, MF_BYCOMMAND | MF_ENABLED);
        }
        GetCursorPos(&pt);
        TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, g_hwnd, NULL);
    }
}

LRESULT ListViewDraw(LPNMHDR pNMHdr)
{
    int iRow;
    HWND hwndList;
    LPNMLVCUSTOMDRAW pListDraw;

    hwndList = pNMHdr->hwndFrom;
    pListDraw = (NMLVCUSTOMDRAW *)pNMHdr;

    switch (pListDraw->nmcd.dwDrawStage)
    {
        case CDDS_PREPAINT:
            return CDRF_NOTIFYITEMDRAW;

        case CDDS_ITEMPREPAINT:
            iRow = (int)pListDraw->nmcd.dwItemSpec;
            /*
                if (iRow % 2 == 0)
                {
                    pListDraw->clrTextBk = RGB(202, 221, 250);
                }
            */
            if (GetListViewItemParam(hwndList, iRow) == 1)
            {
                pListDraw->clrTextBk = RGB(119, 119, 119);
            }
            return CDRF_NEWFONT;

        default:
            break;
    }
    return CDRF_DODEFAULT;
}

int CmpPid(const void *a, const void *b)
{
    return ((PLIST_INFO)a)->pid - ((PLIST_INFO)b)->pid;
}

int CmpIndex(const void *a, const void *b)
{
    return ((PLIST_INFO)a)->index - ((PLIST_INFO)b)->index;
}

/* ListView�ıȽϻص����������������������Լ����� */
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, int iSubItem)
{
    int ret = 0;
    HDITEM item;
    HWND hwndHeader;
    TCHAR szBuf1[MAX_PATH], szBuf2[MAX_PATH];

    //hwndList = pNMLV->hdr.hwndFrom;
    ListView_GetItemText(g_hwndListView, lParam1, iSubItem, szBuf1, MAX_PATH);
    ListView_GetItemText(g_hwndListView, lParam2, iSubItem, szBuf2, MAX_PATH);

    hwndHeader = ListView_GetHeader(g_hwndListView);
    item.mask = HDI_FORMAT;
    Header_GetItem(hwndHeader, iSubItem, &item);

    switch (iSubItem)
    {
        case 0: // ��������
        case 2: // ·��
        case 3: // ����������
            if (item.fmt & HDF_SORTUP) //����
            {
                ret = lstrcmpi(szBuf1, szBuf2);
            }
            else if (item.fmt & HDF_SORTDOWN) //����
            {
                ret = lstrcmpi(szBuf2, szBuf1);
            }
            break;

        case 1: // PID
            if (item.fmt & HDF_SORTUP)
            {
                ret = _ttoi(szBuf1) - _ttoi(szBuf2);
            }
            else if (item.fmt & HDF_SORTDOWN)
            {
                ret = _ttoi(szBuf2) - _ttoi(szBuf1);
            }
            break;

        default:
            break;
    }
    return ret;
}

void SortListView(void)
{
    int i, count;
    HWND hwndHeader;
    hwndHeader = ListView_GetHeader(g_hwndListView);

    count = Header_GetItemCount(hwndHeader);
    for (i = 0; i < count; i++)
    {
        HDITEM item;
        item.mask = HDI_FORMAT;
        Header_GetItem(hwndHeader, i, &item);

        if ((item.fmt & HDF_SORTUP) || (item.fmt & HDF_SORTDOWN))
        {
            ListView_SortItemsEx(g_hwndListView, CompareFunc, (LPARAM)i);
            break;
        }
    }
}

/* ��ʼ��ListView */
BOOL InitListViewColumns(void)
{
    LVCOLUMN lvc = {0};
    LPTSTR ColNames[] = {_T("��������"), _T("PID"), _T("·��")};
    int ColWidth[] = {100, 60, 250};
    int iColNums = (sizeof(ColNames) / sizeof(ColNames[0]));
    int iCol = 0;

    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    for (iCol = 0; iCol < iColNums; iCol++)
    {
        lvc.iSubItem = iCol;
        lvc.pszText = ColNames[iCol];      //�е�����
        lvc.cx = ColWidth[iCol];           //�еĿ��
        lvc.fmt = LVCFMT_LEFT;
        if (ListView_InsertColumn(g_hwndListView, iCol, &lvc) == -1)
        {
            return FALSE;
        }
    }

    /* �����϶������� | ѡ��һ���� | �����ͼ */
    ListView_SetExtendedListViewStyle(g_hwndListView,
                                      LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER /* | LVS_EX_GRIDLINES*/);
    return TRUE;
}

/* ��ʼ��ListView���� */
BOOL InitProcessList(void)
{
    int iIdx, iItem = 0;
    LVITEM lvitem;
    TCHAR szPID[20];
    TCHAR szFullPath[MAX_PATH];
    HANDLE hProcessSnap;
    SHFILEINFO fileInfo;
    PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };

    //ListView_DeleteAllItems(g_hwndListView);

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    ZeroMemory(&lvitem, sizeof(lvitem));
    lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
    lvitem.iImage = 0;
    lvitem.state = 0;
    lvitem.stateMask = 0;

    if (Process32First(hProcessSnap, &pe32))
    {
        do
        {
            if (pe32.th32ProcessID < 10)
            {
                continue;
            }
            //���LVITEM
            lvitem.iItem = iItem;
            lvitem.pszText = (LPTSTR)pe32.szExeFile;
            //��lParam����־�����Ƿ񱻹���
            lvitem.lParam = IsProcessSuspend(pe32.th32ProcessID);

            //������
            if (ListView_InsertItem(g_hwndListView, &lvitem) == -1)
            {
                return FALSE;
            }

            //��ȡ���̵�Pid
            wsprintf(szPID, _T("%u"), pe32.th32ProcessID);

            //���õڶ��е��ı�
            ListView_SetItemText(g_hwndListView, iItem, lvProcessId, (LPTSTR)szPID);

            //��ȡ����·��, ���õ����е��ı�
            if (GetProcessPathByPID(pe32.th32ProcessID, szFullPath, MAX_PATH))
            {
                if (SHGetFileInfo(szFullPath, 0, &fileInfo, sizeof(fileInfo), SHGFI_ICON | SHGFI_SMALLICON))
                {
                    iIdx = ImageList_AddIcon(g_hImageList, fileInfo.hIcon);
                    DestroyIcon(fileInfo.hIcon);
                    SetListViewItemImageIndex(g_hwndListView, iItem, iIdx);
                }
                else
                {
                    iIdx = ImageList_AddIcon(g_hImageList, LoadIcon(NULL, IDI_APPLICATION));
                    SetListViewItemImageIndex(g_hwndListView, iItem, iIdx);
                }
                ListView_SetItemText(g_hwndListView, iItem, lvProcessPath, (LPTSTR)szFullPath);
            }
            else
            {
                iIdx = ImageList_AddIcon(g_hImageList, LoadIcon(NULL, IDI_APPLICATION));
                SetListViewItemImageIndex(g_hwndListView, iItem, iIdx);
                ListView_SetItemText(g_hwndListView, iItem, lvProcessPath, _T("�ܾ�����"));
            }
            iItem++;
        }
        while (Process32Next(hProcessSnap, &pe32));

        /* ����״̬�� */
        UpdateStatusBar();
    }
    CloseHandle(hProcessSnap);
    /* Ĭ��ѡ�е�һ��
     * ListView_SetSelectionMark(g_hwndListView, 0);
     */
    //Ĭ��ѡ�в�������һ��
    ListView_SetItemState(g_hwndListView, 0, LVIS_FOCUSED | LVIS_SELECTED, -1);
    return TRUE;
}

/* ����ListView */
BOOL UpdateListView(void)
{
    int iIdx, iItem, nItems;
    LVITEM lvitem;
    TCHAR szBuffer[1024];
    HANDLE hProcessSnap;
    SHFILEINFO fileInfo;
    PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
    PLIST_INFO pListInfo, pSearch;
    LIST_INFO tListInfo;

    if (!IsWindow(g_hwndListView))
    {
        return FALSE;
    }

    iItem = nItems = ListView_GetItemCount(g_hwndListView);

    pListInfo = (PLIST_INFO)malloc(nItems * sizeof(LIST_INFO));

    for (iIdx = 0; iIdx < nItems; iIdx++)
    {
        pListInfo[iIdx].pid = GetPIDFromItemIndex(iIdx);
        pListInfo[iIdx].index = iIdx;
        pListInfo[iIdx].flag = 0;
    }

    qsort(pListInfo, nItems, sizeof(LIST_INFO), CmpPid);

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        free(pListInfo);
        return FALSE;
    }

    ZeroMemory(&lvitem, sizeof(lvitem));
    lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
    lvitem.iImage = 0;
    lvitem.state = 0;
    lvitem.stateMask = 0;
    lvitem.lParam = 0;

    if (Process32First(hProcessSnap, &pe32))
    {
        do
        {
            if (pe32.th32ProcessID < 10)
            {
                continue;
            }

            tListInfo.pid = pe32.th32ProcessID;
            pSearch = (PLIST_INFO)bsearch(&tListInfo, pListInfo, nItems, sizeof(LIST_INFO), CmpPid);
            if (pSearch != NULL)
            {
                pSearch->flag = 1;
            }
            else
            {
                lvitem.iItem = iItem;
                lvitem.pszText = (LPTSTR)pe32.szExeFile;

                if (ListView_InsertItem(g_hwndListView, &lvitem) == -1)
                {
                    return FALSE;
                }

                wsprintf(szBuffer, _T("%u"), pe32.th32ProcessID);
                ListView_SetItemText(g_hwndListView, iItem, lvProcessId, (LPTSTR)szBuffer);
                //��ȡ����·��, ���õ����е��ı�
                if (GetProcessPathByPID(pe32.th32ProcessID, szBuffer, sizeof(szBuffer) / sizeof(TCHAR)))
                {
                    if (SHGetFileInfo(szBuffer, 0, &fileInfo, sizeof(fileInfo), SHGFI_ICON | SHGFI_SMALLICON))
                    {
                        iIdx = ImageList_AddIcon(g_hImageList, fileInfo.hIcon);
                        DestroyIcon(fileInfo.hIcon);
                        SetListViewItemImageIndex(g_hwndListView, iItem, iIdx);
                    }
                    else
                    {
                        iIdx = ImageList_AddIcon(g_hImageList, LoadIcon(NULL, IDI_APPLICATION));
                        SetListViewItemImageIndex(g_hwndListView, iItem, iIdx);
                    }
                    ListView_SetItemText(g_hwndListView, iItem, lvProcessPath, (LPTSTR)szBuffer);
                }
                else
                {
                    iIdx = ImageList_AddIcon(g_hImageList, LoadIcon(NULL, IDI_APPLICATION));
                    SetListViewItemImageIndex(g_hwndListView, iItem, iIdx);
                    ListView_SetItemText(g_hwndListView, iItem, lvProcessPath, _T("�ܾ�����"));
                }
                iItem++;
            }
        }
        while (Process32Next(hProcessSnap, &pe32));
    }
    CloseHandle(hProcessSnap);

    qsort(pListInfo, nItems, sizeof(LIST_INFO), CmpIndex);

    for (iIdx = nItems - 1; iIdx >= 0; iIdx--)
    {
        if (pListInfo[iIdx].flag == 0)
        {
            /* ɾ��һ��ʱ����ͬ��ListView��ImageList������ */
            ResyncImageList(iIdx);
            ListView_DeleteItem(g_hwndListView, iIdx);
        }
    }
    free(pListInfo);

    //���ý��̹����״̬
    nItems = ListView_GetItemCount(g_hwndListView);
    for (iItem = 0; iItem < nItems; iItem++)
    {
        SetListViewItemParam(g_hwndListView, iItem, IsProcessSuspend(GetPIDFromItemIndex(iItem)));
    }

    //��ȡ����������, ���õ����е��ı�
    if (bShowCmdLine)
    {
        for (iItem = 0; iItem < nItems; iItem++)
        {
            ListView_GetItemText(g_hwndListView, iItem, lvProcessCmdLine, szBuffer, sizeof(szBuffer) / sizeof(TCHAR));
            if (szBuffer[0] != _T('\0'))
            {
                continue;
            }
            if (GetProcessCommandLineByPID(GetPIDFromItemIndex(iItem), szBuffer, sizeof(szBuffer) / sizeof(TCHAR)))
            {
                RemoveQuotes(szBuffer);
                ListView_SetItemText(g_hwndListView, iItem, lvProcessCmdLine, (LPTSTR)szBuffer);
            }
            else
            {
                ListView_SetItemText(g_hwndListView, iItem, lvProcessCmdLine, _T("�ܾ�����"));
            }
        }
    }

    SortListView();
    UpdateStatusBar();
    SetFocus(g_hwndListView);
    return TRUE;
}

/* ����ѡ�еĽ��� */
void KillSelectProcess(void)
{
    int iIdx, ret;
    HANDLE hProcess;
    TCHAR szPID[56], szExeFile[MAX_PATH], szPrompt[256];

    if (!IsWindow(g_hwndListView))
    {
        return;
    }

    //��ȡ��ǰѡ����е�����
    iIdx = ListView_GetSelectionMark(g_hwndListView);

    ListView_GetItemText(g_hwndListView, iIdx, lvProcessName, szExeFile, sizeof(szExeFile) / sizeof(TCHAR));
    ListView_GetItemText(g_hwndListView, iIdx, lvProcessId, szPID, sizeof(szPID) / sizeof(TCHAR));

    wsprintf(szPrompt, _T("�Ƿ�Ҫ���� \"%s\"?"), szExeFile);
    ret = MessageBox(g_hwnd, szPrompt, _T("���������"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);

    if (ret == IDCANCEL)
    {
        return;
    }

    // ������ֹ����
    hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, _ttoi(szPID));
    if (hProcess != NULL)
    {
        if (TerminateProcess(hProcess, 0))
        {
            ResyncImageList(iIdx);
            ListView_DeleteItem(g_hwndListView, iIdx);
            UpdateStatusBar();
        }
        else
        {
            MessageBox(g_hwnd, _T("�޷���ɲ�����\n\n�ܾ����ʡ�"), _T("�޷���ֹ����"), MB_OK | MB_ICONSTOP);
        }
        CloseHandle(hProcess);
    }
    else
    {
        MessageBox(g_hwnd, _T("�޷��򿪽��̾����\n\n�ܾ����ʡ�"), _T("�޷���ֹ����"), MB_OK | MB_ICONSTOP);
    }
}

/* ��ȡѡ�����PID */
DWORD GetPIDFromSelectItem(void)
{
    int iIdx;
    TCHAR szPID[20];

    if (!IsWindow(g_hwndListView))
    {
        return 0;
    }

    iIdx = ListView_GetSelectionMark(g_hwndListView);
    ListView_GetItemText(g_hwndListView, iIdx, lvProcessId, szPID, sizeof(szPID) / sizeof(TCHAR));
    return (DWORD)_ttoi(szPID);
}

/* ͨ��Item��Index��ȡPID */
DWORD GetPIDFromItemIndex(int iIdx)
{
    int nItems;
    TCHAR szPID[20];

    if (!IsWindow(g_hwndListView))
    {
        return 0;
    }

    nItems = ListView_GetItemCount(g_hwndListView);
    if (iIdx >= nItems)
    {
        return 0;
    }

    ListView_GetItemText(g_hwndListView, iIdx, lvProcessId, szPID, sizeof(szPID) / sizeof(TCHAR));
    return (DWORD)_ttoi(szPID);
}

/* �ͷ����н����ڴ� */
void EmptyAllProcessWorkingSet(void)
{
    int iIdx, nItems;
    TCHAR szPID[56];

    if (!IsWindow(g_hwndListView))
    {
        return;
    }

    nItems = ListView_GetItemCount(g_hwndListView);

    for (iIdx = 0; iIdx < nItems; iIdx++)
    {
        ListView_GetItemText(g_hwndListView, iIdx, lvProcessId, szPID, sizeof(szPID) / sizeof(TCHAR));
        ReduceProcessWorkingSet(_ttoi(szPID));
    }
    UpdateStatusBar();
}

/* ����ͬ��ImageList��ListView */
void ResyncImageList(int iItem)
{
    int iIdx, iImage, nItems, tmp;

    iImage = GetListViewItemImageIndex(g_hwndListView, iItem);
    if (iImage < 0)
    {
        return;
    }

    nItems = ListView_GetItemCount(g_hwndListView);
    ImageList_Remove(g_hImageList, iImage);

    for (iIdx = 0; iIdx < nItems; iIdx++)
    {
        tmp = GetListViewItemImageIndex(g_hwndListView, iIdx);
        if (tmp > iImage)
        {
            SetListViewItemImageIndex(g_hwndListView, iIdx, tmp - 1);
        }
    }
}

/* ��ȡListViewItem��ͼ������ */
int GetListViewItemImageIndex(HWND hwndList, int iItem)
{
    LVITEM lvitem;
    ZeroMemory(&lvitem, sizeof(LVITEM));
    lvitem.mask = LVIF_IMAGE;
    lvitem.iItem = iItem;
    ListView_GetItem(hwndList, &lvitem);
    return lvitem.iImage;
}

/* ����ListViewItem��ͼ������ */
void SetListViewItemImageIndex(HWND hwndList, int iItem, int iIdx)
{
    LVITEM lvitem;
    ZeroMemory(&lvitem, sizeof(LVITEM));
    lvitem.mask = LVIF_IMAGE;
    lvitem.iItem = iItem;
    lvitem.iImage = iIdx;
    ListView_SetItem(hwndList, &lvitem);
}

/* ��ȡListViewItem�ı�־(�����Ƿ����)*/
UINT GetListViewItemParam(HWND hwndList, int iItem)
{
    LVITEM lvitem;
    ZeroMemory(&lvitem, sizeof(LVITEM));
    lvitem.mask = LVIF_PARAM;
    lvitem.iItem = iItem;
    ListView_GetItem(hwndList, &lvitem);
    return (UINT)(lvitem.lParam);
}

/* ����ListViewItem�ı�־(�����Ƿ����)*/
void SetListViewItemParam(HWND hwndList, int iItem, LPARAM lParam)
{
    LVITEM lvitem;
    ZeroMemory(&lvitem, sizeof(LVITEM));
    lvitem.mask = LVIF_PARAM;
    lvitem.iItem = iItem;
    lvitem.lParam = lParam;
    ListView_SetItem(hwndList, &lvitem);
}
