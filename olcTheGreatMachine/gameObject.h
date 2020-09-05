#pragma once
#include "olcPixelGameEngine.h"

class gameObject
{
public:
	virtual void drawSelf(olc::PixelGameEngine* pge)
	{
		pge->DrawCircle(objPos, 3);
	}

	bool pointInRect(olc::vi2d rectCoord, olc::vi2d rectSize, olc::vi2d point)
	{
		olc::vi2d rEnd = { rectCoord.x + rectSize.x, rectCoord.y + rectSize.y };
		return (point.x >= rectCoord.x && point.x <= rEnd.x && point.y >= rectCoord.y && point.y <= rEnd.y);
	}

	bool circleCircle(olc::vi2d circle1, int circle1radius, olc::vi2d circle2, int circle2radius)
	{
		return ( ((circle2.x - circle1.x)*(circle2.x - circle1.x)) + ((circle1.y - circle2.y) * (circle1.y - circle2.y)) <= ((circle1radius + circle2radius)*(circle1radius + circle2radius)) );
	}

public:
	olc::vi2d objPos{ 0, 0 };
};