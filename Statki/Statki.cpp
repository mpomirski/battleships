#include <iostream>
#define SIZE_Y 21
#define SIZE_X 10

//TODO: SPLIT INTO MULTIPLE FILES

typedef struct Position {
	int y;
	int x;
} Position_t;
class Player;
class Ship;
class Board;

//Loop for N/W/S/E position system
Position_t positionLoop(Position_t position, char direction, int increment) {
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

//Game board
class Board {
public:
	void initBoard();
	void printBoard();
	void placeChar(Position_t, char);
	char board[SIZE_Y][SIZE_X];
};

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

void Board::printBoard(){
	int player_A_ships_remaining = 0;
	int player_B_ships_remaining = 0;

	for (int i = 0; i < SIZE_Y; i++) {
		for (int j = 0; j < SIZE_X; j++) {
			std::cout << board[i][j];
			if (board[i][j] == '+') {
				if (i < 10) {
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


//Battleships
class Ship {
public:
	int size;
	char ship_class[4];
	bool is_placed;
	Ship();
};

Ship::Ship(){
	size = 0;
	strcpy_s(ship_class, "");
	is_placed = false;
};

//Player
class Player {
public:
	char name;
	Ship ships[4][10];
	Player();
	void initShips(int default_ship_numbers[4]);
	Ship* selectShip(int index, char ship_class[4]);
	bool placeShip(Board* board, Position_t position, char direction, int index, char ship_class[4]);

private:
	bool isValidPlacement(Position_t position, int ship_length, char direction);
	static const char names[4][4];
};

const char Player::names[4][4] = { "CAR", "BAT", "CRU", "DES" };

Player::Player() {
	name = ' ';
}

void Player::initShips(int default_ship_numbers[4]) {
	//default_ship_numbers[4] = { 1, 2, 3, 4 };
	//1 carrier, 2 battleships, 3 cruisers, 4 destroyers
	const int ship_sizes[4] = { 5, 4, 3, 2 };

	Ship carrier, battleship, cruiser, destroyer;
	Ship choose_ships[4] = { carrier, battleship, cruiser, destroyer };

	for (int i = 0; i < (sizeof(choose_ships) / sizeof(Ship)); i++) {
		choose_ships[i].size = ship_sizes[i];
		strcpy_s(choose_ships[i].ship_class, names[i]);
	}

	//For every element in default_ship_numbers, add the corresponding
	for (int i = 0; i < sizeof(default_ship_numbers); i++) {
		for (int j = 0; j < default_ship_numbers[i]; j++) {
			ships[i][j] = choose_ships[i];
		}
	}
	
}

Ship* Player::selectShip(int index, char ship_class[4]) {
	//const char Player::names[4][4] = { "CAR", "BAT", "CRU", "DES" };
	for (int i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
		if (!strcmp(ship_class, names[i])) {
			if (ships[i][index].size != 0) {
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

bool Player::placeShip(Board* board, Position_t position, char direction, int index, char ship_class[4]) {
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
				board->placeChar(position, '+');
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

	//Hideous code ahead, Cpp's fault ¯\_ (ツ)_/¯
	char command[30];
	while (std::cin >> command) {
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
			int position_y, position_x;
			char direction;
			int index;
			char ship_class[4];

			std::cin >> position_y >> position_x >> direction >> index >> ship_class;
			if ((player->placeShip(board, { position_y, position_x }, direction, index, ship_class)) == true) {
				break;
			};
		}

		else if (!strcmp(command, "SHOOT")) {
			int y, x;
			std::cin >> y >> x;
			if (board->board[y][x] == '+') {
				board->placeChar({ y, x }, 'x');
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

	int default_ship_numbers[4] = { 1, 2, 3, 4 };

	A->initShips(default_ship_numbers);
	B->initShips(default_ship_numbers);

	userInputHandler(&board, players);

	//B.placeShip(&board, B.ships[0][0], { 15, 5 }, 'W');
	//A.placeShip(&board, A.ships[0][0], { 6, 0 }, 'N');

	return 0;
}