#pragma once

#include "interface.h"

class IUniformRandomStream
{
public:
	virtual void SetSeed(int iSeed) = 0;
	virtual float RandomFloat(float flMinVal = 0.0f, float flMaxVal = 1.0f) = 0;
	virtual int	RandomInt(int iMinVal, int iMaxVal) = 0;
	virtual float RandomFloatExp(float flMinVal = 0.0f, float flMaxVal = 1.0f, float flExponent = 1.0f) = 0;
};

MAKE_INTERFACE_SIGNATURE(IUniformRandomStream, UniformRandomStream, "client.dll", "48 8B 0D ? ? ? ? F3 0F 59 CA 44 8D 42", 0x0, 1);