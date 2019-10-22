#include "ESP.h"


WORD get_player_count()
{
	uint64_t game_manager = Offsets::gameManager();

	if (!game_manager)
		return NULL;

	return Read<WORD>(game_manager + offset_object_list_length);
}
/*
uint64_t get_player_by_id(unsigned int i)
{
	uint64_t game_manager = Offsets::gameManager();

	if (!game_manager)
		return NULL;

	uint64_t entity_list = Read<uint64_t>(game_manager + offset_gamemanager_object_list2);

	if (!entity_list)
		return NULL;

	if (i > get_player_count())
		return NULL;

	uint64_t entity = Read<uint64_t>(entity_list + (sizeof(PVOID) * i));

	if (!entity)
		return NULL;

	return Read<uint64_t>(entity + OFFSET_STATUS_MANAGER_LOCALENTITY);
}
*/

namespace ESPStates {
	bool setThisRound = false;
}

void ESP::cavESP(bool active)
{
	if (!Offsets::isInGame()) return;

	BYTE spotted = active;

	uintptr_t entity_list = Read<uintptr_t>(Offsets::gameManager() + offset_object_list);
	int entity_count = Read<DWORD>(Offsets::gameManager() + offset_object_list_length) & 0x3fffffff;
	if (entity_count == NULL) return;

	for (int i = 0; i < entity_count; i++)
	{
		uintptr_t entity_object = Read<uintptr_t>(entity_list + i * 0x8);

		uintptr_t addr2 = Read<uintptr_t>(entity_object + 0x28);
		if (addr2 == NULL) continue;
		addr2 = Read<uintptr_t>(addr2 + 0xD8);
		if (addr2 == NULL) continue;

		for (int a = 0; a < 30; a++)
		{
			uintptr_t addr3 = Read<uintptr_t>(addr2 + (a * 8));
			if (addr3 == NULL) continue;

			if (Read<uintptr_t>(addr3) != Offsets::marker())
				continue;

			//Write<BYTE>(addr3 + 0x532, spotted);
			Write<BYTE>(addr3 + 0x534, spotted);
		}
	}
}

#define PATCH_DRONEFIX 0x10f6f3b

