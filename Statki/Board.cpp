#include <iostream>
#include "Board.h"
//Game board

void Board::initBoard() {
	for (int i = 0; i < SIZE_Y; i++) {
		for (int j = 0; j < SIZE_X; j++) {
			//if (i == SIZE_Y / 2) {
			//	board[i][j] = 'x';
			//}
			//else {
			board[i][j] = ' ';
			//}
		}
	}
}

const void Board::printBoard(int border_y) {
	int player_A_ships_remaining = 0;
	int player_B_ships_remaining = 0;

	for (int i = 0; i < SIZE_Y; i++) {
		for (int j = 0; j < SIZE_X; j++) {
			std::cout << board[i][j];
			if (board[i][j] == '+') {
				if (i < border_y/*(SIZE_Y / 2)*/) {
					player_A_ships_remaining++;
				}
				else {
					player_B_ships_remaining++;
				}
			}
		}
		std::cout << "\n";
	}
	if ((player_A_ships_remaining + player_B_ships_remaining) != 0)
		std::cout << "PARTS REMAINING:" << ": A : " << player_A_ships_remaining << " B : " << player_B_ships_remaining << "\n";
}

void Board::placeChar(Position_t position, char c) {
	board[position.y][position.x] = c;
}
