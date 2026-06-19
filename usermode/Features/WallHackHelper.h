#pragma once
#define NOMINMAX
#include <chrono>
#include <unordered_map>
#include "../Core/Config.h"
#include "../Game/Entity.h"
#include "../OS-ImGui/imgui/imgui.h"

namespace WallHackHelper
{
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    // her dusman icin gorunur oldugu an
    static std::unordered_map<DWORD64, TimePoint> g_visibleSince;
    static std::unordered_map<DWORD64, bool> g_wasVisible;
    // gorunur olmadan once crosshair uzerindeydi mi (pre-aim)
    static std::unordered_map<DWORD64, bool> g_wasPreAimed;
    // hangi delay uygulanacak
    static std::unordered_map<DWORD64, int> g_effectiveDelay;

    static inline void Update(const std::vector<std::pair<int, CEntity>>& entities,
        int localTeam, int crosshairEntIndex)
    {
        if (!WallHackHelperCFG::Enabled) return;

        auto now = Clock::now();

        for (const auto& [idx, entity] : entities)
        {
            if (!entity.IsAlive()) continue;
            if (entity.Controller.TeamID == localTeam) continue;

            DWORD64 addr = entity.Controller.Address;
            bool isVisible = entity.Pawn.bSpottedByMask > 0;
            bool wasVis = g_wasVisible.count(addr) ? g_wasVisible[addr] : false;

            if (isVisible && !wasVis)
            {
                // yeni gorunur oldu
                g_visibleSince[addr] = now;

                // gorunmeden once crosshair bu dusmana bakiyordu mu?
                bool preAimed = g_wasPreAimed.count(addr) ? g_wasPreAimed[addr] : false;
                g_effectiveDelay[addr] = preAimed
                    ? WallHackHelperCFG::PreAimDelayMs   // zaten nisanliydi = kisa sure
                    : WallHackHelperCFG::SafeDelayMs;    // cold peek = uzun sure
            }
            else if (!isVisible)
            {
                g_visibleSince.erase(addr);
                g_effectiveDelay.erase(addr);
            }

            // crosshair su an bu dusmana bakiyor mu kaydet (sonraki frame icin)
            bool onCrosshair = (crosshairEntIndex > 0 && (idx + 1) == crosshairEntIndex);
            g_wasPreAimed[addr] = onCrosshair;
            g_wasVisible[addr] = isVisible;
        }

        // artik olmayan dusmanlari temizle
        for (auto it = g_visibleSince.begin(); it != g_visibleSince.end();)
        {
            bool found = false;
            for (const auto& [idx, entity] : entities)
                if (entity.Controller.Address == it->first) { found = true; break; }
            if (!found) it = g_visibleSince.erase(it);
            else ++it;
        }
    }

    // ESP'de kullanilacak rengi dondur
    // helper aciksa: gorunmez/yeni gorunur = kirmizi, SafeDelayMs gectiyse = yesil
    static inline ImColor GetBoneColor(DWORD64 controllerAddr)
    {
        auto it = g_visibleSince.find(controllerAddr);
        if (it == g_visibleSince.end())
            return WallHackHelperCFG::UnsafeColor; // hic gorunmedi veya gorunmez = kirmizi

        auto now = Clock::now();
        float elapsedMs = std::chrono::duration<float, std::milli>(now - it->second).count();

        int delay = g_effectiveDelay.count(controllerAddr)
            ? g_effectiveDelay[controllerAddr]
            : WallHackHelperCFG::SafeDelayMs;

        if (elapsedMs >= (float)delay)
            return WallHackHelperCFG::SafeColor;

        float t = elapsedMs / (float)delay;
        ImVec4 unsafe = WallHackHelperCFG::UnsafeColor.Value;
        ImVec4 safe   = WallHackHelperCFG::SafeColor.Value;
        return ImColor(
            unsafe.x + (safe.x - unsafe.x) * t,
            unsafe.y + (safe.y - unsafe.y) * t,
            unsafe.z + (safe.z - unsafe.z) * t,
            unsafe.w
        );
    }

    static inline ImColor GetHeadColor(DWORD64 controllerAddr)
    {
        return GetBoneColor(controllerAddr);
    }

    // kac ms kaldi goster (UI icin)
    static inline float GetRemainingMs(DWORD64 controllerAddr)
    {
        auto it = g_visibleSince.find(controllerAddr);
        if (it == g_visibleSince.end()) return -1.f;

        auto now = Clock::now();
        float elapsed = std::chrono::duration<float, std::milli>(now - it->second).count();
        int delay = g_effectiveDelay.count(controllerAddr)
            ? g_effectiveDelay[controllerAddr]
            : WallHackHelperCFG::SafeDelayMs;
        return (std::max)(0.f, (float)delay - elapsed);
    }
}
