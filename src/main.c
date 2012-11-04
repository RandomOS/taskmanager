/******************************
 * 2011-12-04
******************************/
#include "main.h"
#include "process.h"
#include "listview.h"
#include "statusbar.h"
#include "oncommand.h"

typedef struct _NMRUNFILEDLGW
{
    NMHDR hdr;
    LPCWSTR lpszFile;
    LPCWSTR lpszDirectory;
    UINT nShow;
} NMRUNFILEDLGW, *LPNMRUNFILEDLGW, *PNMRUNFILEDLGW;

typedef NMRUNFILEDLGW NMRUNFILEDLG;
typedef PNMRUNFILEDLGW PNMRUNFILEDLG;
typedef LPNMRUNFILEDLGW LPNMRUNFILEDLG;

HWND g_hwnd = NULL;
HINSTANCE g_hInst = NULL;
HMENU g_hmenu = NULL;
HWND g_hwndListView = NULL;
HWND g_hwndStatusBar = NULL;
HIMAGELIST g_hImageList = NULL;
BOOL IsTimerOn = FALSE;
BOOL bShowCmdLine = FALSE;
int SelectedRunAsMode = 0;

#ifdef _MSC_VER /* VC */
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
#else /* Mingw */
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
#endif
{
    HWND hwnd;
    HACCEL hAccel;
    HANDLE hObject;
    INITCOMMONCONTROLSEX icex;
    MSG msg;

    hObject = CreateMutex(NULL, FALSE, _T("My_Task_Manager"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        hwnd = FindWindow(_T("#32770"), _T("任务管理器"));
        if (hwnd != NULL)
        {
            SetForegroundWindow(hwnd);
        }
        CloseHandle(hObject);
        return 0;
    }

    g_hInst = hInstance;
    icex.dwSize = sizeof(icex);
    icex.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icex);

    hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_Proc);
    hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL));

    while (GetMessage(&msg, NULL, 0, 0))
    {
        /* 这里处理WM_KEYDOWN消息，并转化为WM_COMMAND消息 */
        if (!TranslateAccelerator(hwnd, hAccel, &msg))
        {
            /* 这里也处理WM_KEYDOWN消息，VK_TAB 转化为控件选择功能(按Tab键)
             * IsDialogMessage的作用也就是转化下Tab什么的 */
            if (!IsWindow(hwnd) || !IsDialogMessage(hwnd, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    return 0;
}

BOOL CALLBACK Main_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
            /* BEGIN MESSAGE CRACK */
            HANDLE_MSG(hwnd, WM_INITDIALOG, Main_OnInitDialog);
            HANDLE_MSG(hwnd, WM_COMMAND, Main_OnCommand);
            HANDLE_MSG(hwnd, WM_SIZE, Main_OnSize);
            HANDLE_MSG(hwnd, WM_NOTIFY, Main_OnNotify);
            //HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGING, Main_OnWindowPosChanging);
            HANDLE_MSG(hwnd, WM_GETMINMAXINFO, Main_OnGetMinMaxInfo);
            HANDLE_MSG(hwnd, WM_CLOSE, Main_OnClose);
            HANDLE_MSG(hwnd, WM_DESTROY, Main_OnDestroy);
            /* END MESSAGE CRACK */
    }

    switch (uMsg)
    {
        case WM_TIMER:
            if (wParam == ID_TIMER)
            {
                UpdateListView();
            }
            break;
    }
    return FALSE;
}

/*******************************************************************************
*  Main_OnInitDialog
*/
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    HICON hIcon;
    hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICONAPP));
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    g_hwnd = hwnd;   //这里才是最早获得窗口句柄的位置
    g_hmenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU_CONTEXT));
    g_hwndStatusBar = GetDlgItem(hwnd, IDC_SBR);
    g_hwndListView  = GetDlgItem(hwnd, IDC_LSV_PROCESS);
    g_hImageList = ImageList_Create(16 , 16, ILC_COLOR32 | ILC_MASK, 40, 10);

    /* 将ImageList与ListView关联 */
    ListView_SetImageList(g_hwndListView, g_hImageList, LVSIL_SMALL);
    EnableDebugPrivilege();

    /* 初始化状态栏 */
    InitStatusBar();

    /* 初始化列表视图 */
    InitListViewColumns();
    InitProcessList();

    /* 设置进程列表自动更新定时器 */
    SetTimer(hwnd, ID_TIMER, 1000, NULL);
    IsTimerOn = TRUE;

    SetWindowTheme(g_hwndListView, L"explorer", NULL);
    EnableThemeDialogTexture(hwnd, ETDT_ENABLE);
    return TRUE;
}

