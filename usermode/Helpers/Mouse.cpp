#include "Mouse.h"
#include <winternl.h>
#include <stdio.h>

#pragma comment(lib, "ntdll.lib")

typedef struct _MOUSE_INPUT_DATA {
    USHORT UnitId;
    USHORT Flags;
    union {
        ULONG Buttons;
        struct {
            USHORT ButtonFlags;
            USHORT ButtonData;
        };
    };
    ULONG RawButtons;
    LONG LastX;
    LONG LastY;
    ULONG ExtraInformation;
} MOUSE_INPUT_DATA, * PMOUSE_INPUT_DATA;

typedef NTSTATUS(NTAPI* pNtDeviceIoControlFile_t)(
    HANDLE FileHandle, HANDLE Event, PVOID ApcRoutine, PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock, ULONG IoControlCode,
    PVOID InputBuffer, ULONG InputBufferLength,
    PVOID OutputBuffer, ULONG OutputBufferLength);

static pNtDeviceIoControlFile_t g_NtDeviceIoControlFile = nullptr;
static HANDLE g_mouseHandle = INVALID_HANDLE_VALUE;
static bool g_useHID = false;

#define IOCTL_MOUSE 0x2a2010

static HANDLE TryOpenMouse(const wchar_t* path)
{
    return CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, OPEN_EXISTING, 0, nullptr);
}

bool mouse_open()
{
    if (g_mouseHandle != INVALID_HANDLE_VALUE)
        return g_useHID;

    if (!g_NtDeviceIoControlFile)
    {
        HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
        if (ntdll)
            g_NtDeviceIoControlFile = (pNtDeviceIoControlFile_t)GetProcAddress(ntdll, "NtDeviceIoControlFile");
    }

    const wchar_t* paths[] = {
        L"\\\\?\\ROOT#SYSTEM#0002#{1abc05c0-c378-41b9-9cef-df1aba82b015}",
        L"\\\\?\\ROOT#SYSTEM#0001#{1abc05c0-c378-41b9-9cef-df1aba82b015}",
        L"\\\\?\\ROOT#SYSTEM#0003#{1abc05c0-c378-41b9-9cef-df1aba82b015}",
    };

    for (const auto& path : paths)
    {
        HANDLE h = TryOpenMouse(path);
        if (h != INVALID_HANDLE_VALUE)
        {
            g_mouseHandle = h;
            g_useHID = true;
            return true;
        }
    }

    g_useHID = false;
    return false;
}

void mouse_move(int button, int x, int y, int wheel)
{
    if (g_useHID && g_mouseHandle != INVALID_HANDLE_VALUE && g_NtDeviceIoControlFile)
    {
        MOUSE_INPUT_DATA mid = {};
        mid.LastX = x;
        mid.LastY = y;

        if (button == MOUSE_PRESS)
            mid.ButtonFlags = 0x0001;
        else if (button == MOUSE_RELEASE)
            mid.ButtonFlags = 0x0002;
        else if (x != 0 || y != 0)
            mid.Flags = 0x0001;

        if (wheel != 0)
        {
            mid.ButtonFlags = 0x0400;
            mid.ButtonData = (USHORT)(wheel * 120);
        }

        IO_STATUS_BLOCK isb = {};
        g_NtDeviceIoControlFile(g_mouseHandle, nullptr, nullptr, nullptr,
            &isb, IOCTL_MOUSE, &mid, sizeof(mid), nullptr, 0);
    }
    else
    {
        INPUT input = {};
        input.type = INPUT_MOUSE;

        if (button == MOUSE_PRESS)
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        else if (button == MOUSE_RELEASE)
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        else if (x != 0 || y != 0)
        {
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            input.mi.dx = x;
            input.mi.dy = y;
        }

        if (wheel != 0)
        {
            input.mi.dwFlags = MOUSEEVENTF_WHEEL;
            input.mi.mouseData = (DWORD)(wheel * 120);
        }

        SendInput(1, &input, sizeof(INPUT));
    }
}

void mouse_click(bool press)
{
    mouse_event(press ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
