#pragma once
#include "..\Core\Config.h"
#include "..\Core\Render.h"
#include "..\Features\Radar.h"
#include "..\Config\ConfigMenu.h"
#include "..\Config\ConfigSaver.h"

#include "..\Resources\Language.h"
#include "..\Resources\Images.h"
#include "..\Helpers\KeyManager.h"

#include "../Features/ESP.h"

// accent: #DC4838
static constexpr ImU32  kAccent      = IM_COL32(220,  72,  56, 255);
static constexpr ImU32  kAccentDim   = IM_COL32(220,  72,  56, 120);
static constexpr ImU32  kAccentTrans = IM_COL32(220,  72,  56,   0);
static constexpr ImU32  kSurface1    = IM_COL32( 14,  14,  16, 255);
static constexpr ImU32  kSurface2    = IM_COL32( 18,  18,  21, 255);
static constexpr ImU32  kDivider     = IM_COL32( 32,  32,  36, 255);

namespace GUI
{
    void LoadDefaultConfig()
    {
        if (!MenuConfig::defaultConfig)
            return;
        MyConfigSaver::LoadConfig("default.cfg");
        MenuConfig::defaultConfig = false;
    }

    // ── helpers ──────────────────────────────────────────────────────────────

    void AlignRight(float ContentWidth)
    {
        float col = ImGui::GetColumnWidth() - ImGui::GetStyle().ItemSpacing.x;
        ImGui::SetCursorPosX(ImGui::GetColumnOffset() + col - ContentWidth);
    }

