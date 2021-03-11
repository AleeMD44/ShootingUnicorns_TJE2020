/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef WORLD_H
#define WORLD_H

#include "image.h"
#include "gameMap.h"
#include "Sprite.h"


class World
{
public:

	Sprite* player;
	gameMap* map;
	Image tileset;
	Image minifontw;
	Image minifontb;

	World();

};
#endif 