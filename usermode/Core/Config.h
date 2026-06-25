#pragma once
#include "../OS-ImGui/imgui/imgui.h"
#include <Windows.h>

namespace MenuConfig
{
	inline std::string name = "C2C";
	inline std::string version = "1.0.0.0";
	inline std::string author = "C2C";

	inline std::string path = "";
	inline std::string docPath = "";
	inline int RenderFPS = 1000;
	inline int RenderDistance = 1000;

	struct {
		// 0: Visual 1: Config
		int MenuPage = 0;
		ImVec2 MainWinSize = ImVec2(600.f, 500.f);
		ImVec2 Button1Pos = ImVec2(0.f, 59.f);
		ImVec2 Button2Pos = ImVec2(0.f, 118.f);
		ImVec2 ChildPos = ImVec2(60.f, 0.f);
		ImVec2 ChildSize = ImVec2(540.f, 500.f);
	} WCS;	// Window Component Settings

	inline bool defaultConfig = false;

	// 0: Window 1: Collapse
	inline int WindowStyle = 0;
	inline bool ShowMenu = true;
	inline bool TeamCheck = true;
	inline bool BypassOBS = false;
	inline bool WorkInSpec = true;

	inline ImVec2 MarkWinPos;
	inline ImVec2 RadarWinPos;
	inline ImVec2 SpecWinPos;
	inline ImVec2 BombWinPos;
	inline bool MarkWinChengePos = false;
	inline bool RadarWinChengePos = false;
	inline bool SpecWinChengePos = false;
	inline bool BombWinChengePos = false;
	inline int HotKey = VK_END;
}

namespace ESPConfig
{
	inline int HotKey = VK_F6;
	inline bool ESPenabled = true;
	inline bool ShowBoneESP = true;
	inline bool ShowHeadBox = true;
	inline bool ShowPreview = true;
	inline bool ShowBoxESP = false;
	inline int BoxType = 0;
	inline bool FlashCheck = false;
	inline bool ShowHealthBar = true;
	inline bool ShowHealthNum = false;
	inline bool AmmoBar = false;
	inline bool ArmorBar = false;
	inline bool ShowArmorNum = false;
	inline bool ShowDistance = false;
	inline bool ShowLineToEnemy = false;
	inline int LinePos = 0;
	inline bool ShowEyeRay = false;
	inline bool OutLine = true;
	inline bool FilledVisBox = false;
	inline bool MultiColor = false;
	inline bool VisibleCheck = false;
	inline bool ShowWeaponESP = false;
	inline bool ShowPlayerName = false;
	inline bool ShowOutOfFOVArrow = false;
	inline bool ShowIsScoped = false;
	inline bool ShowIsBlind = false;
	inline bool EnemySound = false;
	inline float BoxRounding = 5.0f;
	inline float OutOfFOVRadiusFactor = 0.45f;

	inline ImColor BoneColor = ImColor(131, 137, 150, 180);
	inline ImColor HeadBoxColor = ImColor(131, 137, 150, 180);
	inline ImColor BoxColor = ImColor(59, 71, 148, 180);
	inline ImColor FilledColor = ImColor(59, 71, 148, 128);
	inline ImColor FilledColor2 = ImColor(59, 71, 148, 128);
	inline ImColor BoxFilledVisColor = ImColor(0, 98, 98, 128);
	inline ImColor VisibleColor = ImColor(59, 71, 148, 180);
	inline ImColor OutOfFOVArrowColor = ImColor(59, 71, 148, 128);
	inline ImColor EnemySoundColor = ImColor(255, 255, 255, 255);
	inline ImVec4 EyeRayColor = ImVec4(0, 98, 98, 255);
	inline ImVec4 LineToEnemyColor = ImVec4(59, 71, 148, 180);
}

namespace RadarCFG
{
	inline bool  ShowRadar  = false;
	inline float RadarSize  = 245.f;   // radar penceresinin genisligi/yuksekligi (px)
	inline ImVec2 RadarPos  = {35.f, 35.f};
}

namespace MiscCFG
{
	inline bool WaterMark = true;
	inline bool SniperCrosshair = true;
	inline ImColor SniperCrosshairColor = ImColor(32, 178, 170, 255);
	inline bool SpecList = false;
	inline bool bmbTimer = true;
	inline bool FastStop = false;
	inline bool AutoKnife = false;
	inline bool AutoZeus = false;
	inline float FastStopDelay = 100.f;
	inline float AutoKnifeDistance = 70.0f;
	inline float AutoZeusDistance = 130.0f;
	inline ImColor BombTimerCol = ImColor(131, 137, 150, 255);
}

namespace TriggerBotCFG
{
	inline bool Enabled = false;
	inline bool AlwaysActive = false;
	inline bool HeadOnly = false;
	inline int Delay = 100; // ms
	inline int HotKey = VK_LBUTTON; // Left Click
}

namespace AnalyticsCFG
{
	inline bool Enabled = true;
	inline ImVec2 WinPos = ImVec2(25.f, 280.f);
}

