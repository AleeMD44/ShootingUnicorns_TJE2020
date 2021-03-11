/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "image.h"

enum eCellType {
	EMPTY = 0,
	GROUNDB = 29,
	WALL = 11, WALL2B = 28, WALL3B = 43,
	WALL2 = 82, WALL2b = 83, WALL2C = 84, WALL2D = 84, WALL2E = 85, WALL2F = 98, WALL2G = 99, WALL2H = 100,
	CORNERLU = 59, CORNERRU = 60, CORNERLD = 75, CORNERRD = 76,
	GUNB = 41,
	BULLETB = 40,
	EXIT = 12, EXIT2 = 10,
	ARROWRIGHTB = 16,
	ARROWLEFTB = 32,
	ARROWDOWNB = 48,
	ARROWUPB = 64,
	UNICORN = 34, UNICORN2 = 35, UNICORN3 = 36, UNICORN4 = 37,
	PLAYER = 20,


};

struct sCell {
	eCellType type;
};

struct sMapHeader {
	int w;
	int h;
	unsigned char bytes;
	unsigned char extra[7];
};

class gameMap
{
public:
	int width;
	int height;
	sCell* data;
	bool cameramove = true;

	int notmovableObjects[20] = {EMPTY, WALL, WALL2B, WALL3B, CORNERRU, CORNERLU, CORNERLD, CORNERRD, UNICORN, UNICORN2, UNICORN3, UNICORN4, WALL2, WALL2b, WALL2C, WALL2D, WALL2E, WALL2F, WALL2G, WALL2H};

	gameMap(int w, int h);
	gameMap();

	sCell& getCell(int x, int y);
	bool loadGameMap(const char* filename);
	void render(Image& fb, Image& tileset);
	bool isValid(Vector2 target, int facing);
	bool isObject(Vector2 pos, eCellType from, eCellType to);
	void changeTile(Vector2 pos, eCellType type);
	bool movable(sCell& cell);
};
#endif 