/*******************************************************************************
*  Main_OnCommand
*/
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
            ON_COMMAND(IDC_BTN_REFRESH, OnRefresh);
            ON_COMMAND(IDM_KILL, OnKill);
            ON_COMMAND(IDC_BTN_KILL, OnKill);
            ON_COMMAND(IDM_RUN, OnRun);
            ON_COMMAND(IDM_RUNASLIMIT, OnRunAsLimit);
            ON_COMMAND(IDM_EXIT, OnExit);
            ON_COMMAND(IDM_TOPMOST, OnTopMost);
            ON_COMMAND(IDM_AUTOREFRESH, OnAutoRefresh);
            ON_COMMAND(IDM_SHOWCMDLINE, OnShowCmdLine);
            ON_COMMAND(IDM_SYSINFO, OnSysInfo);
            ON_COMMAND(IDM_EMPTYALL, OnEmptyAll);
            ON_COMMAND(IDM_ABOUT, OnAbout);
            ON_COMMAND(IDM_OPENLOCATION, OnOpenLocation);
            ON_COMMAND(IDM_EMPTYMEM, OnEmptyMem);
            ON_COMMAND(IDM_SUSPEND, OnSuspend);
            ON_COMMAND(IDM_RESUME, OnResume);
            ON_COMMAND(IDM_COPYCMDLINE, OnCopyCmdLine);
            ON_COMMAND(IDM_PROPERTIES, OnProperties);
    }
}

/*******************************************************************************
*  Main_OnSize
*/
void Main_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    if (state != SIZE_MINIMIZED)
    {
        MoveWindow(GetDlgItem(hwnd, IDC_LSV_PROCESS),
                   20,                   //left
                   16,                   //top
                   cx - 20 * 2,          //cx
                   cy - 83,              //cy
                   TRUE);

        MoveWindow(GetDlgItem(hwnd, IDC_SBR),
                   0,
                   cy - 22,
                   cx,
                   22,
                   TRUE);

        MoveWindow(GetDlgItem(hwnd, IDC_BTN_KILL),
                   cx - 100,
                   cy - 58,
                   81,
                   23,
                   TRUE);

        MoveWindow(GetDlgItem(hwnd, IDC_BTN_REFRESH),
                   cx - 200,
                   cy - 58,
                   81,
                   23,
                   TRUE);

        InvalidateRect(GetDlgItem(hwnd, IDC_BTN_KILL), NULL, TRUE);
        InvalidateRect(GetDlgItem(hwnd, IDC_BTN_REFRESH), NULL, TRUE);
    }
}

