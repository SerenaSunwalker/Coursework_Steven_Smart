/*
=================
cKey.h
- Header file for class definition - SPECIFICATION
- Header file for the Key class which is a child of cSprite class
=================
*/
#ifndef _CKEY_H
#define _CKey_H
#include "cSprite.h"

class cKey : public cSprite
{
private:
	SDL_Point keyVelocity;

public:
	cKey();
	void update(double deltaTime);		// Rocket update method
	void setKeytVelocity(SDL_Point rocketVel);   // Sets the velocity for the rocket
	SDL_Point getKeyVelocity();				 // Gets the rocket velocity
};
#endif