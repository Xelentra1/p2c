#include "GunMod.h"

std::vector<GunInfo> gunInfoList;

namespace gunStates {
	bool NoRecoil = false;
	bool NoSpread = false;
	bool damageMult = false;
}

void FixGuns() {
	for (GunInfo& gun : gunInfoList) {
		if (gun.bulletsPerShot == 2.0F || gun.bulletsPerShot == 4.0F) {
			gun.bulletsPerShot = 1.0F;
		}
		else if (gun.bulletsPerShot >= 16.0F) {
			gun.bulletsPerShot = 8.0F;
		}
	}
}

GunInfo GetGun(GunInfo gun) { // TODO: remove guns from gunlist when there are > 5 guns in the list, since most will be invalid
	for (GunInfo gunInfo : gunInfoList) {
		if (gunInfo.gunPtr == gun.gunPtr) return gunInfo;
	}
	return GunInfo(NULL, NULL, NULL, NULL, NULL);
}

GunInfo GetValidGun(GunInfo gun) {
	GunInfo tempGun = GetGun(gun);

	if (tempGun.gunPtr == NULL) {
		gunInfoList.push_back(gun);
		return gun;
	}
	else {
		return tempGun;
	}
}

GunInfo GetBulletInfo(GunInfo gun) {
	for (GunInfo gunInfo : gunInfoList) {
		if (gunInfo.bulletPtr == gun.bulletPtr) return gunInfo;
	}
	return GunInfo(NULL, NULL, NULL, NULL, NULL);
}

GunInfo GetValidBulletInfo(GunInfo gun) {
	GunInfo tempGun = GetBulletInfo(gun);

	if (tempGun.bulletPtr == NULL) {
		gunInfoList.push_back(gun);
		return gun;
	}
	else {
		return tempGun;
	}
}

void PrintGunList() {
	std::cout << "Gun List:" << std::endl;
	for (GunInfo gun : gunInfoList) {
		std::cout << "    - Recoil: " << gun.recoil << std::endl << "      Spread: " << gun.spread << std::endl << "Bullets/Shot: " << gun.bulletsPerShot << std::endl << std::endl;
	}
}

GunInfo CurrentGun() {
	FixGuns();
	return GunInfo(Offsets::localGun(), Offsets::damageMult(), Offsets::getRecoil(), Offsets::getSpread(), Read<unsigned int>(Offsets::damageMult()));
}

void NoRecoil(bool active, GunInfo currentGun) {
	if (!Offsets::isInGame()) return;
	MemVars::PID = Offsets::currentPID();

	gunStates::NoRecoil = active;

	intptr_t recoilPtr = Offsets::localGun() + offset_recoil;

	//PrintGunList();

	Write<float>(recoilPtr, (active ? 0.002F : currentGun.recoil));
}

void NoSpread(bool active, GunInfo currentGun) {
	if (!Offsets::isInGame()) return;
	MemVars::PID = Offsets::currentPID();

	gunStates::NoSpread = active;

	intptr_t spreadPtr = Offsets::localGun() + offset_spread;

	//PrintGunList();

	Write<float>(spreadPtr, (active ? 0.002F : currentGun.spread));
}

void NoRecoil(bool active) {
	if (!Offsets::isInGame()) return;
	MemVars::PID = Offsets::currentPID();

	gunStates::NoRecoil = active;

	intptr_t recoilPtr = Offsets::localGun() + offset_recoil;
	GunInfo currentGun = CurrentGun();
	currentGun = GetValidGun(currentGun);

	//PrintGunList();

	Write<float>(recoilPtr, (active ? 0.002F : currentGun.recoil));
}

void NoSpread(bool active) {
	if (!Offsets::isInGame()) return;
	MemVars::PID = Offsets::currentPID();

	gunStates::NoSpread = active;

	intptr_t spreadPtr = Offsets::localGun() + offset_spread;
	GunInfo currentGun = CurrentGun();
	currentGun = GetValidGun(currentGun);

	//PrintGunList();

	Write<float>(spreadPtr, (active ? 0.002F : currentGun.spread));
}

void SetDamage(intptr_t damagePtr, unsigned int damage) {
	Write<unsigned int>(damagePtr, damage);
}

void DamageMultiplier(bool active, GunInfo currentGun) {
	if (!Offsets::isInGame()) return;
	MemVars::PID = Offsets::currentPID();

	gunStates::damageMult = active;

	intptr_t damagePtr = Offsets::damageMult();

	SetDamage(damagePtr, (active ? currentGun.bulletsPerShot * 2 : currentGun.bulletsPerShot));
}

void DamageMultiplier(bool active) {
	if (!Offsets::isInGame()) return;
	MemVars::PID = Offsets::currentPID();

	gunStates::damageMult = active;

	intptr_t damagePtr = Offsets::damageMult();
	GunInfo currentGun = CurrentGun();
	currentGun = GetValidBulletInfo(currentGun);

	SetDamage(damagePtr, (active ? currentGun.bulletsPerShot * 2 : currentGun.bulletsPerShot));
}

void SetThisRound(GunInfo gun, bool wasSet) {
	for (GunInfo& tempGun : gunInfoList) {
		if (tempGun.gunPtr == gun.gunPtr) {
			tempGun.setThisRound = wasSet;
		}
	}
}

void MultSetThisRound(GunInfo gun, bool wasSet) {
	for (GunInfo& tempGun : gunInfoList) {
		if (tempGun.bulletPtr == gun.bulletPtr) {
			tempGun.multSetThisRound = wasSet;
		}
	}
}

void GunModWatcher() {
	MemVars::PID = Offsets::currentPID();
	bool listCleared = false;
	while (true) {
		//std::cout << "Watcher running" << std::endl;
		Sleep(500);
		if (Offsets::isInGame()) {
			listCleared = false;
			GunInfo currentGun = CurrentGun();
			GunInfo currentInfo = GetValidGun(currentGun);
			GunInfo currentMult = GetValidBulletInfo(currentGun);
			if (!currentInfo.setThisRound) {
				std::cout << "Set Recoil/Spread" << std::endl;
				SetThisRound(currentInfo, true);
				NoRecoil(gunStates::NoRecoil, currentInfo);
				NoSpread(gunStates::NoSpread, currentInfo);
			}
			if (!currentMult.multSetThisRound) {
				std::cout << "Set Damage Mult" << std::endl;
				MultSetThisRound(currentMult, true);
				DamageMultiplier(gunStates::damageMult, currentMult);
			}
		}
		else if (!listCleared) {
			listCleared = true;
			std::cout << "Cleared set list" << std::endl;
			for (GunInfo& gun : gunInfoList) {
				SetThisRound(gun, false);
				MultSetThisRound(gun, false);
			}
		}
	}
}