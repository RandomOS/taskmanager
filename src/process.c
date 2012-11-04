#include "process.h"
#include "auxfunction.h"

/* 提升进程权限 */
BOOL EnableDebugPrivilege(void)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LPCTSTR szPrivName = _T("SeDebugPrivilege");

    //得到进程的令牌句柄
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        ODS(_T("Get current process token failed!"));
        return FALSE;
    }

    //查询进程的权限
    if (!LookupPrivilegeValue(NULL, szPrivName, &tp.Privileges[0].Luid))
    {
        ODS(_T("Look up privilege value failed!"));
        CloseHandle(hToken);
        return FALSE;
    }
    else
    {
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    }

    //调整进程访问的权限
    AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
    if (GetLastError() != ERROR_SUCCESS)
    {
        ODS(_T("Adjust process privilege failed!"));
        CloseHandle(hToken);
        return FALSE;
    }

    CloseHandle(hToken);
    return TRUE;
}

/* 结束进程方案1 TerminateProcess */
BOOL KillProcessMethod_1(DWORD dwProcessID)
{
    BOOL bRet;
    HANDLE hProcess;

    hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    bRet = TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
    return bRet;
}

/* 结束进程方案2 TerminateJobObject */
BOOL KillProcessMethod_2(DWORD dwProcessID)
{
    BOOL bRet;
    HANDLE hProcess, hJob;

    hProcess = OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE, FALSE, dwProcessID);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    hJob = CreateJobObject(NULL, _T("TestJob"));

    if (!AssignProcessToJobObject(hJob, hProcess))
    {
        return FALSE;
    }

    bRet = TerminateJobObject(hJob, 0);
    CloseHandle(hProcess);
    CloseHandle(hJob);
    return bRet;
}

/* 进程是否被挂起 */
BOOL IsProcessSuspend(DWORD dwProcessID)
{
    ULONG i;
    BOOL bRet = FALSE;
    DWORD cbBuffer = 0x2800;
    PVOID pBuffer;
    NTSTATUS Status;

    PSYSTEM_PROCESS_INFORMATION pInfo;
    _NtQuerySystemInformation NtQuerySystemInformation;

    NtQuerySystemInformation = (_NtQuerySystemInformation)GetProcAddress(
                                   GetModuleHandle(_T("ntdll.dll")), "NtQuerySystemInformation");

    if (NtQuerySystemInformation == NULL)
    {
        ODS(_T("NtQuerySystemInformation address error!"));
        return FALSE;
    }

    do
    {
        pBuffer = malloc(cbBuffer);
        if (pBuffer == NULL)
        {
            ODS(_T("Memory allocated error!"));
            return FALSE;
        }

        Status = NtQuerySystemInformation(SystemProcessInformation, pBuffer, cbBuffer, NULL);

        if (Status == STATUS_INFO_LENGTH_MISMATCH)
        {
            free(pBuffer);
            cbBuffer *= 2;
        }
    }
    while (Status == STATUS_INFO_LENGTH_MISMATCH);

    if (!NT_SUCCESS(Status))
    {
        ODS(_T("NtQuerySystemInformation failed!"));
        free(pBuffer);
        return FALSE;
    }

    pInfo = (PSYSTEM_PROCESS_INFORMATION)pBuffer;

    while (1)
    {
        if (pInfo->ProcessId == dwProcessID)
        {
            bRet = pInfo->ThreadCount != 0;
            for (i = 0; i < pInfo->ThreadCount; i++)
            {
                if (pInfo->Threads[i].State != StateWait || pInfo->Threads[i].WaitReason != Suspended)
                {
                    bRet = FALSE;
                    break;
                }
            }
            break;
        }

        if (pInfo->NextEntryDelta == 0)
        {
            break;
        }
        else
        {
            pInfo = (PSYSTEM_PROCESS_INFORMATION)(((PUCHAR)pInfo) + pInfo->NextEntryDelta);
        }
    }

    free(pBuffer);
    return bRet;
}

