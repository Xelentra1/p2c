#pragma once
#include "Main.h"

class GlowColor {

public:
	struct Color {

		float red = 0;
		float green = 0;
		float blue = 0;

	};
	struct ColorData {

		float alpha = 0; //alpha
		float distance = 20;

	};

	Color gColor = Color();
	ColorData gColorData = ColorData();

	GlowColor() {} //default constructor
	GlowColor(float r, float g, float b, float a, float distance) 
	{
		gColor.red = r;
		gColor.green = g;
		gColor.blue = b;

		gColorData.alpha = a;
		gColorData.distance = distance;
	};
};

namespace Glow{
	void setColor(float r, float g, float b, float a, float distance);
	void setColor(GlowColor color);
	void ToggleGlow(bool active);
	void printColor();
	GlowColor currentGlowColor();
	void setGlowColor(float r, float g, float b, float a, float distance);
}