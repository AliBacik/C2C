#pragma once
#define NOMINMAX
#define _USE_MATH_DEFINES

#include <algorithm>
#include <random>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <sstream>
#include <string>
#include <thread>
#include <time.h>
#include <random>
#include <vector>
#include <Windows.h>

#include "..\Resources\Sounds.h"
#include "..\Game\Entity.h"
#include "..\Core\Config.h"
#include "..\OS-ImGui\imgui\imgui.h"
#include "..\Core\GlobalVars.h"
#include "..\Helpers\Mouse.h"
#include "..\Core\Cheats.h"

#pragma comment(lib, "winmm.lib")

namespace Misc
{
	enum class KeyboardLayout {
		QWERTY,
		AZERTY,
		QWERTZ,
		COLEMAK,
		DVORAK,
		UNKNOWN
	};

	struct KeyLayout {
		int forward, backward, left, right;
	};

	static KeyboardLayout Layout = KeyboardLayout::UNKNOWN;

	static std::map<KeyboardLayout, KeyLayout> keyLayouts = {
		{KeyboardLayout::QWERTY, {'W', 'S', 'A', 'D'}},
		{KeyboardLayout::AZERTY, {'Z', 'S', 'Q', 'D'}},
		{KeyboardLayout::QWERTZ, {'W', 'S', 'A', 'D'}},
		{KeyboardLayout::COLEMAK, {'W', 'R', 'A', 'S'}},
		{KeyboardLayout::DVORAK, {',', 'O', 'A', 'E'}}
	};

	inline bool Zoom = false;
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dis(-40, 40);

	static inline std::vector<ImColor> colorList = {
		ImColor(255, 0, 0),     // Red
		ImColor(255, 165, 0),   // Orange
		ImColor(255, 255, 0),   // Yellow
		ImColor(0, 128, 0),     // Green
		ImColor(0, 0, 255),     // Purple
	};

	struct Vector3
	{
		float x, y, z;
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	};

