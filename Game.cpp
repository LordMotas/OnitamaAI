#include "Game.h"
#include "BoardSpace.h"
#include <Windows.h>

Game::Game()
{
	GameBoard = Board();
}

void Game::startGame()
{
	int fiftyMoveRule = 0, boardValue = 0;
	bool gameFinished = false;
	//false = Red, true = Blue
	bool player = determineFirstPlayer();
	while (!gameFinished) {
		Card cardToPlay;
		//Display Header
		printHeader();
		if (player)//Blue (Player/AI)
		{
			printAll();
			std::string choice;
			//If Player 1 is AI instead of human
			if (player1.level != 0) {
				//Get the string that is used by the AI
				std::pair<std::string, Card> logicResult = AIlogic(player1.hand, player2.hand, 0);

				//Swap cards with the middle
				GameBoard.movePiece(logicResult.first);
				printAll(); 
				GameBoard.playCard(player1, logicResult.second);
			}
			else {
				tryAgain:
				std::cout << "Coordinate format: 5A3A = Move the piece at 5A to space 3A" << std::endl;
				std::cout << "Coordinates: ";
				std::cin >> choice;
				//Perform validation here
				std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> playerMoves = generatePlayerMoves();
				if (GameBoard.validateInput(choice, playerMoves)) {
					GameBoard.movePiece(choice);
					//Swap cards with the middle
					printAll();
					GameBoard.playCard(player1);
				}
				else {
					system("cls");
					printAll();
					std::cout << "Invalid Input! Try Again!" << std::endl;
					goto tryAgain;
				}
			}
			printAll();
		}
		else {//Red (AI)
			//Get the string that is used by the AI
			printAll();
			std::pair<std::string, Card> logicResult = AIlogic(player2.hand, player1.hand, 1);

			//Swap cards with the middle
			GameBoard.movePiece(logicResult.first);
			GameBoard.playCard(player2, logicResult.second);
			printAll();
		}
		moves++;
		//This will check for victory conditions and finish the game if there is one
		if (isTerminal(GameBoard.board)) {
			gameFinished = true;
			goto Victory;
		}
		//Switch to Red
		if (player) {
			//Check for draw
			if (heuristicSum(GameBoard.board) == boardValue) {
				fiftyMoveRule++;
				if (fiftyMoveRule == 50)
					gameFinished = true;
			}
			else {
				boardValue = heuristicSum(GameBoard.board);
				fiftyMoveRule = 0;
			}
			player = false;
		}
		//Switch to Blue
		else {
			//Check for draw
			if (heuristicSum(GameBoard.board) == boardValue) {
				fiftyMoveRule++;
				if (fiftyMoveRule == 50)
					gameFinished = true;
			}
			else {
				fiftyMoveRule = 0;
				boardValue = heuristicSum(GameBoard.board);
			}
			player = true;
		}
	}
Victory:
	printAll();
	std::cout << "Game Over" << std::endl;
	if (fiftyMoveRule == 50) {
		std::cout << "The game is a draw!" << std::endl;
		draws++;
	}
	else if (player) {
		std::cout << "The winner is: Blue" << std::endl;
		opponentWins++;
	}
	else {
		std::cout << "The winner is: Red" << std::endl;
		aiWins++;
	}
}

void Game::printHeader()
{
	system("cls");
	std::cout << "=======================" << std::endl;
	std::cout << "  O  N  I  T  A  M  A  " << std::endl;
	std::cout << "=======================" << std::endl;
	std::cout << std::endl;
}

void Game::printPlayerCards(Player player)
{
	std::cout << "1) " << player.hand.first.name;
	for (int i = 0; size_t(i) < player.hand.first.coordinatesVector.size(); i++) {
		std::cout << " (" << player.hand.first.coordinatesVector[i].first << ", " << player.hand.first.coordinatesVector[i].second << ")";
	}
	std::cout << std::endl;
	std::cout << "2) " << player.hand.second.name;
	for (int i = 0; size_t(i) < player.hand.second.coordinatesVector.size(); i++) {
		std::cout << " (" << player.hand.second.coordinatesVector[i].first << ", " << player.hand.second.coordinatesVector[i].second << ")";
	}
	std::cout << std::endl;
}

