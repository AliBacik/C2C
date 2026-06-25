#include "Offsets.h"
#include "../Helpers/StorageMgr.h"
#include "../Helpers/WebApi.h"
#include "../Libs/json/json.hpp"
#include "../Helpers/Logger.h"
#include <iostream>

using json = nlohmann::json;

Offsets::Offsets() {}
Offsets::~Offsets() {}

static DWORD SafeGetDWORD(const json& obj, const std::vector<std::string>& keys, DWORD defaultValue = 0)
{
    const json* current = &obj;
    for (const auto& key : keys)
    {
        if (!current->contains(key) || current->at(key).is_null())
            return defaultValue;
        current = &current->at(key);
    }
    if (current->is_number_unsigned() || current->is_number_integer() || current->is_number_float())
        return current->get<DWORD>();
    if (current->is_object() && current->contains("offset"))
        return SafeGetDWORD(*current, {"offset"}, defaultValue);
    return defaultValue;
}

void Offsets::LoadFromJson()
{
    std::string offsetsData  = storage::ReadStorageFile("offsets.json");
    std::string buttonsData  = storage::ReadStorageFile("buttons.json");
    std::string clientDllData= storage::ReadStorageFile("client_dll.json");
    SetOffsets(offsetsData, buttonsData, clientDllData);
}