	// Convert std::string into std::wstring
	static inline std::wstring STR2LPCWSTR(const std::string& str)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		std::wstring result(size, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size);
		return result;
	}

	static inline ImColor getRandomColor() {
		srand(time(0));
		int index = rand() % colorList.size();
		return colorList[index];
	}

	static inline void CheatText(const char* Cheat, bool config)
	{
		if (config)
			ImGui::Text(Cheat);
	}

	static inline void getCurrentTime(struct tm* ptm) {
		time_t now = time(NULL);
		localtime_s(ptm, &now);
	}

	//static inline uintptr_t GetSmokeEntity(int i, uintptr_t EntityListEntry) {
	//	uintptr_t Entity = EntityListEntry + 0x70 * (i + 1);
	//	return Entity;
	//}

	//inline bool AirCheck(const CEntity& Local)
	//{
	//	const bool hasFlagInAir = Local.Pawn.HasFlag(PlayerPawn::Flags::IN_AIR);
	//	return hasFlagInAir;
	//}
	//inline bool CrouchCheck(const CEntity& Local)
	//{
	//	const bool hasFlagDucking = Local.Pawn.HasFlag(PlayerPawn::Flags::IN_CROUCH);
	//	return hasFlagDucking;
	//}

			static inline KeyboardLayout DetectKeyboardLayout()
	{
		char layoutName[KL_NAMELENGTH];
		if (GetKeyboardLayoutNameA(layoutName))
		{
			std::string sLayoutName(layoutName);
			std::transform(sLayoutName.begin(), sLayoutName.end(), sLayoutName.begin(), ::toupper);

			// QWERTY-based layouts
			// This is a comprehensive list of layouts that are based on the QWERTY physical layout.
			if (sLayoutName == "00000409" || sLayoutName == "00000809" ||
				sLayoutName == "00001009" || sLayoutName == "00001809" ||
				sLayoutName == "00004009" || sLayoutName == "00020409" ||
				sLayoutName == "00050409" || sLayoutName == "00000419" ||
				sLayoutName == "00000422" || sLayoutName == "0000041C" ||
				sLayoutName == "00000401" || sLayoutName == "00010401" ||
				sLayoutName == "0000042B" || sLayoutName == "00000423" ||
				sLayoutName == "00010402" || sLayoutName == "00000804" ||
				sLayoutName == "00000406" || sLayoutName == "00000413" ||
				sLayoutName == "00000425" || sLayoutName == "0000040B" ||
				sLayoutName == "00000408" || sLayoutName == "0000040D" ||
				sLayoutName == "0000040F" || sLayoutName == "00000410" ||
				sLayoutName == "00000411" || sLayoutName == "00000412" ||
				sLayoutName == "00000426" || sLayoutName == "00000427" ||
				sLayoutName == "00000414" || sLayoutName == "00000415" ||
				sLayoutName == "00000416" || sLayoutName == "00000816" ||
				sLayoutName == "00000418" || sLayoutName == "0000040A" ||
				sLayoutName == "0000041D" || sLayoutName == "0000041F" ||
				sLayoutName == "00140C00" || sLayoutName == "0000044D" ||
				sLayoutName == "0000042C" || sLayoutName == "00000445" ||
				sLayoutName == "0000045C" || sLayoutName == "00000404" ||
				sLayoutName == "00000439" || sLayoutName == "00000438" ||
				sLayoutName == "00020437" || sLayoutName == "00000468" ||
				sLayoutName == "0000044B" || sLayoutName == "00000454" ||
				sLayoutName == "0000042F" || sLayoutName == "00090C00" ||
				sLayoutName == "00000448" || sLayoutName == "00000463" ||
				sLayoutName == "00000485" || sLayoutName == "00030C00" ||
				sLayoutName == "0000042A" || sLayoutName == "0000046D" ||
				sLayoutName == "00000429" || sLayoutName == "0002042B" ||
				sLayoutName == "0003042B" || sLayoutName == "0001042B" ||
				sLayoutName == "0001042C" || sLayoutName == "00020445" ||
				sLayoutName == "00010445" || sLayoutName == "00000813" ||
				sLayoutName == "000B0C00" || sLayoutName == "00020402" ||
				sLayoutName == "00040402" || sLayoutName == "00000402" ||
				sLayoutName == "00011009" || sLayoutName == "0000085F" ||
				sLayoutName == "0001045C" || sLayoutName == "00001004" ||
				sLayoutName == "00000C04" || sLayoutName == "00001404" ||
				sLayoutName == "00010405" || sLayoutName == "00020405" ||
				sLayoutName == "00000437" || sLayoutName == "00010408" ||
				sLayoutName == "00020408" || sLayoutName == "00030408" ||
				sLayoutName == "0000046F" || sLayoutName == "00000447" ||
				sLayoutName == "00010439" || sLayoutName == "00000470" ||
				sLayoutName == "00000421" || sLayoutName == "0000085D" ||
				sLayoutName == "0001045D" || sLayoutName == "0000043F" ||
				sLayoutName == "00000453" || sLayoutName == "00000440" ||
				sLayoutName == "0000080A" || sLayoutName == "0000046E" ||
				sLayoutName == "0000044C" || sLayoutName == "0000043A" ||
				sLayoutName == "0001043A" || sLayoutName == "00000481" ||
				sLayoutName == "0000044E" || sLayoutName == "00000450" ||
				sLayoutName == "00000850" || sLayoutName == "00000461" ||
				sLayoutName == "00010415" || sLayoutName == "00000446" ||
				sLayoutName == "00020418" || sLayoutName == "00010418" ||
				sLayoutName == "00010419" || sLayoutName == "0000083B" ||
				sLayoutName == "0000043B" || sLayoutName == "00000C1A" ||
				sLayoutName == "0000081A" || sLayoutName == "0000046C" ||
				sLayoutName == "00000432" || sLayoutName == "0000045B" ||
				sLayoutName == "00000424" || sLayoutName == "0000082E" ||
				sLayoutName == "0000042E" || sLayoutName == "0000045A" ||
				sLayoutName == "00000428" || sLayoutName == "00000449" ||
				sLayoutName == "00000444" || sLayoutName == "0000044A" ||
				sLayoutName == "0000041E" || sLayoutName == "00000451" ||
				sLayoutName == "0001041F" || sLayoutName == "00000442" ||
				sLayoutName == "00000420" || sLayoutName == "00000480" ||
				sLayoutName == "00000843" || sLayoutName == "00000452" ||
				sLayoutName == "00000488" || sLayoutName == "0000046A")
				return KeyboardLayout::QWERTY;

			// Dvorak layouts
			if (sLayoutName == "00010409" || sLayoutName == "00030409" || sLayoutName == "00040409")
				return KeyboardLayout::DVORAK;

			// Colemak layouts
			if (sLayoutName == "00060409")
				return KeyboardLayout::COLEMAK;

			// AZERTY-based layouts
			if (sLayoutName == "0000040C" || sLayoutName == "0000080C" ||
				sLayoutName == "0000100C" || sLayoutName == "0000140C" ||
				sLayoutName == "0000180C" || sLayoutName == "0001080C" ||
				sLayoutName == "00000C0C" || sLayoutName == "00020401")
				return KeyboardLayout::AZERTY;

			// QWERTZ-based layouts
			if (sLayoutName == "00000407" || sLayoutName == "00000807" ||
				sLayoutName == "00000C07" || sLayoutName == "0000040E" ||
				sLayoutName == "0000041B" || sLayoutName == "00000405" ||
				sLayoutName == "0000041A" || sLayoutName == "0000201A" ||
				sLayoutName == "0000082C")
				return KeyboardLayout::QWERTZ;
		}
		return KeyboardLayout::UNKNOWN;
	}

	void Watermark(const CEntity&) noexcept;
	void CleanTraces();
}

