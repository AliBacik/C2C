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

ID3D11ShaderResourceView* MenuButton1 = NULL;
ID3D11ShaderResourceView* MenuButton2 = NULL;
ID3D11ShaderResourceView* MenuButton1Pressed = NULL;
ID3D11ShaderResourceView* MenuButton2Pressed = NULL;

bool Button1Pressed = true;
bool Button2Pressed = false;

int buttonW = 0;
int buttonH = 0;

namespace GUI
{
	void LoadImages()
	{
		if (MenuButton1 == NULL)
		{
			Gui.LoadTextureFromMemory(Images::VisualButton, sizeof Images::VisualButton, &MenuButton1, &buttonW, &buttonH);
			Gui.LoadTextureFromMemory(Images::ConfigButton, sizeof Images::ConfigButton, &MenuButton2, &buttonW, &buttonH);
			Gui.LoadTextureFromMemory(Images::VisualButtonPressed, sizeof Images::VisualButtonPressed, &MenuButton1Pressed, &buttonW, &buttonH);
			Gui.LoadTextureFromMemory(Images::ConfigButtonPressed, sizeof Images::ConfigButtonPressed, &MenuButton2Pressed, &buttonW, &buttonH);

			MenuConfig::RadarWinPos = ImVec2(25.f, 25.f);
		}
	}

	void LoadDefaultConfig()
	{
		if (!MenuConfig::defaultConfig)
			return;

		MyConfigSaver::LoadConfig("default.cfg");
		MenuConfig::defaultConfig = false;
	}

	void AlignRight(float ContentWidth)
	{
		float ColumnContentWidth = ImGui::GetColumnWidth() - ImGui::GetStyle().ItemSpacing.x;
		float checkboxPosX = ImGui::GetColumnOffset() + ColumnContentWidth - ContentWidth;
		ImGui::SetCursorPosX(checkboxPosX);
	}

	void PutSwitch(const char* string, float CursorX, float ContentWidth, bool* v, bool ColorEditor = false, const char* lable = NULL, float col[4] = NULL, const char* Tip = NULL)
	{
		ImGui::PushID(string);
		float CurrentCursorX = ImGui::GetCursorPosX();
		float CurrentCursorY = ImGui::GetCursorPosY();
		ImGui::SetCursorPosX(CurrentCursorX + CursorX);
		ImGui::TextDisabled(string);
		if (Tip && ImGui::IsItemHovered())
			ImGui::SetTooltip(Tip);
		ImGui::SameLine();
		ImGui::SetCursorPosY(CurrentCursorY - 2);
		if (ColorEditor) {
			AlignRight(ContentWidth + ImGui::GetFrameHeight() + 7);
			ImGui::ColorEdit4(lable, col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
			ImGui::SameLine();
		}
		else {
			AlignRight(ContentWidth);
		}

		Gui.SwitchButton(string, v);
		ImGui::PopID();
	}

	void PutColorEditor(const char* text, const char* lable, float CursorX, float ContentWidth, float col[4], const char* Tip = NULL)
	{
		ImGui::PushID(text);
		float CurrentCursorX = ImGui::GetCursorPosX();
		ImGui::SetCursorPosX(CurrentCursorX + CursorX);
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
		float CurrentCursorX = ImGui::GetCursorPosX();
		float SliderWidth = ImGui::GetColumnWidth() - ImGui::GetStyle().ItemSpacing.x - CursorX - 15;
		ImGui::SetCursorPosX(CurrentCursorX + CursorX);
		ImGui::TextDisabled(string);
		if (Tip && ImGui::IsItemHovered())
			ImGui::SetTooltip(Tip);
		ImGui::SameLine();
		ImGui::TextDisabled(format, *v);
		ImGui::SetCursorPosX(CurrentCursorX + CursorX);
		ImGui::SetNextItemWidth(SliderWidth);
		Gui.SliderScalarEx2("", ImGuiDataType_Float, v, p_min, p_max, "", ImGuiSliderFlags_None);
		ImGui::PopID();
	}

	void PutSliderInt(const char* string, float CursorX, int* v, const void* p_min, const void* p_max, const char* format, const char* Tip = NULL)
	{
		ImGui::PushID(string);
		float CurrentCursorX = ImGui::GetCursorPosX();
		float SliderWidth = ImGui::GetColumnWidth() - ImGui::GetStyle().ItemSpacing.x - CursorX - 15;
		ImGui::SetCursorPosX(CurrentCursorX + CursorX);
		ImGui::TextDisabled(string);
		if (Tip && ImGui::IsItemHovered())
			ImGui::SetTooltip(Tip);
		ImGui::SameLine();
		ImGui::TextDisabled(format, *v);
		ImGui::SetCursorPosX(CurrentCursorX + CursorX);
		ImGui::SetNextItemWidth(SliderWidth);
		Gui.SliderScalarEx2("", ImGuiDataType_Float, v, p_min, p_max, "", ImGuiSliderFlags_None);
		ImGui::PopID();
	}

	void DrawGui()
	{
		LoadImages();
		ImColor BorderColor = ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border));

