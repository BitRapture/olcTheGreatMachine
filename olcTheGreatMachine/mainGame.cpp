#include "mainGame.h"

mainGame::mainGame()
{
	// Light Blue
	colorScheme[0].r = 83;
	colorScheme[0].g = 158;
	colorScheme[0].b = 224;
	// Mid Blue
	colorScheme[1].r = 41;
	colorScheme[1].g = 31;
	colorScheme[1].b = 202;
	// Dark Blue
	colorScheme[2].r = 6;
	colorScheme[2].g = 4;
	colorScheme[2].b = 39;

	// Construct player rays
	for (int i = 0; i < 8; ++i)
	{
		playerColRays[i].a = { 256, 152 };
	}
	pCRdirs[0] = { 0, 8 };
	pCRdirs[1] = { 8, 8 };
	pCRdirs[2] = { 8, 0 };
	pCRdirs[3] = { 8, -8 };
	pCRdirs[4] = { 0, -8 };
	pCRdirs[5] = { -8, -8 };
	pCRdirs[6] = { -8, 0 };
	pCRdirs[7] = { -8, 8 };
	playerHand.a = { 253, 148 };

	// Sprites
	sprPlayer[0] = std::make_unique<olc::Sprite>("./media/CogGuy1.png");
	sprPlayer[1] = std::make_unique<olc::Sprite>("./media/CogGuy2.png");
	sprPlayerHand = std::make_unique<olc::Sprite>("./media/CogGuy Hand.png");

	sprGuns[0] = std::make_unique<olc::Sprite>("./media/Gun Pistol.png");

	sprWalls[0] = std::make_unique<olc::Sprite>("./media/Wall1.png");
	sprWalls[1] = std::make_unique<olc::Sprite>("./media/Wall2.png");

	sprWallTop = std::make_unique<olc::Sprite>("./media/Wall Top.png");
	sprFloorCog = std::make_unique<olc::Sprite>("./media/Cog.png");
}

void mainGame::constructVars()
{
	// Decals
	dclPlayer[0] = std::make_unique<olc::Decal>(sprPlayer[0].get());
	dclPlayer[1] = std::make_unique<olc::Decal>(sprPlayer[1].get());
	dclPlayerHand = std::make_unique<olc::Decal>(sprPlayerHand.get());

	dclGuns[0] = std::make_unique<olc::Decal>(sprGuns[0].get());

	dclWalls[0] = std::make_unique<olc::Decal>(sprWalls[0].get());
	dclWalls[1] = std::make_unique<olc::Decal>(sprWalls[1].get());

	dclWallTop = std::make_unique<olc::Decal>(sprWallTop.get());
	dclFloorCog = std::make_unique<olc::Decal>(sprFloorCog.get());
}

