#pragma once
#include "Position.h"


#define SIZE_Y 21
#define SIZE_X 10
#define SHIP_TYPES 4
#define SHIP_LENGTHS 10
#define MAX_SEGMENTS 5

class Board {
public:
	void initBoard();
	const void printBoard(int border_y);
	void placeChar(Position_t, char);
	char board[SIZE_Y][SIZE_X];
};
