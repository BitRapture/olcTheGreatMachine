#pragma once
#include "olcPixelGameEngine.h"
#include "enums.h"
#include <random>

#include "ogRayCast.h"


/*
 .......................... Name is either: Just another cog, OR, feed the machine ..........................
 .......................... Name is either: Just another cog, OR, feed the machine ..........................
 .......................... Name is either: Just another cog, OR, feed the machine ..........................
 .......................... Name is either: Just another cog, OR, feed the machine ..........................
 .......................... Name is either: Just another cog, OR, feed the machine ..........................
 .......................... Name is either: Just another cog, OR, feed the machine ..........................
 .......................... Name is either: Just another cog, OR, feed the machine ..........................
			
			You are the engineer to the wonderful multi-layered machine. You must keep it operational
			whilst dealing with creatures of the damned. The better you keep the machine running, the
			more rewards you shall receive to defeat creatures with as well as upgrades. You don't die
			but if the machine dies, you lose.

 
 */

class mainGame
{
// Globals
private:
	bool finishedTutorial = false, createLevel = false;
	int level = 0;
	float playerSp = 0, playerSX = 0, playerSY = 0;

	float pi = acos(0);

// Level Design
private:
	std::vector<std::tuple<olc::vi2d, olc::vi2d, char>> allWalls;
	// vi2d rectXY, vi2d rectXY2, char noDrawWalls (DEBUG)
	std::vector<std::tuple<olc::vi2d, olc::vi2d>> usableLines;
	olc::vf2d camera{ 256 - 64, 144  - 64 };
	olc::Pixel colorScheme[3];

	std::vector<ogRayCast> shadowCast;
	std::vector<std::tuple<float, olc::vf2d>> SCDraw;
	std::vector<std::tuple<olc::vi2d, int>> drawWalls;
	std::vector< std::tuple<olc::vi2d, float, float>> drawFloorCogs;
	bool drawShadow = true, debugStopDraw = false;
	
// Game Objects
private:
	ogRayCast playerColRays[8];
	olc::vf2d pCRdirs[8];

	ogRayCast playerHand;

// Sprites
private:
	std::unique_ptr<olc::Sprite> sprPlayer[2], sprPlayerHand, sprGuns[1], sprWalls[2], sprWallTop, sprFloorCog;
	std::unique_ptr<olc::Decal> dclPlayer[2], dclPlayerHand, dclGuns[1], dclWalls[2], dclWallTop, dclFloorCog;

	int sprPlayerFrames = 0;
	bool sprPlayerAnim = 0;

public:
	mainGame();

	void constructVars();

	void updateScene(olc::PixelGameEngine*);

	void renderScene(olc::PixelGameEngine*);
};

