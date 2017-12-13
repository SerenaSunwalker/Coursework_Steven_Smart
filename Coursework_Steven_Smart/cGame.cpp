/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}

int Steps = 0;
int Win = 0;
bool Lose = false;
bool Menu = true;
void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);

	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Store the textures
	textureName = { "Wall", "bullet","thePlayer","theBackground","theKey"};
	texturesToUse = { "Images\\Wall.png","Images\\bullet.png", "Images\\Player.png", "Images\\Room.png","Images\\Key.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	//Button Textures
	/*
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png" };
	btnPos = { { 400, 375 },{ 400, 300 },{ 400, 300 },{ 500, 500 },{ 400, 300 },{ 740, 500 },{ 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;
    */
	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 32);
	}
	gameTextList = { "The Maze in Time", "Steps","WIN!","LOSE!","Press Space to Begin, use the arrow keys to move. try to finish within 100 moves."};
	
	theTextureMgr->addTexture("Title", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Steps", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Win", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Lose", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Menu", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	// Load game sounds
	soundList = { "theme", "shot", "explosion" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio/who10Edit.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	theRocket.setSpritePos({ 32, 416 });
	theRocket.setTexture(theTextureMgr->getTexture("thePlayer"));
	theRocket.setSpriteDimensions(theTextureMgr->getTexture("thePlayer")->getTWidth(), theTextureMgr->getTexture("thePlayer")->getTHeight());
	theRocket.setRocketVelocity({ 0, 0 });

	//Stored wall positions
	int Xcords[203] = {/*Top row*/0, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448,480,512,544,576,608,640,672,704,736/*Bottom Row*/,  0, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448,480,512,544,576,608,640,672,704,736/*Left Wall*/,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0/*Right Wall*/,736,736,736,736,736,736,736,736,736,736,736,736,736,736/*The Maze*/, 32,224,352,416,640,128,160,224,288,352,416,640, 96,128,224,288,352,416,512,544,576,608,704, 96,288,352,704,736, 96,160,224,256,288,320,352,416,448,512,576,608,672,224,288,416,512,640,736, 96,160,224,288,352,416,480,512,576,608,672,736, 96,160,352,416,448,576,672, 96,160,192,224,288,352,384,416,480,512,544,576,640,672,736,288,480,544,672,736, 64, 96,160,224,288,320,352,384,416,544,576,608,672,736,160,224,288,416,480,544,608,160,192,224,288,352,416,480,544,608,640,672,736};
	int Ycords[203] = {/*Top row*/0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0/*Bottom Row*/,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480/*Left Wall*/, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448/*Right Wall*/, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448/*The Maze*/, 32, 32, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96,128,128,128,128,128,160,160,160,160,160,160,160,160,160,160,160,160,160,192,192,192,192,192,192,224,224,224,224,224,224,224,224,224,224,224,224,256,256,256,256,256,256,256,288,288,288,288,288,288,288,288,288,288,288,288,288,288,288,320,320,320,320,320,352,352,352,352,352,352,352,352,352,352,352,352,352,352,384,384,384,384,384,384,384,416,416,416,416,416,416,416,416,416,416,416,416};
	// Create vector array of textures
	for (int astro = 0; astro < 203; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos({ Xcords[astro] /* * (rand() % 5 + 1)*/, Ycords[astro]/* * (rand() % 5 + 1) */ });
		//theAsteroids[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
		int randAsteroid = rand() % 1;
		theAsteroids[astro]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
		theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
		theAsteroids[astro]->setAsteroidVelocity({ 3, 3 });
		theAsteroids[astro]->setActive(true);
	}

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
	// Render each asteroid in the vector array
	for (int draw = 0; draw < theAsteroids.size(); draw++)
	{
		theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSpriteDimensions(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
	}
	// Render each bullet in the vector array
	for (int draw = 0; draw < theBullets.size(); draw++)
	{
		theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
	}
	// Render the Title
	cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
	cTexture* stepTextTexture = theTextureMgr->getTexture("Steps");
	cTexture* winTextTexture = theTextureMgr->getTexture("Win");
	cTexture* loseTextTexture = theTextureMgr->getTexture("Lose");
	cTexture* menuTextTexture = theTextureMgr->getTexture("Menu");
	SDL_Rect pos = { 0, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	SDL_Rect pos2 = { 416, 0, stepTextTexture->getTextureRect().w, stepTextTexture->getTextureRect().h };
	SDL_Rect pos3 = { 384, 224, winTextTexture->getTextureRect().w, winTextTexture->getTextureRect().h };
	SDL_Rect pos4 = { 384, 224, loseTextTexture->getTextureRect().w, loseTextTexture->getTextureRect().h };
	SDL_Rect pos5 = { 416, 0, menuTextTexture->getTextureRect().w, menuTextTexture->getTextureRect().h };
	FPoint scale = { 1, 1 };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	stepTextTexture->renderTexture(theRenderer, stepTextTexture->getTexture(), &stepTextTexture->getTextureRect(), &pos2, scale);

	if (Win = 0)
	{
		winTextTexture->renderTexture(theRenderer, winTextTexture->getTexture(), &stepTextTexture->getTextureRect(), &pos3, scale);
	}


	// render the rocket
	//if (theRocket.isActive())
	//{
		theRocket.render(theRenderer, &theRocket.getSpriteDimensions(), &theRocket.getSpritePos(), theRocket.getSpriteRotAngle(), &theRocket.getSpriteCentre(), theRocket.getSpriteScale());
	//}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// Update the visibility and position of each asteriod
	vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
	while (asteroidIterator != theAsteroids.end())
	{
		if ((*asteroidIterator)->isActive() == false)
		{
			asteroidIterator = theAsteroids.erase(asteroidIterator);
		}
		else
		{
			(*asteroidIterator)->update(deltaTime);
			++asteroidIterator;
		}
	}
	// Update the visibility and position of each bullet
	vector<cBullet*>::iterator BulletIterartor = theBullets.begin();
	while (BulletIterartor != theBullets.end())
	{
		if ((*BulletIterartor)->isActive() == false)
		{
			BulletIterartor = theBullets.erase(BulletIterartor);
		}
		else
		{
			(*BulletIterartor)->update(deltaTime);
			++BulletIterartor;
		}
	}
	// Rocket Iterrator
	/*
	vector<cRocket*>::iterator RocketIterartor = theRockets.begin();
	while (RocketIterartor != theRockets.end())
	{
		if ((*RocketIterartor)->isActive() == false)
		{
			RocketIterartor = theRockets.erase(RocketIterartor);
		}
		else
		{
			(*RocketIterartor)->update(deltaTime);
			++RocketIterartor;
		}
	}
	*/
	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	//Bullet collision
	for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
	{
		//(*bulletIterartor)->update(deltaTime);
		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
			{
				// if a collision set the bullet and asteroid to false
				(*asteroidIterator)->setActive(false);
				(*bulletIterartor)->setActive(false);
				theSoundMgr->getSnd("explosion")->play(0);
				cout << "Bullet collided";
			}
		}
	}

	//Player Collision
	//for (vector<cRocket*>::iterator rocketIterartor = theRockets.begin(); rocketIterartor != theRockets.end(); ++rocketIterartor)
	//{
		//(*rocketIterartor)->update(deltaTime);
	    /*
		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &theRocket.getBoundingRect()))
			{
				// if a collision set the rocket and asteroid to false

				(*asteroidIterator)->setActive(false);
				theRocket.setActive(false);
				theSoundMgr->getSnd("explosion")->play(0);
				cout << "Player Collided";


			}
		}*/
	//}

	// Update the Rockets position
	theRocket.update(deltaTime);
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
					if (theRocket.getSpritePos().y < (renderHeight - theRocket.getSpritePos().h) && theRocket.isActive())
					{
						theRocket.setSpritePos({ theRocket.getSpritePos().x, theRocket.getSpritePos().y + 32 });
						theRocket.setBoundingRect(theRocket.getSpritePos());
						Steps++;

						for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
						{
							if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &theRocket.getBoundingRect()))
							{
								//cout << "Collision detected" << endl;
								theRocket.setSpritePos({ theRocket.getSpritePos().x, theRocket.getSpritePos().y - 32 });
							}
						}
						if (Steps > 100)
						{
							cout << "Lose";
						}
					}
				}
				break;

				case SDLK_UP:
				{

						if (theRocket.getSpritePos().y > 0 /*&& theRocket.isActive()*/)
						{
							theRocket.setSpritePos({ theRocket.getSpritePos().x, theRocket.getSpritePos().y - 32 });
							theRocket.setBoundingRect(theRocket.getSpritePos());
							Steps++;

							for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
							{
								if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &theRocket.getBoundingRect()))
								{
									theRocket.setSpritePos({ theRocket.getSpritePos().x, theRocket.getSpritePos().y + 32 });
								}
							}
							if (theRocket.getSpritePos().y < 92 && theRocket.getSpritePos().x > 640)
						    {
								cout << "win";
								Win = 0;
						    }
							if (Steps > 100)
							{
								cout << "Lose";
							}
						}

				}
				break;
				case SDLK_RIGHT:
				{
					if (theRocket.getSpritePos().x < (renderWidth - theRocket.getSpritePos().w) && theRocket.isActive())
					{
						theRocket.setSpritePos({ theRocket.getSpritePos().x + 32, theRocket.getSpritePos().y });
						theRocket.setBoundingRect(theRocket.getSpritePos());
						Steps++;

						for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
						{
							if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &theRocket.getBoundingRect()))
							{
								theRocket.setSpritePos({ theRocket.getSpritePos().x - 32, theRocket.getSpritePos().y });
							}
						}
						if (Steps > 100)
						{
							cout << "Lose";
						}
					}
				}
				break;

				case SDLK_LEFT:
				{
					if (theRocket.getSpritePos().x > 0 && theRocket.isActive())
					{
						theRocket.setSpritePos({ theRocket.getSpritePos().x - 32, theRocket.getSpritePos().y });
						theRocket.setBoundingRect(theRocket.getSpritePos());
						Steps++;

						for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
						{
							if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &theRocket.getBoundingRect()))
							{
								theRocket.setSpritePos({ theRocket.getSpritePos().x + 32, theRocket.getSpritePos().y });
							}
						}
						if (Steps > 100)
						{
							cout << "Lose";
						}
					}
				}
				break;
				case SDLK_SPACE:
				{
					/*
					theBullets.push_back(new cBullet);
					int numBullets = theBullets.size() - 1;
					theBullets[numBullets]->setSpritePos({ theRocket.getBoundingRect().x + theRocket.getSpriteCentre().x, theRocket.getBoundingRect().y + theRocket.getSpriteCentre().y });
					theBullets[numBullets]->setSpriteTranslation({ 2, 2 });
					theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
					theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
					theBullets[numBullets]->setBulletVelocity({ 2, 2 });
					theBullets[numBullets]->setSpriteRotAngle(theRocket.getSpriteRotAngle());
					theBullets[numBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << theRocket.getBoundingRect().x << " y: " << theRocket.getBoundingRect().y << endl;
                    */
					cout << "Steps Taken:" << Steps;
				}
				theSoundMgr->getSnd("shot")->play(0);
				break;
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

