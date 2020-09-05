#pragma once
#include "olcPixelGameEngine.h"
#include "enums.h"

#include "mainMenu.h"
#include "mainGame.h"

class gameSys
{
private: 
	gameStates currentState = gameStates::mainMenu;
	mainMenu mM;
	mainGame mG;

public:
	void constructGame(olc::PixelGameEngine*);

	bool updateGame(olc::PixelGameEngine*);

	void renderGame(olc::PixelGameEngine*);
};

