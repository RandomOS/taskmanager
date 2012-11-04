#ifndef _LISTVIEW_H_
#define _LISTVIEW_H_
#include "common.h"

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER   0x00010000
#endif

#ifndef HDF_SORTUP
#define HDF_SORTUP      0x0400
#define HDF_SORTDOWN    0x0200
#endif

enum ListColNames
{
    lvProcessName,
    lvProcessId,
    lvProcessPath,
    lvProcessCmdLine
};

typedef struct
{
    DWORD pid;
    UINT index;
    CHAR flag;
} LIST_INFO, *PLIST_INFO;

void ListViewColumnClick(LPNMHDR pNMHdr);
void ListViewRowRClick(LPNMHDR pNMHdr);
LRESULT ListViewDraw(LPNMHDR pNMHdr);
int CmpPid(const void *a, const void *b);
int CmpIndex(const void *a, const void *b);
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, int iSubItem);
void SortListView(void);
BOOL InitListViewColumns(void);
BOOL InitProcessList(void);
BOOL UpdateListView(void);
void KillSelectProcess(void);
DWORD GetPIDFromSelectItem(void);
DWORD GetPIDFromItemIndex(int index);
void EmptyAllProcessWorkingSet(void);
void ResyncImageList(int iItem);
int GetListViewItemImageIndex(HWND hwndList, int iItem);
void SetListViewItemImageIndex(HWND hwndList, int iItem, int iIdx);
UINT GetListViewItemParam(HWND hWndList, int iItem);
void SetListViewItemParam(HWND hWndList, int iItem, LPARAM lParam);
#endif
