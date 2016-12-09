#include <iostream>
#include "AI.h"
#include "Board.h"
#include "Game.h"
#include "CardsList.h"
#include <fstream>

void printHeader() {
	system("cls");
	std::cout << "=======================" << std::endl;
	std::cout << "  O  N  I  T  A  M  A  " << std::endl;
	std::cout << "=======================" << std::endl;
	std::cout << std::endl;
}

int aiWins = 0, opponentWins = 0, draws = 0, moveCount = 0;
std::vector<int> moves;
Game gameCopy;

int main() {
	int choice;
	std::ofstream file;
	file.open("output.txt");
	CardsList cardsList = CardsList();
	//Board board;
	printHeader();
	system("Pause");
	printHeader(); 
	std::cout << "What mode do you wish to play?" << std::endl;
	std::cout << "1) AI vs AI Levels 1-2 (Analysis)" << std::endl;
	std::cout << "2) AI vs AI Level 3 (Analysis)" << std::endl;
	std::cout << "3) AI vs Player" << std::endl;
	std::cin >> choice;
	Game game;
	switch (choice) {
	case 1:
		goto aiTrial;
		break;
	case 2:
		goto minimaxTrial;
		break;
	case 3:
		break;
	}
	printHeader();
ownGame:
	char ownGame;
	std::cout << "Do you own the game? (y/n): ";
	std::cin >> ownGame;
	if (ownGame == 'y' || ownGame == 'Y') {
		//Initialize the AI and player
		for (int i = 0; i < 5; i++) {
			printHeader();
			if (i < 2)
				std::cout << "What cards does the AI have?" << std::endl;
			else if (i < 4)
				std::cout << "What cards do you have?" << std::endl;
			else
				std::cout << "What is the middle card?" << std::endl;
			Card cardToUse = cardsList.chooseFromList();
			switch (i) {
			case 0:
				game.player2.hand.first = cardToUse;
				break;
			case 1:
				game.player2.hand.second = cardToUse;
				break;
			case 2:
				game.player1.hand.first = cardToUse;
				break;
			case 3:
				game.player1.hand.second = cardToUse;
				break;
			case 4:
				game.GameBoard.middleCard = cardToUse;
				break;
			}
		}
	}
	else if (ownGame == 'n' || ownGame == 'N') {
		//Do random
		int number;
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> firstPlayerFirstRandomCard(0, cardsList.cardsList.size() - 1);
		number = firstPlayerFirstRandomCard(mt);
		game.player1.hand.first = cardsList.cardsList[number];
		cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

		std::uniform_int_distribution<int> firstPlayerSecondRandomCard(0, cardsList.cardsList.size() - 1);
		number = firstPlayerSecondRandomCard(mt);
		game.player1.hand.second = cardsList.cardsList[number];
		cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

		std::uniform_int_distribution<int> secondPlayerFirstRandomCard(0, cardsList.cardsList.size() - 1);
		number = secondPlayerFirstRandomCard(mt);
		game.player2.hand.first = cardsList.cardsList[number];
		cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

		std::uniform_int_distribution<int> secondPlayerSecondRandomCard(0, cardsList.cardsList.size() - 1);
		number = secondPlayerSecondRandomCard(mt);
		game.player2.hand.second = cardsList.cardsList[number];
		cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

		std::uniform_int_distribution<int> middleRandomCard(0, cardsList.cardsList.size() - 1);
		number = middleRandomCard(mt);
		game.GameBoard.middleCard = cardsList.cardsList[number];
		cardsList.cardsList.erase(cardsList.cardsList.begin() + number);
	}
	else {
		system("cls");
		printHeader();
		std::cout << "Invaid input! Try Again!" << std::endl;
		goto ownGame;
	}
	//Play against the computer
	tryAgain:
		printHeader();
		std::cout << "Select a difficulty setting:" << std::endl;
		std::cout << "1) Easy (Random)" << std::endl;
		std::cout << "2) Medium (Smart)" << std::endl;
		std::cout << "3) Hard (Minimax)" << std::endl;
		std::cin >> choice;
	switch (choice) {
		//Completely random
	case 1:
		//Random, but will take moves that take pieces
	case 2:
		//Uses Negamax algorithm to win every game
	case 3:
		if (choice == 3) {
			printHeader();
			std::cout << "Depth of the tree? ";
			std::cin >> game.player2.depth;
			printHeader();
			std::cout << "1) Sum of values" << std::endl << "2) Ratio of values" << std::endl << "Heuristic function to use? ";
			std::cin >> game.player2.heuristic;
		}
		game.player2.level = choice;
		gameCopy = game;
		game.startGame();
		return 0;
	default:
		std::cout << "Invalid selection!" << std::endl;
		goto tryAgain;
		break;
	}
aiTrial:
	file << "AIWins, OpponentWins, Level, Draws, Depth, Heuristic, AvgMoves, StdDev" << std::endl;
	int numRounds = 100;
	game.player2.level = 3;
	//For increasing depth from 1 - 4
	for (int k = 1; k <= 4; k++) {
		game.player2.depth = k;
		//Perform each set 5 times
		for(int l = 1; l <= 5; l++) {
			//For each difficulty level 1 - 2
			for (int i = 1; i <= 2; i++) {
				game.player1.level = i;
				//For each heuristic function 1 - 2
				for (int j = 1; j <= 2; j++) {
					game.player2.heuristic = j;
					for (int m = 0; m < numRounds; m++) {
						cardsList = CardsList();

						int number;
						std::random_device rd;
						std::mt19937 mt(rd());
						std::uniform_int_distribution<int> firstPlayerFirstRandomCard(0, cardsList.cardsList.size() - 1);
						number = firstPlayerFirstRandomCard(mt);
						game.player1.hand.first = cardsList.cardsList[number];
						cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

						std::uniform_int_distribution<int> firstPlayerSecondRandomCard(0, cardsList.cardsList.size() - 1);
						number = firstPlayerSecondRandomCard(mt);
						game.player1.hand.second = cardsList.cardsList[number];
						cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

						std::uniform_int_distribution<int> secondPlayerFirstRandomCard(0, cardsList.cardsList.size() - 1);
						number = secondPlayerFirstRandomCard(mt);
						game.player2.hand.first = cardsList.cardsList[number];
						cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

						std::uniform_int_distribution<int> secondPlayerSecondRandomCard(0, cardsList.cardsList.size() - 1);
						number = secondPlayerSecondRandomCard(mt);
						game.player2.hand.second = cardsList.cardsList[number];
						cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

						std::uniform_int_distribution<int> middleRandomCard(0, cardsList.cardsList.size() - 1);
						number = middleRandomCard(mt);
						game.GameBoard.middleCard = cardsList.cardsList[number];
						cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

						gameCopy = game;

						game.startGame();
						if (game.aiWins > 0)
							aiWins++;
						else if (game.opponentWins > 0)
							opponentWins++;
						else
							draws++;
						moves.push_back(game.moves);
						moveCount++;
						game = gameCopy;
					}
					//Calculate the mean and standard deviation of moves
					int sumOfMoves = 0;
					double average, stdDev = 0.0;
					for (int l = 0; size_t(l) < moves.size(); l++) {
						sumOfMoves += moves[l];
					}
					average = (double)sumOfMoves / (double)moveCount;
					for (int l = 0; size_t(l) < moves.size(); l++) {
						stdDev += pow(moves[l] - average, 2);
					}
					stdDev = stdDev / 100;
					file << aiWins << "," << opponentWins << "," << i << "," << draws << "," << k << "," << j << "," << average << "," << stdDev << std::endl;
					aiWins = 0;
					opponentWins = 0;
					draws = 0;
					moveCount = 0;
					moves.clear();
				}
			}
		}
	}
	file.close();
	system("Pause");
	return 0;
minimaxTrial:
	file << "AIWins, OpponentWins, Draws, AI(2)Depth, AI(2)Heuristic, AI(1)Depth, AI(1)Heuristic, AverageMoves, StdDev" << std::endl;
	numRounds = 100;
	game.player2.level = 3;
	game.player1.level = 3;
	//For increasing depth from 1 - 4
	for (int k = 1; k <= 4; k++) {
		game.player2.depth = k;
		//For increasing depth from 1 - 4
		for (int i = 1; i <= 4; i++) {
			game.player1.depth = i;
			//For each heuristic function 1 -2 
			for (int j = 1; j <= 2; j++) {
				game.player1.heuristic = j;
				for (int n = 1; n <= 2; n++) {
					game.player2.heuristic = n;
					for (int m = 0; m < numRounds; m++) {
						cardsList = CardsList();

						int number;
						std::random_device rd;
						std::mt19937 mt(rd());
						std::uniform_int_distribution<int> firstPlayerFirstRandomCard(0, cardsList.cardsList.size() - 1);
						number = firstPlayerFirstRandomCard(mt);
						game.player1.hand.first = cardsList.cardsList[number];
						cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

						std::uniform_int_distribution<int> firstPlayerSecondRandomCard(0, cardsList.cardsList.size() - 1);
						number = firstPlayerSecondRandomCard(mt);
						game.player1.hand.second = cardsList.cardsList[number];
						cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

						std::uniform_int_distribution<int> secondPlayerFirstRandomCard(0, cardsList.cardsList.size() - 1);
						number = secondPlayerFirstRandomCard(mt);
						game.player2.hand.first = cardsList.cardsList[number];
						cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

						std::uniform_int_distribution<int> secondPlayerSecondRandomCard(0, cardsList.cardsList.size() - 1);
						number = secondPlayerSecondRandomCard(mt);
						game.player2.hand.second = cardsList.cardsList[number];
						cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

						std::uniform_int_distribution<int> middleRandomCard(0, cardsList.cardsList.size() - 1);
						number = middleRandomCard(mt);
						game.GameBoard.middleCard = cardsList.cardsList[number];
						cardsList.cardsList.erase(cardsList.cardsList.begin() + number);

						gameCopy = game;

						game.startGame();
						if (game.aiWins > 0)
							aiWins++;
						else if (game.opponentWins > 0)
							opponentWins++;
						else
							draws++;
						moves.push_back(game.moves);
						moveCount++;
						game = gameCopy;
					}
					//Calculate the mean and standard deviation of moves
					int sumOfMoves = 0;
					double average, stdDev = 0.0;
					for (int l = 0; size_t(l) < moves.size(); l++) {
						sumOfMoves += moves[l];
					}
					average = (double)sumOfMoves / (double)moveCount;
					for (int l = 0; size_t(l) < moves.size(); l++) {
						stdDev += pow(moves[l] - average, 2);
					}
					stdDev = stdDev / 100;
					file << aiWins << "," << opponentWins << "," << draws << "," << k << "," << n << "," << i << "," << j << "," << average << "," << stdDev << std::endl;
					aiWins = 0;
					opponentWins = 0;
					draws = 0;
					moveCount = 0;
					moves.clear();
				}
			}
		}
	}
	file.close();
	system("Pause");
	return 0;
}