void Game::printAll()
{
	printHeader();
	std::cout << "(x, y) -> (1, -1) = move right 1, then move down 1" << std::endl << std::endl;
	printPlayerCards(player1);
	GameBoard.printBoard();
	printPlayerCards(player2);
	std::cout << std::endl;
}

bool Game::determineFirstPlayer()
{
	if (GameBoard.middleCard.firstPlayer == "Blue")
		return true;
	else
		return false;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> Game::generatePlayerMoves()
{
	//Find all possible moves of the opponents
	std::vector<std::pair<int, int>> playerLocations;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (GameBoard.board[i][j].piece.isBlue == 0)
				playerLocations.push_back(std::pair<int, int>(i, j));
		}
	}
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> playerMoves;
	for (int i = 0; size_t(i) < playerLocations.size(); i++) {
		//First card in their hand
		for (int j = 0; size_t(j) < player1.hand.first.coordinatesVector.size(); j++) {
			//Add the move if it doesn't go out of bounds
			int newY = playerLocations[i].first + (-1) * player1.hand.first.coordinatesVector[j].second;
			int newX = playerLocations[i].second + player1.hand.first.coordinatesVector[j].first;
			if (newY <= 4 && newY >= 0 && newX <= 4 && newX >= 0) {
				std::pair<int, int> currentLocationPair = std::pair<int, int>(playerLocations[i].first, playerLocations[i].second);
				std::pair<int, int> newLocationPair = std::pair<int, int>(newY, newX);
				playerMoves.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(currentLocationPair, newLocationPair));
			}
		}
		//Second card in their hand
		for (int j = 0; size_t(j) < player1.hand.second.coordinatesVector.size(); j++) {
			//Add the move if it doesn't go out of bounds
			int newY = playerLocations[i].first + (-1) * player1.hand.second.coordinatesVector[j].second;
			int newX = playerLocations[i].second + player1.hand.second.coordinatesVector[j].first;
			if (newY <= 4 && newY >= 0 && newX <= 4 && newX >= 0) {
				std::pair<int, int> currentLocationPair = std::pair<int, int>(playerLocations[i].first, playerLocations[i].second);
				std::pair<int, int> newLocationPair = std::pair<int, int>(newY, newX);
				playerMoves.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(currentLocationPair, newLocationPair));
			}
		}
	}
	return playerMoves;
}