    void PutSwitch(const char* string, float CursorX, float ContentWidth, bool* v,
                   bool ColorEditor = false, const char* lable = NULL, float col[4] = NULL, const char* Tip = NULL)
    {
        ImGui::PushID(string);
        float cx = ImGui::GetCursorPosX();
        float cy = ImGui::GetCursorPosY();
        ImGui::SetCursorPosX(cx + CursorX);
        ImGui::TextDisabled(string);
        if (Tip && ImGui::IsItemHovered())
            ImGui::SetTooltip(Tip);
        ImGui::SameLine();
        ImGui::SetCursorPosY(cy - 2);
        if (ColorEditor) {
            AlignRight(ContentWidth + ImGui::GetFrameHeight() + 7);
            ImGui::ColorEdit4(lable, col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
            ImGui::SameLine();
        } else {
            AlignRight(ContentWidth);
        }
        Gui.SwitchButton(string, v);
        ImGui::PopID();
    }

    void PutColorEditor(const char* text, const char* lable, float CursorX, float ContentWidth, float col[4], const char* Tip = NULL)
    {
        ImGui::PushID(text);
        float cx = ImGui::GetCursorPosX();
        ImGui::SetCursorPosX(cx + CursorX);
        ImGui::TextDisabled(text);
        if (Tip && ImGui::IsItemHovered())
            ImGui::SetTooltip(Tip);
        ImGui::SameLine();
        AlignRight(ContentWidth + ImGui::GetFrameHeight() + 8);
        ImGui::ColorEdit4(lable, col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
        ImGui::PopID();
    }

    void PutSliderFloat(const char* string, float CursorX, float* v, const void* p_min, const void* p_max, const char* format, const char* Tip = NULL)
    {
        ImGui::PushID(string);
        float cx = ImGui::GetCursorPosX();
        float sliderW = ImGui::GetColumnWidth() - ImGui::GetStyle().ItemSpacing.x - CursorX - 15;
        ImGui::SetCursorPosX(cx + CursorX);
        ImGui::TextDisabled(string);
        if (Tip && ImGui::IsItemHovered())
            ImGui::SetTooltip(Tip);
        ImGui::SameLine();
        ImGui::TextDisabled(format, *v);
        ImGui::SetCursorPosX(cx + CursorX);
        ImGui::SetNextItemWidth(sliderW);
        Gui.SliderScalarEx2("", ImGuiDataType_Float, v, p_min, p_max, "", ImGuiSliderFlags_None);
        ImGui::PopID();
    }

    void PutSliderInt(const char* string, float CursorX, int* v, const void* p_min, const void* p_max, const char* format, const char* Tip = NULL)
    {
        ImGui::PushID(string);
        float cx = ImGui::GetCursorPosX();
        float sliderW = ImGui::GetColumnWidth() - ImGui::GetStyle().ItemSpacing.x - CursorX - 15;
        ImGui::SetCursorPosX(cx + CursorX);
        ImGui::TextDisabled(string);
        if (Tip && ImGui::IsItemHovered())
            ImGui::SetTooltip(Tip);
        ImGui::SameLine();
        ImGui::TextDisabled(format, *v);
        ImGui::SetCursorPosX(cx + CursorX);
        ImGui::SetNextItemWidth(sliderW);
        Gui.SliderScalarEx2("", ImGuiDataType_Float, v, p_min, p_max, "", ImGuiSliderFlags_None);
        ImGui::PopID();
    }

    // ── sidebar tab button ────────────────────────────────────────────────────

    bool SideTab(const char* label, bool active, float w, float h)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::InvisibleButton(label, ImVec2(w, h));
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 p2 = ImVec2(pos.x + w, pos.y + h);

        // background
        ImU32 bg = active  ? IM_COL32(22, 22, 26, 255)
                 : hovered ? IM_COL32(18, 18, 21, 255)
                           : IM_COL32(14, 14, 16, 255);
        dl->AddRectFilled(pos, p2, bg);

        if (active) {
            // left accent bar
            dl->AddRectFilled(pos, ImVec2(pos.x + 3, p2.y), kAccent);
            // subtle right glow line
            dl->AddRectFilled(ImVec2(p2.x - 1, pos.y), p2, kAccentDim);
        } else if (hovered) {
            dl->AddRectFilled(pos, ImVec2(pos.x + 2, p2.y), kAccentDim);
        }

        // label — centered
        ImVec2 ts = ImGui::CalcTextSize(label);
        ImVec2 tc = ImVec2(pos.x + (w - ts.x) * 0.5f, pos.y + (h - ts.y) * 0.5f);
        ImU32 textCol = active  ? kAccent
                      : hovered ? IM_COL32(200, 200, 204, 255)
                                : IM_COL32(100, 100, 110, 255);
        dl->AddText(tc, textCol, label);

        return clicked;
    }

    // ── main draw ─────────────────────────────────────────────────────────────

    void DrawGui()
    {
        constexpr float SIDEBAR_W = 54.f;
        constexpr float WIN_W     = 620.f;
        constexpr float WIN_H     = 500.f;
        constexpr float TAB_H     = 46.f;
        constexpr float CONTENT_W = WIN_W - SIDEBAR_W;

        MenuConfig::WCS.MainWinSize = ImVec2(WIN_W, WIN_H);

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                               | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

        ImGui::SetNextWindowPos(
            { (ImGui::GetIO().DisplaySize.x - WIN_W) * 0.5f,
              (ImGui::GetIO().DisplaySize.y - WIN_H) * 0.5f },
            ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(WIN_W, WIN_H));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,  ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg,  ImVec4(0.055f, 0.055f, 0.062f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_Border,    ImVec4(0.14f,  0.14f,  0.16f,  1.f));

        ImGui::Begin("C2C##main", nullptr, flags);

        ImDrawList* dl  = ImGui::GetWindowDrawList();
        ImVec2      wP  = ImGui::GetWindowPos();

        // ── sidebar ──────────────────────────────────────────────────────────
        // fill sidebar bg
        dl->AddRectFilled(wP, ImVec2(wP.x + SIDEBAR_W, wP.y + WIN_H), kSurface1, 8.f, ImDrawFlags_RoundCornersLeft);
        // right divider
        dl->AddLine(ImVec2(wP.x + SIDEBAR_W, wP.y + 1), ImVec2(wP.x + SIDEBAR_W, wP.y + WIN_H - 1), kDivider, 1.f);

        // logo area — top of sidebar
        {
            float lx = wP.x + SIDEBAR_W * 0.5f;
            float ly = wP.y + 22.f;
            dl->AddCircleFilled(ImVec2(lx, ly), 16.f, kAccent, 32);
            dl->AddCircleFilled(ImVec2(lx, ly), 10.f, kSurface1, 32);
            // inner accent ring
            dl->PathArcTo(ImVec2(lx, ly), 6.f, 0.6f * IM_PI, 2.4f * IM_PI, 20);
            dl->PathStroke(kAccent, false, 2.f);
        }

        ImGui::SetCursorPos(ImVec2(0, 44.f));

        // Visual tab
        if (SideTab("ESP", MenuConfig::WCS.MenuPage == 0, SIDEBAR_W, TAB_H))
            MenuConfig::WCS.MenuPage = 0;

        // Settings tab
        if (SideTab("CFG", MenuConfig::WCS.MenuPage == 1, SIDEBAR_W, TAB_H))
            MenuConfig::WCS.MenuPage = 1;

        // bottom version tag
        {
            const char* ver = "1.0";
            ImVec2 vs = ImGui::CalcTextSize(ver);
            dl->AddText(
                ImVec2(wP.x + (SIDEBAR_W - vs.x) * 0.5f, wP.y + WIN_H - vs.y - 8.f),
                IM_COL32(60, 60, 68, 255), ver);
        }

        // ── content panel ────────────────────────────────────────────────────
        ImGui::SetCursorPos(ImVec2(SIDEBAR_W, 0));

        // top header bar
        {
            dl->AddRectFilled(
                ImVec2(wP.x + SIDEBAR_W, wP.y),
                ImVec2(wP.x + WIN_W,     wP.y + 32.f),
                kSurface2);
            dl->AddLine(
                ImVec2(wP.x + SIDEBAR_W, wP.y + 32.f),
                ImVec2(wP.x + WIN_W,     wP.y + 32.f),
                kDivider, 1.f);

            // page label
            const char* pageLabel = MenuConfig::WCS.MenuPage == 0 ? "Visual / ESP" : "Settings / Config";
            dl->AddText(ImVec2(wP.x + SIDEBAR_W + 14.f, wP.y + 8.f), IM_COL32(200, 200, 204, 255), pageLabel);

            // top-right accent dot row
            float dotX = wP.x + WIN_W - 14.f;
            float dotY = wP.y + 16.f;
            dl->AddCircleFilled(ImVec2(dotX,        dotY), 4.f, kAccent, 12);
            dl->AddCircleFilled(ImVec2(dotX - 12.f, dotY), 4.f, kAccentDim, 12);
            dl->AddCircleFilled(ImVec2(dotX - 24.f, dotY), 4.f, kDivider, 12);
        }

        ImGui::SetCursorPos(ImVec2(SIDEBAR_W + 2.f, 34.f));

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.071f, 0.071f, 0.082f, 1.f));
        ImGui::BeginChild("##page", ImVec2(CONTENT_W - 2.f, WIN_H - 34.f), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        // ── PAGE 0: Visual / ESP ─────────────────────────────────────────────
        if (MenuConfig::WCS.MenuPage == 0)
        {
            ImGui::Columns(2, nullptr, false);
            ImGui::SetColumnWidth(0, 270.f);

            // ── Left: ESP ────────────────────────────────────────────────────
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6.f);
            ImGui::GradientText("ESP - Skeleton");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.f);

