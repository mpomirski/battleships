#pragma once
#include "Position.h"

#define SHIP_TYPES 4
#define SHIP_LENGTHS 10
#define MAX_SEGMENTS 5

class Board {
public:
	void printBoard(int border_y) const;
	void placeChar(Position_t, char);
	char** board = nullptr;
	int size_y = 21, size_x = 10;
	Board();
	void changeSize(int y, int x);
};
