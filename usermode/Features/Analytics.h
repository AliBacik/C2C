#pragma once
#define NOMINMAX
#include <chrono>
#include <deque>
#include <string>
#include <cmath>
#include <unordered_map>
#include <vector>
#include "../Core/Config.h"
#include "../Game/Entity.h"
#include "../Offsets/Offsets.h"
#include "../Core/GlobalVars.h"
#include "../OS-ImGui/imgui/imgui.h"

namespace Analytics
{
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    static float g_score = 0.f;
    static TimePoint g_lastUpdate = Clock::now();

    // tracking korelasyonu
    struct TrackSample
    {
        float localYaw;
        float enemyYaw;
        TimePoint time;
    };
    static std::deque<TrackSample> g_trackSamples;
    static float g_trackingScore = 0.f;

    static int g_preFires = 0;
    static int g_totalTrackedShots = 0;

    // triggerbot atis zamanlari
    static std::deque<TimePoint> g_shotTimes;
    static TimePoint g_enemyVisibleSince{};
    static bool g_enemyWasVisible = false;

    // headshot orani
    static int g_totalShots = 0;
    static int g_headshotShots = 0;
    static float g_hsRatio = 0.f;
    // onceki ShotsFired degeri ile karsilastirmak icin
    static int g_prevShotsFired = 0;
    // headshot tespiti: crosshair pitch -5 ile 10 derece arasinda ve ates edildiyse headshot sayiyoruz
    // (kaba ama okuma tabanlı tek yol)

    // peek counter
    struct PeekEvent
    {
        TimePoint visibleAt;   // dusman gorunur oldu
        float reactionMs;      // ne kadar surede ates edildi
    };
    static std::deque<PeekEvent> g_peekEvents;
    static std::unordered_map<int, bool> g_enemyWasVisibleMap; // idx -> onceki frame gorunurlugu

    // pre-fire / wallbang detection
    struct ShotRecord
    {
        TimePoint shotAt;
        float angleToClosestEnemy; // atis aninda en yakin dusmanin acisi
        bool enemyOnCrosshair;
    };
    static std::deque<ShotRecord> g_shotRecords;
    static int g_preFires;           // atis aninda gorunur enemy yoktu ama sonra gorunur oldu
    static int g_totalTrackedShots;

    // ekran uyarisi
    static std::string g_warningText = "";
    static float g_warningAlpha = 0.f;
    static TimePoint g_warningShownAt{};

    static inline void AddScore(float amount)
    {
        g_score = (std::min)(100.f, g_score + amount);
    }

    static inline void DecayScore(float dt)
    {
        g_score = (std::max)(0.f, g_score - dt * 2.5f);
    }

    static inline void ShowWarning(const std::string& text)
    {
        g_warningText = text;
        g_warningAlpha = 1.f;
        g_warningShownAt = Clock::now();
    }

    static inline float CalcAngleToEnemy(const CEntity& local, const CEntity& enemy)
    {
        float dx = enemy.Pawn.Pos.x - local.Pawn.Pos.x;
        float dy = enemy.Pawn.Pos.y - local.Pawn.Pos.y;
        // CS2 yaw: east=0, north=90 (or -90 depending on convention)
        // ViewAngle.y is in -180..180 where 0=east, 90=south, -90=north
        // atan2(dy,dx) gives math angle, convert to CS2 yaw convention
        float angle = atan2f(dy, dx) * (180.f / 3.14159265f);
        // Result is in -180..180 matching ViewAngle.y range
        return angle;
    }

    static inline float AngleDiff(float a, float b)
    {
        float diff = fmodf(a - b + 360.f, 360.f);
        if (diff > 180.f) diff -= 360.f;
        return fabsf(diff);
    }

    static inline float CalcTrackingCorrelation()
    {
        if (g_trackSamples.size() < 6) return 0.f;

        float correlatedFrames = 0.f;
        float totalFrames = 0.f;

        for (size_t i = 1; i < g_trackSamples.size(); i++)
        {
            float dt = std::chrono::duration<float>(
                g_trackSamples[i].time - g_trackSamples[i - 1].time).count();
            if (dt <= 0.f) continue;

            float localDelta = g_trackSamples[i].localYaw - g_trackSamples[i - 1].localYaw;
            float enemyDelta = g_trackSamples[i].enemyYaw - g_trackSamples[i - 1].enemyYaw;

            if (fabsf(enemyDelta) > 0.5f)
            {
                totalFrames += 1.f;
                float ratio = (fabsf(enemyDelta) > 0.01f) ? fabsf(localDelta / enemyDelta) : 0.f;
                bool sameDirection = (localDelta * enemyDelta) > 0.f;
                if (sameDirection && ratio > 0.6f && ratio < 1.6f)
                    correlatedFrames += 1.f;
            }
        }

        if (totalFrames < 3.f) return 0.f;
        return correlatedFrames / totalFrames;
    }

