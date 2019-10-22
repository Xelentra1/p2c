#include "PlayerMod.h"

namespace PlayerStates {
	bool setThisRound = false;
}

void NoFlash(bool active) {
	if (!Offsets::isInGame()) return;

	Write<uint8_t>(Offsets::noflashPtr(), (active ? 0 : 1));
}

void NoClip(bool active) {
	if (!Offsets::isInGame()) return;

	Vector3 emptyVector = Vector3(0, 0, 0);
	Vector3 defaultVector = Vector3(0.0001788139343f, 0.0001788139343f, 0.03051757626f);

	Write<Vector3>(Offsets::noclipPtr(), (active ? emptyVector : defaultVector));
}

void PlayerModWatcher() {
	//while (true) {
		//Sleep(3000);
		if (Offsets::isInGame() && !PlayerStates::setThisRound) {
			Sleep(300);
			PlayerStates::setThisRound = true;
			if (CS::ID(NoFlash_ID)) {
				NoFlash(true);
			}
			if (CS::ID(Noclip_ID)) {
				NoClip(true);
			}
		}
		else if (PlayerStates::setThisRound) {
			PlayerStates::setThisRound = false;
		}
	//}
}

/*
void Teleport() {

	Vector3 High;
	High.y = 20;
	uint64_t r1 = Read<uint64_t>(getLocalPlayer() + 0x98);
	uint64_t aa = Read<uint64_t>(r1 + 0x148);

	uint8_t write1 = 3;
	int write2 = 3;
	uint8_t write3 = 3;

	Write<uint8_t>(aa + 0xA1, write1);

	Write<uint8_t>(aa + 0xA3, write1);
	Write<uint8_t>(aa + 0xA4, write1);
	Write<uint8_t>(aa + 0x13F, write1);
	Write<int>(aa + 0x140, write2);

	Write<uint8_t>(aa + 0x140, write3);

	Write<Vector3>(aa + 0x150, getPlayerHead(getRandomPlayer()));

	Write<Vector3>(aa + 0x180, getPlayerFeet(getLocalPlayer()));

}
*/