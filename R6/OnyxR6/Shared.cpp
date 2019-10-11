#pragma once
#include "Shared.h"
#include "GunMod.h"
#include "Glow.h"
#include "ESP.h"
#include "PlayerMod.h"
#include "Misc.h"

uint32_t _currentPID = NULL;

uint32_t currentPID() {
	return _currentPID;
}

void setPID(uint32_t PID) {
	_currentPID = PID;
}

void unsetPID() {
	_currentPID = NULL;
}

bool states[] {
	false, false, false, false, false, false, false, false, false, false, false, false
};

bool config[]{
	false, false
};

GlowColor _glowColor = GlowColor(255, 0, 255, 1, 5);

typedef void(*EnableFunction)(bool);

EnableFunction funcs[] = { NULL, NoSpread, NoRecoil, Glow::ToggleGlow, DamageMultiplier, NoClip, NoFlash, ESP::cavESP, NULL, Misc::UnlockAll, NULL, NULL };

bool CS::ID(int ID) {
	return states[ID];
}

bool CT::ID(int ID) {
	if (ID == All_ID) { // Toggle all
		states[ID] = !states[ID];
		for (int i = 1; i < 8; i++)
		{
			states[i] = states[ID];
			if (funcs[i] != NULL) funcs[i](states[ID]);
		}
		return states[ID];
	}
	else { // Toggle normal
		states[ID] = !states[ID];
		if (funcs[ID] != NULL) funcs[ID](states[ID]); // Toggle cheat with it's current enabled / disabled state if it has a toggle function
		return states[ID];
	}
}

bool CFG::ID(int ID) {
	return config[ID];
}