#include "Offsets.h"

namespace Offsets {
	bool _hasInit = false;

	bool hasInit() {
		return _hasInit;
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
		return Read<float>(localGun() + offset_recoil);
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

	HANDLE currentHandle() {
		return MemVars::processHandle;
	}

	std::uint32_t currentPID() {
		return MemVars::PID;
	}

	void init() {
		if (!_hasInit) {
			MemVars::PID = GetProcessID("RainbowSix.exe");
			//std::cout << "PROCESS ID: " << PID << std::endl;
			if (MemVars::PID != NULL) {

				//MemVars::processHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, MemVars::PID);
				baseAddress = GetModuleBaseAddr("RainbowSix.exe");
				statusManager = Read<uintptr_t>(baseAddress + offset_status_manager);
				glowManager = Read<uintptr_t>(baseAddress + offset_glow_manager);
				_gameManager = Read<uintptr_t>(baseAddress + offset_game_manager);
				outlineManager = ReadChain(_gameManager, std::vector<uintptr_t>{ offset_outlines });
				networkManager = Read<uintptr_t>(baseAddress + offset_network_manager);
				profileManager = Read<uintptr_t>(baseAddress + offset_profile_manager);
				localplayer = ReadChain(profileManager, std::vector<uintptr_t>{ offset_localplayer });
				_hasInit = true;
			}   //std::cout << "Test recoil value: " << Read<double>(gunBase()) << std::endl;
		}
	}
}