    static inline bool IsEnemyVisible(int crosshairEntIndex,
        const std::vector<std::pair<int, CEntity>>& entities, int localTeam)
    {
        if (crosshairEntIndex <= 0) return false;
        for (const auto& [idx, entity] : entities)
        {
            if (!entity.IsAlive()) continue;
            if (entity.Controller.TeamID == localTeam) continue;
            if ((idx + 1) == crosshairEntIndex) return true;
        }
        return false;
    }

    static inline void Update(const CEntity& local,
        const std::vector<std::pair<int, CEntity>>& entities, bool triggerbotFired)
    {
        auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - g_lastUpdate).count();
        g_lastUpdate = now;

        DecayScore(dt);

        // uyari alpha decay
        float warnAge = std::chrono::duration<float>(now - g_warningShownAt).count();
        if (warnAge > 2.f)
            g_warningAlpha = (std::max)(0.f, g_warningAlpha - dt * 1.5f);

        if (!local.IsAlive()) return;

        int crosshairEntIndex = 0;
        memoryManager.ReadMemory<int>(local.Pawn.Address + Offset.Pawn.iIDEntIndex, crosshairEntIndex);

        bool enemyOnCrosshair = IsEnemyVisible(crosshairEntIndex, entities, local.Controller.TeamID);

        // --- Tracking korelasyonu ---
        // crosshair entity olmasa da (duvar arkasi) en yakin dusmaninı takip edip etmedigini kontrol et
        if (!enemyOnCrosshair)
        {
            const CEntity* closestEnemy = nullptr;
            float closestAngleDiff = 30.f;

            for (const auto& [idx, entity] : entities)
            {
                if (!entity.IsAlive()) continue;
                if (entity.Controller.TeamID == local.Controller.TeamID) continue;

                float angleToEnemy = CalcAngleToEnemy(local, entity);
                float diff = AngleDiff(local.Pawn.ViewAngle.y, angleToEnemy);
                if (diff < closestAngleDiff)
                {
                    closestAngleDiff = diff;
                    closestEnemy = &entity;
                }
            }

            if (closestEnemy)
            {
                float enemyYaw = CalcAngleToEnemy(local, *closestEnemy);
                g_trackSamples.push_back({ local.Pawn.ViewAngle.y, enemyYaw, now });
                if (g_trackSamples.size() > 20)
                    g_trackSamples.pop_front();

                float correlation = CalcTrackingCorrelation();
                g_trackingScore = g_trackingScore * 0.85f + correlation * 0.15f;

                if (g_trackingScore > 0.55f)
                {
                    AddScore(dt * 15.f);
                    if (g_warningAlpha < 0.1f)
                        ShowWarning("Wall tracking detected! Stop.");
                }
                else if (g_trackingScore > 0.35f)
                    AddScore(dt * 5.f);
            }
        }
        else
        {
            g_trackSamples.clear();
            g_trackingScore *= 0.9f;
        }

        // --- Pre-fire / Wallbang detection ---
        // Atis aninda crosshair'da enemy yoksa ama atis o yonde biri peek yapinca kisa sure sonra
        // gorunur olursa bu pre-fire / wall-track sayilir

        // Ates edildiyse kaydet
        if (triggerbotFired || (currentShotsFired > g_prevShotsFired && !enemyOnCrosshair))
        {
            ShotRecord rec;
            rec.shotAt = now;
            rec.enemyOnCrosshair = enemyOnCrosshair;
            rec.angleToClosestEnemy = 999.f;

            // En yakin dusmanin acisini kaydet
            for (const auto& [idx, entity] : entities)
            {
                if (!entity.IsAlive()) continue;
                if (entity.Controller.TeamID == local.Controller.TeamID) continue;
                float a = AngleDiff(local.Pawn.ViewAngle.y, CalcAngleToEnemy(local, entity));
                if (a < rec.angleToClosestEnemy)
                    rec.angleToClosestEnemy = a;
            }

            g_shotRecords.push_back(rec);
            if (g_shotRecords.size() > 30)
                g_shotRecords.pop_front();

            if (!enemyOnCrosshair)
                g_totalTrackedShots++;
        }

