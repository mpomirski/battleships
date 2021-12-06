#pragma once
#include "Position.h"
#include "Vector.h"

#define SIZE_Y 21
#define SIZE_X 10
#define SHIP_TYPES 4
#define SHIP_LENGTHS 10
#define MAX_SEGMENTS 5


class Ship {
public:
	int size;
	char ship_class[SHIP_TYPES];
	bool is_placed;
	Ship();
	Vector<Position_t> segments;
};
