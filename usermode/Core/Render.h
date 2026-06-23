#pragma once
#define NOMINMAX
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <chrono>
#include <map>
#include <Windows.h>
#include <unordered_map>
#include "../Core/Config.h"
#include "../Game/Entity.h"
#include "../OS-ImGui/imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../OS-ImGui/imgui/imgui_internal.h"
namespace Render
{

	inline void DrawHeadCircle(const CEntity& Entity, ImColor Color)
	{
		if (!ESPConfig::ShowHeadBox)
			return;

		const auto& BoneList = Entity.GetBone().BonePosList;
		if (BoneList.empty()) return;

		const BoneJointPos& Head = BoneList[BONEINDEX::head];
		const BoneJointPos& Neck = BoneList[BONEINDEX::neck_0];

		Gui.Circle(Head.ScreenPos, std::abs(Head.ScreenPos.y - Neck.ScreenPos.y) + 2.0f, Color, 1.2f);
	}


	inline void DrawHealth(int Health, const ImVec2& Pos)
	{
		if (!ESPConfig::ShowHealthNum)
			return;

		char health_str[4];
		snprintf(health_str, sizeof(health_str), "%d", Health);

		Gui.StrokeText(health_str, Pos, ImColor(0, 220, 0, 255), 12, false);
	}


	inline void DrawDistance(const CEntity& LocalEntity, const CEntity& Entity, ImVec4 Rect)
	{
		if (!ESPConfig::ShowDistance)
			return;

		int distance = static_cast<int>(Entity.Pawn.Pos.DistanceTo(LocalEntity.Pawn.Pos)) / 100;
		std::string dis_str = std::to_string(distance) + "m";

		Gui.StrokeText(dis_str, { Rect.x + Rect.z + 4, Rect.y }, ImColor(0, 98, 98, 255), 14, false);

	}

	inline void DrawCrossHair(ImDrawList* drawList, const ImVec2& pos, ImColor color) noexcept
	{
		constexpr float gap = 0.1f;
		constexpr int size = 6;
		constexpr float thickness = 1.1f;

		float left = pos.x - (gap + size);
		float right = pos.x + (gap + 1 + size);
		float top = pos.y - (gap + size);
		float bottom = pos.y + (gap + 1 + size);

		float xMin = pos.x - thickness + 1;
		float xMax = pos.x + thickness;
		float yMin = pos.y - thickness + 1;
		float yMax = pos.y + thickness;

		drawList->AddRectFilled(ImVec2(left, yMin), ImVec2(pos.x - gap, yMax), color);  // Left
		drawList->AddRectFilled(ImVec2(pos.x + gap + 1, yMin), ImVec2(right, yMax), color); // Right
		drawList->AddRectFilled(ImVec2(xMin, top), ImVec2(xMax, pos.y - gap), color);  // Top
		drawList->AddRectFilled(ImVec2(xMin, pos.y + gap + 1), ImVec2(xMax, bottom), color); // Bottom
	}

	inline void LineToEnemy(ImVec4 Rect, ImColor Color, float Thickness)
	{
		if (!ESPConfig::ShowLineToEnemy)
			return;

		const float CenterX = Gui.Window.Size.x * 0.5f;
		const float CenterY = Gui.Window.Size.y * 0.5f;
		const float StartX = Rect.x + Rect.z * 0.5f;
		float StartY = Rect.y;
		float EndX = CenterX, EndY = 0;

		switch (ESPConfig::LinePos)
		{
		case 1: EndY = CenterY; break;
		case 2: StartY += Rect.w; EndY = Gui.Window.Size.y; break;
		default: break;
		}

		Gui.Line({ StartX, StartY }, { EndX, EndY }, Color, Thickness);
	}

