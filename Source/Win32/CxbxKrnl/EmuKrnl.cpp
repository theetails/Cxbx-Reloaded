// ******************************************************************
// *
// *    .,-:::::    .,::      .::::::::.    .,::      .:
// *  ,;;;'````'    `;;;,  .,;;  ;;;'';;'   `;;;,  .,;; 
// *  [[[             '[[,,[['   [[[__[[\.    '[[,,[['  
// *  $$$              Y$$$P     $$""""Y$$     Y$$$P    
// *  `88bo,__,o,    oP"``"Yo,  _88o,,od8P   oP"``"Yo,  
// *    "YUMMMMMP",m"       "Mm,""YUMMMP" ,m"       "Mm,
// *
// *   Cxbx->Win32->CxbxKrnl->EmuKrnl.cpp
// *
// *  This file is part of the Cxbx project.
// *
// *  Cxbx and Cxbe are free software; you can redistribute them
// *  and/or modify them under the terms of the GNU General Public
// *  License as published by the Free Software Foundation; either
// *  version 2 of the license, or (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// *  GNU General Public License for more details.
// *
// *  You should have recieved a copy of the GNU General Public License
// *  along with this program; see the file COPYING.
// *  If not, write to the Free Software Foundation, Inc.,
// *  59 Temple Place - Suite 330, Bostom, MA 02111-1307, USA.
// *
// *  (c) 2002-2003 Aaron Robinson <caustik@caustik.com>
// *
// *  All rights reserved
// *
// ******************************************************************
#define _CXBXKRNL_INTERNAL
#define _XBOXKRNL_LOCAL_

// ******************************************************************
// * prevent name collisions
// ******************************************************************
namespace xboxkrnl
{
    #include <xboxkrnl/xboxkrnl.h>
};

#include <stdio.h>
#include <locale.h>

// ******************************************************************
// * prevent name collisions
// ******************************************************************
namespace NtDll
{
    #include "EmuNtDll.h"
};

#include "Emu.h"
#include "EmuFS.h"
#include "EmuFile.h"

// ******************************************************************
// * prevent name collisions
// ******************************************************************
namespace XTL
{
    #include "EmuXTL.h"
};

// ******************************************************************
// * Loaded at run-time to avoid linker conflicts
// ******************************************************************
static HMODULE hNtDll = GetModuleHandle("ntdll");

NtDll::FPTR_RtlInitAnsiString              NT_RtlInitAnsiString            = (NtDll::FPTR_RtlInitAnsiString)GetProcAddress(hNtDll, "RtlInitAnsiString");
NtDll::FPTR_RtlInitUnicodeString           NT_RtlInitUnicodeString         = (NtDll::FPTR_RtlInitUnicodeString)GetProcAddress(hNtDll, "RtlInitUnicodeString");
NtDll::FPTR_RtlAnsiStringToUnicodeString   NT_RtlAnsiStringToUnicodeString = (NtDll::FPTR_RtlAnsiStringToUnicodeString)GetProcAddress(hNtDll, "RtlAnsiStringToUnicodeString");
NtDll::FPTR_RtlUnicodeStringToAnsiString   NT_RtlUnicodeStringToAnsiString = (NtDll::FPTR_RtlUnicodeStringToAnsiString)GetProcAddress(hNtDll, "RtlUnicodeStringToAnsiString");
NtDll::FPTR_RtlNtStatusToDosError          NT_RtlNtStatusToDosError        = (NtDll::FPTR_RtlNtStatusToDosError)GetProcAddress(hNtDll, "RtlNtStatusToDosError");
NtDll::FPTR_RtlInitializeCriticalSection   NT_RtlInitializeCriticalSection = (NtDll::FPTR_RtlInitializeCriticalSection)GetProcAddress(hNtDll, "RtlInitializeCriticalSection");
NtDll::FPTR_RtlEnterCriticalSection        NT_RtlEnterCriticalSection      = (NtDll::FPTR_RtlEnterCriticalSection)GetProcAddress(hNtDll, "RtlEnterCriticalSection");
NtDll::FPTR_RtlLeaveCriticalSection        NT_RtlLeaveCriticalSection      = (NtDll::FPTR_RtlLeaveCriticalSection)GetProcAddress(hNtDll, "RtlLeaveCriticalSection");

NtDll::FPTR_NtAllocateVirtualMemory        NT_NtAllocateVirtualMemory      = (NtDll::FPTR_NtAllocateVirtualMemory)GetProcAddress(hNtDll, "NtAllocateVirtualMemory");
NtDll::FPTR_NtClose                        NT_NtClose                      = (NtDll::FPTR_NtClose)GetProcAddress(hNtDll, "NtClose");
NtDll::FPTR_NtDelayExecution               NT_NtDelayExecution             = (NtDll::FPTR_NtDelayExecution)GetProcAddress(hNtDll, "NtDelayExecution");
NtDll::FPTR_NtDuplicateObject              NT_NtDuplicateObject            = (NtDll::FPTR_NtDuplicateObject)GetProcAddress(hNtDll, "NtDuplicateObject");
NtDll::FPTR_NtQueryInformationFile         NT_NtQueryInformationFile       = (NtDll::FPTR_NtQueryInformationFile)GetProcAddress(hNtDll, "NtQueryInformationFile");
NtDll::FPTR_NtQueryFullAttributesFile      NT_NtQueryFullAttributesFile    = (NtDll::FPTR_NtQueryFullAttributesFile)GetProcAddress(hNtDll, "NtQueryFullAttributesFile");
NtDll::FPTR_NtQueryVolumeInformationFile   NT_NtQueryVolumeInformationFile = (NtDll::FPTR_NtQueryVolumeInformationFile)GetProcAddress(hNtDll, "NtQueryVolumeInformationFile");
NtDll::FPTR_NtCreateEvent                  NT_NtCreateEvent                = (NtDll::FPTR_NtCreateEvent)GetProcAddress(hNtDll, "NtCreateEvent");
NtDll::FPTR_NtCreateFile                   NT_NtCreateFile                 = (NtDll::FPTR_NtCreateFile)GetProcAddress(hNtDll, "NtCreateFile");
NtDll::FPTR_NtReadFile                     NT_NtReadFile                   = (NtDll::FPTR_NtReadFile)GetProcAddress(hNtDll, "NtReadFile");
NtDll::FPTR_NtWriteFile                    NT_NtWriteFile                  = (NtDll::FPTR_NtWriteFile)GetProcAddress(hNtDll, "NtWriteFile");
NtDll::FPTR_NtSetInformationFile           NT_NtSetInformationFile         = (NtDll::FPTR_NtSetInformationFile)GetProcAddress(hNtDll, "NtSetInformationFile");

