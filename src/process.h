#ifndef _PROCESS_H_
#define _PROCESS_H_
#include "common.h"

typedef LONG    NTSTATUS;
typedef ULONG   ACCESS_MASK;
typedef ULONG   KPRIORITY;
typedef DWORD   ACCESS_MASK;

#define NT_SUCCESS(Status)          ((NTSTATUS)(Status) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define STATUS_ACCESS_DENIED        ((NTSTATUS)0xC0000022L)

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef enum _THREAD_STATE
{
    StateInitialized,
    StateReady,
    StateRunning,
    StateStandby,
    StateTerminated,
    StateWait,
    StateTransition,
    StateUnknown
} THREAD_STATE;

typedef enum _KWAIT_REASON
{
    Executive,
    FreePage,
    PageIn,
    PoolAllocation,
    DelayExecution,
    Suspended,
    UserRequest,
    WrExecutive,
    WrFreePage,
    WrPageIn,
    WrPoolAllocation,
    WrDelayExecution,
    WrSuspended,
    WrUserRequest,
    WrEventPair,
    WrQueue,
    WrLpcReceive,
    WrLpcReply,
    WrVirtualMemory,
    WrPageOut,
    WrRendezvous,
    Spare2,
    Spare3,
    Spare4,
    Spare5,
    Spare6,
    WrKernel,
    MaximumWaitReason
} KWAIT_REASON;

typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemBasicInformation,
    SystemProcessorInformation,
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemPathInformation,
    SystemProcessInformation,
    SystemCallCountInformation,
    SystemDeviceInformation,
    SystemProcessorPerformanceInformation,
    SystemFlagsInformation,
    SystemCallTimeInformation,
    SystemModuleInformation,
    SystemLocksInformation,
    SystemStackTraceInformation,
    SystemPagedPoolInformation,
    SystemNonPagedPoolInformation,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPageFileInformation,
    SystemVdmInstemulInformation,
    SystemVdmBopInformation,
    SystemFileCacheInformation,
    SystemPoolTagInformation,
    SystemInterruptInformation,
    SystemDpcBehaviorInformation,
    SystemFullMemoryInformation,
    SystemLoadGdiDriverInformation,
    SystemUnloadGdiDriverInformation,
    SystemTimeAdjustmentInformation,
    SystemSummaryMemoryInformation,
    SystemMirrorMemoryInformation,
    SystemPerformanceTraceInformation,
    SystemObsolete0,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemExtendServiceTableInformation,
    SystemPrioritySeperation,
    SystemVerifierAddDriverInformation,
    SystemVerifierRemoveDriverInformation,
    SystemProcessorIdleInformation,
    SystemLegacyDriverInformation,
    SystemCurrentTimeZoneInformation,
    SystemLookasideInformation,
    SystemTimeSlipNotification,
    SystemSessionCreate,
    SystemSessionDetach,
    SystemSessionInformation,
    SystemRangeStartInformation,
    SystemVerifierInformation,
    SystemVerifierThunkExtend,
    SystemSessionProcessInformation,
    SystemLoadGdiDriverInSystemSpace,
    SystemNumaProcessorMap,
    SystemPrefetcherInformation,
    SystemExtendedProcessInformation,
    SystemRecommendedSharedDataAlignment,
    SystemComPlusPackage,
    SystemNumaAvailableMemory,
    SystemProcessorPowerInformation,
    SystemEmulationBasicInformation,
    SystemEmulationProcessorInformation,
    SystemExtendedHandleInformation,
    SystemLostDelayedWriteInformation,
    SystemBigPoolInformation,
    SystemSessionPoolTagInformation,
    SystemSessionMappedViewInformation,
    SystemHotpatchInformation,
    SystemObjectSecurityMode,
    SystemWatchdogTimerHandler,
    SystemWatchdogTimerInformation,
    SystemLogicalProcessorInformation,
    SystemWow64SharedInformationObsolete,
    SystemRegisterFirmwareTableInformationHandler,
    SystemFirmwareTableInformation,
    SystemModuleInformationEx,
    SystemVerifierTriageInformation,
    SystemSuperfetchInformation,
    SystemMemoryListInformation,
    SystemFileCacheInformationEx,
    SystemThreadPriorityClientIdInformation,
    SystemProcessorIdleCycleTimeInformation,
    SystemVerifierCancellationInformation,
    SystemProcessorPowerInformationEx,
    SystemRefTraceInformation,
    SystemSpecialPoolInformation,
    SystemProcessIdInformation,
    SystemErrorPortInformation,
    SystemBootEnvironmentInformation,
    SystemHypervisorInformation,
    SystemVerifierInformationEx,
    SystemTimeZoneInformation,
    SystemImageFileExecutionOptionsInformation,
    SystemCoverageInformation,
    SystemPrefetchPatchInformation,
    SystemVerifierFaultsInformation,
    SystemSystemPartitionInformation,
    SystemSystemDiskInformation,
    SystemProcessorPerformanceDistribution,
    SystemNumaProximityNodeInformation,
    SystemDynamicTimeZoneInformation,
    SystemCodeIntegrityInformation,
    SystemProcessorMicrocodeUpdateInformation,
    SystemProcessorBrandString,
    SystemVirtualAddressInformation,
    SystemLogicalProcessorAndGroupInformation,
    SystemProcessorCycleTimeInformation,
    SystemStoreInformation,
    SystemRegistryAppendString,
    SystemAitSamplingValue,
    SystemVhdBootInformation,
    SystemCpuQuotaInformation,
    SystemNativeBasicInformation,
    SystemSpare1,
    SystemLowPriorityIoInformation,
    SystemTpmBootEntropyInformation,
    SystemVerifierCountersInformation,
    SystemPagedPoolInformationEx,
    SystemSystemPtesInformationEx,
    SystemNodeDistanceInformation,
    SystemAcpiAuditInformation,
    SystemBasicPerformanceInformation,
    SystemQueryPerformanceCounterInformation,
    MaxSystemInfoClass
} SYSTEM_INFORMATION_CLASS;