/*******************************************************************************
*  Main_OnNotify
*/
LRESULT Main_OnNotify(HWND hwnd, int idCtrl, LPNMHDR pNMHdr)
{
    switch (pNMHdr->code)
    {
        case LVN_COLUMNCLICK:
            if (idCtrl == IDC_LSV_PROCESS)
            {
                ListViewColumnClick(pNMHdr);
            }
            break;

        case NM_RCLICK:
            if (idCtrl == IDC_LSV_PROCESS)
            {
                ListViewRowRClick(pNMHdr);
            }
            break;

        case NM_CUSTOMDRAW:
            if (idCtrl == IDC_LSV_PROCESS)
            {
                SetWindowLong(hwnd, DWL_MSGRESULT, (LONG)ListViewDraw(pNMHdr));
                return TRUE;
            }
            break;

        case RFN_VALIDATE:
            if (SelectedRunAsMode == RUNAS_MODE_LIMITED)
            {
                LONG result = RF_RETRY;
                LPNMRUNFILEDLG runFileDlg = (LPNMRUNFILEDLG)pNMHdr;
                DWORD hSaferLevel = SAFER_LEVELID_NORMALUSER;
                SAFER_LEVEL_HANDLE hAuthzLevel = NULL;
#ifndef _MSC_VER
                HMODULE hModule;
                pfnSaferCreateLevel SaferCreateLevel;
                pfnSaferCloseLevel SaferCloseLevel;
                pfnSaferComputeTokenFromLevel SaferComputeTokenFromLevel;
                hModule = GetModuleHandle(_T("advapi32.dll"));
                if (hModule == NULL)
                {
                    hModule = LoadLibrary(_T("advapi32.dll"));
                }
                SaferCreateLevel = (pfnSaferCreateLevel)GetProcAddress(hModule, "SaferCreateLevel");
                SaferCloseLevel = (pfnSaferCloseLevel)GetProcAddress(hModule, "SaferCloseLevel");
                SaferComputeTokenFromLevel = (pfnSaferComputeTokenFromLevel)GetProcAddress(hModule, "SaferComputeTokenFromLevel");
#endif
                if (SaferCreateLevel(SAFER_SCOPEID_USER, hSaferLevel, 0, &hAuthzLevel, NULL))
                {
                    //  Generate the restricted token we will use.
                    HANDLE hToken = NULL;
                    WCHAR szCommandLine[MAX_PATH];

                    if (SaferComputeTokenFromLevel(
                                hAuthzLevel,    // SAFER Level handle
                                NULL,           // NULL is current thread token.
                                &hToken,        // Target token
                                0,              // No flags
                                NULL))          // Reserved
                    {
                        DWORD fStatus = ERROR_SUCCESS;
                        STARTUPINFO si;
                        PROCESS_INFORMATION pi;

                        ZeroMemory(&si, sizeof(STARTUPINFO));
                        si.cb = sizeof(STARTUPINFO);
                        si.lpDesktop = NULL;
                        lstrcpy(szCommandLine, (LPWSTR)runFileDlg->lpszFile);

                        if (CreateProcessAsUserW(
                                    hToken,
                                    NULL,
                                    szCommandLine,
                                    NULL, NULL,
                                    FALSE, CREATE_NEW_CONSOLE,
                                    NULL, NULL,
                                    &si, &pi))
                        {
                            CloseHandle(pi.hProcess);
                            CloseHandle(pi.hThread);
                            result = RF_CANCEL;
                        }
                        else
                        {
                            fStatus = GetLastError();
                            MessageBox(hwnd, _T("无法打开指定的程序。"), _T("提示"), MB_ICONERROR);
                            result = RF_RETRY;
                        }
                    }
                    SaferCloseLevel(hAuthzLevel);
                }
                SetWindowLong(hwnd, DWL_MSGRESULT, result);
                return TRUE;
            }

        default:
            break;
    }
    return FALSE;
}

/*******************************************************************************
*  Main_OnWindowPosChanging(禁止改变窗口大小)
*/
BOOL Main_OnWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos)
{
    lpwpos->flags |= SWP_NOSIZE;
    return FALSE;
}

/*******************************************************************************
*  Main_OnGetMinMaxInfo
*/
void Main_OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
    lpMinMaxInfo->ptMinTrackSize.x = 428;
    lpMinMaxInfo->ptMinTrackSize.y = 504;
}

/*******************************************************************************
*  Main_OnClose
*/
void Main_OnClose(HWND hwnd)
{
    if (IsTimerOn)
    {
        KillTimer(hwnd, ID_TIMER);
    }

    ImageList_Destroy(g_hImageList);
    DestroyWindow(hwnd);
}

/*******************************************************************************
*  Main_OnDestroy
*/
void Main_OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}
