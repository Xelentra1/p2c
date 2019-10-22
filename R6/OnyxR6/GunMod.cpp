#include "GunMod.h"
#include "Shared.h"

std::vector<GunInfo> gunInfoList;

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
	//return GunInfo(NULL, NULL, NULL, NULL, NULL);
	return GunInfo(NULL, NULL, NULL, NULL);
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
	//return GunInfo(NULL, NULL, NULL, NULL, NULL);
	return GunInfo(NULL, NULL, NULL, NULL);
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
		std::cout << "      Spread: " << gun.spread << std::endl << "Bullets/Shot: " << gun.bulletsPerShot << std::endl << std::endl;
	}
}

GunInfo CurrentGun() {
	FixGuns();
	return GunInfo(Offsets::localGun(), Offsets::damageMult(), Offsets::getSpread(), Read<unsigned int>(Offsets::damageMult()));
}

/*
void NoRecoil(bool active, GunInfo currentGun) {
	if (!Offsets::isInGame()) return;

	intptr_t recoilPtr = Offsets::localGun() + offset_recoil;

	//PrintGunList();

	Write<float>(recoilPtr, (active ? 0.002F : currentGun.recoil));
}
*/

void NoSpread(bool active, GunInfo currentGun) {
	if (!Offsets::isInGame()) return;

	intptr_t spreadPtr = Offsets::localGun() + offset_spread;

	//PrintGunList();

	Write<float>(spreadPtr, (active ? 0.002F : currentGun.spread));
}

void NoRecoil(bool active) {
	//if (!Offsets::isInGame()) return;

	/*
	intptr_t recoilPtr = Offsets::localGun() + offset_recoil;
	GunInfo currentGun = CurrentGun();
	currentGun = GetValidGun(currentGun);

	//PrintGunList();

	Write<float>(recoilPtr, (active ? 0.002F : currentGun.recoil));
	*/
	
	uintptr_t GunBase = Offsets::localGun();
	Write<bool>(GunBase + 0x168, false);
	Write<float>(GunBase + 0x14C, active ? -0.1002 : 1);
	Write<float>(GunBase + 0x15C, active ? -0.1004 : 1);

	/*
	if (change_protection(Offsets::base() + 0x1298380, PAGE_EXECUTE_READWRITE, 4) == 0) { // Recoil Patch
		Write<uint8_t>(Offsets::base() + 0x1298380 + 0x02, !active);
		//std::cout << "Original byte (hex): " << std::hex << original << std::endl;
	}
	change_protection(Offsets::base() + 0x1298380, PAGE_EXECUTE_READ, 4);
	*/
}

void NoSpread(bool active) {
	if (!Offsets::isInGame()) return;

	
	GunInfo currentGun = CurrentGun();
	currentGun = GetValidGun(currentGun);
	
	
	//PrintGunList();
	Write<float>(Offsets::localGun() + offset_spread, (active ? 0.002F : currentGun.spread));
	//Write<float>(spreadPtr, (active ? 0.002F : 1.0F));
}

void SetDamage(intptr_t damagePtr, unsigned int damage) {
	Write<unsigned int>(damagePtr, damage);
}

void DamageMultiplier(bool active, GunInfo currentGun) {
	if (!Offsets::isInGame()) return;

	intptr_t damagePtr = Offsets::damageMult();

	SetDamage(damagePtr, (active ? currentGun.bulletsPerShot * 2 : currentGun.bulletsPerShot));
}

void DamageMultiplier(bool active) {
	if (!Offsets::isInGame()) return;

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

/*
void MultSetThisRound(GunInfo gun, bool wasSet) {
	for (GunInfo& tempGun : gunInfoList) {
		if (tempGun.bulletPtr == gun.bulletPtr) {
			tempGun.multSetThisRound = wasSet;
		}
	}
}
*/

void GunModWatcher() {
	bool listCleared = false;
	//while (true) {
		//std::cout << "Watcher running" << std::endl;
		//Sleep(500);
		if (Offsets::isInGame()) {
			Sleep(300);
			listCleared = false;
			GunInfo currentGun = CurrentGun();
			GunInfo currentInfo = GetValidGun(currentGun);
			GunInfo currentMult = GetValidBulletInfo(currentGun);
			if (!currentInfo.setThisRound) {
				//std::cout << "Set Recoil/Spread" << std::endl;
				SetThisRound(currentInfo, true);
				if (CS::ID(NoRecoil_ID)) {
					NoRecoil(true);
				}
				if (CS::ID(NoSpread_ID)) {
					NoSpread(true, currentInfo);
				}
			}
			//if (!currentMult.multSetThisRound) {
				//std::cout << "Set Damage Mult" << std::endl;
			//	MultSetThisRound(currentMult, true);
				//DamageMultiplier(CS::ID(DamageMult_ID), currentMult);
			//}
		}
		else if (!listCleared) {
			listCleared = true;
			//std::cout << "Cleared set list" << std::endl;
			for (GunInfo& gun : gunInfoList) {
				SetThisRound(gun, false);
				//MultSetThisRound(gun, false);
			}
		}
	//}
}