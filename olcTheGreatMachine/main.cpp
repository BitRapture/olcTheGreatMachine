#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "gameSys.h"

class Enviro : public olc::PixelGameEngine
{
public:
	Enviro()
	{
		sAppName = "The Great Machine Entry";
	}

private:
	gameSys gS;

	float fTargetFPS = 1.f / 100.f;
	float fAccFPS = 0;

public:
	bool OnUserCreate() override
	{
		gS.constructGame(this);
		return true;
	}
	
	bool OnUserUpdate(float fElapsedTime) override
	{	
		gS.renderGame(this);

		//fAccFPS += fElapsedTime;
		//if (fAccFPS >= fTargetFPS)
		//{
		//	fAccFPS -= fTargetFPS;
		//	fElapsedTime = fTargetFPS;
		//}
		//else
		//	return true;

		return gS.updateGame(this);
	}
};

int main()
{
	Enviro Game;
	if (Game.Construct(512, 288, 2, 2, true, true))
		Game.Start();
	return 0;
}