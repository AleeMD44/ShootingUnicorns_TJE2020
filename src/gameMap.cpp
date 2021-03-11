#include "gameMap.h"
#include "utils.h"
#include "image.h"
#include <assert.h>
#include <cmath>
#include <iostream>
#include "game.h"


gameMap::gameMap(int w, int h) {
	width = w;
	height = h;
	data = new sCell[w*h];
}

gameMap::gameMap() {}

sCell& gameMap::getCell(int x, int y) {
	return data[x + y * width];
}

bool gameMap::loadGameMap(const char * filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
		return false;

	sMapHeader header;
	fread(&header, sizeof(sMapHeader), 1, file);

	assert(header.bytes == 1);
	unsigned char* cells = new unsigned char[header.w*header.h];
	fread(cells, header.bytes, header.w*header.h, file);
	fclose(file);

	width = header.w;
	height = header.h;

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			getCell(i, j).type = (eCellType)cells[i + j * width];
	delete[] cells;
	return true;
}

void gameMap::render(Image& fb, Image& tileset)
{
	int cs = tileset.width / 16;
	for(int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			sCell& cell = getCell(i,j);
			
			if (cell.type == 0)
				continue;

			int tilex = (cell.type % 16)*cs;
			int tiley = (cell.type / 16)*cs;

			Area area(tilex, tiley, cs, cs);
			if (i < -cs || i * cs > fb.width || j < -cs || j * cs > fb.height)
				continue;
			fb.drawImage(tileset, i*cs, j*cs, area);
		}
}

bool gameMap::isValid(Vector2 target, int facing)
{
	target.x = floor(target.x / 8);
	target.y = floor(target.y / 8);

	Vector2 aux;

	bool move = false;

	if (facing == 0) {//face_right
		aux.x = target.x + 1;
		aux.y = target.y;

		sCell& cell = getCell(aux.x, aux.y);

		move = movable(cell);
	}

	if (facing == 1) {//face_left
		if (target.x > 0) {
			aux.x = target.x - 1;
			aux.y = target.y;

			sCell& cell = getCell(aux.x, aux.y);
			move = movable(cell);
		}
	}

	if (facing == 2) {//face_down
		if (target.y > 0) {
			aux.y = target.y + 1;
			aux.x = target.x;

			sCell& cell = getCell(aux.x, aux.y);
			move = movable(cell);
		}
	}

	if (facing == 3) {//face_up
		if (target.y > 0) {
			aux.y = target.y - 1;
			aux.x = target.x;

			sCell& cell = getCell(aux.x, aux.y);
			move = movable(cell);
		}
	}
	return move;
}

bool gameMap::isObject(Vector2 pos, eCellType from, eCellType to)
{
	pos.x = floor(pos.x / 8);
	pos.y = floor(pos.y / 8);

	sCell& cell = getCell(pos.x, pos.y);

	if (cell.type == from) {
		cell.type = (eCellType)to;
		return true;
	}
	return false;
}

bool gameMap::movable(sCell& cell) {
	
	for (int i = 0; i < (sizeof(notmovableObjects)/sizeof(*notmovableObjects)); i++) {
		if (cell.type == notmovableObjects[i])
			return false;
	}
	return true;
}


void gameMap::changeTile(Vector2 pos, eCellType type) {

	if (pos.x < Game::instance->world->tileset.width && pos.x >= 0 && pos.y >= 0 && pos.y < Game::instance->world->tileset.height) {
		sCell& cell = getCell(pos.x, pos.y);
		cell.type = (eCellType)type;
	}
}


