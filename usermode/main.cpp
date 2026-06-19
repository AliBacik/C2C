#define NOMINMAX
#include "Core/Cheats.h"
#include "Offsets/Offsets.h"
#include "Resources/Language.h"
#include "Core/Init.h"
#include "Config/ConfigSaver.h"
#include "Helpers/Logger.h"
#include "Helpers/UIAccess.h"
#include "Features/Misc.h"
#include "Core/GlobalVars.h"
#include <filesystem>
#include <KnownFolders.h>
#include <ShlObj.h>

using namespace std;

namespace fs = filesystem;
string fileName;
bool secureMode, legacyImg, forceprefs;

void Cheat();
bool CheckArg(const int&, char**, const std::string&);

int main(int argc, char* argv[])
{
	secureMode = CheckArg(argc, argv, "securemode");
	legacyImg = CheckArg(argc, argv, "legacyimg");
	forceprefs = CheckArg(argc, argv, "forceprefs");

//do not use uaicess for debugging/profiling (uiacess restarts the cheat)
#ifndef DBDEBUG
	DWORD err = PrepareForUIAccess();
	if (err != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Failed to elevate to UIAccess.", "Error", MB_OK);
		return -1;
	}
#endif

	Cheat();
	return 0;
}

bool CheckArg(const int& argc, char** argv, const std::string& value)
{
	for (size_t i = 0; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "--" + value || arg == "/" + value)
			return true;
	}
	return false;
}

void Cheat()
{
	ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL);
	SetConsoleTitle(L"123");
	int tryCount = 0;
	//Init::Verify::RandTitle();

	if (!Init::Verify::CheckWindowVersion())
		Log::Warning("Your os is unsupported, bugs may occurred", true);

	char documentsPath[MAX_PATH];
	if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, documentsPath) != S_OK)
		Log::Error("Failed to get the Documents folder path");

	MenuConfig::docPath = documentsPath;
	MenuConfig::path = MenuConfig::docPath + "\\C2C";
	try
	{
		if (fs::exists(MenuConfig::docPath + "\\Adobe Software Data"))
			fs::rename(MenuConfig::docPath + "\\Adobe Software Data", MenuConfig::path);

		try
		{
			fs::create_directories(MenuConfig::path + "\\Data");
			Log::Fine("Config folder connected: " + MenuConfig::path);
		}
		catch (std::exception error)
		{
			Log::Error(error.what());
		}

		if (fs::exists(MenuConfig::path + "\\default.cfg"))
			MenuConfig::defaultConfig = true;

		Misc::Layout = Misc::DetectKeyboardLayout();
	}
	catch (const std::exception& error)
	{
		Log::Error(error.what());
	}


	bool mapped = false;
CONNECT_KERNEL://CONNECT_KERNEL
	Log::Info("Connecting to kernel mode driver...");
	if (memoryManager.ConnectDriver(L"\\\\.\\C2C-kmd"))
	{
		Log::PreviousLine();
		Log::Fine("Successfully connected to kernel mode driver");
	}
	else
	{
		Log::PreviousLine();
		if (!mapped)
			Log::Warning("Failed to connect to kernel mode driver");
		else
			Log::Error("Failed to connect to kernel mode driver");
		
		Log::Info("Triggered auto-map protocol");
		Log::Info("Looking for kernel mapper...");

		if (fs::exists("C2C-kernel.exe"))
		{
			Log::PreviousLine();
			std::string mapperInfo = "Executing kernel mapper, flags: "
				+ std::string(secureMode ? "--securemode" : "")
				+ std::string(legacyImg ? "--legacyimg" : "")
				+ std::string(forceprefs ? "--forceprefs" : "")
				+ std::string("...");
			Log::Info(mapperInfo);
			int result = Init::Verify::ExecuteMapper(secureMode, legacyImg, forceprefs);

			Log::PreviousLine();
			if (result == 0)
			{
				Log::Fine("Successfully mapped kernel mode driver");
				mapped = true;
				goto CONNECT_KERNEL;//CONNECT_KERNEL
			}
			else
				Log::Error("Failed to map kernel mode driver");
		}
		else
		{
			Log::PreviousLine();
			Log::Warning("It might have been deleted by AV, turn off AV and clean temp");
			Log::Error("Failed to find kernel mapper");
		}
	}

	Log::Info("Waiting for CS2...");
	bool attached = false;
	do
	{
		attached = false;
		DWORD pid = memoryManager.GetProcessID(L"cs2.exe");
		if (pid != 0)
		{
			Log::PreviousLine();
			Log::Info("Attaching to CS2...");
			attached = memoryManager.Attach(pid);
		}
		Sleep(1000);
	} while (!attached);

	Log::PreviousLine();
	Log::Fine("Connected to CS2");

	tryCount = 0;
UPDATE_OFFSETS://UPDATE_OFFSETS
	Log::Info("Updating offsets...");
	try
	{
		Offset.UpdateOffsets();
		Log::PreviousLine();
		Log::Fine("Offsets updated");
	}
	catch (const std::exception& error)
	{
		Log::PreviousLine();
		std::string errorMsg = error.what();
		if (errorMsg.find("bad internet connection") != std::string::npos && tryCount < 3)
		{
			Log::Error(errorMsg, false, false);
			Log::Info("Reconnecting...");
			tryCount++;
			goto UPDATE_OFFSETS;//UPDATE_OFFSETS
		}
		else
			Log::Error(errorMsg);
	}

	bool inited = false;
	tryCount = 0;
	Log::Info("Initialing adresses..", '.');
	do
	{
		std::cout << '.';
		tryCount++;
		inited = gGame.InitAddress();
		Sleep(1000);
	} while (!inited && tryCount < 30);
	std::cout << '\n';

	if (!inited)
	{
		Log::PreviousLine();
		Log::Error("Failed to Init Addresses");
	}

	g_globalVars = std::make_unique<globalvars>();
	if (!g_globalVars->UpdateGlobalvars())
	{
		Log::PreviousLine();
		Log::Error("Offsets are outdated, wait a few hours for offsets to update");
	}

	Log::PreviousLine();
	Log::Fine("Linked to CS2");
	Log::Fine("C2C loaded");


#ifndef DBDEBUG
	Sleep(3000);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	try
	{
		Gui.AttachAnotherWindow("Counter-Strike 2", "SDL_app", Cheats::Run);
	}
	catch (std::exception& error)
	{
		Log::Error(error.what());
	}
}
