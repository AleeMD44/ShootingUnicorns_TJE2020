#include "Sprite.h"
#include "game.h"
#include <cmath>

Sprite::Sprite(Vector2 pp, int f, int ps) {
	player_pos = pp;
	target_pos = pp;
	facing = f;
}
Sprite::Sprite() {}

bool Sprite::Move(int direction) {
	facing = direction;
	Vector2 aux = player_pos;

	if (direction == FACE_RIGHT) {	
		if (Game::instance->world->map->isValid(aux, facing)) {
			player_pos.x +=  8;
		}
		else {return false; }
	}
	if (direction == FACE_DOWN) {
		if (Game::instance->world->map->isValid(aux, facing)) {
			player_pos.y += 8;
		}
		else {return false;}
	}
	if (direction == FACE_LEFT) {
		if (Game::instance->world->map->isValid(aux, facing)) {
			player_pos.x -= 8;
		}
		else { return false;}
	}
	if (direction == FACE_UP) {
		if (Game::instance->world->map->isValid(aux, facing)) {
			player_pos.y -= 8;
		}
		else { return false;}
	}
	return true;
}

bool Sprite::BulletMove(int direction) {
	facing = direction;
	Vector2 aux = player_pos;

	if (direction == FACE_RIGHT) {
		if (Game::instance->world->map->isValid(aux, facing)) {
			player_pos.x += 8;
		}
		else { shoot = false; return false; }
	}
	if (direction == FACE_DOWN) {
		if (Game::instance->world->map->isValid(aux, facing)) {
			player_pos.y += 8;
		}
		else { shoot = false; return false; }
	}
	if (direction == FACE_LEFT) {
		if (Game::instance->world->map->isValid(aux, facing)) {
			player_pos.x -= 8;
		}
		else { shoot = false; return false; }
	}
	if (direction == FACE_UP) {
		if (Game::instance->world->map->isValid(aux, facing)) {
			player_pos.y -= 8;
		}
		else { shoot = false; return false; }
	}
	return true;
}
