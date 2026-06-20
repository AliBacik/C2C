#pragma once
#include <ntifs.h>

typedef struct _PEB_LDR_DATA {
    ULONG Length;
    BOOLEAN Initialized;
    HANDLE SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB {
    UCHAR InheritedAddressSpace;
    UCHAR ReadImageFileExecOptions;
    UCHAR BeingDebugged;
    UCHAR Spare;
    HANDLE Mutant;
    PVOID ImageBaseAddress;
    PPEB_LDR_DATA Ldr;
} PEB, *PPEB;

// C2C cihaz ve driver isimleri
#define C2C_DEVICE_NAME  L"\\Device\\C2C-kmd"
#define C2C_DOS_NAME     L"\\DosDevices\\C2C-kmd"
#define C2C_DRIVER_NAME  L"\\Driver\\C2C-kmd"

// C2C IOCTL kodlari (MemoryMgr.h ile birebir)
#define C2C_DEVICE_TYPE       0x8000
#define IOCTL_ATTACH          CTL_CODE(C2C_DEVICE_TYPE, 0x4452, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_READ            CTL_CODE(C2C_DEVICE_TYPE, 0x4453, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_GET_MODULE_BASE CTL_CODE(C2C_DEVICE_TYPE, 0x4454, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_GET_PID         CTL_CODE(C2C_DEVICE_TYPE, 0x4455, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_BATCH_READ      CTL_CODE(C2C_DEVICE_TYPE, 0x4456, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_MOUSE_CLICK     CTL_CODE(C2C_DEVICE_TYPE, 0x4457, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// IOCTL_READ - MemoryMgr.h _Request ile ayni
struct Request
{
    HANDLE process_id;
    PVOID  target;
    PVOID  buffer;
    SIZE_T size;
};

// IOCTL_GET_PID - MemoryMgr.h _PID_PACK ile ayni
struct PID_PACK
{
    UINT32 pid;
    WCHAR  name[1024];
};

// IOCTL_GET_MODULE_BASE - MemoryMgr.h _MODULE_PACK ile ayni
struct MODULE_PACK
{
    UINT32 pid;
    UINT64 baseAddress;
    SIZE_T size;
    WCHAR  moduleName[1024];
};

// IOCTL_BATCH_READ - MemoryMgr.h BatchReadRequest/BatchReadHeader ile ayni
struct BatchReadRequest
{
    UINT64 address;
    SIZE_T size;
    SIZE_T offset_in_buffer;
};

// IOCTL_MOUSE_CLICK
struct MouseClickRequest
{
    BOOLEAN press;   // TRUE = button down, FALSE = button up
};

struct BatchReadHeader
{
    HANDLE process_id;
    UINT32 num_requests;
    SIZE_T total_buffer_size;
    // Ardindan: BatchReadRequest dizisi, sonra output buffer
};