void Offsets::SetOffsets(const std::string& offsetsData, const std::string& buttonsData, const std::string& client_dllData)
{
    json offsetsJson    = json::parse(offsetsData);
    json buttonsJson    = json::parse(buttonsData);
    json client_dllJson = json::parse(client_dllData)["client.dll"]["classes"];

    this->EntityList             = SafeGetDWORD(offsetsJson, {"client.dll", "dwEntityList"});
    this->Matrix                 = SafeGetDWORD(offsetsJson, {"client.dll", "dwViewMatrix"});
    this->ViewAngle              = SafeGetDWORD(offsetsJson, {"client.dll", "dwViewAngles"});
    this->LocalPlayerController  = SafeGetDWORD(offsetsJson, {"client.dll", "dwLocalPlayerController"});
    this->LocalPlayerPawn        = SafeGetDWORD(offsetsJson, {"client.dll", "dwLocalPlayerPawn"});
    this->GlobalVars             = SafeGetDWORD(offsetsJson, {"client.dll", "dwGlobalVars"});
    this->PlantedC4              = SafeGetDWORD(offsetsJson, {"client.dll", "dwPlantedC4"});
    this->InputSystem            = SafeGetDWORD(offsetsJson, {"inputsystem.dll", "dwInputSystem"});
    this->Sensitivity            = SafeGetDWORD(offsetsJson, {"client.dll", "dwSensitivity"});
    this->Sensitivity_sensitivity= SafeGetDWORD(offsetsJson, {"client.dll", "dwSensitivity_sensitivity"});

    this->Buttons.Attack = SafeGetDWORD(buttonsJson, {"client.dll", "attack"});
    this->Buttons.Jump   = SafeGetDWORD(buttonsJson, {"client.dll", "jump"});
    this->Buttons.Right  = SafeGetDWORD(buttonsJson, {"client.dll", "right"});
    this->Buttons.Left   = SafeGetDWORD(buttonsJson, {"client.dll", "left"});

    this->Entity.IsAlive       = SafeGetDWORD(client_dllJson, {"CCSPlayerController",   "fields", "m_bPawnIsAlive"});
    this->Entity.PlayerPawn    = SafeGetDWORD(client_dllJson, {"CCSPlayerController",   "fields", "m_hPlayerPawn"});
    this->Entity.iszPlayerName = SafeGetDWORD(client_dllJson, {"CBasePlayerController", "fields", "m_iszPlayerName"});

    this->Pawn.BulletServices      = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_pBulletServices"});
    this->Pawn.CameraServices      = SafeGetDWORD(client_dllJson, {"C_BasePlayerPawn",            "fields", "m_pCameraServices"});
    this->Pawn.pClippingWeapon     = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_pClippingWeapon"});
    this->Pawn.isScoped            = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_bIsScoped"});
    this->Pawn.isDefusing          = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_bIsDefusing"});
    this->Pawn.TotalHit            = SafeGetDWORD(client_dllJson, {"CCSPlayer_BulletServices",    "fields", "m_totalHitsOnServer"});
    this->Pawn.Pos                 = SafeGetDWORD(client_dllJson, {"C_BasePlayerPawn",            "fields", "m_vOldOrigin"});
    this->Pawn.CurrentArmor        = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_ArmorValue"});
    this->Pawn.MaxHealth           = SafeGetDWORD(client_dllJson, {"C_BaseEntity",                "fields", "m_iMaxHealth"});
    this->Pawn.CurrentHealth       = SafeGetDWORD(client_dllJson, {"C_BaseEntity",                "fields", "m_iHealth"});
    this->Pawn.GameSceneNode       = SafeGetDWORD(client_dllJson, {"C_BaseEntity",                "fields", "m_pGameSceneNode"});
    this->Pawn.BoneArray           = SafeGetDWORD(client_dllJson, {"CSkeletonInstance",           "fields", "m_modelState"}) + 0x80;
    this->Pawn.angEyeAngles        = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_angEyeAngles"});
    this->Pawn.vecLastClipCameraPos= SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_vecLastClipCameraPos"});
    this->Pawn.iShotsFired         = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_iShotsFired"});
    this->Pawn.flFlashDuration     = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawnBase",          "fields", "m_flFlashDuration"});
    this->Pawn.aimPunchAngle       = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_aimPunchAngle"});
    this->Pawn.aimPunchCache       = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_aimPunchCache"});
    this->Pawn.iIDEntIndex         = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_iIDEntIndex"});
    this->Pawn.iTeamNum            = SafeGetDWORD(client_dllJson, {"C_BaseEntity",                "fields", "m_iTeamNum"});
    this->Pawn.iFovStart           = SafeGetDWORD(client_dllJson, {"CCSPlayerBase_CameraServices","fields", "m_iFOVStart"});
    this->Pawn.fFlags              = SafeGetDWORD(client_dllJson, {"C_BaseEntity",                "fields", "m_fFlags"});
    this->Pawn.bSpottedByMask      = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_entitySpottedState"})
                                   + SafeGetDWORD(client_dllJson, {"EntitySpottedState_t",         "fields", "m_bSpottedByMask"});
    this->Pawn.AbsVelocity         = SafeGetDWORD(client_dllJson, {"C_BaseEntity",                "fields", "m_vecAbsVelocity"});
    this->Pawn.m_bWaitForNoAttack  = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_bWaitForNoAttack"});
    this->Pawn.m_pWeaponServices   = SafeGetDWORD(client_dllJson, {"C_BasePlayerPawn",            "fields", "m_pWeaponServices"});
    this->Pawn.m_flEmitSoundTime   = SafeGetDWORD(client_dllJson, {"C_CSPlayerPawn",              "fields", "m_flEmitSoundTime"});

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

    this->PlayerController.m_nTickBase         = SafeGetDWORD(client_dllJson, {"CBasePlayerController",    "fields", "m_nTickBase"});
    this->PlayerController.m_steamID           = SafeGetDWORD(client_dllJson, {"CBasePlayerController",    "fields", "m_steamID"});
    this->PlayerController.m_hPawn             = SafeGetDWORD(client_dllJson, {"CBasePlayerController",    "fields", "m_hPawn"});
    this->PlayerController.m_pObserverServices = SafeGetDWORD(client_dllJson, {"C_BasePlayerPawn",         "fields", "m_pObserverServices"});
    this->PlayerController.m_hObserverTarget   = SafeGetDWORD(client_dllJson, {"CPlayer_ObserverServices", "fields", "m_hObserverTarget"});
    this->PlayerController.m_hController       = SafeGetDWORD(client_dllJson, {"C_BasePlayerPawn",         "fields", "m_hController"});
    this->PlayerController.PawnArmor           = SafeGetDWORD(client_dllJson, {"CCSPlayerController",      "fields", "m_iPawnArmor"});
    this->PlayerController.HasDefuser          = SafeGetDWORD(client_dllJson, {"CCSPlayerController",      "fields", "m_bPawnHasDefuser"});
    this->PlayerController.HasHelmet           = SafeGetDWORD(client_dllJson, {"CCSPlayerController",      "fields", "m_bPawnHasHelmet"});

    this->EconEntity.AttributeManager          = SafeGetDWORD(client_dllJson, {"C_EconEntity",             "fields", "m_AttributeManager"});

    this->WeaponBaseData.WeaponDataPTR         = SafeGetDWORD(client_dllJson, {"C_BaseEntity",             "fields", "m_nSubclassID"}) + 0x08;
    this->WeaponBaseData.szName                = SafeGetDWORD(client_dllJson, {"CCSWeaponBaseVData",        "fields", "m_szName"});
    this->WeaponBaseData.Clip1                 = SafeGetDWORD(client_dllJson, {"C_BasePlayerWeapon",        "fields", "m_iClip1"});
    this->WeaponBaseData.MaxClip               = SafeGetDWORD(client_dllJson, {"CBasePlayerWeaponVData",    "fields", "m_iMaxClip1"});
    this->WeaponBaseData.Item                  = SafeGetDWORD(client_dllJson, {"C_AttributeContainer",      "fields", "m_Item"});
    this->WeaponBaseData.ItemDefinitionIndex   = SafeGetDWORD(client_dllJson, {"C_EconItemView",            "fields", "m_iItemDefinitionIndex"});
    this->WeaponBaseData.hMyWeapons            = SafeGetDWORD(client_dllJson, {"CPlayer_WeaponServices",    "fields", "m_hMyWeapons"});
    this->WeaponBaseData.hActiveWeapon         = SafeGetDWORD(client_dllJson, {"CPlayer_WeaponServices",    "fields", "m_hActiveWeapon"});

    this->C4.m_bBeingDefused     = SafeGetDWORD(client_dllJson, {"C_PlantedC4", "fields", "m_bBeingDefused"});
    this->C4.m_flDefuseCountDown = SafeGetDWORD(client_dllJson, {"C_PlantedC4", "fields", "m_flDefuseCountDown"});
    this->C4.m_flC4Blow          = SafeGetDWORD(client_dllJson, {"C_PlantedC4", "fields", "m_flC4Blow"});
    this->C4.m_flTimerLength     = SafeGetDWORD(client_dllJson, {"C_PlantedC4", "fields", "m_flTimerLength"});
    this->C4.m_nBombSite         = SafeGetDWORD(client_dllJson, {"C_PlantedC4", "fields", "m_nBombSite"});

    Log::Fine("[Offsets] Tum offsetler yuklendi.");
}

