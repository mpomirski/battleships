#include <iostream>
#define SIZE_Y 21
#define SIZE_X 10

typedef struct Position {
	int x;
	int y;
} Position_t;


//Game board
class Board {
public:
	void initBoard();
	void printBoard();
	void placeChar(Position_t, char);

private:
	char board[SIZE_Y][SIZE_X];
};

void Board::initBoard() {
	for (int i = 0; i < SIZE_Y; i++) {
		for (int j = 0; j < SIZE_X; j++) {
			board[i][j] = '0';
		}
	}
}

void Board::printBoard() {
	for (int i = 0; i < SIZE_Y; i++) {
		for (int j = 0; j < SIZE_X; j++) {
			std::cout << board[i][j] << " ";
		}
		std::cout << "\n";
	}
}

void Board::placeChar(Position_t position, char c) {
	board[position.y][position.x] = c;
}


//Battleships
//
class Ship {
public:
	int size;
	char ship_class[4];
	Ship();
};

Ship::Ship(){
	size = 0;
	strcpy_s(ship_class, "");
};

//Player
class Player {
public:
	char name;
	Ship ships[4][10];
	bool his_turn;
	Player();
	void initShips(int default_ship_numbers[4]);
	void placeShip(Board* board, Ship ship, Position_t position, char direction);

private:
	bool isValidPlacement(Position_t position, int ship_length, char direction);
};

Player::Player() {
	name = ' ';
	his_turn = false;
}

void Player::initShips(int default_ship_numbers[4]) {
	//default_ship_numbers[4] = { 1, 2, 3, 4 };
	//1 carrier, 2 battleships, 3 cruisers, 4 destroyers

	const char names[4][4] = { "CAR", "BAT", "CRU", "DES" };
	const int ship_sizes[4] = { 5, 4, 3, 2 };

	Ship carrier, battleship, cruiser, destroyer;
	Ship choose_ships[4] = { carrier, battleship, cruiser, destroyer };

	for (int i = 0; i < (sizeof(choose_ships) / sizeof(Ship)); i++) {
		choose_ships[i].size = default_ship_numbers[i];
		strcpy_s(choose_ships[i].ship_class, names[i]);
	}

	//For every element in default_ship_numbers, add the corresponding
	for (int i = 0; i < sizeof(default_ship_numbers); i++) {
		for (int j = 0; j < default_ship_numbers[i]; j++) {
			ships[i][j] = choose_ships[i];
		}
	}
	
}
//DOKOŃCZYĆ TUTAJ!!!!!!!
void Player::placeShip(Board* board, Ship ship, Position_t position, char direction) {
	if (isValidPlacement(position, ship.size, direction)) {
		board->placeChar(position, 'x');
	}
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

	return false;
}


int main()
{
	Board board;
	board.initBoard();
	board.printBoard();

	Player A, B;
	A.name = 'A';
	B.name = 'B';

	int default_ship_numbers[4] = { 1, 2, 3, 4 };

	A.initShips(default_ship_numbers);
	B.initShips(default_ship_numbers);

	//dokonczyc tu
	//A.placeShip(board*, A.ships[0][0], )


	return 0;
}