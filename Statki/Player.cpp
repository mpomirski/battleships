#include <iostream>
#include "Player.h"

Player::Player() {
	name = ' ';
	isFleetSet = false;
	current_ships = 0;
	max_ships = 0;
	ship_segments = 0;
}
const char names[SHIP_TYPES][4] = { "CAR", "BAT", "CRU", "DES" };

//Loop for N/W/S/E position system
static Position_t positionLoop(Position_t position, char direction, int increment) {
	switch (direction) {
	case 'N':
		return { position.y + increment, position.x };
		break;
	case 'E':
		return { position.y, position.x - increment };
		break;
	case 'S':
		return { position.y - increment, position.x };
		break;
	case 'W':
		return { position.y, position.x + increment };
		break;
	}
	return { 0, 0 };
}

void Player::initShips(int default_ship_numbers[SHIP_TYPES]) {
	//default_ship_numbers[4] = { 1, 2, 3, 4 };
	//1 carrier, 2 battleships, 3 cruisers, 4 destroyers
	const int ship_sizes[SHIP_TYPES] = { 5, 4, 3, 2 };

	Ship carrier, battleship, cruiser, destroyer;
	Ship choose_ships[SHIP_TYPES] = { carrier, battleship, cruiser, destroyer };

	for (int i = 0; i < (sizeof(choose_ships) / sizeof(Ship)); i++) {
		choose_ships[i].size = ship_sizes[i];
		strcpy_s(choose_ships[i].ship_class, names[i]);
	}

	for (int i = 0; i < sizeof(default_ship_numbers); i++) {
		for (int j = 0; j < default_ship_numbers[i]; j++) {
			ships[i][j] = choose_ships[i];
		}
		max_ships += default_ship_numbers[i];
	}

	isFleetSet = true;
}

Ship* Player::selectShip(int index, char ship_class[SHIP_TYPES]) {
	//const char Player::names[4][4] = { "CAR", "BAT", "CRU", "DES" };
	for (int i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
		if (!strcmp(ship_class, names[i])) {
			if (&ships[i][index].size != 0) {
				return &ships[i][index];
			}
			else {
				throw "INVALID OPERATION";
			}
		}
	}
	//Only here not to upset the compiler
	return &ships[0][0];
}

Vector<Position_t*> Player::getAllShipPositions() {
	Vector <Position_t*> shipPositions;
	for (int i = 0; i < SHIP_TYPES; i++) {
		for (int j = 0; j < SHIP_LENGTHS; j++) {
			if (ships[i][j].size > 0) {
				for (int k = 0; k < MAX_SEGMENTS; k++) {
					if (ships[i][j].segments[k].x >= 0 && ships[i][j].segments[k].y >= 0) {
						shipPositions.push(&(ships[i][j].segments[k]));
					}
				}
			}
		}
	}
	return shipPositions;
}

bool Player::placeShip(Board* board, Position_t position, char direction, int index, char ship_class[SHIP_TYPES]) {
	Ship* ship;
	try {
		ship = selectShip(index, ship_class);
	}
	catch (const char* error) {
		std::cout << error;
		std::cout << " \"PLACE_SHIP " << position.y << " " << position.x << " " << direction << " " << index << " " << ship_class << "\": ALL SHIPS OF THE CLASS ALREADY SET\n";
		return 1;
	}

	if (ship->is_placed) {
		std::cout << "INVALID OPERATION \"PLACE_SHIP " << position.y << " " << position.x << " " << direction << " " << index << " " << ship_class << "\": SHIP ALREADY PRESENT\n";
		return 1;
	}

	if (isValidPlacement(position, ship->size, direction)) {

		int count = 1;
		for (int i = 0; i < ship->size; i++) {
			//Check if the ship doesn't collide
			position = positionLoop(position, direction, 1);
			if (board->board[position.y][position.x] == ' ') {
				count++;
			}
		}

		if (count >= ship->size) {
			for (int i = 0; i < ship->size; i++) {
				//Draw the ship on the board
				position = positionLoop(position, direction, -1);
				ship->segments[i] = position;
				if (position.is_hit) {
					board->placeChar(position, 'x');
				}
				else {
					board->placeChar(position, '+');
				}
			}


		}
		else {
			std::cout << "ERROR PLACING SHIP\n";
			return 1;
		}
	}
	else {
		std::cout << "INVALID OPERATION \"PLACE_SHIP " << position.y << " " << position.x << " " << direction << " " << index << " " << ship_class << "\": NOT IN STARTING POSITION";
		return 1;
	}

	ship->is_placed = true;
	current_ships++;
	return 0;
}

bool Player::isValidPlacement(Position_t position, int ship_length, char direction) {
	int min_y = 0, max_y = 0;
	if (name == 'A') {
		min_y = 0;
		max_y = 9;
	}
	else if (name == 'B') {
		min_y = 11;
		max_y = 20;
	}

	if ((position.x < SIZE_X && position.x >= 0) && (position.y >= min_y && position.y <= max_y)) {
		switch (direction) {
		case 'N':
			return position.y + ship_length - 1 <= max_y;
			break;
		case 'E':
			return position.x + 1 >= ship_length;
			break;
		case 'S':
			return position.y - ship_length + 1 >= min_y;
			break;
		case 'W':
			return position.x + ship_length <= SIZE_X;
			break;
		}
	}

	return false;
}

void Player::Shoot(Position_t position, Board* board) {
	for (int i = 0; i < getAllShipPositions().cur_length; i++) {
		if (position.x == (getAllShipPositions().arr[i])->x && position.y == (getAllShipPositions().arr[i])->y) {
			getAllShipPositions()[i]->is_hit = true;
			ship_segments--;
			board->placeChar({ position.y, position.x }, 'x');
		}
	}
}