            PutSwitch(Text::ESP::Enable.c_str(), 10.f, ImGui::GetFrameHeight() * 1.7f, &ESPConfig::ESPenabled);
            if (ESPConfig::ESPenabled)
            {
                PutSwitch(Text::ESP::Skeleton.c_str(),    10.f, ImGui::GetFrameHeight() * 1.7f, &ESPConfig::ShowBoneESP,
                          true, "###BoneCol", reinterpret_cast<float*>(&ESPConfig::BoneColor));
                PutSwitch(Text::ESP::HeadBox.c_str(),     10.f, ImGui::GetFrameHeight() * 1.7f, &ESPConfig::ShowHeadBox,
                          true, "###HeadBoxCol", reinterpret_cast<float*>(&ESPConfig::HeadBoxColor));
                PutSwitch("Health Bar",      10.f, ImGui::GetFrameHeight() * 1.7f, &ESPConfig::ShowHealthBar);
                PutSwitch("View Direction",  10.f, ImGui::GetFrameHeight() * 1.7f, &ESPConfig::ShowEyeRay);
                PutSwitch("Offscreen Arrow", 10.f, ImGui::GetFrameHeight() * 1.7f, &ESPConfig::ShowOutOfFOVArrow,
                          true, "###ArrowCol", reinterpret_cast<float*>(&ESPConfig::OutOfFOVArrowColor));
            }

