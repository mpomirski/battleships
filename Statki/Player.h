#pragma once
#include "Ship.h"
#include "Board.h"
#include "Position.h"
#include "Vector.h"

#define SHIP_TYPES 4
#define SHIP_LENGTHS 10
#define MAX_SEGMENTS 5

class Player {
public:
	char name;
	bool isFleetSet;
	int current_ships;
	int max_ships;
	int ship_segments;
	short round;
	int min_y, max_y;
	int min_x, max_x;
	bool bounded;

	Player();
	void initShips(int default_ship_numbers[SHIP_TYPES]);
	bool placeShip(Board* board, Position_t position, char direction, int index, char ship_class[SHIP_TYPES], Vector<int>* set_segments = nullptr);
	Vector<Position_t*> getAllShipPositions();
	void Shoot(Position_t position, Board* board);

private:
	const bool isValidPlacement(Position_t position, int ship_length, char direction);

	Ship ships[SHIP_TYPES][SHIP_LENGTHS];
	Ship* selectShip(int index, char ship_class[SHIP_TYPES]);
};