typedef enum _PROCESSINFOCLASS
{
    ProcessBasicInformation,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    ProcessWx86Information,
    ProcessHandleCount,
    ProcessAffinityMask,
    ProcessPriorityBoost,
    ProcessDeviceMap,
    ProcessSessionInformation,
    ProcessForegroundInformation,
    ProcessWow64Information,
    ProcessImageFileName,
    ProcessLUIDDeviceMapsEnabled,
    ProcessBreakOnTermination,
    ProcessDebugObjectHandle,
    ProcessDebugFlags,
    ProcessHandleTracing,
    ProcessIoPriority,
    ProcessExecuteFlags,
    ProcessResourceManagement,
    ProcessCookie,
    ProcessImageInformation,
    ProcessCycleTime,
    ProcessPagePriority,
    ProcessInstrumentationCallback,
    ProcessThreadStackAllocation,
    ProcessWorkingSetWatchEx,
    ProcessImageFileNameWin32,
    ProcessImageFileMapping,
    ProcessAffinityUpdateMode,
    ProcessMemoryAllocationMode,
    ProcessGroupInformation,
    ProcessTokenVirtualizationEnabled,
    ProcessConsoleHostProcess,
    ProcessWindowInformation,
    ProcessHandleInformation,
    ProcessMitigationPolicy,
    ProcessDynamicFunctionTableInformation,
    ProcessHandleCheckingMode,
    ProcessKeepAliveCount,
    ProcessRevokeFileHandles,
    MaxProcessInfoClass
} PROCESSINFOCLASS;

typedef struct _CLIENT_ID
{
    DWORD        UniqueProcess;
    DWORD        UniqueThread;
} CLIENT_ID, * PCLIENT_ID;

typedef struct _VM_COUNTERS
{
    ULONG PeakVirtualSize;
    ULONG VirtualSize;
    ULONG PageFaultCount;
    ULONG PeakWorkingSetSize;
    ULONG WorkingSetSize;
    ULONG QuotaPeakPagedPoolUsage;
    ULONG QuotaPagedPoolUsage;
    ULONG QuotaPeakNonPagedPoolUsage;
    ULONG QuotaNonPagedPoolUsage;
    ULONG PagefileUsage;
    ULONG PeakPagefileUsage;
} VM_COUNTERS, *PVM_COUNTERS;

