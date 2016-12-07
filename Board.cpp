#include "Board.h"
#include "Game.h"
#include "BoardSpace.h"
#include <Windows.h>

Board::Board()
{
	//     O
	// B B B B B
	// X X X X X 
	// X X X X X
	// X X X X X
	// R R R R R
	//     O
	board = { {BoardSpace(Piece(-1, 0, 'P')), BoardSpace(Piece(-1, 0, 'P')), BoardSpace(Piece(-10, 0, 'O'), -1), BoardSpace(Piece(-1, 0, 'P')), BoardSpace(Piece(-1, 0, 'P')) },
				  { BoardSpace(Piece()), BoardSpace(Piece()), BoardSpace(Piece()), BoardSpace(Piece()), BoardSpace(Piece()) },
				  { BoardSpace(Piece()), BoardSpace(Piece()), BoardSpace(Piece()), BoardSpace(Piece()), BoardSpace(Piece()) },
				  { BoardSpace(Piece()), BoardSpace(Piece()), BoardSpace(Piece()), BoardSpace(Piece()), BoardSpace(Piece()) },
				  { BoardSpace(Piece(1, 1, 'P')), BoardSpace(Piece(1, 1, 'P')), BoardSpace(Piece(10, 1, 'O'), 1), BoardSpace(Piece(1, 1, 'P')), BoardSpace(Piece(1, 1, 'P')) } };
}

void Board::printBoard()
{
	std::cout << "     A   B   C   D   E " << std::endl;
	std::cout << "   ---------------------" << std::endl;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < 5; i++) {
		std::cout << " " << i + 1 << " | ";
		for (int j = 0; j < 5; j++) {
			switch (board[i][j].piece.isBlue) {
			//Blue
			case 0:
				SetConsoleTextAttribute(hConsole, 9);
				break;
			//Red
			case 1:
				SetConsoleTextAttribute(hConsole, 4);
				break;
			case -1:
				SetConsoleTextAttribute(hConsole, 7);
				break;
			}
			std::cout << board[i][j].piece.letter;
			SetConsoleTextAttribute(hConsole, 7);
			std::cout << " | ";
		}
		if (i == 2) {
			//Print out the middle card
			std::cout << "Mid: " << middleCard.name;
			for (int i = 0; size_t(i) < middleCard.coordinatesVector.size(); i++) {
				std::cout << " (" << middleCard.coordinatesVector[i].first << ", " << middleCard.coordinatesVector[i].second << ")";
			}
		}
		std::cout << std::endl;
		std::cout << "   ---------------------" << std::endl;
	}
}

void Board::movePiece(std::string coordinateString)
{
	//Coordinates for old location
	int first = std::stoi(coordinateString.substr(0, 1)) - 1;
	int second = std::toupper(coordinateString[1]) - 65;

	//Coordinates for new location
	int third = std::stoi(coordinateString.substr(2, 1)) - 1;
	int fourth = std::toupper(coordinateString[3]) - 65;

	//Make the move
	board[third][fourth] = board[first][second];
	board[first][second] = BoardSpace(Piece());
}

void Board::playCard(Player &playerWithCard)
{
	Card cardPlayed = playerWithCard.promptCard();
	Card temp = middleCard;
	middleCard = cardPlayed;
	if (playerWithCard.hand.first.name == cardPlayed.name) {
		playerWithCard.hand.first = temp;
	}
	else {
		playerWithCard.hand.second = temp;
	}
}

void Board::playCard(AI &playerWithCard, Card chosenCard)
{
	Card AIcard;
	if (playerWithCard.hand.first.name == chosenCard.name)
		AIcard = playerWithCard.hand.first;
	else
		AIcard = playerWithCard.hand.second;
	Card temp = middleCard;
	middleCard = AIcard;
	if (playerWithCard.hand.first.name == AIcard.name) {
		playerWithCard.hand.first = temp;
	}
	else {
		playerWithCard.hand.second = temp;
	}
}

bool Board::isVictory(bool player)
{
	//Check blue's victory conditions
	if (player) {
		if (!redKingExists()) {
			return true;
		}
		if (blueKingOnRedSpace()) {
			return true;
		}
	}
	//Check red's victory conditions
	else {
		if (!blueKingExists()) {
			return true;
		}
		if (redKingOnBlueSpace()) {
			return true;
		}
	}
	return false;
}

bool Board::redKingExists()
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			//If the piece is red and the king piece
			if (board[i][j].piece.isBlue == 1 && board[i][j].piece.isOnmyo)
				return true;
		}
	}
	return false;
}

bool Board::blueKingExists()
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			//If the piece is blue and the king piece
			if (board[i][j].piece.isBlue == 0 && board[i][j].piece.isOnmyo)
				return true;
		}
	}
	return false;
}

bool Board::redKingOnBlueSpace()
{
	if (board[0][2].piece.isOnmyo && board[0][2].piece.isBlue == 1)
		return true;
	else
		return false;
}

bool Board::blueKingOnRedSpace()
{
	if (board[4][2].piece.isOnmyo && board[4][2].piece.isBlue == 0)
		return true;
	else
		return false;
}

bool Board::validateInput(std::string validationString, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> playerMoves)
{
	//Check if the coordinates are 4 characters long
	if (validationString.length() != 4)
		return false;
	int first = std::stoi(validationString.substr(0, 1)) - 1;
	int second = std::toupper(validationString[1]);
	int third = std::stoi(validationString.substr(2, 1)) - 1;
	int fourth = std::toupper(validationString[3]);
	//Check to make sure the coordinates don't go out of bounds (1-5) and they use valid letters (A-E)
	if (first > 4 || first < 0 || third > 4 || third < 0 || second < 65 || second > 69 || fourth < 65 || fourth > 69)
		return false;
	//Use playerMoves to determine if it's a valid move or not
	second = second - 65;
	fourth = fourth - 65;
	for (int i = 0; size_t(i) < playerMoves.size(); i++) {
		if (playerMoves[i].first.first == first &&
			playerMoves[i].first.second == second &&
			playerMoves[i].second.first == third &&
			playerMoves[i].second.second == fourth)
			return true;
	}
	return false;
}