std::pair<std::string, Card> Game::AIlogic(std::pair<Card, Card> aiCardPair, std::pair<Card, Card> playerCardPair, int aiColor)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::vector<std::pair<int, int>> currentLocations;
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> possibleLocations;
	std::vector<Card> possibleLocationsCards;
	//Store all locations of each piece
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (GameBoard.board[i][j].piece.isBlue == aiColor)
				currentLocations.push_back(std::pair<int, int>(i, j));
		}
	}
	//Populate possible moves
	for (int i = 0; size_t(i) < currentLocations.size(); i++) {
		for (int j = 0; size_t(j) < aiCardPair.first.coordinatesVector.size(); j++) {
			int newX, newY;
			if (aiColor == 0) { //Player 1 as an AI
				newY = currentLocations[i].first + aiCardPair.first.coordinatesVector[j].second;
				newX = currentLocations[i].second + aiCardPair.first.coordinatesVector[j].first;
			}
			else {
				newY = currentLocations[i].first + (-1) * aiCardPair.first.coordinatesVector[j].second;
				newX = currentLocations[i].second + aiCardPair.first.coordinatesVector[j].first;
			}
			//Add the move if it doesn't go out of bounds
			if (newY <= 4 && newY >= 0 && newX <= 4 && newX >= 0 && GameBoard.board[newY][newX].piece.isBlue != aiColor) {
				std::pair<int, int> currentLocationPair = std::pair<int, int>(currentLocations[i].first, currentLocations[i].second);
				std::pair<int, int> newLocationPair = std::pair<int, int>(newY, newX);
				possibleLocations.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(currentLocationPair, newLocationPair));
				possibleLocationsCards.push_back(aiCardPair.first);
			}
		}
		for (int j = 0; size_t(j) < aiCardPair.second.coordinatesVector.size(); j++) {
			int newX, newY;
			if (aiColor == 0) { //Player 1 as an AI
				newY = currentLocations[i].first + aiCardPair.second.coordinatesVector[j].second;
				newX = currentLocations[i].second + aiCardPair.second.coordinatesVector[j].first;
			}
			else {
				newY = currentLocations[i].first + (-1) * aiCardPair.second.coordinatesVector[j].second;
				newX = currentLocations[i].second + aiCardPair.second.coordinatesVector[j].first;
			}
			//Add the move if it doesn't go out of bounds
			if (newY <= 4 && newY >= 0 && newX <= 4 && newX >= 0 && GameBoard.board[newY][newX].piece.isBlue != aiColor) {
				std::pair<int, int> currentLocationPair = std::pair<int, int>(currentLocations[i].first, currentLocations[i].second);
				std::pair<int, int> newLocationPair = std::pair<int, int>(newY, newX);
				possibleLocations.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(currentLocationPair, newLocationPair));
				possibleLocationsCards.push_back(aiCardPair.second);
			}
		}
	}
	int location;
	//Player 2 AI
	if (aiColor == 1) {
		std::uniform_int_distribution<int> random(0, possibleLocations.size() - 1);
		switch (player2.level) {
		case 1:
			//Random move
			location = random(mt);
			break;
		case 2:
			//Select moves that won't lose pieces and will win if possible
			location = trimWinMoves(possibleLocations, possibleLocationsCards, 0);
			break;
		case 3:
			//Use a Minimax decision tree to determine a winning move
			std::pair<std::pair<Card, Card>, std::pair<Card, Card>> cardPairs;
			cardPairs.first = aiCardPair;
			cardPairs.second = playerCardPair;
			std::pair<std::pair<std::pair<int, int>, std::pair<int, int>>, double> minimaxResult;
			minimaxResult = minimax(GameBoard.board, std::pair<std::pair<int, int>, std::pair<int, int>>{}, player2.depth, INT_MIN, INT_MAX, cardPairs, GameBoard.middleCard, true);
			for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
				if (minimaxResult.first.first.first == possibleLocations[i].first.first &&
					minimaxResult.first.first.second == possibleLocations[i].first.second &&
					minimaxResult.first.second.first == possibleLocations[i].second.first &&
					minimaxResult.first.second.second == possibleLocations[i].second.second) {
					location = i;
					break;
				}
				else {
					location = 0;
				}
			}
			break;
		}
	}
	else {
		std::uniform_int_distribution<int> random(0, possibleLocations.size() - 1);
		switch (player1.level) {
		case 1:
			//Random move
			location = random(mt);
			break;
		case 2:
			//Select moves that won't lose pieces and will win if possible
			location = trimWinMoves(possibleLocations, possibleLocationsCards, 1);
			break;
		case 3:
			//Use a Minimax decision tree to determine a winning move
			std::pair<std::pair<Card, Card>, std::pair<Card, Card>> cardPairs;
			cardPairs.first = aiCardPair;
			cardPairs.second = playerCardPair;
			std::pair<std::pair<std::pair<int, int>, std::pair<int, int>>, double> minimaxResult;
			minimaxResult = minimax(GameBoard.board, std::pair<std::pair<int, int>, std::pair<int, int>>{}, player1.depth, INT_MIN, INT_MAX, cardPairs, GameBoard.middleCard, false);
			for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
				if (minimaxResult.first.first.first == possibleLocations[i].first.first &&
					minimaxResult.first.first.second == possibleLocations[i].first.second &&
					minimaxResult.first.second.first == possibleLocations[i].second.first &&
					minimaxResult.first.second.second == possibleLocations[i].second.second) {
					location = i;
					break;
				}
				else {
					location = 0;
				}
			}
			break;
		}
	}
	std::string coordinates = "";
	return std::pair<std::string, Card>(convertToCoordinates(possibleLocations[location]), possibleLocationsCards[location]);
}

