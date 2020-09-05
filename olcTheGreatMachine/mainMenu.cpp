#include "mainMenu.h"

mainMenu::mainMenu()
{
	sprMainMenuArtwork = std::make_unique<olc::Sprite>("./media/MainMenuArtwork.png");
	sprPlayButton = std::make_unique<olc::Sprite>("./media/PlayButton.png");
	sprExitButton = std::make_unique<olc::Sprite>("./media/ExitButton.png");
	sprPlayButtonP = std::make_unique<olc::Sprite>("./media/PlayButtonP.png");
	sprExitButtonP = std::make_unique<olc::Sprite>("./media/ExitButtonP.png");
}

void mainMenu::constructVars()
{
	dclMainMenuArtwork = std::make_unique<olc::Decal>(sprMainMenuArtwork.get());
	dclPlayButton = std::make_unique<olc::Decal>(sprPlayButton.get());
	dclExitButton = std::make_unique<olc::Decal>(sprExitButton.get());
	dclPlayButtonP = std::make_unique<olc::Decal>(sprPlayButtonP.get());
	dclExitButtonP = std::make_unique<olc::Decal>(sprExitButtonP.get());
}

void mainMenu::updateScene(olc::PixelGameEngine* pge, gameStates* gs)
{
	olc::vi2d mPos = pge->GetMousePos();

	if (mPos.x >= 218 && mPos.x <= 301 && mPos.y >= 97 && mPos.y <= 119)
		playButtonP = true;
	else if (mPos.x >= 218 && mPos.x <= 301 && mPos.y >= 151 && mPos.y <= 173)
		exitButtonP = true;
	else
	{
		playButtonP = false;
		exitButtonP = false;
	}

	if (playButtonP && pge->GetMouse(0).bHeld)
		*gs = gameStates::mainGame;
	if (exitButtonP && pge->GetMouse(0).bHeld)
		*gs = gameStates::closeGame;
}

void mainMenu::renderScene(olc::PixelGameEngine* pge)
{
	pge->DrawDecal({ 0, 0 }, dclMainMenuArtwork.get());
	if (playButtonP)
		pge->DrawDecal({ 218, 97 }, dclPlayButtonP.get());
	else
		pge->DrawDecal({ 218, 97 }, dclPlayButton.get());
	if (exitButtonP)
		pge->DrawDecal({ 218, 151 }, dclExitButtonP.get());
	else
		pge->DrawDecal({ 218, 151 }, dclExitButton.get());
}