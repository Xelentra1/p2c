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

void AttemptOutlines() {

	uint8_t ret_stub[] = { 0x41, 0xC7, 0x84, 0x24, 0x10, 0x03,0x00,0x00,0x01,0x00,0x00,0x00,  //mov [r12+00000310],01
			0x90, //nop
			0x90, //nop
			0x90, //nop
			0x90 //nop };
	};

	if (change_protection(currentPID(), 0x19FEC7C, PAGE_EXECUTE_READWRITE, 0x64) == 0)
	{
		Write<uint8_t[16]>(0x19FEC7C, ret_stub);
		//WriteVirtualMemoryRaw(0x19FEC7C, reinterpret_cast<uintptr_t>(ret_stub), sizeof(ret_stub));
		change_protection(currentPID(), 0x19FEC7C, PAGE_EXECUTE_READ, 0x64);
	}

	


	//executeshell(shell, GameInfo.base + 0x19FEC7C);


	/*
	if (myteam == 3)
		WriteMem<INT8>(myteam, 4)

	if (myteam == 4)
		WriteMem<INT8>(myteam, 3)
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
	while (true) {
		Sleep(3000);
		if (Offsets::isInGame() && !ESPStates::setThisRound) {
			ESPStates::setThisRound = true;
			ESP::cavESP(CS::ID(CavESP_ID));
		}
		else if (ESPStates::setThisRound) {
			ESPStates::setThisRound = false;
		}
	}
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
