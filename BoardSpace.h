#pragma once
#include "Piece.h"

class BoardSpace {
public:
	Piece piece;
	//0: Not any space
	//1: Onmyo space for computer
	//-1: Onmyo space for player
	int isOnmyoSpace;
	BoardSpace(Piece, int);
	BoardSpace(Piece);
};