            // ── Right: Preview + Radar ───────────────────────────────────────
            ImGui::NextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.f);
            ImGui::GradientText("Preview");
            ESP::RenderPreview({ ImGui::GetColumnWidth(), ImGui::GetCursorPosY() });
            ImGui::Dummy({ 0.f, ImGui::GetFrameHeight() * 9 });

            ImGui::GradientText("Radar");
            static const float RadarPointSizeProportionMin = 0.2f, RadarPointSizeProportionMax = 2.f;
            static const float ProportionMin = 500.f, ProportionMax = 15000.f;
            static const float RadarRangeMin = 100.f, RadarRangeMax = 300.f;
            static const float AlphaMin = 0.f, AlphaMax = 1.f;
            PutSwitch(Text::Radar::Toggle.c_str(), 5.f, ImGui::GetFrameHeight() * 1.7f, &RadarCFG::ShowRadar);
            if (RadarCFG::ShowRadar)
            {
                PutSwitch(Text::Radar::CustomCheck.c_str(), 5.f, ImGui::GetFrameHeight() * 1.7f, &RadarCFG::customRadar);
                if (RadarCFG::customRadar)
                {
                    PutSwitch(Text::Radar::CrossLine.c_str(), 5.f, ImGui::GetFrameHeight() * 1.7f, &RadarCFG::ShowRadarCrossLine);
                    PutSliderFloat(Text::Radar::SizeSlider.c_str(),       5.f, &RadarCFG::RadarPointSizeProportion, &RadarPointSizeProportionMin, &RadarPointSizeProportionMax, "%.1f");
                    PutSliderFloat(Text::Radar::ProportionSlider.c_str(), 5.f, &RadarCFG::Proportion, &ProportionMin, &ProportionMax, "%.1f");
                    PutSliderFloat(Text::Radar::RangeSlider.c_str(),      5.f, &RadarCFG::RadarRange, &RadarRangeMin, &RadarRangeMax, "%.1f");
                    PutSliderFloat(Text::Radar::AlphaSlider.c_str(),      5.f, &RadarCFG::RadarBgAlpha, &AlphaMin, &AlphaMax, "%.1f");
                }
            }

            ImGui::Columns(1);
        }

        // ── PAGE 1: Settings / Config ─────────────────────────────────────────
        if (MenuConfig::WCS.MenuPage == 1)
        {
            float childH  = WIN_H - 34.f - ImGui::GetCursorPosY() - 8.f;
            float leftW   = 265.f;
            float rightW  = CONTENT_W - leftW - 14.f;

            // ── Left child ───────────────────────────────────────────────────
            ImGui::BeginChild("##cfg_left", ImVec2(leftW, childH), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::GradientText("Trigger Bot");
                PutSwitch("Enable Trigger Bot", 5.f, ImGui::GetFrameHeight() * 1.7f, &TriggerBotCFG::Enabled);
                if (TriggerBotCFG::Enabled)
                {
                    static const int DelayMin = 10, DelayMax = 500;
                    PutSliderInt("Delay (ms)", 5.f, &TriggerBotCFG::Delay, &DelayMin, &DelayMax, "%d ms");
                    PutSwitch("Head Only",     5.f, ImGui::GetFrameHeight() * 1.7f, &TriggerBotCFG::HeadOnly);
                    PutSwitch("Always Active", 5.f, ImGui::GetFrameHeight() * 1.7f, &TriggerBotCFG::AlwaysActive);

                    if (!TriggerBotCFG::AlwaysActive)
                    {
                        static std::string tbKeyName = KeyMgr::GetKeyName(TriggerBotCFG::HotKey);
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5.f);
                        ImGui::TextDisabled("Hotkey: %s", tbKeyName.c_str());
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(leftW - 75.f);
                        if (ImGui::Button("Set##tb", { 70.f, 24.f }))
                        {
                            std::thread([&]() {
                                KeyMgr::GetPressedKey(TriggerBotCFG::HotKey, &tbKeyName);
                            }).detach();
                        }
                    }
                }

                ImGui::NewLine();
                ImGui::GradientText("Settings");

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5.f);
                ImGui::TextDisabled("Menu Hotkey");
                ImGui::SameLine();
                ImGui::SetCursorPosX(leftW - 75.f);
                if (ImGui::Button("Set##menu", { 70.f, 24.f }))
                {
                    std::thread([&]() {
                        std::string kn = "Menu";
                        KeyMgr::GetPressedKey(MenuConfig::HotKey, &kn);
                    }).detach();
                }

                PutSwitch("Behavior Meter",     5.f, ImGui::GetFrameHeight() * 1.7f, &AnalyticsCFG::Enabled);
                PutSwitch("Bomb Timer",         5.f, ImGui::GetFrameHeight() * 1.7f, &MiscCFG::bmbTimer);
                PutSwitch("Sniper Crosshair",   5.f, ImGui::GetFrameHeight() * 1.7f, &MiscCFG::SniperCrosshair,
                          true, "###SniperCol", reinterpret_cast<float*>(&MiscCFG::SniperCrosshairColor));
                PutSwitch("Spectator Mode",     5.f, ImGui::GetFrameHeight() * 1.7f, &MenuConfig::WorkInSpec);
                PutSwitch("Team Check",         5.f, ImGui::GetFrameHeight() * 1.7f, &MenuConfig::TeamCheck);
                PutSwitch("Anti-Record (OBS)",  5.f, ImGui::GetFrameHeight() * 1.7f, &MenuConfig::BypassOBS);

                ImGui::NewLine();

                // accent divider
                {
                    ImVec2 sp = ImGui::GetCursorScreenPos();
                    ImGui::GetWindowDrawList()->AddRectFilledMultiColor(
                        ImVec2(sp.x + 5, sp.y), ImVec2(sp.x + leftW - 10, sp.y + 1),
                        kAccent, kAccentTrans, kAccentTrans, kAccent);
                    ImGui::Dummy(ImVec2(0, 4.f));
                }

                ImGui::NewLine();

                // action buttons
                ImGui::SetCursorPosX(5.f);
                ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.50f, 0.12f, 0.09f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.86f, 0.28f, 0.22f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(1.00f, 0.40f, 0.28f, 1.f));
                if (ImGui::Button("Unhook", { 115.f, 26.f }))
                    Init::Client::Exit();
                ImGui::SameLine();
                if (ImGui::Button("Clear Traces", { 115.f, 26.f }))
                {
                    Misc::CleanTraces();
                    Init::Client::Exit();
                }
                ImGui::PopStyleColor(3);
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // ── Right child: Config ───────────────────────────────────────────
            ImGui::BeginChild("##cfg_right", ImVec2(rightW, childH), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::GradientText("Config");
                ConfigMenu::RenderCFGmenu();
            }
            ImGui::EndChild();
        }

        ImGui::NewLine();
        ImGui::EndChild();
        ImGui::PopStyleColor(); // ChildBg override

        ImGui::End();

        ImGui::PopStyleColor(2); // WindowBg, Border
        ImGui::PopStyleVar(2);   // WindowRounding, WindowPadding

        LoadDefaultConfig();
    }
}
