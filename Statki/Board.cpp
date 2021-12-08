#include <iostream>
#include "Board.h"
//Game board

Board::Board() {
	board = new char* [size_y];
	for (int i = 0; i < size_y; i++) {
		board[i] = new char[size_x];
	}

	//Initialize the board with blanks
	for (int i = 0; i < size_y; i++) {
		for (int j = 0; j < size_x; j++) {
			board[i][j] = ' ';
		}
	}
}

void Board::changeSize(int y, int x) {
	size_y = y;
	size_x = x;
	board = new char* [size_y];
	for (int i = 0; i < y; i++) {
		board[i] = new char[size_x];
	}

	for (int i = 0; i < size_y; i++) {
		for (int j = 0; j < size_x; j++) {
			//if (i == SIZE_Y / 2) {
			//	board[i][j] = 'x';
			//}
			//else {
			board[i][j] = ' ';
			//}
		}
	}
}

void Board::printBoard(int border_y) const{
	int player_A_ships_remaining = 0;
	int player_B_ships_remaining = 0;

	//It just counts the ships placed on the board
	for (int i = 0; i < size_y; i++) {
		for (int j = 0; j < size_x; j++) {
			std::cout << board[i][j];

			if (board[i][j] == '+') {
				if (i < border_y) {
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