/* 挂起进程 */
BOOL SuspendProcess(DWORD dwProcessID)
{
    _NtSuspendProcess NtSuspendProcess;
    HANDLE hProcess;
    NTSTATUS Status;

    NtSuspendProcess = (_NtSuspendProcess)GetProcAddress(
                           GetModuleHandle(_T("ntdll.dll")), "NtSuspendProcess");

    if (NtSuspendProcess == NULL)
    {
        ODS(_T("NtSuspendProcess address error!"));
        return FALSE;
    }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    Status = NtSuspendProcess(hProcess);

    if (!NT_SUCCESS(Status))
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    CloseHandle(hProcess);
    return TRUE;
}

/* 恢复进程 */
BOOL ResumeProcess(DWORD dwProcessID)
{
    _NtResumeProcess NtResumeProcess;
    HANDLE hProcess;
    NTSTATUS Status;

    NtResumeProcess = (_NtResumeProcess)GetProcAddress(
                          GetModuleHandle(_T("ntdll.dll")), "NtResumeProcess");

    if (NtResumeProcess == NULL)
    {
        ODS(_T("NtResumeProcess address error!"));
        return FALSE;
    }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    Status = NtResumeProcess(hProcess);

    if (!NT_SUCCESS(Status))
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    CloseHandle(hProcess);
    return TRUE;
}

/* 通过进程PID获取进程路径 */
BOOL GetProcessPathByPID(DWORD dwProcessID, LPTSTR szFullPath, DWORD nSize)
{
    HANDLE hProcess;

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    if (!GetModuleFileNameEx(hProcess, NULL, szFullPath, nSize))
    {
        CloseHandle(hProcess);
        return FALSE;
    }
    BeautyPath(szFullPath);
    CloseHandle(hProcess);
    return TRUE;
}

#if 0
/* 通过进程PID获取进程路径2 */
BOOL GetProcessPathByPID2(DWORD dwProcessID, LPTSTR szFullPath, DWORD nSize)
{
    HANDLE hModuleSnap;
    MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };

    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    Module32First(hModuleSnap, &me32);
    CloseHandle(hModuleSnap);

    lstrcpyn(szFullPath, me32.szExePath, nSize);
    szFullPath[lstrlen(me32.szExePath)] = _T('\0');
    BeautyPath(szFullPath);
    return TRUE;
}

/* XP Or Above 返回DOS device path */
BOOL GetProcessPathByPID3(DWORD dwProcessID, LPTSTR szFullPath, DWORD nSize)
{
    HANDLE hProcess;

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    if (!GetProcessImageFileName(hProcess, szFullPath, nSize))
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    DosDevicePathToWin32Path(szFullPath);
    CloseHandle(hProcess);
    return TRUE;
}

/* Vista Or Above */
BOOL GetProcessPathByPID4(DWORD dwProcessID, LPTSTR szFullPath, DWORD nSize)
{
    HANDLE hProcess;
    _QueryFullProcessImageName QueryFullProcessImageName;

#ifdef UNICODE
    QueryFullProcessImageName = (_QueryFullProcessImageName)GetProcAddress(
                                    GetModuleHandle(_T("kernel32.dll")), "QueryFullProcessImageNameW");
#else
    QueryFullProcessImageName = (_QueryFullProcessImageName)GetProcAddress(
                                    GetModuleHandle(_T("kernel32.dll")), "QueryFullProcessImageNameA");
#endif

    if (QueryFullProcessImageName == NULL)
    {
        ODS(_T("QueryFullProcessImageName address error!"));
        return FALSE;
    }

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    if (!QueryFullProcessImageName(hProcess, 0, szFullPath, &nSize))
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    CloseHandle(hProcess);
    return TRUE;
}

