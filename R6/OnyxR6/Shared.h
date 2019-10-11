#pragma once
#include <iostream>
#define All_ID 0
#define NoSpread_ID 1
#define NoRecoil_ID 2
#define Glow_ID 3
#define DamageMult_ID 4
#define Noclip_ID 5
#define NoFlash_ID 6
#define CavESP_ID 7
#define TriggerBot_ID 8
#define UnlockAll_ID 9

#define cfg_legitSpread
#define cfg_legitRecoil

uint32_t currentPID();
void setPID(uint32_t PID);
void unsetPID();

namespace CS {
	bool ID(int ID);
}

namespace CT {
	bool ID(int ID);
}

namespace CFG {
	bool ID(int ID);
}