namespace TriggerBot
{
	inline std::chrono::time_point<std::chrono::system_clock> g_LastShotTime;
	inline std::chrono::time_point<std::chrono::system_clock> g_TargetFoundTime;
	inline bool g_HasValidTarget = false;

	static inline float RandomJitterF(float base, float stddev)
	{
		static std::random_device rd;
		static std::mt19937 rng(rd());
		std::normal_distribution<float> dist(base, stddev);
		return std::clamp(dist(rng), base * 0.5f, base + stddev * 3.f);
	}

	static inline bool CheckScopeWeapon(const std::string& weapon)
	{
		return weapon == "awp" || weapon == "ssg08" || weapon == "scar20" || weapon == "g3Sg1";
	}

	static inline std::string GetWeapon(const CEntity& local)
	{
		DWORD64 weaponServices = 0;
		if (!memoryManager.ReadMemory(local.Pawn.Address + Offset.Pawn.m_pWeaponServices, weaponServices) || weaponServices == 0)
			return local.Pawn.WeaponName;

		uint32_t activeWeaponHandle = 0;
		if (!memoryManager.ReadMemory(weaponServices + Offset.WeaponBaseData.hActiveWeapon, activeWeaponHandle) || activeWeaponHandle == 0)
			return local.Pawn.WeaponName;

		DWORD64 activeWeapon = CEntity::ResolveEntityHandle(activeWeaponHandle);
		if (activeWeapon == 0)
			return local.Pawn.WeaponName;

		short weaponIndex = -1;
		DWORD64 idxAddr = activeWeapon + Offset.EconEntity.AttributeManager + Offset.WeaponBaseData.Item + Offset.WeaponBaseData.ItemDefinitionIndex;
		if (!memoryManager.ReadMemory(idxAddr, weaponIndex) || weaponIndex == -1)
			return local.Pawn.WeaponName;

		auto it = CEntity::weaponNames.find(weaponIndex);
		return (it != CEntity::weaponNames.end()) ? it->second : "";
	}