/* Vista Or Above 在基本用户的权限下也可 返回DOS device path */
BOOL GetProcessPathByPID5(DWORD dwProcessID, LPTSTR szFullPath, DWORD nSize)
{
    NTSTATUS Status;
    PVOID pBuffer;
    SYSTEM_PROCESS_ID_INFORMATION info;
    _NtQuerySystemInformation NtQuerySystemInformation;

    NtQuerySystemInformation = (_NtQuerySystemInformation)GetProcAddress(
                                   GetModuleHandle(_T("ntdll.dll")), "NtQuerySystemInformation");

    if (NtQuerySystemInformation == NULL)
    {
        ODS(_T("NtQuerySystemInformation address error!"));
        return FALSE;
    }

    pBuffer = malloc(0x100);
    info.ProcessId = (HANDLE)dwProcessID;
    info.ImageName.Length = 0;
    info.ImageName.MaximumLength = (USHORT)0x100;
    info.ImageName.Buffer = pBuffer;

    Status = NtQuerySystemInformation(SystemProcessIdInformation, &info, sizeof(info), NULL);

    if (Status == STATUS_INFO_LENGTH_MISMATCH)
    {
        free(pBuffer);
        pBuffer = malloc(info.ImageName.MaximumLength);
        info.ImageName.Buffer = pBuffer;
        Status = NtQuerySystemInformation(SystemProcessIdInformation, &info, sizeof(info), NULL);
    }

    if (!NT_SUCCESS(Status))
    {
        ODS(_T("NtQuerySystemInformation failed!"));
        free(pBuffer);
        return FALSE;
    }

#ifdef UNICODE
    lstrcpynW(szFullPath, info.ImageName.Buffer, nSize);
#else
    WideCharToMultiByte(CP_ACP, 0, info.ImageName.Buffer, -1, szFullPath, nSize, NULL, NULL);
#endif

    free(pBuffer);
    DosDevicePathToWin32Path(szFullPath);
    return TRUE;
}
#endif

#if 0
BOOL GetProcessMemInfoByPID(DWORD dwProcessID, LPDWORD lpdwWorkingSet)
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
    (*lpdwWorkingSet) = pmc.WorkingSetSize;

    CloseHandle(hProcess);
    return TRUE;
}
#endif

/* 通过进程PID获取进程命令行参数 */
BOOL GetProcessCommandLineByPID(DWORD dwProcessID, LPTSTR szCommandLine, DWORD nSize)
{
    HANDLE hProcess;
    NTSTATUS Status;
    PVOID pebAddress;
    PVOID rtlUserProcParamsAddress;
    UNICODE_STRING commandLine;
    WCHAR *commandLineContents;
    PROCESS_BASIC_INFORMATION pbi;
    _NtQueryInformationProcess NtQueryInformationProcess;

    NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(
                                    GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess");

    if (NtQueryInformationProcess == NULL)
    {
        ODS(_T("NtQueryInformationProcess address error!"));
        return FALSE;
    }

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    Status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);

    if (!NT_SUCCESS(Status))
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    pebAddress = pbi.PebBaseAddress;

    if (!ReadProcessMemory(hProcess, (PCHAR)pebAddress + 0x10,
                           &rtlUserProcParamsAddress, sizeof(PVOID), NULL))
    {
        ODS(_T("Could not read the address of ProcessParameters!"));
        CloseHandle(hProcess);
        return FALSE;
    }

    /* read the CommandLine UNICODE_STRING structure */
    if (!ReadProcessMemory(hProcess, (PCHAR)rtlUserProcParamsAddress + 0x40,
                           &commandLine, sizeof(commandLine), NULL))
    {
        ODS(_T("Could not read CommandLine!"));
        CloseHandle(hProcess);
        return FALSE;
    }

    /* allocate memory to hold the command line */
    commandLineContents = (WCHAR *)malloc(commandLine.Length + sizeof(WCHAR));

    /* read the command line */
    if (!ReadProcessMemory(hProcess, commandLine.Buffer,
                           commandLineContents, commandLine.Length + sizeof(WCHAR), NULL))
    {
        ODS(_T("Could not read the command line string!"));
        CloseHandle(hProcess);
        return FALSE;
    }

#ifdef UNICODE
    lstrcpynW(szCommandLine, commandLineContents, nSize);
#else
    WideCharToMultiByte(CP_ACP, 0, commandLineContents, -1, szCommandLine, nSize, NULL, NULL);
#endif

    free(commandLineContents);
    CloseHandle(hProcess);
    return TRUE;
}

/* 释放进程内存 */
BOOL ReduceProcessWorkingSet(DWORD dwProcessID)
{
    BOOL bRet;
    HANDLE hProcess;

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA, FALSE, dwProcessID);
    if (hProcess == NULL)
    {
        return FALSE;
    }

    bRet = EmptyWorkingSet(hProcess);
    CloseHandle(hProcess);
    return bRet;
}
