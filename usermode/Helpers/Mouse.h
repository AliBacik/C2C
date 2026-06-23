#pragma once
#ifndef MOUSE_H
#define MOUSE_H

#include <Windows.h>

#define MOUSE_PRESS   1
#define MOUSE_RELEASE 2

bool mouse_open();
void mouse_move(int button, int x, int y, int wheel);

#endif
