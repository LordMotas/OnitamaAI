#pragma once
#include <vector>
#include "BoardSpace.h"
#include "Board.h"
#include "Player.h"
#include "AI.h"
#include <cctype>
#include <sstream>
#include <climits>
#include <random>
#include <algorithm>

class Game {
public:
	//Score for games
	int aiWins = 0;
	int opponentWins = 0;
	int draws = 0;
	int moves = 0;

	//Two players
	AI player1;
	AI player2;

	//Game board (5X5 board)
	Board GameBoard;

	//Constructor
	Game();

	//Starts the game
	void startGame();

	//Printing methods
	void static printHeader();
	void static printPlayerCards(Player);
	void printAll(bool);

	//Determines the first player
	bool determineFirstPlayer();

	//Returns a list of player possible moves
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> generatePlayerMoves();

	//AI Methods
	std::pair<std::string, Card> AIlogic(std::pair<Card, Card>, std::pair<Card, Card>, int);
	int trimWinMoves(std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &, std::vector<Card> &, int);

	//Handles the conversion from pairs to coordinates
	std::string convertToCoordinates(std::pair<std::pair<int, int>, std::pair<int, int>>);

	//Heuristic functions
	int heuristicSum(std::vector<std::vector<BoardSpace>>);
	double heuristicRatio(std::vector<std::vector<BoardSpace>>);
	bool isTerminal(std::vector<std::vector<BoardSpace>>);

	//Minimax search methods
	std::pair<std::pair<std::pair<int, int>, std::pair<int, int>>, double> minimax(std::vector<std::vector<BoardSpace>>, std::pair<std::pair<int, int>, std::pair<int, int>>, int, double, double, std::pair<std::pair<Card, Card>, std::pair<Card, Card>>, Card, bool);
	std::pair<std::pair<std::vector<Card>, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>, std::vector<std::vector<std::vector<BoardSpace>>>> generateBoards(std::vector<std::vector<BoardSpace>>, std::pair<std::pair<Card, Card>, std::pair<Card, Card>>, bool);
	void swapCards(std::pair<std::pair<Card, Card>, std::pair<Card, Card>> &, Card, Card &);
};