		ImGuiWindowFlags Flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
		ImGui::SetNextWindowPos({ (ImGui::GetIO().DisplaySize.x - MenuConfig::WCS.MainWinSize.x) / 2.0f, (ImGui::GetIO().DisplaySize.y - MenuConfig::WCS.MainWinSize.y) / 2.0f }, ImGuiCond_Once);
		ImGui::SetNextWindowSize(MenuConfig::WCS.MainWinSize);
		ImGui::Begin("C2C", nullptr, Flags);
		{
			ImGui::SetCursorPos(MenuConfig::WCS.Button1Pos);
			if (!Button1Pressed)
				ImGui::Image((void*)MenuButton1, ImVec2(buttonW, buttonH));
			if (Button1Pressed)
				ImGui::Image((void*)MenuButton1Pressed, ImVec2(buttonW, buttonH));
			if (ImGui::IsItemClicked())
			{
				MenuConfig::WCS.MenuPage = 0;
				Button1Pressed = true;
				Button2Pressed = false;
			}
			ImGui::GetWindowDrawList()->AddRect(
				ImVec2(MenuConfig::WCS.Button1Pos.x + ImGui::GetWindowPos().x, MenuConfig::WCS.Button1Pos.y + ImGui::GetWindowPos().y),
				ImVec2(MenuConfig::WCS.Button1Pos.x + buttonW + ImGui::GetWindowPos().x, MenuConfig::WCS.Button1Pos.y + buttonH + ImGui::GetWindowPos().y),
				BorderColor, 0.f, ImDrawFlags_RoundCornersNone | ImDrawCornerFlags_Top | ImDrawCornerFlags_Bot, 1.f, true);

			ImGui::SetCursorPos(MenuConfig::WCS.Button2Pos);
			if (!Button2Pressed)
				ImGui::Image((void*)MenuButton2, ImVec2(buttonW, buttonH));
			if (Button2Pressed)
				ImGui::Image((void*)MenuButton2Pressed, ImVec2(buttonW, buttonH));
			if (ImGui::IsItemClicked())
			{
				MenuConfig::WCS.MenuPage = 1;
				Button1Pressed = false;
				Button2Pressed = true;
			}
			ImGui::GetWindowDrawList()->AddRect(
				ImVec2(MenuConfig::WCS.Button2Pos.x + ImGui::GetWindowPos().x, MenuConfig::WCS.Button2Pos.y + ImGui::GetWindowPos().y),
				ImVec2(MenuConfig::WCS.Button2Pos.x + buttonW + ImGui::GetWindowPos().x, MenuConfig::WCS.Button2Pos.y + buttonH + ImGui::GetWindowPos().y),
				BorderColor, 0.f, ImDrawFlags_RoundCornersNone | ImDrawCornerFlags_Top | ImDrawCornerFlags_Bot, 1.f, true);

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);

			ImGui::SetCursorPos(MenuConfig::WCS.ChildPos);