#define RDATA_RECOIL_PITCH 0x3945974
#define RDATA_RECOIL_YAW 0x3946740
void AttemptOutlines(float x, float y, float z) {

	//BYTE fix_b[7] = { 0x0f, 0x2f, 0x3d, 0x36, 0xEA, 0x84, 0x02 };
	//Write<BYTE[7]>(Offsets::base() + PATCH_DRONEFIX, fix_b);



	/*
	// Actual code
	Vector4 enemyOutlineColor = { (uint8_t)1, (uint8_t)0, (uint8_t)1, 1.0 };
	uintptr_t GMComponentContainer = Read<uintptr_t>(Offsets::gameManager() + 0x320);
	uintptr_t GMComponentMarkAndOutline = Read<uintptr_t>(GMComponentContainer + 0x968);
	Write<byte>(GMComponentMarkAndOutline + 0x48, 255);
	Write<byte>(GMComponentMarkAndOutline + 0x310, 1);
	Write<Vector4>(GMComponentMarkAndOutline + 0x48, enemyOutlineColor);
	uintptr_t tmp = Read<uintptr_t>(Offsets::base() + 0x5E784C0);
	uintptr_t tmp1 = Read<uintptr_t>(tmp + 0x90);   //88 38 68 20
	uintptr_t tmp2 = Read<uintptr_t>(tmp1 + 0x38);   //90 38 128
	uintptr_t tmp3 = Read<uintptr_t>(tmp2 + 0x128);
	//Sleep(60000);
	std::cout << "Sleep 1 min" << std::endl;
	if (tmp3 == 3) {
		Write<byte>(tmp2 + 0x128, 4);
		std::cout << "Write to team 3" << std::endl;
		Write<byte>(tmp3 + 0x20, 4);
		std::cout << "Write to team 4" << std::endl;
	}
	tmp = Read<uintptr_t>(Offsets::base() + 0x5E784C0);
	tmp1 = Read<uintptr_t>(tmp + 0x88);   //88 38 68 20
	tmp2 = Read<uintptr_t>(tmp1 + 0x38);   //90 38 128
	tmp3 = Read<uintptr_t>(tmp2 + 0x68);
	uint64_t tmp4 = Read<uintptr_t>(tmp3 + 0x20);
	if (tmp4 == 3) {
		Write<byte>(tmp3 + 0x20, 4);
		std::cout << "Write to team 4" << std::endl;
		Write<byte>(tmp2 + 0x128, 4);
		std::cout << "Write to team 3" << std::endl;
	}
	// End actual code
	*/

	//Write<byte>(tmp2 + 0x128, 3);
	//Write<byte>(tmp3 + 0x20, 3);
	/*
	uintptr_t game_manager = Read<uintptr_t>(Offsets::base() + offset_game_manager);

	Vector4 enemyOutlineColor = { x, y, z, 1.0 };
	uintptr_t GMComponentContainer = Read<uintptr_t>(game_manager + 0x320);
	uintptr_t GMComponentMarkAndOutline = Read<uintptr_t>(GMComponentContainer + 0x968);
	Write<BYTE>(GMComponentMarkAndOutline + 0x48, 255);
	Write<BYTE>(GMComponentMarkAndOutline + 0x310, 1);
	Write<Vector4>(GMComponentMarkAndOutline + 0x48, enemyOutlineColor);

	uintptr_t tmp = Read<uintptr_t>(Offsets::base() + 0x5E784C0);
	uintptr_t tmp1 = Read<uintptr_t>(tmp + 0x90);   //88 38 68 20
	uintptr_t tmp2 = Read<uintptr_t>(tmp1 + 0x38);   //90 38 128
	//uintptr_t tmp3 = Read<uintptr_t>(tmp2 + 0x128);

	uintptr_t tmpf = Read<uintptr_t>(Offsets::base() + 0x5E784C0);
	uintptr_t tmp1f = Read<uintptr_t>(tmpf + 0x88);   //88 38 68 20
	uintptr_t tmp2f = Read<uintptr_t>(tmp1f + 0x38);   //90 38 128
	uintptr_t tmp3f = Read<uintptr_t>(tmp2f + 0x68);
	uint64_t tmp4f = Read<uintptr_t>(tmp3f + 0x20);

	Write<BYTE>(tmp2 + 0x128, tmp4f);
	*/
}

/*
uintptr_t getOutlineComponent() {

	uintptr_t temp;
	temp = Read<uintptr_t>(Offsets::gameManager() + 0x320); //read address of gamemanager + 0x320
	temp = Read<uintptr_t>(temp + 0x968); // read address of (gamemanager + 0x320) + 0x968
	return temp;

}

void outlineEsp(bool enable) {
	MemVars::PID = Offsets::currentPID();
	uintptr_t outlineComponent = getOutlineComponent();
	if (!outlineComponent)
		return;

	if (!Write<BYTE>(outlineComponent + 0x310, enable)) {
		std::cout << "Write failed x3" << std::endl;
	}
}

void setOutlineColor(bool team, Vector4 color)
{
	MemVars::PID = Offsets::currentPID();
	uintptr_t outlineComponent = getOutlineComponent();
	if (!outlineComponent)
		return;

	if (!Write<Vector4>(outlineComponent + (team ? 0x48 : 0x58), color)) {
		std::cout << "Write failed x4" << std::endl;
	}
}


void setOutline(bool update, bool visible)
{
	MemVars::PID = Offsets::currentPID();
	uintptr_t outlineComponent = getOutlineComponent();
	if (!outlineComponent)
		return;

	uintptr_t objectArray = Read<uintptr_t>(outlineComponent + 0x2f0); //list
	WORD objectArraySize = Read<WORD>(outlineComponent + 0x2f8); //size
	std::cout << "SIZE: " << objectArraySize << std::endl;
	for (size_t offset = 0x0; offset < 2 * 0x68; offset += 0x90)
	{
		if (!Write<BYTE>(objectArray + offset + 0x0, visible)) {
			std::cout << "Write failed x1" << std::endl;
		}
		if (!Write<BYTE>(objectArray + offset + 0x2, update)) {
			std::cout << "Write failed x2" << std::endl;
		}
	}
}

bool outline()
{
	MemVars::PID = Offsets::currentPID();
	uint64_t outline = Read<uint64_t>(Offsets::base() + 0x50660B0);

	if (!outline)
		return false;

	uint64_t teamo = Read<uint64_t>(outline + 0x88);
	uint64_t teamou = Read<uint64_t>(teamo + 0x38);
	uint64_t teamout = Read<uint64_t>(teamou + 0x68);

	Write<unsigned int>(teamout + 0x20, 5);

	return true;
}
*/