// ******************************************************************
// * (Helper) PCSTProxyParam
// ******************************************************************
typedef struct _PCSTProxyParam
{
    IN PVOID StartContext1;
    IN PVOID StartContext2;
    IN PVOID StartRoutine;
}
PCSTProxyParam;

// ******************************************************************
// * (Helper) PCSTProxy
// ******************************************************************
#pragma warning(push)
#pragma warning(disable: 4731)  // disable ebp modification warning
static DWORD WINAPI PCSTProxy
(
    IN PVOID Parameter
)
{
    PCSTProxyParam *iPCSTProxyParam = (PCSTProxyParam*)Parameter;

    uint32 StartContext1 = (uint32)iPCSTProxyParam->StartContext1;
    uint32 StartContext2 = (uint32)iPCSTProxyParam->StartContext2;
    uint32 StartRoutine  = (uint32)iPCSTProxyParam->StartRoutine;

    delete iPCSTProxyParam;

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): PCSTProxy\n"
               "(\n"
               "   StartContext1       : 0x%.08X\n"
               "   StartContext2       : 0x%.08X\n"
               "   StartRoutine        : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), StartContext1, StartContext2, StartRoutine);
    }
    #endif

    EmuGenerateFS(g_pTLS, g_pTLSData);

    // ******************************************************************
    // * use the special calling convention
    // ******************************************************************
    __try
    {
        EmuSwapFS();   // Xbox FS

        __asm
        {
            mov         esi, StartRoutine
            push        StartContext2
            push        StartContext1
            push        offset callComplete
            lea         ebp, [esp-4]
            jmp near    esi
        }
    }
    __except(EmuException(GetExceptionInformation()))
    {
        printf("Emu: WARNING!! Problem with ExceptionFilter\n");
    }

callComplete:

    EmuSwapFS();    // Win2k/XP FS

    EmuCleanThread();

    return 0;
}
#pragma warning(pop)

using namespace xboxkrnl;

// ******************************************************************
// * 0x0018 ExQueryNonVolatileSetting
// ******************************************************************
XBSYSAPI EXPORTNUM(24) NTSTATUS NTAPI xboxkrnl::ExQueryNonVolatileSetting
(
    IN  DWORD               ValueIndex,
    OUT DWORD              *Type,
    OUT PUCHAR              Value,
    IN  SIZE_T              ValueLength,
    OUT PSIZE_T             ResultLength OPTIONAL
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): ExQueryNonVolatileSetting\n"
               "(\n"
               "   ValueIndex          : 0x%.08X\n"
               "   Type                : 0x%.08X\n"
               "   Value               : 0x%.08X\n"
               "   ValueLength         : 0x%.08X\n"
               "   ResultLength        : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), ValueIndex, Type, Value, ValueLength, ResultLength);
    }
    #endif

    // ******************************************************************
    // * handle eeprom read
    // ******************************************************************
    switch(ValueIndex)
    {
        // Factory Game Region
        case 0x104:
        {
            // TODO: configurable region or autodetect of some sort
            if(Type != 0)
                *Type = 0x04;

            if(Value != 0)
                *Value = 0x01;  // North America

            if(ResultLength != 0)
                *ResultLength = 0x04;
        }
        break;

        // Factory AC Region
        case 0x103:
        {
            // TODO: configurable region or autodetect of some sort
            if(Type != 0)
                *Type = 0x04;

            if(Value != 0)
                *Value = 0x01; // NTSC_M

            if(ResultLength != 0)
                *ResultLength = 0x04;
        }
        break;

        // Language
        case 0x007:
        {
            // TODO: configurable language or autodetect of some sort
            if(Type != 0)
                *Type = 0x04;

            if(Value != 0)
                *Value = 0x01;  // English

            if(ResultLength != 0)
                *ResultLength = 0x04;
        }
        break;

        // Video Flags
        case 0x008:
        {
            // TODO: configurable video flags or autodetect of some sort
            if(Type != 0)
                *Type = 0x04;

            if(Value != 0)
                *Value = 0x10;  // Letterbox

            if(ResultLength != 0)
                *ResultLength = 0x04;
        }
        break;

        case EEPROM_MISC:
        {
            if(Type != 0)
                *Type  = 0x04;

            if(Value != 0)
                *Value = 0;

            if(ResultLength != 0)
                *ResultLength = 0x04;
        }
        break;

        default:
            printf("ExQueryNonVolatileSetting unknown ValueIndex (%d)\n", ValueIndex);
            break;
    }

    EmuSwapFS();   // Xbox FS

    return STATUS_SUCCESS;
}

// ******************************************************************
// * 0x0025 - FscSetCacheSize
// ******************************************************************
XBSYSAPI EXPORTNUM(37) xboxkrnl::LONG NTAPI xboxkrnl::FscSetCacheSize(ULONG uCachePages)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): FscSetCacheSize\n"
               "(\n"
               "   uCachePages         : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), uCachePages);
    }
    #endif

    printf("*Warning* FscSetCacheSize is being ignored\n");

    EmuSwapFS();   // Xbox FS

    return 0;
}

// ******************************************************************
// * 0x0031 - HalReturnToFirmware
// ******************************************************************
XBSYSAPI EXPORTNUM(49) VOID DECLSPEC_NORETURN xboxkrnl::HalReturnToFirmware
(
    RETURN_FIRMWARE Routine
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): HalReturnToFirmware\n"
               "(\n"
               "   Routine             : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), Routine);
    }
    #endif

    EmuCleanup("Xbe has rebooted : HalReturnToFirmware(%d)", Routine);

    EmuSwapFS();   // Xbox FS
}

