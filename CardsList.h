#pragma once
#include "Card.h"

class CardsList {
public:
	std::vector<Card> cardsList;
	CardsList();

	//Prints out the list and removes one
	Card chooseFromList();
};