	inline ImVec4 Get2DBox(const CEntity& Entity)
	{
		const auto& bonePosList = Entity.GetBone().BonePosList;
		if (bonePosList.size() <= BONEINDEX::head)
			return ImVec4(0, 0, 0, 0);

		// head ve ayak world pos'larini her frame world-to-screen ile hesapla
		// BonePosList[x].Pos = world space, bu her zaman guncelleniyor
		const Vec3& headWorld  = bonePosList[BONEINDEX::head].Pos;
		const Vec3& ankLWorld  = bonePosList[BONEINDEX::ankle_L].Pos;
		const Vec3& ankRWorld  = bonePosList[BONEINDEX::ankle_R].Pos;

		Vec2 headScreen, ankLScreen, ankRScreen;
		if (!gGame.View.WorldToScreen(headWorld, headScreen))
			return ImVec4(0, 0, 0, 0);

		// ayak noktalarini dene, ikisi de fail ederse head'i kullan
		bool hasAnkL = gGame.View.WorldToScreen(ankLWorld, ankLScreen);
		bool hasAnkR = gGame.View.WorldToScreen(ankRWorld, ankRScreen);

		float feetY = headScreen.y; // fallback
		float centerX = headScreen.x;
		if (hasAnkL && hasAnkR)
		{
			feetY   = std::max(ankLScreen.y, ankRScreen.y);
			centerX = (ankLScreen.x + ankRScreen.x) * 0.5f;
		}
		else if (hasAnkL) { feetY = ankLScreen.y; centerX = ankLScreen.x; }
		else if (hasAnkR) { feetY = ankRScreen.y; centerX = ankRScreen.x; }

		float height = (feetY - headScreen.y) * 1.06f;
		if (height < 10.f) return ImVec4(0, 0, 0, 0);

		float width  = height * 0.55f;
		float x      = centerX - width * 0.5f;
		float y      = headScreen.y - height * 0.05f;

		return ImVec4(x, y, width, height);
	}

	inline void DrawBone(const CEntity& Entity, ImColor Color, float Thickness)
	{
		if (!ESPConfig::ShowBoneESP)
			return;

		const auto& bonePosList = Entity.GetBone().BonePosList;
		if (bonePosList.size() < 23)
			return;

		float screenW = (float)GetSystemMetrics(SM_CXSCREEN);
		float screenH = (float)GetSystemMetrics(SM_CYSCREEN);

#ifdef BONE_DEBUG
		// bone index debug: her bone'u noktala ve index'ini yaz
		ImDrawList* dbgDraw = ImGui::GetBackgroundDrawList();
		for (size_t di = 0; di < bonePosList.size() && di < 30; ++di)
		{
			Vec2 ds;
			if (!gGame.View.WorldToScreen(bonePosList[di].Pos, ds)) continue;
			if (ds.x <= 0.f || ds.y <= 0.f || ds.x >= screenW || ds.y >= screenH) continue;
			dbgDraw->AddCircleFilled(ImVec2(ds.x, ds.y), 3.f, IM_COL32(255,255,0,255));
			char lbl[8]; snprintf(lbl, sizeof(lbl), "%d", (int)di);
			dbgDraw->AddText(ImVec2(ds.x + 4, ds.y - 6), IM_COL32(255,255,255,255), lbl);
		}
#endif

		for (const auto& boneChain : BoneJointList::List)
		{
			Vec2 prevScreen{};
			bool hasPrev = false;

			for (const auto& index : boneChain)
			{
				if (index >= bonePosList.size())
					continue;

				Vec2 curScreen;
				if (!gGame.View.WorldToScreen(bonePosList[index].Pos, curScreen))
					continue;

				if (curScreen.x <= 0.f || curScreen.y <= 0.f ||
					curScreen.x >= screenW || curScreen.y >= screenH)
					continue;

				if (hasPrev)
					Gui.Line(prevScreen, curScreen, Color, Thickness);

				prevScreen = curScreen;
				hasPrev = true;
			}
		}
	}

	inline void ShowLosLine(const CEntity& Entity, const float Length, ImColor Color, float Thickness)
	{
		if (!ESPConfig::ShowEyeRay)
			return;

		const auto& bonePosList = Entity.GetBone().BonePosList;
		const BoneJointPos& head = bonePosList[BONEINDEX::head];
		const Vec2 startPoint = head.ScreenPos;

		const float degToRad = M_PI / 180.0f;
		const float viewAngleX = Entity.Pawn.ViewAngle.x * degToRad;
		const float viewAngleY = Entity.Pawn.ViewAngle.y * degToRad;

		const float lineLength = cos(viewAngleX) * Length;

		Vec3 temp;
		temp.x = head.Pos.x + cos(viewAngleY) * lineLength;
		temp.y = head.Pos.y + sin(viewAngleY) * lineLength;
		temp.z = head.Pos.z - sin(viewAngleX) * Length;

		Vec2 endPoint;
		if (!gGame.View.WorldToScreen(temp, endPoint))
			return;

		Gui.Line(startPoint, endPoint, Color, Thickness);
	}

	inline ImVec4 Get2DBoneRect(const CEntity& Entity)
	{
		// Get2DBox ile ayni world-to-screen tabanli hesaplama
		return Get2DBox(Entity);
	}


