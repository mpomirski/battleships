#pragma once
#include "Ship.h"
#include "Board.h"
#include "Position.h"
#include "Vector.h"

#define SIZE_Y 21
#define SIZE_X 10
#define SHIP_TYPES 4
#define SHIP_LENGTHS 10
#define MAX_SEGMENTS 5

class Player {
public:
	char name;
	Ship ships[SHIP_TYPES][SHIP_LENGTHS];
	bool isFleetSet;
	int current_ships;
	int max_ships;
	int ship_segments;
	Player();
	void initShips(int default_ship_numbers[SHIP_TYPES]);
	Ship* selectShip(int index, char ship_class[SHIP_TYPES]);
	bool placeShip(Board* board, Position_t position, char direction, int index, char ship_class[SHIP_TYPES]);
	Vector<Position_t*> getAllShipPositions();
	void Shoot(Position_t position, Board* board);

private:
	bool isValidPlacement(Position_t position, int ship_length, char direction);
};
