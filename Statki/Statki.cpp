#include <iostream>
#define SIZE_Y 21
#define SIZE_X 10

typedef struct Position {
	int y;
	int x;
} Position_t;

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
			if (i == SIZE_Y / 2) {
				board[i][j] = 'x';
			}
			else {
				board[i][j] = '0';
			}
		}
	}
}

void Board::printBoard() {
	for (int i = 0; i < SIZE_Y; i++) {
		for (int j = 0; j < SIZE_X; j++) {
			std::cout << board[i][j] << " ";
		}
		std::cout << "\t" << i;
		std::cout << "\n";
	}
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
	bool his_turn;
	Player();
	void initShips(int default_ship_numbers[4]);
	Ship* selectShip(int index, char ship_class[4]);
	void placeShip(Board* board, Position_t position, char direction, int index, char ship_class[4]);

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
	//I'll refactor this later
	if (!strcmp(ship_class, "CAR")) {
		if (ships[0][index].size != 0) {
			return &ships[0][index];
		}
		else {
			throw "SHIP DOESN'T EXIST";
		}
	}
	else if (!strcmp(ship_class, "BAT")) {
		if (index <= (sizeof(ships[1]) / sizeof(ships[1][0]))) {
			return &ships[1][index];
		}
	}
	else if (!strcmp(ship_class, "CRU")) {
		if (index <= (sizeof(ships[2]) / sizeof(ships[2][0]))) {
			return &ships[2][index];
		}
	}
	else if (!strcmp(ship_class, "DES")) {
		if (index <= (sizeof(ships[3]) / sizeof(ships[3][0]))) {
			return &ships[3][index];
		}
	}
}

void Player::placeShip(Board* board, Position_t position, char direction, int index, char ship_class[4]) {
	Ship* ship;
	try {
		ship = selectShip(index, ship_class);
	}
	catch (const char* error) {
		std::cout << error;
		return;
	}

	if (ship->is_placed) {
		std::cout << "SHIP ALREADY PLACED\n";
		return;
	}

	if (isValidPlacement(position, ship->size, direction)) {
		int count = 1;
		for (int i = 0; i < ship->size; i++) {
			//Check if the ship doesn't collide
			position = positionLoop(position, direction, 1);
			if (board->board[position.y][position.x] == '0') {
				count++;
			}
		}

		if (count >= ship->size) {
			for (int i = 0; i < ship->size; i++) {
				//Draw the ship on the board
				position = positionLoop(position, direction, -1);
				board->placeChar(position, 'x');
			}
		}
		else {
			std::cout << "ERROR PLACING SHIP\n";
		}
	}
	else {
		std::cout << "ERROR NOT VALID PLACEMENT\n";
	}

	ship->is_placed = true;

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


	//TODO: CHOOSING PLAYERS
	for (int i = 0; i < 2; i++) {
		if (players[i]->his_turn) {
			player = players[i];
		}
	}

	//Hideous code ahead, Cpp's fault ¯\_ (ツ)_/¯
	char command[30];
	while (std::cin >> command) {
		if (strcmp(command, "[playerA]") == false || strcmp(command, "[playerB]") == false) {
			player->his_turn = !player->his_turn;
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
			player->placeShip(board, { position_y, position_x }, direction, index, ship_class);
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