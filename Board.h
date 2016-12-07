#pragma once
#include <vector>
#include "BoardSpace.h"
#include "Player.h"
#include "AI.h"
#include <cctype>
#include <sstream>
#include <climits>

class Board {
public:
	Card middleCard;

	//Game board (5X5 board)
	std::vector<std::vector<BoardSpace>> board;
	
	//Constructor
	Board();

	//Printing methods
	void printBoard();
	
	//Movement of pieces
	void movePiece(std::string);

	//Movement of cards
	void playCard(Player &);
	void playCard(AI &, Card);

	//Validation methods
	bool isVictory(bool);
	bool redKingExists();
	bool blueKingExists();
	bool redKingOnBlueSpace();
	bool blueKingOnRedSpace();

	//Validation methods
	bool validateInput(std::string, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>);
};