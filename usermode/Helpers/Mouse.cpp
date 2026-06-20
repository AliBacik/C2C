#include "Mouse.h"
#include "../Core/MemoryMgr.h"

void mouse_click(bool press)
{
    memoryManager.MouseClick(press);
}
