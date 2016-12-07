#include "Player.h"

Player::Player(Card firstCard, Card secondCard)
{
	hand.first = firstCard;
	hand.second = secondCard;
}

Player::Player()
{
	hand.first = Card();
	hand.second = Card();
}

Card Player::promptCard()
{
	int choice;
	tryAgain:
	std::cout << "Which card did you play?" << std::endl;
	std::cout << "1) " << hand.first.name << " ";
	for (int i = 0; size_t(i) < hand.first.coordinatesVector.size(); i++) {
		std::cout << "(" << hand.first.coordinatesVector[i].first << ", " << hand.first.coordinatesVector[i].second  << ") ";
	}
	std::cout << std::endl << "2) " << hand.second.name << " ";
	for (int i = 0; size_t(i) < hand.second.coordinatesVector.size(); i++) {
		std::cout << "(" << hand.second.coordinatesVector[i].first << ", " << hand.second.coordinatesVector[i].second << ") ";
	}
	std::cout << std::endl;
	std::cin >> choice;
	if (choice > 2 || choice < 1) {
		system("cls");
		std::cout << "Invalid input!" << std::endl;
		goto tryAgain;
	}
	if (choice == 1)
		return hand.first;
	else
		return hand.second;
}
