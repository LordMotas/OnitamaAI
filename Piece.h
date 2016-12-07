#pragma once
#include <iostream>

class Piece {
public:
	bool isOnmyo;
	int pieceValue;
	char letter;
	std::pair<int, int> coordinates;
	int isBlue; //0 = blue, 1 = red, -1 = null
	Piece(int, int, char);
	Piece();
};