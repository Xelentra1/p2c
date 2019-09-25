#pragma once
#include "Main.h"

class Vector4
{
public:
	Vector4() : x(0.f), y(0.f), z(0.f), w(0.f)
	{

	}

	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{

	}
	~Vector4()
	{

	}

	float x;
	float y;
	float z;
	float w;
};

//void setOutline(bool update, bool visible);
//void outlineEsp(bool enable);
//void setOutlineColor(bool team, Vector4 color);
//bool outline();

namespace ESP {
	//void enableESP(bool active);
	void cavESP(bool active);
	void ESPWatcher();
	
	//void setOutline(bool update, bool visible);
	//void outlineEsp(bool toggle);
}