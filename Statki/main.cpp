#include <iostream>
#include "Position.h"
#include "Ship.h"
#include "Player.h"
#include "Board.h"
#include "Vector.h"
#define NUM_OF_PLAYERS 2

Player* choosePlayerByName(char* name, Player* players[NUM_OF_PLAYERS]);

int charToInt(char ch);
void printHandler(int print_mode, Board* board, Player players[NUM_OF_PLAYERS]);
void userInputHandler(Player* players[NUM_OF_PLAYERS]);
void stateHandler(Board* board, Player* players[NUM_OF_PLAYERS], Player* player);
void playerHandler(Player* player, Player* other_player, Player* players[2], char command[30]);

void initDefaultShips(Player* players[NUM_OF_PLAYERS]);
void placeShip(Player* player, Player* players[NUM_OF_PLAYERS], Board* board, bool* all_ships_placed);
void shoot(Board* board, bool all_ships_placed, Player* other_player);
void win(bool ships_flag_set, Player* players[NUM_OF_PLAYERS]);

int main()
{
	Player* A = new Player;
	Player* B = new Player;
	Player* players[NUM_OF_PLAYERS] = { A, B };

	A->name = 'A';
	B->name = 'B';

	userInputHandler(players);

	exit(0);
	return 0;
}


void printHandler(int print_mode, Board* board, Player players[NUM_OF_PLAYERS]) {
	switch (print_mode) {
	case 0: {
		board->printBoard(players[0].max_y + 1);
		break;
	}
	}
}

Player* choosePlayerByName(char* name, Player* players[NUM_OF_PLAYERS]) {
	if (!strcmp(&name[0], "A")) {
		return players[0];
	}
	else {
		return players[1];
	}
}

//If the fleet isn't set yet, initialize it using the default ship values
void initDefaultShips(Player* players[NUM_OF_PLAYERS]) {
	int default_ship_numbers[SHIP_TYPES] = { 1, 2, 3, 4 };
	for (int i = 0; i < NUM_OF_PLAYERS; i++) {
		if (!players[i]->isFleetSet) {
			players[i]->initShips(default_ship_numbers);
		}
	}
}

int charToInt(char ch) {
	return((ch >= '0' && ch <= '9') ? ch - '0' : 0);
}

void stateHandler(Board* board,  Player* players[NUM_OF_PLAYERS], Player* player) {
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

		if (!strcmp(command, "BOARD_SIZE")) {
			int y, x;
			std::cin >> y >> x;
			board->changeSize(y, x);
		}

		if (!strcmp(command, "[state]")) {
			return;
		}
	}
};

void playerHandler(Player* player, Player* other_player, Player* players[2], char command[30]) {
	player->round++;
	if (player->round == other_player->round) {
		std::cout << "INVALID OPERATION \"" << command << " \": THE OTHER PLAYER EXPECTED\n";
		throw "e";
	}
}

void placeShip(Player* player, Player* players[NUM_OF_PLAYERS], Board* board, bool* all_ships_placed) {
	initDefaultShips(players);

	int position_y, position_x;
	char direction;
	int index;
	char ship_class[4];

	std::cin >> position_y >> position_x >> direction >> index >> ship_class;
	//Checks and executes placeShip, returning true if it failed
	if ((player->placeShip(board, { position_y, position_x, false }, direction, index, ship_class)) == true) {
		throw "e";
	};

	if (!*all_ships_placed) {
		if (players[0]->current_ships == players[0]->max_ships && players[1]->current_ships == players[1]->max_ships) {
			*all_ships_placed = true;
		}
	}
};

void shoot(Board* board, bool all_ships_placed, Player* other_player) {
	int y, x;
	std::cin >> y >> x;

	if (y < board->size_y && x < board->size_x) {
		if (all_ships_placed) {
			other_player->Shoot({ y, x, false }, board);
		}
		else {
			std::cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": NOT ALL SHIPS PLACED\n";
			throw "e";
		}
	}
	else {
		std::cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": FIELD DOES NOT EXIST\n";
		throw "e";
	}
}

void win(bool *ships_flag_set, Player* players[NUM_OF_PLAYERS]) {
	if (!*ships_flag_set) {
		for (int i = 0; i < NUM_OF_PLAYERS; i++) {
			players[i]->ship_segments = players[i]->getAllShipPositions().cur_length;
		}
		*ships_flag_set = true;
	}
	for (int i = 0; i < NUM_OF_PLAYERS; i++) {
		if (players[i]->ship_segments == 0) {
			std::cout << players[(i + 1) % 2]->name << " won\n";
			throw "e";
		}
	}
}

void userInputHandler(Player* players[NUM_OF_PLAYERS]) {
	Board* board = new Board();
	Player* player = players[0];
	Player* other_player = players[1];
	int default_ship_numbers[SHIP_TYPES] = { 1, 2, 3, 4 };
	bool all_ships_placed = false;
	bool ships_flag_set = false;

	char command[30];
	while (std::cin >> command) {
		if (!strcmp(command, "[state]")) {
			stateHandler(board, players, player);
		}

		if (!strcmp(command, "[playerA]")) {
			try { playerHandler(player, other_player, players, command); }
			catch (const char* _) { break; }
			player = players[0];
			other_player = players[1];
		}

		else if (!strcmp(command, "[playerB]")) {
			try { playerHandler(player, other_player, players, command); }
			catch (const char* _) { break; }
			player = players[1];
			other_player = players[0];
		}

		if (!strcmp(command, "PLACE_SHIP")) {
			try { placeShip(player, players, board, &all_ships_placed); }
			catch (const char* _) { break; }
		}

		if (!strcmp(command, "SHOOT")) {
			try { shoot(board, all_ships_placed, other_player); }
			catch (const char* _) { break; }

		}

		if (all_ships_placed) {
			try { win(&ships_flag_set, players); }
			catch (const char* _) { return; }
		}
	}
}
