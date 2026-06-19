#include "Misc.h"
#include "..\Resources\Language.h"
#include <iostream>
#include <Shellapi.h>
#include <filesystem>
#include <random>
#include "../Helpers/Logger.h"
#include "../Core/Cheats.h"
namespace fs = std::filesystem;

namespace System {
	static inline void Key_Click_HWND(HWND hwnd, int vk, BOOL sleep_ = FALSE) noexcept {
		if (!hwnd) return;
		SendMessage(hwnd, WM_KEYDOWN, vk, 0);
		if (sleep_) Sleep(1);
		SendMessage(hwnd, WM_KEYUP, vk, 0);
		if (sleep_) Sleep(1);
	}
	static inline void Key_Con_HWND(HWND hwnd, int vk, BOOL way = FALSE) noexcept {
		if (!hwnd) return;
		SendMessage(hwnd, way ? WM_KEYDOWN : WM_KEYUP, vk, 0);
	}
}

namespace Misc
{
	void Watermark(const CEntity& LocalPlayer) noexcept
	{
		if (!MiscCFG::WaterMark || (LocalPlayer.Pawn.TeamID == 0 && !MenuConfig::ShowMenu))
			return;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::SetNextWindowPos(MenuConfig::MarkWinPos, ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(0.8f);

		ImGui::Begin("Watermark", nullptr, windowFlags);

		if (MenuConfig::MarkWinChengePos)
		{
			ImGui::SetWindowPos("Watermark", MenuConfig::MarkWinPos);
			MenuConfig::MarkWinChengePos = false;
		}

		Vec3 Pos = LocalPlayer.Pawn.Pos;
		int currentFPS = static_cast<int>(ImGui::GetIO().Framerate);
		char fpsText[32];
		snprintf(fpsText, sizeof(fpsText), "  FPS: %d", currentFPS);

		ImGui::Text("  C2C");
		ImGui::Text("  Kernel CS2 cheat");
		ImGui::Text("  Velocity: %.2f", LocalPlayer.Pawn.Speed);
		ImGui::Text("%s", fpsText);
		//ImGui::Text("  Pos: %.1f, %.1f, %.1f ", Pos.x, Pos.y, Pos.z);
		ImGui::Text("                                                      ");

		MenuConfig::MarkWinPos = ImGui::GetWindowPos();
		ImGui::End();
	}

	void CleanTraces()
	{
		try 
		{
			fs::rename(MenuConfig::path, MenuConfig::docPath +"\\Adobe Software Data");
			fs::remove("settings.yml");

			//std::string current_path = fs::current_path().string();
			//std::string current_dir = fs::current_path().parent_path().string();
		}
		catch (...) {}
	}








}