void mainGame::updateScene(olc::PixelGameEngine* pge)
{
	// Create a level
	if (!createLevel)
	{
		allWalls.clear();
		usableLines.clear();
		shadowCast.clear();
		SCDraw.clear();
		drawWalls.clear();
		drawFloorCogs.clear();

		camera = { 256 - 64, 144 - 64 };
		int cX = 99, cY = 99, totalCells = (10 + (level*2) <= 30) ? 10 + (level*2) : 30;
		bool* usedCells = new bool[200*200];
		for (int x = 0; x < 200 * 200; ++x)
			usedCells[x] = false;
		std::random_device rDevice;
		std::mt19937 generator(rDevice());
		std::uniform_int_distribution<> range(0, 4), wallRange(0, 1), midRange(0, 9), cogRange(0, 2);
		std::uniform_real_distribution<float> cogAmount(0.005f, 0.05f);
		while (totalCells > 0)
		{
			if (!usedCells[cX + 200 * cY])
			{
				allWalls.push_back(std::make_tuple<olc::vi2d, olc::vi2d, char>({ (cX - 99) * 128, (cY - 99) * 128 }, { ((cX - 99) * 128) + 128, ((cY - 99) * 128) + 128 }, 0));
				usedCells[cX + 200 * cY] = true;
				totalCells--;

				if (cogRange(generator) == 1 || totalCells == 0)
					drawFloorCogs.push_back(std::make_tuple<olc::vi2d, float, float>({ (cX - 99) * 128, (cY - 99) * 128 }, 0, cogAmount(generator)));
			}
			int ranW = range(generator);
			switch (ranW)
			{
			case 1:
				cX++;
				break;
			case 2:
				cX--;
				break;
			case 3:
				cY++;
				break;
			case 4:
				cY--;
				break;
			default:
				if (midRange(generator) == 1)
				{
					cX = 99;
					cY = 99;
				}
			}
			if (cX > 199 || cX < 0 || cY > 199 || cY < 0)
			{
				cX = 99;
				cY = 99;
			}
		}
		for (int i = 0; i < allWalls.size(); ++i)
		{
			cX = (std::get<0>(allWalls[i]).x == 0) ? 99 : (std::get<0>(allWalls[i]).x / 128) + 99;
			cY = (std::get<0>(allWalls[i]).y == 0) ? 99 : (std::get<0>(allWalls[i]).y / 128) + 99;
			if (usedCells[(cX + 1) + 200 * cY])
				std::get<2>(allWalls[i]) |= 1;
			else
				usableLines.push_back(std::make_tuple<olc::vi2d, olc::vi2d>({ std::get<0>(allWalls[i]).x + 128,  std::get<0>(allWalls[i]).y }, { std::get<0>(allWalls[i]).x + 128,  std::get<0>(allWalls[i]).y + 128 }));
			
			if (usedCells[(cX - 1) + 200 * cY])
				std::get<2>(allWalls[i]) |= 2;
			else
				usableLines.push_back(std::make_tuple<olc::vi2d, olc::vi2d>({ std::get<0>(allWalls[i]).x,  std::get<0>(allWalls[i]).y + 128 }, { std::get<0>(allWalls[i]).x, std::get<0>(allWalls[i]).y }));
			
			if (usedCells[cX + 200 * (cY - 1)])
				std::get<2>(allWalls[i]) |= 4;
			else
			{
				usableLines.push_back(std::make_tuple<olc::vi2d, olc::vi2d>({ std::get<0>(allWalls[i]).x, std::get<0>(allWalls[i]).y }, { std::get<0>(allWalls[i]).x + 128,  std::get<0>(allWalls[i]).y }));
				drawWalls.push_back(std::make_tuple<olc::vi2d, char>({ std::get<0>(allWalls[i]).x, std::get<0>(allWalls[i]).y - 32 }, wallRange(generator)));
				drawWalls.push_back(std::make_tuple<olc::vi2d, char>({ std::get<0>(allWalls[i]).x + 32, std::get<0>(allWalls[i]).y - 32 }, wallRange(generator)));
				drawWalls.push_back(std::make_tuple<olc::vi2d, char>({ std::get<0>(allWalls[i]).x + 64, std::get<0>(allWalls[i]).y - 32 }, wallRange(generator)));
				drawWalls.push_back(std::make_tuple<olc::vi2d, char>({ std::get<0>(allWalls[i]).x + 96, std::get<0>(allWalls[i]).y - 32 }, wallRange(generator)));
			}
			
			if (usedCells[cX + 200 * (cY + 1)])
				std::get<2>(allWalls[i]) |= 8;
			else
				usableLines.push_back(std::make_tuple< olc::vi2d, olc::vi2d>({ std::get<0>(allWalls[i]).x + 128, std::get<0>(allWalls[i]).y + 128 }, { std::get<0>(allWalls[i]).x,  std::get<0>(allWalls[i]).y + 128 }));
			
		}
		for (int i = 0; i < usableLines.size(); ++i)
		{
			ogRayCast ray;
			ray.a = { 256, 152 };
			ray.memPoint = std::get<0>(usableLines[i]);
			shadowCast.push_back(ray);
			ray.offSet = 1;
			shadowCast.push_back(ray);
			ray.offSet = -1;
			shadowCast.push_back(ray);
			SCDraw.push_back(std::make_tuple<float, olc::vf2d>(0, { 0, 0 }));
			SCDraw.push_back(std::make_tuple<float, olc::vf2d>(0, { 0, 0 }));
			SCDraw.push_back(std::make_tuple<float, olc::vf2d>(0, { 0, 0 }));
		}
		allWalls.clear();
		drawShadow = true;

		createLevel = true;
	}
	else
	{
		// Debug
		if (pge->GetKey(olc::F5).bReleased)
			createLevel = false;
		if (pge->GetKey(olc::F2).bReleased)
			debugStopDraw = !debugStopDraw;
		
		// Movement
		bool bRight = (pge->GetKey(olc::RIGHT).bHeld || pge->GetKey(olc::D).bHeld),
			bLeft = (pge->GetKey(olc::LEFT).bHeld || pge->GetKey(olc::A).bHeld),
			bUp = (pge->GetKey(olc::UP).bHeld || pge->GetKey(olc::W).bHeld),
			bDown = (pge->GetKey(olc::DOWN).bHeld || pge->GetKey(olc::S).bHeld),
			diag = false;
		float nDagX = 0, nDagY = 0;
		if (!bRight && !bLeft && !bUp && !bDown)
		{
			playerSp *= 0.95f;
			if (playerSp != 0)
				drawShadow = true;
			sprPlayerAnim = 0;
		}
		else
		{
			nDagX = playerSX = bLeft - bRight;
			nDagY = playerSY = bUp - bDown;
			if (playerSX != 0 && playerSY != 0)
			{
				playerSX *= 0.707107;
				playerSY *= 0.707107;
				diag = true;
			}
			sprPlayerFrames++;
			if (sprPlayerFrames >= 12)
			{
				sprPlayerAnim = !sprPlayerAnim;
				sprPlayerFrames = 0;
			}

			if (playerSp < 3.f)
				playerSp += .05f;
			else
				playerSp = 3.f;
			drawShadow = true;
		}
		// Wall Collisions
		olc::vf2d potentialM = { camera.x + (playerSX * playerSp), camera.y + (playerSY * playerSp) },
			potentialDM1 = { camera.x + (nDagX * playerSp), camera.y },
			potentialDM2 = { camera.x, camera.y + (nDagY * playerSp) };
		bool freeM = true, freeDM1 = diag, freeDM2 = diag;
		for (int i = 0; i < 8; ++i)
		{
			playerColRays[i].b = {pCRdirs[i].x + 256, pCRdirs[i].y + 152};
			for (int ii = 0; ii < usableLines.size(); ++ii)
			{
				olc::vi2d b = std::get<0>(usableLines[ii]), e = std::get<1>(usableLines[ii]);
				if (playerColRays[i].cast({ b.x + potentialM.x, b.y + potentialM.y }, { e.x + potentialM.x, e.y + potentialM.y }))
					freeM = false;

				if (playerColRays[i].cast({ b.x + potentialDM1.x, b.y + potentialDM1.y }, { e.x + potentialDM1.x, e.y + potentialDM1.y }))
					freeDM1 = false;
				else if (playerColRays[i].cast({ b.x + potentialDM2.x, b.y + potentialDM2.y }, { e.x + potentialDM2.x, e.y + potentialDM2.y }))
					freeDM2 = false;
			}
		}
		// Apply Movement
		if (freeM)
			camera = potentialM;
		else if (freeDM1)
			camera = potentialDM1;
		else if (freeDM2)
			camera = potentialDM2;
		if (playerSp < 0.001f)
			playerSp = 0;

		// Hand look at mouse
		playerHand.memPoint = pge->GetMousePos();
		playerHand.memPoint = { playerHand.memPoint.x - playerHand.a.x, playerHand.memPoint.y - playerHand.a.y };
		if (playerHand.memPoint.mag() != 0)
			playerHand.b = { ((playerHand.memPoint.x / playerHand.memPoint.mag()) * 6) + playerHand.a.x, ((playerHand.memPoint.y / playerHand.memPoint.mag()) * 6) + playerHand.a.y };
		playerHand.memPoint.y = atan2f(playerHand.b.y - playerHand.a.y, playerHand.b.x - playerHand.a.x);
		playerHand.memPoint.x = playerHand.memPoint.y * (180 / pi);

	}
}