// ******************************************************************
// * 0x0042 - IoCreateFile
// ******************************************************************
XBSYSAPI EXPORTNUM(66) NTSTATUS NTAPI xboxkrnl::IoCreateFile
(
    OUT PHANDLE             FileHandle,
    IN  ACCESS_MASK         DesiredAccess,
    IN  POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    IN  PLARGE_INTEGER      AllocationSize,
    IN  ULONG               FileAttributes,
    IN  ULONG               ShareAccess,
    IN  ULONG               Disposition,
    IN  ULONG               CreateOptions,
    IN  ULONG               Options
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): IoCreateFile\n"
               "(\n"
               "   FileHandle          : 0x%.08X\n"
               "   DesiredAccess       : 0x%.08X\n"
               "   ObjectAttributes    : 0x%.08X (%s)\n"
               "   IoStatusBlock       : 0x%.08X\n"
               "   AllocationSize      : 0x%.08X\n"
               "   FileAttributes      : 0x%.08X\n"
               "   ShareAccess         : 0x%.08X\n"
               "   Disposition         : 0x%.08X\n"
               "   CreateOptions       : 0x%.08X\n"
               "   Options             : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), FileHandle, DesiredAccess, ObjectAttributes, ObjectAttributes->ObjectName->Buffer,
               IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, Disposition, CreateOptions, Options);
    }
    #endif

    NTSTATUS ret = STATUS_SUCCESS;

    // TODO: Use NtCreateFile if necessary. If it will work, we're fine
    EmuCleanup("IoCreateFile not implemented");

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x0043 IoCreateSymbolicLink
// ******************************************************************
XBSYSAPI EXPORTNUM(67) NTSTATUS xboxkrnl::IoCreateSymbolicLink
(
    IN PSTRING SymbolicLinkName,
    IN PSTRING DeviceName
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): IoCreateSymbolicLink\n"
               "(\n"
               "   SymbolicLinkName    : 0x%.08X (%s)\n"
               "   DeviceName          : 0x%.08X (%s)\n"
               ");\n",
               GetCurrentThreadId(), SymbolicLinkName, SymbolicLinkName->Buffer,
               DeviceName, DeviceName->Buffer);
    }
    #endif

    EmuCleanup("IoCreateSymbolicLink not implemented");

    // TODO: Actually um...implement this function
    NTSTATUS ret = STATUS_OBJECT_NAME_COLLISION;

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x0063 - KeDelayExecutionThread
// ******************************************************************
XBSYSAPI EXPORTNUM(99) NTSTATUS NTAPI xboxkrnl::KeDelayExecutionThread
(
    IN KPROCESSOR_MODE  WaitMode,
    IN BOOLEAN          Alertable,
    IN PLARGE_INTEGER   Interval
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): KeDelayExecutionThread\n"
               "(\n"
               "   WaitMode            : 0x%.08X\n"
               "   Alertable           : 0x%.08X\n"
               "   Interval            : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), WaitMode, Alertable, Interval);
    }
    #endif

    NTSTATUS ret = NT_NtDelayExecution(Alertable, (NtDll::LARGE_INTEGER*)Interval);

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x006B - KeInitializeDpc
// ******************************************************************
XBSYSAPI EXPORTNUM(107) VOID NTAPI xboxkrnl::KeInitializeDpc
(
    KDPC                *Dpc,
    PKDEFERRED_ROUTINE   DeferredRoutine,
    PVOID                DeferredContext
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): KeInitializeDpc\n"
               "(\n"
               "   Dpc                 : 0x%.08X\n"
               "   DeferredRoutine     : 0x%.08X\n"
               "   DeferredContext     : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), Dpc, DeferredRoutine, DeferredContext);
    }
    #endif

    Dpc->Number = 0;
    Dpc->DeferredRoutine = DeferredRoutine;
    Dpc->Type = DpcObject;
    Dpc->DeferredContext = DeferredContext;

    EmuSwapFS();   // Xbox FS

    return;
}

// ******************************************************************
// * 0x0071 - KeInitializeTimerEx
// ******************************************************************
XBSYSAPI EXPORTNUM(113) VOID NTAPI xboxkrnl::KeInitializeTimerEx
(
    IN PKTIMER      Timer,
    IN TIMER_TYPE   Type
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): KeInitializeTimerEx\n"
               "(\n"
               "   Timer               : 0x%.08X\n"
               "   Type                : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), Timer, Type);
    }
    #endif
    
    Timer->Header.Type               = Type + 8;
    Timer->Header.Inserted           = 0;
    Timer->Header.Size               = sizeof(*Timer) / sizeof(ULONG);
    Timer->Header.SignalState        = 0;
    Timer->TimerListEntry.Blink      = NULL;
    Timer->TimerListEntry.Flink      = NULL;
    Timer->Header.WaitListHead.Flink = &Timer->Header.WaitListHead;
    Timer->Header.WaitListHead.Blink = &Timer->Header.WaitListHead;
    Timer->DueTime.QuadPart          = 0;
    Timer->Period                    = 0;

    EmuSwapFS();   // Xbox FS

    return;
}

// ******************************************************************
// * 0x0080 - KeQuerySystemTime
// ******************************************************************
XBSYSAPI EXPORTNUM(128) VOID NTAPI xboxkrnl::KeQuerySystemTime
(
    PLARGE_INTEGER CurrentTime
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): KeQuerySystemTime\n"
               "(\n"
               "   CurrentTime         : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), CurrentTime);
    }
    #endif

    SYSTEMTIME SystemTime;

    GetSystemTime(&SystemTime);

    SystemTimeToFileTime(&SystemTime, (FILETIME*)CurrentTime);

    EmuSwapFS();   // Xbox FS

    return;
}

// ******************************************************************
// * 0x0095 - KeSetTimer
// ******************************************************************
XBSYSAPI EXPORTNUM(149) xboxkrnl::BOOLEAN NTAPI xboxkrnl::KeSetTimer
(
    IN PKTIMER        Timer,
    IN LARGE_INTEGER  DueTime,
    IN PKDPC          Dpc OPTIONAL
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): KeSetTimer\n"
               "(\n"
               "   Timer               : 0x%.08X\n"
               "   DueTime             : 0x%I64X\n"
               "   Dpc                 : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), Timer, DueTime, Dpc);
    }
    #endif

    EmuCleanup("KeSetTimer: Not Implemented!");

    EmuSwapFS();   // Xbox FS

    return TRUE;
}

