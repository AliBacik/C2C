#include "Offsets.h"
#include "../Helpers/SchemaSystem.h"
#include "../Helpers/PatternScan.h"
#include <iostream>

Offsets::Offsets() {}
Offsets::~Offsets() {}

// client.dll ve schemasystem.dll module boyutlari (genis tutuyoruz, scan erken cikar zaten)
static constexpr SIZE_T CLIENT_SCAN_SIZE   = 0x6000000; // ~96MB
static constexpr SIZE_T SCHEMA_MODULE_SIZE = 0x800000;  // ~8MB

void Offsets::UpdateOffsets()
{
    std::cout << "[Offsets] Runtime offset tarama basliyor...\n";

    DWORD64 clientBase = memoryManager.GetModuleBase(L"client.dll");
    DWORD64 schemaBase = memoryManager.GetModuleBase(L"schemasystem.dll");

    if (clientBase == 0) {
        std::cout << "[Offsets] HATA: client.dll bulunamadi\n";
        return;
    }
    if (schemaBase == 0) {
        std::cout << "[Offsets] HATA: schemasystem.dll bulunamadi\n";
        return;
    }

    // ----------------------------------------------------------------
    // 1. Schema — class field offset'leri
    // ----------------------------------------------------------------
    if (!g_schema.Init(schemaBase)) {
        std::cout << "[Offsets] HATA: SchemaSystem init basarisiz\n";
        return;
    }

    auto get = [&](const char* cls, const char* field) -> DWORD {
        DWORD v = g_schema.Get(cls, field);
        if (v == 0)
            std::cout << "[Offsets] UYARI: " << cls << "::" << field << " = 0\n";
        return v;
    };

    this->Entity.IsAlive       = get("CCSPlayerController",  "m_bPawnIsAlive");
    this->Entity.PlayerPawn    = get("CCSPlayerController",  "m_hPlayerPawn");
    this->Entity.iszPlayerName = get("CBasePlayerController","m_iszPlayerName");

    this->Pawn.BulletServices      = get("C_CSPlayerPawn",              "m_pBulletServices");
    this->Pawn.CameraServices      = get("C_BasePlayerPawn",            "m_pCameraServices");
    this->Pawn.pClippingWeapon     = get("C_CSPlayerPawn",              "m_pClippingWeapon");
    this->Pawn.isScoped            = get("C_CSPlayerPawn",              "m_bIsScoped");
    this->Pawn.isDefusing          = get("C_CSPlayerPawn",              "m_bIsDefusing");
    this->Pawn.TotalHit            = get("CCSPlayer_BulletServices",    "m_totalHitsOnServer");
    this->Pawn.Pos                 = get("C_BaseEntity",                "m_vOldOrigin");
    this->Pawn.CurrentArmor        = get("C_CSPlayerPawn",              "m_ArmorValue");
    this->Pawn.MaxHealth           = get("C_BaseEntity",                "m_iMaxHealth");
    this->Pawn.CurrentHealth       = get("C_BaseEntity",                "m_iHealth");
    this->Pawn.GameSceneNode       = get("C_BaseEntity",                "m_pGameSceneNode");
    this->Pawn.BoneArray           = get("CSkeletonInstance",           "m_modelState") + 0x80;
    this->Pawn.angEyeAngles        = get("C_CSPlayerPawn",              "m_angEyeAngles");
    this->Pawn.vecLastClipCameraPos= get("C_CSPlayerPawn",              "m_vecLastClipCameraPos");
    this->Pawn.iShotsFired         = get("C_CSPlayerPawn",              "m_iShotsFired");
    this->Pawn.flFlashDuration     = get("C_CSPlayerPawnBase",          "m_flFlashDuration");
    this->Pawn.aimPunchAngle       = get("C_CSPlayerPawn",              "m_aimPunchAngle");
    this->Pawn.aimPunchCache       = get("C_CSPlayerPawn",              "m_aimPunchCache");
    this->Pawn.iIDEntIndex         = get("C_CSPlayerPawn",              "m_iIDEntIndex");
    this->Pawn.iTeamNum            = get("C_BaseEntity",                "m_iTeamNum");
    this->Pawn.iFovStart           = get("CCSPlayerBase_CameraServices","m_iFOVStart");
    this->Pawn.fFlags              = get("C_BaseEntity",                "m_fFlags");
    this->Pawn.bSpottedByMask      = get("C_CSPlayerPawn",              "m_entitySpottedState")
                                   + get("EntitySpottedState_t",         "m_bSpottedByMask");
    this->Pawn.AbsVelocity         = get("C_BaseEntity",                "m_vecAbsVelocity");
    this->Pawn.m_bWaitForNoAttack  = get("C_CSPlayerPawn",              "m_bWaitForNoAttack");
    this->Pawn.m_pWeaponServices   = get("C_BasePlayerPawn",            "m_pWeaponServices");
    this->Pawn.m_flEmitSoundTime   = get("C_CSPlayerPawn",              "m_flEmitSoundTime");

    this->PlayerController.m_nTickBase         = get("CBasePlayerController",   "m_nTickBase");
    this->PlayerController.m_steamID           = get("CBasePlayerController",   "m_steamID");
    this->PlayerController.m_hPawn             = get("CBasePlayerController",   "m_hPawn");
    this->PlayerController.m_pObserverServices = get("C_BasePlayerPawn",        "m_pObserverServices");
    this->PlayerController.m_hObserverTarget   = get("CPlayer_ObserverServices","m_hObserverTarget");
    this->PlayerController.m_hController       = get("C_BasePlayerPawn",        "m_hController");
    this->PlayerController.PawnArmor           = get("CCSPlayerController",     "m_iPawnArmor");
    this->PlayerController.HasDefuser          = get("CCSPlayerController",     "m_bPawnHasDefuser");
    this->PlayerController.HasHelmet           = get("CCSPlayerController",     "m_bPawnHasHelmet");

    this->EconEntity.AttributeManager          = get("C_EconEntity",            "m_AttributeManager");

    this->WeaponBaseData.WeaponDataPTR         = get("C_BaseEntity",            "m_nSubclassID") + 0x08;
    this->WeaponBaseData.szName                = get("CCSWeaponBaseVData",       "m_szName");
    this->WeaponBaseData.Clip1                 = get("C_BasePlayerWeapon",       "m_iClip1");
    this->WeaponBaseData.MaxClip               = get("CBasePlayerWeaponVData",   "m_iMaxClip1");
    this->WeaponBaseData.Item                  = get("C_AttributeContainer",     "m_Item");
    this->WeaponBaseData.ItemDefinitionIndex   = get("C_EconItemView",           "m_iItemDefinitionIndex");
    this->WeaponBaseData.hMyWeapons            = get("CPlayer_WeaponServices",   "m_hMyWeapons");

    this->C4.m_bBeingDefused     = get("C_PlantedC4", "m_bBeingDefused");
    this->C4.m_flDefuseCountDown = get("C_PlantedC4", "m_flDefuseCountDown");
    this->C4.m_flC4Blow          = get("C_PlantedC4", "m_flC4Blow");
    this->C4.m_flTimerLength     = get("C_PlantedC4", "m_flTimerLength");
    this->C4.m_nBombSite         = get("C_PlantedC4", "m_nBombSite");

    // ----------------------------------------------------------------
    // 2. Pattern scan — global pointer'lar (client.dll + offset)
    //    Deadlocked-rust find_offsets.rs ile ayni pattern'ler.
    // ----------------------------------------------------------------

    // dwLocalPlayerPawn: "48 83 3D ? ? ? ? 00 0F 95 C0 C3"
    {
        DWORD64 match = PatternScan::Scan(clientBase, CLIENT_SCAN_SIZE,
            "48 83 3D ? ? ? ? 00 0F 95 C0 C3");
        if (match) {
            DWORD64 abs = PatternScan::ResolveRelative(match, 3, 8);
            this->LocalPlayerPawn = (DWORD)(abs - clientBase);
            std::cout << "[Offsets] dwLocalPlayerPawn = 0x" << std::hex << this->LocalPlayerPawn << "\n";
        } else {
            std::cout << "[Offsets] HATA: dwLocalPlayerPawn pattern bulunamadi\n";
        }
    }

    // dwViewMatrix: "C6 83 ? ? 00 00 01 4C 8D 05"
    {
        DWORD64 match = PatternScan::Scan(clientBase, CLIENT_SCAN_SIZE,
            "C6 83 ? ? 00 00 01 4C 8D 05");
        if (match) {
            DWORD64 abs = PatternScan::ResolveRelative(match + 0x0A, 0, 4);
            this->Matrix = (DWORD)(abs - clientBase);
            std::cout << "[Offsets] dwViewMatrix = 0x" << std::hex << this->Matrix << "\n";
        } else {
            std::cout << "[Offsets] HATA: dwViewMatrix pattern bulunamadi\n";
        }
    }

    // dwEntityList: "4C 8B 0D ? ? ? ? 4D 85 C9"
    {
        DWORD64 match = PatternScan::Scan(clientBase, CLIENT_SCAN_SIZE,
            "4C 8B 0D ? ? ? ? 4D 85 C9");
        if (match) {
            DWORD64 abs = PatternScan::ResolveRelative(match, 3, 7);
            this->EntityList = (DWORD)(abs - clientBase);
            std::cout << "[Offsets] dwEntityList = 0x" << std::hex << this->EntityList << "\n";
        } else {
            std::cout << "[Offsets] HATA: dwEntityList pattern bulunamadi\n";
        }
    }

    // dwLocalPlayerController: "48 8B 05 ? ? ? ? 48 85 C0 74 ? 8B 88"
    {
        DWORD64 match = PatternScan::Scan(clientBase, CLIENT_SCAN_SIZE,
            "48 8B 05 ? ? ? ? 48 85 C0 74 ? 8B 88");
        if (match) {
            DWORD64 abs = PatternScan::ResolveRelative(match, 3, 7);
            this->LocalPlayerController = (DWORD)(abs - clientBase);
            std::cout << "[Offsets] dwLocalPlayerController = 0x" << std::hex << this->LocalPlayerController << "\n";
        } else {
            std::cout << "[Offsets] HATA: dwLocalPlayerController pattern bulunamadi\n";
        }
    }

    // dwGlobalVars: "48 8D 05 ? ? ? ? 48 8B 00 8B 48 ? E9"
    {
        DWORD64 match = PatternScan::Scan(clientBase, CLIENT_SCAN_SIZE,
            "48 8D 05 ? ? ? ? 48 8B 00 8B 48 ? E9");
        if (match) {
            DWORD64 abs = PatternScan::ResolveRelative(match, 3, 7);
            this->GlobalVars = (DWORD)(abs - clientBase);
            std::cout << "[Offsets] dwGlobalVars = 0x" << std::hex << this->GlobalVars << "\n";
        } else {
            std::cout << "[Offsets] HATA: dwGlobalVars pattern bulunamadi\n";
        }
    }

    // dwPlantedC4: "48 8D 35 ? ? ? ? 66 0F EF C0 C6 05 ? ? ? ? 01 48 8D 3D"
    {
        DWORD64 match = PatternScan::Scan(clientBase, CLIENT_SCAN_SIZE,
            "48 8D 35 ? ? ? ? 66 0F EF C0 C6 05 ? ? ? ? 01 48 8D 3D");
        if (match) {
            DWORD64 abs = PatternScan::ResolveRelative(match, 3, 7);
            this->PlantedC4 = (DWORD)(abs - clientBase);
            std::cout << "[Offsets] dwPlantedC4 = 0x" << std::hex << this->PlantedC4 << "\n";
        } else {
            std::cout << "[Offsets] HATA: dwPlantedC4 pattern bulunamadi\n";
        }
    }

    // dwViewAngles: "48 8B 0D ? ? ? ? F3 0F 11 51 ? F3 0F 11 59"
    {
        DWORD64 match = PatternScan::Scan(clientBase, CLIENT_SCAN_SIZE,
            "48 8B 0D ? ? ? ? F3 0F 11 51 ? F3 0F 11 59");
        if (match) {
            DWORD64 abs = PatternScan::ResolveRelative(match, 3, 7);
            this->ViewAngle = (DWORD)(abs - clientBase);
            std::cout << "[Offsets] dwViewAngles = 0x" << std::hex << this->ViewAngle << "\n";
        } else {
            std::cout << "[Offsets] HATA: dwViewAngles pattern bulunamadi\n";
        }
    }

    // dwInputSystem (inputsystem.dll): interface export pointer
    {
        DWORD64 inputBase = memoryManager.GetModuleBase(L"inputsystem.dll");
        if (inputBase) {
            // "48 8D 05 ? ? ? ? C3 CC CC CC CC 48 8D 05 ? ? ? ? C3 CC CC CC CC 48 8D 05"
            DWORD64 match = PatternScan::Scan(inputBase, 0x100000,
                "48 8D 05 ? ? ? ? C3 CC CC CC CC 48 8D 05");
            if (match) {
                DWORD64 abs = PatternScan::ResolveRelative(match, 3, 7);
                this->InputSystem = (DWORD)(abs - inputBase);
                std::cout << "[Offsets] dwInputSystem = 0x" << std::hex << this->InputSystem << "\n";
            } else {
                std::cout << "[Offsets] HATA: dwInputSystem pattern bulunamadi\n";
            }
        }
    }

    // Buttons — dwAttack, dwJump, dwLeft, dwRight
    // Pattern: "48 8D 05 ? ? ? ? C3" her buton icin farkli context'te
    // En guvenilir yol: "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 05" gibi buton-specific pattern
    {
        // attack: "F3 0F 10 05 ? ? ? ? F3 0F 59 C1"
        DWORD64 match = PatternScan::Scan(clientBase, CLIENT_SCAN_SIZE,
            "F3 0F 10 05 ? ? ? ? F3 0F 59 C1");
        if (match) {
            DWORD64 abs = PatternScan::ResolveRelative(match, 4, 8);
            this->Buttons.Attack = (DWORD)(abs - clientBase);
            std::cout << "[Offsets] attack = 0x" << std::hex << this->Buttons.Attack << "\n";
        } else {
            std::cout << "[Offsets] UYARI: attack pattern bulunamadi\n";
        }
    }
    {
        // jump: "48 8D 0D ? ? ? ? E8 ? ? ? ? 84 C0 75 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 84 C0 74"
        DWORD64 match = PatternScan::Scan(clientBase, CLIENT_SCAN_SIZE,
            "48 8D 0D ? ? ? ? E8 ? ? ? ? 84 C0 75 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 84 C0 74");
        if (match) {
            DWORD64 abs = PatternScan::ResolveRelative(match, 3, 7);
            this->Buttons.Jump = (DWORD)(abs - clientBase);
            std::cout << "[Offsets] jump = 0x" << std::hex << this->Buttons.Jump << "\n";
        } else {
            std::cout << "[Offsets] UYARI: jump pattern bulunamadi\n";
        }
    }

    // Sensitivity (cvar pointer)
    {
        DWORD64 match = PatternScan::Scan(clientBase, CLIENT_SCAN_SIZE,
            "48 8B 0D ? ? ? ? 4C 8B C6 F3 0F 10 0D");
        if (match) {
            DWORD64 abs = PatternScan::ResolveRelative(match, 3, 7);
            this->Sensitivity = (DWORD)(abs - clientBase);
            std::cout << "[Offsets] dwSensitivity = 0x" << std::hex << this->Sensitivity << "\n";
        }
        // sensitivity_sensitivity offset schemasystem'de yok, sabit offset
        this->Sensitivity_sensitivity = 0x58;
    }

    // GlobalVar struct offsetleri (engine sabitleri, CS2'de degismez)
    this->GlobalVar.RealTime         = 0x00;
    this->GlobalVar.FrameCount       = 0x04;
    this->GlobalVar.MaxClients       = 0x10;
    this->GlobalVar.IntervalPerTick  = 0x14;
    this->GlobalVar.CurrentTime      = 0x30;
    this->GlobalVar.CurrentTime2     = 0x38;
    this->GlobalVar.TickCount        = 0x48;
    this->GlobalVar.IntervalPerTick2 = 0x44;
    this->GlobalVar.CurrentNetchan   = 0x0048;
    this->GlobalVar.CurrentMap       = 0x0180;
    this->GlobalVar.CurrentMapName   = 0x0188;

    std::cout << "[Offsets] Tum offset'ler hazir.\n";
}
