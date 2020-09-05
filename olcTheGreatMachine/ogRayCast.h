#pragma once
#include "olcPixelGameEngine.h"

class ogRayCast
{
public:

	bool cast(olc::vf2d pointA, olc::vf2d pointB, bool calculate = false);

public:
	olc::vf2d a{ 0, 0 }, b{ 0, 0 }, memPoint{ 0, 0 };
	char offSet = 0;
};

