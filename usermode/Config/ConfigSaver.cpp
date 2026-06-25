#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "ConfigSaver.h"
#include "../Resources/Language.h"
#include "../Core/Config.h"
#include "../Features/Misc.h"
#include "../Helpers/KeyManager.h"
#include <json.hpp>

using json = nlohmann::json;

namespace MyConfigSaver 
{
    void SaveConfig(const std::string& filename, const std::string& author) {
        std::ofstream configFile(MenuConfig::path + '\\' + filename);
        if (!configFile.is_open()) {
            return;
        }

        json ConfigData;

        ConfigData["0"]["Name"] = MenuConfig::name;
        ConfigData["0"]["Version"] = MenuConfig::version;
        ConfigData["0"]["Author"] = author;

        ConfigData["ESP"]["Hotkey"] =               ESPConfig::HotKey;
        ConfigData["ESP"]["Enable"]=                ESPConfig::ESPenabled;
        ConfigData["ESP"]["BoneESP"]=               ESPConfig::ShowBoneESP;
        ConfigData["ESP"]["BoxESP"]=                ESPConfig::ShowBoxESP;
        ConfigData["ESP"]["BoxType"]=               ESPConfig::BoxType;
        ConfigData["ESP"]["BoneColor"]["r"]=        ESPConfig::BoneColor.Value.x;
        ConfigData["ESP"]["BoneColor"]["g"]=        ESPConfig::BoneColor.Value.y;
        ConfigData["ESP"]["BoneColor"]["b"]=        ESPConfig::BoneColor.Value.z;
        ConfigData["ESP"]["BoneColor"]["a"]=        ESPConfig::BoneColor.Value.w;

        ConfigData["ESP"]["HeadBoxColor"]["r"]=     ESPConfig::HeadBoxColor.Value.x;
        ConfigData["ESP"]["HeadBoxColor"]["g"]=     ESPConfig::HeadBoxColor.Value.y;
        ConfigData["ESP"]["HeadBoxColor"]["b"]=     ESPConfig::HeadBoxColor.Value.z;
        ConfigData["ESP"]["HeadBoxColor"]["a"]=     ESPConfig::HeadBoxColor.Value.w;



        ConfigData["Radar"]["Enable"] =            RadarCFG::ShowRadar;
        ConfigData["Radar"]["Type"]=               RadarCFG::RadarType;
        ConfigData["Radar"]["Range"]=              RadarCFG::RadarRange;
        ConfigData["Radar"]["Proportion"]=         RadarCFG::Proportion;
        ConfigData["Radar"]["PointProportion"]=    RadarCFG::RadarPointSizeProportion;
        ConfigData["Radar"]["Alpha"]=              RadarCFG::RadarBgAlpha;
        ConfigData["Radar"]["Custom"]=             RadarCFG::customRadar;
        ConfigData["Radar"]["Crossline"]=          RadarCFG::ShowRadarCrossLine;

        ConfigData["Radar"]["CrosslineColor"]["r"]=         RadarCFG::RadarCrossLineColor.Value.x;
        ConfigData["Radar"]["CrosslineColor"]["g"]=         RadarCFG::RadarCrossLineColor.Value.y;
        ConfigData["Radar"]["CrosslineColor"]["b"]=         RadarCFG::RadarCrossLineColor.Value.z;
        ConfigData["Radar"]["CrosslineColor"]["a"]=         RadarCFG::RadarCrossLineColor.Value.w;



        ConfigData["Triggerbot"]["Enable"]=    TriggerBotCFG::Enabled;
        ConfigData["Triggerbot"]["Delay"]=     TriggerBotCFG::Delay;
        ConfigData["Triggerbot"]["Hotkey"]=    TriggerBotCFG::HotKey;
        ConfigData["Triggerbot"]["HeadOnly"]=  TriggerBotCFG::HeadOnly;

        ConfigData["Misc"]["WorkInSpec"]=    MenuConfig::WorkInSpec;
        ConfigData["Misc"]["Watermark"]=     MiscCFG::WaterMark;
        ConfigData["Misc"]["TeamCheck"]=     MenuConfig::TeamCheck;
        ConfigData["Misc"]["AntiRecord"]=    MenuConfig::BypassOBS;
        ConfigData["Misc"]["MenuKey"]=       MenuConfig::HotKey;

        ConfigData["MenuConfig"]["MarkWinPos"]["x"] = MenuConfig::MarkWinPos.x;
        ConfigData["MenuConfig"]["MarkWinPos"]["y"] = MenuConfig::MarkWinPos.y;

        ConfigData["MenuConfig"]["BombWinPos"]["x"] = MenuConfig::BombWinPos.x;
        ConfigData["MenuConfig"]["BombWinPos"]["y"] = MenuConfig::BombWinPos.y;

        // normalize et — farkli cozunurlukler arasi tasimak icin
        ImVec2 screen = ImGui::GetIO().DisplaySize;
        ConfigData["MenuConfig"]["RadarWinPos"]["nx"] = (screen.x > 0) ? MenuConfig::RadarWinPos.x / screen.x : 0.f;
        ConfigData["MenuConfig"]["RadarWinPos"]["ny"] = (screen.y > 0) ? MenuConfig::RadarWinPos.y / screen.y : 0.f;

        ConfigData["MenuConfig"]["SpecWinPos"]["x"] = MenuConfig::SpecWinPos.x;
        ConfigData["MenuConfig"]["SpecWinPos"]["y"] = MenuConfig::SpecWinPos.y;

        configFile << ConfigData.dump(4);
        configFile.close();
    }