void Offsets::UpdateOffsets()
{
    std::string offsets, buttons, client_dll;

    auto hasValidPayload = [](const std::string& data) {
        return !data.empty() && data.find("{") != std::string::npos;
    };

    bool hasLocalFiles = storage::FileExists("offsets.json") &&
                         storage::FileExists("buttons.json") &&
                         storage::FileExists("client_dll.json");
    bool hasValidLocalData = false;

    if (hasLocalFiles)
    {
        try
        {
            offsets    = storage::ReadStorageFile("offsets.json");
            buttons    = storage::ReadStorageFile("buttons.json");
            client_dll = storage::ReadStorageFile("client_dll.json");
            hasValidLocalData = hasValidPayload(offsets) && hasValidPayload(buttons) && hasValidPayload(client_dll);
        }
        catch (...) { hasValidLocalData = false; }
    }

    bool shouldDownload = !hasValidLocalData;

    if (shouldDownload)
    {
        if (!hasLocalFiles)
            Log::Info("[Offsets] Yerel offset dosyasi bulunamadi, GitHub'dan indiriliyor...");
        else
            Log::Warning("[Offsets] Yerel offset dosyasi bozuk, yeni indiriliyor...");

        try
        {
            offsets    = Web::Get("https://raw.githubusercontent.com/some-random-guy-ah/cs2-offsets/refs/heads/main/offsets.json");
            buttons    = Web::Get("https://raw.githubusercontent.com/some-random-guy-ah/cs2-offsets/refs/heads/main/buttons.json");
            client_dll = Web::Get("https://raw.githubusercontent.com/some-random-guy-ah/cs2-offsets/refs/heads/main/client_dll.json");

            if (!hasValidPayload(offsets) || !hasValidPayload(buttons) || !hasValidPayload(client_dll))
                throw std::runtime_error("Indirilen offset verisi gecersiz");

            storage::WriteStorageFile("offsets.json",    offsets);
            storage::WriteStorageFile("buttons.json",    buttons);
            storage::WriteStorageFile("client_dll.json", client_dll);
            Log::Fine("[Offsets] GitHub'dan basariyla indirildi ve kaydedildi.");
        }
        catch (const std::exception& e)
        {
            Log::Error(std::string("[Offsets] Indirme basarisiz: ") + e.what());
            throw std::runtime_error("Offset yuklenemedi. Data klasorune offsets.json/buttons.json/client_dll.json koyun.");
        }
    }
    else
    {
        Log::Info("[Offsets] Yerel offset dosyalari okunuyor...");
    }

    SetOffsets(offsets, buttons, client_dll);
}