int Game::trimWinMoves(std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> & possibleLocations, std::vector<Card> & possibleLocationsCards, int aiColor)
{
	//Initialize random number
	std::random_device rd;
	std::mt19937 mt(rd());
	//Look for a winning move
	for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
		//If the ending space is the enemy king, take it
		if (GameBoard.board[possibleLocations[i].second.first][possibleLocations[i].second.second].piece.isOnmyo) {
			return i;
		}
		//If the ending space is the king space and you're using the king, take it
		if (GameBoard.board[possibleLocations[i].second.first][possibleLocations[i].second.second].isOnmyoSpace &&
			GameBoard.board[possibleLocations[i].first.first][possibleLocations[i].first.second].piece.isOnmyo) {
			return i;
		}
	}

	//Find all possible moves of the opponents
	std::vector<std::pair<int, int>> opponentLocations;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (GameBoard.board[i][j].piece.isBlue == aiColor)
				opponentLocations.push_back(std::pair<int, int>(i, j));
		}
	}
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> opponentMoves;
	std::vector<Card> opponentMovesCards;
	for (int i = 0; size_t(i) < opponentLocations.size(); i++) {
		//First card in their hand
		for (int j = 0; size_t(j) < player1.hand.first.coordinatesVector.size(); j++) {
			//Add the move if it doesn't go out of bounds
			int newY = opponentLocations[i].first + player1.hand.first.coordinatesVector[j].second;
			int newX = opponentLocations[i].second + (-1) * player1.hand.first.coordinatesVector[j].first;
			if (newY <= 4 && newY >= 0 && newX <= 4 && newX >= 0) {
				std::pair<int, int> currentLocationPair = std::pair<int, int>(opponentLocations[i].first, opponentLocations[i].second);
				std::pair<int, int> newLocationPair = std::pair<int, int>(newY, newX);
				opponentMoves.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(currentLocationPair, newLocationPair));
				opponentMovesCards.push_back(player1.hand.first);
			}
		}
		//Second card in their hand
		for (int j = 0; size_t(j) < player1.hand.second.coordinatesVector.size(); j++) {
			//Add the move if it doesn't go out of bounds
			int newY = opponentLocations[i].first + player1.hand.second.coordinatesVector[j].second;
			int newX = opponentLocations[i].second + (-1) * player1.hand.second.coordinatesVector[j].first;
			if (newY <= 4 && newY >= 0 && newX <= 4 && newX >= 0) {
				std::pair<int, int> currentLocationPair = std::pair<int, int>(opponentLocations[i].first, opponentLocations[i].second);
				std::pair<int, int> newLocationPair = std::pair<int, int>(newY, newX);
				opponentMoves.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(currentLocationPair, newLocationPair));
				opponentMovesCards.push_back(player1.hand.second);
			}
		}
	}

	int numKingTakePieces = 0;
	int previousX = -1, previousY = -1;
	int kingX = -1, kingY = -1;
	//Count the number of pieces that can take the king looking at the first coordinate
	for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
		for (int j = 0; size_t(j) < opponentMoves.size(); j++) {
			//If the opponent has a move that is the location of my king
			if (possibleLocations[i].first.first == opponentMoves[j].second.first &&
				possibleLocations[i].first.second == opponentMoves[j].second.second &&
				GameBoard.board[possibleLocations[i].first.first][possibleLocations[i].first.second].piece.isOnmyo &&
				GameBoard.board[possibleLocations[i].first.first][possibleLocations[i].first.second].piece.isBlue == 1) {
				if (!(previousY == opponentMoves[j].first.first && previousX == opponentMoves[j].first.second)) {
					numKingTakePieces++;
					previousY = opponentMoves[j].first.first;
					previousX = opponentMoves[j].first.second;
					kingX = possibleLocations[i].first.second;
					kingY = possibleLocations[i].first.first;
				}
			}
		}
	}

	//Look for a losing move (enemy takes your king)
	std::vector<int> eraseIndexVector;
	for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
		for (int j = 0; size_t(j) < opponentMoves.size(); j++) {
			if (possibleLocations[i].second.first == opponentMoves[j].second.first &&
				possibleLocations[i].second.second == opponentMoves[j].second.second &&
				GameBoard.board[possibleLocations[i].first.first][possibleLocations[i].first.second].piece.isOnmyo &&
				GameBoard.board[possibleLocations[i].first.first][possibleLocations[i].first.second].piece.isBlue == 1)
				if (std::find(eraseIndexVector.begin(), eraseIndexVector.end(), i) == eraseIndexVector.end())
					eraseIndexVector.push_back(i);
		}
	}

	//Look for a move to lose a piece
	for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
		for (int j = 0; size_t(j) < opponentMoves.size(); j++) {
			if (possibleLocations[i].second.first == opponentMoves[j].second.first &&
				possibleLocations[i].second.second == opponentMoves[j].second.second)
				if (std::find(eraseIndexVector.begin(), eraseIndexVector.end(), i) == eraseIndexVector.end())
					eraseIndexVector.push_back(i);
		}
	}

	//Strip out all "bad" moves
	size_t originalSize = eraseIndexVector.size();
	if (eraseIndexVector.size() == possibleLocations.size()) {
		std::uniform_int_distribution<int> random(0, possibleLocations.size() - 1);
		return random(mt);
	}
	for (int i = 0; i < (int)originalSize; i++) {		
		possibleLocations.erase(possibleLocations.begin() + eraseIndexVector[i]);
		possibleLocationsCards.erase(possibleLocationsCards.begin() + eraseIndexVector[i]);
		for (int j = 0; size_t(j) < eraseIndexVector.size(); j++) {
			if (eraseIndexVector[j] >= eraseIndexVector[i])
				eraseIndexVector[j]--;
		}
	}

	//If only one piece can take the king and you can safely take it, do it
	if (numKingTakePieces == 1) {
		for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
			for (int j = 0; size_t(j) < opponentMoves.size(); j++) {
				if (possibleLocations[i].first.first == opponentMoves[j].second.first &&
					possibleLocations[i].first.second == opponentMoves[j].second.second &&
					GameBoard.board[possibleLocations[i].first.first][possibleLocations[i].first.second].piece.isOnmyo &&
					GameBoard.board[possibleLocations[i].first.first][possibleLocations[i].first.second].piece.isBlue == 1) {
					//This means that the opponent at opponentMoves[j].first.first, opponentMoves[j].first.second can take the king
					for (int k = 0; size_t(k) < possibleLocations.size(); k++) {
						if (possibleLocations[k].second.first == opponentMoves[j].first.first &&
							possibleLocations[k].second.second == opponentMoves[j].first.second) {
							return k;
						}
					}
				}
			}
		}
		//If the piece cannot be taken, move the king away
		for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
			if (possibleLocations[i].first.first == kingY && possibleLocations[i].first.second == kingX) {
				return i;
			}
		}
		//If not, the game is checkmate
	}
	else if (numKingTakePieces > 1) {
		//Move the king away - potential checkmate situation
		for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
			if (possibleLocations[i].first.first == kingY && possibleLocations[i].first.second == kingX) {
				return i;
			}
		}
		//If nothing is found by this point, the game is a checkmate
	}

	//Look for a losing move (enemy king can take your king space)
	for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
		for (int j = 0; size_t(j) < opponentMoves.size(); j++) {
			if (opponentMoves[j].second.first == 4 && opponentMoves[j].second.second == 2 &&
				GameBoard.board[opponentMoves[j].first.first][opponentMoves[j].first.second].piece.isOnmyo &&
				GameBoard.board[possibleLocations[i].first.first][possibleLocations[i].first.second].piece.isBlue == 0) {
				//Take the enemy king or forfeit
				for (int k = 0; size_t(k) < possibleLocations.size(); k++) {
					if (possibleLocations[k].second.first == opponentMoves[j].first.first &&
						possibleLocations[k].second.second == opponentMoves[j].first.second) {
						return k;
					}
				}
				//At this point the game is checkmate, so take a random move
				std::uniform_int_distribution<int> random(0, possibleLocations.size() - 1);
				return random(mt);
			}
		}
	}

	//If possible, pick a safe move that can take an enemy piece
	for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
		for (int j = 0; size_t(j) < opponentMoves.size(); j++) {
			if (possibleLocations[i].second.first == opponentMoves[j].first.first &&
				possibleLocations[i].second.second == opponentMoves[j].first.second) {
				return i;
			}
		}
	}

	//Pick a different move
	std::uniform_int_distribution<int> random(0, possibleLocations.size() - 1);
	return random(mt);
}