	class HealthBar
	{
	private:
		using TimePoint_ = std::chrono::steady_clock::time_point;
		const int ShowBackUpHealthDuration = 500;
		float MaxHealth = 0.f;
		float CurrentHealth = 0.f;
		float LastestBackupHealth = 0.f;
		ImVec2 RectPos{};
		ImVec2 RectSize{};
		bool InShowBackupHealth = false;
		TimePoint_ BackupHealthTimePoint{};
		int MaxAmmo = 0;
		int CurrentAmmo = 0;
		int MaxArmor = 0;
		int CurrentArmor = 0;

	public:
		HealthBar() {}

		void HealthBarV(float MaxHealth, float CurrentHealth, const ImVec2& Pos, const ImVec2& Size, bool ShowBar, bool ShowNum);
		void ArmorBarV(bool HasHelmet, float MaxArmor, float CurrentArmor, const ImVec2& Pos, const ImVec2& Size, bool ShowBar, bool ShowNum);
		void AmmoBarH(float MaxAmmo, float CurrentAmmo, const ImVec2& Pos, const ImVec2& Size);

	private:
		inline ImColor Mix(const ImColor& Col_1, const ImColor& Col_2, float t)
		{
			ImColor col;
			col.Value.x = t * Col_1.Value.x + (1.0f - t) * Col_2.Value.x;
			col.Value.y = t * Col_1.Value.y + (1.0f - t) * Col_2.Value.y;
			col.Value.z = t * Col_1.Value.z + (1.0f - t) * Col_2.Value.z;
			col.Value.w = Col_1.Value.w;
			return col;
		}

		ImColor FirstStageColor = ImColor(0, 255, 0, 255);
		ImColor SecondStageColor = ImColor(255, 232, 0, 255);
		ImColor ThirdStageColor = ImColor(255, 39, 0, 255);
		ImColor BackupHealthColor = ImColor(255, 255, 255, 220);
		ImColor FrameColor = ImColor(45, 45, 45, 220);
		ImColor BackGroundColor = ImColor(0, 0, 0, 255);
		ImColor AmmoColor = ImColor(255, 255, 0, 255);
		ImColor ArmorColor = ImColor(0, 128, 255, 255);
		ImColor ArmorWithHelmetColor = ImColor(255, 0, 255, 255);
	};

