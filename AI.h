#pragma once
#include "Player.h"

class AI : public Player{
public:
	//Difficulty of the AI
	int level;
	
	//Depth of the negamax tree
	int depth;

	//Heuristic function to use
	int heuristic;
};