// ******************************************************************
// * 0x009C - KeTickCount
// ******************************************************************
XBSYSAPI EXPORTNUM(156) xboxkrnl::DWORD xboxkrnl::KeTickCount = 0;

// ******************************************************************
// * 0x00A4 - LaunchDataPage (actually a pointer)
// ******************************************************************
XBSYSAPI EXPORTNUM(164) xboxkrnl::DWORD xboxkrnl::LaunchDataPage = 0;

// ******************************************************************
// * 0x00A5 - MmAllocateContiguousMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(165) xboxkrnl::PVOID NTAPI xboxkrnl::MmAllocateContiguousMemory
(
	IN ULONG NumberOfBytes
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): MmAllocateContiguousMemory\n"
               "(\n"
               "   NumberOfBytes            : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), NumberOfBytes);
    }
    #endif

    // TODO: Make this much more efficient and correct if necessary!
    // HACK: Should be aligned!!
    PVOID pRet = (PVOID)new unsigned char[NumberOfBytes];

    EmuSwapFS();   // Xbox FS

    return pRet;
}

// ******************************************************************
// * 0x00A6 - MmAllocateContiguousMemoryEx
// ******************************************************************
XBSYSAPI EXPORTNUM(166) xboxkrnl::PVOID NTAPI xboxkrnl::MmAllocateContiguousMemoryEx
(
	IN ULONG			NumberOfBytes,
	IN PHYSICAL_ADDRESS LowestAcceptableAddress,
	IN PHYSICAL_ADDRESS HighestAcceptableAddress,
	IN ULONG			Alignment OPTIONAL,
	IN ULONG			ProtectionType
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): MmAllocateContiguousMemoryEx\n"
               "(\n"
               "   NumberOfBytes            : 0x%.08X\n"
               "   LowestAcceptableAddress  : 0x%.08X\n"
               "   HighestAcceptableAddress : 0x%.08X\n"
               "   Alignment                : 0x%.08X\n"
               "   ProtectionType           : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), NumberOfBytes, LowestAcceptableAddress, HighestAcceptableAddress,
               Alignment, ProtectionType);
    }
    #endif

    // TODO: Make this much more efficient and correct if necessary!
    // HACK: Should be aligned!!
    PVOID pRet = (PVOID)new unsigned char[NumberOfBytes];

    EmuSwapFS();   // Xbox FS

    return pRet;
}

// ******************************************************************
// * 0x00AB - MmFreeContiguousMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(171) VOID NTAPI xboxkrnl::MmFreeContiguousMemory
(
	IN PVOID BaseAddress
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): MmFreeContiguousMemory\n"
               "(\n"
               "   BaseAddress              : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), BaseAddress);
    }
    #endif

    delete BaseAddress;

    EmuSwapFS();   // Xbox FS

    return;
}

// ******************************************************************
// * 0x00B2 - MmPersistContiguousMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(178) VOID NTAPI xboxkrnl::MmPersistContiguousMemory
(
    IN PVOID   BaseAddress,
    IN ULONG   NumberOfBytes,
    IN BOOLEAN Persist
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): MmPersistContiguousMemory\n"
               "(\n"
               "   BaseAddress              : 0x%.08X\n"
               "   NumberOfBytes            : 0x%.08X\n"
               "   Persist                  : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), BaseAddress, NumberOfBytes, Persist);
    }
    #endif

    // TODO: Actually set this up to be remember across a "reboot"
    printf("*Warning* MmPersistContiguousMemory is being ignored\n");

    EmuSwapFS();   // Xbox FS
}

// ******************************************************************
// * 0x00B8 - NtAllocateVirtualMemory
// ******************************************************************
XBSYSAPI EXPORTNUM(184) NTSTATUS NTAPI xboxkrnl::NtAllocateVirtualMemory
(
    IN OUT PVOID    *BaseAddress,
    IN ULONG         ZeroBits,
    IN OUT PULONG    AllocationSize,
    IN DWORD         AllocationType,
    IN DWORD         Protect
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtAllocateVirtualMemory\n"
               "(\n"
               "   BaseAddress         : 0x%.08X (0x%.08X)\n"
               "   ZeroBits            : 0x%.08X\n"
               "   AllocationSize      : 0x%.08X (0x%.08X)\n"
               "   AllocationType      : 0x%.08X\n"
               "   Protect             : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), BaseAddress, *BaseAddress, ZeroBits, AllocationSize, *AllocationSize, AllocationType, Protect);
    }
    #endif

    NTSTATUS ret = NT_NtAllocateVirtualMemory(GetCurrentProcess(), BaseAddress, ZeroBits, AllocationSize, AllocationType, Protect);

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x00BB - NtClose
// ******************************************************************
XBSYSAPI EXPORTNUM(187) NTSTATUS NTAPI xboxkrnl::NtClose
(
    IN HANDLE Handle
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtClose\n"
               "(\n"
               "   Handle              : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), Handle);
    }
    #endif

    NTSTATUS ret = STATUS_SUCCESS;
    
    // ******************************************************************
    // * delete 'special' handles
    // ******************************************************************
    if(IsEmuHandle(Handle))
    {
        EmuHandle *iEmuHandle = EmuHandleToPtr(Handle);

        delete iEmuHandle;

        ret = STATUS_SUCCESS;
    }
    // ******************************************************************
    // * close normal handles
    // ******************************************************************
    else
    {
        ret = NT_NtClose(Handle);
    }

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x00BD - NtCreateEvent
// ******************************************************************
XBSYSAPI EXPORTNUM(189) NTSTATUS NTAPI xboxkrnl::NtCreateEvent
(
    OUT PHANDLE             EventHandle,
    IN  POBJECT_ATTRIBUTES  ObjectAttributes OPTIONAL,
    IN  EVENT_TYPE          EventType,
    IN  BOOLEAN             InitialState
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtCreateEvent\n"
               "(\n"
               "   EventHandle         : 0x%.08X\n"
               "   ObjectAttributes    : 0x%.08X (\"%s\")\n"
               "   EventType           : 0x%.08X\n"
               "   InitialState        : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), EventHandle, ObjectAttributes, ObjectAttributes->ObjectName->Buffer,
               EventType, InitialState);
    }
    #endif

    char *szBuffer = ObjectAttributes->ObjectName->Buffer;

    wchar_t wszObjectName[160];

    NtDll::UNICODE_STRING    NtUnicodeString;
    NtDll::OBJECT_ATTRIBUTES NtObjAttr;

    // ******************************************************************
    // * Initialize Object Attributes
    // ******************************************************************
    {
        mbstowcs(wszObjectName, szBuffer, 160);

        NT_RtlInitUnicodeString(&NtUnicodeString, wszObjectName);

        InitializeObjectAttributes(&NtObjAttr, &NtUnicodeString, ObjectAttributes->Attributes, ObjectAttributes->RootDirectory, NULL);
    }

    // ******************************************************************
    // * Redirect to NtCreateEvent
    // ******************************************************************
    NTSTATUS ret = NT_NtCreateEvent(EventHandle, EVENT_ALL_ACCESS, &NtObjAttr, (NtDll::EVENT_TYPE)EventType, InitialState);

    EmuSwapFS();   // Xbox FS

    return STATUS_SUCCESS;
}

