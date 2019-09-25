#pragma once
#include "Main.h"
#include "GunMod.h"
#include "Glow.h"
#include "ESP.h"
#include "PlayerMod.h"

#define Log(x) std::cout << x << std::endl;

int main() {
	ClearPIDCacheTable();
	Offsets::init();
	if (!Offsets::hasInit()) {
		std::cout << "Failed to grab offsets" << std::endl;
		return 901;
	}
	ClearPIDCacheTable();
	std::cout << "[Onyx Dev] Offsets Loaded, type \"help\" for commands." << std::endl;

	//if (!IsElevated()) { // This can run usermode, not needed
		//std::cout << "Need to run as admin, exiting" << std::endl;
		//return 902;
	//}

	if (!Offsets::isInGame()) {
		std::cout << "Need to be in game, exiting" << std::endl;
		//return 903;
	}

	MemVars::PID = Offsets::currentPID(); // Important

	std::thread GunModThread = std::thread(GunModWatcher);
	std::thread PlayerModThread = std::thread(PlayerModWatcher);
	std::thread ESPThread = std::thread(ESP::ESPWatcher);
	//std::thread GunModThread = std::thread(GunModWatcher);
	
	//int entity_count = Read<DWORD>(Offsets::gameManager() + offset_object_list_length) & 0x3fffffff;
	//std::cout << entity_count << std::endl;

	//ESP::esp_test(true);


	/*
	uintptr_t silentbase = ReadChain(Offsets::base(), std::vector<uintptr_t>{ 0x51C4700, 0x720 });
	Vector4 outSilent = Read<Vector4>(silentbase + 0x28);

	Log("Silent cords:");
	Log(outSilent.x);
	Log(outSilent.y);
	Log(outSilent.z);
	Log(outSilent.w);

	Write<Vector4>(silentbase + 0x28, Vector4(50, 50, 50, 20)); // crash
	*/
	

	//if (outline()) {
	//	std::cout << "Outline worked" << std::endl;
	//} else std::cout << "Outline failed" << std::endl;


	//ToggleAll(true);
	//std::cout << "Noclip enabled" << std::endl;
	//std::cout << std::dec << ESP::max_entities() << std::endl;

	while (true) {
		std::string input;
		std::getline(std::cin, input);
		ConsoleCMD(input);
	}

	//SetDamage(Offsets::damageMult(), 2);

	//NoRecoil(false);
	//std::cout << std::dec << sizeof(testbyte) << std::endl;

	/*
	Vector4 testVector = Vector4();

	std::cout << std::hex << Offsets::viewAnglePtr() << std::endl;

	if (!ReadProcessMemory(Offsets::currentHandle(), (BYTE*)Offsets::viewAnglePtr(), &testVector, sizeof(testVector), nullptr)) {
		std::cout << "Failed to read" << std::endl;
	}
	else {
		std::cout << "Read vector:" << std::endl << "X: " << testVector.x << std::endl << "Y: " << testVector.y << std::endl << "Z: " << testVector.z << std::endl << "W: " << testVector.w << std::endl;
	}
	*/

	//if (!WriteProcessMemory(Offsets::currentHandle(), (BYTE*)Offsets::noflashPtr(), &testbyte, sizeof(testbyte), nullptr)) {
	//	std::cout << "Failed to write" << std::endl;
	//}


	/*
	Vector3 emptyVector = Vector3(0, 0, 0);
	if (!WriteProcessMemory(Offsets::currentHandle(), (BYTE*)Offsets::noclipPtr(), &emptyVector, sizeof(emptyVector), nullptr)) {
		std::cout << "Failed to write" << std::endl;
	}
	*/

	//Write<Vector3>(Offsets::noclip(), emptyVector);

	/*
	std::cout << std::dec << Read<unsigned int>(Offsets::damageMult()) << std::endl;
	DamageMultiplier(true);
	std::cout << std::dec << Read<unsigned int>(Offsets::damageMult()) << std::endl;
	Sleep(5000);
	DamageMultiplier(false);
	std::cout << std::dec << Read<unsigned int>(Offsets::damageMult()) << std::endl;
	*/
	/*
	Glow::setColor(0, 255, 255, 1, 5);
	Sleep(5000);
	Glow::setColor(0, 0, 0, 0, 0);
	*/

	return 0;
}

namespace CheatStates {
	bool allon = false;
	bool nospread = false;
	bool norecoil = false;
	bool glow = false;
	bool damageMult = false;
	bool noclip = false;
	bool noflash = false;
	bool cavESP = false;
}

char to_lowercase(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 32;

	return c;
}

void ConsoleCMD(std::string input) {
	std::cout << std::endl;

	for (char& c : input) {
		c = to_lowercase(c);
	}

	if (input.rfind("help", 0) == 0) {
		std::cout << "[Onyx Dev Menu]" << std::endl << "\"all\": Toggle all cheats" << std::endl << "\"ns\": Toggle no spread" << std::endl << "\"nr\": Toggle no recoil" << std::endl << "\"glow\": Toggle glow" << std::endl << "\"mult\": Toggle damage multiplier" << std::endl << "\"clip\": Toggle noclip" << std::endl << "\"flash\": Toggle no flash" << std::endl << "\"cav\": Toggle cav ESP" << std::endl << "\"off\": Force all off" << std::endl;
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
	}
}

void ToggleAll(bool active) {
	NoSpread(active);
	NoRecoil(active);
	Glow::ToggleGlow(active);
	DamageMultiplier(active);
	NoClip(active);
	NoFlash(active);
	ESP::cavESP(active);
}






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