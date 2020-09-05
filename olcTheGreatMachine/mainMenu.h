#pragma once
#include "olcPixelGameEngine.h"
#include "enums.h"

class mainMenu
{
private:
	std::unique_ptr<olc::Sprite> sprMainMenuArtwork, sprPlayButton, sprExitButton, sprPlayButtonP, sprExitButtonP;
	std::unique_ptr<olc::Decal> dclMainMenuArtwork, dclPlayButton, dclExitButton, dclPlayButtonP, dclExitButtonP;

	bool playButtonP = false, exitButtonP = false;

public:
	mainMenu();

	void constructVars();

	void updateScene(olc::PixelGameEngine*, gameStates*);

	void renderScene(olc::PixelGameEngine*);
};

