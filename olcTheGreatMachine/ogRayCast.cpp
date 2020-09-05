#include "ogRayCast.h"

bool ogRayCast::cast(olc::vf2d pointA, olc::vf2d pointB, bool calculate)
{
	float p1a = pointB.y - pointA.y,
		p1b = pointA.x - pointB.y,
		p1c = p1a * (pointA.x) + p1b * (pointA.y),
		p2a = b.y - a.y,
		p2b = a.x - b.x,
		p2c = p2a * (a.x) + p2b * (a.y);

	float det = p1a * p2b - p2a * p1b;
	 
	if (det == 0)
		return false;
	else
	{
		float t = ((a.x - pointA.x) * (pointA.y - pointB.y) - (a.y - pointA.y) * (pointA.x - pointB.x)) / ((a.x - b.x) * (pointA.y - pointB.y) - (a.y - b.y) * (pointA.x - pointB.x)),
			u = -(((a.x - b.x) * (a.y - pointA.y) - (a.y - b.y) * (a.x - pointA.x)) / ((a.x - b.x) * (pointA.y - pointB.y) - (a.y - b.y) * (pointA.x - pointB.x)));
		if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
		{
			if (calculate)
				b = { a.x + (t * (b.x - a.x)), a.y + (t * (b.y - a.y)) };
			return true;
		}
		return false;
	}
}