    // Function to load the configuration from a file
    void LoadConfig(const std::string& filename) {

        json ConfigData;
        std::ifstream configFile(MenuConfig::path + '\\' + filename);
        if (configFile)
        {
            configFile >> ConfigData;
        }
        else
        {
            return;
        }

        if (ConfigData.contains("ESP"))
        {
            ESPConfig::ESPenabled = ReadData(ConfigData["ESP"], { "Enable" }, false);
            ESPConfig::ShowBoneESP = ReadData(ConfigData["ESP"], { "BoneESP" }, false);
            ESPConfig::ShowHeadBox = ReadData(ConfigData["ESP"], { "HeadBox" }, false);
            ESPConfig::ShowPreview = ReadData(ConfigData["ESP"], { "Preview" }, false);

            ESPConfig::BoneColor.Value.x = ReadData(ConfigData["ESP"], { "BoneColor","r" }, 0.f);
            ESPConfig::BoneColor.Value.y = ReadData(ConfigData["ESP"], { "BoneColor","g" }, 0.f);
            ESPConfig::BoneColor.Value.z = ReadData(ConfigData["ESP"], { "BoneColor","b" }, 0.f);
            ESPConfig::BoneColor.Value.w = ReadData(ConfigData["ESP"], { "BoneColor","a" }, 255.f);

            ESPConfig::HeadBoxColor.Value.x = ReadData(ConfigData["ESP"], { "HeadBoxColor","r" }, 0.f);
            ESPConfig::HeadBoxColor.Value.y = ReadData(ConfigData["ESP"], { "HeadBoxColor","g" }, 0.f);
            ESPConfig::HeadBoxColor.Value.z = ReadData(ConfigData["ESP"], { "HeadBoxColor","b" }, 0.f);
            ESPConfig::HeadBoxColor.Value.w = ReadData(ConfigData["ESP"], { "HeadBoxColor","a" }, 255.f);
        }

        if (ConfigData.contains("Radar"))
        {
            RadarCFG::ShowRadar = ReadData(ConfigData["Radar"],{"Enable"}, false);
            RadarCFG::RadarType = ReadData(ConfigData["Radar"],{"Type"}, 2);
            RadarCFG::RadarRange = ReadData(ConfigData["Radar"],{"Range"}, 150.f);
            RadarCFG::Proportion = ReadData(ConfigData["Radar"],{"Proportion"}, 3300.f);
            RadarCFG::RadarPointSizeProportion = ReadData(ConfigData["Radar"],{"PointProportion"}, 1.f);
            RadarCFG::RadarBgAlpha = ReadData(ConfigData["Radar"],{"Alpha"}, 0.1f);
            RadarCFG::customRadar = ReadData(ConfigData["Radar"],{"Custom"}, false);
            RadarCFG::ShowRadarCrossLine = ReadData(ConfigData["Radar"],{"Crossline"}, false);
            RadarCFG::RadarCrossLineColor.Value.x = ReadData(ConfigData["Radar"],{"CrosslineColor","r"}, 0.f);
            RadarCFG::RadarCrossLineColor.Value.y = ReadData(ConfigData["Radar"],{"CrosslineColor","g"}, 0.f);
            RadarCFG::RadarCrossLineColor.Value.z = ReadData(ConfigData["Radar"],{"CrosslineColor","b"}, 0.f);
            RadarCFG::RadarCrossLineColor.Value.w = ReadData(ConfigData["Radar"],{"CrosslineColor","a"}, 255.f);
        }

        if (ConfigData.contains("Triggerbot"))
        {
            TriggerBotCFG::Enabled   = ReadData(ConfigData["Triggerbot"], { "Enable" },   false);
            TriggerBotCFG::Delay     = ReadData(ConfigData["Triggerbot"], { "Delay" },    100);
            TriggerBotCFG::HotKey    = ReadData(ConfigData["Triggerbot"], { "Hotkey" },   VK_MENU);
            TriggerBotCFG::HeadOnly  = ReadData(ConfigData["Triggerbot"], { "HeadOnly" }, false);
        }

        if (ConfigData.contains("Misc"))
        {
            MenuConfig::WorkInSpec = ReadData(ConfigData["Misc"], { "WorkInSpec" }, false);
            MiscCFG::WaterMark = ReadData(ConfigData["Misc"], { "Watermark" }, false);
            MenuConfig::TeamCheck = ReadData(ConfigData["Misc"], { "TeamCheck" }, true);
            MenuConfig::BypassOBS = ReadData(ConfigData["Misc"], { "AntiRecord" }, false);
            MenuConfig::HotKey = ReadData(ConfigData["Misc"], { "MenuKey" }, VK_END);
            Text::Misc::HotKey = KeyMgr::GetKeyName(MenuConfig::HotKey);
        }

        if (ConfigData.contains("MenuConfig"))
        {
            MenuConfig::MarkWinPos.x = ReadData(ConfigData["MenuConfig"], { "MarkWinPos","x" }, ImGui::GetIO().DisplaySize.x - 300.0f);
            MenuConfig::MarkWinPos.y = ReadData(ConfigData["MenuConfig"], { "MarkWinPos","y" }, 100.f);

            MenuConfig::BombWinPos.x = ReadData(ConfigData["MenuConfig"], { "BombWinPos","x" }, (ImGui::GetIO().DisplaySize.x - 200.0f) / 2.0f);
            MenuConfig::BombWinPos.y = ReadData(ConfigData["MenuConfig"], { "BombWinPos","y" }, 80.0f);

            ImVec2 screen = ImGui::GetIO().DisplaySize;
            float rnx = ReadData(ConfigData["MenuConfig"], { "RadarWinPos","nx" }, -1.f);
            float rny = ReadData(ConfigData["MenuConfig"], { "RadarWinPos","ny" }, -1.f);
            if (rnx >= 0.f && rny >= 0.f) {
                MenuConfig::RadarWinPos.x = rnx * screen.x;
                MenuConfig::RadarWinPos.y = rny * screen.y;
            } else {
                // eski format veya yok — fallback sol ust
                MenuConfig::RadarWinPos.x = ReadData(ConfigData["MenuConfig"], { "RadarWinPos","x" }, 35.f);
                MenuConfig::RadarWinPos.y = ReadData(ConfigData["MenuConfig"], { "RadarWinPos","y" }, 35.f);
            }

            MenuConfig::SpecWinPos.x = ReadData(ConfigData["MenuConfig"], { "SpecWinPos","x" }, 10.0f);
            MenuConfig::SpecWinPos.y = ReadData(ConfigData["MenuConfig"], { "SpecWinPos","y" }, ImGui::GetIO().DisplaySize.y / 2 - 200);

            MenuConfig::MarkWinChengePos = true;
            MenuConfig::BombWinChengePos = true;
            MenuConfig::RadarWinChengePos = true;
            MenuConfig::SpecWinChengePos = true;
        }
    }
}