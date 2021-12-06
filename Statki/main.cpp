#include <iostream>
#include "Position.h"
#include "Ship.h"
#include "Player.h"
#include "Board.h"
#include "Vector.h"

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
	int default_ship_numbers[SHIP_TYPES] = { 1, 2, 3, 4 };
	bool all_ships_placed = false;

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
		}

		else if (!strcmp(command, "[playerB]")){
			player = players[1];
		}

		if (!strcmp(command, "PRINT")) {
			int print_mode = 0;
			std::cin >> print_mode;
			printHandler(print_mode, board);
		}

		else if (!strcmp(command, "PLACE_SHIP")) {
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
				for (int i = 0; i < 2; i++) {
					if (players[i]->current_ships != players[i]->max_ships) {
						break;
					}
					else {
						all_ships_placed = true;
					}
				}
			}

		}

		else if (!strcmp(command, "SHOOT")) {
			int y, x;
			std::cin >> y >> x;
			
			if (y < SIZE_Y && x < SIZE_X) {
				if (all_ships_placed) {
					player->Shoot({ y, x, false });
					player->getAllShipPositions();
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