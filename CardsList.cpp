#include "CardsList.h"
#include <iostream>

CardsList::CardsList()
{
	cardsList.push_back(Card("Tiger", "Blue", { std::pair<int,int>(0, -1), std::pair<int,int>(0, 2) }));
	cardsList.push_back(Card("Crab", "Blue", { std::pair<int,int>(0, 1), std::pair<int,int>(-2, 0), std::pair<int,int>(2, 0) }));
	cardsList.push_back(Card("Monkey", "Blue", { std::pair<int,int>(-1, -1), std::pair<int,int>(-1, 1), std::pair<int,int>(1, -1), std::pair<int,int>(1, 1) }));
	cardsList.push_back(Card("Crane", "Blue", { std::pair<int,int>(-1, -1), std::pair<int,int>(1, -1), std::pair<int,int>(0, 1) }));
	cardsList.push_back(Card("Dragon", "Red", { std::pair<int,int>(-1, -1), std::pair<int,int>(1, -1), std::pair<int,int>(-2, 1), std::pair<int,int>(2, 1) }));
	cardsList.push_back(Card("Elephant", "Red", { std::pair<int,int>(-1, 0), std::pair<int,int>(1, 0), std::pair<int,int>(-1, 1), std::pair<int,int>(1, 1) }));
	cardsList.push_back(Card("Mantis", "Red", { std::pair<int,int>(-1, 1), std::pair<int,int>(1, 1), std::pair<int,int>(0, -1) }));
	cardsList.push_back(Card("Boar", "Red", { std::pair<int,int>(-1, 0), std::pair<int,int>(1, 0), std::pair<int,int>(0, 1) }));
	cardsList.push_back(Card("Frog", "Red", { std::pair<int,int>(-2, 0), std::pair<int,int>(-1, 1), std::pair<int,int>(1, -1) }));
	cardsList.push_back(Card("Goose", "Blue", { std::pair<int,int>(-1, 0), std::pair<int,int>(-1, 1), std::pair<int,int>(1, 0), std::pair<int,int>(1, -1) }));
	cardsList.push_back(Card("Horse", "Red", { std::pair<int,int>(-1, 0), std::pair<int,int>(0, -1), std::pair<int,int>(0, 1) }));
	cardsList.push_back(Card("Eel", "Blue", { std::pair<int,int>(-1, 1), std::pair<int,int>(-1, -1), std::pair<int,int>(1, 0) }));
	cardsList.push_back(Card("Rabbit", "Blue", { std::pair<int,int>(-1, -1), std::pair<int,int>(1, 1), std::pair<int,int>(2, 0) }));
	cardsList.push_back(Card("Rooster", "Red", { std::pair<int,int>(-1, 0), std::pair<int,int>(-1, -1), std::pair<int,int>(1, 0), std::pair<int,int>(1, 1) }));
	cardsList.push_back(Card("Ox", "Blue", { std::pair<int,int>(0, -1), std::pair<int,int>(0, 1), std::pair<int,int>(1, 0) }));
	cardsList.push_back(Card("Cobra", "Red", { std::pair<int,int>(-1, 0), std::pair<int,int>(1, 1), std::pair<int,int>(1, -1) }));
}

Card CardsList::chooseFromList()
{
	int choice;
	tryAgain:
	for (int i = 0; size_t(i) < cardsList.size(); i++) {
		std::cout << i + 1 << ") " << cardsList[i].name << std::endl;
	}
	std::cin >> choice;
	if (choice > int(cardsList.size())) {
		system("cls");
		std::cout << "Invalid input!" << std::endl;
		goto tryAgain;
	}
	Card returnCard = cardsList[choice-1];
	cardsList.erase(cardsList.begin() + choice - 1);
	return returnCard;
}
