#include "Mouse.h"
#include "../Core/MemoryMgr.h"

void mouse_click(bool press)
{
    // once kernel mouclass uzerinden dene
    if (memoryManager.MouseClick(press))
        return;

    // fallback: SendInput
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = press ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}