// ******************************************************************
// * 0x00BE - NtCreateFile
// ******************************************************************
XBSYSAPI EXPORTNUM(190) NTSTATUS NTAPI xboxkrnl::NtCreateFile
(
    OUT PHANDLE             FileHandle, 
    IN  ACCESS_MASK         DesiredAccess,
    IN  POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    IN  PLARGE_INTEGER      AllocationSize OPTIONAL, 
    IN  ULONG               FileAttributes, 
    IN  ULONG               ShareAccess, 
    IN  ULONG               CreateDisposition, 
    IN  ULONG               CreateOptions 
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtCreateFile\n"
               "(\n"
               "   FileHandle          : 0x%.08X\n"
               "   DesiredAccess       : 0x%.08X\n"
               "   ObjectAttributes    : 0x%.08X (\"%s\")\n"
               "   IoStatusBlock       : 0x%.08X\n"
               "   AllocationSize      : 0x%.08X\n"
               "   FileAttributes      : 0x%.08X\n"
               "   ShareAccess         : 0x%.08X\n"
               "   CreateDisposition   : 0x%.08X\n"
               "   CreateOptions       : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), FileHandle, DesiredAccess, ObjectAttributes, ObjectAttributes->ObjectName->Buffer,
               IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions);
    }
    #endif

    char *szBuffer = ObjectAttributes->ObjectName->Buffer;

    // ******************************************************************
    // * D:\ should map to current directory
    // ******************************************************************
    if( (szBuffer[0] == 'D' || szBuffer[0] == 'd') && szBuffer[1] == ':' && szBuffer[2] == '\\')
    {
        szBuffer += 3;

        ObjectAttributes->RootDirectory = g_hCurDir;

        #ifdef _DEBUG_TRACE
        printf("EmuKrnl (0x%X): NtCreateFile Corrected path...\n", GetCurrentThreadId());
        printf("  Org:\"%s\"\n", ObjectAttributes->ObjectName->Buffer);
        printf("  New:\"$XbePath\\%s\"\n", szBuffer);
        #endif
    }
    else if( (szBuffer[0] == 'T' || szBuffer[0] == 't') && szBuffer[1] == ':' && szBuffer[2] == '\\')
    {
        szBuffer += 3;

        ObjectAttributes->RootDirectory = g_hTDrive;
        #ifdef _DEBUG_TRACE
        printf("EmuKrnl (0x%X): NtCreateFile Corrected path...\n", GetCurrentThreadId());
        printf("  Org:\"%s\"\n", ObjectAttributes->ObjectName->Buffer);
        printf("  New:\"$CxbxPath\\%s\"\n", szBuffer);
        #endif
    }
    else if( (szBuffer[0] == 'U' || szBuffer[0] == 'u') && szBuffer[1] == ':' && szBuffer[2] == '\\')
    {
        szBuffer += 3;

        ObjectAttributes->RootDirectory = g_hUDrive;
        #ifdef _DEBUG_TRACE
        printf("EmuKrnl (0x%X): NtCreateFile Corrected path...\n", GetCurrentThreadId());
        printf("  Org:\"%s\"\n", ObjectAttributes->ObjectName->Buffer);
        printf("  New:\"$CxbxPath\\%s\"\n", szBuffer);
        #endif
    }

    wchar_t wszObjectName[160];

    NtDll::UNICODE_STRING    NtUnicodeString;
    NtDll::OBJECT_ATTRIBUTES NtObjAttr;

    // ******************************************************************
    // * Initialize Object Attributes
    // ******************************************************************
    {
        mbstowcs(wszObjectName, szBuffer, 160);

        NT_RtlInitUnicodeString(&NtUnicodeString, wszObjectName);

        InitializeObjectAttributes(&NtObjAttr, &NtUnicodeString, ObjectAttributes->Attributes, ObjectAttributes->RootDirectory, NULL);
    }

    // ******************************************************************
    // * Redirect to NtCreateFile
    // ******************************************************************
    NTSTATUS ret = NT_NtCreateFile
    (
        FileHandle, DesiredAccess, &NtObjAttr, (NtDll::IO_STATUS_BLOCK*)IoStatusBlock,
        (NtDll::LARGE_INTEGER*)AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, NULL, NULL
    );

    // NOTE: We can map this to IoCreateFile once implemented (if ever necessary)
    //       xboxkrnl::IoCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, 0);

    EmuSwapFS();   // Xbox FS

    return STATUS_SUCCESS;
}