void ESP::ESPWatcher() {
	//while (true) {
		//Sleep(3000);
		if (Offsets::isInGame() && !ESPStates::setThisRound) {
			Sleep(300);
			ESPStates::setThisRound = true;
			if (CS::ID(CavESP_ID)) {
				ESP::cavESP(true);
			}
		}
		else if (ESPStates::setThisRound) {
			ESPStates::setThisRound = false;
		}
	//}
}


































/*
WORD max_entities() {
	if (!Offsets::isInGame()) return 0;
	MemVars::PID = Offsets::currentPID();

	const uintptr_t game_manager = Offsets::gameManager();
	if (!game_manager)
		std::cout << "failed x7" << std::endl;
		return 0;

	return Read<WORD>(game_manager + 0x1C8 + 0x8);
}

std::uintptr_t _entity(std::uint32_t index) {
	if (!Offsets::isInGame()) return 0;
	MemVars::PID = Offsets::currentPID();

	const uintptr_t game_manager = Offsets::gameManager();
	if (!game_manager) {
		std::cout << "failed x5" << std::endl;
		return 0;
	}
	const uintptr_t entity_list = Read<uintptr_t>(game_manager + 0x1C8);
	if (!entity_list) {
		std::cout << "failed x6" << std::endl;
		return 0;
	}

	return Read<uintptr_t>(entity_list + (sizeof(PVOID) * index));
}

void enable_marker(std::uintptr_t entity, bool active) {
	if (!Offsets::isInGame()) return;
	MemVars::PID = Offsets::currentPID();

	const uintptr_t entity_info = Read<uintptr_t>(entity + 0x28);
	if (!entity_info) {
		std::cout << "failed x1" << std::endl;
		return;
	}

	const uintptr_t component_list = Read<uintptr_t>(entity_info + 0xd8);

	if (!component_list) {
		std::cout << "failed x2" << std::endl;
		return;
	}

	for (auto current_component = 0x80; current_component < 0xf0; current_component += sizeof(std::uintptr_t)) {
		const uintptr_t component = Read<uintptr_t>(component_list + current_component);
		if (!component) {
			std::cout << "failed x3" << std::endl;
			continue;
		}
		if (Read<uintptr_t>(component) != (Offsets::marker())) {
			std::cout << "failed x4" << std::endl;
			continue;
		}
		std::cout << "past x4" << std::endl;

		if (!Write<std::uint8_t>(component + 0x532, (active ? 1 : 0))) {
			std::cout << "Write failed x1" << std::endl;
		}
		std::cout << "past wx1" << std::endl;
		if (!Write<std::uint8_t>(component + 0x534, (active ? 1 : 0))) {
			std::cout << "Write failed x2" << std::endl;
		}
		std::cout << "past wx2" << std::endl;

	}
}

void ESP::enableESP(bool active) {
	if (!Offsets::isInGame()) return;
	MemVars::PID = Offsets::currentPID();

	for (auto idx = 0u; idx <= max_entities(); idx++) {
		const uintptr_t entity = _entity(idx);
		if (!entity) {
			continue;
		}

		uintptr_t playerInfo = Read<uintptr_t>(entity + 0xA8);
		
		std::cout << Read<BYTE>(playerInfo + 0x19E) << std::endl;
		//enable_marker(entity, active);
	}
}

*/
