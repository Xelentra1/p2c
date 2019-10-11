#include "Glow.h"

GlowColor _currentGlowColor = GlowColor(255, 0, 255, 1, 5);

GlowColor Glow::currentGlowColor() {
	return GlowColor(_currentGlowColor.gColor.red, _currentGlowColor.gColor.green, _currentGlowColor.gColor.green, _currentGlowColor.gColorData.alpha, _currentGlowColor.gColorData.distance);
}

void Glow::setGlowColor(float r, float g, float b, float a = 1.0F, float distance = 5.0F) {
	_currentGlowColor.gColor.red = r;
	_currentGlowColor.gColor.green = g;
	_currentGlowColor.gColor.blue = b;
	_currentGlowColor.gColorData.alpha = a;
	_currentGlowColor.gColorData.distance = distance;

	Glow::setColor(_currentGlowColor);
}

GlowColor defaultColor = GlowColor(0, 0, 0, 0, 0);

void Glow::setColor(float r, float g, float b, float a, float distance) {
	//if (!Offsets::isInGame()) return;

	uintptr_t glowBase = Offsets::glowBase();

	GlowColor color = GlowColor(r, g, b, a, distance);

	Write<GlowColor::Color>(glowBase + 0x110, color.gColor);
	Write<GlowColor::ColorData>(glowBase + 0x130, color.gColorData);
}

void Glow::setColor(GlowColor color) {
	uintptr_t glowBase = Offsets::glowBase();

	Write<GlowColor::Color>(glowBase + 0x110, color.gColor);
	Write<GlowColor::ColorData>(glowBase + 0x130, color.gColorData);
}

void Glow::ToggleGlow(bool active) {
	Glow::setColor((active ? _currentGlowColor : defaultColor));
}

void Glow::printColor() {
	GlowColor _glowColor = GlowColor();

	uintptr_t glowBase = Offsets::glowBase();

	_glowColor.gColor = Read<GlowColor::Color>(glowBase + 0x110);
	_glowColor.gColorData = Read<GlowColor::ColorData>(glowBase + 0x130);

	std::cout << "GCOLOR: R: " << _glowColor.gColor.red << " G: " << _glowColor.gColor.green << " B: " << _glowColor.gColor.blue << " Alpha: " << _glowColor.gColorData.alpha << " Distance: " << _glowColor.gColorData.distance << std::endl;
}