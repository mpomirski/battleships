#include <iostream>
#include "Ship.h"

Ship::Ship() {
	size = 0;
	strcpy_s(ship_class, "");
	is_placed = false;
	for (int i = 0; i < (sizeof(segments) / sizeof(segments[0])); i++) {
		segments[i] = { -1, -1, false };
	}
};