std::string Game::convertToCoordinates(std::pair<std::pair<int, int>, std::pair<int, int>> selectedLocation) {
	std::ostringstream oss;
	oss << selectedLocation.first.first + 1;
	switch (selectedLocation.first.second) {
	case 0:
		oss << "A";
		break;
	case 1:
		oss << "B";
		break;
	case 2:
		oss << "C";
		break;
	case 3:
		oss << "D";
		break;
	case 4:
		oss << "E";
		break;
	}
	oss << selectedLocation.second.first + 1;
	switch (selectedLocation.second.second) {
	case 0:
		oss << "A";
		break;
	case 1:
		oss << "B";
		break;
	case 2:
		oss << "C";
		break;
	case 3:
		oss << "D";
		break;
	case 4:
		oss << "E";
		break;
	}
	return oss.str();
}

int Game::heuristicSum(std::vector<std::vector<BoardSpace>> board)
{
	int totalValue = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			totalValue += board[i][j].piece.pieceValue;
		}
	}
	return totalValue;
}

double Game::heuristicRatio(std::vector<std::vector<BoardSpace>> board)
{
	double blueValue = 0.0, redValue = 0.0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (board[i][j].piece.isBlue == 0)
				blueValue++;
			else if (board[i][j].piece.isBlue == 1)
				redValue++;
		}
	}
	return redValue/blueValue;
}

