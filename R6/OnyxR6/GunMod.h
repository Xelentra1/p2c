#pragma once
#include "Main.h"

void NoRecoil(bool active);
void NoSpread(bool active);
void DamageMultiplier(bool active);
void SetDamage(intptr_t damagePtr, unsigned int damage);
void GunModWatcher();

class GunInfo {
public:
	uintptr_t gunPtr;
	uintptr_t bulletPtr;
	float recoil;
	float spread;
	float bulletsPerShot;
	bool setThisRound;
	bool multSetThisRound;

	GunInfo(uintptr_t _gunPtr, uintptr_t _bulletPtr, float _recoil, float _spread, float _bulletPerShot) {
		gunPtr = _gunPtr;
		bulletPtr = _bulletPtr;
		recoil = _recoil;
		spread = _spread;
		bulletsPerShot = _bulletPerShot;
		setThisRound = false;
		multSetThisRound = false;
	}
};