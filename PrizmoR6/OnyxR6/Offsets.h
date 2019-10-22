#pragma once
#include "MemoryFunctions.h"

#define offset_gunstat_base 0x78, 0xC8, 0x208
#define offset_recoil 0xB0 // local ] gun_base ] gunstat ] recoil
#define offset_spread 0x50 // local ] gun_base ] gunstat ] spread
#define offset_ammo 0xCC // local ] gun_base ] ammo
#define offset_ammo2 0xD0 // local ] gun_base ] ammo2

#define offset_status_manager 0x52124B8 //0x5225220 //0x51f0e30 //0x51b8820  //0x51CF9F8
#define offset_roundstate 0x2e8

#define offset_glow_manager 0x5e99be0 //0x5E3F4E8 //0x5e56e40 old
#define offset_glowbase 0xB8

#define offset_game_manager 0x5225280  //0x5203C00 //0x51CB748 // 0x51E27C8
#define offset_damage_modifier_chain 0x1F8, 0xD8, 0x48, 0x130, 0x130, 0x0
#define offset_damage_modifier_end 0x40

#define offset_outlines 0x320, 0x968
#define offset_outline_objlist 0x2F0
#define offset_outline_objlist_size 0x2f8
#define offset_outline_fixedsize 0x68
#define offset_outline_enabled 0x310
#define offset_outline_color_friendly 0x48
#define offset_outline_color_enemy 0x58

#define offset_network_manager 0x52124C0 //0x51F0E40 //0x51B8958 //0x51CFA08
#define offset_noclip 0xF8, 0x8
#define offset_noclip_end 0x530

#define offset_profile_manager 0x5212338 //0x51F0CB8 //0x51B87B0  //0x51CF880
#define offset_localplayer 0x68, 0x0, 0x28
#define offset_noflash 0x30, 0x30, 0x28
#define offset_noflash_end 0x40

#define offset_set_view_angle 0x1170
#define offset_set_view_angle_end 0xC0 // vector 4

#define offset_marker 0x37EC998 //0x37d8fc0 // 0x37BBA90 // new 0x37BBA90  // Orig 0x37AC120
#define offset_gamemanager_object_list2 0x1D8
#define offset_object_list 0x1C8
#define offset_object_list_length 0x1D0

#define offset_trigger_manager 0x51CB748

#define offset_unlocks 0x134b7f5 // 0x132F4C5

#define offset_gun_info_ex 0x78, 0xc8
#define offset_gun_info_reloadtime 0x24a0

// marker: 0x37AC120
/*
GameManager: 0x51E27C8
StatusManager: 0x51E2768
RoundManager: 0x51CF9F8
GameProfileManager: 0x51CF880
NetworkManager: 0x51CFA08
GlowManager:0x5E56E40
GameRenderer: 0x51E2680
FovManager: 0x51e27c0
*/
/*
OutlineComp = GameManager + 0x320 ] 0x968 // seems fine
ObjList = OutlineComp + 0x2F0 // seems fine
ObjListSize = OutlineComp + 0x2F8 // wrong
fixed size = 0x68 or 0x90 // ?
Color = OutlineComp + 0x48 // enemies at 0x58? //Vector4 (RGBA)
Enable = OutlineComp + 0x310 or 0x348 //bool
*/

// "RainbowSix.exe" + 0x51E27C8 (GameManager)] + 0x1F8] + 0xD8] + 0x48] + 0x130] + 0x130] + 0x0] + 0x40

namespace Offsets {
	bool hasInit();
	void unInit();

	uintptr_t base();
	uintptr_t localGun();
	uintptr_t glowBase();
	uintptr_t damageMult();
	uintptr_t outlineComp();
	uintptr_t noclipPtr();
	uintptr_t noflashPtr();
	uintptr_t viewAnglePtr();
	uintptr_t gameManager();
	uintptr_t localPlayer();
	uintptr_t marker();
	float getRecoil();
	float getSpread();
	//double getAmmo();
	double getRoundStatus();
	bool isInGame();

