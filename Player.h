#pragma once
#include <iostream>
#include "Card.h"

class Player {
public:
	std::pair<Card, Card> hand;
	Player(Card, Card);
	Player();
	
	//The card to be played by the player
	virtual Card promptCard();
};