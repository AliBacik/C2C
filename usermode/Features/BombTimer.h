#pragma once
#include "../Core/Config.h"
#include "../Core/MemoryMgr.h"
#include "../Offsets/Offsets.h"
#include "../Game/Game.h"
#include "../OS-ImGui/imgui/imgui.h"

namespace BombTimer
{
    static inline void Render()
    {
        if (!MiscCFG::bmbTimer) return;

        DWORD64 clientBase = gGame.GetClientDLLAddress();
        if (!clientBase) return;

        DWORD64 plantedC4ListPtr = 0;
        if (!memoryManager.ReadMemory<DWORD64>(clientBase + Offset.PlantedC4, plantedC4ListPtr))
            return;

        DWORD64 plantedC4 = 0;
        if (!memoryManager.ReadMemory<DWORD64>(plantedC4ListPtr, plantedC4))
            return;

        if (!plantedC4) return;

        float blowTime = 0.f, timerLength = 0.f, defuseCountDown = 0.f;
        bool beingDefused = false;
        int bombSite = 0;

        memoryManager.ReadMemory<float>(plantedC4 + Offset.C4.m_flC4Blow, blowTime);
        memoryManager.ReadMemory<float>(plantedC4 + Offset.C4.m_flTimerLength, timerLength);
        memoryManager.ReadMemory<float>(plantedC4 + Offset.C4.m_flDefuseCountDown, defuseCountDown);
        memoryManager.ReadMemory<bool>(plantedC4 + Offset.C4.m_bBeingDefused, beingDefused);
        memoryManager.ReadMemory<int>(plantedC4 + Offset.C4.m_nBombSite, bombSite);

        if (timerLength <= 0.f) return;

        // kalan sure = patlama zamani - simdi (game time)
        // biz simdi yerine timerLength uzerinden oranli hesapliyoruz
        // blowTime - currentTime icin global vars lazim, oran kullanalim
        float remainRatio = blowTime > 0.f ? (blowTime - timerLength) / timerLength : 0.f;
        // daha saglam yontem: defuse countdown dan kalan sure
        float timeLeft = defuseCountDown > 0.f ? defuseCountDown : timerLength;

        // clamp
        if (timeLeft < 0.f) timeLeft = 0.f;
        if (timeLeft > timerLength) timeLeft = timerLength;

        const char* site = (bombSite == 0) ? "A" : "B";

        ImVec2 screen = ImGui::GetIO().DisplaySize;
        const float panelW = 180.f;
        const float panelH = beingDefused ? 66.f : 52.f;
        const float margin  = 20.f;
        ImVec2 panelPos(margin, screen.y - panelH - margin);

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

        ImGui::SetNextWindowPos(panelPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(panelW, panelH));
        ImGui::SetNextWindowBgAlpha(0.72f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 8.f));
        ImGui::Begin("##bombtimer", nullptr, flags);
        {
            ImColor timeColor;
            if (timeLeft <= 5.f)
                timeColor = ImColor(220, 60, 60, 255);
            else if (timeLeft <= 10.f)
                timeColor = ImColor(220, 180, 60, 255);
            else
                timeColor = ImColor(80, 200, 80, 255);

            ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.7f), "BOMB");
            ImGui::SameLine();
            ImGui::TextColored(timeColor, "%s  %.1fs", site, timeLeft);

            float progress = timerLength > 0.f ? timeLeft / timerLength : 0.f;
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)timeColor);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 0.9f));
            ImGui::ProgressBar(progress, ImVec2(-1.f, 5.f), "");
            ImGui::PopStyleColor(2);

            if (beingDefused)
                ImGui::TextColored(ImColor(100, 180, 255, 255), "DEFUSING...");
        }
        ImGui::End();
        ImGui::PopStyleVar(2);
    }
}