	void init();
}
/*
class OffsetClass {

public:
    OffsetClass() { }

	bool hasInit = false;

	uintptr_t baseAddress = NULL;
	uintptr_t statusManager = NULL;
	/*
	static uintptr_t statusManager;
	static uintptr_t glowManager;
	static uintptr_t gameManager;
	static uintptr_t fovManager;
	static uintptr_t cameraManager;
	static uintptr_t spectatorManager;
	static uintptr_t headpos;
	*/

	//uintptr_t gunBase() {
		//std::cout << std::hex << Read<uintptr_t>(baseAddress + offset_local_player) << std::endl;
		/*
		std::cout << std::hex << baseAddress << std::endl;
		std::cout << std::hex << offset_local_player << std::endl;
		std::cout << std::hex << Read<uintptr_t>(baseAddress + offset_local_player) << std::endl;
		return 5;
		*/
		/*
		return ReadChain(baseAddress, std::vector<uintptr_t>{ offset_local_player, offset_gun_base });
	}

	uintptr_t recoilProperties() {
		return ReadChain(gunBase(), std::vector<uintptr_t>{ offset_gunstat_base });
	}
	
	float getRecoil() {
		return Read<float>(recoilProperties() + offset_recoil);
	}

	float getSpread() {
		return Read<float>(recoilProperties() + offset_spread);
	}

	double getAmmo() {
		return Read<int>(gunBase() + offset_ammo);
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
		if (!hasInit) {

			PID = GetProcessID("RainbowSix.exe");
			//std::cout << "PROCESS ID: " << PID << std::endl;
			if (PID != NULL) {
				OffsetClass::baseAddress = GetModuleBaseAddr("RainbowSix.exe");
				std::cout << "BaseAddr: " << std::hex << baseAddress << std::endl;
				OffsetClass::statusManager = Read<uintptr_t>(baseAddress + offset_status_manager);
				hasInit = true;
			}   //std::cout << "Test recoil value: " << Read<double>(gunBase()) << std::endl;
		}
	}

};

extern OffsetClass Offsets;
*/
/*  ----------Good Code Start----------

uintptr_t local_player = Read<uintptr_t>(offsets::baseAddress + offset_local_player);

//std::cout << std::hex << local_player;

uintptr_t temp_ptr = Read<uintptr_t>(local_player + 0x20);

std::cout << std::hex << temp_ptr << std::endl;


uintptr_t gun_base = Read<uintptr_t>(temp_ptr + 0xC8);

std::cout << std::hex << gun_base << std::endl;
//ammo = Read<uintptr_t>(gun_base + 0xCC);
ammo = gun_base + offset_ammo;
std::cout << std::hex << gun_base + 0xCC << std::endl;

//  ----------Good Code End----------*/

//offsets::spread = Read<uintptr_t>(offsets::baseAddress + offset_local_player + offset_gun_base + offset_gunstat_base + offset_spread);
//offsets::ammo = Read<uintptr_t>(offsets::baseAddress + offset_local_player + offset_gun_base + offset_ammo);
//offsets::ammo = Read<uintptr_t>(offsets::baseAddress + offset_local_player + offset_gun_base + offset_ammo);
/*
offsets::statusManager = Read<uintptr_t>(offsets::baseAddress + OFFSET_STATUS_MANAGER);
offsets::glowManager = Read<uintptr_t>(offsets::baseAddress + OFFSET_GLOW_MANAGER);
offsets::gameManager = Read<uintptr_t>(offsets::baseAddress + OFFSET_GAME_MANAGER);
offsets::fovManager = Read<uintptr_t>(offsets::baseAddress + OFFSET_FOV_CHANGER);
offsets::cameraManager = Read<uintptr_t>(offsets::baseAddress + OFFSET_CAMERA_MANAGER);
offsets::spectatorManager = Read<uintptr_t>(offsets::baseAddress + OFFSET_SPOOF_SPECTATE);
offsets::headpos = Read<uintptr_t>(offsets::baseAddress + OFFSET_SKELETON_HEADPOSITION);
*/