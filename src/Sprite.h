/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "gameMap.h"

enum {FACE_RIGHT, FACE_LEFT, FACE_DOWN, FACE_UP};

class Sprite
{
public:
	Vector2 player_pos;
	Vector2 target_pos;

	int facing = FACE_RIGHT;

	bool shoot = false;

	int numBullets = 0;

	int life = 16;

	Sprite();
	Sprite(Vector2 pp, int f, int ps);

	bool Move(int direction);
	bool BulletMove(int direction);
};
#endif 