// ******************************************************************
// * 0x00C5 - NtDuplicateObject
// ******************************************************************
XBSYSAPI EXPORTNUM(197) NTSTATUS NTAPI xboxkrnl::NtDuplicateObject
(
    HANDLE  SourceHandle,
    HANDLE *TargetHandle,
    DWORD   Options
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtDuplicateObject\n"
               "(\n"
               "   SourceHandle        : 0x%.08X\n"
               "   TargetHandle        : 0x%.08X\n"
               "   Options             : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), SourceHandle, TargetHandle, Options);
    }
    #endif

    // ******************************************************************
    // * redirect to Win2k/XP
    // ******************************************************************
    NTSTATUS ret = NT_NtDuplicateObject
    (
        GetCurrentProcess(),
        SourceHandle,
        GetCurrentProcess(),
        TargetHandle,
        0, 0, Options
    );

    if(ret != STATUS_SUCCESS)
        printf("*Warning* Object was not duplicated\n");

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x00CA - NtOpenFile
// ******************************************************************
XBSYSAPI EXPORTNUM(202) NTSTATUS NTAPI xboxkrnl::NtOpenFile
(
    OUT PHANDLE             FileHandle,
    IN  ACCESS_MASK         DesiredAccess,
    IN  POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK    IoStatusBlock,
    IN  ULONG               ShareAccess,
    IN  ULONG               OpenOptions
)
{
    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        EmuSwapFS();   // Win2k/XP FS
        printf("EmuKrnl (0x%X): NtOpenFile\n"
               "(\n"
               "   FileHandle          : 0x%.08X\n"
               "   DesiredAccess       : 0x%.08X\n"
               "   ObjectAttributes    : 0x%.08X (\"%s\")\n"
               "   IoStatusBlock       : 0x%.08X\n"
               "   ShareAccess         : 0x%.08X\n"
               "   CreateOptions       : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), FileHandle, DesiredAccess, ObjectAttributes, ObjectAttributes->ObjectName->Buffer,
               IoStatusBlock, ShareAccess, OpenOptions);
        EmuSwapFS();   // Xbox FS
    }
    #endif

    return NtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, NULL, 0, ShareAccess, FILE_OPEN, OpenOptions);
}

// ******************************************************************
// * 0x00D2 - NtQueryFullAttributesFile
// ******************************************************************
XBSYSAPI EXPORTNUM(210) NTSTATUS NTAPI xboxkrnl::NtQueryFullAttributesFile
(   
    IN  POBJECT_ATTRIBUTES          ObjectAttributes,
    OUT PVOID                       Attributes
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtQueryFullAttributesFile\n"
               "(\n"
               "   ObjectAttributes    : 0x%.08X (%s)\n"
               "   Attributes          : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), ObjectAttributes, ObjectAttributes->ObjectName->Buffer, Attributes);
    }
    #endif

    char *szBuffer = ObjectAttributes->ObjectName->Buffer;

    wchar_t wszObjectName[160];

    NtDll::UNICODE_STRING    NtUnicodeString;
    NtDll::OBJECT_ATTRIBUTES NtObjAttr;

    // ******************************************************************
    // * Initialize Object Attributes
    // ******************************************************************
    {
        mbstowcs(wszObjectName, szBuffer, 160);

        NT_RtlInitUnicodeString(&NtUnicodeString, wszObjectName);

        InitializeObjectAttributes(&NtObjAttr, &NtUnicodeString, ObjectAttributes->Attributes, ObjectAttributes->RootDirectory, NULL);
    }

	NTSTATUS ret = NT_NtQueryFullAttributesFile(&NtObjAttr, Attributes);

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x00D3 - NtQueryInformationFile
// ******************************************************************
XBSYSAPI EXPORTNUM(211) NTSTATUS NTAPI xboxkrnl::NtQueryInformationFile
(   
    IN  HANDLE                      FileHandle,
    OUT PIO_STATUS_BLOCK            IoStatusBlock,
    OUT PFILE_FS_SIZE_INFORMATION   FileInformation, 
    IN  ULONG                       Length, 
    IN  FILE_INFORMATION_CLASS      FileInfo
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtQueryInformationFile\n"
               "(\n"
               "   FileHandle          : 0x%.08X\n"
               "   IoStatusBlock       : 0x%.08X\n"
               "   FileInformation     : 0x%.08X\n"
               "   Length              : 0x%.08X\n"
               "   FileInformationClass: 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), FileHandle, IoStatusBlock, FileInformation, 
               Length, FileInfo);
    }
    #endif

	NTSTATUS ret = NT_NtQueryInformationFile
	(
		FileHandle,
        (NtDll::PIO_STATUS_BLOCK)IoStatusBlock,
        (NtDll::PFILE_FS_SIZE_INFORMATION)FileInformation,
		Length,
        (NtDll::FILE_INFORMATION_CLASS)FileInfo
	);

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x00DA - NtQueryVolumeInformationFile
// ******************************************************************
XBSYSAPI EXPORTNUM(218) NTSTATUS NTAPI xboxkrnl::NtQueryVolumeInformationFile
(
    IN  HANDLE                      FileHandle,
    OUT PIO_STATUS_BLOCK            IoStatusBlock,
    OUT PFILE_FS_SIZE_INFORMATION   FileInformation,
    IN  ULONG                       Length,
    IN  FS_INFORMATION_CLASS        FileInformationClass
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtQueryVolumeInformationFile\n"
               "(\n"
               "   FileHandle          : 0x%.08X\n"
               "   IoStatusBlock       : 0x%.08X\n"
               "   FileInformation     : 0x%.08X\n"
               "   Length              : 0x%.08X\n"
               "   FileInformationClass: 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), FileHandle, IoStatusBlock, FileInformation, 
               Length, FileInformationClass);
    }
    #endif

    // Safety/Sanity Check
    if(FileInformationClass != FileFsSizeInformation && FileInformationClass != FileDirectoryInformation)
        EmuCleanup("NtQueryVolumeInformationFile: Unsupported FileInformationClass");

    NTSTATUS ret = NT_NtQueryVolumeInformationFile
    (
        FileHandle,
        (NtDll::PIO_STATUS_BLOCK)IoStatusBlock,
        (NtDll::PFILE_FS_SIZE_INFORMATION)FileInformation, Length,
        (NtDll::FS_INFORMATION_CLASS)FileInformationClass
    );
/*
    {
        FILE_FS_SIZE_INFORMATION *SizeInfo = (FILE_FS_SIZE_INFORMATION*)FileInformation;

        SizeInfo->TotalAllocationUnits.QuadPart     = 0x4C468;
        SizeInfo->AvailableAllocationUnits.QuadPart = 0x2F125;
        SizeInfo->SectorsPerAllocationUnit          = 32;
        SizeInfo->BytesPerSector                    = 512;
    }
*/
    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x00DA - NtReadFile
