#pragma once
#include "Position.h"
#include "Vector.h"

#define SHIP_TYPES 4
#define SHIP_LENGTHS 10
#define MAX_SEGMENTS 5


class Ship {
public:
	int size;
	char ship_class[SHIP_TYPES];
	bool is_placed;
	Ship();
	Position_t segments[MAX_SEGMENTS];
};

