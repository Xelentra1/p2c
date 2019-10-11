#include "Offsets.h"

bool patch_terminate()
{
	HMODULE module = GetModuleHandleA("kernelbase.dll");

	if (!module)
		return false;

	uint64_t r6_module = GetModuleBaseAddr("kernelbase.dll");

	if (!r6_module)
		return false;

	uint64_t api = reinterpret_cast<uint64_t>(GetProcAddress(module, ("TerminateProcess"))) - reinterpret_cast<uint64_t>(module) + r6_module;

	if (change_protection(currentPID(), api, PAGE_EXECUTE_READWRITE, 0x64) == 0)
	{
		uint8_t ret_stub[] = { 0xC3, 0x90, 0x90, 0x90 };

		WriteVirtualMemoryRaw(api, reinterpret_cast<uintptr_t>(ret_stub), sizeof(ret_stub));
		change_protection(currentPID(), api, PAGE_EXECUTE_READ, 0x64);
	}

	/*
	if (change_protection(currentPID(), Offsets::base() + 0x1298380, PAGE_EXECUTE_READWRITE, 4) == 0) { // Recoil Patch
		original = Read<uint8_t>(Offsets::base() + 0x1298380 + 0x02);
		Write<uint8_t>(Offsets::base() + 0x1298380 + 0x02, 0x00);
		std::cout << "Original byte (hex): " << std::hex << original << std::endl;
	}
	change_protection(currentPID(), Offsets::base() + 0x1298380, PAGE_EXECUTE_READ, 4);
	*/

	return true;
}

namespace Offsets {
	bool _hasInit = false;

	bool hasInit() {
		return _hasInit;
	}

	void unInit() {
		_hasInit = false;
		unsetPID();
	}

	uintptr_t baseAddress = NULL;
	uintptr_t statusManager = NULL;
	uintptr_t glowManager = NULL;
	uintptr_t _gameManager = NULL;
	uintptr_t gameManager() {
		return _gameManager;
	}
	uintptr_t outlineManager = NULL;
	uintptr_t networkManager = NULL;
	uintptr_t profileManager = NULL;

	uintptr_t localplayer = NULL;
	/*
	static uintptr_t statusManager;
	static uintptr_t glowManager;
	static uintptr_t gameManager;
	static uintptr_t fovManager;
	static uintptr_t cameraManager;
	static uintptr_t spectatorManager;
	static uintptr_t headpos;
	*/
	uintptr_t base() {
		return baseAddress;
	}

	uintptr_t glowBase() {
		return Read<uintptr_t>(glowManager + offset_glowbase);
	}

	uintptr_t damageMult() {
		return ReadChain(_gameManager, std::vector<uintptr_t>{ offset_damage_modifier_chain }) + offset_damage_modifier_end;
	}

	uintptr_t outlineComp() {
		return outlineManager;
	}

	uintptr_t noclipPtr() {
		networkManager = Read<uintptr_t>(baseAddress + offset_network_manager);
		return ReadChain(networkManager, std::vector<uintptr_t>{ offset_noclip }) + offset_noclip_end;
	}

	uintptr_t noflashPtr() {
		return ReadChain(localplayer, std::vector<uintptr_t>{ offset_noflash }) + offset_noflash_end;
	}

	uintptr_t viewAnglePtr() {
		return ReadChain(localplayer, std::vector<uintptr_t>{ offset_set_view_angle }) + offset_set_view_angle_end;
	}

	uintptr_t marker() {
		return baseAddress + offset_marker;
	}

	uintptr_t localGun() {
		return ReadChain(localplayer, std::vector<uintptr_t>{ offset_gunstat_base });
	}

	float getRecoil() {
		return Read<float>(localGun() + offset_recoil); // offset_recoil
	}

	float getSpread() {
		return Read<float>(localGun() + offset_spread);
	}

	double getRoundStatus() {
		return Read<int>(statusManager + offset_roundstate);
	}

	bool isInGame() {
		double status = Read<int>(statusManager + offset_roundstate);
		if (status == 2 || status == 3) return true;
		return false;
	}


	void init() {
		if (!_hasInit) {
			 setPID(GetProcessID("RainbowSix.exe"));
			//std::cout << "PROCESS ID: " << PID << std::endl;
			if (currentPID() != NULL) {

				//MemVars::processHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, MemVars::PID);
				baseAddress = GetModuleBaseAddr("RainbowSix.exe");
				statusManager = Read<uintptr_t>(baseAddress + offset_status_manager);
				glowManager = Read<uintptr_t>(baseAddress + offset_glow_manager);
				_gameManager = Read<uintptr_t>(baseAddress + offset_game_manager);
				outlineManager = ReadChain(_gameManager, std::vector<uintptr_t>{ offset_outlines });
				networkManager = Read<uintptr_t>(baseAddress + offset_network_manager);
				profileManager = Read<uintptr_t>(baseAddress + offset_profile_manager);
				localplayer = ReadChain(profileManager, std::vector<uintptr_t>{ offset_localplayer });
				patch_terminate();
				_hasInit = true;
			}   //std::cout << "Test recoil value: " << Read<double>(gunBase()) << std::endl;
		}
	}
}