// ******************************************************************
XBSYSAPI EXPORTNUM(219) NTSTATUS NTAPI xboxkrnl::NtReadFile
(
	IN  HANDLE          FileHandle,            // TODO: correct paramters
	IN  HANDLE          Event OPTIONAL,
	IN  PVOID           ApcRoutine OPTIONAL,
	IN  PVOID           ApcContext,
	OUT PVOID           IoStatusBlock,
	OUT PVOID           Buffer,
	IN  ULONG           Length,
	IN  PLARGE_INTEGER  ByteOffset OPTIONAL
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtReadFile\n"
               "(\n"
               "   FileHandle          : 0x%.08X\n"
               "   Event               : 0x%.08X\n"
               "   ApcRoutine          : 0x%.08X\n"
               "   ApcContext          : 0x%.08X\n"
               "   IoStatusBlock       : 0x%.08X\n"
               "   Buffer              : 0x%.08X\n"
               "   Length              : 0x%.08X\n"
               "   ByteOffset          : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), FileHandle, Event, ApcRoutine, 
               ApcContext, IoStatusBlock, Buffer, Length, ByteOffset);
    }
    #endif

    NTSTATUS ret = NT_NtReadFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, (NtDll::LARGE_INTEGER*)ByteOffset, 0);

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x00E2 - NtSetInformationFile
// ******************************************************************
XBSYSAPI EXPORTNUM(226) NTSTATUS NTAPI xboxkrnl::NtSetInformationFile
(	
	IN  HANDLE  FileHandle,            // TODO: correct paramters
	OUT	PVOID	IoStatusBlock,
	IN	PVOID	FileInformation,
	IN	ULONG	Length,
	IN	ULONG	FileInformationClass
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtSetInformationFile\n"
               "(\n"
               "   FileHandle           : 0x%.08X\n"
               "   IoStatusBlock        : 0x%.08X\n"
               "   FileInformation      : 0x%.08X\n"
               "   Length               : 0x%.08X\n"
               "   FileInformationClass : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), FileHandle, IoStatusBlock, FileInformation, 
               Length, FileInformationClass);
    }
    #endif

    NTSTATUS ret = NT_NtSetInformationFile(FileHandle, IoStatusBlock, FileInformation, Length, FileInformationClass);

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x00DA - NtWriteFile
// ******************************************************************
XBSYSAPI EXPORTNUM(236) NTSTATUS NTAPI xboxkrnl::NtWriteFile
(	
	IN  HANDLE  FileHandle,            // TODO: correct paramters
	IN	PVOID	Event,
	IN	PVOID	ApcRoutine,
	IN	PVOID	ApcContext,
	OUT	PVOID	IoStatusBlock,
	IN	PVOID	Buffer,
	IN	ULONG	Length,
	IN	PVOID	ByteOffset
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): NtWriteFile\n"
               "(\n"
               "   FileHandle          : 0x%.08X\n"
               "   Event               : 0x%.08X\n"
               "   ApcRoutine          : 0x%.08X\n"
               "   ApcContext          : 0x%.08X\n"
               "   IoStatusBlock       : 0x%.08X\n"
               "   Buffer              : 0x%.08X\n"
               "   Length              : 0x%.08X\n"
               "   ByteOffset          : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), FileHandle, Event, ApcRoutine, 
               ApcContext, IoStatusBlock, Buffer, Length, ByteOffset);
    }
    #endif

    NTSTATUS ret = NT_NtWriteFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, (NtDll::LARGE_INTEGER*)ByteOffset, 0);

    EmuSwapFS();   // Xbox FS

    return ret;
}
// ******************************************************************
// * 0x00FF - PsCreateSystemThreadEx
// ******************************************************************
XBSYSAPI EXPORTNUM(255) NTSTATUS NTAPI xboxkrnl::PsCreateSystemThreadEx
(
    OUT PHANDLE         ThreadHandle,
    IN  ULONG           ThreadExtraSize,
    IN  ULONG           KernelStackSize,
    IN  ULONG           TlsDataSize,
    OUT PULONG          ThreadId OPTIONAL,
    IN  PVOID           StartContext1,
    IN  PVOID           StartContext2,
    IN  BOOLEAN         CreateSuspended,
    IN  BOOLEAN         DebugStack,
    IN  PKSTART_ROUTINE StartRoutine
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): PsCreateSystemThreadEx\n"
               "(\n"
               "   ThreadHandle        : 0x%.08X\n"
               "   ThreadExtraSize     : 0x%.08X\n"
               "   KernelStackSize     : 0x%.08X\n"
               "   TlsDataSize         : 0x%.08X\n"
               "   ThreadId            : 0x%.08X\n"
               "   StartContext1       : 0x%.08X\n"
               "   StartContext2       : 0x%.08X\n"
               "   CreateSuspended     : 0x%.08X\n"
               "   DebugStack          : 0x%.08X\n"
               "   StartRoutine        : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), ThreadHandle, ThreadExtraSize, KernelStackSize, TlsDataSize, ThreadId,
               StartContext1, StartContext2, CreateSuspended, DebugStack, StartRoutine);
    }
    #endif

    // ******************************************************************
    // * create thread, using our special proxy technique
    // ******************************************************************
    {
        DWORD dwThreadId;

        // PCSTProxy is responsible for cleaning up this pointer
        ::PCSTProxyParam *iPCSTProxyParam = new ::PCSTProxyParam();

        iPCSTProxyParam->StartContext1 = StartContext1;
        iPCSTProxyParam->StartContext2 = StartContext2;
        iPCSTProxyParam->StartRoutine  = StartRoutine;

        *ThreadHandle = CreateThread(NULL, NULL, &PCSTProxy, iPCSTProxyParam, CreateSuspended ? CREATE_SUSPENDED : NULL, &dwThreadId);

        if(ThreadId != NULL)
            *ThreadId = dwThreadId;
    }

    EmuSwapFS();   // Xbox FS

    return STATUS_SUCCESS;
}

// ******************************************************************
// * PsTerminateSystemThread
// ******************************************************************
XBSYSAPI EXPORTNUM(258) VOID NTAPI xboxkrnl::PsTerminateSystemThread(IN NTSTATUS ExitStatus)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): PsTerminateSystemThread\n"
               "(\n"
               "   ExitStatus          : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), ExitStatus);
    }
    #endif

    ExitThread(ExitStatus);

    EmuSwapFS();   // Xbox FS

    return;
}