			ImGui::BeginChild("Page", MenuConfig::WCS.ChildSize, false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
			{
				ImGui::Text("   C2C");
				ImGui::Separator();

				if (MenuConfig::WCS.MenuPage == 0)
				{
					ImGui::Columns(2, nullptr, false);
					ImGui::SetCursorPos(ImVec2(15.f, 24.f));
					ImGui::GradientText("ESP - Skeleton");

					PutSwitch(Text::ESP::Enable.c_str(), 10.f, ImGui::GetFrameHeight() * 1.7, &ESPConfig::ESPenabled);

					if (ESPConfig::ESPenabled)
					{
						PutSwitch(Text::ESP::Skeleton.c_str(), 10.f, ImGui::GetFrameHeight() * 1.7, &ESPConfig::ShowBoneESP, true, "###BoneCol", reinterpret_cast<float*>(&ESPConfig::BoneColor));
						PutSwitch(Text::ESP::HeadBox.c_str(), 10.f, ImGui::GetFrameHeight() * 1.7, &ESPConfig::ShowHeadBox, true, "###HeadBoxCol", reinterpret_cast<float*>(&ESPConfig::HeadBoxColor));
					}
					ImGui::NewLine();

					ImGui::GradientText("Trigger Bot");
					PutSwitch("Enable##tb", 10.f, ImGui::GetFrameHeight() * 1.7, &TriggerBotCFG::Enabled);
					if (TriggerBotCFG::Enabled)
					{
						static const int DelayMin = 10, DelayMax = 500;
						PutSliderInt("Delay (ms)", 10.f, &TriggerBotCFG::Delay, &DelayMin, &DelayMax, "%d ms");
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.f);
						ImGui::TextDisabled("Hotkey: ALT (hold)");
					}
					ImGui::NewLine();

					ImGui::NextColumn();
					ImGui::SetCursorPosY(24.f);
					ImGui::GradientText("Preview");
					ESP::RenderPreview({ ImGui::GetColumnWidth(), ImGui::GetCursorPosY() });
					ImGui::Dummy({ 0.f, ImGui::GetFrameHeight() * 9 });

					ImGui::GradientText("Radar");
					static const float RadarPointSizeProportionMin = 0.2f, RadarPointSizeProportionMax = 2.f;
					static const float ProportionMin = 500.f, ProportionMax = 15000.f;
					static const float RadarRangeMin = 100.f, RadarRangeMax = 300.f;
					static const float AlphaMin = 0.f, AlphaMax = 1.f;
					PutSwitch(Text::Radar::Toggle.c_str(), 5.f, ImGui::GetFrameHeight() * 1.7, &RadarCFG::ShowRadar);
					if (RadarCFG::ShowRadar)
					{
						PutSwitch(Text::Radar::CustomCheck.c_str(), 5.f, ImGui::GetFrameHeight() * 1.7, &RadarCFG::customRadar);

						if (RadarCFG::customRadar)
						{
							PutSwitch(Text::Radar::CrossLine.c_str(), 5.f, ImGui::GetFrameHeight() * 1.7, &RadarCFG::ShowRadarCrossLine);
							PutSliderFloat(Text::Radar::SizeSlider.c_str(), 5.f, &RadarCFG::RadarPointSizeProportion, &RadarPointSizeProportionMin, &RadarPointSizeProportionMax, "%.1f");
							PutSliderFloat(Text::Radar::ProportionSlider.c_str(), 5.f, &RadarCFG::Proportion, &ProportionMin, &ProportionMax, "%.1f");
							PutSliderFloat(Text::Radar::RangeSlider.c_str(), 5.f, &RadarCFG::RadarRange, &RadarRangeMin, &RadarRangeMax, "%.1f");
							PutSliderFloat(Text::Radar::AlphaSlider.c_str(), 5.f, &RadarCFG::RadarBgAlpha, &AlphaMin, &AlphaMax, "%.1f");
						}
					}

					ImGui::Columns(1);
				}

				if (MenuConfig::WCS.MenuPage == 1)
				{
					ImGui::Columns(2, nullptr, false);
					ImGui::SetCursorPos(ImVec2(15.f, 24.f));
					ImGui::GradientText("Settings");

					ImGui::TextDisabled("Menu Hotkey");
					ImGui::SameLine();
					AlignRight(70.f);
					if (ImGui::Button("Set", { 70.f, 25.f }))
					{
						std::thread([&]() {
							std::string keyName = "Menu";
							KeyMgr::GetPressedKey(MenuConfig::HotKey, &keyName);
							}).detach();
					}

					PutSwitch("Behavior Meter", 5.f, ImGui::GetFrameHeight() * 1.7, &AnalyticsCFG::Enabled);
					PutSwitch("WH Helper", 5.f, ImGui::GetFrameHeight() * 1.7, &WallHackHelperCFG::Enabled);
					if (WallHackHelperCFG::Enabled)
					{
						static const int SafeMin = 50, SafeMax = 800;
						static const int PreAimMin = 50, PreAimMax = 300;
						PutSliderInt("Cold Peek (ms)", 5.f, &WallHackHelperCFG::SafeDelayMs, &SafeMin, &SafeMax, "%d ms");
						PutSliderInt("Pre-Aim (ms)", 5.f, &WallHackHelperCFG::PreAimDelayMs, &PreAimMin, &PreAimMax, "%d ms");
					}
					PutSwitch("Spectator Mode", 5.f, ImGui::GetFrameHeight() * 1.7, &MenuConfig::WorkInSpec);
					PutSwitch("Team Check", 5.f, ImGui::GetFrameHeight() * 1.7, &MenuConfig::TeamCheck);
					PutSwitch("Anti-Record (OBS)", 5.f, ImGui::GetFrameHeight() * 1.7, &MenuConfig::BypassOBS);

					ImGui::NewLine();
					ImGui::Separator();
					ImGui::NewLine();

					if (ImGui::Button("Unhook", { 125.f, 25.f }))
						Init::Client::Exit();
					ImGui::SameLine();
					if (ImGui::Button("Clear Traces", { 125.f, 25.f }))
					{
						Misc::CleanTraces();
						Init::Client::Exit();
					}

					ImGui::NextColumn();
					ImGui::SetCursorPosY(24.f);
					ImGui::GradientText("Config");
					ConfigMenu::RenderCFGmenu();

					ImGui::Columns(1);
				}

				ImGui::NewLine();
			}
			ImGui::EndChild();
		}
		ImGui::End();

		LoadDefaultConfig();
	}
}