typedef struct _SYSTEM_THREAD_INFORMATION
{
    LARGE_INTEGER   KernelTime;
    LARGE_INTEGER   UserTime;
    LARGE_INTEGER   CreateTime;
    ULONG           WaitTime;
    PVOID           StartAddress;
    CLIENT_ID       ClientId;
    KPRIORITY       Priority;
    KPRIORITY       BasePriority;
    ULONG           ContextSwitchCount;
    LONG            State;
    KWAIT_REASON    WaitReason;
} SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFORMATION
{
    ULONG          NextEntryDelta;          //构成结构序列的偏移量；
    ULONG          ThreadCount;             //线程数目；
    ULONG          Reserved[6];
    LARGE_INTEGER  CreateTime;              //创建时间；
    LARGE_INTEGER  UserTime;                //用户模式(Ring 3)的CPU时间；
    LARGE_INTEGER  KernelTime;              //内核模式(Ring 0)的CPU时间；
    UNICODE_STRING ProcessName;             //进程名称；
    KPRIORITY      BasePriority;            //进程优先权；
    ULONG          ProcessId;               //进程标识符；
    ULONG          InheritedFromProcessId;  //父进程的标识符；
    ULONG          HandleCount;             //句柄数目；
    ULONG          Reserved2[2];
    VM_COUNTERS    VmCounters;              //虚拟存储器的结构；
    IO_COUNTERS    IoCounters;              //IO计数结构； Windows 2000 only
    SYSTEM_THREAD_INFORMATION Threads[1];   //进程相关线程的结构数组
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

typedef struct _PROCESS_BASIC_INFORMATION
{
    LONG ExitStatus;
    PVOID PebBaseAddress;
    ULONG_PTR AffinityMask;
    LONG BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR ParentProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;

typedef struct _SYSTEM_PROCESS_ID_INFORMATION
{
    HANDLE ProcessId;
    UNICODE_STRING ImageName;
} SYSTEM_PROCESS_ID_INFORMATION, *PSYSTEM_PROCESS_ID_INFORMATION;

// 定义NtQuerySystemInformation函数
typedef NTSTATUS(NTAPI *_NtQuerySystemInformation)(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,  // 定义服务类型号
    OUT PVOID SystemInformation,                         // 用户存储信息的缓冲区
    IN ULONG SystemInformationLength,                    // 缓冲区大小
    OUT PULONG ReturnLength                              // 返回信息长度
);

// 定义NtQueryInformationProcess函数
typedef NTSTATUS(NTAPI *_NtQueryInformationProcess)(
    IN HANDLE ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength,
    OUT PULONG ReturnLength
);

typedef NTSTATUS(NTAPI *_NtSuspendProcess)(
    IN HANDLE ProcessHandle);

typedef NTSTATUS(NTAPI *_NtResumeProcess)(
    IN HANDLE ProcessHandle);

typedef BOOL (WINAPI *_QueryFullProcessImageName)(
    IN HANDLE hProcess,
    IN DWORD dwFlags,
    OUT LPTSTR lpExeName,
    IN PDWORD lpdwSize
);

BOOL EnableDebugPrivilege(void);
BOOL KillProcessMethod_1(DWORD dwProcessID);
BOOL KillProcessMethod_2(DWORD dwProcessID);
BOOL IsProcessSuspend(DWORD dwProcessID);
BOOL SuspendProcess(DWORD dwProcessID);
BOOL ResumeProcess(DWORD dwProcessID);
BOOL GetProcessPathByPID(DWORD dwProcessID, LPTSTR szFullPath, DWORD nSize);
//BOOL GetProcessPathByPID2(DWORD dwProcessID, LPTSTR szFullPath, DWORD nSize);
//BOOL GetProcessPathByPID3(DWORD dwProcessID, LPTSTR szFullPath, DWORD nSize);
//BOOL GetProcessPathByPID4(DWORD dwProcessID, LPTSTR szFullPath, DWORD nSize);
//BOOL GetProcessPathByPID5(DWORD dwProcessID, LPTSTR szFullPath, DWORD nSize);
//BOOL GetProcessMemInfoByPID(DWORD dwProcessID, PDWORD dwWorkingSet);
BOOL GetProcessCommandLineByPID(DWORD dwProcessID, LPTSTR szCommandLine, DWORD nSize);
BOOL ReduceProcessWorkingSet(DWORD dwProcessID);
#endif