// ******************************************************************
// * RtlAnsiStringToUnicodeString
// ******************************************************************
XBSYSAPI EXPORTNUM(260) NTSTATUS NTAPI xboxkrnl::RtlAnsiStringToUnicodeString
(
    PUNICODE_STRING DestinationString,
    PSTRING         SourceString,
    UCHAR           AllocateDestinationString
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): RtlAnsiStringToUnicodeString\n"
               "(\n"
               "   DestinationString         : 0x%.08X\n"
               "   SourceString              : 0x%.08X\n"
               "   AllocateDestinationString : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), DestinationString, SourceString, AllocateDestinationString);
    }
    #endif

    NTSTATUS ret = NT_RtlAnsiStringToUnicodeString((NtDll::UNICODE_STRING*)DestinationString, (NtDll::STRING*)SourceString, AllocateDestinationString);

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x0115 RtlEnterCriticalSection
// ******************************************************************
XBSYSAPI EXPORTNUM(277) VOID NTAPI xboxkrnl::RtlEnterCriticalSection
(
  IN PRTL_CRITICAL_SECTION CriticalSection
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): RtlEnterCriticalSection\n"
               "(\n"
               "   CriticalSection     : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), CriticalSection);
    }
    #endif

    // This seems redundant, but xbox software doesn't always do it
    if(CriticalSection->LockCount == -1)
        NT_RtlInitializeCriticalSection((NtDll::_RTL_CRITICAL_SECTION*)CriticalSection);

    NT_RtlEnterCriticalSection((NtDll::_RTL_CRITICAL_SECTION*)CriticalSection);

    EmuSwapFS();   // Xbox FS

    return;
}

// ******************************************************************
// * 0x0121 - RtlInitAnsiString
// ******************************************************************
XBSYSAPI EXPORTNUM(289) VOID NTAPI xboxkrnl::RtlInitAnsiString 
(
  IN OUT PANSI_STRING DestinationString,
  IN     PCSZ         SourceString
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): RtlInitAnsiString\n"
               "(\n"
               "   DestinationString   : 0x%.08X\n"
               "   SourceString        : 0x%.08X (\"%s\")\n"
               ");\n",
               GetCurrentThreadId(), DestinationString, SourceString, SourceString);
    }
    #endif

    NT_RtlInitAnsiString((NtDll::PANSI_STRING)DestinationString, (NtDll::PCSZ)SourceString);

    EmuSwapFS();   // Xbox FS

    return;
}

// ******************************************************************
// * 0x0123 - RtlInitializeCriticalSection
// ******************************************************************
XBSYSAPI EXPORTNUM(291) VOID NTAPI xboxkrnl::RtlInitializeCriticalSection
(
  IN PRTL_CRITICAL_SECTION CriticalSection
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): RtlInitializeCriticalSection\n"
               "(\n"
               "   CriticalSection     : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), CriticalSection);
    }
    #endif

    NT_RtlInitializeCriticalSection((NtDll::_RTL_CRITICAL_SECTION*)CriticalSection);

    EmuSwapFS();   // Xbox FS

    return;
}

// ******************************************************************
// * 0x0126 RtlEnterCriticalSection
// ******************************************************************
XBSYSAPI EXPORTNUM(294) VOID NTAPI xboxkrnl::RtlLeaveCriticalSection
(
  IN PRTL_CRITICAL_SECTION CriticalSection
)
{
    EmuSwapFS();   // Win2k/XP FS

    // Note: We need to execute this before debug output to avoid trouble
    NT_RtlLeaveCriticalSection((NtDll::_RTL_CRITICAL_SECTION*)CriticalSection);

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): RtlLeaveCriticalSection\n"
               "(\n"
               "   CriticalSection     : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), CriticalSection);
    }
    #endif

    EmuSwapFS();   // Xbox FS

    return;
}

// ******************************************************************
// * 0x012D - RtlNtStatusToDosError
// ******************************************************************
XBSYSAPI EXPORTNUM(301) xboxkrnl::ULONG NTAPI xboxkrnl::RtlNtStatusToDosError
(
    IN NTSTATUS Status
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): RtlNtStatusToDosError\n"
               "(\n"
               "   Status              : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), Status);
    }
    #endif

    ULONG ret = NT_RtlNtStatusToDosError(Status);

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x0134 - RtlUnicodeStringToAnsiString
// ******************************************************************
XBSYSAPI EXPORTNUM(308) xboxkrnl::NTSTATUS NTAPI xboxkrnl::RtlUnicodeStringToAnsiString
(
    IN OUT PSTRING         DestinationString,
    IN     PUNICODE_STRING SourceString,
    IN     BOOLEAN         AllocateDestinationString
)
{
    EmuSwapFS();   // Win2k/XP FS

    // ******************************************************************
    // * debug trace
    // ******************************************************************
    #ifdef _DEBUG_TRACE
    {
        printf("EmuKrnl (0x%X): RtlUnicodeStringToAnsiString\n"
               "(\n"
               "   DestinationString         : 0x%.08X\n"
               "   SourceString              : 0x%.08X\n"
               "   AllocateDestinationString : 0x%.08X\n"
               ");\n",
               GetCurrentThreadId(), DestinationString, SourceString, AllocateDestinationString);
    }
    #endif

    NTSTATUS ret = NT_RtlUnicodeStringToAnsiString((NtDll::STRING*)DestinationString, (NtDll::UNICODE_STRING*)SourceString, AllocateDestinationString);

    EmuSwapFS();   // Xbox FS

    return ret;
}

// ******************************************************************
// * 0x0141 - XboxHardwareInfo
// ******************************************************************
XBSYSAPI EXPORTNUM(322) XBOX_HARDWARE_INFO xboxkrnl::XboxHardwareInfo = 
{
    0xC0000035,
    0,0,0,0
};

// ******************************************************************
// * HalBootSMCVideoMode
// ******************************************************************
// TODO: Verify this!
XBSYSAPI EXPORTNUM(356) xboxkrnl::DWORD xboxkrnl::HalBootSMCVideoMode = 1;
