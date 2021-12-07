#include <iostream>
#include "Position.h"
#include "Ship.h"
#include "Player.h"
#include "Board.h"
#include "Vector.h"
#define NUM_OF_PLAYERS 2

void printHandler(int print_mode, Board* board) {
	switch (print_mode) {
	case 0: {
		board->printBoard();
		break;
		}
	}
}


void userInputHandler(Board* board, Player* players[2]) {
	Player* player = players[0];
	Player* other_player = players[1];
	int default_ship_numbers[SHIP_TYPES] = { 1, 2, 3, 4 };
	bool all_ships_placed = false;
	int turn_number = 1;
	bool ships_flag_set = false;

	//Hideous code ahead, Cpp's fault ¯\_ (ツ)_/¯
	char command[30];
	while (std::cin >> command) {

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

		if (!strcmp(command, "[playerA]")) {
			player = players[0];
			other_player = players[1];
		}

		else if (!strcmp(command, "[playerB]")){
			player = players[1];
			other_player = players[0];
		}

		if (!strcmp(command, "PRINT")) {
			int print_mode = 0;
			std::cin >> print_mode;
			printHandler(print_mode, board);
		}

		if (!strcmp(command, "PLACE_SHIP")) {
			//MOVE THIS TO A FUNCTION

			//If the fleet isn't set yet, initialize it using the defaults
			for (int i = 0; i < 2; i++) {
				if (!players[i]->isFleetSet) {
					players[i]->initShips(default_ship_numbers);
				}
			}

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
					std::cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": NOOT ALL SHIPS PLACED\n";
					break;
				}
			}
		}

		if (all_ships_placed && !ships_flag_set) {
			for (int i = 0; i < 2; i++) {
				players[i]->ship_segments = players[i]->getAllShipPositions().cur_length;
			}
			ships_flag_set = true;
		}

		if (all_ships_placed) {
			for (int i = 0; i < 2; i++) {
				if (players[i]->ship_segments == 0) {
					std::cout << players[(i+1)%2]->name << " won\n";
					return;
				}
			}
		}
	}
}

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

	//B.placeShip(&board, B.ships[0][0], { 15, 5 }, 'W');
	//A.placeShip(&board, A.ships[0][0], { 6, 0 }, 'N');

	return 0;
}