#pragma once
#include "Main.h"
#include "GunMod.h"
#include "Glow.h"
#include "ESP.h"
#include "PlayerMod.h"
#include "Misc.h"
#include "Utility.h"
#include "ConsoleUI.h"

struct Util {
	struct Timer {

		std::chrono::time_point<std::chrono::steady_clock> start;
		std::chrono::duration<float> duration;
		std::string _name;

		Timer(std::string name) {
			start = std::chrono::high_resolution_clock::now();
			_name = name;
		}

		~Timer() {
			duration = std::chrono::high_resolution_clock::now() - start;
			std::cout << _name << " took " << duration.count() << "s" << std::endl;
		}
	};
};

void Setup() {
	while (!Offsets::hasInit()) {
		uint32_t temppid = GetProcessID("RainbowSix.exe");
		//std::cout << "PROCESS ID: " << PID << std::endl;

		if (temppid != NULL) {
			//Sleep(35000);
			temppid = NULL;
			temppid = GetProcessID("RainbowSix.exe");
			if (temppid != NULL) {
				//std::cout << "PID IS: " << std::hex << temppid << std::endl;
				Offsets::init();
				return;
			}
		}
		Sleep(1000);
	}
}

void keyThread() {
	HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

	// Keep this app running until we're told to stop
	MSG msg;
	while (!GetMessageA(&msg, NULL, NULL, NULL)) {    //this while loop keeps the hook
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	UnhookWindowsHookEx(hhkLowLevelKybd);
}

int main() {
	ClearPIDCacheTable();
	Setup();
	//Print("[Onyx Dev] Offsets Loaded, type \"help\" for commands.");

	// Start threads to keep cheats on between rounds
	std::thread GunModThread = std::thread(GunModWatcher);
	std::thread PlayerModThread = std::thread(PlayerModWatcher);
	std::thread ESPThread = std::thread(ESP::ESPWatcher);
	std::thread TriggerThread = std::thread(Misc::TriggerThread);

	//std::thread GunCheckDebug = std::thread(GunCheck);

	//NoRecoil(true);

	/*
	while (true) {
		std::string input;
		std::getline(std::cin, input);
		ConsoleCMD(input);
	}
	*/
	
	std::thread KeyThread = std::thread(keyThread);

	UI::Setup();
	std::thread _ConsoleThread = std::thread(UI::ConsoleThread);


	

	KeyThread.join();

	TriggerThread.join();
	

	return 0;
}




/*
void ConsoleCMD(std::string input) {
	std::cout << std::endl;

	for (char& c : input) {
		c = to_lowercase(c);
	}

	if (input.rfind("help", 0) == 0) {
		std::cout << "[Onyx Dev Menu]" << std::endl << "\"all\": Toggle all cheats" << std::endl << "\"ns\": Toggle no spread" << std::endl << "\"nr\": Toggle no recoil" << std::endl << "\"glow\": Toggle glow" << std::endl << "\"mult\": Toggle damage multiplier" << std::endl << "\"clip\": Toggle noclip" << std::endl << "\"flash\": Toggle no flash" << std::endl << "\"cav\": Toggle cav ESP" << std::endl << "\"trigger\": Toggle triggerbot" << std::endl << "\"off\": Force all off" << std::endl;
	} else if (input.rfind("all", 0) == 0) {
		CheatStates::allon = !CheatStates::allon;
		CheatStates::nospread = CheatStates::allon;
		CheatStates::norecoil = CheatStates::allon;
		CheatStates::glow = CheatStates::allon;
		CheatStates::damageMult = CheatStates::allon;
		CheatStates::noclip = CheatStates::allon;
		CheatStates::noflash = CheatStates::allon;
		std::cout << "[Onyx] All State: " << CheatStates::allon << std::endl;
		ToggleAll(CheatStates::allon);
	} else if (input.rfind("ns", 0) == 0) {
		CheatStates::nospread = !CheatStates::nospread;
		std::cout << "[Onyx] Nospread State: " << CheatStates::nospread << std::endl;
		NoSpread(CheatStates::nospread);
	} else if (input.rfind("nr", 0) == 0) {
		CheatStates::norecoil = !CheatStates::norecoil;
		std::cout << "[Onyx] Norecoil State: " << CheatStates::norecoil << std::endl;
		NoRecoil(CheatStates::norecoil);
	} else if (input.rfind("glow", 0) == 0) {
		CheatStates::glow = !CheatStates::glow;
		std::cout << "[Onyx] Glow State: " << CheatStates::glow << std::endl;
		Glow::ToggleGlow(CheatStates::glow);
	} else if (input.rfind("mult", 0) == 0) {
		CheatStates::damageMult = !CheatStates::damageMult;
		std::cout << "[Onyx] Damage Multiplier State: " << CheatStates::damageMult << std::endl;
		DamageMultiplier(CheatStates::damageMult);
	} else if (input.rfind("clip", 0) == 0) {
		CheatStates::noclip = !CheatStates::noclip;
		std::cout << "[Onyx] Noclip State: " << CheatStates::noclip << std::endl;
		NoClip(CheatStates::noclip);
	} else if (input.rfind("flash", 0) == 0) {
		CheatStates::noflash = !CheatStates::noflash;
		std::cout << "[Onyx] Flash State: " << CheatStates::noflash << std::endl;
		NoFlash(CheatStates::noflash);
	} else if (input.rfind("cav", 0) == 0) {
		CheatStates::cavESP = !CheatStates::cavESP;
		std::cout << "[Onyx] Cav ESP State: " << CheatStates::cavESP << std::endl;
		ESP::cavESP(CheatStates::cavESP);
	} else if (input.rfind("off", 0) == 0) {
		std::cout << "[Onyx] Diabled Everything" << std::endl;
		CheatStates::allon = false;
		CheatStates::nospread = false;
		CheatStates::norecoil = false;
		CheatStates::glow = false;
		CheatStates::damageMult = false;
		CheatStates::noclip = false;
		CheatStates::noflash = false;
		CheatStates::cavESP = false;
		ToggleAll(false);
	} else if (input.rfind("trigger", 0) == 0) {
		CheatStates::triggerbot = !CheatStates::triggerbot;
		Misc::ToggleTriggerbot(CheatStates::triggerbot);
		std::cout << "[Onyx] Triggerbot State: " << CheatStates::triggerbot << std::endl;
	} else if (input.rfind("unlock", 0) == 0) {
		CheatStates::unlockAll = !CheatStates::unlockAll;

		Misc::UnlockAll(CheatStates::unlockAll);
	}
}
*/


/*
void ToggleAll(bool active) {
	NoSpread(active);
	NoRecoil(active);
	Glow::ToggleGlow(active);
	DamageMultiplier(active);
	NoClip(active);
	NoFlash(active);
	ESP::cavESP(active);
}
*/






/*
BOOL IsElevated() {
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	return fRet;
}
*/