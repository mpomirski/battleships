#include <iostream>
#include "Position.h"
#include "Ship.h"
#include "Player.h"
#include "Board.h"
#include "Vector.h"
#define NUM_OF_PLAYERS 2

void printHandler(int print_mode, Board* board, Player players[2]);
Player* choosePlayerByName(char* name, Player* players[2]);
void initDefaultShips(Player* players[2]);
int charToInt(char ch);
void userInputHandler(Board* board, Player* players[2]);

//Main
int main()
{
	Board board;
	board.initBoard();

	Player* A = new Player;
	Player* B = new Player;
	Player* players[2] = { A, B };

	A->name = 'A';
	B->name = 'B';

	userInputHandler(&board, players);

	exit(0);
	return 0;
}


void printHandler(int print_mode, Board* board, Player players[2]) {
	switch (print_mode) {
	case 0: {
		board->printBoard(players[0].max_y + 1);
		break;
	}
	}
}

Player* choosePlayerByName(char* name, Player* players[2]) {
	if (!strcmp(&name[0], "A")) {
		return players[0];
	}
	else {
		return players[1];
	}
}

void initDefaultShips(Player* players[2]) {
	int default_ship_numbers[SHIP_TYPES] = { 1, 2, 3, 4 };
	//If the fleet isn't set yet, initialize it using the defaults
	for (int i = 0; i < 2; i++) {
		if (!players[i]->isFleetSet) {
			players[i]->initShips(default_ship_numbers);
		}
	}
}

int charToInt(char ch) {
	return((ch >= '0' && ch <= '9') ? ch - '0' : 0);
}

void stateHandler(Board* board, Player* players[2], Player* player) {
	char command[30];

	while (std::cin >> command) {
		if (!strcmp(command, "PRINT")) {
			int print_mode = 0;
			std::cin >> print_mode;
			printHandler(print_mode, board, *players);
		}

		if (!strcmp(command, "SET_FLEET")) {
			char player_name;
			std::cin >> player_name;

			int ship_numbers[SHIP_TYPES];
			for (int i = 0; i < SHIP_TYPES; i++) {
				std::cin >> ship_numbers[i];
			}
			if (player_name == 'A') {
				players[0]->initShips(ship_numbers);
				players[0]->isFleetSet = true;
			}
			else {
				players[1]->initShips(ship_numbers);
				players[1]->isFleetSet = true;
			}
		}

		if (!strcmp(command, "NEXT_PLAYER")) {
			char name[2];
			std::cin >> name;
			player = choosePlayerByName(name, players);
		}

		if (!strcmp(command, "REEF")) {
			int y, x;
			std::cin >> y >> x;
			board->placeChar({ y, x }, '#');
		}

		if (!strcmp(command, "SHIP")) {
			initDefaultShips(players);

			char name[2];
			char direction;
			int y, x, index;
			char positions[6] = {'\0', '\0', '\0', '\0', '\0', '\0' };
			char ship_class[4];
			Vector<int> position_vect;

			std::cin >> name >> y >> x >> direction >> index >> ship_class >> positions;
			player = choosePlayerByName(name, players);

			for (int i = 0; i < 5; i++) {
				if (positions[i] != '\0') {
					position_vect.push(charToInt(positions[i]));
				}
			}
			player->placeShip(board, { y, x, false }, direction, index, ship_class, &position_vect);
		}

		if (!strcmp(command, "INIT_POSITION")) {
			char name[2];
			int x1, x2, y1, y2;
			std::cin >> name;
			std::cin >> y1 >> x1 >> y2 >> x2;
			choosePlayerByName(name, players)->bounded = true;
			choosePlayerByName(name, players)->min_y = y1;
			choosePlayerByName(name, players)->max_y = y2;
			choosePlayerByName(name, players)->min_x = x1;
			choosePlayerByName(name, players)->max_x = x2;
		}

		if (!strcmp(command, "[state]")) {
			return;
		}
	}
};

void userInputHandler(Board* board, Player* players[2]) {
	Player* player = players[0];
	Player* other_player = players[1];
	int default_ship_numbers[SHIP_TYPES] = { 1, 2, 3, 4 };
	bool all_ships_placed = false;
	bool ships_flag_set = false;

	//Hideous code ahead, Cpp's fault ¯\_ (ツ)_/¯
	char command[30];
	while (std::cin >> command) {

		if (!strcmp(command, "[state]")) {
			stateHandler(board, players, player);
		}

		if (!strcmp(command, "[playerA]")) {
			player->round++;
			if (player->round == other_player->round) {
				std::cout << "INVALID OPERATION \"" << command << " \": THE OTHER PLAYER EXPECTED\n";
				break;
			}
			player = players[0];
			other_player = players[1];
		}

		else if (!strcmp(command, "[playerB]")) {
			player->round++;
			if (player->round == other_player->round) {
				std::cout << "INVALID OPERATION \"" << command << " \": THE OTHER PLAYER EXPECTED\n";
				break;
			}
			player = players[1];
			other_player = players[0];
		}

		if (!strcmp(command, "PLACE_SHIP")) {
			initDefaultShips(players);

			int position_y, position_x;
			char direction;
			int index;
			char ship_class[4];

			std::cin >> position_y >> position_x >> direction >> index >> ship_class;
			//Checks and executes placeShip, returning true if it failed
			if ((player->placeShip(board, { position_y, position_x, false }, direction, index, ship_class)) == true) {
				break;
			};

			if (!all_ships_placed) {
				if (players[0]->current_ships == players[0]->max_ships && players[1]->current_ships == players[1]->max_ships) {
					all_ships_placed = true;
				}
			}

		}

		if (!strcmp(command, "SHOOT")) {
			int y, x;
			std::cin >> y >> x;

			if (y < SIZE_Y && x < SIZE_X) {
				if (all_ships_placed) {
					other_player->Shoot({ y, x, false }, board);
				}
				else {
					std::cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": NOT ALL SHIPS PLACED\n";
					break;
				}
			}
			else {
				std::cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": FIELD DOES NOT EXIST\n";
				break;
			}
		}

		if (all_ships_placed) {
			if (!ships_flag_set) {
				for (int i = 0; i < 2; i++) {
					players[i]->ship_segments = players[i]->getAllShipPositions().cur_length;
				}
				ships_flag_set = true;
			}
			for (int i = 0; i < 2; i++) {
				if (players[i]->ship_segments == 0) {
					std::cout << players[(i + 1) % 2]->name << " won\n";
					return;
				}
			}
		}
	}
}
