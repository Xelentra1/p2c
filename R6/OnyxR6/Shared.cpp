#pragma once
#include "Shared.h"
#include "GunMod.h"
#include "Glow.h"
#include "ESP.h"
#include "PlayerMod.h"
#include "Misc.h"
#include "ConsoleUI.h"

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

EnableFunction funcs[] = { NULL, NoSpread, NoRecoil, Glow::ToggleGlow, NULL, NoClip, NoFlash, ESP::cavESP, NULL, Misc::UnlockAll, NULL, NULL };

bool CS::ID(int ID) {
	return states[ID];
}

void UpdateUI(int ID) {
	SetCharW(CS::ID(ID) ? '+' : WCHAR{ L'┄' }, { 3, (SHORT)ID + 1 }, 1);
	SetColor(CS::ID(ID) ? COLOR_ENABLED : COLOR_DISABLED, { 3, (SHORT)ID + 1 }, 1);
}

bool CT::ID(int ID) {
	if (ID == All_ID) { // Toggle all
		states[ID] = !states[ID];
		UpdateUI(ID);
		for (int i = 1; i < 6; i++)
		{
			if (states[i] != states[ID]) {
				states[i] = states[ID];
				UpdateUI(i);
				if (funcs[i] != NULL) funcs[i](states[ID]);
			}
		}
		return states[ID];
	}
	else { // Toggle normal
		states[ID] = !states[ID];
		UpdateUI(ID);
		if (funcs[ID] != NULL) funcs[ID](states[ID]); // Toggle cheat with it's current enabled / disabled state if it has a toggle function
		return states[ID];
	}
}

void CT::Reset() {
	for (bool &state : states) {
		state = false;
	}
}

bool CFG::ID(int ID) {
	return config[ID];
}