bool Game::isTerminal(std::vector<std::vector<BoardSpace>> board)
{
	bool redKingExists = false, blueKingExists = false;
	//Loop through the board and find if the following 4 cases are true
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			//1) Check if Red king exists
			if (board[i][j].piece.isOnmyo && board[i][j].piece.isBlue == 1)
				redKingExists = true;
			//2) Check if Blue king exists
			if (board[i][j].piece.isOnmyo && board[i][j].piece.isBlue == 0)
				blueKingExists = true;
		}
		//3) Red king on Blue space
		if (board[0][2].piece.isBlue == 1 && board[0][2].piece.isOnmyo)
			return true;
		//4) Blue king on red space
		if (board[4][2].piece.isBlue == 0 && board[4][2].piece.isOnmyo)
			return true;
	}
	if (!redKingExists || !blueKingExists)
		return true;
	else
		return false;
}

std::pair<std::pair<std::pair<int, int>, std::pair<int, int>>, double> Game::minimax(std::vector<std::vector<BoardSpace>> Node, std::pair<std::pair<int, int>, std::pair<int, int>> coordinates, int depth, double alpha, double beta, std::pair<std::pair<Card, Card>, std::pair<Card, Card>> cardPair, Card middleCard, bool player)
{
	//if depth = 0 or node is a terminal node
	if (depth == 0 || isTerminal(Node)) {
		std::pair<std::pair<std::pair<int, int>, std::pair<int, int>>, double> heuristicPair;
		//heuristicPair.first is the move that brough the Node to this point
		heuristicPair.first = coordinates;
		switch (player2.heuristic) {
		case 1:
			heuristicPair.second = heuristicSum(Node); //The heuristic value of the node as a sum
			break;
		case 2:
			heuristicPair.second = heuristicRatio(Node); //The heuristic value of the node as a ratio
			break;
		}
		return heuristicPair;
	}	
	//childNodes = generateMoves(node)
	std::pair<std::pair<std::vector<Card>, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>, std::vector<std::vector<std::vector<BoardSpace>>>> childNodes;

	childNodes = generateBoards(Node, cardPair, player);
	//childNodes.first.first is the Card that was played
	//childNodes.first.second another pair that contains the coordinates that were moved into
	//childNodes.second - A vector that contains all of the new board configurations

	std::pair<std::pair<std::pair<int, int>, std::pair<int, int>>, double> bestValue;
	//Initialize bestValue.first to some null value
	bestValue.first = coordinates;
	if (player) {
		bestValue.second = INT_MIN;
		//For each child of node
		for (int i = 0; size_t(i) < childNodes.second.size(); i++) {
			//Swap the cards and use that in the next function
			std::pair<std::pair<Card, Card>, std::pair<Card, Card>> newCardPair = cardPair;
			swapCards(newCardPair, childNodes.first.first[i], middleCard);

			std::pair<std::pair<std::pair<int, int>, std::pair<int, int>>, double> v;
			//Handle it if it's the first time called
			if (coordinates == std::pair<std::pair<int, int>, std::pair<int, int>>{})
				v = minimax(childNodes.second[i], childNodes.first.second[i], depth - 1, alpha, beta, newCardPair, middleCard, false);
			else
				v = minimax(childNodes.second[i], coordinates, depth - 1, alpha, beta, newCardPair, middleCard, false);

			if (bestValue.second != v.second) {
				bestValue.second = max(bestValue.second, v.second);
				if (bestValue.second == v.second)
					bestValue.first = v.first;
			}
			//bestValue.first is a pair of coordinates (first and second each)
			alpha = (int)max(alpha, v.second);
			if (beta <= alpha)
				break;
		}
		return bestValue;
	}
	else {
		bestValue.second = INT_MAX;
		//For each child of node
		for (int i = 0; size_t(i) < childNodes.second.size(); i++) {
			//Swap the cards and use that in the next function
			swapCards(cardPair, childNodes.first.first[i], middleCard);

			//Handle it if it's the first time called
			std::pair<std::pair<std::pair<int, int>, std::pair<int, int>>, double> v;
			if (coordinates == std::pair<std::pair<int, int>, std::pair<int, int>>{})
				v = minimax(childNodes.second[i], childNodes.first.second[i], depth - 1, alpha, beta, cardPair, middleCard, true);
			else
				v = minimax(childNodes.second[i], coordinates, depth - 1, alpha, beta, cardPair, middleCard, true);

			if (bestValue.second != v.second) {
				bestValue.second = min(bestValue.second, v.second);
				if (bestValue.second == v.second)
					bestValue.first = v.first;
			}
			beta = (int)min(beta, v.second);
			if (beta <= alpha)
				break;
		}
		return bestValue;
	}
}

