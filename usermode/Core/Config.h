#pragma once
#include "../Game/Bone.h"
#include "../Game/Game.h"

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

	inline ImColor BoneColor = ImColor(131, 137, 150, 180);
	inline ImColor HeadBoxColor = ImColor(131, 137, 150, 180);
}

namespace RadarCFG
{
	inline bool ShowRadar = false;
	inline float RadarRange = 125;
	inline float RadarPointSizeProportion = 1.f;
	inline bool ShowRadarCrossLine = false;
	inline ImColor RadarCrossLineColor = ImColor(131, 137, 150, 180);
	// 0: circle 1: arrow 2: circle with arrow
	inline int RadarType = 2;
	inline float Proportion = 2700.f;
	inline bool customRadar = false;
	inline float RadarBgAlpha = 0.1f;
}

namespace MiscCFG
{
	inline bool WaterMark = true;
}

namespace TriggerBotCFG
{
	inline bool Enabled = false;
	inline int Delay = 100; // ms
	inline int HotKey = VK_MENU; // Alt
}

namespace AnalyticsCFG
{
	inline bool Enabled = true;
	inline ImVec2 WinPos = ImVec2(25.f, 280.f);
}

namespace WallHackHelperCFG
{
	inline bool Enabled = true;
	inline int SafeDelayMs = 250;     // cold peek: bu sure gectikten sonra yesil
	inline int PreAimDelayMs = 90;    // pre-aim peek: zaten nisanliysa daha kisa sure
	inline ImColor UnsafeColor = ImColor(220, 60, 60, 200);
	inline ImColor SafeColor   = ImColor(80, 200, 80, 200);
}
