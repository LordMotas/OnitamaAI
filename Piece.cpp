#include "Piece.h"
#include <iostream>

Piece::Piece(int value, int color, char pieceLetter)
{
	pieceValue = value;
	if (value == 10 || value == -10)
		isOnmyo = true;
	else
		isOnmyo = false;
	isBlue = color;
	letter = pieceLetter;
}

Piece::Piece() {
	letter = ' ';
	isBlue = -1;
	isOnmyo = false;
	pieceValue = 0;
}