std::pair<std::pair<std::vector<Card>, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>, std::vector<std::vector<std::vector<BoardSpace>>>> Game::generateBoards(std::vector<std::vector<BoardSpace>> board, std::pair<std::pair<Card, Card>, std::pair<Card, Card>> cardPair, bool player)
{
	//Given board, find all possible moves of said board down one level
	std::pair<std::pair<std::vector<Card>, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>, std::vector<std::vector<std::vector<BoardSpace>>>> newGameBoards;
	std::vector<std::pair<int, int>> currentLocations;
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> possibleLocations;
	std::vector<Card> possibleLocationsCards;
	std::string coordinates;
	Board newBoard;
	switch (player) {
	case true:		//Use the AI pair of cards (the first set)
		//Store all locations of a particular piece
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (GameBoard.board[i][j].piece.isBlue == 1)
					currentLocations.push_back(std::pair<int, int>(i, j));
			}
		}
		//Populate possible moves
		for (int i = 0; size_t(i) < currentLocations.size(); i++) {
			for (int j = 0; size_t(j) < cardPair.first.first.coordinatesVector.size(); j++) {
				//Add the move if it doesn't go out of bounds
				int newY = currentLocations[i].first + (-1) * cardPair.first.first.coordinatesVector[j].second;
				int newX = currentLocations[i].second + cardPair.first.first.coordinatesVector[j].first;
				if (newY <= 4 && newY >= 0 && newX <= 4 && newX >= 0 && GameBoard.board[newY][newX].piece.isBlue != 1) {
					std::pair<int, int> currentLocationPair = std::pair<int, int>(currentLocations[i].first, currentLocations[i].second);
					std::pair<int, int> newLocationPair = std::pair<int, int>(newY, newX);
					possibleLocations.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(currentLocationPair, newLocationPair));
					possibleLocationsCards.push_back(cardPair.first.first);
				}
			}
			for (int j = 0; size_t(j) < cardPair.first.second.coordinatesVector.size(); j++) {
				//Add the move if it doesn't go out of bounds
				int newY = currentLocations[i].first + (-1) * cardPair.first.second.coordinatesVector[j].second;
				int newX = currentLocations[i].second + cardPair.first.second.coordinatesVector[j].first;
				if (newY <= 4 && newY >= 0 && newX <= 4 && newX >= 0 && GameBoard.board[newY][newX].piece.isBlue != 1) {
					std::pair<int, int> currentLocationPair = std::pair<int, int>(currentLocations[i].first, currentLocations[i].second);
					std::pair<int, int> newLocationPair = std::pair<int, int>(newY, newX);
					possibleLocations.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(currentLocationPair, newLocationPair));
					possibleLocationsCards.push_back(cardPair.first.second);
				}
			}
		}
		//Now perform each move in the possibleLocations vector and add it to the vector newGameBoards
		newBoard.board = board;
		for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
			coordinates = convertToCoordinates(possibleLocations[i]);
			newBoard.movePiece(coordinates);
			newGameBoards.second.push_back(newBoard.board);
			newGameBoards.first.first.push_back(possibleLocationsCards[i]);
			newGameBoards.first.second.push_back(possibleLocations[i]);
			newBoard.board = board;
		}
		break;
	case false:	//Use the player pair of cards (the second set)
				//Store all locations of a particular piece
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (GameBoard.board[i][j].piece.isBlue == 0)
					currentLocations.push_back(std::pair<int, int>(i, j));
			}
		}
		//Populate possible moves
		for (int i = 0; size_t(i) < currentLocations.size(); i++) {
			for (int j = 0; size_t(j) < cardPair.second.first.coordinatesVector.size(); j++) {
				//Add the move if it doesn't go out of bounds
				int newY = currentLocations[i].first + cardPair.second.first.coordinatesVector[j].second;
				int newX = currentLocations[i].second + cardPair.second.first.coordinatesVector[j].first;
				if (newY <= 4 && newY >= 0 && newX <= 4 && newX >= 0 && GameBoard.board[newY][newX].piece.isBlue != 0) {
					std::pair<int, int> currentLocationPair = std::pair<int, int>(currentLocations[i].first, currentLocations[i].second);
					std::pair<int, int> newLocationPair = std::pair<int, int>(newY, newX);
					possibleLocations.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(currentLocationPair, newLocationPair));
					possibleLocationsCards.push_back(cardPair.second.first);
				}
			}
			for (int j = 0; size_t(j) < cardPair.second.second.coordinatesVector.size(); j++) {
				//Add the move if it doesn't go out of bounds
				int newY = currentLocations[i].first + cardPair.second.second.coordinatesVector[j].second;
				int newX = currentLocations[i].second + cardPair.second.second.coordinatesVector[j].first;
				if (newY <= 4 && newY >= 0 && newX <= 4 && newX >= 0 && GameBoard.board[newY][newX].piece.isBlue != 0) {
					std::pair<int, int> currentLocationPair = std::pair<int, int>(currentLocations[i].first, currentLocations[i].second);
					std::pair<int, int> newLocationPair = std::pair<int, int>(newY, newX);
					possibleLocations.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>(currentLocationPair, newLocationPair));
					possibleLocationsCards.push_back(cardPair.second.second);
				}
			}
		}
		//Now perform each move in the possibleLocations vector and add it to the vector newGameBoards
		newBoard.board = board;
		for (int i = 0; size_t(i) < possibleLocations.size(); i++) {
			coordinates = convertToCoordinates(possibleLocations[i]);
			newBoard.movePiece(coordinates);
			newGameBoards.second.push_back(newBoard.board);
			newGameBoards.first.first.push_back(possibleLocationsCards[i]);
			newGameBoards.first.second.push_back(possibleLocations[i]);
			newBoard.board = board;
		}
		break;
	}
	return newGameBoards;
}

void Game::swapCards(std::pair<std::pair<Card, Card>, std::pair<Card, Card>> &newCardPair, Card cardToSwap, Card &middleCard)
{
	Card temp;
	//Find cardToSwap in newCardPair
	if (newCardPair.first.first.name == cardToSwap.name) {
		temp = newCardPair.first.first;
		newCardPair.first.first = middleCard;
	}
	else if (newCardPair.first.second.name == cardToSwap.name) {
		temp = newCardPair.first.second;
		newCardPair.first.second = middleCard;
	}
	else if (newCardPair.second.first.name == cardToSwap.name) {
		temp = newCardPair.second.first;
		newCardPair.second.first = middleCard;
	}
	else if (newCardPair.second.second.name == cardToSwap.name) {
		temp = newCardPair.second.second;
		newCardPair.second.second = middleCard;
	}
	//Swap it with the middle card
	middleCard = temp;
}