        // --- Peek counter ---
        // her dusman icin gorunurluk degisimini takip et
        for (const auto& [idx, entity] : entities)
        {
            if (!entity.IsAlive()) continue;
            if (entity.Controller.TeamID == local.Controller.TeamID) continue;

            bool isVisibleNow = entity.Pawn.bSpottedByMask > 0;
            bool wasVisible = g_enemyWasVisibleMap.count(idx) ? g_enemyWasVisibleMap[idx] : false;

            if (isVisibleNow && !wasVisible)
            {
                // Dusman yeni gorunur oldu: son 350ms icinde bu yonde atis yapilmis mi?
                float enemyAngle = CalcAngleToEnemy(local, entity);
                for (auto& rec : g_shotRecords)
                {
                    if (rec.enemyOnCrosshair) continue; // crosshair'da enemy vardi zaten
                    float msAgo = std::chrono::duration<float>(now - rec.shotAt).count() * 1000.f;
                    if (msAgo < 0.f || msAgo > 350.f) continue;
                    float angleDiff = AngleDiff(local.Pawn.ViewAngle.y, enemyAngle);
                    // atis aninda bu dusmanin yonunde (30 derece tolerans) ates edildiyse pre-fire
                    if (rec.angleToClosestEnemy < 30.f)
                    {
                        g_preFires++;
                        AddScore(18.f);
                        if (g_warningAlpha < 0.1f)
                            ShowWarning("Pre-fire detected! Be less obvious.");
                        break;
                    }
                }

                // peek basladi
                PeekEvent ev;
                ev.visibleAt = now;
                ev.reactionMs = -1.f;
                g_peekEvents.push_back(ev);
                if (g_peekEvents.size() > 20)
                    g_peekEvents.pop_front();
            }

            g_enemyWasVisibleMap[idx] = isVisibleNow;
        }

        // peek sonrasi ates edildiyse reaksiyon suresini kaydet
        if (triggerbotFired && !g_peekEvents.empty())
        {
            auto& lastPeek = g_peekEvents.back();
            if (lastPeek.reactionMs < 0.f)
            {
                lastPeek.reactionMs = std::chrono::duration<float>(now - lastPeek.visibleAt).count() * 1000.f;

                // peek sonrasi 150ms altinda ates = cok hizli
                if (lastPeek.reactionMs < 150.f)
                {
                    AddScore(12.f);
                    if (lastPeek.reactionMs < 80.f && g_warningAlpha < 0.1f)
                        ShowWarning("Reaction too fast! Slow down.");
                }

                // son 5 peek'te ortalama reaksiyon
                if (g_peekEvents.size() >= 5)
                {
                    float totalReaction = 0.f;
                    int counted = 0;
                    for (auto& ev : g_peekEvents)
                    {
                        if (ev.reactionMs > 0.f)
                        {
                            totalReaction += ev.reactionMs;
                            counted++;
                        }
                    }
                    if (counted > 0)
                    {
                        float avgReaction = totalReaction / counted;
                        if (avgReaction < 120.f)
                            AddScore(8.f);
                    }
                }
            }
        }

        // --- Headshot orani ---
        // Sadece crosshair'da enemy varken sayiyoruz
        int currentShotsFired = local.Pawn.ShotsFired;
        if (currentShotsFired > g_prevShotsFired && enemyOnCrosshair)
        {
            int newShots = currentShotsFired - g_prevShotsFired;
            g_totalShots += newShots;

            // pitch -5 ile 3 derece arasindaysa kafa hizasinda sayiyoruz
            float pitch = local.Pawn.ViewAngle.x;
            if (pitch > -5.f && pitch < 3.f)
                g_headshotShots += newShots;

            if (g_totalShots >= 10)
            {
                g_hsRatio = (float)g_headshotShots / (float)g_totalShots;

                // %85+ hs orani cok yuksek
                if (g_hsRatio > 0.85f)
                {
                    AddScore(dt * 10.f);
                    if (g_warningAlpha < 0.1f)
                        ShowWarning("High headshot ratio! Aim lower.");
                }
            }
        }
        g_prevShotsFired = currentShotsFired;

        // --- TriggerBot tutarlilik ---
        if (triggerbotFired)
        {
            g_shotTimes.push_back(now);
            if (g_shotTimes.size() > 10)
                g_shotTimes.pop_front();

            if (g_shotTimes.size() >= 5)
            {
                std::vector<float> intervals;
                for (size_t i = 1; i < g_shotTimes.size(); i++)
                {
                    float interval = std::chrono::duration<float>(
                        g_shotTimes[i] - g_shotTimes[i - 1]).count();
                    intervals.push_back(interval);
                }
                float mean = 0.f;
                for (float v : intervals) mean += v;
                mean /= intervals.size();

                float variance = 0.f;
                for (float v : intervals) variance += (v - mean) * (v - mean);
                variance /= intervals.size();

                if (variance < 0.001f)
                    AddScore(5.f);
            }
        }

