#include "gameSys.h"

void gameSys::constructGame(olc::PixelGameEngine* pge) 
{
	mM.constructVars();
	mG.constructVars();
}

bool gameSys::updateGame(olc::PixelGameEngine* pge)
{
	switch (currentState)
	{
	case gameStates::mainMenu:
		mM.updateScene(pge, &currentState);
		break;
	case gameStates::mainGame:
		mG.updateScene(pge);
		break;
	default:
		return false;
	}
	return true;
}

void gameSys::renderGame(olc::PixelGameEngine* pge)
{
	switch (currentState)
	{
	case gameStates::mainMenu:
		mM.renderScene(pge);
		break;
	case gameStates::mainGame:
		mG.renderScene(pge);
		break;
	default:
		return;
	}

}