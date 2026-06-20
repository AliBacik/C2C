#include "Mouse.h"

void mouse_click(bool press)
{
    mouse_event(press ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