        // genel score uyarisi
        if (g_score >= 67.f && g_warningAlpha < 0.1f)
            ShowWarning("Too obvious! Play more carefully.");
        else if (g_score >= 34.f && g_warningAlpha < 0.1f)
            ShowWarning("Be more careful.");
    }

    static inline void Render()
    {
        if (!AnalyticsCFG::Enabled) return;

        // --- Ekran ortasi buyuk uyari ---
        if (g_warningAlpha > 0.01f && !g_warningText.empty())
        {
            ImDrawList* bgDraw = ImGui::GetBackgroundDrawList();
            ImVec2 screen = ImGui::GetIO().DisplaySize;

            ImVec2 textSize = ImGui::CalcTextSize(g_warningText.c_str());
            float padding = 12.f;
            float x = (screen.x - textSize.x) * 0.5f;
            float y = screen.y * 0.15f;

            ImColor bgCol = ImColor(0, 0, 0, (int)(180 * g_warningAlpha));
            ImColor textCol;
            if (g_score >= 67.f)
                textCol = ImColor(220, 60, 60, (int)(255 * g_warningAlpha));
            else
                textCol = ImColor(220, 180, 50, (int)(255 * g_warningAlpha));

            bgDraw->AddRectFilled(
                ImVec2(x - padding, y - padding),
                ImVec2(x + textSize.x + padding, y + textSize.y + padding),
                bgCol, 6.f);
            bgDraw->AddText(ImVec2(x, y), textCol, g_warningText.c_str());
        }

        // --- Kucuk panel ---
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        ImGui::SetNextWindowBgAlpha(0.75f);
        ImGui::SetNextWindowPos(AnalyticsCFG::WinPos, ImGuiCond_Once);
        ImGui::Begin("##Analytics", nullptr, flags);
        AnalyticsCFG::WinPos = ImGui::GetWindowPos();

        ImColor barColor;
        const char* statusText;
        if (g_score < 34.f)
        {
            barColor = ImColor(80, 200, 80, 255);
            statusText = "Legit";
        }
        else if (g_score < 67.f)
        {
            barColor = ImColor(220, 180, 50, 255);
            statusText = "Caution";
        }
        else
        {
            barColor = ImColor(220, 60, 60, 255);
            statusText = "Obvious!";
        }

        ImGui::TextDisabled("Behavior  ");
        ImGui::SameLine();
        ImGui::TextColored(barColor.Value, "%s", statusText);

        // ana bar
        ImVec2 barPos = ImGui::GetCursorScreenPos();
        float barWidth = 160.f;
        float barHeight = 6.f;
        float fillWidth = barWidth * (g_score / 100.f);
        ImDrawList* draw = ImGui::GetWindowDrawList();
        draw->AddRectFilled(barPos, ImVec2(barPos.x + barWidth, barPos.y + barHeight), ImColor(40, 40, 40, 200), 3.f);
        draw->AddRectFilled(barPos, ImVec2(barPos.x + fillWidth, barPos.y + barHeight), barColor, 3.f);
        ImGui::Dummy(ImVec2(barWidth, barHeight + 6.f));

        // hs orani
        if (g_totalShots >= 10)
        {
            char hsText[32];
            snprintf(hsText, sizeof(hsText), "HS: %.0f%%", g_hsRatio * 100.f);
            ImColor hsColor = g_hsRatio > 0.85f ? ImColor(220, 60, 60, 255) :
                              g_hsRatio > 0.65f ? ImColor(220, 180, 50, 255) :
                                                  ImColor(160, 160, 160, 255);
            ImGui::TextColored(hsColor.Value, "%s", hsText);
        }

        // peek ortalama reaksiyon
        if (g_peekEvents.size() >= 3)
        {
            float total = 0.f; int cnt = 0;
            for (auto& ev : g_peekEvents)
                if (ev.reactionMs > 0.f) { total += ev.reactionMs; cnt++; }
            if (cnt > 0)
            {
                float avg = total / cnt;
                char peekText[32];
                snprintf(peekText, sizeof(peekText), "React: %.0fms", avg);
                ImColor peekColor = avg < 120.f ? ImColor(220, 60, 60, 255) :
                                    avg < 200.f ? ImColor(220, 180, 50, 255) :
                                                  ImColor(160, 160, 160, 255);
                ImGui::TextColored(peekColor.Value, "%s", peekText);
            }
        }

        // tracking uyarisi
        if (g_trackingScore > 0.5f)
            ImGui::TextColored(ImColor(220, 60, 60, 255).Value, "! Wall tracking");

        // pre-fire sayaci
        if (g_totalTrackedShots >= 3)
        {
            float preFireRate = (float)g_preFires / (float)g_totalTrackedShots;
            char pfText[32];
            snprintf(pfText, sizeof(pfText), "Pre-fire: %d", g_preFires);
            ImColor pfColor = g_preFires >= 3 ? ImColor(220, 60, 60, 255) :
                              g_preFires >= 1 ? ImColor(220, 180, 50, 255) :
                                                ImColor(160, 160, 160, 255);
            ImGui::TextColored(pfColor.Value, "%s", pfText);
        }

        ImGui::End();
    }
}