	//////////////////////////////////////////////////////////////////////////
	// Vertical Health Bar
	//////////////////////////////////////////////////////////////////////////
	void HealthBar::HealthBarV(float MaxHealth, float CurrentHealth, const ImVec2& Pos, const ImVec2& Size, bool ShowBar, bool ShowNum)
	{
		if (ShowBar || ShowNum)
		{
			this->MaxHealth = MaxHealth;
			this->CurrentHealth = CurrentHealth;
			this->RectPos = Pos;
			this->RectSize = Size;
		}
		if (ShowBar)
		{
			ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

			float proportion = (MaxHealth > 0.f) ? CurrentHealth / MaxHealth : 0.f;
			proportion = (proportion < 0.f) ? 0.f : proportion;

			float height = RectSize.y * proportion;
			ImVec2 rectBR = { RectPos.x + RectSize.x, RectPos.y + RectSize.y };

			DrawList->AddRectFilled(RectPos, rectBR, BackGroundColor, 5, 15);

			float colorLerpT = powf(proportion, 2.5f);
			ImColor color = (proportion > 0.5f && proportion <= 1.f) ?
				Mix(FirstStageColor, SecondStageColor, colorLerpT * 3.f - 1.f) :
				Mix(SecondStageColor, ThirdStageColor, colorLerpT * 4.f);

			ImVec2 healthRectTL = { RectPos.x, RectPos.y + RectSize.y - height };
			DrawList->AddRectFilled(healthRectTL, rectBR, color, 0);

			DrawList->AddRect(RectPos, rectBR, FrameColor, 0, 15, 1);
		}
		if (ShowNum)
		{
			char healthStr[16];
			snprintf(healthStr, sizeof(healthStr), "%.f", CurrentHealth);
			Gui.StrokeText(healthStr, {RectPos.x + RectSize.x+6 , RectPos.y}, ImColor(255, 255, 255), 13.f, true);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Vertical Armor Bar
	//////////////////////////////////////////////////////////////////////////
	void HealthBar::ArmorBarV(bool HasHelmet, float MaxArmor, float CurrentArmor, const ImVec2& Pos, const ImVec2& Size, bool ShowBar, bool ShowNum)
	{
		if (ShowBar || ShowNum)
		{
			this->MaxArmor = MaxArmor;
			this->CurrentArmor = CurrentArmor;
			this->RectPos = Pos;
			this->RectSize = Size;
		}
		if (ShowBar)
		{
			ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

			float proportion = (MaxArmor > 0.f) ? CurrentArmor / MaxArmor : 0.f;
			proportion = (proportion < 0.f) ? 0.f : proportion;

			float height = RectSize.y * proportion;
			ImVec2 rectBR = { RectPos.x + RectSize.x, RectPos.y + RectSize.y };

			DrawList->AddRectFilled(RectPos, rectBR, BackGroundColor, 5, 15);

			ImColor color = HasHelmet ? ArmorWithHelmetColor : ArmorColor;
			ImVec2 armorRectTL = { RectPos.x, RectPos.y + RectSize.y - height };
			DrawList->AddRectFilled(armorRectTL, rectBR, color, 0);

			DrawList->AddRect(RectPos, rectBR, FrameColor, 0, 15, 1);
		}
		if (ShowNum)
		{
			char armorStr[16];
			snprintf(armorStr, sizeof(armorStr), "%.f", CurrentArmor);
			Gui.StrokeText(armorStr, { RectPos.x - RectSize.x -3 , RectPos.y }, ImColor(255, 255, 255), 13.f, true);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Horizontal Ammo Bar
	//////////////////////////////////////////////////////////////////////////
	void HealthBar::AmmoBarH(float MaxAmmo, float CurrentAmmo, const ImVec2& Pos, const ImVec2& Size)
	{
		ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

		this->MaxAmmo = MaxAmmo;
		this->CurrentAmmo = CurrentAmmo;
		this->RectPos = Pos;
		this->RectSize = Size;

		float proportion = (MaxAmmo > 0) ? CurrentAmmo / static_cast<float>(MaxAmmo) : 0.f;
		proportion = (proportion < 0.f) ? 0.f : proportion;

		float width = RectSize.x * proportion;
		ImVec2 rectBR = { RectPos.x + RectSize.x, RectPos.y + RectSize.y };

		DrawList->AddRectFilled(RectPos, rectBR, BackGroundColor, 5, 15);

		ImVec2 ammoRectBR = { RectPos.x + width, RectPos.y + RectSize.y };
		DrawList->AddRectFilled(RectPos, ammoRectBR, AmmoColor, 0);

		DrawList->AddRect(RectPos, rectBR, FrameColor, 0, 15, 1);
	}

	void DrawHealthBar(DWORD Sign, float MaxHealth, float CurrentHealth, const ImVec2& Pos, const ImVec2& Size)
	{
		static std::unordered_map<DWORD, HealthBar> HealthBarMap;
		if (HealthBarMap.size() > 64)
			HealthBarMap.clear();

		HealthBar& hb = HealthBarMap[Sign];
		hb.HealthBarV(MaxHealth, CurrentHealth, Pos, Size, ESPConfig::ShowHealthBar,ESPConfig::ShowHealthNum);
	}

	void DrawAmmoBar(DWORD Sign, float MaxAmmo, float CurrentAmmo, const ImVec2& Pos, const ImVec2& Size)
	{
		static std::unordered_map<DWORD, HealthBar> HealthBarMap;
		if (HealthBarMap.size() > 64)
			HealthBarMap.clear();

		HealthBar& hb = HealthBarMap[Sign];
		hb.AmmoBarH(MaxAmmo, CurrentAmmo, Pos, Size);
	}
	void DrawArmorBar(DWORD Sign, float MaxArmor, float CurrentArmor, bool HasHelmet, const ImVec2& Pos, const ImVec2& Size)
	{
		static std::unordered_map<DWORD, HealthBar> HealthBarMap;
		if (HealthBarMap.size() > 64)
			HealthBarMap.clear();

		HealthBar& hb = HealthBarMap[Sign];
		hb.ArmorBarV(HasHelmet, MaxArmor, CurrentArmor, Pos, Size, ESPConfig::ArmorBar, ESPConfig::ShowArmorNum);
	}

	ImVec2 GetScreenCenterImVec2()
	{
		int W = GetSystemMetrics(SM_CXSCREEN);
		int H = GetSystemMetrics(SM_CYSCREEN);
		return ImVec2(W / 2.0f, H / 2.0f);
	}

	inline ImVec4 rgba2ImVec(int r, int g, int b, int a)
	{
		return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

}