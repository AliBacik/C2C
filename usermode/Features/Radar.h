#pragma once
#define _USE_MATH_DEFINES
#include "../Game/Game.h"
#include <math.h>
#include <vector>
#include <string>

struct RadarEntry {
    float x, y;
    int   team;
    int   health;
    bool  valid;
};

class Base_Radar
{
public:
    void Draw(ImDrawList* draw,
              const std::vector<RadarEntry>& players,
              float local_x, float local_y, float local_yaw, int local_team,
              float map_scale, float rx, float ry, float size);
};
