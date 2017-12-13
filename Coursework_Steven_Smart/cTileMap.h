/*
=================
cTileMap.h
- Header file for class definition - SPECIFICATION
- Header file for the tileMap class which is a child of cSprite class
=================
*/
#ifndef _CTILEMAP_H
#define _CTILEMAP_H
#include "cSprite.h"
//#include "treeplanterGame.h"


class cTileMap: public cSprite
{
protected:
	int tileMap[8][8];

private:
	SDL_Point mapStartXY;
	SDL_Point tileClickedRC;
	cSprite aTile;
	void initialiseMap();		// Set the initial values for the map

public:
	cTileMap();

	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, cTextureMgr* theTxtMgr, vector<LPCSTR> theTxt);		// Default render function
	void update(SDL_Point theMapAreaClicked, int theTreeToPlant);
	void setMapStartXY(SDL_Point startPosXY);
	SDL_Point getMapStartXY();
};
#endif