void mainGame::renderScene(olc::PixelGameEngine* pge)
{	

	for (int i = 0; i < drawFloorCogs.size(); ++i)
	{
		pge->DrawRotatedDecal({ (float)std::get<0>(drawFloorCogs[i]).x + camera.x + 64, (float)std::get<0>(drawFloorCogs[i]).y + camera.y + 64 }, dclFloorCog.get(), std::get<1>(drawFloorCogs[i]), { 32, 32 });
		std::get<1>(drawFloorCogs[i]) += std::get<2>(drawFloorCogs[i]);
		if (std::get<1>(drawFloorCogs[i]) > pi)
			std::get<1>(drawFloorCogs[i]) = 0;
	}

	if (!debugStopDraw)
	{
		if (drawShadow)
		{
			// Glorious shadowcasting code
			pge->Clear(colorScheme[1]);
			for (int i = 0; i < shadowCast.size(); ++i)
			{
				shadowCast[i].b = { shadowCast[i].memPoint.x + camera.x, shadowCast[i].memPoint.y + camera.y };
				if (shadowCast[i].offSet != 0)
				{
					float angle = atan2f(shadowCast[i].b.y - shadowCast[i].a.y, shadowCast[i].b.x - shadowCast[i].a.x);
					shadowCast[i].b = { (cosf(angle + (0.0001f * shadowCast[i].offSet)) * 1000) + shadowCast[i].a.x, (sinf(angle + (0.0001f * shadowCast[i].offSet)) * 1000) + shadowCast[i].a.y };
				}
				shadowCast[i].cast({ 0, -100 }, { 512, -100 }, true);
				shadowCast[i].cast({ -100, 0 }, { -100, 288 }, true);
				shadowCast[i].cast({ 612, 0 }, { 612, 288 }, true);
				shadowCast[i].cast({ 0, 388 }, { 512, 388 }, true);
				for (int ii = 0; ii < usableLines.size(); ++ii)
				{
					olc::vi2d b = std::get<0>(usableLines[ii]), e = std::get<1>(usableLines[ii]);
					shadowCast[i].cast({ b.x + camera.x, b.y + camera.y }, { e.x + camera.x, e.y + camera.y }, true);
				}
				std::get<0>(SCDraw[i]) = atan2f(shadowCast[i].b.y - shadowCast[i].a.y, shadowCast[i].b.x - shadowCast[i].a.x);
				std::get<1>(SCDraw[i]) = shadowCast[i].b;
			}
			std::sort(SCDraw.begin(), SCDraw.end(), [](std::tuple<float, olc::vf2d> const& a, std::tuple<float, olc::vf2d> const& b) {
				return (std::get<0>(a) < std::get<0>(b));
				});
			for (int i = 0; i < shadowCast.size(); ++i)
			{
				olc::vf2d nP1, nP2;
				nP1 = std::get<1>(SCDraw[i]);
				if (i + 1 < shadowCast.size())
				{
					nP2 = std::get<1>(SCDraw[i + 1]);
					pge->FillTriangle({ 256, 152 }, nP1, nP2, colorScheme[2]);
				}
				else
				{
					nP2 = std::get<1>(SCDraw[0]);
					pge->FillTriangle({ 256, 152 }, nP1, nP2, colorScheme[2]);
				}
			}
			drawShadow = false;
		}
	}
	else
	{
		pge->Clear(colorScheme[2]);
		for (int i = 0; i < usableLines.size(); ++i)
		{
			olc::vi2d b = std::get<0>(usableLines[i]), e = std::get<1>(usableLines[i]);
			pge->DrawLine({ b.x + (int)camera.x, b.y + (int)camera.y }, { e.x + (int)camera.x, e.y + (int)camera.y }, colorScheme[1]);
		}
	}

	for (int i = 0; i < drawWalls.size(); ++i)
	{
		olc::vi2d b = std::get<0>(drawWalls[i]);
		pge->DrawDecal({ (float)b.x + camera.x, (float)b.y + camera.y }, dclWalls[std::get<1>(drawWalls[i])].get());
		pge->DrawDecal({ (float)b.x + camera.x, ((float)b.y - 32) + camera.y }, dclWallTop.get());
	}

	bool handDraw = (playerHand.memPoint.x >= 0 && playerHand.memPoint.x <= 360),
		handDraw2 = (playerHand.memPoint.x >= -180 && playerHand.memPoint.x <= 180);

	if (!handDraw)
	{
		if (handDraw2)
			pge->DrawRotatedDecal(playerHand.b, dclGuns[0].get(), playerHand.memPoint.y, { 6, 6 });
		else
			pge->DrawRotatedDecal(playerHand.b, dclGuns[0].get(), playerHand.memPoint.y, { 6, 6 }, { 1, -1 });
		
		pge->DrawDecal(playerHand.b, dclPlayerHand.get());
	}

	pge->DrawDecal({ 240, 128 }, dclPlayer[sprPlayerAnim].get());

	if (handDraw)
	{
		if (handDraw2)
			pge->DrawRotatedDecal(playerHand.b, dclGuns[0].get(), playerHand.memPoint.y, { 6, 6 });
		else
			pge->DrawRotatedDecal(playerHand.b, dclGuns[0].get(), playerHand.memPoint.y, { 6, 6 }, { 1, -1 });


		pge->DrawDecal(playerHand.b, dclPlayerHand.get());
	}
	if (!handDraw2)
		pge->DrawDecal({ 240 + 19, 128 + 20 }, dclPlayerHand.get());
	else
		pge->DrawDecal({ 240 + 7, 128 + 20 }, dclPlayerHand.get());
}