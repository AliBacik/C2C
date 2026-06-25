#include "Radar.h"
#include <algorithm>

void Base_Radar::Draw(ImDrawList* draw,
                      const std::vector<RadarEntry>& players,
                      float local_x, float local_y, float local_yaw, int local_team,
                      float map_scale, float rx, float ry, float size)
{
    float cx   = rx + size * 0.5f;
    float cy   = ry + size * 0.5f;
    float half = size * 0.5f;

    // background
    draw->AddRectFilled({rx, ry}, {rx + size, ry + size},
                        IM_COL32(15, 15, 15, 80), 4.0f);
    draw->AddRect({rx, ry}, {rx + size, ry + size},
                  IM_COL32(80, 80, 80, 100), 4.0f, 0, 1.5f);

    // crosshair
    draw->AddLine({cx - 5, cy}, {cx + 5, cy}, IM_COL32(255, 255, 255, 60));
    draw->AddLine({cx, cy - 5}, {cx, cy + 5}, IM_COL32(255, 255, 255, 60));

    // range = same formula as new-cheat
    float range = (210.0f * map_scale) / 1.0f;
    float scale = half / range;

    float yaw_rad = (local_yaw - 90.0f) * 3.14159265f / 180.0f;
    float cos_y   = cosf(yaw_rad);
    float sin_y   = sinf(yaw_rad);

    draw->PushClipRect({rx, ry}, {rx + size, ry + size}, true);

    for (const auto& p : players) {
        if (!p.valid || p.health <= 0) continue;

        float dx = p.x - local_x;
        float dy = p.y - local_y;

        float rot_x =  dx * cos_y + dy * sin_y;
        float rot_y = -dx * sin_y + dy * cos_y;

        float px = cx + rot_x * scale;
        float py = cy - rot_y * scale;

        // clamp to radar bounds
        px = std::clamp(px, rx + 4.0f, rx + size - 4.0f);
        py = std::clamp(py, ry + 4.0f, ry + size - 4.0f);

        bool enemy = (p.team != local_team);
        ImU32 col  = enemy ? IM_COL32(237, 85, 106, 230) : IM_COL32(85, 170, 237, 230);

        // shadow + dot
        draw->AddCircleFilled({px + 1, py + 1}, 4.5f, IM_COL32(0, 0, 0, 80), 12);
        draw->AddCircleFilled({px, py},          4.5f, col,                    12);
        draw->AddCircle      ({px, py},          4.5f, IM_COL32(0, 0, 0, 120),12, 1.0f);
    }

    draw->PopClipRect();
}