	static inline bool CanTrigger(const CEntity& localEntity, const CEntity& targetEntity, int localPlayerControllerIndex)
	{
		if (targetEntity.Pawn.Address == 0)
			return false;

		if (MenuConfig::TeamCheck && localEntity.Pawn.TeamID == targetEntity.Pawn.TeamID)
			return false;

		bool waitForNoAttack = false;
		if (!memoryManager.ReadMemory<bool>(localEntity.Pawn.Address + Offset.Pawn.m_bWaitForNoAttack, waitForNoAttack))
			return false;
		if (waitForNoAttack)
			return false;

		std::string currentWeapon = GetWeapon(localEntity);

		if (TriggerBotCFG::HeadOnly)
		{
			// graznak weapon kontrolu — bıçak, nade, c4 vs.
			if (currentWeapon == "smokegrenade" || currentWeapon == "flashbang" || currentWeapon == "hegrenade" ||
				currentWeapon == "molotov"  || currentWeapon == "decoy"  || currentWeapon == "incgrenade" ||
				currentWeapon == "t_knife"  || currentWeapon == "ct_knife" || currentWeapon == "c4")
				return false;
		}

		if (CheckScopeWeapon(currentWeapon))
		{
			bool isScoped = false;
			memoryManager.ReadMemory<bool>(localEntity.Pawn.Address + Offset.Pawn.isScoped, isScoped);
			if (!isScoped)
				return false;
		}

		return true;
	}

	static inline void ExecuteShot()
	{
		if (GetAsyncKeyState(VK_LBUTTON) < 0)
			return;

		g_LastShotTime = std::chrono::system_clock::now();

		// hold süresi: 12ms ± 8ms jitter (her atışta farklı)
		long long holdUs = (long long)(RandomJitterF(12.f, 8.f) * 1000.f);

		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		std::this_thread::sleep_for(std::chrono::microseconds(holdUs));
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}

	// legacy compat — Cheats.cpp bunu cagiriyor
	static inline void Tick() {}

	static inline void Update(const CEntity& localEntity, const std::vector<EntityResult>& /*entities*/, int /*unused*/)
	{
		if (!TriggerBotCFG::Enabled)
			return;

		if (MenuConfig::ShowMenu)
			return;

		if (!localEntity.IsAlive())
			return;

		if (!TriggerBotCFG::AlwaysActive && !(GetAsyncKeyState(TriggerBotCFG::HotKey) & 0x8000))
			return;

		// m_iIDEntIndex'i oku — cs2'nin crosshair entity handle'i
		DWORD uHandle = 0;
		if (!memoryManager.ReadMemory<DWORD>(localEntity.Pawn.Address + Offset.Pawn.iIDEntIndex, uHandle))
		{
			g_HasValidTarget = false;
			g_TargetFoundTime = std::chrono::system_clock::now();
			return;
		}

		if (uHandle == 0 || uHandle == (DWORD)-1)
		{
			g_HasValidTarget = false;
			g_TargetFoundTime = std::chrono::system_clock::now();
			return;
		}

		DWORD64 pawnAddress = CEntity::ResolveEntityHandle(uHandle);
		if (pawnAddress == 0)
		{
			g_HasValidTarget = false;
			g_TargetFoundTime = std::chrono::system_clock::now();
			return;
		}

		CEntity targetEntity;
		if (!targetEntity.UpdatePawn(pawnAddress))
		{
			g_HasValidTarget = false;
			g_TargetFoundTime = std::chrono::system_clock::now();
			return;
		}

		if (!CanTrigger(localEntity, targetEntity, 0))
		{
			g_HasValidTarget = false;
			g_TargetFoundTime = std::chrono::system_clock::now();
			return;
		}

		if (!g_HasValidTarget)
			g_TargetFoundTime = std::chrono::system_clock::now();
		g_HasValidTarget = true;

		auto now = std::chrono::system_clock::now();
		long long timeSinceLastShot   = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_LastShotTime).count();
		long long timeSinceTargetFound= std::chrono::duration_cast<std::chrono::milliseconds>(now - g_TargetFoundTime).count();

		// jittered delay: Delay ± 20ms stddev, her hedef bulunduğunda yeniden hesaplanır
		static long long jitteredDelay = TriggerBotCFG::Delay;
		if (!g_HasValidTarget)
			jitteredDelay = (long long)RandomJitterF((float)TriggerBotCFG::Delay, 20.f);

		if (timeSinceLastShot >= 400 && timeSinceTargetFound >= jitteredDelay)
			ExecuteShot();
	}
}